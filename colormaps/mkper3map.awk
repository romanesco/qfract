#!/usr/bin/awk -f
BEGIN {n=0}
{
  if (n % 3 == 0)
    {print $1" "$2" "$3}
  else { if (n % 3 == 1)
    {print $3" "$1" "$2}
  else
    {print $2" "$3" "$1}}
  n++
}

