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

#include "rustsettingspage.h"
#include "rusteditorconstants.h"
#include "rusteditorsettingswidget.h"
#include <QCoreApplication>
using namespace RustEditor::Internal;

RustSettingsPage::RustSettingsPage(QObject *parent):
    Core::IOptionsPage(parent)
{
    setId(Constants::RUSTEDITOR_SETTINGS_ID);
    setDisplayName(tr("Rust Configurations"));
    setCategory(Constants::RUSTEDITOR_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("Rust",Constants::RUSTEDITOR_SETTINGS_TR_CATEGORY));
    setCategoryIcon(QLatin1String(Constants::RUSTEDITOR_SETTINGS_CATEGORY_ICON));
}

QWidget *RustSettingsPage::widget()
{
    if(!settingsWidget)
        settingsWidget = new RustEditorSettingsWidget;
    return settingsWidget;
}

void RustSettingsPage::apply()
{
    settingsWidget->saveSettings();
}

void RustSettingsPage::finish()
{
    delete settingsWidget;
}
