/********************************************************************************************
 Developers: Charlie Koch & Tyler Raborn
 
 CS1566 Intro to Graphics Final Project
 
 This code is freely distributable under the GNU General Public License. See LICENSE.txt for more details.
 ********************************************************************************************/
#include <cmath>
#include "config.h"
#include "Shape3D.h"
#include "SOIL/SOIL.h"

#define BEGIN_GAME 0
#define CONTROLS 1
#define QUIT 2
#define RESUME 3
#define BACK 4

void init_menus();
void loadTex(GLuint t1, char* t_file, int w, int h);
void drawTex(int the_x, int the_y, int width, int height, float col[4]);
void drawNumbers(int n, int x, int y);
void update_color();
void start_menu();
void end_menu();

void game_start();
void pause_screen();
void game_over(GLfloat* pos);
void game_won(int e1, int h1);

void menu_press_down();
void menu_press_up();
void menu_confirm();