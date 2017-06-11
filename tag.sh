#!/bin/bash

SOURCE_DIR=`pwd`

VERSION=`git describe --tags`
if [ -z "$VERSION" ]; then
    VERSION=` git rev-parse HEAD`
fi

sed -i "s/^\!define PRODUCT_VERSION.*/\!define PRODUCT_VERSION \"${VERSION}\"/g" ${SOURCE_DIR}/Install/Install.nsi

APPVERYOR_VERSION="version: '${VERSION}.{build}'"
sed -i "s/^version: '.*{build}'/${APPVERYOR_VERSION}/g" ${SOURCE_DIR}/appveyor.yml

#git tag -a v${VERSION} -m "Release v${VERSION}"
#git push origin :refs/tags/v${VERSION}
#git push origin v${VERSION}
