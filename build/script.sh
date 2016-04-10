#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $CI == "true" && $BUILD_OS_NAME == 'linux' ]]; then
  . /opt/qt55/bin/qt55-env.sh
fi

echo "Creating build directory ${BUILD_DIR}"
mkdir -p "${BUILD_DIR}" && cd "${BUILD_DIR}"
mkdir -p "${PLUGIN_OUT_PATH}"

echo "Building..."

echo "Executing qmake..."
qmake "${PROJECT_DIR}"/rusteditor.pro -r ${ENV_QMAKE_PARAMS} CONFIG+=release

echo "Executing make..."
make

