# pour mettre un set de cartes dans un tableau 13x4 (sera plus pratique à charger en godot)

from PIL import Image

frameW = 64
frameH = 64

nbCols = 13
nbLines = 1

prePath = "sol/"
pathPerLine = [""]
postPath = ".png"

base = Image.new(mode="RGBA", size=(frameW * nbCols, frameH * nbLines))

for line in range(nbLines):
    for col in range(nbCols):
        im = Image.open(prePath + pathPerLine[line] + str(col + 1) + postPath)
        im2 = im.resize((frameW, frameH))
        
        base.paste(im2, (frameW * col, frameH * line))

base.save("output.png")