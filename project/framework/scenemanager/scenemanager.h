#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include "bezier.h"

#include "demo/meshes/mesh.h"
#include "demo/scene/scene.h"

enum E_OBJECT_TYPE {
        E_OT_ROOT = 0,
	E_OT_MESH,
	E_OT_CAMERA,
        E_OT_LIGHT,
        E_OT_CURVE,
        E_OT_DUMMY,
};


struct S_sceneNode;
typedef void (*funcRenderCallback)(struct S_sceneNode *node);

struct S_sceneNode{
	GLfloat modelview[16];
	GLfloat noRotModelview[16];
	GLint nodeType;
	GLint visible;

        // render callback function
	funcRenderCallback renderCallback;

        // content is different for each type of node
        void *content;

        struct S_sceneNode *children;
	struct S_sceneNode *next;

        struct S_sceneNode *parent;
        struct S_sceneNode *root;

        // noise movement of model -- optional
        GLint noisePos;

};

struct S_light{
        GLfloat lightAmbient[4];
        GLfloat lightDiffuse[4];
        GLfloat lightSpecular[4];

        GLfloat lightPosition[4];
        GLfloat lightDirection[4];
        GLfloat lightTargetPoint[3];

        GLfloat lightCutOff;
};

struct S_rootContent{
        struct S_sceneNode *activeCamera;
        // True if rendering is set to shadow
        GLint isShadowRendering;
        // scene has only one light
        struct S_light light;
};

struct S_cameraContent{
        GLfloat projectionMatrix[16];

        GLfloat fov, nearClip, farClip;
        GLfloat pos[3], tar[3], up[3];

        GLuint startTime, stopTime;
        struct S_bezier *lookAtCurve;

};

struct S_meshContent{
        GLint meshID;
        GLint materialID;
};

struct S_curveContent{
        struct S_bezier *curve;
        struct S_bezier *speed;
        GLuint startTime;
        GLuint stopTime;
};



void deleteScene(struct S_sceneNode *root);

struct S_sceneNode *createRoot();
struct S_sceneNode *createCamera(struct S_sceneNode *parent, GLfloat fov, GLfloat nearClip, GLfloat farClip, GLfloat *up, struct S_bezier *lookatCurve, GLuint startTime, GLuint stopTime);
struct S_sceneNode *createMesh(struct S_sceneNode *parent, GLint meshID, GLint materialID);
struct S_sceneNode *createDummy(struct S_sceneNode *parent);
struct S_sceneNode *createCurve(struct S_sceneNode *parent, struct S_bezier *curve, struct S_bezier *speed, GLuint startTime, GLuint stopTime);
void setLight(struct S_sceneNode *root, GLfloat *ambient, GLfloat *difuse, GLfloat *specular, GLfloat *position, GLfloat *targetPoint, GLfloat lightCutOff);

void initSceneManger();

#endif //_SCENEMANAGER_H_
