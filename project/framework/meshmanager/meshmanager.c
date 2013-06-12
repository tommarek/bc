/**
\file
 All mesh manager operations
*/
#include "meshmanager.h"

#include <GL/gl.h>
#include <GL/glu.h>
//#include <assert.h>

#include "framework/util.h"
#include "framework/extensions.h"

struct S_3DmodelUnpacked *meshes[E_MESH_COUNT];

/**
 * calculates uv coords.
 * Uses sphere mapping.
 *                  X
 * u = --------------------------
 *        sqrt(X^2 + Y^2 + Z^2)
 *
 *                  Y
 * v = --------------------------
 *        sqrt(X^2 + Y^2 + Z^2)

 * @param model model to calculate coords on
 */
void calculateUVcoords(struct S_3DmodelUnpacked *model){
	// for each vertex
	for(int i = 0; i < model->vertexCount; ++i){
		for(int j = 0; j < 2; ++j){
			model->vertices[i].tex[j] = model->vertices[i].position[j] / mySqrt(
						    model->vertices[i].position[0] * model->vertices[i].position[0] + // X^2
						    model->vertices[i].position[1] * model->vertices[i].position[1] + // Y^2
						    model->vertices[i].position[2] * model->vertices[i].position[2]); // Z^2
		}
	}

}



/**
 * Calculate normals from surroundings quads
 * uses gouraud shading only
 * accepting only quads!!!
 * @param model pointer to unpacked model
 */
void calculateNormals(struct S_3DmodelUnpacked *model){
	int quad;

	GLfloat *normalVector, a[3], b[3];
	normalVector = checkMalloc(3 * sizeof(GLfloat));

	GLfloat *normalArray = checkMalloc(model->quadIdCount * sizeof(GLfloat));

	// create normal array
	for(int i = 0; i < model->quadIdCount; i += 4){
		for(int j = 0; j < 3; ++j){
			a[j] = model->vertices[model->quads[i + 1]].position[j] - model->vertices[model->quads[i + 0]].position[j];
			b[j] = model->vertices[model->quads[i + 3]].position[j] - model->vertices[model->quads[i + 0]].position[j];
		}
		// calculate normal
		CrossProduct(a, b, normalVector);

		// fill vectors to normal array
		for(int j = 0; j < 3; ++j){
			normalArray[i + j] = normalVector[j];
		}
	}

	// for each vertex calculate normal vecotr
	for(int i = 0; i < model->vertexCount; ++i){
		// set normal vector to 0
		myMemset(normalVector, 0, 3 * sizeof(GLfloat));

		// search for refenrence in quad indices
		for(int j = 0; j < model->quadIdCount; ++j){
			if(i == model->quads[j]){
				quad = j - (j % 4);

				for(int k = 0; k < 3; ++k){
					normalVector[k] += normalArray[quad + k];
				}
			}
		}
		// normalize before sotring result
		VectorNormalize(normalVector);

		// store result to vertex
		for(int j = 0; j < 3; ++j){
			model->vertices[i].normal[j] = normalVector[j];
		}
	}
	free(normalVector);
}

// help structure for catmull clark
struct edge{
	GLuint id;
	INDEXARRAYTYPE edgePoint;
	INDEXARRAYTYPE endpoint1, endpoint2;
	unsigned count;
};

struct vertex{
	// face point
	GLfloat facepoint[3];
	GLuint facesNum;

	// midpoints
	GLfloat midpoints[3];
	GLuint midpointsNum;

};


/**
 * Return index of newly created centroid point of a given polygon.
 */
static INDEXARRAYTYPE getCentroid(INDEXARRAYTYPE *vertexIndices, unsigned vertexIndexCount, struct S_3DmodelUnpacked *model){
	INDEXARRAYTYPE centroid = (model->vertexCount)++;

	for(int axis = 0; axis < 3; ++axis){
		GLfloat tmp = 0;
		for(int i = 0; i < vertexIndexCount; ++i)
			tmp += model->vertices[vertexIndices[i]].position[axis];
		model->vertices[centroid].position[axis] = tmp / vertexIndexCount;
	}

	return centroid;
}


