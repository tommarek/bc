#include "shaders.h"

#include "framework/materialmanager/materialmanager.h"

// vertex shaders
/**
 * classic phong vertex shader - using original texture coordinates
 */
const char* vPhong =

	"varying vec4 ShadowCoord;"
	"varying vec3 normal, lightVec, lightDir, eyeVec;"

	"const vec4 lightDirection = vec4(-1.0, -1.0, -1.0, 0.0);"
	"const vec4 lightPosition = vec4(15.0, 15.0, 15.0, 1.0);"

	"void main(){"
		"normal = gl_NormalMatrix * gl_Normal;" // in eye space

		"vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;" // in eye space
		"lightVec = ((gl_ModelViewMatrix * lightPosition) - vVertex).xyz;"// eye space
		"lightDir = (gl_ModelViewMatrix * lightDirection).xyz;"// eye space
		"eyeVec = -vVertex.xyz;"

		"ShadowCoord = gl_TextureMatrix[7] * gl_Vertex;"

		"gl_TexCoord[0] = gl_MultiTexCoord0;"

		"gl_Position = ftransform();"

	"}";
/**
 * shadow vertex shader - simple 
 */
const char* vShadow =
	"void main(){"
		"gl_Position = ftransform();"
	"}";

/**
 * classic phong vertex shader - sphere mapping
 * gl_TexCoord[0].st contains sphere mapping - changes according to camera position
 * gl_TexCoord[1].st contains original mapping
 */
const char* vPhongSphere =
		"varying vec3 normal, lightVec, eyeVec;"

		"void main(){"
			"normal = gl_NormalMatrix * gl_Normal;"

			"vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);" // = u
			"lightVec = vec3(gl_LightSource[0].position.xyz - vVertex);"
			"eyeVec = -vVertex;"
			"vec3 reflec = reflect( vVertex, normal );"


		 	"gl_TexCoord[1] = gl_MultiTexCoord0;"

			"float m = 2.0 * sqrt( reflec.x*reflec.x + reflec.y*reflec.y + (reflec.z+1.0)*(reflec.z+1.0) );"
			"gl_TexCoord[0].s = reflec.x/m + 0.5;"
			"gl_TexCoord[0].t = reflec.y/m + 0.5;"

			"gl_Position = ftransform();"
		"}";

/**
 * classic phong vertex shader - for bump mapping
 * gl_TexCoord[0].st original mapping
 */
const char* vPhongBump =
	"varying vec3 lightVec, eyeVec;"
	//"attribute vec3 vTangent;"

	"void main(void){"
		"vec3 vTangent;"
		"vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0));"
		"vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0));"
		"if(length(c1)>length(c2)){"
			"vTangent = c1;"
		"}"
		"else{"
			"vTangent = c2;"
		"}"
			"vTangent = normalize(vTangent);"

		"vec3 n = normalize(gl_NormalMatrix * gl_Normal);"
		"vec3 t = normalize(gl_NormalMatrix * vTangent);"
		"vec3 b = cross(n, t);"

		"vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);"
		"vec3 tmpVec = gl_LightSource[0].position.xyz - vVertex;"

		"lightVec.x = dot(tmpVec, t);"
		"lightVec.y = dot(tmpVec, b);"
		"lightVec.z = dot(tmpVec, n);"

		"tmpVec = -vVertex;"
		"eyeVec.x = dot(tmpVec, t);"
		"eyeVec.y = dot(tmpVec, b);"
		"eyeVec.z = dot(tmpVec, n);"

		"gl_TexCoord[0] = gl_MultiTexCoord0;"

		"gl_Position = ftransform();"
	"}";

// fragment shaders
/**
 * shadow fragment shader - simple
 */
const char* fShadow =
	"void main(){"
		"gl_FragColor = gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient;"
	"}";

/**
 * Phong shading fragment shader with 1 texture
 */
