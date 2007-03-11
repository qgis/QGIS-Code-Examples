#/bin/bash
qmake
make
mkdir qgis_example4.app/Contents/MacOS/lib/
cp /Applications/qgis.app/Contents/MacOS/lib/libqgis_core.dylib qgis_example4.app/Contents/MacOS/lib/
cp /Applications/qgis.app/Contents/MacOS/lib/libqgis_gui.dylib qgis_example4.app/Contents/MacOS/lib/
