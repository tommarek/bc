#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <GL/gl.h>
#include <GL/glu.h>

struct S_bezier{
        GLfloat *controlPoints;
        GLfloat *helpPoints;
        
        GLuint controlPointsCount;
        GLuint maxPointsCount;
};



extern struct S_bezier curves[];

struct S_bezier *createBezier(GLuint pointsCount);
void bezierAddControlPoint(struct S_bezier *curve, GLfloat x, GLfloat y, GLfloat z);
void bezierGetPoint(struct S_bezier *curve, GLfloat time, GLfloat *result);
void bezierGetPointRealTimeLinear(struct S_bezier *curve, GLint realTime, GLint start, GLint stop, GLfloat *result);

void drawCurve(struct S_bezier* curve);
#endif