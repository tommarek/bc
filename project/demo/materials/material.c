#include "material.h"

#include "framework/util.h"

#include "shaders.h"

struct S_material *materialGround(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 5.0f;

	// set color to brown
	mat->color[0] = 0.2f;
	mat->color[1] = 0.4f;
	mat->color[2] = 0.3f;
	mat->color[3] = 1.00f;

	// ambient color of model
	mat->ambient[0] = 0.1f;
	mat->ambient[1] = 0.1f;
	mat->ambient[2] = 0.1f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}


struct S_material *materialPianoBody(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 50.0f;

	// set color to brown
	mat->color[0] = 0.05f;
	mat->color[1] = 0.05f;
	mat->color[2] = 0.05f;
	mat->color[3] = 1.00f;

	// ambient color of model
	mat->ambient[0] = 0.2f;
	mat->ambient[1] = 0.2f;
	mat->ambient[2] = 0.2f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}

struct S_material *materialPianoStrings(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 50.0f;

	// set color to brown
	mat->color[0] = 0.71f;
	mat->color[1] = 0.65f;
	mat->color[2] = 0.26f;
	mat->color[3] = 1.00f;

	// ambient color of model
	mat->ambient[0] = 0.5f;
	mat->ambient[1] = 0.5f;
	mat->ambient[2] = 0.5f;
	mat->ambient[3] = 1.00f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}

struct S_material *materialText(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 15.0f;

	// set color to brown
	mat->color[0] = 0.71f;
	mat->color[1] = 0.65f;
	mat->color[2] = 0.26f;
	mat->color[3] = 1.00f;

	// ambient color of model
	mat->ambient[0] = 1.0f;
	mat->ambient[1] = 1.0f;
	mat->ambient[2] = 1.0f;
	mat->ambient[3] = 1.00f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_NOSHADOW];

	return mat;
}

struct S_material *materialPianoWhiteKey(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 50.0f;

	// set color to brown
	mat->color[0] = 1.0f;
	mat->color[1] = 1.0f;
	mat->color[2] = 1.0f;
	mat->color[3] = 1.0f;

	// ambient color of model
	mat->ambient[0] = 1.0f;
	mat->ambient[1] = 1.0f;
	mat->ambient[2] = 1.0f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}

struct S_material *materialPianoBlackKey(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 50.0f;

	// set color to brown
	mat->color[0] = 0.0f;
	mat->color[1] = 0.0f;
	mat->color[2] = 0.0f;
	mat->color[3] = 1.0f;

	// ambient color of model
	mat->ambient[0] = 0.2f;
	mat->ambient[1] = 0.2f;
	mat->ambient[2] = 0.2f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}

/**
 * material for puppet's body
 */
struct S_material *materialPuppetWood(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 10.0f;

	// set color to yelow
	mat->color[0] = 0.71785f;
	mat->color[1] = 0.5234375f;
	mat->color[2] = 0.04296875f;
	mat->color[3] = 1.0f;

	// ambient color of model
	mat->ambient[0] = 0.2f;
	mat->ambient[1] = 0.2f;
	mat->ambient[2] = 0.2f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}

/**
 * material for puppet's eye
 */
struct S_material *materialPuppetEye(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 10.0f;

	// set color to yelow
	mat->color[0] = 1.0f;
	mat->color[1] = 1.0f;
	mat->color[2] = 1.0f;
	mat->color[3] = 1.0f;

	// ambient color of model
	mat->ambient[0] = 0.2f;
	mat->ambient[1] = 0.2f;
	mat->ambient[2] = 0.2f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}

/**
 * material for puppet's eye
 */
struct S_material *materialPuppetHawk(){

	struct S_material *mat;
	mat = checkMalloc(sizeof(struct S_material));

	// set material properties
	mat->shininess = 10.0f;

	// set color to yelow
	mat->color[0] = 0.1f;
	mat->color[1] = 0.1f;
	mat->color[2] = 0.1f;
	mat->color[3] = 1.0f;

	// ambient color of model
	mat->ambient[0] = 0.2f;
	mat->ambient[1] = 0.2f;
	mat->ambient[2] = 0.2f;
	mat->ambient[3] = 1.0f;

	// difuse color of material
	mat->specular[0] = 1.0f;
	mat->specular[1] = 1.0f;
	mat->specular[2] = 1.0f;
	mat->specular[3] = 1.0f;

	// textures
	mat->textueCount = 0;

	// shaders
	mat->shader = shaders[E_SH_NOTEX_SHADOW];

	return mat;
}
/**
 * Initialise all materials
 */
void initMaterials(){
	materials[E_MAT_PIANO_BODY] = materialPianoBody();
	materials[E_MAT_PIANO_STRINGS] = materialPianoStrings();
	materials[E_MAT_PIANO_WHITE_KEY] = materialPianoWhiteKey();
	materials[E_MAT_PIANO_BLACK_KEY] = materialPianoBlackKey();

	materials[E_MAT_PUPPET_WOOD] = materialPuppetWood();
	materials[E_MAT_PUPPET_EYE] = materialPuppetEye();
	materials[E_MAT_PUPPET_HAIR] = materialPuppetHawk();

	materials[E_MAT_TEXT] = materialText();

	materials[E_MAT_GROUND] = materialGround();
}