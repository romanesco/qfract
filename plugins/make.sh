#!/bin/sh

i=`basename $1 .cpp`

#DEBUG=""
#DEBUG=echo

# yes for Windows
#WIN32=yes

# yes if you are building universal binary for Mac
MAC_UNIVERSAL=yes

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
	CXXFLAGS="-arch arm64 -arch x86_64 -bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math"
    else
	CXXFLAGS="-bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math"
    fi
fi

#$DEBUG g++-fsf-4.9 $CXXFLAGS -DPLUGIN -o $i.so $i.cpp -lm
$DEBUG g++ $CXXFLAGS -DPLUGIN -o $i.so $i.cpp -lm

