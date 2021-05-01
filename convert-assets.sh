#!/usr/bin/env sh

cd graphics
for FILE in $(ls *.ase); do
	libresprite -b $FILE --sheet "$(basename $FILE .ase).png" --data /dev/null --sheet-type columns --verbose;
done
