/**
\file
 All texture manager operations
*/
#include "texturemanager.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "framework/util.h"


/**
 * Colorize function
 * linear interpolation between two colors
 */
void colorize(struct S_texture *texture, GLuint color0, GLuint color1){
	GLint X, Y;
	GLint size = texture->textureWidth * texture->textureHeight;

	//color0
	GLint r0 = (color0 >> 24) & 0xFF;
	GLint g0 = (color0 >> 16) & 0xFF;
	GLint b0 = (color0 >> 8) & 0xFF;
	GLint a0 = color0 & 0xFF;

	//color1
	GLint r1 = (color1 >> 24) & 0xFF;
	GLint g1 = (color1 >> 16) & 0xFF;
	GLint b1 = (color1 >> 8) & 0xFF;
	GLint a1 = color1 & 0xFF;


	for(GLint i = 0; i < size; ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;

		// save colorized values
		texture->tex[i * 4 + 0] = (r0 * (1.0f - texture->tex[i * 4 + 0]) + r1 * texture->tex[i * 4 + 0]) / 255.0f;
		texture->tex[i * 4 + 1] = (g0 * (1.0f - texture->tex[i * 4 + 1]) + g1 * texture->tex[i * 4 + 1]) / 255.0f;
		texture->tex[i * 4 + 2] = (b0 * (1.0f - texture->tex[i * 4 + 2]) + b1 * texture->tex[i * 4 + 2]) / 255.0f;
		texture->tex[i * 4 + 3] = (a0 * (1.0f - texture->tex[i * 4 + 3]) + a1 * texture->tex[i * 4 + 3]) / 255.0f;
	}
}

//////////////////////////////////////////////////////TEXTURE GENERATION//////////////////////////////////////////////////////
/**
 * Generate sinus texture
 * settings:settingX - frequency in X axis
 *	    settingY - frequency in Y axis
 * +-------->
 * |         X
 * |
 * v Y
 *
 * call generateSinePlasma(texture, freqX, freqY, offsetX, offsetY, 0x000000FF, 0xFFFFFFFF);
 */
void generateSinePlasma(struct S_texture *texture, GLint freqX, GLint freqY, GLint offsetX, GLint offsetY){
	GLint X,Y;
	GLint size = texture->textureWidth * texture->textureHeight;
	GLfloat value;

	// malloc space for result texture layer
	texture->tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 * sizeof(GLfloat));


	for(GLint i = 0; i < size; ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;

		value = sinf((X + offsetX) * (2 * M_PI / texture->textureWidth) * freqX) + sinf((Y + offsetY) * (2*M_PI/texture->textureHeight) * freqY) + 2;
		value /= 4;

		// save colorized values
		texture->tex[i * 4 + 0] = value;
		texture->tex[i * 4 + 1] = value;
		texture->tex[i * 4 + 2] = value;
		texture->tex[i * 4 + 3] = 1.0f;
	}

	return;
}

/**
 * Generate gradient sphere
 */
void generateGradientSphere(struct S_texture *texture, GLint offsetX, GLint offsetY){
	GLint X,Y;
	GLfloat value, Xd, Yd;

	// malloc space for result texture layer
	texture->tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 * sizeof(GLfloat));

	for(GLint i = 0; i < ((texture->textureWidth) * (texture->textureHeight)); ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;

		Xd = (GLfloat)(X - offsetX - texture->textureWidth / 2) / (texture->textureWidth / 2);
		Yd = (GLfloat)(Y - offsetY - texture->textureHeight / 2) / (texture->textureHeight / 2);

		value = mySqrt(Xd * Xd + Yd * Yd);

		// save colorized values
		texture->tex[i * 4 + 0] = value;
		texture->tex[i * 4 + 1] = value;
		texture->tex[i * 4 + 2] = value;
		texture->tex[i * 4 + 3] = 1.0f;
	}

}


/**
 * Generate seamless noise
 */
static GLfloat cosInterpolate(GLfloat first, GLfloat second, GLint remainder, GLfloat *interpolationTable){
	return ((first + interpolationTable[remainder] * (second - first)));
}

