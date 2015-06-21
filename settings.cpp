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

#include "settings.h"

#include <utils/fileutils.h>

#include <QSettings>

namespace {
    const QLatin1String RACER_LOCATION_KEY("RACER_PATH");
    const QLatin1String RUST_SRC_LOCATION_KEY("RUST_SRC_PATH");
}


using namespace RustEditor::Internal;

Settings::Settings()
{

}

void Settings::load(const QSettings &settings)
{
    racer_path = Utils::FileName::fromString(settings.value(RACER_LOCATION_KEY).toString());
    rust_src_path = Utils::FileName::fromString(settings.value(RUST_SRC_LOCATION_KEY).toString());

}

void Settings::save(QSettings &settings)
{
    settings.setValue(RACER_LOCATION_KEY, racer_path.toString());
    settings.setValue(RUST_SRC_LOCATION_KEY, rust_src_path.toString());
}

Utils::FileName Settings::racerPath() const
{
    return racer_path;
}

void Settings::setRacerPath(const Utils::FileName &newPath)
{
    racer_path = newPath;
}

Utils::FileName Settings::rustSrcPath() const
{
    return rust_src_path;
}

void Settings::setRustSrcPath(const Utils::FileName &newPath)
{
    rust_src_path = newPath;
}
