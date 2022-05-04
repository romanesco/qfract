#!/bin/sh

#DEBUG=
DEBUG=echo

#QFRACT=build/Release/qfract.app
QFRACT=qfract.app

#QT_LIB_DIR=/Library/Frameworks
#QT_LIB_DIR=/Developer/SDKs/QtSDK/Desktop/Qt/473/gcc/lib
#QTDIR=/usr/local/opt/qt
QTDIR=/opt/homebrew/opt/qt

QT_LIB_DIR=$QTDIR/lib
#QT_PLUGIN_DIR=$QTDIR/plugins
QT_PLUGIN_DIR=$QTDIR/share/qt/plugins
PLUGIN_INSTALL_DIR=$QFRACT/Contents/Resources/plugins

# for boost_thread
BOOST_VERSION=1_53
#LIBBOOST_DIR=/usr/local/opt/boost/lib
LIBBOOST_DIR=/opt/homebrew/opt/boost/lib
#LIBBOOST_THREAD=/sw/lib/libboost_thread-mt-${BOOST_VERSION}.dylib

LIB_REFERENCE_DIR=@executable_path/../Resources/lib
LIB_INSTALL_DIR=$QFRACT/Contents/Resources/lib

for i in thread system
do
  ${DEBUG} mkdir -p $QFRACT/Contents/Resources/lib
  #DYLIB=libboost_${i}-mt-${BOOST_VERSION}.dylib
  DYLIB=libboost_${i}-mt.dylib
  DYLIB_FULL_PATH=$LIBBOOST_DIR/$DYLIB
  ${DEBUG} cp "${DYLIB_FULL_PATH}" ${LIB_INSTALL_DIR}
  ${DEBUG} chmod 644 ${LIB_INSTALL_DIR}/$DYLIB
  ${DEBUG} install_name_tool -change \
 	${DYLIB_FULL_PATH} \
 	${LIB_REFERENCE_DIR}/${DYLIB} \
 	$QFRACT/Contents/MacOS/qfract
  ${DEBUG} install_name_tool -id \
	${LIB_REFERENCE_DIR}/${DYLIB} \
	${LIB_INSTALL_DIR}/${DYLIB}
done


# for Qt Frameworks
FRAMEWORK_REFERENCE_DIR=@executable_path/../Frameworks
FRAMEWORK_INSTALL_DIR=$QFRACT/Contents/Frameworks
FRAMEWORK_ORIGINAL_DIR=$QT_LIB_DIR

for i in Core Gui PrintSupport Widgets DBus
do
  ${DEBUG} mkdir -p $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/Current
  ${DEBUG} cp $QT_LIB_DIR/Qt$i.framework/Versions/Current/Qt$i \
    $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/Current/
  ${DEBUG} install_name_tool -id \
    $FRAMEWORK_REFERENCE_DIR/Qt$i.framework/Versions/Current/Qt$i \
    $FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/Current/Qt$i
  ${DEBUG} install_name_tool -change $FRAMEWORK_ORIGINAL_DIR/Qt$i.framework/Versions/Current/Qt$i \
    $FRAMEWORK_REFERENCE_DIR/Qt$i.framework/Versions/Current/Qt$i \
    $QFRACT/Contents/MacOS/qfract
done

for i in Core Gui PrintSupport Widgets DBus
do
FILE=$FRAMEWORK_INSTALL_DIR/Qt$i.framework/Versions/Current/Qt$i
  for j in Core Gui Widgets
  do
     ORIGINAL=`otool -L $FILE | grep -v : | grep Qt$j | awk '{print $1}'`
     #${DEBUG} install_name_tool -change $QT_LIB_DIR/Qt$j.framework/Versions/Current/Qt$j \
     #${DEBUG} install_name_tool -change $FRAMEWORK_ORIGINAL_DIR/Qt$j.framework/Versions/Current/Qt$j \
     ${DEBUG} install_name_tool -change $ORIGINAL \
     $FRAMEWORK_REFERENCE_DIR/Qt$j.framework/Versions/Current/Qt$j \
     $FILE
  done
done

LIBQCOCOA_INSTALL_DIR=$PLUGIN_INSTALL_DIR/platforms
LIBQCOCOA=$LIBQCOCOA_INSTALL_DIR/libqcocoa.dylib
# libqcocoa.h
${DEBUG} mkdir -p $LIBQCOCOA_INSTALL_DIR
${DEBUG} cp $QT_PLUGIN_DIR/platforms/libqcocoa.dylib \
  $LIBQCOCOA_INSTALL_DIR
${DEBUG} install_name_tool -id \
  @executable_path/../Resources/plugins/libqcocoa.dylib \
  $LIBQCOCOA_INSTALL_DIR/libqcocoa.dylib
for j in Core Gui Widgets PrintSupport DBus
do
   ORIGINAL=`otool -L $LIBQCOCOA | grep -v : | grep Qt$j | awk '{print $1}'`
   #${DEBUG} install_name_tool -change @rpath/Qt$j.framework/Versions/Current/Qt$j \
   ${DEBUG} install_name_tool -change $ORIGINAL \
   $FRAMEWORK_REFERENCE_DIR/Qt$j.framework/Versions/Current/Qt$j \
   $LIBQCOCOA
done


#cp -R $QT_LIB_DIR/QtGui.framework/Versions/Current/Resources/qt_menu.nib \
#  $QFRACT/Contents/Resources/
