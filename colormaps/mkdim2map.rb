#!/usr/local/bin/ruby

for n in 0..255
  printf( "%d\t%d\t%d\n", (n % 16)*16, (n/16)*16, 0)
end


