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

#ifndef RUSTCOMPLETIONASSIST_H
#define RUSTCOMPLETIONASSIST_H

#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/iassistprocessor.h>
#include <texteditor/codeassist/assistinterface.h>


#include <QIcon>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QTextCursor>

namespace RustEditor {
namespace Internal {

class RustCompletionAssistInterface;

class RustCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT

public:
    bool supportsEditor(Core::Id editorId) const Q_DECL_OVERRIDE;
    TextEditor::IAssistProcessor *createProcessor() const Q_DECL_OVERRIDE;

    int activationCharSequenceLength() const Q_DECL_OVERRIDE;
    bool isActivationCharSequence(const QString &sequence) const Q_DECL_OVERRIDE;
};

class RustCompletionAssistProcessor : public TextEditor::IAssistProcessor
{
public:
    RustCompletionAssistProcessor();
    ~RustCompletionAssistProcessor();

    TextEditor::IAssistProposal *perform(const TextEditor::AssistInterface *interface) Q_DECL_OVERRIDE;

private:
    const QIcon &getRacerIcon(const QString &type);
    bool acceptsIdleEditor() const;

    int m_startPosition;
    QScopedPointer<const RustCompletionAssistInterface> m_interface;

    QIcon m_keywordIcon;
    QIcon m_varIcon;
    QIcon m_functionIcon;
    QIcon m_typeIcon;
    QIcon m_constIcon;
    QIcon m_attributeIcon;
    QIcon m_uniformIcon;
    QIcon m_varyingIcon;
    QIcon m_otherIcon;
};

class RustCompletionAssistInterface : public TextEditor::AssistInterface
{
public:
    RustCompletionAssistInterface(QTextDocument *textDocument,
                                  int position, const QString &fileName,
                                  TextEditor::AssistReason reason,
                                  const QString &mimeType);

    const QString &mimeType() const { return m_mimeType; }

private:
    QString m_mimeType;
};

} // namespace Internal
} // namespace RustEditor

#endif // RUSTCOMPLETIONASSIST_H
