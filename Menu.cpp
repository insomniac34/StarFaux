/********************************************************************************************
 Developers: Charlie Koch & Tyler Raborn
 
 CS1566 Intro to Graphics Final Project
 
 This code is freely distributable under the GNU General Public License. See LICENSE.txt for more details.
 ********************************************************************************************/

#include "Menu.h"

extern bool PAUSED;
extern bool QUIT_GAME;
extern int game_status;

int markex = 0;
GLfloat explosionWidthX = 1.0;
GLfloat explosionWidthY = 1.0;
GLfloat explosionWidthZ = 1.0;
int colorFlag = 0;

Shape3D* my_ship;
GLuint title;
GLuint start_game;
GLuint controls;
GLuint keyboard;
GLuint back;
GLuint game_isWon;
GLuint paused_game;
GLuint game_isOver;
GLuint enemies;
GLuint health_left;
GLuint final_score;
GLuint quit;
GLuint hint;
GLuint numbers[10];

int selection = 0;

int current_menu = 0;

int show_controls = 0;

float cam_intervalX;
float cam_intervalY;

float default_color[4] = {0, 0, 0, 1};
float current_color[4] = {0, 0, 0, 1};
float colorStep = 0.02;
float selection_color[4] = {0.4, 0.4, 0.4, 1};