/**
 * catmull clark
 * Return index of newly created or previously existing midpoint of an edge.
 * This edge point is set to endpoint1 + endpoint2
 */
static INDEXARRAYTYPE getEdgePoint(INDEXARRAYTYPE endpoint1, INDEXARRAYTYPE endpoint2, struct S_3DmodelUnpacked *model, struct edge *existing, unsigned *existingCount){
	GLuint edgeId = endpoint1 > endpoint2 ? ((endpoint1 << 16) | endpoint2) : (endpoint1 | (endpoint2 << 16)); // finta od RGBA

	// searching in existing points
	for(int i = 0; i < *existingCount; ++i){
		if(existing[i].id == edgeId){
			(existing[i].count)++;
			return existing[i].edgePoint;
		}
	}

	// not found
	// create new vertex
	INDEXARRAYTYPE edgePoint = (model->vertexCount)++;
	// edge point position is created by adding two endpoints
	for(int i = 0; i < 3; ++i){
		model->vertices[edgePoint].position[i] = (model->vertices[endpoint1].position[i] + model->vertices[endpoint2].position[i]);
	}
	existing[*existingCount].id = edgeId;
	existing[*existingCount].edgePoint = edgePoint;
/*
	printf("edgepoint[%d] = %d; model->vertexCount = %d\n", *existingCount ,existing[*existingCount].edgePoint, model->vertexCount );
*/
	existing[*existingCount].count = 1;
	// store endpoints
	existing[*existingCount].endpoint1 = endpoint1;
	existing[*existingCount].endpoint2 = endpoint2;
	++(*existingCount);

	return edgePoint;
}

/**
 * Subdivide model using catmull-clark algorithm
 * results are quads
 *
 * http://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface
 * @param *model pointer to model
 */
