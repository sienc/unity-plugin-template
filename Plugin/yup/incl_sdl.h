#pragma once

#ifdef YUP_INCLUDE_SDL

// Include math.h here so M_PI is defined before SDL tries to redefine it
//#include <math.h>

// SDL standard includes
#include <SDL.h>

#ifdef YUP_INCLUDE_GLEW
#include <GL/glew.h>
#include <GL/wglew.h>
#include <SDL_opengl.h>
#endif // YUP_INCLUDE_GLEW

//#include <SDL_syswm.h>

#else

#ifdef YUP_INCLUDE_GLEW
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#endif // YUP_INCLUDE_SDL