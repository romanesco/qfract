#!/usr/bin/awk -f
BEGIN {n=0}
{if (n++ % 2 == 0) {print $2" "$3" "$1} else {print $3" "$1" "$2} }

