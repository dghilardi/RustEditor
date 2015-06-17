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

#include "rusteditorfactory.h"
#include "rusteditorwidget.h"
#include "rustcompletionassist.h"

#include <QCoreApplication>

#include <coreplugin/fileiconprovider.h>

#include <texteditor/texteditor.h>
#include <texteditor/textdocument.h>
#include <texteditor/normalindenter.h>
#include <texteditor/texteditoractionhandler.h>

#include <utils/hostosinfo.h>
#include <utils/theme/theme.h>

#include "rusteditorconstants.h"

using namespace TextEditor;
using namespace RustEditor::Internal;

RustEditorFactory::RustEditorFactory()
{
    setId(Constants::RUSTEDITOR_ID);
    setDisplayName(qApp->translate("OpenWith::Editors", Constants::RUSTEDITOR_DISPLAY_NAME));
    addMimeType(Constants::RUST_SOURCE_MIMETYPE);

    setDocumentCreator([]() { return new TextDocument(Constants::RUSTEDITOR_ID);});
    setEditorWidgetCreator([]() { return new RustEditorWidget; });
    setIndenterCreator([](){return new TextEditor::NormalIndenter;});

    setCompletionAssistProvider(new RustCompletionAssistProvider);
    setCommentStyle(Utils::CommentDefinition::CppStyle);
    setCodeFoldingSupported(true);
    setMarksVisible(true);
    setParenthesesMatchingEnabled(true);
    setUseGenericHighlighter(true);

    setEditorActionHandlers(TextEditorActionHandler::Format
                            | TextEditorActionHandler::UnCommentSelection
                            | TextEditorActionHandler::UnCollapseAll);

    if (!Utils::HostOsInfo::isMacHost() && !Utils::HostOsInfo::isWindowsHost()) {
        Core::FileIconProvider::registerIconOverlayForMimeType(
                    QIcon(Utils::creatorTheme()->imageFile(Utils::Theme::IconOverlayCppSource, QLatin1String(":/rusteditor/images/qt_rs.png"))),
                    Constants::RUST_SOURCE_MIMETYPE);
    }
}
