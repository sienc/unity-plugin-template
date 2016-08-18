#ifdef YUP_INCLUDE_SDL

#include "SdlApp.h"

#include <cstdio>
#include "Log.h"

BEGIN_NAMESPACE_YUP

bool SdlApp::init()
{
	if (!App::init())
		return false;

	SDL_StartTextInput();
	return true;
}

bool SdlApp::update()
{
	handleInput();
	return exitFlag();
}

void SdlApp::shutdown()
{
	SDL_StopTextInput();

	if (mWindow != nullptr)
		SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

void SdlApp::setWindowSize(int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
}

void SdlApp::handleInput()
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		if (sdlEvent.type == SDL_QUIT)
			setExitFlag();
		else if (sdlEvent.type == SDL_KEYDOWN)
			onKeyDown(sdlEvent.key.keysym.sym);
		else if (sdlEvent.type == SDL_WINDOWEVENT)
		{
			switch (sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				// Handle resize
				mWindowWidth = sdlEvent.window.data1;
				mWindowHeight = sdlEvent.window.data2;
				onResize(mWindowWidth, mWindowHeight);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				draw();
			}

		}
	}
}

bool SdlApp::createWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	mWindowWidth = w;
	mWindowHeight = h;

	// Create an application window with the following settings:
	mWindow = SDL_CreateWindow(title, x, y, w, h, flags);

	// Check that the window was successfully made
	if (mWindow == NULL) {
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		setExitFlag();
		return false;
	}

	// We must call SDL_CreateRenderer in order for draw calls to affect this window.
	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

	// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	// Clear the entire screen to our selected color.
	SDL_RenderClear(mRenderer);

	// Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window.
	SDL_RenderPresent(mRenderer);

	return true;
}

#ifdef YUP_INCLUDE_GLEW

bool SdlApp::createGLWindow(const char * title, int x, int y, int w, int h, Uint32 flags)
{
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	mWindowWidth = w;
	mWindowHeight = h;
	uint32_t unWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | flags;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	if (mDebugOpenGL)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	mWindow = SDL_CreateWindow(title, x, y, mWindowWidth, mWindowHeight, unWindowFlags);
	if (mWindow == nullptr)
	{
		LogE("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	mGlContext = SDL_GL_CreateContext(mWindow);
	if (mGlContext == nullptr)
	{
		LogE("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum nGlewError = glewInit();
	if (nGlewError != GLEW_OK)
	{
		LogE("Error initializing GLEW! %s\n", glewGetErrorString(nGlewError));
		return false;
	}
	glGetError(); // to clear the error caused deep in GLEW

	if (SDL_GL_SetSwapInterval(mVBlank ? 1 : 0) < 0)
	{
		LogE("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

#endif // YUP_INCLUDE_GLEW

SDL_Renderer *SdlApp::getRenderer()
{
	return mRenderer;
}

void SdlApp::onKeyDown(SDL_Keycode key)
{
	if (key == SDLK_ESCAPE || key == SDLK_q)
		setExitFlag();
}

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_SDL