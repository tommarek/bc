/**
\file
 Initialisation of OpenGL extensions
*/
#include "extensions.h"

#include "framework/util.h"

// VBO
PFNGLGENBUFFERSARBPROC			MYglGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC			MYglBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC			MYglBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC		MYglDeleteBuffersARB = NULL;
PFNGLBUFFERSUBDATAARBPROC		MYglBufferSubDataARB = NULL;
// shaders
PFNGLCREATEPROGRAMOBJECTARBPROC		MYglCreateProgramObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC		MYglDeleteObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC		MYglCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC		MYglShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC		MYglCompileShaderARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC	MYglGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC		MYglAttachObjectARB = NULL;
PFNGLGETINFOLOGARBPROC			MYglGetInfoLogARB = NULL;
PFNGLLINKPROGRAMARBPROC			MYglLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC		MYglUseProgramObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC		MYglGetUniformLocationARB = NULL;
PFNGLUNIFORM1FARBPROC			MYglUniform1fARB = NULL;
PFNGLUNIFORM3FARBPROC			MYglUniform3fARB = NULL;
PFNGLUNIFORM1IARBPROC			MYglUniform1iARB = NULL;

#ifndef NOSHADOW
// fbo
PFNGLGENFRAMEBUFFERSEXTPROC		MYglGenFramebuffersEXT = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC		MYglBindFramebufferEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	MYglFramebufferTexture2DEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	MYglCheckFramebufferStatusEXT = NULL;
#endif

void initExtensions(){
#ifndef NDEBUG
	//some opengl stuff you might want to know
	print("OpenGL: %s\n",(const char*)glGetString(GL_VERSION));
	int myMaxTextureUnits, myMaxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &myMaxTextureUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &myMaxTextureSize);
	print("Max Texture Units: %d, Max Texture Size: %d\n",myMaxTextureUnits,myMaxTextureSize);
#endif

	// check wether extensions are present
	const GLubyte* glExtensionList = glGetString(GL_EXTENSIONS);
	const char* neededExtensions[64] = {
		"GL_ARB_fragment_shader",
		"GL_ARB_fragment_program",
		"GL_ARB_shading_language_100",
		"GL_ARB_vertex_shader",
		"GL_ARB_shader_objects",
		"GL_ARB_vertex_buffer_object",
#ifndef NOSHADOW
		"GL_ARB_framebuffer_object",
#endif
	};
	for(int i = 0; neededExtensions[i] != NULL; i++)
		if (!gluCheckExtension((const GLubyte*)neededExtensions[i], glExtensionList))
			error("You're missing the OpenGL extension: %s\n\t-> Get a better gfx card :-)\n", neededExtensions[i]);


	MYglCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)engineGetProcAddress("glCreateProgramObjectARB");
	MYglDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)engineGetProcAddress("glDeleteObjectARB");
	MYglCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)engineGetProcAddress("glCreateShaderObjectARB");
	MYglShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)engineGetProcAddress("glShaderSourceARB");
	MYglCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)engineGetProcAddress("glCompileShaderARB");
	MYglGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)engineGetProcAddress("glGetObjectParameterivARB");
	MYglAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)engineGetProcAddress("glAttachObjectARB");
	MYglGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)engineGetProcAddress("glGetInfoLogARB");
	MYglLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)engineGetProcAddress("glLinkProgramARB");
	MYglUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)engineGetProcAddress("glUseProgramObjectARB");
	MYglGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)engineGetProcAddress("glGetUniformLocationARB");
	MYglUniform1fARB = (PFNGLUNIFORM1FARBPROC)engineGetProcAddress("glUniform1fARB");
	MYglUniform3fARB = (PFNGLUNIFORM3FARBPROC)engineGetProcAddress("glUniform3fARB");
	MYglUniform1iARB = (PFNGLUNIFORM1IARBPROC)engineGetProcAddress("glUniform1iARB");

	MYglGenBuffersARB = (PFNGLGENBUFFERSARBPROC)engineGetProcAddress("glGenBuffersARB");
	MYglBindBufferARB = (PFNGLBINDBUFFERARBPROC)engineGetProcAddress("glBindBufferARB");
	MYglBufferDataARB = (PFNGLBUFFERDATAARBPROC)engineGetProcAddress("glBufferDataARB");
	MYglDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)engineGetProcAddress("glDeleteBuffersARB");
	MYglBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC )engineGetProcAddress("glBufferSubDataARB");

#ifndef NOSHADOW
	MYglGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)engineGetProcAddress("glGenFramebuffersEXT");
	MYglBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)engineGetProcAddress("glBindFramebufferEXT");
	MYglFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)engineGetProcAddress("glFramebufferTexture2DEXT");
	MYglCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)engineGetProcAddress("glCheckFramebufferStatusEXT");
#endif
}
