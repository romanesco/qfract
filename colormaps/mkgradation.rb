#!/usr/bin/env ruby

# billiard green
r0 = 0x00
g0 = 0x48
b0 = 0x40

# ¼—tF
#r0=0x6e
#g0=0x81
#b0=0x5c

# — —t–ö
r1=0xc1
g1=0xd8
b1=0xac

# Science 
#r0=38
#g0=150
#b0=60
#
#r1=248
#g1=208
#b1=31

# dark blue
#r0=0
#g0=32
#b0=91
#
#r1=255
#g1=255
#b1=255

for n in 0..255
  r = (r0*(255-n) + r1*n)/255
  g = (g0*(255-n) + g1*n)/255
  b = (b0*(255-n) + b1*n)/255
  printf( "%d\t%d\t%d\n", r, g, b)
end


