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

#ifndef RUSTEDITORSETTINGSWIDGET_H
#define RUSTEDITORSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class RustEditorSettingsWidget;
}
namespace RustEditor{
namespace Internal{

class RustEditorSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RustEditorSettingsWidget(QWidget *parent = 0);
    ~RustEditorSettingsWidget();

private:
    Ui::RustEditorSettingsWidget *ui;
};

}//Internal
}//RustEditor
#endif // RUSTEDITORSETTINGSWIDGET_H