/* cubic interpolation may be used instead of cosInterpolate but cos is ok
 * static GLfloat cubicInterpolate(GLfloat first, GLfloat second, GLfloat third, GLfloat fourth, GLint remainder){
	GLfloat P = (fourth - third) - (first - second);
	GLfloat Q = (first - second) - P;
	GLfloat R = third - first;
	GLfloat S = second;

	return (P * pow(remainder, 3) + Q * pow(remainder, 2) + (R * remainder) + S);

}*/

/**
 * Generate seamless noise
 * freq must be 2 ^n
 */
void generateValueNoise(struct S_texture *texture, GLint freq, GLfloat amplitude){
	GLint step = (GLint)(texture->textureWidth / freq);
	if(step == 0) step = 1;
	GLint stepsX = (texture->textureWidth / step);
	GLint stepsY = (texture->textureHeight / step);
	GLint pointX, pointY, remainderX, remainderY;

	GLfloat value, tmp1, tmp2;

	// checkMalloc space for result texture layer
	texture->tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 * sizeof(GLfloat));

	//  fill in table containtng lattice points
	GLfloat *latticePointTable = checkMalloc(sizeof(GLfloat) * stepsX * stepsY);
	for(GLint i = 0; i < (stepsX * stepsY); ++i){
		latticePointTable[i] = amplitude * ((GLfloat)rand() / RAND_MAX);
	}

	// fill in interpolation table
	GLfloat *interpolationTable = checkMalloc(step * sizeof(GLfloat));
	for(GLint i = 0; i < step; ++i){
		interpolationTable[i] = (1 - cosf(i * M_PI / step)) / 2;
		//interpolationTable[i] = (i / (GLfloat)step);
	}

	for(GLint i = 0; i < texture->textureHeight; ++i){
		for(GLint j = 0; j < texture->textureWidth; ++j){
			// calculate distance from lattice points
			pointX = j / step;
			pointY = i / step;
			remainderX = j % step;
			remainderY = i % step;

			// calculate X interpolations
			tmp1 = cosInterpolate(latticePointTable[(stepsX * pointY) + pointX], latticePointTable[(stepsX * pointY) + ((pointX + 1) % stepsX)], remainderX, interpolationTable);
			tmp2 = cosInterpolate(latticePointTable[(stepsX * ((pointY + 1) % stepsX)) + pointX], latticePointTable[(stepsX * ((pointY + 1) % stepsX)) + ((pointX + 1) % stepsX)], remainderX, interpolationTable);

			// calculate Y interpolation
			value = cosInterpolate(tmp1, tmp2, remainderY, interpolationTable);

			// save colorized values
			texture->tex[(i * texture->textureWidth + j) * 4 + 0] = value;
			texture->tex[(i * texture->textureWidth + j) * 4 + 1] = value;
			texture->tex[(i * texture->textureWidth + j) * 4 + 2] = value;
			texture->tex[(i * texture->textureWidth + j) * 4 + 3] = 1.0f;
		}
	}

	free(latticePointTable);
	free(interpolationTable);
}

/**
 * Generate multioctave value noise
 */
