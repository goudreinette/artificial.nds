#!/bin/sh


# Sprites
grit params.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit running.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit generatebutton.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit backbutton.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit aboutbutton.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit slidertrack.bmp -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit sliderknob.png -ftb -fh! -gTFF00FF -gt -gB8 -m!


for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite


# Backgrounds
grit splash.png -ftb -fh! -gb -gB16
grit clear.png -ftb -fh! -gb -gB16
grit background.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit paramsbackground.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit instructionsbackground.bmp -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit aboutpage.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img *.map ../nitrofiles/bg


# Graphics parts
grit dot.png -ftb -fh! -gb -gB16
grit circlesmall.png -ftb -fh! -gb -gB16
grit pixel.png -ftb -fh! -gb -gB16
grit pixel1.png -ftb -fh! -gb -gB16
grit pixel2.png -ftb -fh! -gb -gB16
grit pixel3.png -ftb -fh! -gb -gB16
grit pixel4.png -ftb -fh! -gb -gB16
grit pixel5.png -ftb -fh! -gb -gB16

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.img ../nitrofiles/graphicsparts
