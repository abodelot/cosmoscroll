#!/bin/sh
# --= release build script for Linux =--

RELEASE_DIR=../cosmoscroll-linux

# delete release dir if it already exists
if [ -d $RELEASE_DIR ]; then
	echo "cleaning previous release"
	rm -r $RELEASE_DIR
fi

# export the bin directory to the release directory
svn export ../bin $RELEASE_DIR

# copy the binary itself (not versionned in svn)
cp ../bin/cosmoscroll $RELEASE_DIR/

# copy libraries dependencies
mkdir $RELEASE_DIR/lib32
cp /usr/lib/libsfml*so* $RELEASE_DIR/lib32/
cp /usr/lib/libdumb*so* $RELEASE_DIR/lib32/

# copy launch script
cp run.sh $RELEASE_DIR/

echo "release packaged in $RELEASE_DIR"
