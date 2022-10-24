# mkdir
###### Make directories

## Usage

`mkdir [-p] [-m mode] dir...`

## POSIX

Strives to meet the POSIX standard for `mkdir`.

* https://pubs.opengroup.org/onlinepubs/9699919799/utilities/mkdir.html

## Issues

`-m` mode switch relies on setmode()/getmode() not present in POSIX
specification of `<unistd.h>`, perhaps implement one in this package at some
point.
