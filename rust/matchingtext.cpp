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

#include "matchingtext.h"

#include <QTextDocument>
#include <QTextBlock>
#include <QDebug>

using namespace Rust;

enum { MAX_NUM_LINES = 20 };

static bool shouldOverrideChar(QChar ch)
{
    switch (ch.unicode()) {
    case ')': case ']': case ';': case '"': case '\'':
        return true;

    default:
        return false;
    }
}

bool MatchingText::shouldInsertMatchingText(const QTextCursor &tc)
{
    QTextDocument *doc = tc.document();
    return shouldInsertMatchingText(doc->characterAt(tc.selectionEnd()));
}

bool MatchingText::shouldInsertMatchingText(QChar lookAhead)
{
    switch (lookAhead.unicode()) {
    case '{': case '}':
    case ']': case ')':
    case ';': case ',':
        return true;

    default:
        if (lookAhead.isSpace())
            return true;

        return false;
    } // switch
}

QString MatchingText::insertMatchingBrace(const QTextCursor &cursor, const QString &textToProcess,
                                          QChar la, int *skippedChars)
{
    QTextCursor tc = cursor;
    QTextDocument *doc = tc.document();
    QString text = textToProcess;

    const QString blockText = tc.block().text().mid(tc.positionInBlock()); //text from the cursor to the end of the block
    const QString trimmedBlockText = blockText.trimmed();
    const int length = qMin(blockText.length(), textToProcess.length());

    const QChar previousChar = doc->characterAt(tc.selectionEnd() - 1);

    //if text begins with " or ' check if it is escaped (previous char == '\' not escaped itself)
    bool escape = false;
    if (! text.isEmpty() && (text.at(0) == QLatin1Char('"') ||
                             text.at(0) == QLatin1Char('\''))) {
        if (previousChar == QLatin1Char('\\')) {
            int escapeCount = 0;
            int index = tc.selectionEnd() - 1;
            do {
                ++escapeCount;
                --index;
            } while (doc->characterAt(index) == QLatin1Char('\\'));

            if ((escapeCount % 2) != 0)
                escape = true;
        }
    }

    if (!escape) {
        for (int i = 0; i < length; ++i) {
            const QChar ch1 = blockText.at(i);
            const QChar ch2 = textToProcess.at(i);

            if (ch1 != ch2)
                break;
            else if (! shouldOverrideChar(ch1))
                break;

            ++*skippedChars;
        }
    }

    if (*skippedChars != 0) {
        tc.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, *skippedChars);
        text = textToProcess.mid(*skippedChars);
    }

    if (text.isEmpty() || !shouldInsertMatchingText(la))
        return QString();

    // todo: handle stringliteral and charliteral (at begin of texttoprocess)

    QString result;

    foreach (const QChar &ch, text) {
        if      (ch == QLatin1Char('('))  result += QLatin1Char(')');
        else if (ch == QLatin1Char('['))  result += QLatin1Char(']');
        else if (ch == QLatin1Char('"'))  result += QLatin1Char('"');
        else if (ch == QLatin1Char('\'')) result += QLatin1Char('\'');
        // Handle '{' appearance within functinon call context
        else if (ch == QLatin1Char('{') && !trimmedBlockText.isEmpty() && trimmedBlockText.at(0) == QLatin1Char(')'))
            result += QLatin1Char('}');

    }

    return result;
}

QString MatchingText::insertParagraphSeparator(const QTextCursor &tc)
{
    Q_UNUSED(tc)
    //TODO
    return QString();
}


