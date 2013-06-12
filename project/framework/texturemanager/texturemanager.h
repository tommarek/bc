#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "demo/textures/texture.h"

#define MIN(a, b) (a < b ? 1 : 0)
#define MAX(a, b) (a > b ? 1 : 0)

struct S_texture{
	GLint textureWidth, textureHeight;
	GLfloat *tex;
	GLuint ID;
};

extern struct S_texture textura;

void colorize(struct S_texture *, GLuint, GLuint);
void generateSinePlasma(struct S_texture *, GLint, GLint, GLint, GLint);
void generateGradientSphere(struct S_texture *, GLint, GLint);
void generateValueNoise(struct S_texture *, GLint, GLfloat);
void generateMultioctaveValueNoise(struct S_texture *, GLint, GLfloat);

void distortSine(struct S_texture *, GLfloat, GLint, GLint);
void distortMarble(struct S_texture *, GLfloat, GLfloat, GLfloat);
void distortMap(struct S_texture *, struct S_texture *, GLfloat);

void opAdd(struct S_texture *, struct S_texture *);
void opSub(struct S_texture *, struct S_texture *);
void opMul(struct S_texture *, struct S_texture *);
void opDiv(struct S_texture *, struct S_texture *);
void opNeg(struct S_texture *);
void opEmboss(struct S_texture *);
void opCreateNormalMap(struct S_texture *);

void loadTexture(struct S_texture *);
void destroyTexture(struct S_texture *);

void initTextureManager();


#endif