const char* fPhongNoTexShadow =
	// shadow mapping
	"uniform sampler2D ShadowMap;"
	"uniform float stepW, stepH;"
	"varying vec4 ShadowCoord;"
	// other
	"varying vec3 normal, lightVec, lightDir, eyeVec;"

	"float getShadow(){"
		"float i, j, distance;"
		"vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w;"
		"shadowCoordinateWdivide.z += 0.0005;"
		"vec2 texCoord = shadowCoordinateWdivide.st;"

		"float shadow = 0.0;"

		"for (i = -1.0; i <= 1.0; i += 1.0){"
			"for (j = -1.0; j <= 1.0; j += 1.0){"
				"distance = texture2D(ShadowMap, texCoord + vec2(i * stepW, j * stepH)).z;"
				"if (distance >= shadowCoordinateWdivide.z){"
					"shadow += 1.0;"
				"}"
			"}"
		"}"
		"shadow = shadow / 9.0;"
		"return shadow;"
	"}"

	"void main (void){"
		"vec4 final_color = vec4(0.0);"
		// ambient lighting
		"vec4 ambient = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);"

		// shadows
		"float shadow = 1.0;"

		"vec3 L = normalize(lightVec);"
		"vec3 D = normalize(lightDir);"

		"if (dot(-L, D) > gl_LightSource[0].spotCosCutoff){"
			"if (ShadowCoord.w > 0.0)"
				"shadow = getShadow();"
			"vec3 N = normalize(normal);"
			"float lambertTerm = dot(N,L);"
			"if(lambertTerm > 0.0){"
				"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;"

				"vec3 E = normalize(eyeVec);"
				"vec3 R = reflect(-L, N);"
				"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
				"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;"
			"}"
		"}"

		"gl_FragColor = final_color * shadow + ambient;"
	"}";

/**
 * Phong shading fragment shader with no texture and no shadow
 */
const char* fPhongNoTexNoShadow =
	// shadow mapping
	"uniform float stepW, stepH;"
	// other
	"varying vec3 normal, lightVec, lightDir, eyeVec;"

	"void main (void){"
		"vec4 final_color = vec4(0.0);"
		// ambient lighting
		"vec4 ambient = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);"

		"vec3 L = normalize(lightVec);"
		"vec3 D = normalize(lightDir);"

		"if (dot(-L, D) > gl_LightSource[0].spotCosCutoff){"
			"vec3 N = normalize(normal);"
			"float lambertTerm = dot(N,L);"
			"if(lambertTerm > 0.0){"
				"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;"

				"vec3 E = normalize(eyeVec);"
				"vec3 R = reflect(-L, N);"
				"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
				"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;"
			"}"
		"}"

		"gl_FragColor = final_color + ambient;"
	"}";

/**
 * Phong shading fragment shader with 1 texture
 */
const char* fPhong1tex =
	"uniform sampler2D tex0;"

	"varying vec3 normal, lightVec, eyeVec;"

	"const float cos_outer_cone_angle = 0.8;" // 36 degrees of outer angle

	"void main (void){"

		"vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient * texture2D(tex0, gl_TexCoord[0].st)) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);"
		"vec3 L = normalize(lightVec);"
		"vec3 D = normalize(gl_LightSource[0].spotDirection);"

		"float cos_cur_angle = dot(-L, D);"
		"float cos_inner_cone_angle = gl_LightSource[0].spotCosCutoff;"
		"float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;"

		"if (cos_cur_angle > cos_inner_cone_angle){"
			"vec3 N = normalize(normal);"

			"float lambertTerm = max( dot(N,L), 0.0);"
			"if(lambertTerm > 0.0){"
				"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;"

				"vec3 E = normalize(eyeVec);"
				"vec3 R = reflect(-L, N);"
				"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
				"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;"
			"}"
		"}"

		"else if(cos_cur_angle > cos_outer_cone_angle){"
			"float falloff = (cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle;"

			"vec3 N = normalize(normal);"

			"float lambertTerm = max( dot(N,L), 0.0);"
			"if(lambertTerm > 0.0){"
				"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm * falloff;"

				"vec3 E = normalize(eyeVec);"
				"vec3 R = reflect(-L, N);"
				"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
				"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular * falloff;"
			"}"
		"}"

		"gl_FragColor = final_color;"
	"}";

/**
 * Phong shading fragment shader with 1 texture and 1 bump map
 * may be used with vPhong
 */
