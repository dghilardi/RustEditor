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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "settings.h"

#include <QObject>

namespace RustEditor{
namespace Internal{

class Configuration : public QObject
{
    Q_OBJECT
private:
    static Configuration *cfg_instance;
    Settings rusteditorSettings;
private:
    void load();
    void save();

    Utils::FileName cfgPathLookup(const Utils::FileName &cfg, const Utils::FileName &def, const QString &envKey, bool &save);
public:
    Configuration(QObject *parent);

    static Configuration *getInstancePtr();
    static const Settings &getSettingsPtr();
    static void setSettings(const Settings &newSettings);
};

} //Internal
} //RustEditor

#endif // CONFIGURATION_H
