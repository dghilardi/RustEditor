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

#include "rustautocompleter.h"

bool RustEditor::Internal::RustAutoCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor, const QString &textToInsert) const
{
    Q_UNUSED(cursor)
    Q_UNUSED(textToInsert)

    return true;
}

bool RustEditor::Internal::RustAutoCompleter::contextAllowsElectricCharacters(const QTextCursor &cursor) const
{
    Q_UNUSED(cursor)

    return true;
}

bool RustEditor::Internal::RustAutoCompleter::isInComment(const QTextCursor &cursor) const
{
    Q_UNUSED(cursor)

    return false;
}

bool RustEditor::Internal::RustAutoCompleter::isInString(const QTextCursor &cursor) const
{
    Q_UNUSED(cursor)

    return false;
}

QString RustEditor::Internal::RustAutoCompleter::insertMatchingBrace(const QTextCursor &cursor, const QString &text, QChar la, int *skippedChars) const
{
    Q_UNUSED(cursor)
    Q_UNUSED(text)
    Q_UNUSED(la)
    Q_UNUSED(skippedChars)

    return QString();
}

QString RustEditor::Internal::RustAutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
    Q_UNUSED(cursor)

    return QString();
}
