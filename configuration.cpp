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

#include "configuration.h"

#include <QSettings>
#include <QFileInfo>

#include <utils/environment.h>
#include <utils/fileutils.h>
#include <coreplugin/icore.h>

namespace {
    const QLatin1String SETTINGS_GROUP("RustEditorConfiguration");

    const QLatin1String RUST_SRC_ENV("RUST_SRC_PATH");
    const QLatin1String RUST_SRC_DEF("/usr/src/rust");

    const QLatin1String RACER_PATH_DEF("/usr/bin");
}

using namespace RustEditor::Internal;

Configuration *Configuration::cfg_instance = NULL;

Configuration::Configuration(QObject *parent):
    QObject(parent)
{
    load();
    cfg_instance = this;
}

Configuration *Configuration::getInstancePtr()
{
    Q_ASSERT(cfg_instance != NULL);
    return cfg_instance;
}

const Settings &Configuration::getSettingsPtr()
{
    return cfg_instance->rusteditorSettings;
}

void Configuration::setSettings(const Settings &newSettings)
{
    cfg_instance->rusteditorSettings = newSettings;
    cfg_instance->save();
}

void Configuration::load()
{
    bool saveSettings = false;

    QSettings *qtCreatorSettings = Core::ICore::settings();
    qtCreatorSettings->beginGroup(SETTINGS_GROUP);
    rusteditorSettings.load(*qtCreatorSettings);

    Utils::FileName rustSrcPath = cfgPathLookup(rusteditorSettings.rustSrcPath(), Utils::FileName::fromString(RUST_SRC_DEF), RUST_SRC_ENV, saveSettings);
    rusteditorSettings.setRustSrcPath(rustSrcPath);

    Utils::FileName racerPath = cfgPathLookup(rusteditorSettings.racerPath(), Utils::FileName::fromString(RACER_PATH_DEF), QLatin1String(""), saveSettings);
    rusteditorSettings.setRacerPath(racerPath);

    qtCreatorSettings->endGroup();

    if (saveSettings)
        save();
}

void Configuration::save()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SETTINGS_GROUP);
    rusteditorSettings.save(*settings);
    settings->endGroup();
}

Utils::FileName Configuration::cfgPathLookup(const Utils::FileName &cfg, const Utils::FileName &def, const QString &envKey, bool &save)
{
    Utils::FileName result;
    if (cfg.isEmpty()) {
        Utils::Environment env = Utils::Environment::systemEnvironment();
        if (!envKey.isEmpty()){
            Utils::FileName location = env.searchInPath(envKey);
            QFileInfo fi = location.toFileInfo();

            if (fi.exists() && fi.isDir()) result = location;
            else result = def;

        }else{

            result = def;
        }
    }else{
        result = cfg;
    }
    save = save || (result != cfg);
    return result;
}