void generateMultioctaveValueNoise(struct S_texture *texture, GLint octave, GLfloat persistence){
	GLint frequency = 1;
	GLfloat amplitude = 1;

	// checkMalloc space for result texture layer
	texture->tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 *sizeof(GLfloat));

	// create help ayer
	struct S_texture tmp;
	tmp.textureWidth = texture->textureWidth;
	tmp.textureHeight = texture->textureHeight;

	// fill result matrix with zeroes
	for(GLint i = 0; i < texture->textureWidth * texture->textureHeight; ++i){
		texture->tex[i * 4 + 0] = 0;
                texture->tex[i * 4 + 1] = 0;
                texture->tex[i * 4 + 2] = 0;
		texture->tex[i * 4 + 3] = 1.0f;
	}

	for(GLint i = 0; i < octave; ++i){
		frequency *= 2;
		amplitude *= persistence;

		generateValueNoise(&tmp, frequency, amplitude);

		for(GLint j = 0; j < texture->textureWidth * texture->textureHeight; ++j){
			texture->tex[j * 4 + 0] += tmp.tex[j * 4 + 0];
			texture->tex[j * 4 + 1] += tmp.tex[j * 4 + 1];
			texture->tex[j * 4 + 2] += tmp.tex[j * 4 + 2];
			texture->tex[j * 4 + 3]  = tmp.tex[j * 4 + 3];
		}

		// free tmp texture
		free(tmp.tex);
	}

	// average values in each pixel.. alpha color is the same
	for(GLint j = 0; j < texture->textureWidth * texture->textureHeight; ++j){
		texture->tex[j * 4 + 0] /= octave;
		texture->tex[j * 4 + 1] /= octave;
		texture->tex[j * 4 + 2] /= octave;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////DISTORTIONS//////////////////////////////////////////////////////////

/**
 * sine distortion
 */
void distortSine(struct S_texture *texture, GLfloat strength, GLint freqX, GLint freqY){
	GLint offset, X, Y;
	struct S_texture result;
	result.tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 * sizeof(GLfloat));

	for(GLint i = 0; i < texture->textureWidth * texture->textureHeight; ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;
		// determines offset of new value
		offset = ((sinf(X * (2 * M_PI / texture->textureWidth) * freqX) + sinf(Y * (2*M_PI/texture->textureHeight) * freqY) + 2 ) / 4) * strength;

		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] = texture->tex[(i * 4 + (offset * 4) + j) % (texture->textureHeight * texture->textureHeight * 4) + j];
		}
	}
	free(texture->tex);
	texture->tex = result.tex;
}

/**
 * marble disort
 * GLint marble(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat turbulence){
 *	 return (GLint)((sin(x + turbulence * internal_multioctave_abs_noise(x, y, z, w)) + 1.0) * 127.5);
 */
void distortMarble(struct S_texture *texture, GLfloat turbulence, GLfloat freqX, GLfloat freqY){
	GLint X, Y;


	for(GLint i = 0; i < texture->textureWidth * texture->textureHeight; ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;

		texture->tex[i * 4 + 0] = myFabs(2 * texture->tex[i * 4 + 0] - 1);

		GLfloat value = (1 + sinf(X * (2 * M_PI / texture->textureWidth) * freqX + (texture->tex[i * 4 + 0] + texture->tex[i * 4 + 1] + texture->tex[i * 4 + 2])  * turbulence)) / 2;

		for(GLint j = 0; j < 3; ++j){
			texture->tex[i * 4 + j] = value;
		}
	}
}

/**
 * Map distort
 * reslut is in texture1
 */
void distortMap(struct S_texture *texture, struct S_texture *map, GLfloat strength){
	GLint offset;

	struct S_texture result;
	result.tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 *sizeof(GLfloat));

	for(GLint i = 0; i < texture->textureWidth * texture->textureHeight; ++i){
		offset = (map->tex[i * 4 + 0] + texture->tex[i * 4 + 1] + texture->tex[i * 4 + 2]) * strength;
	//	printf("offset = %d \n", offset);

		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] = texture->tex[(i * 4 + (offset * 4) + j) % (texture->textureHeight * texture->textureHeight * 4)];
		}
	}

	free(texture->tex);
	texture->tex = result.tex;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////OPERATIONS//////////////////////////////////////////////////////////

/**
 * Add two layers
 * @param texture1 first texture to add
 * @param texture1 second texture to add
 * @result stored in texture1
 */
void opAdd(struct S_texture *texture1, struct S_texture *texture2){
	for(GLint i = 0; i < texture1->textureWidth * texture1->textureHeight; ++i){
		for(GLint j = 0; j < 3; ++j){
			texture1->tex[i * 4 + j] += texture2->tex[i * 4 + j];
			if(texture1->tex[i * 4 + j] > 1.0f) texture1->tex[i * 4 + j] = 1;
		}
	}
}

/**
 * SUB two layers
 *
 * texture1  = texture1 - texture2
 */
void opSub(struct S_texture *texture1, struct S_texture *texture2){
	for(GLint i = 0; i < (texture1->textureWidth * texture1->textureHeight); ++i){
		for(GLint j = 0; j < 3; ++j){
			texture1->tex[i * 4 + j] -= texture2->tex[i * 4 + j];
				if(MIN(texture1->tex[i * 4 + j], 0.0f)) texture1->tex[i * 4 + j] = 0;
		}
	}
}

