Compiling pvisu with mingw under Windows:

1. Add the object files (from src/objects/*.cpp)
in this directory.

2. Modify the pvisu.pro with the following lines:

SOURCES += pobject.cpp
SOURCES += main.cpp
SOURCES += region.cpp
SOURCES += graph.cpp
SOURCES += collection.cpp
SOURCES += bundled.cpp
SOURCES += errc.cpp

3. qmake pvisu.pro

4. Modify the Makefile.release to remove
all reference to the word 'thread' (this is the mean
to get rid of the .dll files).

5. make

6. Move the resulting pvisu.exe in the bin directory.