void catmullClark(struct S_3DmodelUnpacked *model){
	//assert(model->vertexCount <= 1 << 16); // kvuli finte s << 16 v getEdgePoint()

	// oldVertexCount contains count of original vertices
	GLuint oldVertexCount = model->vertexCount;

	struct edge *edges;
	struct vertex *vertices;

	unsigned newQuadCount = model->quadIdCount; // exactly the number of new quads

	// list of new edge points
	edges = checkMalloc(newQuadCount * sizeof(struct edge));

	model->vertices = checkRealloc(model->vertices, sizeof(struct S_vertex) * (5 * model->quadIdCount / 4 + oldVertexCount));

	// vertex infromations
	vertices = checkMalloc(oldVertexCount * sizeof(struct vertex));

	// set vertices.facesNum and vertices.midpointsNum to 0
	for(int i = 0; i < oldVertexCount; ++i){
		for(int j = 0; j < 3; j++){
			vertices[i].facepoint[j] = 0;
			vertices[i].midpoints[j] = 0;
		}
		vertices[i].facesNum = 0;
		vertices[i].midpointsNum = 0;
	}

	unsigned edgeCount = 0;
	GLuint newQuadIdCount = model->quadIdCount;

	model->quads = checkRealloc(model->quads, 4 * newQuadCount * sizeof(INDEXARRAYTYPE));

	// quads
	for(int i = 0; i < model->quadIdCount; i += 4){
		// new face point creation
		INDEXARRAYTYPE facePoint = getCentroid(model->quads + i, 4, model);
		// put facepoint to vertex structure for each vertex in quad
		for(int j = 0; j < 4; ++j){
			// each coord
			for(int k = 0; k < 3; ++k){
				vertices[model->quads[i + j]].facepoint[k] += model->vertices[facePoint].position[k];
			}
			vertices[model->quads[i + j]].facesNum += 1;
		}

		// new edge point creation
		INDEXARRAYTYPE edgePoints[4];
		for(int j = 0; j < 4; ++j){
			edgePoints[j] = getEdgePoint(model->quads[i + j], model->quads[i + (j + 1) % 4], model, edges, &edgeCount);
			for(int k = 0; k < 3; ++k){
				model->vertices[edgePoints[j]].position[k] += model->vertices[facePoint].position[k];
			}
		}

		// create 4 new quads from old one
		//                                        ep[1]
		//                                   _____________
		//              model->quads[i + 2] |      |      | model->quads[i + 1]
		//                                  |  3   |  2   |
		//                          ep[2]   |_____f|p_____|ep[0]
		//                                  |      |      |
		//                                  |  4   |  1   |
		//              model->quads[i + 3] |______|______| model->quads[i + 0]
		//                                       ep[3]
		//

		// 2
		model->quads[newQuadIdCount + 0] = edgePoints[0];
		model->quads[newQuadIdCount + 1] = model->quads[i + 1];
		model->quads[newQuadIdCount + 2] = edgePoints[1];
		model->quads[newQuadIdCount + 3] = facePoint;
		newQuadIdCount += 4;

		// 3
		model->quads[newQuadIdCount + 0] = facePoint;
		model->quads[newQuadIdCount + 1] = edgePoints[1];
		model->quads[newQuadIdCount + 2] = model->quads[i + 2];
		model->quads[newQuadIdCount + 3] = edgePoints[2];
		newQuadIdCount += 4;

		// 4
		model->quads[newQuadIdCount + 0] = edgePoints[3];
		model->quads[newQuadIdCount + 1] = facePoint;
		model->quads[newQuadIdCount + 2] = edgePoints[2];
		model->quads[newQuadIdCount + 3] = model->quads[i + 3];
		newQuadIdCount += 4;

		// 1
		model->quads[i + 1] = edgePoints[0];
		model->quads[i + 2] = facePoint;
		model->quads[i + 3] = edgePoints[3];
	}

	// all midpoints in all edges are set to average of facepoints and endpoints (+2)
	for(int i = 0; i < edgeCount; ++i){
		for(int j = 0; j < 3; ++j){
			// count - how many times was that edge visited
			model->vertices[edges[i].edgePoint].position[j] /= (edges[i].count + 2);
			// add midpoint to its origin
			vertices[edges[i].endpoint1].midpoints[j] += model->vertices[edges[i].edgePoint].position[j];
			vertices[edges[i].endpoint2].midpoints[j] += model->vertices[edges[i].edgePoint].position[j];
		}
		vertices[edges[i].endpoint1].midpointsNum += 1;
		vertices[edges[i].endpoint2].midpointsNum += 1;
	}


	// set new position of original vertices
	for(int i = 0; i < oldVertexCount; ++i){
		if (vertices[i].facesNum  > 2){
			for(int j = 0; j < 3; ++j){
				model->vertices[i].position[j] += ((vertices[i].facepoint[j] / vertices[i].facesNum) +
								  2 * (vertices[i].midpoints[j] / vertices[i].midpointsNum) -
								  3 * model->vertices[i].position[j]) /
								  vertices[i].facesNum;
			}
		}
	}




	model->quadIdCount = newQuadIdCount;

	free(edges);
	free(vertices);
	return;
}