/**
 * Mul two layers
 *
 * texture1  = texture1 * texture2
 */
void opMul(struct S_texture *texture1, struct S_texture *texture2){
	for(GLint i = 0; i < texture1->textureWidth * texture1->textureHeight; ++i){
		for(GLint j = 0; j < 3; ++j){
			texture1->tex[i * 4 + j] *= texture2->tex[i * 4 + j];
		}
	}
}

/**
 * Div two layers
 *
 * texture1  = texture1 / texture2
 */
void opDiv(struct S_texture *texture1, struct S_texture *texture2){
	for(GLint i = 0; i < texture1->textureWidth * texture1->textureHeight; ++i){
		for(GLint j = 0; j < 3; ++j){
			texture1->tex[i * 4 + j] /= texture2->tex[i * 4 + j] + 0.0000001;
		}
	}
}

/**
 * Neg layer
 */
void opNeg(struct S_texture *texture1){
	for(GLint i = 0; i < texture1->textureWidth * texture1->textureHeight; ++i){
		for(GLint j = 0; j < 3; ++j){
			texture1->tex[i * 4 + j] = 1.0f - texture1->tex[i * 4 + j];
		}
	}
}

/**
 * emboss layer
 * emboss matrix
 * -1 0 1
 * -1 0 1
 * -1 0 1
 */
void opEmboss(struct S_texture *texture){
	GLint X, Y, coordX, coordY;
	struct S_texture result;
	result.tex = checkMalloc(texture->textureWidth * texture->textureHeight * 4 *sizeof(GLfloat));

	for(GLint i = 0; i < texture->textureWidth * texture->textureHeight; ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;

		coordX = X - 1;
		coordY = Y - 1;
		// -1 mod 10 = -1!!! -> when X - 1  < 0 add textureWidth. This prevents wrong memory access :-)
		if(coordX < 0) coordX += texture->textureWidth;
		if(coordY < 0) coordY += texture->textureHeight;

		// top left point
		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] = texture->tex[(coordX % texture->textureWidth + (coordY % texture->textureHeight) * texture->textureWidth) * 4 + j] * (-1);
		}
		// center left point
		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] += texture->tex[(coordX % texture->textureWidth + Y * texture->textureWidth) * 4 + j] * (-1);
		}
		// bottom left point
		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] += texture->tex[(coordX % texture->textureWidth + ((Y + 1) % texture->textureHeight) * texture->textureWidth) * 4 + j] * (-1);
		}

		// top right point
		coordX = X + 1;
		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] += texture->tex[(coordX % texture->textureWidth + (coordY % texture->textureHeight) * texture->textureWidth) * 4 + j];
		}
		// center right point
		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] += texture->tex[(coordX % texture->textureWidth + Y * texture->textureWidth) * 4 + j];
		}
		// bottom right point
		coordY = Y + 1;
		for(GLint j = 0; j < 3; ++j){
			result.tex[i * 4 + j] += texture->tex[(coordX % texture->textureWidth + (coordY % texture->textureHeight) * texture->textureWidth) * 4 + j];
		}

	}
	opAdd(texture, &result);

	free(result.tex);
}


/**
 * create normal map from bump map
 * get surroundings from each point - 3x3 matrix
 * divide it to 4 2x2 matrix with point [1][1] in each matrix
 * calculate normal vector for each matrix (2x2)
 * point normal = matrix normals added and normalized, normal is stored in colors
 * @param texture source bump map overwritten by normal map
 */
