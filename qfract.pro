TEMPLATE	= app
TARGET		= qfract
CONFIG		+= qt warn_on release
#CONFIG		+= qt warn_on release debug
QT		+= widgets printsupport
QMAKE_CXXFLAGS_RELEASE		+= -O3
QMAKE_LFLAGS_RELEASE		+= -O3
CONFIG		+= link_prl

# icons
win32:RC_FILE	= qfract.rc
macx:ICON	= qfract.icns

# for Win
#  Boost
win32:QMAKE_CXXFLAGS_RELEASE	+= -Ic:/Boost/include/boost-1_48 -ffast-math
win32:QMAKE_LFLAGS_RELEASE	+= -Lc:/Boost/lib
win32:LIBS	+= -lboost_thread-mgw45-mt-1_48
#  dlfcn (from http://code.google.com/p/dlfcn-win32/)
win32:LIBS	+= -ldl

# for Mac (fink 32bit)
#QMAKE_CXXFLAGS_RELEASE		+= -arch i386 -I/sw/include -fast -ffast-math
#QMAKE_LFLAGS_RELEASE	+= -arch i386 -L/sw/lib

# for Mac 
# -stdlib=libc++ is required because boost libraries are
# linked to libc++.1.dylib and not libstdc++.6.dylib
QMAKE_CXXFLAGS_RELEASE		+= -I/sw/include -stdlib=libc++
QMAKE_LFLAGS_RELEASE	+= -L/sw/lib -stdlib=libc++
QMAKE_MACOSX_DEPLOYMENT_TARGET	= 10.7
QMAKE_MAC_SDK = macosx10.9

# for Boost (Mac, Linux)
LIBS	+= -lboost_thread-mt -lboost_system-mt

# for universal binary for Mac
#QMAKE_MAC_SDK	= /Developer/SDKs/MacOSX10.4u.sdk
#macx:CONFIG	+= x86 ppc

macx:LIBS	+= -framework CoreFoundation

HEADERS		= canvas.h \
		  config.h \
		  colormap.h \
		  double.h \
		  pixmap.h \
		  plugin.h \
		  plugindialog.h \
		  setparam.h \
		  window.h
SOURCES		= canvas.cpp \
		  colormap.cpp \
		  main.cpp \
		  pixmap.cpp \
		  plugin.cpp \
		  plugindialog.cpp \
		  pluginlist.c \
		  setparam.cpp \
		  window.cpp
TRANSLATIONS	= qfract-ja_JP.ts
