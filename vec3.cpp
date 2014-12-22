//written by Tyler Raborn

#include "vec3.h"

vec3::vec3()
{

}

vec3::vec3(GLdouble x, GLdouble y, GLdouble z)
{
    this->w=1.0;
    this->x=x;
    this->y=y;
    this->z=z;

    this->length = sqrt(pow(x, 2)+pow(y, 2)+pow(z, 2));
}

vec3::vec3(GLdouble v[3])
{
    this->w=1.0;
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];

    this->length = sqrt(pow(x, 2)+pow(y, 2)+pow(z, 2));
}

vec3::vec3(Vertex a, Vertex b)
{
    this->w=1.0;
    this->x = (a.x - b.x);
    this->y = (a.y - b.y);
    this->z = (a.z - b.z);

    //printf("inside of vec3: \nVertex a = {%f, %f, %f}\nVertex b = {%f, %f, %f}\n", a.x, a.y, a.z, b.x, b.y, b.z);

    this->length = sqrt(pow(x, 2)+pow(y, 2)+pow(z, 2));
}

vec3::vec3(vec3 v, GLdouble M[4][4])
{
    this->w=1.0;
    GLdouble *result = vectorMatrixMult(M);
    this->x = result[0];
    this->y = result[1];
    this->z = result[2];
    
    this->length = sqrt(pow(x, 2)+pow(y, 2)+pow(z, 2));

    delete[] result;
}

//public methods:
GLdouble vec3::dot(vec3 v)
{
    return (
            (this->x * v.x) + 
            (this->y * v.y) + 
            (this->z * v.z)
           );
}

vec3* vec3::cross(vec3 v)
{
    GLdouble temp[3];

    temp[0] = ((this->y*v.z) - (this->z*v.y));
    temp[1] = ((this->z*v.x) - (this->x*v.z));
    temp[2] = ((this->x*v.y) - (this->y*v.x));

    vec3 *ret = new vec3(temp);

    return ret;
}

vec3* vec3::normalize()
{
    GLdouble mag = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    vec3 *ret = new vec3((this->x/mag), (this->y/mag), (this->z/mag));
    return ret;
}

vec3* vec3::multMatrix(GLdouble m[4][4])
{
    GLdouble *curV = vectorMatrixMult(m);
    vec3 *ret = new vec3(curV);
    delete[] curV;
    return ret;
}

GLdouble* vec3::toArray()
{
    GLdouble *ret = new GLdouble[3];
    ret[0]=this->x;
    ret[1]=this->y;
    ret[2]=this->z;
    return ret;
}

//internal functions
GLdouble* vec3::vectorMatrixMult(GLdouble A[4][4])
{
    GLdouble* ret = new GLdouble[4];

    for (int i = 0; i < 4; i++)
    {
        ret[i] = ((A[i][0]*this->x) + (A[i][1]*this->y) + (A[i][2]*this->z) + (A[i][3]*this->w));
    }

    return ret;
}

void vec3::print()
{
    printf("{%g, %g, %g}\n", this->x, this->y, this->z);
}

vec3::~vec3()
{

}
