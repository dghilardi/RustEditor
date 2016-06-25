[![Build Status](https://travis-ci.org/dghilardi/RustEditor.svg?branch=master)](https://travis-ci.org/dghilardi/RustEditor)
[![Build status](https://ci.appveyor.com/api/projects/status/92d4nk5kd8n4k0re?svg=true)](https://ci.appveyor.com/project/dghilardi/rusteditor)

RustEditor
==========

Attempt to create a plugin for qtcreator letting it became a rust IDE

RustEditor includes code from qtcreator, read the file LICENSE-3RD-PARTY.txt to gain information about licenses.

## Compile

If you want to compile qtcreator and rusteditor from source you should do something like this:

```sh
mkdir qtcreator_plg_dev && cd $_

#Download and compile qt-creator
git clone https://github.com/qtproject/qt-creator.git qtcreator-git
mkdir qtcreator-bld && cd $_
qmake -r CONFIG+=release ../qtcreator-git/qtcreator.pro
make -j4

cd ..

#Download and compile RustEditor
git clone https://github.com/dghilardi/RustEditor.git rusteditor-git
mkdir rusteditor-bld && cd $_
qmake -r CONFIG+=release ../rusteditor-git/rusteditor.pro "QTC_SOURCE=$(pwd)/../qtcreator-git" "QTC_BUILD=$(pwd)/../qtcreator-bld" "QTC_MAJOR=4" "QTC_MINOR=0"
make -j4

cd ..

#Run qtcreator
./qtcreator-bld/bin/qtcreator
```


## Develop

 * Download qtcreator source and compile it (see [Compile](##Compile) subsection)
 * Set environment variable QTC_SOURCE to the path where qtcreator source is located
 * Set environment variable QTC_BUILD to the build directory of qtcreator
 * Open the project and click Run
 * Once the project has build and the IDE asks the executable, browse and find the previously built qtcreator executable
