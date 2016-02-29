/*
 *  RustEditor: Plugin to add Rust language support to QtCreator IDE.
 *  Copyright (C) 2015  Davide Ghilardi
 *
 *  This file is part of RustEditor.
 *
 *  RustEditor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  RustEditor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with RustEditor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "rustcompletionassist.h"
#include "rusteditorconstants.h"
#include "rusteditorplugin.h"
#include "configuration.h"

#include <coreplugin/idocument.h>
#include <coreplugin/id.h>
#include <texteditor/completionsettings.h>
#include <texteditor/convenience.h>
#include <texteditor/codeassist/assistproposalitem.h>
#include <texteditor/codeassist/genericproposalmodel.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/functionhintproposal.h>

#include <utils/faketooltip.h>

#include <QIcon>
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QProcess>
#include <QTemporaryFile>
#include <QTextDocument>

using namespace TextEditor;

using namespace RustEditor::Internal;


enum RacerOutputPosMeaning{
    MATCH,
    LINENUM,
    CHARNUM,
    FILEPATH,
    TYPE,
    DESCR
};

enum CompletionOrder {
    SpecialMemberOrder = -5
};

static bool isActivationChar(const QChar &ch)
{
    return ch == QLatin1Char('(') || ch == QLatin1Char('.') || ch == QLatin1Char(',');
}

static bool isIdentifierChar(QChar ch)
{
    return ch.isLetterOrNumber() || ch == QLatin1Char('_');
}

static bool isDelimiter(QChar ch)
{
    switch (ch.unicode()) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ')':
    case '?':
    case '!':
    case ':':
    case ';':
    case ',':
    case '+':
    case '-':
    case '*':
    case '/':
        return true;

    default:
        return false;
    }
}

static bool checkStartOfIdentifier(const QString &word)
{
    if (! word.isEmpty()) {
        const QChar ch = word.at(0);
        if (ch.isLetter() || ch == QLatin1Char('_'))
            return true;
    }

    return false;
}

// ----------------------------
// RustCompletionAssistProvider
// ----------------------------
bool RustCompletionAssistProvider::supportsEditor(Core::Id editorId) const
{
    return editorId == Constants::RUSTEDITOR_ID;
}

IAssistProcessor *RustCompletionAssistProvider::createProcessor() const
{
    return new RustCompletionAssistProcessor;
}

int RustCompletionAssistProvider::activationCharSequenceLength() const
{
    return 1;
}

bool RustCompletionAssistProvider::isActivationCharSequence(const QString &sequence) const
{
    return isActivationChar(sequence.at(0));
}

// -----------------------------
// RustCompletionAssistProcessor
// -----------------------------
RustCompletionAssistProcessor::RustCompletionAssistProcessor()
    : m_startPosition(0)
    , m_keywordIcon(QLatin1String(":/rusteditor/images/keyword.png"))
    , m_varIcon(QLatin1String(":/rusteditor/images/var.png"))
    , m_functionIcon(QLatin1String(":/rusteditor/images/func.png"))
    , m_typeIcon(QLatin1String(":/rusteditor/images/type.png"))
    , m_constIcon(QLatin1String(":/rusteditor/images/const.png"))
    , m_attributeIcon(QLatin1String(":/rusteditor/images/attribute.png"))
    , m_uniformIcon(QLatin1String(":/rusteditor/images/uniform.png"))
    , m_varyingIcon(QLatin1String(":/rusteditor/images/varying.png"))
    , m_otherIcon(QLatin1String(":/rusteditor/images/other.png"))
{}

RustCompletionAssistProcessor::~RustCompletionAssistProcessor()
{}

static AssistProposalItem *createCompletionItem(const QString &text, const QIcon &icon, int order = 0)
{
    AssistProposalItem *item = new AssistProposalItem;
    item->setText(text);
    item->setIcon(icon);
    item->setOrder(order);
    return item;
}

/**
 * @brief RustCompletionAssistProcessor::getRacerIcon Match completion type with the relative icon
 * @param type String representing the type of the completion returned by racer
 * @return icon that matches the given type
 */
