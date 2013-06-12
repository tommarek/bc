/**
\file
 All scene manager operations
*/

#include "scenemanager.h"

#include "sceneoperations.h"
#include "framework/materialmanager/materialmanager.h"
#include "framework/meshmanager/meshmanager.h"
#include "framework/sequencemanager/timer.h"
#include "framework/util.h"
#include "framework/extensions.h"

/**
 * Add node, to parent.
 * @param node pointer to node to be added
 * @param parent parent node
 */
void addNode(struct S_sceneNode *node, struct S_sceneNode *parent){
	node->next = parent->children;
	parent->children = node;

	node->parent = parent;

	// add root
	node->root = parent->root;
}

/**
 * delete whlole scene. internal funcitin for deleteNode().
 * @param root scene root
 */
void deleteScene(struct S_sceneNode *root){
	struct S_sceneNode *tmp, *node;
	node = root;
	// while list is not empty
	while (node){
		deleteScene(node->children);

		tmp = node->next;

		free(node);
		node = tmp;
	}
	free(root);
}

/**
 * Internal function for creating all objects.
 */
static struct S_sceneNode *createObject(){
	// create new node
	struct S_sceneNode *node = checkMalloc(sizeof(struct S_sceneNode));

	// set main diagonal to ones
	setIdentity(node);

	node->visible = 1;
	node->noisePos = 0;
	return node;
}

/**
 * Create new camera object
 * @param parent parent node
 * @param objectID camera ID
 */
struct S_sceneNode *createRoot(){
	// create object filled with modelview matrix and visibility settings
	struct S_sceneNode *newNode = createObject();

	newNode->nodeType = E_OT_ROOT;
	newNode->content = checkMalloc(sizeof(struct S_rootContent));

	newNode->parent = NULL;
	newNode->root = newNode;

	return newNode;
}

/**
 * Create new camera object
 * @param parent parent node
 * @param objectID camera ID
 */
struct S_sceneNode *createCamera(struct S_sceneNode *parent, GLfloat fov, GLfloat nearClip, GLfloat farClip, GLfloat *up, struct S_bezier *lookatCurve, GLuint startTime, GLuint stopTime){
	// create object filled with modelview matrix and visibility settings
	struct S_sceneNode *newNode = createObject();

	// init content
	newNode->content = checkMalloc(sizeof(struct S_cameraContent));
	((struct S_cameraContent *)newNode->content)->fov = fov;
	((struct S_cameraContent *)newNode->content)->nearClip = nearClip;
	((struct S_cameraContent *)newNode->content)->farClip = farClip;

	((struct S_cameraContent *)newNode->content)->lookAtCurve = lookatCurve;
	((struct S_cameraContent *)newNode->content)->startTime = startTime;
	((struct S_cameraContent *)newNode->content)->stopTime = stopTime;

	for (int i = 0; i < 3; ++i){
		((struct S_cameraContent *)newNode->content)->up[i] =up[i];
	}

	newNode->nodeType = E_OT_CAMERA;

	addNode(newNode, parent);

	return newNode;
}

/**
 * Render Callback function for meshes
 * @param node graph node to be rendered
 */
static void meshRenderCallback(struct S_sceneNode *node){
	// bind material
	bindMaterial(materials[((struct S_meshContent *)node->content)->materialID], ((struct S_rootContent *)node->root->content)->isShadowRendering);

	// draw
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind our buffers much like we would for texturing
	MYglBindBufferARB(GL_ARRAY_BUFFER, meshes[((struct S_meshContent *)node->content)->meshID]->VBOID);
	MYglBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, meshes[((struct S_meshContent *)node->content)->meshID]->quadID);

	// Set the state of what we are drawing (I don't think order matters here, but I like to do it in the same
	// order I set the pointers
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// Resetup our pointers.  This doesn't reinitialise any data, only how we walk through it
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(12));
	glNormalPointer(GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(20));
	glColorPointer(4, GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(32));
	glVertexPointer(3, GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(0));

	// Actually do our drawing, parameters are Primative (Triangles, Quads, Triangle Fans etc), Elements to
	// draw, Type of each element, Start Offset
	glDrawElements(GL_QUADS, meshes[((struct S_meshContent *)node->content)->meshID]->quadIdCount, GL_UNSIGNED_INT, NULL);

	// Disable our client state back to normal drawing
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * Create new mesh object
 * @param parent parent node
 * @param objectID mesh ID
 * @param materialID material ID
 */
