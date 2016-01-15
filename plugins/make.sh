#!/bin/sh

i=`basename $1 .cpp`

DEBUG=""
#DEBUG=echo

# yes for Windows
#WIN32=yes

# yes if you are building universal binary for Mac
#MAC_UNIVERSAL=yes

OS=`uname`

# Win32 (MinGW)
if [ "x"$WIN32 = "xyes" ]; then
    CXXFLAGS="-shared -Wall -W -pipe -O3 -ffast-math"
fi
# Linux (Debian)
if [ $OS = "Linux" ]; then
    CXXFLAGS="-shared -fPIC -Wall -W -pipe -O3 -ffast-math -I/usr/local/include"
fi

# Mac OS X
if [ $OS = "Darwin" ]; then
    if [ "x"$MAC_UNIVERSAL = xyes ]; then
	CXXFLAGS="-arch ppc -arch i386 -bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math"
    else
	ARCH=`uname -p`
        # Mac OS X (PPC)
	if [ $ARCH="powerpc" ]; then
	    CXXFLAGS="-bundle -flat_namespace -undefined suppress -pipe -O3 -mpowerpc-gfxopt -ffast-math"
	fi
        # Mac OS X (Intel)
	if [ $ARCH="i386" ]; then
	    #CXXFLAGS="-arch i386 -bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math"
	    CXXFLAGS="-bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math"
	fi
    fi
fi

#$DEBUG g++-fsf-4.9 $CXXFLAGS -DPLUGIN -o $i.so $i.cpp -lm
$DEBUG g++ $CXXFLAGS -DPLUGIN -o $i.so $i.cpp -lm

