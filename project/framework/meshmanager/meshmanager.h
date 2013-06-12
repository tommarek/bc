#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdint.h>

#include "framework/texturemanager/texturemanager.h"
#include "demo/meshes/mesh.h"

#define INDEXARRAYTYPE GLuint
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define STRIPSEPARATOR ((uint16_t)-1)

struct S_vertex{
	GLfloat position[3];
	GLfloat tex[2];
	GLfloat normal[3];
	GLubyte padding[32]; // Pads the struct out to 64 bytes for performance increase
};

struct S_3DmodelUnpacked{
	// verticies including everything
	struct S_vertex *vertices; // filled with verticies
	GLuint vertexCount; // nuber of verticies

	GLuint VBOID; // vertex bufferID
	INDEXARRAYTYPE quadID; // quad index buffer

	INDEXARRAYTYPE *quads; // filled with indicies
	GLuint quadIdCount;
};

extern struct S_3DmodelUnpacked *meshes[];

typedef GLfloat (*rotationalModelCallback)(GLfloat);

struct S_3DmodelUnpacked *unpackModel(int16_t vertices[], uint16_t vertexCount, GLfloat vertex0[], GLfloat scale[], uint16_t quadsCount, uint16_t quads[]);
struct S_3DmodelUnpacked *rotationalModel(rotationalModelCallback, GLfloat, unsigned, unsigned, int, int);
struct S_3DmodelUnpacked *mirrorModel(struct S_3DmodelUnpacked *);
void cleanModel(struct S_3DmodelUnpacked *model, GLfloat eps);
struct S_3DmodelUnpacked *mergeModels(struct S_3DmodelUnpacked *model1, struct S_3DmodelUnpacked *model2);

void calculateUVcoords(struct S_3DmodelUnpacked *);
void calculateNormals(struct S_3DmodelUnpacked *);
void catmullClark(struct S_3DmodelUnpacked *);
void createBuffers(struct S_3DmodelUnpacked *);
void exportRaw(struct S_3DmodelUnpacked *);
void destroyMesh(struct S_3DmodelUnpacked *);

void initMeshManager();
#endif