struct S_sceneNode *createMesh(struct S_sceneNode *parent, GLint meshID, GLint materialID){
	// create object filled with modelview matrix and visibility settings
	struct S_sceneNode *newNode = createObject();

	newNode->nodeType = E_OT_MESH;
	newNode->content = checkMalloc(sizeof(struct S_meshContent));

	((struct S_meshContent *)newNode->content)->meshID = meshID;
	((struct S_meshContent *)newNode->content)->materialID = materialID;

	newNode->renderCallback = meshRenderCallback;

	addNode(newNode, parent);

	return newNode;
}

/**
 * Create new dummy object
 * @param parent parent node
 */
struct S_sceneNode *createDummy(struct S_sceneNode *parent){
	struct S_sceneNode *newNode = createObject();

	newNode->nodeType = E_OT_DUMMY;
	newNode->content = NULL;

	newNode->renderCallback = NULL;

	addNode(newNode, parent);
	return newNode;
}



/**
 * Render Callback function for curves
 * @param node scene graph node
 */
static void curveRenderCallback(struct S_sceneNode *node){
	GLfloat pos[3];
	GLint time = getTime(&timer);

	bezierGetPointRealTimeLinear(((struct S_curveContent *)node->content)->curve, time, ((struct S_curveContent *)node->content)->startTime, ((struct S_curveContent *)node->content)->stopTime, pos);

	glTranslatef(pos[0], pos[1], pos[2]);

	// test - render node path
	//drawCurve(((struct S_curveContent *)node->content)->curve);

}

/**
 * Create new dummy object
 * @param parent parent node
 */
struct S_sceneNode *createCurve(struct S_sceneNode *parent, struct S_bezier *curve, struct S_bezier *speed, GLuint startTime, GLuint stopTime){
	struct S_sceneNode *newNode = createObject();

	newNode->nodeType = E_OT_CURVE;
	newNode->content = checkMalloc(sizeof(struct S_curveContent));
	((struct S_curveContent *)newNode->content)->startTime = startTime;
	((struct S_curveContent *)newNode->content)->stopTime = stopTime;
	((struct S_curveContent *)newNode->content)->curve = curve;
	((struct S_curveContent *)newNode->content)->speed = speed;

	newNode->renderCallback = curveRenderCallback;

	addNode(newNode, parent);
	return newNode;
}

/**
 * set lihgt properties
 * @param root scene root node
 * @param ambient ambient color
 * @param difuse difuse color
 * @param specular specular color
 * @param position light position
 * @param targetPoint light target point
 */
void setLight(struct S_sceneNode *root, GLfloat *ambient, GLfloat *difuse, GLfloat *specular, GLfloat *position, GLfloat *targetPoint, GLfloat lightCutOff){
	GLfloat direction[3];
	for (int i = 0; i < 4; ++i){
		((struct S_rootContent *)root->content)->light.lightAmbient[i] = ambient[i];
		((struct S_rootContent *)root->content)->light.lightDiffuse[i] = difuse[i];
		((struct S_rootContent *)root->content)->light.lightSpecular[i] = specular[i];
	}

	VectorSubtract(targetPoint, position, direction);
	VectorNormalize(targetPoint);

	for (int i = 0; i < 3; ++i){
		((struct S_rootContent *)root->content)->light.lightTargetPoint[i] = targetPoint[i];
		((struct S_rootContent *)root->content)->light.lightDirection[i] = direction[i];
		((struct S_rootContent *)root->content)->light.lightPosition[i] = position[i];
	}
	((struct S_rootContent *)root->content)->light.lightDirection[3] = 0.0f; // last must be 0.0
	((struct S_rootContent *)root->content)->light.lightPosition[3] = 1.0f; // last must be 1.0

	((struct S_rootContent *)root->content)->light.lightCutOff = lightCutOff;

}

/**
 * Initialize scene manager - create scene
 */
void initSceneManger(){
    initScenes();
} 