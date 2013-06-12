/**
\file
 Operations with scene nodes
*/
#include "sceneoperations.h"

#include <assert.h>

#include "framework/scenemanager/scenemanager.h"
#include "framework/sequencemanager/timer.h"
#include "framework/util.h"
#include "framework/window.h"

/**
 * Internal function for rotateNode(). Rotates node around X axis.
 * @param node target node
 * @param angle rotation angle (in rad)
 */
static void rotateX(struct S_sceneNode *node, GLfloat angle){
	GLfloat p;
	GLfloat _sin = sinf(-angle);
	GLfloat _cos = cosf(-angle);

	// X to X axis
	p = node->modelview[4];
	node->modelview[4] = p * _cos - node->modelview[8] * _sin;
	node->modelview[8] = p * _sin + node->modelview[8] * _cos;

	// X to Y axis
	p = node->modelview[5];
	node->modelview[5] = p * _cos - node->modelview[9] * _sin;
	node->modelview[9] = p * _sin + node->modelview[9] * _cos;

	// X to Z axis
	p = node->modelview[6];
	node->modelview[6] = p * _cos - node->modelview[10] * _sin;
	node->modelview[10] = p * _sin + node->modelview[10] * _cos;
}

/**
 * Internal function for rotateNode(). Rotates node around Y axis.
 * @param node target node
 * @param angle rotation angle (in rad)
 */
static void rotateY(struct S_sceneNode *node, GLfloat angle){
	GLfloat p;
	GLfloat _sin = sinf(angle);
	GLfloat _cos = cosf(angle);

	// Y to X axis
	p = node->modelview[0];
	node->modelview[0] = p * _cos - node->modelview[8] * _sin;
	node->modelview[8] = p * _sin + node->modelview[8] * _cos;

	// Y to Y axis
	p = node->modelview[1];
	node->modelview[1] = p * _cos - node->modelview[9] * _sin;
	node->modelview[9] = p * _sin + node->modelview[9] * _cos;

	// Y to Z axis
	p = node->modelview[2];
	node->modelview[2] = p * _cos - node->modelview[10] * _sin;
	node->modelview[10] = p * _sin + node->modelview[10] * _cos;
}

/**
 * Internal function for rotateNode(). Rotates node around Z axis.
 * @param node target node
 * @param angle rotation angle (in rad)
 */
static void rotateZ(struct S_sceneNode *node, GLfloat angle){
	GLfloat p;
	GLfloat _sin = sinf(-angle);
	GLfloat _cos = cosf(-angle);

	// Z to X axis
	p = node->modelview[0];
	node->modelview[0] = p * _cos - node->modelview[4] * _sin;
	node->modelview[4] = p * _sin + node->modelview[4] * _cos;

	// Z to Y axis
	p = node->modelview[1];
	node->modelview[1] = p * _cos - node->modelview[5] * _sin;
	node->modelview[5] = p * _sin + node->modelview[5] * _cos;

	// Z to Z axis
	p = node->modelview[2];
	node->modelview[2] = p * _cos - node->modelview[6] * _sin;
	node->modelview[6] = p * _sin + node->modelview[6] * _cos;
}

/**
 * Rotates node around X, Y and Z axis. This function is not the same as glRotate, it doesn't rotate around [0,0,0] if moved.
 * @param node target node
 * @param X rotation angle around X axis (in rad)
 * @param Y rotation angle around Y axis (in rad)
 * @param Z rotation angle around Z axis (in rad)
 */
void rotateNode(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z){
	if(X != 0)
		rotateX(node, X);
	if(Y != 0)
		rotateY(node, Y);
	if(Z != 0)
		rotateZ(node, Z);
}

/**
 * Wrapper around rotateNode(). Rotations are specified in degrees.
 * @param node target node
 * @param X rotation angle around X axis (in deg)
 * @param Y rotation angle around Y axis (in deg)
 * @param Z rotation angle around Z axis (in deg)
 */
void rotateNodeDeg(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z){
	X = deg2rad(X);
	Y = deg2rad(Y);
	Z = deg2rad(Z);
	rotateNode(node, X, Y, Z);
}

/**
 * Move node to position
 * @param node target node
 * @param X position on X axis
 * @param Y position on Y axis
 * @param Z position on Z axis
 */