/**
 * convert packed model to unpacked
 * @param vertices array of vertices
 * @param vercexCount count of vertices in vertices
 * @param vertex0 array containing coordinate of vertex 0
 * @param scale scale of model
 * @param quadsCount count of quads in model
 * @param quads array of quad strips
 */
 struct S_3DmodelUnpacked *unpackModel(
			int16_t vertices[],
                        uint16_t vertexCount,
                        GLfloat vertex0[],
                        GLfloat scale[],
                        uint16_t quadsCount,
                        uint16_t quads[]){
	// create model
	struct S_3DmodelUnpacked *model;
	model = checkMalloc(sizeof(struct S_3DmodelUnpacked));

	// unpack quads
	model->quadIdCount = quadsCount * 4;
	model->quads = checkMalloc(sizeof(INDEXARRAYTYPE) * model->quadIdCount);

	model->quads[0] = quads[0];
	model->quads[1] = quads[1];
	model->quads[2] = quads[3];
	model->quads[3] = quads[2];
	for(int i = 4, j = 4; i < model->quadIdCount; i += 4, j += 2){
		if(quads[j] == STRIPSEPARATOR){
			j += 3;
		}
		model->quads[i + 0] = quads[j - 2];
		model->quads[i + 1] = quads[j - 1];
		model->quads[i + 2] = quads[j + 1];
		model->quads[i + 3] = quads[j + 0];
	}

	// unpack vertices
	model->vertexCount = vertexCount;
	model->vertices = checkMalloc(sizeof(struct S_vertex) * model->vertexCount);

	model->vertices[0].position[0] = vertex0[0];
	model->vertices[0].position[1] = vertex0[1];
	model->vertices[0].position[2] = vertex0[2];
	for(int i = 1; i < vertexCount; ++i){
		for(int j = 0; j < 3; ++j){
			model->vertices[i].position[j] = model->vertices[i - 1].position[j] + (scale[j] * vertices[(i- 1) * 3 + j]);
		}
	}
	return model;
}

/**
 * load unpacked model to VB
 * QUADS ONLY!!!
 * @param model pointer to model
 */
void createBuffers(struct S_3DmodelUnpacked *model){
	MYglGenBuffersARB(1, &(model->VBOID)); // Create the buffer ID, this is basically the same as generating texture ID's
	MYglBindBufferARB(GL_ARRAY_BUFFER, model->VBOID); // Bind the buffer (vertex array data)

	MYglBufferDataARB(GL_ARRAY_BUFFER, model->vertexCount * sizeof(struct S_vertex), NULL, GL_STATIC_DRAW);
	MYglBufferSubDataARB(GL_ARRAY_BUFFER, 0, model->vertexCount * sizeof(struct S_vertex), model->vertices); // Actually upload the data

	glVertexPointer(3, GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(0));
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(12));
	glNormalPointer(GL_FLOAT, sizeof(struct S_vertex), BUFFER_OFFSET(20));

	MYglGenBuffersARB(1, &(model->quadID)); // Generate buffer
	MYglBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, model->quadID); // Bind the element array buffer

	MYglBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, model->quadIdCount * sizeof(INDEXARRAYTYPE), model->quads, GL_STATIC_DRAW);
}

/*------------------------------------------------------------------------------*/
/*                     functions for rotational model                           */
GLfloat valec(GLfloat x){
	return 3;
}
/*------------------------------------------------------------------------------*/

static inline int rotationalModelGetIndex(unsigned stepsAngle, int i, int j) {
	return (stepsAngle * i) + j + 2;
}

/**
 * Function creates rotational model from function specified in parameter.
 * model rotates around X axis, begining and end fo model are always closed
 * @param length specifies length of model, starting from 0.
 * @param stepsLen specifies how many steps will be on x(length) axis
 * @param stepsAngle specifies how many steps will be created with rotation
 * @param function function used to generate model
 * @param front when set to 1 cobject will be closed on front side
 * @param rear when set to 1 cobject will be closed on rear side
 */
struct S_3DmodelUnpacked *rotationalModel(rotationalModelCallback function, GLfloat lengtht, unsigned stepsLength, unsigned stepsAngle, int front, int rear){
	//assert(stepsLength >= 2 && stepsAngle >= 2);
	struct S_3DmodelUnpacked *model;
	GLfloat value;

	// create space for new model
	model = checkMalloc(sizeof(struct S_3DmodelUnpacked));

	model->vertexCount = stepsAngle * stepsLength + 2;
	model->vertices = checkMalloc(sizeof(struct S_vertex) * model->vertexCount);

	// quad id count calculation
	model->quadIdCount = 4 * (stepsLength - 1) * stepsAngle;
	if (front == 1){
		model->quadIdCount += 4 * stepsAngle;
	}
	if (rear == 1){
		model->quadIdCount += 4 * stepsAngle;
	}
	model->quads = checkMalloc(sizeof(INDEXARRAYTYPE) * model->quadIdCount);

