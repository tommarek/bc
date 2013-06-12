#ifndef DRAW_H
#define DRAW_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "demo/scene/scene.h"

// Location/Normals
#define X_POS 0
#define Y_POS 1
#define Z_POS 2
// Texture Coordinates
#define U_POS 0
#define V_POS 1
// Colours
#define R_POS 0
#define G_POS 1
#define B_POS 2
#define A_POS 3

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

extern GLuint depthTextureId;
extern GLfloat shadowMapWidthStep, shadowMapHeightStep, lightPos[3], lightDir[3];

void drawGLScene(struct S_sceneNode *sceneNode);
int initGL(GLvoid);

#endif // DRAW_H
