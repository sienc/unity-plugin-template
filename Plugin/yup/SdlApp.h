// ========================================================================== //
//
//  SdlApp.h
//  ---
//  SDL enabled app
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_SDL

#include <atomic>

#include "yup.h"
#include "inc_sdl.h"
#include "App.h"

BEGIN_NAMESPACE_YUP

class SdlApp : public App
{
private:
	SDL_Window *mWindow = nullptr;

	// 2D window
	SDL_Renderer *mRenderer = nullptr;

	// OpenGL window
	SDL_GLContext mGlContext = nullptr;
	bool mDebugOpenGL = false;
	bool mVBlank = false;

	int mWindowWidth = 1280;
	int mWindowHeight = 720;

	std::atomic_bool mExitFlag = false;

public:
	SdlApp(int argc, char *argv[]) : yup::App(argc, argv) {}
	virtual ~SdlApp() {}

protected:
	virtual bool init() override;
	virtual bool update() override;
	virtual void shutdown() override;

public:
	int getWindowWidth() const { return mWindowWidth; }
	int getWindowHeight() const { return mWindowHeight; }

	void setWindowSize(int width, int height);

	SDL_Window *sdlWindow() { return mWindow; }

private:
	void handleInput();

protected:
	bool createWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
#ifdef YUP_INCLUDE_GLEW
	bool createGLWindow(const char *title, int x, int y, int w, int h, Uint32 flags = 0);
#endif // YUP_INCLUDE_GLEW
	SDL_Renderer *getRenderer();

	void setExitFlag() { mExitFlag = true; }
	bool exitFlag() const { return mExitFlag; }

	// virtual functions that needs overloading
protected:
	virtual void draw() {}
	virtual void onResize(int width, int height) {}
	virtual void onKeyDown(SDL_Keycode key);
};

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_SDL