DEFINES += RUSTEDITOR_LIBRARY

# RustEditor files

SOURCES += rusteditorplugin.cpp \
    rusteditorfactory.cpp \
    rusteditorwidget.cpp  \
    rustautocompleter.cpp \
    rust/matchingtext.cpp \
    rustcompletionassist.cpp \
    rustsettingspage.cpp \
    rusteditorsettingswidget.cpp \
    configuration.cpp \
    settings.cpp

HEADERS += rusteditorplugin.h \
        rusteditor_global.h \
        rusteditorconstants.h \
    rusteditorfactory.h \
    rusteditorwidget.h  \
    rustautocompleter.h \
    rust/matchingtext.h \
    rustcompletionassist.h \
    rustsettingspage.h \
    rusteditorsettingswidget.h \
    configuration.h \
    settings.h

# Qt Creator linking

## Set QTC Version
QTC_MAJ = $$(QTC_MAJOR)
QTC_MIN = $$(QTC_MINOR)
isEmpty(QTC_MAJ) || isEmpty(QTC_MIN){
    QTC_MAJ = $$QTC_MAJOR
    QTC_MIN = $$QTC_MINOR
}
isEmpty(QTC_MAJ) || isEmpty(QTC_MIN){
    QTC_MAJ = 3
    QTC_MIN = 6
}

DEFINES += "QTC_VERSION_MAJOR=$${QTC_MAJ}"
DEFINES += "QTC_VERSION_MINOR=$${QTC_MIN}"
message("Target Qt Creator version: $${QTC_MAJ}.$${QTC_MIN}")


## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=$$QTC_SOURCE
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/Users/example/qtcreator-src
!exists($$QTCREATOR_SOURCES):\
    error("Set variable QTC_SOURCE to the QtCreator's sources path (current path is \"$$QTC_SOURCE\")")

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=$$QTC_BUILD
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/Users/example/qtcreator-build
!exists($$IDE_BUILD_TREE): \
    error("Set variable QTC_BUILD to the QtCreator's libraries path (current path is \"$$QTC_BUILD\")")

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = RustEditor

LIBS += -L$$IDE_PLUGIN_PATH/QtProject \
        -L$$IDE_BUILD_TREE \
        -L$$IDE_BUILD_TREE/plugins/QtProject \
        -L$$IDE_BUILD_TREE/plugins

QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    texteditor

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

RESOURCES += \
    rusteditor.qrc

FORMS += \
    rusteditorsettingswidget.ui

## define output path
!isEmpty($$(PLUGIN_OUT_PATH)) {
    DESTDIR = $$(PLUGIN_OUT_PATH)
    message("You set output path to $$DESTDIR")
}
!isEmpty(PLUGIN_OUT_PATH) {
    DESTDIR = $$PLUGIN_OUT_PATH
    message("You set output path to $$DESTDIR")
}
message("Plugin output path is $$DESTDIR")
