#ifdef YUP_INCLUDE_OPENVR
#ifdef YUP_INCLUDE_SDL

#include "VRSdlApp.h"

BEGIN_NAMESPACE_YUP

VRSdlApp::VRSdlApp(int argc, char *argv[])
	: SdlApp(argc, argv)
{
}


VRSdlApp::~VRSdlApp()
{
}

bool VRSdlApp::init()
{
	SdlApp::init();

	if (!createGLWindow("Viveclops", 100, 100, getWindowWidth(), getWindowHeight()))
		return false;

	if (!mVRManager.init() || !mVRManager.initGL())
		return false;

	// Initialize Renderable
	this->onInit();

	mVRManager.setDisplaySize(getWindowWidth(), getWindowHeight());
	mVRManager.setScene(this);

	return true;
}

bool VRSdlApp::update()
{
	mVRManager.RenderFrame();

	SDL_GL_SwapWindow(sdlWindow());

	mVRManager.UpdateHMDMatrixPose();

	return SdlApp::update();
}

void VRSdlApp::shutdown()
{
	mVRManager.shutdown();
	mVRManager.shutdownGL();

	// Shutdown Renderable
	this->onShutdown();

	SdlApp::shutdown();
}

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_SDL
#endif // YUP_INCLUDE_OPENVR
