#ifndef _SCENEOPERATIONS_H_
#define _SCENEOPERATIONS_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include "framework/scenemanager/scenemanager.h"

void rotateNode(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z);
void rotateNodeDeg(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z);
void translateNode(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z);
void scaleNode(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z);
void storeNoRotMatrix(struct S_sceneNode *node);


void lookAt(struct S_sceneNode *node, GLfloat *pos, GLfloat *tar, GLfloat *up);
void setPerspective(struct S_sceneNode *node, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
void setPerspectiveDeg(struct S_sceneNode *node, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
void useCamera(struct S_sceneNode *node);

void setIdentity(struct S_sceneNode *node);

void renderSceneNode(struct S_sceneNode *node);
#endif