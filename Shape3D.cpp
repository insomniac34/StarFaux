/****************************************************
 FILE:		Shape3D.cpp
 PLATFORM:	Mac OS X
 AUTHOR:		Charles Koch
 DATE:		October 26, 2013
 DESC:		Class for creating 3D primitives.
 ****************************************************/

#include "Shape3D.h"


// SHAPE3D CLASS CONSTRUCTOR
/////////////////////////////////////////////

Shape3D :: Shape3D()
{
	// initialize local coordinate system
	local_x[0] = 1.0;
	local_x[1] = 0.0;
	local_x[2] = 0.0;
	local_x[3] = 1.0;
	
	local_y[0] = 0.0;
	local_y[1] = 1.0;
	local_y[2] = 0.0;
	local_y[3] = 1.0;
	
	local_z[0] = 0.0;
	local_z[1] = 0.0;
	local_z[2] = 1.0;
	local_z[3] = 1.0;
	
	local_p[0] = 0.0;
	local_p[1] = 0.0;
	local_p[2] = 0.0;
	local_p[3] = 1.0;
	
	scale[0] = 1.0;
	scale[1] = 1.0;
	scale[2] = 1.0;
	scale[3] = 1.0;
	
	rotation[0] = 0.0;
	rotation[1] = 0.0;
	rotation[2] = 0.0;
	rotation[3] = 1.0;
	
	// default color is red
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;
	color[3] = 1.0;
	
	// define number of vertices
	verts_size = 0;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = 0;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	spawnLock = 0;
}

void Shape3D :: set_color4f(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void Shape3D :: set_color(float c[4])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
	color[3] = c[3];
}

float* Shape3D :: get_color()
{
	return color;
}

// DRAW FUNCTION
/////////////////////////////////////////////

void Shape3D :: draw(int draw_mode)
{
	
	int my_draw = GL_LINE_LOOP;
	int normals = 0;
	int axes = 0;
	
	if((draw_mode > 3 && draw_mode < 6) || (draw_mode > 11 && draw_mode < 14))
	{
		normals = 1;
	}
	if(draw_mode - 8 > -1)
	{
		axes = 1;
	}
	draw_mode = draw_mode & 3;
	switch(draw_mode)
	{
		case WIREFRAME:
			my_draw = GL_LINE_LOOP;
			break;
		case SOLID:
			my_draw = GL_TRIANGLES;
			break;
	}
	
	if(axes > 0)
	{
		glLineWidth(5.0);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex4fv(local_p);
		glVertex4f(local_p[0] + local_x[0], local_p[1] + local_x[1], local_p[2] + local_x[2], 1.0);
		glVertex4fv(local_p);
		glVertex4f(local_p[0] + local_y[0], local_p[1] + local_y[1], local_p[2] + local_y[2], 1.0);
		glVertex4fv(local_p);
		glVertex4f(local_p[0] + local_z[0], local_p[1] + local_z[1], local_p[2] + local_z[2], 1.0);
		glEnd();
		glLineWidth(1.0);
	}
	
	glColor4fv(color);
	glShadeModel(GL_SMOOTH);
	
	for( int i = 0; i < faces_size; i++ )
	{
		glBegin(GL_TRIANGLES);
		glNormal3fv(f_norms[i]);
		glVertex4fv(verts[faces[i][0]]);
		glNormal3fv(f_norms[i]);
		glVertex4fv(verts[faces[i][1]]);
		glNormal3fv(f_norms[i]);
		glVertex4fv(verts[faces[i][2]]);
		glEnd();
	}
	
	if(normals > 0)
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
		for( int i = 0; i < verts_size; i++ )
		{
			glVertex4fv(verts[i]);
			glVertex4f(verts[i][0] + v_norms[i][0], verts[i][1] + v_norms[i][1], verts[i][2] + v_norms[i][2], 1.0);
		}
		glEnd();
	}
	glShadeModel(GL_FLAT);
}

// VECTOR FUNCTIONS
/////////////////////////////////////////////

void Shape3D :: apply_verts(float m[16])
{
	for( int i = 0; i < verts_size; i ++ )
	{
		vm_dot(verts[i], m, verts[i]);
	}
}

void Shape3D :: apply_norms(float m[16])
{
	for( int i = 0; i < faces_size; i ++ )
	{
		vm_dot(f_norms[i], m, f_norms[i]);
		normalize(f_norms[i]);
	}
	
	for( int i = 0; i < verts_size; i ++ )
	{
		vm_dot(v_norms[i], m, v_norms[i]);
		normalize(v_norms[i]);
	}
}

void Shape3D :: v_translate(GLfloat v[4], float x, float y, float z)
{
	v[0] += x;
	v[1] += y;
	v[2] += z;
	v[3] = 1.0;
}

void Shape3D :: v_rotate_x(GLfloat v[4], float theta)
{
	GLfloat v1 = v[1]*cos(theta) - v[2]*sin(theta);
	GLfloat v2 = v[1]*sin(theta) + v[2]*cos(theta);
	v[1] = v1;
	v[2] = v2;
}

void Shape3D :: v_rotate_y(GLfloat v[4], float theta)
{
	GLfloat v0 = v[0]*cos(theta) + v[2]*sin(theta);
	GLfloat v2 = -(v[0]*sin(theta)) + v[2]*cos(theta);
	v[0] = v0;
	v[2] = v2;
}

void Shape3D :: v_rotate_z(GLfloat v[4], float theta)
{
	GLfloat v0 = v[0]*cos(theta) - v[1]*sin(theta);
	GLfloat v1 = v[0]*sin(theta) + v[1]*cos(theta);
	v[0] = v0;
	v[1] = v1;
}

// LOCAL TRANSFORMATIONS
/////////////////////////////////////////////

