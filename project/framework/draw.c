/**
\file
 Main draw file - shadows, rendering , OpenGL init, etc.
*/
#include "draw.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "framework/texturemanager/texturemanager.h"
#include "framework/meshmanager/meshmanager.h"
#include "framework/materialmanager/materialmanager.h"
#include "framework/scenemanager/scenemanager.h"
#include "framework/scenemanager/sceneoperations.h"
#include "framework/sequencemanager/timer.h"
#include "framework/window.h"
#include "framework/util.h"
#include "framework/extensions.h"

#define SHADOWMAPRATIO 2
// Hold id of the framebuffer for light POV rendering
GLuint fboId;
// Z values will be rendered to this texture when using fboId framebuffer
GLuint depthTextureId;
// shadow map width and height to be passed into shader
GLfloat shadowMapWidthStep;
GLfloat shadowMapHeightStep;
// light and camera position passed to shader
GLfloat lightPos[3];
GLfloat lightDir[3];

#ifndef NOSHADOW
/**
 * Create shadow frame buffer
 */
static void generateShadowFBO(){
	// set shadow map size
	GLint shadowMapWidth = window.width * SHADOWMAPRATIO;
	GLint shadowMapHeight = window.height * SHADOWMAPRATIO;

	// set steps
	shadowMapWidthStep = 1.0 / (shadowMapWidth / 2);
	shadowMapHeightStep = 1.0 / (shadowMapHeight / 2);

	// Try to use a texture depth component
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);

	// GL_LINEAR does not make sense for depth texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artefact on the edges of the shadowmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	MYglGenFramebuffersEXT(1, &fboId);
	MYglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	MYglFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depthTextureId, 0);

	// check FBO status
	GLenum FBOstatus = MYglCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

	// switch back to window-system-provided framebuffer
	MYglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

/**
 * Generate texgen matrix for shadow mapping
 */
static void setTextureMatrix(){
	static GLfloat modelView[16];
	static GLfloat projection[16];

	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5
	// y = y* 0.5 + 0.5
	// z = z* 0.5 + 0.5
	// Moving from unit cube [-1,1] to [0,1]
	const GLfloat bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0};

	// Grab modelview and transformation matrices
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);


	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);

	glLoadIdentity();
	glLoadMatrixf(bias);

	// concatating all matrice into one.
	glMultMatrixf(projection);
	glMultMatrixf(modelView);

	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}
#endif

/**
 * general OpenGL initialization function
 */
int initGL(){
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);     // Hidden surface removal
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glFrontFace(GL_CCW);         // Counterclockwise polygons face out
	glEnable(GL_CULL_FACE);      // Do not calculate inside collisions
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_COLOR_MATERIAL);

	// set global ambient lighting
	GLfloat GlobalAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GlobalAmbient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); // light 1 will be used only
#ifndef NOSHADOW
	generateShadowFBO();
#endif
	// white
	glEnable(GL_NORMALIZE);

	glFlush();

	return True;
}



/**
 * drawing function
 */
void drawGLScene(struct S_sceneNode *sceneNode){
	// set lights according to root node and camrea
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_AMBIENT, ((struct S_rootContent *)sceneNode->content)->light.lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ((struct S_rootContent *)sceneNode->content)->light.lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, ((struct S_rootContent *)sceneNode->content)->light.lightPosition);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, ((struct S_rootContent *)sceneNode->content)->light.lightDirection);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ((struct S_rootContent *)sceneNode->content)->light.lightSpecular);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, ((struct S_rootContent *)sceneNode->content)->light.lightCutOff);

#ifndef NOSHADOW
	// draw shadows
	((struct S_rootContent *)sceneNode->content)->isShadowRendering = True;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MYglBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0,0,window.width * SHADOWMAPRATIO,window.height * SHADOWMAPRATIO);
	glCullFace(GL_FRONT);

	// set matrices
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90 ,window.width / window.height, 10, 40000);
	gluLookAt(((struct S_rootContent *)sceneNode->content)->light.lightPosition[0], ((struct S_rootContent *)sceneNode->content)->light.lightPosition[1],((struct S_rootContent *)sceneNode->content)->light.lightPosition[2],
		((struct S_rootContent *)sceneNode->content)->light.lightTargetPoint[0], ((struct S_rootContent *)sceneNode->content)->light.lightTargetPoint[1], ((struct S_rootContent *)sceneNode->content)->light.lightTargetPoint[2],
		0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// draw scene
	renderSceneNode(sceneNode);
	// store texgen matrix
	setTextureMatrix();


	// draw scene
	MYglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
	glViewport(0, 0, window.width, window.height);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);


	((struct S_rootContent *)sceneNode->content)->isShadowRendering = False;
	// use camera
	useCamera(((struct S_rootContent *)sceneNode->content)->activeCamera);
	// render node
	renderSceneNode(sceneNode);
}
