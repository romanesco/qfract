#!/bin/sh

QTDIR=/Users/inou/Qt/6.4.2/macos
QTLIBDIR=$QTDIR/lib
QFRACT_BIN=qfract.app/Contents/MacOS/qfract
LIBDIR=qfract.app/Contents/Resources/lib
FRAMEWORKDIR=qfract.app/Contents/Frameworks/
PLUGINDIR=qfract.app/Contents/Resources/plugins/
BOOSTLIBDIR=/opt/homebrew/opt/boost/lib

mkdir -p $FRAMEWORKDIR
for i in Core Gui DBus Widget PrintSupport
do
cp -Rp /Users/inou/Qt/6.4.2/macos/lib/Qt$i $FRAMEWORKDIR
done

mkdir -p $LIBDIR
for i in system thread
do
cp -Rp /opt/homebrew/lib/libboost_$i-mt.dylib $LIBDIR
done

install_name_tool -delete_rpath $QTLIBDIR $QFRACT_BIN
install_name_tool -add_rpath "@executable_path/../Resources/lib" $QFRACT_BIN

install_name_tool -change $BOOSTLIBDIR/libboost_thread-mt.dylib "@rpath/libboost_thread-mt.dylib" $QFRACT_BIN
install_name_tool -change $BOOSTLIBDIR/libboost_system-mt.dylib "@rpath/libboost_system-mt.dylib" $QFRACT_BIN

install_name_tool -id "@rpath/libboost_system-mt.dylib" $LIBDIR/libboost_system-mt.dylib
install_name_tool -id "@rpath/libboost_thread-mt.dylib" $LIBDIR/libboost_thread-mt.dylib

codesign --force -s - $LIBDIR/libboost_system-mt.dylib
codesign --force -s - $LIBDIR/libboost_thread-mt.dylib

cp $QTDIR/plugins/platforms/libqcocoa.dylib $PLUGINDIR/platforms/