void init_menus()
{
	char mesh_file[50];
	strcpy(mesh_file, "arwing.txt");
	my_ship = new Mesh();
	my_ship->load_mesh(mesh_file);
	
	my_ship->world_translate(1, 0.6, 0);
	my_ship->local_rotate_y(M_PI/2.0);
	my_ship->local_rotate_x(-M_PI/8.0);
	
	char tex_file[50];
	
	glGenTextures(1, &title);
	glBindTexture(GL_TEXTURE_2D, title);
	strcpy(tex_file, "title.png");
	loadTex(title, tex_file, 1024, 256);
	
	glGenTextures(1, &hint);
	glBindTexture(GL_TEXTURE_2D, hint);
	strcpy(tex_file, "hint.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &keyboard);
	glBindTexture(GL_TEXTURE_2D, keyboard);
	strcpy(tex_file, "keyboard.png");
	loadTex(title, tex_file, 1024, 512);
	
	glGenTextures(1, &start_game);
	glBindTexture(GL_TEXTURE_2D, start_game);
	strcpy(tex_file, "start_game.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &controls);
	glBindTexture(GL_TEXTURE_2D, controls);
	strcpy(tex_file, "controls.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &back);
	glBindTexture(GL_TEXTURE_2D, back);
	strcpy(tex_file, "resume.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &game_isWon);
	glBindTexture(GL_TEXTURE_2D, game_isWon);
	strcpy(tex_file, "game_won.png");
	loadTex(title, tex_file, 1024, 64);
	
	glGenTextures(1, &paused_game);
	glBindTexture(GL_TEXTURE_2D, paused_game);
	strcpy(tex_file, "pause.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &game_isOver);
	glBindTexture(GL_TEXTURE_2D, game_isOver);
	strcpy(tex_file, "game_over.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &enemies);
	glBindTexture(GL_TEXTURE_2D, enemies);
	strcpy(tex_file, "enemies.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &health_left);
	glBindTexture(GL_TEXTURE_2D, health_left);
	strcpy(tex_file, "health.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &final_score);
	glBindTexture(GL_TEXTURE_2D, final_score);
	strcpy(tex_file, "score.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &quit);
	glBindTexture(GL_TEXTURE_2D, quit);
	strcpy(tex_file, "quit.png");
	loadTex(title, tex_file, 512, 64);
	
	glGenTextures(1, &numbers[0]);
	glBindTexture(GL_TEXTURE_2D, numbers[0]);
	strcpy(tex_file, "zero.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[1]);
	glBindTexture(GL_TEXTURE_2D, numbers[1]);
	strcpy(tex_file, "one.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[2]);
	glBindTexture(GL_TEXTURE_2D, numbers[2]);
	strcpy(tex_file, "two.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[3]);
	glBindTexture(GL_TEXTURE_2D, numbers[3]);
	strcpy(tex_file, "three.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[4]);
	glBindTexture(GL_TEXTURE_2D, numbers[4]);
	strcpy(tex_file, "four.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[5]);
	glBindTexture(GL_TEXTURE_2D, numbers[5]);
	strcpy(tex_file, "five.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[6]);
	glBindTexture(GL_TEXTURE_2D, numbers[6]);
	strcpy(tex_file, "six.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[7]);
	glBindTexture(GL_TEXTURE_2D, numbers[7]);
	strcpy(tex_file, "seven.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[8]);
	glBindTexture(GL_TEXTURE_2D, numbers[8]);
	strcpy(tex_file, "eight.png");
	loadTex(title, tex_file, 64, 64);
	
	glGenTextures(1, &numbers[9]);
	glBindTexture(GL_TEXTURE_2D, numbers[9]);
	strcpy(tex_file, "nine.png");
	loadTex(title, tex_file, 64, 64);
}

void loadTex(GLuint t1, char* t_file, int w, int h)
{
	int width = w; 
	int height = h;
	unsigned char* image = SOIL_load_image(t_file, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 0.0, 0.0, 0.0, 0.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	SOIL_free_image_data(image);
}

void drawTex(int the_x, int the_y, int width, int height, float col[4])
{
	
	width = width/2;
	height = height/2;
	
	GLfloat vert1[3];
	GLfloat vert2[3];
	GLfloat vert3[3];
	GLfloat vert4[3];
	
	// bottom left corner
	vert1[0] = the_x - width;
	vert1[1] = the_y - height;
	vert1[2] = 0;
	
	// top left corner
	vert2[0] = the_x - width;
	vert2[1] = the_y + height;
	vert2[2] = 0;
	
	// bottom right corner
	vert3[0] = the_x + width;
	vert3[1] = the_y - height;
	vert3[2] = 0;
	
	// top right corner
	vert4[0] = the_x + width;
	vert4[1] = the_y + height;
	vert4[2] = 0;
	
	glColor4fv(col);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3fv(vert1);
	glTexCoord2f(1, 1);
	glVertex3fv(vert3);
	glTexCoord2f(1, 0);
	glVertex3fv(vert4);
	glTexCoord2f(0, 0);
	glVertex3fv(vert2);
	glEnd();
}

void update_color()
{
	current_color[0] += colorStep;
	current_color[1] += colorStep;
	current_color[2] += colorStep;
	if(current_color[0] > selection_color[0] || current_color[1] > selection_color[1] || current_color[2] > selection_color[2])
	{
		current_color[0] = default_color[0];
		current_color[1] = default_color[1];
		current_color[2] = default_color[2];
	}
}

void start_menu()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void end_menu()
{
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void game_start()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(GL_FOV, GL_ASPECT_RATIO, GL_NEAR_CLIPPING_PLANE, GL_FAR_CLIPPING_PLANE);
	gluLookAt(0, 0, -6, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
	
	GLfloat my_amb[4] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat my_diff[4] = { 1, 1, 1, 1.0 };
	GLfloat my_spec[4] = { 0.0, 0.0, 0.0, 1.0 };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, my_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, my_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, my_spec);
	
	
	my_ship->local_rotate_z(M_PI/250.0);
	my_ship->world_rotate_y(M_PI/1600.0);
	my_ship->update();
	my_ship->draw_mesh(GL_TRIANGLES, GL_FLAT);
	
	update_color();
	
	start_menu();
	
	glBindTexture(GL_TEXTURE_2D, title);
	drawTex(600, 600, 1024, 256, default_color);
	
	glBindTexture(GL_TEXTURE_2D, hint);
	drawTex(600, 32, 512, 64, default_color);
	
	if(show_controls == 0 )
	{
		current_menu = 0;
	glBindTexture(GL_TEXTURE_2D, start_game);
	if(selection == BEGIN_GAME)
	{
		drawTex(600, 270, 512, 64, current_color);
	}
	else
	{
		drawTex(600, 270, 512, 64, default_color);
	}
	
	glBindTexture(GL_TEXTURE_2D, controls);
	if(selection == CONTROLS)
	{
		drawTex(600, 206, 512, 64, current_color);
	}
	else
	{
		drawTex(600, 206, 512, 64, default_color);
	}
	
	glBindTexture(GL_TEXTURE_2D, quit);
	if(selection == QUIT)
	{
		drawTex(600, 142, 512, 64, current_color);
	}
	else
	{
		drawTex(600, 142, 512, 64, default_color);
	}
	}
	else
	{
		current_menu = 1;
		glBindTexture(GL_TEXTURE_2D, keyboard);
		drawTex(600, 400, 1024, 512, default_color);
		glBindTexture(GL_TEXTURE_2D, back);
		drawTex(600, 142, 512, 64, current_color);
	}
	
	end_menu();
}

void pause_screen()
{
	start_menu();
	
	update_color();
	
	glBindTexture(GL_TEXTURE_2D, paused_game);
	drawTex(600, 570, 512, 64, default_color);
	
	if(show_controls == 0 )
	{
		current_menu = 2;
		glBindTexture(GL_TEXTURE_2D, back);
		if(selection == BACK)
		{
			drawTex(600, 470, 512, 64, current_color);
		}
		else
		{
			drawTex(600, 470, 512, 64, default_color);
		}
		
		glBindTexture(GL_TEXTURE_2D, controls);
		if(selection == CONTROLS)
		{
			drawTex(600, 406, 512, 64, current_color);
		}
		else
		{
			drawTex(600, 406, 512, 64, default_color);
		}
		
		glBindTexture(GL_TEXTURE_2D, quit);
		if(selection == QUIT)
		{
			drawTex(600, 342, 512, 64, current_color);
		}
		else
		{
			drawTex(600, 342, 512, 64, default_color);
		}
	}
	else
	{
		current_menu = 1;
		glBindTexture(GL_TEXTURE_2D, keyboard);
		drawTex(600, 400, 1024, 512, default_color);
		glBindTexture(GL_TEXTURE_2D, back);
		drawTex(600, 142, 512, 64, current_color);
	}
	
	end_menu();
}

void game_won(int e1, int h1)
{
	int total_score = e1*250 + h1*10;
	
	current_menu = 3;
	
	start_menu();
	
	update_color();
	
	glBindTexture(GL_TEXTURE_2D, game_isWon);
	drawTex(600, 570, 1024, 64, default_color);
	
		current_menu = 3;
		glBindTexture(GL_TEXTURE_2D, enemies);
			drawTex(400, 470, 512, 64, default_color);
	drawNumbers(e1*250, 700, 464);
		
		glBindTexture(GL_TEXTURE_2D, health_left);
			drawTex(400, 406, 512, 64, default_color);
	drawNumbers(h1*10, 700, 400);
		
		glBindTexture(GL_TEXTURE_2D, final_score);
			drawTex(400, 342, 512, 64, default_color);
	drawNumbers(total_score, 700, 334);
	
	glBindTexture(GL_TEXTURE_2D, quit);
	drawTex(600, 242, 512, 64, current_color);
	
	end_menu();
}

void drawNumbers(int n, int x, int y)
{
	int mark = 0;
	int i = 0;
	int k = 0;
	for(int j = 100000; j >= 1; j *= 0.1)
	{
		if(j == 1)
		{
			mark = 1;
		}
		if(n/j > 0 || mark > 0)
		{
			i = n/j;
			n -= i*j;
			glBindTexture(GL_TEXTURE_2D, numbers[i]);
			drawTex(x+k, y, 64, 64, default_color);
			mark = 1;
		}
		k += 44;
	}
}

void game_over(GLfloat* pos)
{
	current_menu = 4;
	
	if(markex == 0)
	{
	explosionWidthX = 1.0;
	explosionWidthY = 1.0;
	explosionWidthZ = 1.0;
	colorFlag = 0;
	
	delete my_ship;
	my_ship = new Sphere(1.0, 10, 10);
	my_ship->local_translate(pos[0], pos[1], pos[2]);
	my_ship->local_scale(explosionWidthX, explosionWidthY, explosionWidthZ);
	my_ship->set_color4f(1, 1, 0, 1);
	
	my_ship->draw(1);
		markex = 1;
	}
	//delete shpPtr;
	else
	{

	glDisable(GL_LIGHTING);
	if (explosionWidthX > 0)
	{
		if (colorFlag==0)
		{
			my_ship->set_color4f(1, 1, 0, 1);
		}
		else if (colorFlag==1)
		{
			my_ship->set_color4f(1, 0, 0, 1);
		}
		else if (colorFlag==2)
		{
			my_ship->set_color4f(1, 1, 1, 1);
			colorFlag = 0;
		}
		
		explosionWidthX-=0.01;
		explosionWidthY-=0.01;
		explosionWidthZ-=0.01;
		
		my_ship->local_scale(explosionWidthX, explosionWidthY, explosionWidthZ);
		my_ship->draw(1);
		
		colorFlag++;
	}
		glEnable(GL_LIGHTING);
	}
	
	start_menu();
	
	update_color();
	
	glBindTexture(GL_TEXTURE_2D, game_isOver);
	drawTex(600, 470, 512, 64, default_color);
	
	glBindTexture(GL_TEXTURE_2D, quit);
	drawTex(600, 400, 512, 64, current_color);
	
	end_menu();
}

void menu_press_down()
{
	switch(current_menu)
	{
		case 0:
			if(selection == BEGIN_GAME)
			{
				selection = CONTROLS;
			}
			else if(selection == CONTROLS)
			{
				selection = QUIT;
			}
			else if(selection == QUIT)
			{
				selection = BEGIN_GAME;
			}
			break;
		case 1:
			break;
		case 2:
			if(selection == BACK)
			{
				selection = CONTROLS;
			}
			else if(selection == CONTROLS)
			{
				selection = QUIT;
			}
			else if(selection == QUIT)
			{
				selection = BACK;
			}
			break;
		case 3:
			break;
		case 4:
			break;
	}
}
void menu_press_up()
{
	switch(current_menu)
	{
		case 0:
			if(selection == BEGIN_GAME)
			{
				selection = QUIT;
			}
			else if(selection == CONTROLS)
			{
				selection = BEGIN_GAME;
			}
			else if(selection == QUIT)
			{
				selection = CONTROLS;
			}
			break;
		case 1:
			break;
		case 2:
			if(selection == BACK)
			{
				selection = QUIT;
			}
			else if(selection == CONTROLS)
			{
				selection = BACK;
			}
			else if(selection == QUIT)
			{
				selection = CONTROLS;
			}
			break;
		case 3:
			break;
		case 4:
			break;
	}
}

void menu_confirm()
{
	switch(current_menu)
	{
		case 0:
			if(selection == BEGIN_GAME)
			{
				game_status = GAME_ON;
				selection = BACK;
				glClearColor(0.5, 0.5, 1.0, 1.0);
			}
			else if(selection == CONTROLS)
			{
				show_controls = 1;
			}
			else if(selection == QUIT)
			{
				QUIT_GAME = true;
			}
			break;
		case 1:
			show_controls = 0;
			break;
		case 2:
			if(selection == BACK)
			{
				PAUSED = false;
			}
			else if(selection == CONTROLS)
			{
				show_controls = 1;
			}
			else if(selection == QUIT && PAUSED)
			{
				exit(0);
			}
			break;
		case 3:
			exit(0);
			break;
		case 4:
			exit(0);
			break;
	}
}