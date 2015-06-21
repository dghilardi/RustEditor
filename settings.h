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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <utils/fileutils.h>

class QSettings;

namespace RustEditor{
namespace Internal{

class Settings
{
private:
    Utils::FileName racer_path;
    Utils::FileName rust_src_path;
public:
    Settings();

    void load(const QSettings &settings);
    void save(QSettings &settings);

    Utils::FileName racerPath() const;
    void setRacerPath(const Utils::FileName &newPath);

    Utils::FileName rustSrcPath() const;
    void setRustSrcPath(const Utils::FileName &newPath);
};

} //Internal
} //RustEditor
#endif // SETTINGS_H
