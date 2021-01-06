# TDune SDL/OPENGL tile/auto-terrain render

    TDune is a top-view tile terrain render example written using SDL and OpenGL graphic library. 
There are 2 separate binaries for SDL and OPENGL versions, the SDL version being able to run in window or fullscreen while the OpenGL one only in fullscreen. 
The terrain/resources are random generated but the generator it tries to generate different terrains (rivers, mountains, hills) continous. 
The maps are saved in binary form you'll find the struct's used for terrain in the source code. The graphics are stored in the *.dat files which actually are large png files, getting the tile from these png's is made by offset.

## Features:


- top-view terrain render with SDL and OpenGL acceleration.
- multiple terrain layers (basic terrain(rivers,hills, mountains), trees, stuctures)
- interface with minimap and minimap viewport generated from the terrain
- level save/load/auto-generate
- per player resources/structures/units (random or manual generation)
- fullscreen or window-mode
- fast and smooth scrolling


## Keys

Q - quit
F - toggle fullscreen/window (sdl only)
N - next level
P - previous level
S - show map statistics

