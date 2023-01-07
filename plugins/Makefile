#CXX	=	g++-fsf-4.8

# Windows
ifeq ($(OS),Windows_NT)
  WIN32	= yes
  CXXFLAGS	= -shared -Wall -W -pipe -O3 -ffast-math
else

# not Windows
  # build universal binary for Mac
  #MAC_UNIVERSAL	= yes

  UNAME	= $(shell uname)

  # Linux
  ifeq ($(UNAME),Linux)
    CXXFLAGS	= -shared -fPIC -Wall -W -pipe -O3 -ffast-math -I/usr/local/include
  endif

  # Mac OS X
  ifeq ($(UNAME),Darwin)
  #	for Mac OS X (Universal)
    ifeq ($(MAC_UNIVERSAL),yes)
      CXXFLAGS	= -arch ppc -arch i386 -bundle -flat_namespace -undefined suppress -pipe -fast -ffast-math
    else
      ARCH	= $(shell uname -p)
      #Mac OS X (PPC)
      ifeq ($(ARCH),powerpc)
        CXXFLAGS	= -bundle -flat_namespace -undefined suppress -pipe -fast -mpowerpc-gfxopt -ffast-math
      endif
      #	 for Mac OS X (Intel)
      ifeq ($(ARCH),i386)
        #CXXFLAGS	=  -arch i386 -bundle -flat_namespace -undefined suppress -pipe -fast -ffast-math
        CXXFLAGS	=  -bundle -flat_namespace -undefined suppress -pipe -fast -ffast-math
      endif # ARCH=i386
      #	 for Mac OS X (Arm)
      ifeq ($(ARCH),arm)
        #CXXFLAGS	=  -arch i386 -bundle -flat_namespace -undefined suppress -pipe -fast -ffast-math
        CXXFLAGS	=  -bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math
      endif # ARCH=i386
    endif # MAC_UNIVERSAL=yes
  endif # UNAME=Darwin

endif # Windows_NT

# for buinding Windows (MinGW) binary with WSL etc.(Note that Qt version of MinGW does not contain make)
#CXX = /mnt/c/Qt/Tools/mingw1120_64/bin/g++.exe
#CXXFLAGS	= -shared -Wall -W -pipe -O3 -ffast-math

HEADERS =	../plugin.h \
		../double.h
