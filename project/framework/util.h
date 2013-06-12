#ifndef UTIL_H
#define UTIL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#define False 0
#define True 1
#define M_PI 3.14159265358979323846

#ifdef NDEBUG
        #define error(args...) _exit(1)
        #define print(args...)
#else
        #include <stdio.h>
        // errors
        #define error(args...)\
                do{\
                        fprintf(stderr, args);\
                        _exit(1);\
                }while(0)
        // printf
        #define print(args...)\
                do{\
                        printf(args);\
                }while(0)
#endif

// macros for vector operations
#define VectorNegate(a,b) ((b)[0]=-((a)[0]),(b)[1]=-((a)[1]),(b)[2]=-((a)[2]))
#define VectorSet(v,x,y,z) ((v)[0]=(x),(v)[1]=(y),(v)[2]=(z))
#define VectorClear(a) ((a)[0]=(a)[1]=(a)[2]=0)
#define DotProduct(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VectorSubtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c) ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorMultiply(a,b,c) ((c)[0]=(a)[0]*(b)[0],(c)[1]=(a)[1]*(b)[1],(c)[2]=(a)[2]*(b)[2])
#define CrossProduct(a,b,c) ((c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1],(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2],(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0])
#define VectorNormalize(v) {float ilength = (float) mySqrt(DotProduct(v,v));if (ilength) ilength = 1.0f / ilength;v[0] *= ilength;v[1] *= ilength;v[2] *= ilength;}
#define VectorNormalize2(v,dest) {float ilength = (float) mySqrt(DotProduct(v,v));if (ilength) ilength = 1.0f / ilength;dest[0] = v[0] * ilength;dest[1] = v[1] * ilength;dest[2] = v[2] * ilength;}
#define VectorDistance2(a, b) (((a)[0] - (b)[0]) * ((a)[0] - (b)[0]) + ((a)[1] - (b)[1]) * ((a)[1] - (b)[1]) + ((a)[2] - (b)[2]) * ((a)[2] - (b)[2]))
#define VectorDistance(a, b) (mySqrt(VectorDistance2(a,b)))
#define VectorScale(a, b, c) ((c)[0]=(a)[0]*(b),(c)[1]=(a)[1]*(b),(c)[2]=(a)[2]*(b))
#define VectorLength(v) (mySqrt((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2]))
#define VectorInterpolate(v1, v2, d, v) ((v)[0]=(v1)[0]+((v2)[0]-(v1)[0])*(d),(v)[1]=(v1)[1]+((v2)[1]-(v1)[1])*(d),(v)[2]=(v1)[2]+((v2)[2]-(v1)[2])*(d))
// macro for linear interpolation
#define Lerp(a,b,t) ((1.0-(t))*(a) + ((t)*(b)))

// global time value
extern GLuint G_time;

void _exit(int);
void *checkMalloc(size_t);
void *checkRealloc(void *, size_t);
void *checkStrdup(void *, size_t);
void myMemset(void *s, GLint c, size_t size);
void myMemcpy(void *dest, void *source, size_t size);

GLfloat deg2rad(GLfloat deg);

GLfloat sinf(GLfloat angle);
GLfloat cosf(GLfloat angle);
GLfloat tanf(GLfloat angle);

GLfloat mySqrt(GLfloat val);
GLfloat myFabs(GLfloat val);

void multMatrices(GLfloat *matrix1, GLfloat *matrix2, GLfloat *result);

#endif // UTIL_H