void Shape3D :: local_scale(float x, float y, float z)
{
	GLfloat lx = local_p[0];
	GLfloat ly = local_p[1];
	GLfloat lz = local_p[2];
	
	GLfloat rot[16] = {
		local_x[0], local_y[0], local_z[0], 0,
		local_x[1], local_y[1], local_z[1], 0,
		local_x[2], local_y[2], local_z[2], 0,
		0, 0, 0, 1
	};
	
	GLfloat inv_rot[16] = {
		rot[0], rot[4], rot[8], 0,
		rot[1], rot[5], rot[9], 0,
		rot[2], rot[6], rot[10], 0,
		0, 0, 0, 1
	};
	
	GLfloat scaling[16] ={
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
	
	GLfloat inv_scaling[16] ={
		1/x, 0, 0, 0,
		0, 1/y, 0, 0,
		0, 0, 1/z, 0,
		0, 0, 0, 1
	};
	
	GLfloat CTM[16];
	GLfloat normals_CTM[16];
	load_identity_matrix(CTM);
	load_identity_matrix(normals_CTM);
	
	// create the CTM for vertices
	// T*R*S*R-1*T-1
	world_translate(-lx, -ly, -lz);
	mm_dot(CTM, rot, CTM);
	mm_dot(CTM, scaling, CTM);
	mm_dot(CTM, inv_rot, CTM);
	
	// create the CTM for normals
	// R*S-1*R-1
	mm_dot(normals_CTM, rot, normals_CTM);
	mm_dot(normals_CTM, inv_scaling, normals_CTM);
	mm_dot(normals_CTM, inv_rot, normals_CTM);
	
	vm_dot(scale, scaling, scale);
	
	apply_verts(CTM);
	world_translate(lx, ly, lz);
	apply_norms(normals_CTM);
}

void Shape3D :: local_rotate_x(float degrees)
{
	arbitrary_rotate(degrees, local_p, local_x);
}

void Shape3D :: local_rotate_y(float degrees)
{
	arbitrary_rotate(degrees, local_p, local_y);
}

void Shape3D :: local_rotate_z(float degrees)
{
	arbitrary_rotate(degrees, local_p, local_z);
}

void Shape3D :: arbitrary_rotate(float degrees, float* axis_p, float* axis_v)
{
	GLfloat ax = axis_p[0];
	GLfloat ay = axis_p[1];
	GLfloat az = axis_p[2];
	
	GLfloat trans[16] = {
		1, 0, 0, -ax,
		0, 1, 0, -ay,
		0, 0, 1, -az,
		0, 0, 0, 1
	};
	
	GLfloat inv_trans[16] = {
		1, 0, 0, ax,
		0, 1, 0, ay,
		0, 0, 1, az,
		0, 0, 0, 1
	};
	
	GLfloat v_len = sqrt(pow(axis_v[0], 2) + pow(axis_v[1], 2) + pow(axis_v[2], 2));
	GLfloat ux = (axis_v[0])/v_len;
	GLfloat uy = (axis_v[1])/v_len;
	GLfloat uz = (axis_v[2])/v_len;
	GLfloat ux2 = pow(ux , 2);
	GLfloat uy2 = pow(uy , 2);
	GLfloat uz2 = pow(uz , 2);
	
	GLfloat cosD = cos(degrees);
	GLfloat sinD = sin(degrees);
	
	GLfloat arb_rot[16];
	
	arb_rot[0] = ux2+cosD*(1-ux2);
	arb_rot[1] = ux*uy*(1-cosD)-uz*sinD;
	arb_rot[2] = uz*ux*(1-cosD)+uy*sinD;
	arb_rot[3] = 0;
	
	arb_rot[4] = ux*uy*(1-cosD)+uz*sinD;
	arb_rot[5] = uy2+cosD*(1-uy2);
	arb_rot[6] = uy*uz*(1-cosD)-ux*sinD;
	arb_rot[7] = 0;
	
	arb_rot[8] = uz*ux*(1-cosD)-uy*sinD;
	arb_rot[9] = uy*uz*(1-cosD)+ux*sinD;
	arb_rot[10] = uz2+cosD*(1-uz2);
	arb_rot[11] = 0;
	
	arb_rot[12] = 0;
	arb_rot[13] = 0;
	arb_rot[14] = 0;
	arb_rot[15] = 1;
	
	GLfloat CTM[16];
	GLfloat normals_CTM[16];
	load_identity_matrix(CTM);
	load_identity_matrix(normals_CTM);
	
	// create the CTM for vertices
	// T*R*T-1
	world_translate(-ax, -ay, -az);
	mm_dot(CTM, arb_rot, CTM);
	
	// create the CTM for normals
	// R
	mm_dot(normals_CTM, arb_rot, normals_CTM);
	
	apply_verts(CTM);
	world_translate(ax, ay, az);
	
	apply_norms(normals_CTM);
	
	vm_dot(local_x, arb_rot, local_x);
	vm_dot(local_y, arb_rot, local_y);
	vm_dot(local_z, arb_rot, local_z);
}

void Shape3D :: local_translate(float x, float y, float z)
{
	GLfloat trans[16] = {
		1, 0, 0, x*local_x[0]+y*local_y[0]+z*local_z[0],
		0, 1, 0, x*local_x[1]+y*local_y[1]+z*local_z[1],
		0, 0, 1, x*local_x[2]+y*local_y[2]+z*local_z[2],
		0, 0, 0, 1
	};
	
	vm_dot(local_p, trans, local_p);
	apply_verts(trans);
}

// WORLD TRANSFORMATIONS
/////////////////////////////////////////////
void Shape3D :: world_scale(float x, float y, float z)
{
	GLfloat scaling[16] ={
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
	
	GLfloat inv_scaling[16] ={
		1/x, 0, 0, 0,
		0, 1/y, 0, 0,
		0, 0, 1/z, 0,
		0, 0, 0, 1
	};
	
	vm_dot(local_p, scaling, local_p);
	
	vm_dot(scale, scaling, scale);
	
	apply_verts(scaling);
	apply_norms(inv_scaling);
	
}

void Shape3D :: world_rotate_x(float theta)
{
	GLfloat rot[16] = {
		1, 0, 0, 0,
		0, cos(theta), -1*sin(theta), 0,
		0, sin(theta), cos(theta), 0,
		0, 0, 0, 1
	};
	
	vm_dot(local_p, rot, local_p);
	vm_dot(local_x, rot, local_x);
	vm_dot(local_y, rot, local_y);
	vm_dot(local_z, rot, local_z);
	
	apply_verts(rot);
	apply_norms(rot);
}

void Shape3D :: world_rotate_y(float theta)
{
	GLfloat rot[16] = {
		cos(theta), 0, sin(theta), 0,
		0, 1, 0, 0,
		-1*sin(theta), 0, cos(theta), 0,
		0, 0, 0, 1
	};
	
	vm_dot(local_p, rot, local_p);
	vm_dot(local_x, rot, local_x);
	vm_dot(local_y, rot, local_y);
	vm_dot(local_z, rot, local_z);
	
	apply_verts(rot);
	apply_norms(rot);
}

void Shape3D :: world_rotate_z(float theta)
{
	GLfloat rot[16] = {
		cos(theta), -1*sin(theta), 0, 0,
		sin(theta), cos(theta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	vm_dot(local_p, rot, local_p);
	vm_dot(local_x, rot, local_x);
	vm_dot(local_y, rot, local_y);
	vm_dot(local_z, rot, local_z);
	
	apply_verts(rot);
	apply_norms(rot);
}

void Shape3D :: world_translate(float x, float y, float z)
{
	GLfloat translation[16] ={
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
	
	vm_dot(local_p, translation, local_p);
	apply_verts(translation);
}

void Shape3D :: reset_to_origin()
{
	GLfloat translation[16] ={
		1, 0, 0, -local_p[0],
		0, 1, 0, -local_p[1],
		0, 0, 1, -local_p[2],
		0, 0, 0, 1
	};
	
	vm_dot(local_p, translation, local_p);
	apply_verts(translation);
}

// DERIVED CLASSES
/////////////////////////////////////////////

Cube :: Cube(float my_size)
{
	// adjust size to form around the origin
	h = my_size/2.0;
	
	// define number of vertices
	verts_size = 8;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = 12;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	// VERTICES
	/////////////////////////////////////////////
	
	// front-top-right corner
	make_vert(0, -h, h, h);
	
	// front-bottom-right corner
	make_vert(1, -h, -h, h);
	
	// front-bottom-left corner
	make_vert(2, h, -h, h);
	
	// front-top-left corner
	make_vert(3, h, h, h);
	
	// back-top-right corner
	make_vert(4, -h, h, -h);
	
	// back-bottom-right corner
	make_vert(5, -h, -h, -h);
	
	// back-bottom-left corner
	make_vert(6, h, -h, -h);
	
	// back-top-left corner
	make_vert(7, h, h, -h);
	
	// FACES + NORMALS
	/////////////////////////////////////////////
	
	// front face 1
	make_face(0, 3, 2, 1);
														
	// front face 2
	make_face(1, 3, 1, 0);
	
	// left face 1
	make_face(2, 7, 6, 2);
	
	// left face 2
	make_face(3, 7, 2, 3);
	
	// back face 1
	make_face(4, 4, 5, 6);
	
	// back face 2
	make_face(5, 4, 6, 7);
	
	// right face 1
	make_face(6, 0, 1, 5);
	
	// right face 2
	make_face(7, 0, 5, 4);
	
	// top face 1
	make_face(8, 7, 3, 0);
	
	// top face 2
	make_face(9, 7, 0, 4);
	
	// bottom face 1
	make_face(10, 2, 6, 5);
	
	// bottom face 2
	make_face(11, 2, 5, 1);
	
	// VERTEX NORMALS
	/////////////////////////////////////////////
	// NOTE: The cube vertices have a varying
	// number of faces (some of which have the same normal).
	// This makes calculating the normals difficult.
	// Because there are only 8 vertices,
	// it is simple enough to just hard code them in.
	
	v_norms[0][0] = -1.0/3.0;
	v_norms[0][1] = 1.0/3.0;
	v_norms[0][2] = 1.0/3.0;
	v_norms[0][3] = 1.0;
	
	v_norms[1][0] = -1.0/3.0;
	v_norms[1][1] = -1.0/3.0;
	v_norms[1][2] = 1.0/3.0;
	v_norms[1][3] = 1.0;
	
	v_norms[2][0] = 1.0/3.0;
	v_norms[2][1] = -1.0/3.0;
	v_norms[2][2] = 1.0/3.0;
	v_norms[2][3] = 1.0;
	
	v_norms[3][0] = 1.0/3.0;
	v_norms[3][1] = 1.0/3.0;
	v_norms[3][2] = 1.0/3.0;
	v_norms[3][3] = 1.0;
	
	v_norms[4][0] = -1.0/3.0;
	v_norms[4][1] = 1.0/3.0;
	v_norms[4][2] = -1.0/3.0;
	v_norms[4][3] = 1.0;
	
	v_norms[5][0] = -1.0/3.0;
	v_norms[5][1] = -1.0/3.0;
	v_norms[5][2] = -1.0/3.0;
	v_norms[5][3] = 1.0;
	
	v_norms[6][0] = 1.0/3.0;
	v_norms[6][1] = -1.0/3.0;
	v_norms[6][2] = -1.0/3.0;
	v_norms[6][3] = 1.0;
	
	v_norms[7][0] = 1.0/3.0;
	v_norms[7][1] = 1.0/3.0;
	v_norms[7][2] = -1.0/3.0;
	v_norms[7][3] = 1.0;
	
}

void Cube :: make_vert(int vert_num, float x, float y, float z)
{
	// allocate new vertex
	GLfloat* v = new GLfloat[4];
	
	// set inital values
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = 1;
	// place into verts array
	verts[vert_num] = v;
	
	// initalize vertex normal array
	v_norms[vert_num] = new GLfloat[4];
	init_vector(v_norms[vert_num]);
}

void Cube :: make_face(int face_num, int v1, int v2, int v3)
{
	// allocate new face
	int* f = new int[3];
	// allocate new face normal
	GLfloat* fn = new GLfloat[4];
	init_vector(fn);
	
	// set face vertices
	f[0] = v1;
	f[1] = v2;
	f[2] = v3;
	// place into faces array
	faces[face_num] = f;
	
	// calculate face normal
	GLfloat vector1[4], vector2[4];
	vv_sub(verts[v2], verts[v1], vector1);
	vv_sub(verts[v2], verts[v3], vector2);
	vv_cross(vector1, vector2, fn);
	normalize(fn);
	// place into face normals array
	f_norms[face_num] = fn;
}

Sphere :: Sphere(float r, int vs, int rs)
{
	// set radius
	radius = r;
	
	// set stack interval
	float stack = M_PI / vs;
	// set slice interval
	float slice = (2 * M_PI) / rs;
	
	// define the number of vertices
	verts_size = (vs - 1) * (rs) + 2;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = (vs - 1) * rs * 2;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	// define to keep track of faces 
	// for vertex normal calculation
	int vert_faces[verts_size];
	
	// VERTICES
	/////////////////////////////////////////////
	
	GLfloat* v;
	
	// current angle of rotation for stack
	float curr_stack;
	// current angle of rotation for slice
	float curr_slice;
	// current positions in the array
	int pos, pos2, pos3;
	
	// loop to set "body" vertices
	for( int i = 0; i < vs - 1; i ++ )
	{
		// calculate stack angle
		curr_stack = -(i+1) * stack;
		
		for( int j = 0; j < rs; j++ )
		{
			// calculate slice angle
			curr_slice = j * slice;
			
			// calculate position in the array
			pos = i * rs + j;
			
			// allocate a new vertex
			v = new GLfloat[4];
			// set vertex at north pole
			v[0] = 0;
			v[1] = r;
			v[2] = 0;
			v[3] = 1;
			
			// rotate by stack angle
			v_rotate_z(v, curr_stack);
			// rotate by slice angle
			v_rotate_y(v, curr_slice);
			
			// store in verts array
			verts[pos] = v;
			// initialize vertex normal
			v_norms[pos] = new GLfloat[4];
			init_vector(v_norms[pos]);
			// initalize vertex face count
			vert_faces[pos] = 0;
		}
	}
	
	pos = verts_size - 2;
	// allocate a new vertex
	v = new GLfloat[4];
	// set vertex at north pole
	v[0] = 0;
	v[1] = r;
	v[2] = 0;
	v[3] = 1;
	// store in verts array
	verts[pos] = v;
	// initialize vertex normal
	v_norms[pos] = new GLfloat[4];
	init_vector(v_norms[pos]);
	// initalize vertex face count
	vert_faces[pos] = 0;
	
	pos = verts_size - 1;
	// allocate a new vertex
	v = new GLfloat[4];
	// set vertex at south pole
	v[0] = 0;
	v[1] = -r;
	v[2] = 0;
	v[3] = 1;
	// store in verts array
	verts[pos] = v;
	// initialize vertex normal
	v_norms[pos] = new GLfloat[4];
	init_vector(v_norms[pos]);
	// initalize vertex face count
	vert_faces[pos] = 0;
	
	
	// FACES + NORMALS
	/////////////////////////////////////////////
	
	// position of face in array
	int f_pos;
	
	// first loop sets the body of the sphere
	for( int i = 0; i < vs - 2; i++ )
	{
		for( int j = 0; j < 2*(rs-1); j+= 2 )
		{
			// calculate position in the array
			pos = i * rs + j/2;
			pos2 = (i + 1) * rs +j/2;
			pos3 = pos2 + 1;
			f_pos = i * 2 * rs + j;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
			f_pos++;
			pos2++;
			pos3 = pos + 1;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
		}
		
		// calculate position in the array
		pos = (i * rs)+ rs - 1;
		pos2 = (i + 1) * rs + rs - 1;
		pos3 = (i + 1) * rs;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
		
		f_pos++;
		pos2 = (i+1) * rs;
		pos3 = i * rs;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// second loop sets the top endcap
	////////////////////////////////////////
	for( int i = 0; i < rs - 1; i++ )
	{
		// set positions in arrays
		pos = verts_size-2;
		pos2 = i;
		pos3 = i + 1;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// set positions in arrays
	pos = verts_size-2;
	pos2 = pos3;
	pos3 = 0;
	f_pos++;
	
	set_face(f_pos, pos, pos2, pos3, vert_faces);
	
	// third loop sets the bottom endcap
	////////////////////////////////////////
	for( int i = 0; i < rs - 1; i++ )
	{
		// set positions in arrays
		pos = verts_size-1;
		pos3 = (vs - 2) * rs + i;
		pos2 = pos3 + 1;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// set positions in arrays
	pos = verts_size-1;
	pos2 = pos3;
	pos3 = (vs - 2) * rs;
	f_pos++;
	
	set_face(f_pos, pos, pos2, pos3, vert_faces);
	
	// VERTEX NORMALS
	/////////////////////////////////////////////
	
	// loop through vertices
	for( int i = 0; i < verts_size; i++ )
	{
		v_norms[i][0] = v_norms[i][0] / vert_faces[i];
		v_norms[i][1] = v_norms[i][1] / vert_faces[i];
		v_norms[i][2] = v_norms[i][2] / vert_faces[i];
		v_norms[i][3] = 1.0;
		normalize(v_norms[i]);
	}
	
	v_norms[verts_size-2][0] = 0.0;
	v_norms[verts_size-2][1] = 1.0;
	v_norms[verts_size-2][2] = 0.0;
	v_norms[verts_size-2][3] = 1.0;
	
	v_norms[verts_size-1][0] = 0.0;
	v_norms[verts_size-1][1] = -1.0;
	v_norms[verts_size-1][2] = 0.0;
	v_norms[verts_size-1][3] = 1.0;
	
}

Cylinder :: Cylinder(float r, float h, int vs, int rs)
{
	// set radius
	radius = r;
	// set height
	height = h/2.0;
	
	// set stack interval
	float stack = h / vs;
	// set slice interval
	float slice = (2 * M_PI) / rs;
	
	// define the number of vertices
	verts_size = (vs + 1) * (rs) + 2;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = (vs + 1) * rs * 2;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	// define to keep track of faces 
	// for vertex normal calculation
	int vert_faces[verts_size];
	
	// VERTICES
	/////////////////////////////////////////////
	
	GLfloat* v;
	
	// current position of the stack on the y-axis
	float curr_stack;
	// current angle of rotation for slice
	float curr_slice;
	// current positions in the array
	int pos, pos2, pos3;
	
	// loop to set "body" vertices
	for( int i = 0; i < vs + 1; i ++ )
	{
		// calculate stack angle
		curr_stack = -i * stack;
		
		for( int j = 0; j < rs; j++ )
		{
			// calculate slice angle
			curr_slice = j * slice;
			
			// calculate position in the array
			pos = i * rs + j;
			
			// allocate a new vertex
			v = new GLfloat[4];
			// set vertex at north pole
			v[0] = radius;
			v[1] = height;
			v[2] = 0;
			v[3] = 1;
			
			// rotate by stack angle
			v_translate(v, 0.0, curr_stack, 0.0);
			// rotate by slice angle
			v_rotate_y(v, curr_slice);
			
			// store in verts array
			verts[pos] = v;
			// initialize vertex normal
			v_norms[pos] = new GLfloat[4];
			init_vector(v_norms[pos]);
			// initalize vertex face count
			vert_faces[pos] = 0;
		}
	}
	
	pos = verts_size - 2;
	// allocate a new vertex
	v = new GLfloat[4];
	// set vertex at north pole
	v[0] = 0;
	v[1] = height;
	v[2] = 0;
	v[3] = 1;
	// store in verts array
	verts[pos] = v;
	// initialize vertex normal
	v_norms[pos] = new GLfloat[4];
	init_vector(v_norms[pos]);
	// initalize vertex face count
	vert_faces[pos] = 0;
	
	pos = verts_size - 1;
	// allocate a new vertex
	v = new GLfloat[4];
	// set vertex at south pole
	v[0] = 0;
	v[1] = -height;
	v[2] = 0;
	v[3] = 1;
	// store in verts array
	verts[pos] = v;
	// initialize vertex normal
	v_norms[pos] = new GLfloat[4];
	init_vector(v_norms[pos]);
	// initalize vertex face count
	vert_faces[pos] = 0;
	
	
	// FACES + NORMALS
	/////////////////////////////////////////////
	
	// position of face in array
	int f_pos;
	
	// first loop sets the body
	for( int i = 0; i < vs; i++ )
	{
		for( int j = 0; j < 2*(rs-1); j+= 2 )
		{
			// calculate position in the array
			pos = i * rs + j/2;
			pos2 = (i + 1) * rs +j/2;
			pos3 = pos2 + 1;
			f_pos = i * 2 * rs + j;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
			f_pos++;
			pos2++;
			pos3 = pos + 1;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
		}
		
		// calculate position in the array
		pos = (i * rs)+ rs - 1;
		pos2 = (i + 1) * rs + rs - 1;
		pos3 = (i + 1) * rs;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
		
		f_pos++;
		pos2 = (i+1) * rs;
		pos3 = i * rs;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// second loop sets the top endcap
	////////////////////////////////////////
	for( int i = 0; i < rs - 1; i++ )
	{
		// set positions in arrays
		pos = verts_size-2;
		pos2 = i;
		pos3 = i + 1;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// set positions in arrays
	pos = verts_size-2;
	pos2 = pos3;
	pos3 = 0;
	f_pos++;
	
	set_face(f_pos, pos, pos2, pos3, vert_faces);
	
	// third loop sets the bottom endcap
	////////////////////////////////////////
	for( int i = 0; i < rs - 1; i++ )
	{
		// set positions in arrays
		pos = verts_size-1;
		pos3 = (vs) * rs + i;
		pos2 = pos3 + 1;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// set positions in arrays
	pos = verts_size-1;
	pos2 = pos3;
	pos3 = (vs) * rs;
	f_pos++;
	
	set_face(f_pos, pos, pos2, pos3, vert_faces);
	
	// VERTEX NORMALS
	/////////////////////////////////////////////
	
	for( int i = 0; i < verts_size; i++ )
	{
		normalize(v_norms[i]);
	}
	
	v_norms[verts_size-2][0] = 0.0;
	v_norms[verts_size-2][1] = 1.0;
	v_norms[verts_size-2][2] = 0.0;
	v_norms[verts_size-2][3] = 1.0;
	
	v_norms[verts_size-1][0] = 0.0;
	v_norms[verts_size-1][1] = -1.0;
	v_norms[verts_size-1][2] = 0.0;
	v_norms[verts_size-1][3] = 1.0;
}

Cone :: Cone(float r, float h, int vs, int rs)
{
	// set radius
	radius = r;
	height = h/2.0;
	
	// set stack_y interval
	float stack_y = h / vs;
	//set stack_x interval
	float stack_x = r/ vs;
	// set slice interval
	float slice = (2 * M_PI) / rs;
	
	// define the number of vertices
	verts_size = (vs - 1) * (rs) + 2;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = (vs - 1) * rs * 2;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	// define to keep track of faces 
	// for vertex normal calculation
	int vert_faces[verts_size];
	
	// VERTICES
	/////////////////////////////////////////////
	
	GLfloat* v;
	
	// current position on the y-axis for stack
	float curr_stack_y;
	// current position on the x-axis for stack
	float curr_stack_x;
	// current angle of rotation for slice
	float curr_slice;
	// current positions in the array
	int pos, pos2, pos3;
	
	// loop to set "body" vertices
	for( int i = 0; i < vs - 1; i ++ )
	{
		// calculate stack angle
		curr_stack_y = -(i+1) * stack_y;
		curr_stack_x = i * stack_x;
		
		for( int j = 0; j < rs; j++ )
		{
			// calculate slice angle
			curr_slice = j * slice;
			
			// calculate position in the array
			pos = i * rs + j;
			
			// allocate a new vertex
			v = new GLfloat[4];
			// set vertex at north pole
			v[0] = 0;
			v[1] = height;
			v[2] = 0;
			v[3] = 1;
			
			// rotate by stack angle
			v_translate(v, curr_stack_x, curr_stack_y, 0.0);
			// rotate by slice angle
			v_rotate_y(v, curr_slice);
			
			// store in verts array
			verts[pos] = v;
			// initialize vertex normal
			v_norms[pos] = new GLfloat[4];
			init_vector(v_norms[pos]);
			// initalize vertex face count
			vert_faces[pos] = 0;
		}
	}
	
	pos = verts_size - 2;
	// allocate a new vertex
	v = new GLfloat[4];
	// set vertex at north pole
	v[0] = 0;
	v[1] = r;
	v[2] = 0;
	v[3] = 1;
	// store in verts array
	verts[pos] = v;
	// initialize vertex normal
	v_norms[pos] = new GLfloat[4];
	init_vector(v_norms[pos]);
	// initalize vertex face count
	vert_faces[pos] = 0;
	
	pos = verts_size - 1;
	// allocate a new vertex
	v = new GLfloat[4];
	// set vertex at south pole
	v[0] = 0;
	v[1] = -r;
	v[2] = 0;
	v[3] = 1;
	// store in verts array
	verts[pos] = v;
	// initialize vertex normal
	v_norms[pos] = new GLfloat[4];
	init_vector(v_norms[pos]);
	// initalize vertex face count
	vert_faces[pos] = 0;
	
	
	// FACES + NORMALS
	/////////////////////////////////////////////
	
	// position of face in array
	int f_pos;
	
	// first loop sets the body
	for( int i = 0; i < vs - 2; i++ )
	{
		for( int j = 0; j < 2*(rs-1); j+= 2 )
		{
			// calculate position in the array
			pos = i * rs + j/2;
			pos2 = (i + 1) * rs +j/2;
			pos3 = pos2 + 1;
			f_pos = i * 2 * rs + j;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
			f_pos++;
			pos2++;
			pos3 = pos + 1;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
		}
		
		// calculate position in the array
		pos = (i * rs)+ rs - 1;
		pos2 = (i + 1) * rs + rs - 1;
		pos3 = (i + 1) * rs;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
		
		f_pos++;
		pos2 = (i+1) * rs;
		pos3 = i * rs;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// second loop sets the top endcap
	////////////////////////////////////////
	for( int i = 0; i < rs - 1; i++ )
	{
		// set positions in arrays
		pos = verts_size-2;
		pos2 = i;
		pos3 = i + 1;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// set positions in arrays
	pos = verts_size-2;
	pos2 = pos3;
	pos3 = 0;
	f_pos++;
	
	set_face(f_pos, pos, pos2, pos3, vert_faces);
	
	// third loop sets the bottom endcap
	////////////////////////////////////////
	for( int i = 0; i < rs - 1; i++ )
	{
		// set positions in arrays
		pos = verts_size-1;
		pos3 = (vs - 2) * rs + i;
		pos2 = pos3 + 1;
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// set positions in arrays
	pos = verts_size-1;
	pos2 = pos3;
	pos3 = (vs - 2) * rs;
	f_pos++;
	
	set_face(f_pos, pos, pos2, pos3, vert_faces);
	
	// VERTEX NORMALS
	/////////////////////////////////////////////
	
	// loop through vertices
	for( int i = 0; i < verts_size; i++ )
	{
		v_norms[i][0] = v_norms[i][0] / vert_faces[i];
		v_norms[i][1] = -v_norms[i][1] / vert_faces[i];
		v_norms[i][2] = v_norms[i][2] / vert_faces[i];
		v_norms[i][3] = 1.0;
		normalize(v_norms[i]);
	}
	
	v_norms[verts_size-2][0] = 0.0;
	v_norms[verts_size-2][1] = 1.0;
	v_norms[verts_size-2][2] = 0.0;
	v_norms[verts_size-2][3] = 1.0;
	
	v_norms[verts_size-1][0] = 0.0;
	v_norms[verts_size-1][1] = -1.0;
	v_norms[verts_size-1][2] = 0.0;
	v_norms[verts_size-1][3] = 1.0;
	
}

Torus :: Torus(float r, float r2, int vs, int rs)
{
	// set radii
	radius = r;
	radius2 = r2;
	
	// set stack interval
	float stack = (2 * M_PI) / vs;
	// set slice interval
	float slice = (2 * M_PI) / rs;
	
	// define the number of vertices
	verts_size = vs * rs;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = verts_size * 2;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	// define to keep track of faces 
	// for vertex normal calculation
	int vert_faces[verts_size];
	
	// VERTICES
	/////////////////////////////////////////////
	
	GLfloat* v;
	
	// current angle of rotation for stack
	float curr_stack;
	// current angle of rotation for slice
	float curr_slice;
	// current positions in the array
	int pos, pos2, pos3;
	
	// loop to set vertices
	for( int i = 0; i < vs; i++ )
	{
		// calculate stack angle
		curr_stack = -i * stack;
		
		for( int j = 0; j < rs; j++ )
		{
			// calculate slice angle
			curr_slice = j * slice;
			
			// calculate position in the array
			pos = i * rs + j;
			
			// allocate new vertex
			v = new GLfloat[4];
			v[0] = 0;
			v[1] = r2;
			v[2] = 0;
			v[3] = 1;
			
			// rotate by stack angle
			v_rotate_z(v, curr_stack);
			// translate to edge of the ring
			v_translate(v, r, 0.0, 0.0);
			// rotate by slice angle
			v_rotate_y(v, curr_slice);
			
			// store in verts array
			verts[pos] = v;
			// initialize vertex normal
			v_norms[pos] = new GLfloat[4];
			init_vector(v_norms[pos]);
			// initalize vertex face count
			vert_faces[pos] = 0;
		}
	}
	
	// FACES + NORMALS
	/////////////////////////////////////////////
	
	// position of face in array
	int f_pos;
	
	// first loop sets the body
	for( int i = 0; i < vs; i++ )
	{
		for( int j = 0; j < 2*(rs-1); j+= 2 )
		{
			// calculate position in the array
			pos = i * rs + j/2;
			if(i == vs-1)
			{
				pos2 = j/2;
			}
			else
			{
				pos2 = (i + 1) * rs +j/2;
			}
			pos3 = pos2 + 1;
			f_pos = i * 2 * rs + j;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
			f_pos++;
			pos2++;
			pos3 = pos + 1;
			
			set_face(f_pos, pos, pos2, pos3, vert_faces);
			
		}
		
		// calculate position in the array
		pos = (i * rs)+ rs - 1;
		if(i == vs-1)
		{
			pos2 = rs - 1;
			pos3 = 0;
		}
		else
		{
			pos2 = (i + 1) * rs + rs - 1;
			pos3 = (i + 1) * rs;
		}
		f_pos++;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
		
		f_pos++;
		if(i == vs-1)
		{
			pos2 = 0;
		}
		else
		{
			pos2 = (i+1) * rs;
		}
		pos3 = i * rs;
		
		set_face(f_pos, pos, pos2, pos3, vert_faces);
	}
	
	// VERTEX NORMALS
	/////////////////////////////////////////////
	
	// loop through vertices
	for( int i = 0; i < verts_size; i++ )
	{
		v_norms[i][0] = v_norms[i][0] / vert_faces[i];
		v_norms[i][1] = v_norms[i][1] / vert_faces[i];
		v_norms[i][2] = v_norms[i][2] / vert_faces[i];
		v_norms[i][3] = 1.0;
		normalize(v_norms[i]);
	}
	
}

House :: House(float my_size)
{
	// adjust size to form around the origin
	h = my_size/2.0;
	float h2 = my_size*(1.0/2.0);
	
	// define number of vertices
	verts_size = 9;
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces_size = 14;
	faces = new int*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
	
	// VERTICES
	/////////////////////////////////////////////
	
	// front-top-right corner
	make_vert(0, -h, h, h);
	
	// front-bottom-right corner
	make_vert(1, -h, -h, h);
	
	// front-bottom-left corner
	make_vert(2, h, -h, h);
	
	// front-top-left corner
	make_vert(3, h, h, h);
	
	// back-top-right corner
	make_vert(4, -h, h, -h);
	
	// back-bottom-right corner
	make_vert(5, -h, -h, -h);
	
	// back-bottom-left corner
	make_vert(6, h, -h, -h);
	
	// back-top-left corner
	make_vert(7, h, h, -h);
	
	// top of the roof
	make_vert(8, 0, h+h2, 0);
	
	// FACES + NORMALS
	/////////////////////////////////////////////
	
	// front face 1
	make_face(0, 3, 2, 1);
	
	// front face 2
	make_face(1, 3, 1, 0);
	
	// left face 1
	make_face(2, 7, 6, 2);
	
	// left face 2
	make_face(3, 7, 2, 3);
	
	// back face 1
	make_face(4, 4, 5, 6);
	
	// back face 2
	make_face(5, 4, 6, 7);
	
	// right face 1
	make_face(6, 0, 1, 5);
	
	// right face 2
	make_face(7, 0, 5, 4);
	
	// top face 1
	make_face(8, 7, 3, 8);
	
	// top face 2
	make_face(9, 3, 0, 8);
	
	// top face 3
	make_face(10, 0, 4, 8);
	
	// top face 4
	make_face(11, 4, 7, 8);
	
	// bottom face 1
	make_face(12, 2, 6, 5);
	
	// bottom face 2
	make_face(13, 2, 5, 1);
	
	// VERTEX NORMALS
	/////////////////////////////////////////////
	// NOTE: The cube vertices have a varying
	// number of faces (some of which have the same normal).
	// This makes calculating the normals difficult.
	// Because there are only 8 vertices,
	// it is simple enough to just hard code them in.
	
	v_norms[0][0] = -1.0/3.0;
	v_norms[0][1] = 1.0/3.0;
	v_norms[0][2] = 1.0/3.0;
	v_norms[0][3] = 1.0;
	
	v_norms[1][0] = -1.0/3.0;
	v_norms[1][1] = -1.0/3.0;
	v_norms[1][2] = 1.0/3.0;
	v_norms[1][3] = 1.0;
	
	v_norms[2][0] = 1.0/3.0;
	v_norms[2][1] = -1.0/3.0;
	v_norms[2][2] = 1.0/3.0;
	v_norms[2][3] = 1.0;
	
	v_norms[3][0] = 1.0/3.0;
	v_norms[3][1] = 1.0/3.0;
	v_norms[3][2] = 1.0/3.0;
	v_norms[3][3] = 1.0;
	
	v_norms[4][0] = -1.0/3.0;
	v_norms[4][1] = 1.0/3.0;
	v_norms[4][2] = -1.0/3.0;
	v_norms[4][3] = 1.0;
	
	v_norms[5][0] = -1.0/3.0;
	v_norms[5][1] = -1.0/3.0;
	v_norms[5][2] = -1.0/3.0;
	v_norms[5][3] = 1.0;
	
	v_norms[6][0] = 1.0/3.0;
	v_norms[6][1] = -1.0/3.0;
	v_norms[6][2] = -1.0/3.0;
	v_norms[6][3] = 1.0;
	
	v_norms[7][0] = 1.0/3.0;
	v_norms[7][1] = 1.0/3.0;
	v_norms[7][2] = -1.0/3.0;
	v_norms[7][3] = 1.0;
	
	v_norms[8][0] = 0.0;
	v_norms[8][1] = 1.0;
	v_norms[8][2] = 0.0;
	v_norms[8][3] = 1.0;

}

void House :: make_vert(int vert_num, float x, float y, float z)
{
	// allocate new vertex
	GLfloat* v = new GLfloat[4];
	
	// set inital values
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = 1;
	// place into verts array
	verts[vert_num] = v;
	
	// initalize vertex normal array
	v_norms[vert_num] = new GLfloat[4];
	init_vector(v_norms[vert_num]);
}

void House :: make_face(int face_num, int v1, int v2, int v3)
{
	// allocate new face
	int* f = new int[3];
	// allocate new face normal
	GLfloat* fn = new GLfloat[4];
	init_vector(fn);
	
	// set face vertices
	f[0] = v1;
	f[1] = v2;
	f[2] = v3;
	// place into faces array
	faces[face_num] = f;
	
	// calculate face normal
	GLfloat vector1[4], vector2[4];
	vv_sub(verts[v2], verts[v1], vector1);
	vv_sub(verts[v2], verts[v3], vector2);
	vv_cross(vector1, vector2, fn);
	normalize(fn);
	// place into face normals array
	f_norms[face_num] = fn;
}

void Shape3D :: set_face(int f_pos, int pos, int pos2, int pos3, int vert_faces[])
{
	// allocate new face
	faces[f_pos] = new int[3];
	
	faces[f_pos][0] = pos;
	faces[f_pos][1] = pos2;
	faces[f_pos][2] = pos3;
	
	// calculate face normal
	calc_face_normal(f_pos);

	// add to vertex normals
	// update vertex face counts
	
	vv_add(v_norms[pos],f_norms[f_pos], v_norms[pos]);
	vert_faces[pos]++;
	
	vv_add(v_norms[pos2], f_norms[f_pos], v_norms[pos2]);
	vert_faces[pos2]++;
	
	vv_add(v_norms[pos3], f_norms[f_pos], v_norms[pos3]);
	vert_faces[pos3]++;
	 
}

void Shape3D :: calc_face_normal(int face_index)
{
	GLfloat* fn = new GLfloat[4];
	// calculate face normal
	GLfloat vector1[4], vector2[4];
	vv_sub(verts[faces[face_index][0]], verts[faces[face_index][1]], vector1);
	vv_sub(verts[faces[face_index][2]], verts[faces[face_index][1]], vector2);
	vv_cross(vector2, vector1, fn);
	normalize(fn);
	// place into face normals array
	f_norms[face_index] = fn;
}

void Mesh :: load_mesh(char* the_mesh)
{
	FILE* fp;
	char buffer[300];
	int status = 0;
	int v = 0;
	int f = 0;
	int* vert_faces;
	
	fp = fopen(the_mesh, "r+");
	if(fp == NULL)
	{
		cout << "Error reading file: " << the_mesh << endl;
		return;
	}
	while(!feof(fp))
	{
		fgets(buffer, 300, fp);
		switch(buffer[0])
		{
			case '#':
				break;
			case 'v':
				status++;
				break;
			case 'f':
				status++;
				break;
			default:
				if(status == 0)
				{
					parse_count(buffer, vert_faces);
					vert_faces = new int[verts_size];
					for( int i = 0; i < verts_size; i++ )
					{
						v_norms[i] = new GLfloat[4];
						v_norms[i][0] = 0.0;
						v_norms[i][1] = 0.0;
						v_norms[i][2] = 0.0;
						v_norms[i][3] = 1.0;
						vert_faces[i] = 0;
					}
				}
				else if(status == 1)
				{
					parse_vert(buffer, v);
					v++;
				}
				else if(status == 2)
				{
					parse_face(buffer, f, vert_faces);
					f++;
				}
				break;
				
				for( int i = 0; i < verts_size; i++ )
				{
					v_norms[i][0] = v_norms[i][0] / vert_faces[i];
					v_norms[i][1] = v_norms[i][1] / vert_faces[i];
					v_norms[i][2] = v_norms[i][2] / vert_faces[i];
					v_norms[i][3] = 1.0;
					normalize(v_norms[i]);
				}
		}
	}
}

void Mesh :: parse_count(char buffer[], int* vert_faces)
{
	char* token;
	token = strtok(buffer, " ");
	verts_size = atoi(token);
	token = strtok(NULL, " ");
	faces_size = atoi(token);
	
	// define the number of verts
	verts = new GLfloat*[verts_size];
	
	// define the number of faces
	faces = new int*[faces_size];
	face_colors = new float*[faces_size];
	
	// define arrays for normals
	v_norms = new GLfloat*[verts_size];
	f_norms = new GLfloat*[faces_size];
}

void Mesh :: parse_vert(char buffer[], int i)
{
	char* token;
	token = strtok(buffer, " ");
	verts[i] = new GLfloat[4];
	verts[i][0] = atof(token);
	token = strtok(NULL, " ");
	verts[i][1] = atof(token);
	token = strtok(NULL, " ");
	verts[i][2] = atof(token);
	token = strtok(NULL, " ");
	verts[i][3] = 1.0;
}

void Mesh :: parse_face(char buffer[], int i, int* vert_faces)
{
	char* token;
	int pos, pos2, pos3;
	float red, green, blue, speed;
	token = strtok(buffer, " ");
	faces[i] = new int[3];
	pos = atoi(token);
	faces[i][0] = pos;
	token = strtok(NULL, " ");
	pos2 = atoi(token);
	faces[i][1] = pos2;
	token = strtok(NULL, " ");
	pos3 = atoi(token);
	faces[i][2] = pos3;
	face_colors[i] = new float[4];
	token = strtok(NULL, " ");
	face_colors[i][0] = atof(token);
	token = strtok(NULL, " ");
	face_colors[i][1] = atof(token);
	token = strtok(NULL, " ");
	face_colors[i][2] = atof(token);
	token = strtok(NULL, " ");
	face_colors[i][3] = atof(token);
	if(face_colors[i][3] > 0)
	{
		token = strtok(NULL, " ");
		red = atof(token);
		token = strtok(NULL, " ");
		green = atof(token);
		token = strtok(NULL, " ");
		blue = atof(token);
		token = strtok(NULL, " ");
		speed = atof(token);
		set_flashing(i, red, green, blue, speed);
	}
	
	// calculate face normal
	calc_face_normal(i);
	
	// add to vertex normals
	// update vertex face counts
	
	vv_add(v_norms[pos],f_norms[i], v_norms[pos]);
	vert_faces[pos]++;
	
	vv_add(v_norms[pos2], f_norms[i], v_norms[pos2]);
	vert_faces[pos2]++;
	
	vv_add(v_norms[pos3], f_norms[i], v_norms[pos3]);
	vert_faces[pos3]++;
}

void Mesh :: draw_mesh(int draw_mode, int shade_mode)
{
	if(shade_mode == GL_FLAT)
	{
	for( int i = 0; i < faces_size; i++ )
	{
		if(face_colors[i][3] > 0)
		{
			glDisable(GL_LIGHTING);
		}
		glColor3fv(face_colors[i]);
		glBegin(draw_mode);
		glNormal3fv(f_norms[i]);
		glVertex4fv(verts[faces[i][0]]);
		glNormal3fv(f_norms[i]);
		glVertex4fv(verts[faces[i][1]]);
		glNormal3fv(f_norms[i]);
		glVertex4fv(verts[faces[i][2]]);
		glEnd();
		if(face_colors[i][3] > 0)
		{
			glEnable(GL_LIGHTING);
		}
	}
	}
	else
	{
		for( int i = 0; i < faces_size; i++ )
		{
		if(face_colors[i][3] > 0)
		{
			glDisable(GL_LIGHTING);
		}
		glColor3fv(face_colors[i]);
		glBegin(draw_mode);
		glNormal3fv(v_norms[faces[i][0]]);
		glVertex4fv(verts[faces[i][0]]);
		glNormal3fv(v_norms[faces[i][1]]);
		glVertex4fv(verts[faces[i][1]]);
		glNormal3fv(v_norms[faces[i][2]]);
		glVertex4fv(verts[faces[i][2]]);
		glEnd();
		if(face_colors[i][3] > 0)
		{
			glEnable(GL_LIGHTING);
		}
		}
	}

	/*
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	for( int i = 0; i < verts_size; i++ )
	{
		glVertex4fv(verts[i]);
		glVertex4f(verts[i][0] + v_norms[i][0], verts[i][1] + v_norms[i][1], verts[i][2] + v_norms[i][2], 1.0);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	 */
}

void Mesh :: set_flashing(int face_num, float r, float g, float b, float speed)
{
	flash_face* my_f = new flash_face;
	my_f->rstart = face_colors[face_num][0];
	my_f->gstart = face_colors[face_num][1];
	my_f->bstart = face_colors[face_num][2];
	my_f->face_num = face_num;
	my_f->rend = r;
	my_f->gend = g;
	my_f->bend = b;
	my_f->r = r/speed;
	my_f->g = g/speed;
	my_f->b = b/speed;
	flashing.push_front(my_f);
}

void Mesh :: update()
{
	int j = 0;
	for(int i = 0; i < flashing.size(); i++)
	{
		j = flashing[i]->face_num;
		face_colors[j][0] += flashing[i]->r;
		face_colors[j][1] += flashing[i]->g;
		face_colors[j][2] += flashing[i]->b;
		if( face_colors[j][0] >= flashing[i]->rend && face_colors[j][1] >= flashing[i]->gend && face_colors[j][3] >= flashing[i]->bend)
		{
			face_colors[j][0] = flashing[i]->rstart;
			face_colors[j][1] = flashing[i]->gstart;
			face_colors[j][2] = flashing[i]->bstart;
		}
	}
}

//added by tyler for code compatibility
GLfloat *Shape3D::getPosition()
{
	GLfloat *ret = new GLfloat[3];
	ret[0] = this->local_p[0];
	ret[1] = this->local_p[1];
	ret[2] = this->local_p[2];
	
	return ret;
}