	// first vertex - filling gap at the beginning of mesh
	model->vertices[0].position[0] = 0;
	model->vertices[0].position[1] = 0;
	model->vertices[0].position[2] = 0;

	// second vertex - filling gap at the end of mesh
	model->vertices[1].position[0] = lengtht;
	model->vertices[1].position[1] = 0;
	model->vertices[1].position[2] = 0;

	// generate vertices
	GLfloat stepLength = lengtht / (stepsLength - 1);
	GLfloat stepAngle = (2 * M_PI) / stepsAngle;
	for (int i = 0; i < stepsLength; ++i){
		for (int j = 0; j < stepsAngle; ++j){
			value = function(i * stepLength);
			model->vertices[rotationalModelGetIndex(stepsAngle, i, j)].position[0] = i * stepLength;
			model->vertices[rotationalModelGetIndex(stepsAngle, i, j)].position[1] = sinf(j * stepAngle) * value;
			model->vertices[rotationalModelGetIndex(stepsAngle, i, j)].position[2] = cosf(j * stepAngle) * value;
		}
	}

	// create quads
	int nextQuad = 0;
	for (int i = 0; i < (stepsLength - 1); ++i){
		for (int j = 0; j < stepsAngle; ++j){
			model->quads[nextQuad + 0] = rotationalModelGetIndex(stepsAngle, i + 1, j);
			model->quads[nextQuad + 1] = rotationalModelGetIndex(stepsAngle, i + 1, (j + 1) % stepsAngle);
			model->quads[nextQuad + 2] = rotationalModelGetIndex(stepsAngle, i + 0, (j + 1) % stepsAngle);
			model->quads[nextQuad + 3] = rotationalModelGetIndex(stepsAngle, i + 0, j);
			nextQuad += 4;
		}

	}

	// filling rear and front gaps
	for(int i = 0; i < stepsAngle; ++i){
		if (front == 1){
			model->quads[nextQuad + 0] = rotationalModelGetIndex(stepsAngle, 0, i);
			model->quads[nextQuad + 1] = rotationalModelGetIndex(stepsAngle, 0, (i + 1) % stepsAngle);
			model->quads[nextQuad + 2] = 0;
			model->quads[nextQuad + 3] = 0;
			nextQuad += 4;
		}
		if (rear == 1){
			model->quads[nextQuad + 0] = 1;
			model->quads[nextQuad + 1] = 1;
			model->quads[nextQuad + 2] = rotationalModelGetIndex(stepsAngle, stepsLength - 1, (i + 1) % stepsAngle);
			model->quads[nextQuad + 3] = rotationalModelGetIndex(stepsAngle, stepsLength - 1, i);
			nextQuad += 4;
		}
	}
	return model;
}

/**
 * Rounds every vertex in X axis to 0 if abs(X) is less than eps
 * @param model model to clean
 * @param eps accuracy
 */
void cleanModel(struct S_3DmodelUnpacked *model, GLfloat eps){
	// rounding edges to 0
	for (int i = 0; i < model->vertexCount; ++i){
		if (abs(model->vertices[i].position[0]) < eps) model->vertices[i].position[0] = 0;
	}
}


/**
 * Mirror model.
 * @param sourceModel model to mirror
 * @param eps
 * @return mirrored model
 */
struct S_3DmodelUnpacked *mirrorModel(struct S_3DmodelUnpacked *sourceModel){
	struct S_3DmodelUnpacked *model;
	GLfloat tmp;

	model = checkMalloc(sizeof(struct S_3DmodelUnpacked));

	model->quadIdCount = sourceModel->quadIdCount;
	model->vertexCount = sourceModel->vertexCount;

	model->quads = checkStrdup(sourceModel->quads, sizeof(INDEXARRAYTYPE) * model->quadIdCount);
	model->vertices = checkStrdup(sourceModel->vertices, sizeof(struct S_vertex) * model->vertexCount);

	// invert one axis
	for (int i = 0; i < model->vertexCount; ++i){
		model->vertices[i].position[0] = -1 * (model->vertices[i].position[0]);
	}

