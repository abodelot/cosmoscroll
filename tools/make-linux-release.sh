#!/bin/sh
# Stand-alone release builder for Linux

# Go to root directory of the git repository
ROOT_PATH=$(git rev-parse --show-cdup)
if [ $ROOT_PATH ]; then
	cd $ROOT_PATH
fi

# Get project name from directory
PROJECT_NAME=$(basename $(pwd))

# Extract version number from latest tag (v0.1 => 0.1)
VERSION=$(git describe --tags --abbrev=0 | cut -c2-)

# Get OS arch
ARCH=$(getconf LONG_BIT)

# Directory in which the release will be packaged
PACKAGE_NAME="$PROJECT_NAME"_"$VERSION"-linux_"$ARCH"bits
echo Packaging "$PACKAGE_NAME.tar.gz" ...

# Copy dependencies
mkdir bin/lib
cp /usr/local/lib/libsfml-graphics.so.2 bin/lib
cp /usr/local/lib/libsfml-window.so.2 bin/lib
cp /usr/local/lib/libsfml-system.so.2 bin/lib
cp /usr/local/lib/libsfml-audio.so.2 bin/lib
cp /usr/local/lib/libsfml-network.so.2 bin/lib
if [ $ARCH = 32 ]; then
	cp /usr/lib/i386-linux-gnu/libGLEW.so.1.7 bin/lib
	cp /usr/lib/i386-linux-gnu/libdumb.so.1 bin/lib
fi

# Copy launcher script
cp tools/run.sh bin

# Create the tarball from bin directory and give it a proper name
tar -czf $PACKAGE_NAME.tar.gz bin/ --transform s/bin/$PACKAGE_NAME/
echo Done!

# Clean up the mess created in the bin/ directory
rm bin/run.sh
rm -r bin/lib

