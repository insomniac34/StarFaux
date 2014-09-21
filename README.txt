

<><><><><><><><><><><>_________________STAR FAUX v1.0_________________<><><><><><><><><><><>
							Written by Tyler Raborn & Charlie Koch

*****************************************************Section I: Usage*********************************************************************

Run the executable in the /bin folder from the command line, or by clicking on it.

*****************************************************Section II: SYSTEM REQUIREMENTS*****************************************************

OS: Mac OSX or Debian-based Linux,
LIBRARIES: the GLUT library (see Section III for install instructions)
CPU: Pentium 2 400MHz,
RAM: 256mb DDR SDRAM,
STORAGE: 1MB of HDD space
GPU: OpenGL-Compliant 3D Accelerator

*****************************************************Section III: INSTALL INSTRUCTIONS*****************************************************

To install GLUT:
If on a Mac:
	-Install Xcode for free from the Apple Store
	-Ensure everything is up to date
	
If on Linux:
	-sudo apt-get install build-essential (to make sure everything is up to date)
	-sudo apt-get install freeglut3-dev
	
Once GLUT is installed, you simply need to run the executable!

*****************************************************Section IV: COMPILATION*****************************************************

If you wish to compile your own executable, simply use the included makefile. Simply type 'make' 
at the command line from inside of the folder the source code is stored in. After compiling, run the executable!

Or, if you're feeling particularly self-loathing, you can compile via direct command:

	g++ -o <executable name> Camera.cpp Shape.cpp Face.cpp Light.cpp matrix.cpp Projectile.cpp vec3.cpp Vertex.cpp World.cpp NPC.cpp Path.cpp Shape3D.cpp matrixmath.cpp player.cpp Billboard.cpp Menu.cpp
	SOIL=SOIL/SOIL.c SOIL/image_DXT.c SOIL/image_helper.c SOIL/stb_image_aug.c -lglut -lGL -lGLU -Wno-write-strings
	
*****************************************************Section V: TERMS AND CONDITIONS*****************************************************

Copyright (C) 2013 Tyler Raborn & Charlie Koch

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
