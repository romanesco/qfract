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
win32:QMAKE_CXXFLAGS_RELEASE    += -march=i686
#  Boost
win32:QMAKE_CXXFLAGS_RELEASE	+= -Ic:/Users/inyo/Downloads/boost_1_60_0 -ffast-math
win32:QMAKE_LFLAGS_RELEASE	+= -Lc:/Users/inyo/Downloads/boost_1_60_0/stage/lib
win32:LIBS	+= -lboost_thread-mgw49-mt-1_60 -lboost_system-mgw49-mt-1_60
#  dlfcn (from http://code.google.com/p/dlfcn-win32/)
win32:QMAKE_CXXFLAGS_RELEASE      += -Ic:/Users/inyo/Downloads/dlfcn-win32-master
win32:LIBS    += c:/Users/inyo/Downloads/dlfcn-win32-master/libdl.a -lpsapi
#win32:LIBS	+= -ldl

# for Mac (fink 32bit)
#QMAKE_CXXFLAGS_RELEASE		+= -arch i386 -I/sw/include -fast -ffast-math
#QMAKE_LFLAGS_RELEASE	+= -arch i386 -L/sw/lib

# for Mac 
# -stdlib=libc++ is required because boost libraries are
# linked to libc++.1.dylib and not libstdc++.6.dylib
macx:QMAKE_CXXFLAGS_RELEASE		+= -I/opt/homebrew/include -stdlib=libc++
macx:QMAKE_LFLAGS_RELEASE	+= -L/opt/homebrew/lib -stdlib=libc++
macx:QMAKE_CXXFLAGS_DEBUG		+= -I/opt/homebrew/include -stdlib=libc++
macx:QMAKE_LFLAGS_DEBUG	+= -L/opt/homebrew/lib -stdlib=libc++
#macx:QMAKE_CXXFLAGS_RELEASE		+= -I/usr/local/include -stdlib=libc++
#macx:QMAKE_LFLAGS_RELEASE	+= -L/usr/local/lib -stdlib=libc++
#macx:QMAKE_MACOSX_DEPLOYMENT_TARGET	= 10.11
#macx:QMAKE_MAC_SDK = macosx10.11

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
