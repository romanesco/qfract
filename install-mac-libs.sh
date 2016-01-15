#!/bin/sh

#QFRACT=build/Release/qfract.app
QFRACT=qfract.app

#QT_LIB_DIR=/Library/Frameworks
#QT_LIB_DIR=/Developer/SDKs/QtSDK/Desktop/Qt/473/gcc/lib
QTDIR=/usr/local/Qt/5.3/clang_64

QT_LIB_DIR=$QTDIR/lib
QT_PLUGIN_DIR=$QTDIR/plugins
PLUGIN_INSTALL_DIR=$QFRACT/Contents/Resources/plugins

# for boost_thread
BOOST_VERSION=1_53
LIBBOOST_DIR=/sw/lib
#LIBBOOST_THREAD=/sw/lib/libboost_thread-mt-${BOOST_VERSION}.dylib

LIB_REFERENCE_DIR=@executable_path/../Resources/lib
LIB_INSTALL_DIR=$QFRACT/Contents/Resources/lib

for i in thread system
do
  mkdir -p $QFRACT/Contents/Resources/lib
  DYLIB=libboost_${i}-mt-${BOOST_VERSION}.dylib
  DYLIB_FULL_PATH=$LIBBOOST_DIR/$DYLIB
  cp "${DYLIB_FULL_PATH}" ${LIB_INSTALL_DIR}
  install_name_tool -change \
 	${DYLIB_FULL_PATH} \
 	${LIB_REFERENCE_DIR}/${DYLIB} \
 	$QFRACT/Contents/MacOS/qfract
  install_name_tool -id \
	${LIB_REFERENCE_DIR}/${DYLIB} \
	${LIB_INSTALL_DIR}/${DYLIB}
done


# for Qt Frameworks
FRAMEWORK_REFERENCE_DIR=@executable_path/../Frameworks
FRAMEWORK_INSTALL_DIR=$QFRACT/Contents/Frameworks

for i in Core Gui PrintSupport Widgets
do
  mkdir -p $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/5
  cp $QT_LIB_DIR/Qt$i.framework/Versions/5/Qt$i \
    $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/5/
  install_name_tool -id \
    $FRAMEWORK_REFERENCE_DIR/Qt$i.framework/Versions/5/Qt$i \
    $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/5/Qt$i
  install_name_tool -change $QT_LIB_DIR/Qt$i.framework/Versions/5/Qt$i \
    $FRAMEWORK_REFERENCE_DIR/Qt$i.framework/Versions/5/Qt$i \
    $QFRACT/Contents/MacOS/qfract
done

for i in Core Gui PrintSupport Widgets
do
  for j in Core Gui Widgets
  do
     install_name_tool -change $QT_LIB_DIR/Qt$j.framework/Versions/5/Qt$j \
     $FRAMEWORK_REFERENCE_DIR/Qt$j.framework/Versions/5/Qt$j \
     $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/5/Qt$i
  done
done

LIBQCOCOA_INSTALL_DIR=$PLUGIN_INSTALL_DIR/platforms
# libqcocoa.h
mkdir -p $LIBQCOCOA_INSTALL_DIR
cp $QT_PLUGIN_DIR/platforms/libqcocoa.dylib \
  $LIBQCOCOA_INSTALL_DIR
install_name_tool -id \
  @executable_path/../Resources/plugins/libqcocoa.dylib \
  $LIBQCOCOA_INSTALL_DIR/libqcocoa.dylib
for j in Core Gui Widgets PrintSupport
do
   install_name_tool -change $QT_LIB_DIR/Qt$j.framework/Versions/5/Qt$j \
   $FRAMEWORK_REFERENCE_DIR/Qt$j.framework/Versions/5/Qt$j \
   $LIBQCOCOA_INSTALL_DIR/libqcocoa.dylib
done


#cp -R $QT_LIB_DIR/QtGui.framework/Versions/5/Resources/qt_menu.nib \
#  $QFRACT/Contents/Resources/
