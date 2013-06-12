#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "demo/materials/shaders.h"
#include "demo/materials/material.h"

struct S_shader{
        GLuint shaderVert, shaderFrag;
        GLuint program;

        const char *sourceVert;
        const char *sourceFrag;
};

struct S_material{
        GLfloat color[4];

        GLfloat shininess;

        GLfloat ambient[4];
        GLfloat specular[4];

        struct S_shader *shader;
        
        GLuint textureID[9];
        GLint textueCount;

};

extern struct S_material *materials[];
extern struct S_shader *shaders[];

void initMaterialManager();
void bindMaterial(struct S_material *, GLint isShadowRendering);
void createShader(struct S_shader *shader);


#endif