#
# Set QTDIR, MAKE and QMAKE-BIN properly before running make.
# (You might need to edit config.h and plugins/Makefile also.)
#

# default
#QTDIR	= /usr
QTDIR	= /Users/inou/Qt/6.4.2/macos
MAKE	= make
QMAKE-BIN	= qmake
#QMAKE-BIN	= qmake-qt4

# for Mac (static)
#QTDIR	= /usr/local/TrollTech/Qt-4.5.2
#QTDIR	= /usr/local/TrollTech/Qt-4.8.0
#QTDIR	= /usr/local/Qt/5.2.0/clang_64
#QTDIR	= /usr/local/Qt/5.6/clang_64

# for HomeBrew
#QTDIR	= /usr/local/opt/qt
#QTDIR	= /opt/homebrew/opt/qt


# for MacPorts
#QTDIR	= /opt/local
#QMAKE-BIN	= qmake-mac
#QMAKE-BIN	= qmake-x11

# for fink
#QTDIR=/sw/lib/qt4-mac
#QTDIR=/sw/lib/qt4-x11

# for mac (not using clang)
#QMAKESPEC = macx-g++

# Win32 (MinGW)
#QTDIR	= C:\Qt\2009.03\qt
#QTDIR	= D:\qt\qt-win-opensource-src-4.5.2# for static
#QTDIR	= c:\QtSDK\Desktop\Qt\4.7.3\MinGW
#MAKE	= mingw32-make

#QTDIR	= /Developer/Tools/Qt
#QMAKE	= /Developer/Tools/Qt/qmake
#QTDIR	= /Developer/SDKs/QtSDK/Desktop/Qt/473/gcc

QMAKE	= $(QTDIR)/bin/$(QMAKE-BIN)

PATH	+=$(QTDIR)/bin

all: exec plugin

exec: Makefile.qmake
	$(MAKE) -f Makefile.qmake

Makefile.qmake: qfract.pro
	$(QMAKE) -o Makefile.qmake qfract.pro

plugin:
	cd plugins && $(MAKE) all

clean: Makefile.qmake
	$(MAKE) -f Makefile.qmake clean
	rm -f *.o *.core *~ Makefile.qmake

clean-plugins:
	cd plugins && $(MAKE) clean

ccmalloc: 
	$(MAKE) -f Makefile.qmake
	rm -f qfract
	ccmalloc g++  -o qfract \
	canvas.o colormap.o main.o pixmap.o plugin.o setparam.o window.o \
	moc_canvas.o moc_setparam.o moc_window.o \
	-L/usr/local/lib -Wl,-rpath,/usr/local/qt/lib \
	-L/usr/local/qt/lib -L/usr/X11R6/lib -lqt -lXext -lX11 -lm
	
# for Mac OS X application buindle
#QFRACT_APP=build/Release/qfract.app
QFRACT_APP=qfract.app
install-mac: plugin
	mkdir -p $(QFRACT_APP)/Contents/Resources/{plugins,colormaps}
	strip $(QFRACT_APP)/Contents/MacOS/qfract
	install plugins/*.so $(QFRACT_APP)/Contents/Resources/plugins/
	cp colormaps/*.map $(QFRACT_APP)/Contents/Resources/colormaps/
	cp qfract-ja_JP.qm $(QFRACT_APP)/Contents/Resources/
	#sh install-mac-libs.sh
	rsync -av --copy-links $(QFRACT_APP)/ qfract-mac.app/Contents/Resources/qfract.app/
dist-mac:
	zip -r qfract-`date "+%y%m%d"`-mac.zip qfract-mac.app README README.jp LICENSE
	
# for Windows
install-win:
	if not exist dist\plugins mkdir dist\plugins 
	if not exist dist\colormaps mkdir dist\colormaps
	copy release\qfract.exe dist
	strip dist\qfract.exe
	copy plugins\*.so dist\plugins
	strip dist\plugins\*.so
	copy colormaps\*.map dist\colormaps
	copy qfract-ja_JP.qm dist
	copy README dist
	copy README.jp dist
	copy LICENSE dist

# source tarball
dist-src:
	mkdir -p qfract-`date "+%y%m%d"`
	tar cvf - `cat FILES` | tar xf - -C qfract-`date "+%y%m%d"`
	tar zcvf qfract-`date "+%y%m%d"`-src.tar.gz qfract-`date "+%y%m%d"`/