void translateNode(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z){
/* also possible
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(node->modelview);
	glTranslatef(X, Y, Z);
	glGetFloatv(GL_MODELVIEW_MATRIX, node->modelview);
	glPopMatrix();
*/
	GLfloat tmp[16] = {1, 0, 0, 0,
			   0, 1, 0, 0,
			   0, 0, 1, 0,
			   X, Y, Z, 1};

	multMatrices(tmp, node->modelview, node->modelview);
}
/**
 * stores actual modelview matrix into NoRot matrix.
 * Used for rotations.
 * @param node target node
 */
void storeNoRotMatrix(struct S_sceneNode *node){
	myMemcpy(node->noRotModelview, node->modelview, 16 * sizeof(GLfloat));
}

/**
 * scale node
 * @param node target node
 * @param X scale on X axis
 * @param Y scale on Y axis
 * @param Z scale on Z axis
 */
void scaleNode(struct S_sceneNode *node, GLfloat X, GLfloat Y, GLfloat Z){
	GLfloat tmp[16] = {X, 0, 0, 0,
			   0, Y, 0, 0,
			   0, 0, Z, 0,
			   0, 0, 0, 1};

	multMatrices(tmp, node->modelview, node->modelview);
}


/**
 * Sets rotation according to input
 * @param node target node
 * @param posX/Y/Z camera position
 * @param tarX/Y/Z target
 * @param upX/Y/Z vector pointing up
 */
void lookAt(struct S_sceneNode *node, GLfloat *pos, GLfloat *tar, GLfloat *up){
	GLfloat xAxis[3], yAxis[3], zAxis[3];

	VectorSubtract(pos, tar, zAxis);
	VectorNormalize(zAxis);

	CrossProduct(up, zAxis, xAxis);
	VectorNormalize(xAxis);

	CrossProduct(zAxis, xAxis, yAxis);

	node->modelview[0] = xAxis[0];
	node->modelview[1] = yAxis[0];
	node->modelview[2] = zAxis[0];
	node->modelview[3] = 0;

	node->modelview[4] = xAxis[1];
	node->modelview[5] = yAxis[1];
	node->modelview[6] = zAxis[1];
	node->modelview[7] = 0;

	node->modelview[8] = xAxis[2];
	node->modelview[9] = yAxis[2];
	node->modelview[10] = zAxis[2];
	node->modelview[11] = 0;

	node->modelview[12] = 0;
	node->modelview[13] = 0;
	node->modelview[14] = 0;
	node->modelview[15] = 1.0f;


	translateNode(node, -pos[0], -pos[1], -pos[2]);
}
/**
 * Sets up a perspective projection matrix.
 * @param node camera node
 * @param fovy specifies the field of view angle, in degrees, in the y direction.
 * @param aspect specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).(in rad)
 * @param zNear specifies the distance from the viewer to the near clipping plane (always positive)
 * @param zFar pecifies the distance from the viewer to the far clipping plane (always positive)
 */
void setPerspective(struct S_sceneNode *node, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar){
	if (node->nodeType != E_OT_CAMERA)
		return;
	
	// set matrix to zeroes
	for(int i = 0; i < 16; ++i){
		((struct S_cameraContent *)node->content)->projectionMatrix[i] = 0;
	}

	float f = 1/(tanf(fovy/2));

	((struct S_cameraContent *)node->content)->projectionMatrix[0]  = f / aspect;
	((struct S_cameraContent *)node->content)->projectionMatrix[5]  = f;
	((struct S_cameraContent *)node->content)->projectionMatrix[10] = (zFar + zNear) / (zNear - zFar);
	((struct S_cameraContent *)node->content)->projectionMatrix[11] = (2 * zFar * zNear) / (zNear - zFar);
	((struct S_cameraContent *)node->content)->projectionMatrix[14] = -1;
}
/**
 * Wrapper around setPerspective(). Sets up a perspective projection matrix.
 * @param node camera node
 * @param fovy specifies the field of view angle, in degrees, in the y direction.
 * @param aspect specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height)
 * @param zNear specifies the distance from the viewer to the near clipping plane (always positive)
 * @param zFar pecifies the distance from the viewer to the far clipping plane (always positive)
 */
void setPerspectiveDeg(struct S_sceneNode *node, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar){
	fovy = deg2rad(fovy);
	setPerspective(node, fovy, aspect, zNear, zFar);
}

