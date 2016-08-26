// ========================================================================== //
//
//  VRSdlApp.h
//  ---
//  VR SDL app
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_OPENVR
#ifdef YUP_INCLUDE_SDL

#include "inc_sdl.h"
#include "SdlApp.h"
#include "VRManager.h"

BEGIN_NAMESPACE_YUP

using gl::Renderable;

class VRSdlApp : public SdlApp, public Renderable
{
private:
	VRManager mVRManager;

public:
	VRSdlApp(int argc, char *argv[]);
	~VRSdlApp();

protected:
	VRManager & vrManager() { return mVRManager; }

protected:
	// SdlApp override
	virtual bool init() override;
	virtual bool update() override;
	virtual void shutdown() override;

};

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_SDL
#endif // YUP_INCLUDE_OPENVR