const char* fPhongBump =
		"uniform sampler2D tex0, tex1;"
		"varying vec3 lightVec, eyeVec;"

		"const float cos_outer_cone_angle = 0.8;" // 36 degrees of outer angle

		"void main (void){"
			"vec3 bump = normalize((texture2D(tex1, gl_TexCoord[0].st).xyz - 0.5) * 2.0);"
			"vec4 final_color = (gl_FrontLightModelProduct.sceneColor + texture2D(tex0, gl_TexCoord[0].st)) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);"
			"vec3 L = normalize(lightVec);"
			"vec3 D = normalize(gl_LightSource[0].spotDirection);"

			"float cos_cur_angle = dot(-L, D);"
			"float cos_inner_cone_angle = gl_LightSource[0].spotCosCutoff;"
			"float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;"

			"vec3 N = normalize(bump);"

			"if (cos_cur_angle > cos_inner_cone_angle){"

				"float lambertTerm = max(dot(N,L), 0.0);"
				"if(lambertTerm > 0.0){"
					"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;"

					"vec3 E = normalize(eyeVec);"
					"vec3 R = reflect(-L, N);"
					"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
					"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;"
				"}"
			"}"

			"else if(cos_cur_angle > cos_outer_cone_angle){"
				"float falloff = (cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle;"

				"float lambertTerm = max(dot(N,L), 0.0);"
				"if(lambertTerm > 0.0){"
					"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm * falloff;"

					"vec3 E = normalize(eyeVec);"
					"vec3 R = reflect(-L, N);"
					"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
					"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular * falloff;"
				"}"
			"}"

			"gl_FragColor = final_color;"
		"}";



/**
 * Phong shading fragment shader with 1 texture and 1 bump map
 * may be used with vPhong
 */
const char* fPhongBumpNoTex =
		"uniform sampler2D tex0;"
		"varying vec3 lightVec, eyeVec;"

		"const float cos_outer_cone_angle = 0.8;" // 36 degrees of outer angle

		"void main (void){"
			"vec3 bump = normalize((texture2D(tex0, gl_TexCoord[0].st).xyz - 0.5) * 2.0);"
			"vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);"
			"vec3 L = normalize(lightVec);"
			"vec3 D = normalize(gl_LightSource[0].spotDirection);"

			"float cos_cur_angle = dot(-L, D);"
			"float cos_inner_cone_angle = gl_LightSource[0].spotCosCutoff;"
			"float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;"

			"vec3 N = normalize(bump);"

			"if (cos_cur_angle > cos_inner_cone_angle){"

				"float lambertTerm = max(dot(N,L), 0.0);"
				"if(lambertTerm > 0.0){"
					"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;"

					"vec3 E = normalize(eyeVec);"
					"vec3 R = reflect(-L, N);"
					"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
					"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;"
				"}"
			"}"

			"else if(cos_cur_angle > cos_outer_cone_angle){"
				"float falloff = (cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle;"

				"float lambertTerm = max(dot(N,L), 0.0);"
				"if(lambertTerm > 0.0){"
					"final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm * falloff;"

					"vec3 E = normalize(eyeVec);"
					"vec3 R = reflect(-L, N);"
					"float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );"
					"final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular * falloff;"
				"}"
			"}"

			"gl_FragColor = final_color;"
		"}";



/**
 * Initialize shaders
 */
void initShaders(){
	// shader for rendering shadows - very simple
	struct S_shader *shadow_shader;
	shadow_shader = checkMalloc(sizeof(struct S_shader));
	shaders[E_SH_SHADOW] = shadow_shader;
	shaders[E_SH_SHADOW]->sourceVert = vShadow;
	shaders[E_SH_SHADOW]->sourceFrag = fShadow;
	createShader(shaders[E_SH_SHADOW]);

	// piano ground shaders
	struct S_shader *notex;
	notex = checkMalloc(sizeof(struct S_shader));
	shaders[E_SH_NOTEX_SHADOW] = notex;
	shaders[E_SH_NOTEX_SHADOW]->sourceVert = vPhong;
#ifndef NOSHADOW
	shaders[E_SH_NOTEX_SHADOW]->sourceFrag = fPhongNoTexShadow;
#else
	shaders[E_SH_NOTEX_SHADOW]->sourceFrag = fPhongNoTexNoShadow;
#endif
	createShader(shaders[E_SH_NOTEX_SHADOW]);

	// text shaders
	struct S_shader *notexnosh;
	notexnosh = checkMalloc(sizeof(struct S_shader));
	shaders[E_SH_NOTEX_NOSHADOW] = notexnosh;
	shaders[E_SH_NOTEX_NOSHADOW]->sourceVert = vPhong;
	shaders[E_SH_NOTEX_NOSHADOW]->sourceFrag = fPhongNoTexNoShadow;
	createShader(shaders[E_SH_NOTEX_NOSHADOW]);

	return;
}
