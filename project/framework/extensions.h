#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

extern PFNGLGENBUFFERSARBPROC                   MYglGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC                   MYglBindBufferARB;
extern PFNGLBUFFERDATAARBPROC                   MYglBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC                MYglDeleteBuffersARB;
extern PFNGLBUFFERSUBDATAARBPROC                MYglBufferSubDataARB;

extern PFNGLCREATEPROGRAMOBJECTARBPROC		MYglCreateProgramObjectARB;
extern PFNGLDELETEOBJECTARBPROC			MYglDeleteObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC		MYglCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC			MYglShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC		MYglCompileShaderARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC		MYglGetObjectParameterivARB;
extern PFNGLATTACHOBJECTARBPROC			MYglAttachObjectARB;
extern PFNGLGETINFOLOGARBPROC			MYglGetInfoLogARB;
extern PFNGLLINKPROGRAMARBPROC			MYglLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC		MYglUseProgramObjectARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC		MYglGetUniformLocationARB;
extern PFNGLUNIFORM1FARBPROC			MYglUniform1fARB;
extern PFNGLUNIFORM3FARBPROC			MYglUniform3fARB;
extern PFNGLUNIFORM1IARBPROC			MYglUniform1iARB;

#ifndef NOSHADOW
extern PFNGLGENFRAMEBUFFERSEXTPROC		MYglGenFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC		MYglBindFramebufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC         MYglFramebufferTexture2DEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	MYglCheckFramebufferStatusEXT;
#endif

#if defined(X11)
#include <GL/glx.h>
#include <GL/glxext.h>
#define engineGetProcAddress(x) (*glXGetProcAddress)((const GLubyte*)(x))

#elif defined(WIN32)
#define engineGetProcAddress(x) wglGetProcAddress(x)

#endif

void initExtensions();
#endif
