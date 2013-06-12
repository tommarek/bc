/**
\file
 All material manager operations
*/

#include "materialmanager.h"

#include "framework/texturemanager/texturemanager.h"

#include "framework/util.h"
#include "framework/extensions.h"
#include "framework/draw.h"

// all materials
struct S_material *materials[E_MAT_COUNT];
struct S_shader *shaders[E_SH_COUNT];

/**
 * function sets input shader to openGL
 * @param material pointer to material structure
 */
static void useShader(struct S_material *material){
	GLint uniformTextureId;
	char uniformName[16];

	MYglUseProgramObjectARB(material->shader->program);

	// pass shadowMap
	GLint shadowMapUniform = MYglGetUniformLocationARB(material->shader->program,"ShadowMap");
	MYglUniform1iARB(shadowMapUniform,7);
	glActiveTextureARB(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,depthTextureId);

	// pass shadowmap steps
	GLint stepW = MYglGetUniformLocationARB(material->shader->program, "stepW");
	MYglUniform1fARB(stepW, shadowMapWidthStep);
	GLint stepH = MYglGetUniformLocationARB(material->shader->program, "stepH");
	MYglUniform1fARB(stepH, shadowMapHeightStep);

	// pass light and camera position
	GLint lightP = MYglGetUniformLocationARB(material->shader->program, "lightPos");
	MYglUniform3fARB(lightP, lightPos[0], lightPos[1], lightPos[2]);
	GLint lightD = MYglGetUniformLocationARB(material->shader->program, "lightDir");
	MYglUniform3fARB(lightD, lightDir[0], lightDir[1], lightDir[2]);

	for (int i = 0; i < material->textueCount; ++i){
		sprintf(uniformName, "tex%d", i);
		uniformTextureId = MYglGetUniformLocationARB(material->shader->program, uniformName);
		glActiveTextureARB(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, material->textureID[i]);
		MYglUniform1iARB(uniformTextureId, i);
	}
}

static void printProgramInfoLog(GLuint obj){
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	MYglGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

	if (infologLength > 0){
		infoLog = (char *)malloc(infologLength);
		MYglGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		print("%s\n",infoLog);
		free(infoLog);
	}
}


/**
 * compiles and loads shader, creates program
 * @param shader pointer to shader source
 */
void createShader(struct S_shader *shader){
	// create vertices
	shader->shaderVert = MYglCreateShaderObjectARB(GL_VERTEX_SHADER);
	shader->shaderFrag = MYglCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	// load source
	MYglShaderSourceARB(shader->shaderVert, 1, &(shader->sourceVert),NULL);
	MYglShaderSourceARB(shader->shaderFrag, 1, &(shader->sourceFrag),NULL);

	// compile shaders
	MYglCompileShaderARB(shader->shaderVert);
	MYglCompileShaderARB(shader->shaderFrag);

	// create program
	shader->program = MYglCreateProgramObjectARB();

	// attach shaders
	MYglAttachObjectARB(shader->program,shader->shaderVert);
	MYglAttachObjectARB(shader->program,shader->shaderFrag);

	MYglLinkProgramARB(shader->program);
	print("program:\n");
	printProgramInfoLog(shader->program);
}

/**
 * Sets Opengl to work with current material, shaders and textures specified in material structure.
 * @param material material to se to work with
 */
void bindMaterial(struct S_material *material, GLint isShadowRendering){
	// color
	glColor4fv(material->color);
	
	// ambient and difuse
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient);
	// specular
	glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);
	// shinines
	glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);

	// use shader
#ifndef NOSHADOW
	if(!isShadowRendering)
		useShader(material);
	else
		MYglUseProgramObjectARB(shaders[E_SH_SHADOW]->program);
#else
	useShader(material);
#endif

	return;
}

/**
 * Initialize material manager. compiles shaders, loads textures
 */
void initMaterialManager(){
	initShaders();
	initMaterials();
	return;
}
