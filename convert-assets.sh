#!/usr/bin/env sh

cd graphics
#for each .ase file
for FILE in *.ase; do
	#create a new directory to work in
	mkdir temp
	#generate the new filename
	NEWNAME="temp/$(basename $FILE .ase).png"
	#export the ase as a png
	libresprite -b $FILE --sheet $NEWNAME --data /dev/null;
	#split the exported png into 16x16 tiles
	magick convert $NEWNAME -crop 16x16 temp/tile.png
	#store the name of the resulting gba-compatible sheet
	FINALNAME="$(basename $FILE .ase).png"
	touch $FINALNAME
	#for each tile
	for TILE in $(ls temp/tile*.png | sort --version-sort); do
		#append it to the generated vertical strip sheet
		magick convert -append $FINALNAME $TILE $FINALNAME
	done
	rm -rf temp
done