SOURCES =	\
		Q.cpp \
		Q2.cpp \
		Qfatou.cpp \
		Qren-domain.cpp \
		Qren-range.cpp \
		Qren.cpp \
		Qrenfatou.cpp \
		arithhedge-exp.cpp \
		arithhedge-skew-exp.cpp \
		arithhedge-skew.cpp \
		arithhedge.cpp \
		bicritper2.cpp \
		bicritper2julia.cpp \
		bicritper2ray.cpp \
		bicritper2rayjulia.cpp \
		biquad1.cpp \
		biquad2.cpp \
		biquadfix1.cpp \
		biquadfix1alt.cpp \
		biquadfix1alt2.cpp \
		biquadfix1alt3.cpp \
		biquadfix2.cpp \
		biquadfix2alt.cpp \
		biquadfix2alt2.cpp \
		biquadfixjulia.cpp \
		biquadfixjuliaalt.cpp \
		biquadfixjuliaalt2.cpp \
		biquadfixjuliaalt3.cpp \
		biquadjulia.cpp \
		biquadreal.cpp \
		biquadrealjulia.cpp \
		biquadrealray.cpp \
		biquadsuperfix.cpp \
		biquadsuperfixjulia.cpp \
		blaschkebiquadsuper.cpp \
		blaschkebiquadsuperjulia.cpp \
		blaschkecubicsuper-log.cpp \
		blaschkecubicsuper-logjulia.cpp \
		blaschkecubicsuper.cpp \
		blaschkecubicsuperjulia.cpp \
		blaschkequad.cpp \
		cauliflower.cpp \
		cubic-2-rayjulia.cpp \
		cubic-a-ray.cpp \
		cubic-a.cpp \
		cubic-a2-ray.cpp \
		cubic-b-ray.cpp \
		cubic-b.cpp \
		cubic-b2-ray.cpp \
		cubic-rayjulia.cpp \
		cubicato-a.cpp \
		cubicato-ajulia.cpp \
		cubicato-arayper3.cpp \
		cubicato-arayper3julia.cpp \
		cubicfix1-2.cpp \
		cubicfix1.cpp \
		cubicfix2-2.cpp \
		cubicfix2-crit.cpp \
		cubicfix2.cpp \
		cubicfixjulia-2.cpp \
		cubicfixjulia.cpp \
		cubicfixmc1.cpp \
		cubicfixmc2.cpp \
		cubicfixmcjulia.cpp \
		cubicfixray.cpp \
		cubicfixrayjulia.cpp \
		cubicjulia.cpp \
		cubiclavaurs.cpp \
		cubiclavaurs2.cpp \
		cubiclavaursjulia.cpp \
		cubiclavaursjulia2.cpp \
		cubicnewton.cpp \
		cubicnewtonjulia.cpp \
		cubicreal.cpp \
		cubicrealjulia.cpp \
		cubicrealray.cpp \
		cubicsuper.cpp \
		cubicsuperjulia.cpp \
		cubicsuperper2.cpp \
		cubicsuperper2julia.cpp \
		cubicsuperper2-2.cpp \
		cubicsuperper2-2julia.cpp \
		cubicsuperper2ray.cpp \
		cubicsuperper2rayjulia.cpp \
		cubicsuperper2ray2.cpp \
		cubicsuperper2ray2julia.cpp \
		cubicsuperper3.cpp \
		cubicsuperper3julia.cpp \
		cubicsuperper3ray.cpp \
		cubicsuperper3rayjulia.cpp \
		cubicsuperper3torusray.cpp \
		cubicsuperper3torusrayjulia.cpp \
		cubicsuperper3ray_inverted.cpp \
		cubicsuperper3ray_invertedjulia.cpp \
		cubicsuperper3torusray_inverted.cpp \
		cubicsuperper3torusray_invertedjulia.cpp \
		cubicsuperray.cpp \
		cubicsuperrayjulia.cpp \
		doublestandard.cpp \
		doublestandardjulia.cpp \
		fix-twocrit-ray.cpp \
		fix-twocrit-rayjulia.cpp \
		fix-twocrit.cpp \
		fix-twocritjulia.cpp \
		henonunstable.cpp \
		jonsson-skew.cpp \
		jonsson-skew2.cpp \
		lavaursray.cpp \
		lavaursray2.cpp \
		lavaursray2julia.cpp \
		lavaursrayjulia.cpp \
		multicorn.cpp \
		multicornjulia.cpp \
		nearcauliflower.cpp \
		per2curves-lambda.cpp \
		per2curves.cpp \
		per2curvesjulia.cpp \
		quad-nearparab.cpp \
		quad-prefatou.cpp \
		quadfix.cpp \
		quadfixexp.cpp \
		quadfixexpjulia.cpp \
		quadfixjulia.cpp \
		quadjulia.cpp \
		quadratic.cpp \
		quadratic2.cpp \
		quadray.cpp \
		quadray2.cpp \
		quadrayjulia.cpp \
		quadrayper2.cpp \
		quadrayper2julia.cpp \
		quadrayper3.cpp \
		quadrayper3julia.cpp \
		quadrayper4.cpp \
		quadrayper4julia.cpp \
		quadsemi.cpp \
		quadsemijulia.cpp \
		quar-fix-p3.cpp \
		quar-fix-p3julia.cpp \
		quar-fix-super-green.cpp \
		quar-fix-super-greenjulia.cpp \
		quar-fix-super.cpp \
		quar-fix-superjulia.cpp \
		quar-sa-parab.cpp \
		quar-sa-parabjulia.cpp \
		quartic1.cpp \
		rat2per2-2.cpp \
		rat2per2-2julia.cpp \
		rat2per2.cpp \
		rat2per2julia.cpp \
		rat2per3-2.cpp \
		rat2per3-2julia.cpp \
		rat2per3.cpp \
		rat2per3julia.cpp \
		rat2per4-2.cpp \
		rat2per4-2julia.cpp \
		rat2per4.cpp \
		rat2per4julia.cpp \
		real-quartic-crit-preper.cpp \
		real-quartic-crit-preperjulia.cpp \
		realimplosion3.cpp \
		realimplosion3julia.cpp \
		realimplosion3ray.cpp \
		realimplosion3rayjulia.cpp \
		semi-parabolic-line-n-alphajulia.cpp \
		semi-parabolic-line.cpp \
		semi-parabolic-linejulia.cpp \
		semi-parabolic-unstable-n-alphajulia.cpp \
		semi-parabolic-unstable.cpp \
		semi-parabolic-unstablejulia.cpp \
		tanlei.cpp \
		tanleijulia.cpp \
		tricorn-green.cpp \
		tricorn-greenjulia.cpp \
		tricorn-ray.cpp \
		tricorn-rayjulia.cpp \
		tricorn.cpp \
		tricornjulia.cpp \
		typeE2.cpp \
		typeE2julia.cpp \
		typeE3.cpp \
		typeE3julia.cpp \
		unicrit-green.cpp \
		unicrit-greenjulia.cpp \
		unicrit-ray.cpp \
		unicrit-rayjulia.cpp \
		unicrit.cpp \
		unicritjulia.cpp