	// change rotation of quads
	for (int i = 0; i < model->quadIdCount; i += 4){
		// swap first and last index
		tmp = model->quads[i + 0];
		model->quads[i + 0] = model->quads[i + 3];
		model->quads[i + 3] = tmp;
		// swap second and third index
		tmp = model->quads[i + 1];
		model->quads[i + 1] = model->quads[i + 2];
		model->quads[i + 2] = tmp;
	}

	return model;
}

/**
 * merge Mirrored an original meshes to one mesh. Searches for possible merging on X axis.
 * @param model1 first model
 * @param model2 second model
 * @return merged model
 */
struct S_3DmodelUnpacked *mergeModels(struct S_3DmodelUnpacked *model1, struct S_3DmodelUnpacked *model2){
	struct S_3DmodelUnpacked *result;
	result = checkMalloc(sizeof(struct S_3DmodelUnpacked));

	result->vertexCount = model1->vertexCount + model2->vertexCount;
	result->quadIdCount = model1->quadIdCount + model2->quadIdCount;

	// insert informations
	// model 1
	result->vertices = checkMalloc(sizeof(struct S_vertex) * result->vertexCount);
	result->quads = checkMalloc(sizeof(INDEXARRAYTYPE) * result->quadIdCount);
	for(int i = 0; i < model1->vertexCount; ++i){
		for (int j = 0; j < 3; ++j){
			result->vertices[i].position[j] = model1->vertices[i].position[j];
			result->vertices[i].normal[j] = model1->vertices[i].normal[j];
		}
		for (int j = 0; j < 2; ++j){
			result->vertices[i].tex[j] = model1->vertices[i].tex[j];
		}
	}
	for(int i = 0; i < model1->quadIdCount; ++i){
		result->quads[i] = model1->quads[i];
	}

	//  model2
	for(int i = 0; i < model2->vertexCount; ++i){
		for (int j = 0; j < 3; ++j){
			result->vertices[i + model1->vertexCount].position[j] = model2->vertices[i].position[j];
			result->vertices[i + model1->vertexCount].normal[j] = model2->vertices[i].normal[j];
		}
		for (int j = 0; j < 2; ++j){
			result->vertices[i + model1->vertexCount].tex[j] = model2->vertices[i].tex[j];
		}
	}
	for(int i = 0; i < model2->quadIdCount; ++i){
		result->quads[i + model1->quadIdCount] = model2->quads[i] + model1->vertexCount;
	}


	// for each point with X == 0
	for (int i = 0; i < result->vertexCount; ++i){
		if(result->vertices[i].position[0] != 0) continue;
		// search for another same vertex
		for (int j = i + 1; j < result->vertexCount; ++j){
			if(result->vertices[j].position[0] == result->vertices[i].position[0] &&
			   result->vertices[j].position[1] == result->vertices[i].position[1] &&
			   result->vertices[j].position[2] == result->vertices[i].position[2]){
			 	// change result quads
			 	for (int k = 0; k < result->quadIdCount; ++k){
			 		if(result->quads[k] == j) result->quads[k] = i; // tady
			 	}
			 }
			 else
			 	continue;
		}
	}
	return result;
}


void destroyMesh(struct S_3DmodelUnpacked *model){
	if(model->quads) free(model->quads);
	if(model->vertices) free(model->vertices);
	free(model);
}


/**
 * Help function to print result to raw
 */
void exportRaw(struct S_3DmodelUnpacked *model){
	FILE *fp;
	if (!(fp = fopen("testOtput.raw", "w"))){
		printf("could not open testOutput.raw");
	}

	// print quads
	for (int i = 0; i < model->quadIdCount; i += 4){
		for (int j = 0; j < 4; ++j){
			for (int k = 0; k < 3; ++k){
				fprintf(fp,"%f ", model->vertices[model->quads[i + j]].position[k]);
			}
		}
		fprintf(fp,"\n");
	}

	fclose(fp);
	return;
}

/**
 * initialization of mesh manager
 */
void initMeshManager(){
	initMeshes();
}