const QIcon &RustCompletionAssistProcessor::getRacerIcon(const QString &type){
    if(type == QLatin1String("Module")) return m_otherIcon;
    else if(type == QLatin1String("Struct")) return m_typeIcon;
    else if(type == QLatin1String("MatchArm")) return m_otherIcon; //??
    else if(type == QLatin1String("Function")) return m_functionIcon;
    else if(type == QLatin1String("Crate")) return m_otherIcon; //
    else if(type == QLatin1String("Let")) return m_varIcon;
    else if(type == QLatin1String("IfLet")) return m_varIcon; //??
    else if(type == QLatin1String("StructField")) return m_attributeIcon;
    else if(type == QLatin1String("Impl")) return m_typeIcon;
    else if(type == QLatin1String("Enum")) return m_typeIcon;
    else if(type == QLatin1String("EnumVariant")) return m_typeIcon;
    else if(type == QLatin1String("Type")) return m_typeIcon;
    else if(type == QLatin1String("FnArg")) return m_varIcon;
    else if(type == QLatin1String("Trait")) return m_typeIcon;
    else if(type == QLatin1String("Const")) return m_constIcon;
    else if(type == QLatin1String("Static")) return m_constIcon;
    else return m_otherIcon;
}

IAssistProposal *RustCompletionAssistProcessor::perform(const AssistInterface *interface)
{

    m_interface.reset(static_cast<const RustCompletionAssistInterface *>(interface));

    if (interface->reason() == IdleEditor && !acceptsIdleEditor())
        return 0;

    //Save the current document to a temporary file so I can call racer on it
    QFile tmpsrc(m_interface->fileName() + QLatin1String(".racer.rs"));
    if(!tmpsrc.open(QFile::ReadWrite)) return 0;
    const QTextDocument *doc = m_interface->textDocument();
    QString fullDoc = doc->toPlainText();
    QTextStream tmpstream(&tmpsrc);
    tmpstream << fullDoc;
    tmpstream.flush();


    int pos = m_interface->position() - 1;
    QChar ch = m_interface->characterAt(pos);
    while (ch.isLetterOrNumber() || ch == QLatin1Char('_'))
        ch = m_interface->characterAt(--pos);

    int charnum, linenum;
    TextEditor::Convenience::convertPosition(doc, m_interface->position(), &linenum, &charnum);

    QList<AssistProposalItemInterface *> m_completions; // all possible completions at given point

    const Settings &rustEditorSettings = Configuration::getSettingsPtr();

    QString rustPath = rustEditorSettings.rustSrcPath().toString();
    QString racerPath = rustEditorSettings.racerPath().toString();
    if(!racerPath.isEmpty() && !racerPath.endsWith(QLatin1Char('/'))) racerPath.append(QLatin1Char('/'));


    //set environment variable pointing to where rust source is located
    QStringList env;
    env << (QLatin1String("RUST_SRC_PATH=")+rustPath);

    //run 'racer complete <linenum> <charnum> <filename>' and retrieve the output
    QProcess process;
    process.setEnvironment(env);
    QStringList params;
    params << QLatin1String("complete") << QString::number(linenum) << QString::number(charnum) << tmpsrc.fileName();
    process.start(racerPath+QLatin1String("racer"), params);
    process.waitForFinished();
    QString result = QString::fromLatin1(process.readAllStandardOutput());

    QStringList lines = result.split(QLatin1String("\n"));
    foreach (QString l, lines) {
        if(l.left(5) == QString::fromLatin1("MATCH")){
            QStringList fields = l.split(QLatin1String(","));
            m_completions << createCompletionItem(fields[MATCH].mid(6),getRacerIcon(fields[TYPE]));
        }
    }

    m_startPosition = pos + 1;

    tmpsrc.remove();
    return new GenericProposal(m_startPosition, m_completions);
}

bool RustCompletionAssistProcessor::acceptsIdleEditor() const
{
    const int cursorPosition = m_interface->position();
    const QChar ch = m_interface->characterAt(cursorPosition - 1);

    const QChar characterUnderCursor = m_interface->characterAt(cursorPosition);

    if (isIdentifierChar(ch) && (characterUnderCursor.isSpace() ||
                                 characterUnderCursor.isNull() ||
                                 isDelimiter(characterUnderCursor))) {
        int pos = m_interface->position() - 1;
        for (; pos != -1; --pos) {
            if (! isIdentifierChar(m_interface->characterAt(pos)))
                break;
        }
        ++pos;

        const QString word = m_interface->textAt(pos, cursorPosition - pos);
        if (word.length() > 2 && checkStartOfIdentifier(word)) {
            for (int i = 0; i < word.length(); ++i) {
                if (! isIdentifierChar(word.at(i)))
                    return false;
            }
            return true;
        }
    }

    return isActivationChar(ch);
}

// -----------------------------
// RustCompletionAssistInterface
// -----------------------------
RustCompletionAssistInterface::RustCompletionAssistInterface(QTextDocument *textDocument,
                                                             int position,
                                                             const QString &fileName,
                                                             AssistReason reason,
                                                             const QString &mimeType)
    : AssistInterface(textDocument, position, fileName, reason)
    , m_mimeType(mimeType)
{
}
