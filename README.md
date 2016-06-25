[![Build Status](https://travis-ci.org/dghilardi/RustEditor.svg?branch=master)](https://travis-ci.org/dghilardi/RustEditor)
[![Build status](https://ci.appveyor.com/api/projects/status/92d4nk5kd8n4k0re?svg=true)](https://ci.appveyor.com/project/dghilardi/rusteditor)

RustEditor
==========

Attempt to create a plugin for qtcreator letting it became a rust IDE

RustEditor includes code from qtcreator, read the file LICENSE-3RD-PARTY.txt to gain information about licenses.

## Develop

 * Download qtcreator source and compile it
 * Set environment variable QTC_SOURCE to the path where qtcreator source is located
 * Set environment variable QTC_BUILD to the build directory of qtcreator
 * Open the project and click Run
 * Once the project has build and the IDE asks the executable, browse and find the previously built qtcreator executable
