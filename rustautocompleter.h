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

#ifndef RUSTAUTOCOMPLETER_H
#define RUSTAUTOCOMPLETER_H

#include <texteditor/autocompleter.h>
namespace RustEditor{
namespace Internal{

class RustAutoCompleter : public TextEditor::AutoCompleter
{
public:
    RustAutoCompleter() {}


    virtual bool contextAllowsAutoParentheses(const QTextCursor &cursor,
                                              const QString &textToInsert = QString()) const;
    virtual bool contextAllowsElectricCharacters(const QTextCursor &cursor) const;
    virtual bool isInComment(const QTextCursor &cursor) const;
    virtual bool isInString(const QTextCursor &cursor) const;
    virtual QString insertMatchingBrace(const QTextCursor &cursor,
                                        const QString &text,
                                        QChar la,
                                        int *skippedChars) const;
    virtual QString insertParagraphSeparator(const QTextCursor &cursor) const;

};


}
}
#endif // RUSTAUTOCOMPLETER_H