void opCreateNormalMap(struct S_texture *texture){
	GLfloat n1[3], n2[3], n3[3], n4[3];
	GLfloat a[3], b[3], c[3]; // a - center point;
	GLfloat *tex;
	// result texture
	tex = checkMalloc(sizeof(GLfloat) * texture->textureHeight * texture->textureWidth * 4);

	GLint X,Y;

	// for each texel
	for(GLint i = 0; i < texture->textureWidth * texture->textureHeight; ++i){
		X = i % texture->textureWidth;
		Y = i / texture->textureWidth;
		a[0] = X;
		a[1] = Y;
		a[2] = texture->tex[i * 4];

		/*  ____ c ____
		 * |    | |    |
		 * | n1 | | n2 |
		 * |____| |____|
		 * b____ a ____b
		 * |    | |    |
		 * | n4 | | n3 |
		 * |____|c|____|
		 */

		/////////////////////////////////////////////////// calculate n1
		// B
		b[0] = X - 1;
		b[1] = Y;
		b[2] = texture->tex[((X == 0 ? texture->textureWidth - 1 : X - 1)  + Y * texture->textureWidth) * 4];

		// C
		c[0] = X;
		c[1] = Y - 1;
		c[2] = texture->tex[(X + (Y == 0 ? texture->textureHeight - 1 : Y - 1) * texture->textureWidth) * 4];


		// calculate vectors
		for (GLint k = 0; k < 3; ++k){
			b[k] = b[k] - a[k];
			c[k] = c[k] - a[k];
		}

		// store vector in n1
		CrossProduct(b, c, n1);

		/////////////////////////////////////////////////// calculate n2
		// B
		b[0] = X + 1;
		b[1] = Y;
		b[2] = texture->tex[((X + 1) % texture->textureWidth + Y * texture->textureWidth) * 4];

		// C
		c[0] = X;
		c[1] = Y - 1;
		c[2] = texture->tex[(X + (Y == 0 ? texture->textureHeight - 1 : Y - 1) * texture->textureWidth) * 4];

		// calculate vectors
		for (GLint k = 0; k < 3; ++k){
			b[k] = b[k] - a[k];
			c[k] = c[k] - a[k];
		}

		// store vector in n2
		CrossProduct(c, b, n2);

		/////////////////////////////////////////////////// calculate n3
		// B
		b[0] = X + 1;
		b[1] = Y;
		b[2] = texture->tex[((X + 1) % texture->textureWidth + Y * texture->textureWidth) * 4];

		// C
		c[0] = X;
		c[1] = Y + 1;
		c[2] = texture->tex[(X + ((Y + 1) % texture->textureHeight) * texture->textureWidth) * 4];

		// calculate vectors
		for (GLint k = 0; k < 3; ++k){
			b[k] = b[k] - a[k];
			c[k] = c[k] - a[k];
		}

		// store vector in n3
		CrossProduct(b, c, n3);

		/////////////////////////////////////////////////// calculate n4
		// B
		b[0] = X - 1;
		b[1] = Y;
		b[2] = texture->tex[((X == 0 ? texture->textureWidth - 1 : X - 1)  + Y * texture->textureWidth) * 4];

		// C
		c[0] = X;
		c[1] = Y + 1;
		c[2] = texture->tex[(X + ((Y + 1) % texture->textureHeight) * texture->textureWidth) * 4];

		// calculate vectors
		for (GLint k = 0; k < 3; ++k){
			b[k] = b[k] - a[k];
			c[k] = c[k] - a[k];
		}

		// store vector in n4
		CrossProduct(c, b, n4);

		//add them and normalize
		for (GLint j = 0; j < 3; ++j){
			n1[j] += n2[j] + n3[j] + n4[j];
		}
		VectorNormalize(n1);

		// transform from <-1,1> to <0,1> and store to colors
		for (GLint j = 0; j < 3; ++j){
			n1[j] = n1[j] * 0.5 + 0.5;
			tex[i * 4 + j] = n1[j];
		}
/*
		tex[i * 4 + 3] = 1;
		printf("%f, %f, %f\n", tex[i * 4 + 0], tex[i * 4 + 1], tex[i * 4 + 2]);
*/
	}
	free(texture->tex);
	texture->tex = tex;
}





/**
 *Function puts created picture into texture buffer
 */
void loadTexture(struct S_texture *texture){
	// generate buffer
	glGenTextures(1, &(texture->ID));
	//use buffer ID
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	// build our texture mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->textureWidth, texture->textureHeight, GL_RGBA, GL_FLOAT, texture->tex);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// the texture wraps over at the edges (repeat)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	free(texture->tex);

	return;
}

void destroyTexture(struct S_texture *texture){
	free(texture->tex);
}


/**
 * Initialize texture manager and create all textures
 */
void initTextureManager(){

	return;
}