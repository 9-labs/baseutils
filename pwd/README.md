# pwd
###### Return name of the current working directory

## Usage

`pwd [-L|-P]`

## POSIX

Strives to meet the POSIX standard for `pwd`.

* https://pubs.opengroup.org/onlinepubs/9699919799/utilities/pwd.html

## Issues

Currently does not check for occurrences of `./` and `../`.
