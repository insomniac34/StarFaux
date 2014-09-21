SOURCES=Camera.cpp Shape.cpp Face.cpp Light.cpp matrix.cpp Projectile.cpp vec3.cpp Vertex.cpp World.cpp NPC.cpp Path.cpp Shape3D.cpp matrixmath.cpp player.cpp Billboard.cpp Menu.cpp PowerUp.cpp

SOIL=SOIL/SOIL.c SOIL/image_DXT.c SOIL/image_helper.c SOIL/stb_image_aug.c

FLAGS=

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
L_FLAGS=-L/usr/local/lib -lglut -lGL -lGLU -std=c++11
endif
ifeq ($(UNAME), Darwin)
L_FLAGS=-framework Foundation -framework AppKit -framework OpenGL -framework GLUT
endif

all:
	g++ $(SOURCES) $(SOIL) $(EXE) $(L_FLAGS) -Wno-write-strings -o StarFaux

clean:
	rm -rf *o all