/**
 * Get modelview matrix from all previous nodes. If previous node is curve, calculate position.
 * @param node camera node
 * @param matrix result matrix
 */
void getCameraMatrix(struct S_sceneNode *node, GLfloat *matrix){
	// if curve - calculate coord in curve and apply it to modelview matrix
	if (node->nodeType == E_OT_CURVE){
		GLfloat position[3];
		GLfloat tmp[16] = {1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, 0,
				   0, 0, 0, 1};

		bezierGetPointRealTimeLinear(((struct S_curveContent *)node->content)->curve, getTime(&timer), ((struct S_curveContent *)node->content)->startTime, ((struct S_curveContent *)node->content)->stopTime, position);
		tmp[12] = position[0];
		tmp[13] = position[1];
		tmp[14] = position[2];

		//print("position [%f, %f, %f]\n", position[0],position[1],position[2]);

		if (node->parent == NULL){
			multMatrices(node->modelview, tmp, matrix);
			return;
		}
		else{
			GLfloat returnedMat[16];
			
			multMatrices(node->modelview, tmp, matrix);
			getCameraMatrix(node->parent, returnedMat);
			multMatrices(returnedMat, matrix, matrix);
			return;
		}
	}
	else{
		if (node->parent == NULL){
			myMemcpy(matrix, node->modelview, sizeof(GLfloat) * 16);
			return;
		}
		else{
			GLfloat returnedMat[16];

			getCameraMatrix(node->parent, returnedMat);
			multMatrices(returnedMat, node->modelview, matrix);
			return;
		}
	}

}

/**
 * uses camera - aplies modelview and projection matrices
 * @param node camera node
 */
void useCamera(struct S_sceneNode *node){
	GLfloat camPos[3];

	if (node->nodeType != E_OT_CAMERA)
		return;

	// set identity matrix
	setIdentity(node);

	// calculate camera matrix according to previous nodes
	GLfloat calculatedMatrix[16];
	GLfloat tar[3];
	getCameraMatrix(node, calculatedMatrix);
	camPos[0] = calculatedMatrix[12];
	camPos[1] = calculatedMatrix[13];
	camPos[2] = calculatedMatrix[14];


	GLfloat aspectRatio = (float)window.width / (float)window.height;
	GLfloat fov = ((struct S_cameraContent *)node->content)->fov;
	GLfloat nearClip = ((struct S_cameraContent *)node->content)->nearClip;
	GLfloat farClip = ((struct S_cameraContent *)node->content)->farClip;
	bezierGetPointRealTimeLinear(((struct S_cameraContent *)node->content)->lookAtCurve, getTime(&timer), ((struct S_cameraContent *)node->content)->startTime, ((struct S_cameraContent *)node->content)->stopTime, tar);
	lookAt(node, camPos, tar, ((struct S_cameraContent *)node->content)->up);

	// setting up projection
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, window.width, window.height);
	setPerspectiveDeg(node, fov, aspectRatio, nearClip, farClip);
	// load projection matrix
	glLoadMatrixf(((struct S_cameraContent *)node->content)->projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(node->modelview);
}

/**
 * Set identity. Matrix is zeroed, on main diagonal are ones.
 * @param node node to load identity
 */
void setIdentity(struct S_sceneNode *node){
	for(GLint i = 0; i < 16; ++i){
		if(i % 5 == 0){
			node->modelview[i] = 1;
			node->noRotModelview[i] = 1;
		}
		else{
			node->modelview[i] = 0;
			node->noRotModelview[i] = 0;
		}
	}
}



/**
 * Render scene node.
 * @param node selecting subtree to render
 */
void renderSceneNode(struct S_sceneNode *node){
	if (!node) return;
	// store matrix
	glPushMatrix();

	// for each node
	while (node){
		glPushMatrix();
		glMultMatrixf(node->modelview);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glActiveTextureARB(GL_TEXTURE7);
		glMultMatrixf(node->modelview);
		glMatrixMode(GL_MODELVIEW);
		
		// draw
                if (node->visible == 1){
                        if (node->renderCallback)
                                node->renderCallback(node);

                        // draw children
                        renderSceneNode(node->children);
                }

		// posunout se na dalsi node
		node = node->next;

		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	// return matrix
	glPopMatrix();
}