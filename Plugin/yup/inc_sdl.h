#pragma once

#ifdef YUP_INCLUDE_SDL

// define HAVE_M_PI so SDL does not try to define M_PI before cmath
#define HAVE_M_PI

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
#endif // YUP_INCLUDE_GLEW

#endif // YUP_INCLUDE_SDL