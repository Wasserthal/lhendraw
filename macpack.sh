mkdir lhendraw.app
mkdir lhendraw.app/Contents
mkdir lhendraw.app/Contents/MacOS
mkdir lhendraw.app/Contents/Resources
mkdir lhendraw.app/gfx
cp build/Debug/ilhendraw lhendraw.app/Contents/MacOS/lhendraw
cp gfx/buttons.bmp lhendraw.app/gfx/buttons.bmp
cp LiberationMono-Regular.lennardfont lhendraw.app
cp LiberationMono-Regular.bin lhendraw.app
cp hotkeys.xml lhendraw.app
cp Info.plist lhendraw.app/Contents
cp lhendraw.icns lhendraw.app/Contents/Resources