OBJECTS = 	\
		Q.so \
		Q2.so \
		Qfatou.so \
		Qren-domain.so \
		Qren-range.so \
		Qren.so \
		Qrenfatou.so \
		arithhedge-exp.so \
		arithhedge-skew-exp.so \
		arithhedge-skew.so \
		arithhedge.so \
		bicritper2.so \
		bicritper2julia.so \
		bicritper2ray.so \
		bicritper2rayjulia.so \
		biquad1.so \
		biquad2.so \
		biquadfix1.so \
		biquadfix1alt.so \
		biquadfix1alt2.so \
		biquadfix1alt3.so \
		biquadfix2.so \
		biquadfix2alt.so \
		biquadfix2alt2.so \
		biquadfixjulia.so \
		biquadfixjuliaalt.so \
		biquadfixjuliaalt2.so \
		biquadfixjuliaalt3.so \
		biquadjulia.so \
		biquadreal.so \
		biquadrealjulia.so \
		biquadrealray.so \
		biquadsuperfix.so \
		biquadsuperfixjulia.so \
		blaschkebiquadsuper.so \
		blaschkebiquadsuperjulia.so \
		blaschkecubicsuper-log.so \
		blaschkecubicsuper-logjulia.so \
		blaschkecubicsuper.so \
		blaschkecubicsuperjulia.so \
		blaschkequad.so \
		cauliflower.so \
		cubic-2-rayjulia.so \
		cubic-a-ray.so \
		cubic-a.so \
		cubic-a2-ray.so \
		cubic-b-ray.so \
		cubic-b.so \
		cubic-b2-ray.so \
		cubic-rayjulia.so \
		cubicato-a.so \
		cubicato-ajulia.so \
		cubicato-arayper3.so \
		cubicato-arayper3julia.so \
		cubicfix1-2.so \
		cubicfix1.so \
		cubicfix2-2.so \
		cubicfix2-crit.so \
		cubicfix2.so \
		cubicfixjulia-2.so \
		cubicfixjulia.so \
		cubicfixmc1.so \
		cubicfixmc2.so \
		cubicfixmcjulia.so \
		cubicfixray.so \
		cubicfixrayjulia.so \
		cubicjulia.so \
		cubiclavaurs.so \
		cubiclavaurs2.so \
		cubiclavaursjulia.so \
		cubiclavaursjulia2.so \
		cubicnewton.so \
		cubicnewtonjulia.so \
		cubicreal.so \
		cubicrealjulia.so \
		cubicrealray.so \
		cubicsuper.so \
		cubicsuperjulia.so \
		cubicsuperper2.so \
		cubicsuperper2julia.so \
		cubicsuperper2-2.so \
		cubicsuperper2-2julia.so \
		cubicsuperper2ray.so \
		cubicsuperper2rayjulia.so \
		cubicsuperper2ray2.so \
		cubicsuperper2ray2julia.so \
		cubicsuperper3.so \
		cubicsuperper3julia.so \
		cubicsuperper3ray.so \
		cubicsuperper3rayjulia.so \
		cubicsuperper3torusray.so \
		cubicsuperper3torusrayjulia.so \
		cubicsuperper3ray_inverted.so \
		cubicsuperper3ray_invertedjulia.so \
		cubicsuperper3torusray_inverted.so \
		cubicsuperper3torusray_invertedjulia.so \
		cubicsuperray.so \
		cubicsuperrayjulia.so \
		doublestandard.so \
		doublestandardjulia.so \
		fix-twocrit-ray.so \
		fix-twocrit-rayjulia.so \
		fix-twocrit.so \
		fix-twocritjulia.so \
		henonunstable.so \
		jonsson-skew.so \
		jonsson-skew2.so \
		lavaursray.so \
		lavaursray2.so \
		lavaursray2julia.so \
		lavaursrayjulia.so \
		multicorn.so \
		multicornjulia.so \
		nearcauliflower.so \
		per2curves-lambda.so \
		per2curves.so \
		per2curvesjulia.so \
		quad-nearparab.so \
		quad-prefatou.so \
		quadfix.so \
		quadfixexp.so \
		quadfixexpjulia.so \
		quadfixjulia.so \
		quadjulia.so \
		quadratic.so \
		quadratic2.so \
		quadray.so \
		quadray2.so \
		quadrayjulia.so \
		quadrayper2.so \
		quadrayper2julia.so \
		quadrayper3.so \
		quadrayper3julia.so \
		quadrayper4.so \
		quadrayper4julia.so \
		quadsemi.so \
		quadsemijulia.so \
		quar-fix-p3.so \
		quar-fix-p3julia.so \
		quar-fix-super-green.so \
		quar-fix-super-greenjulia.so \
		quar-fix-super.so \
		quar-fix-superjulia.so \
		quar-sa-parab.so \
		quar-sa-parabjulia.so \
		quartic1.so \
		rat2per2-2.so \
		rat2per2-2julia.so \
		rat2per2.so \
		rat2per2julia.so \
		rat2per3-2.so \
		rat2per3-2julia.so \
		rat2per3.so \
		rat2per3julia.so \
		rat2per4-2.so \
		rat2per4-2julia.so \
		rat2per4.so \
		rat2per4julia.so \
		real-quartic-crit-preper.so \
		real-quartic-crit-preperjulia.so \
		realimplosion3.so \
		realimplosion3julia.so \
		realimplosion3ray.so \
		realimplosion3rayjulia.so \
		semi-parabolic-line-n-alphajulia.so \
		semi-parabolic-line.so \
		semi-parabolic-linejulia.so \
		semi-parabolic-unstable-n-alphajulia.so \
		semi-parabolic-unstable.so \
		semi-parabolic-unstablejulia.so \
		tanlei.so \
		tanleijulia.so \
		tricorn-green.so \
		tricorn-greenjulia.so \
		tricorn-ray.so \
		tricorn-rayjulia.so \
		tricorn.so \
		tricornjulia.so \
		typeE2.so \
		typeE2julia.so \
		typeE3.so \
		typeE3julia.so \
		unicrit-green.so \
		unicrit-greenjulia.so \
		unicrit-ray.so \
		unicrit-rayjulia.so \
		unicrit.so \
		unicritjulia.so
.SUFFIXES: .cpp .so

.cpp.so: $(HEADERS)
	$(CXX) $(CXXFLAGS) -DPLUGIN -o $@ $<
#	for Linux
#	$(CXX) -shared -fPIC -Wall -W -pipe -O3 -ffast-math -I/usr/local/include -I$QTDIR/include -o $@ $<
#	 for Mac OS X (PPC)
#	$(CXX) -bundle -flat_namespace -undefined suppress -pipe -O3 -mcpu=7450 -mtune=7450 -mpowerpc-gfxopt -ffast-math -I/usr/local/include -I/usr/local/qt/include -o $@ $<
#	 for Mac OS X (Intel)
#	$(CXX) -bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math -I/opt/local/include -o $@ $<
#	for Mac OS X (Universal)
#	$(CXX) -arch ppc -arch i386 -bundle -flat_namespace -undefined suppress -pipe -O3 -ffast-math -I/opt/local/include -o $@ $<
#	 for Win32 (MinGW)
#	$(CXX) -shared -Wall -W -pipe -O3 -o $@ $<

all: $(OBJECTS)

clean:
	rm -f *~ *.o *.so *.core
