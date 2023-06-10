#!/bin/sh


# Sprites
grit params.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit running.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit generatebutton.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit backbutton.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit slidertrack.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!


for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite



# Backgrounds
# grit splash.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
# grit splash.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit splash.png -ftb -fh! -gb -gB16
grit clear.png -ftb -fh! -gb -gB16
grit background.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit instructionsbackground.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img *.map ../nitrofiles/bg
