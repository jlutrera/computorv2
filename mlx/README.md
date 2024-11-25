This is the MinilibX, a simple X-Window (X11R6) programming API
in C, designed for students, suitable for X-beginners.


Contents

 - Source code in C to create the mlx library
 - A public include file mlx.h

Requirements for Linux

 - MinilibX only support TrueColor visual type (8,15,16,24 or 32 bits depth)
 - Coomands: make and gcc
 - X11 include files (package xorg)
 - XShm extension must be present (package libxext-dev)
 - Utility functions from BSD systems - development files (package libbsd-dev)
 - **e.g. _sudo apt-get install gcc make xorg libxext-dev libbsd-dev_ (Debian/Ubuntu)**


MlX Color Opacity / Transparency / Alpha (32 bits depth)
 - 0xFF (fully transparent) or 0x00 (fully opaque)

Compile MinilibX
 - Run make and the file libmlx.a will be created.

Install MinilibX
 - No installation script is provided. You may want to install
     - libmlx.a in /usr/X11/lib or /usr/local/lib
     - mlx.h in /usr/X11/include or /usr/local/include


Olivier CROUZET - 2014-01-06
