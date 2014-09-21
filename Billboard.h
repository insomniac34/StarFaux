/********************************************************************************************
 Developers: Charlie Koch & Tyler Raborn
 
 CS1566 Intro to Graphics Final Project
 
 This code is freely distributable under the GNU General Public License. See LICENSE.txt for more details.
 ********************************************************************************************/
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


class Billboard
{
public:
	
	float color[4];
	float local_p[4];
	float width;
	float height;
	GLuint texture;
	
	Billboard(GLuint tex, float w, float h, float posx, float posy, float posz);
	~Billboard() {};
	
	void draw();
	void billboardBegin();
	void billboardEnd();
	
};