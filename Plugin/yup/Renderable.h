// ========================================================================== //
//
//  Renderable.h
//  ---
//  Base class for all renderable items in the GL space
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_OPENVR

#include <openvr.h>

#include "yup.h"
#include "inc_sdl.h"
#include "Matrices.h"

BEGIN_NAMESPACE_YUP_GL

// Represents a single renderable object in an OpenGL space
class Renderable
{
public:

	// Initialize the renderable object
	virtual bool onInit() { return true; }

	// Render into a flat display
	virtual void onRender(Matrix4 &vpMat, int width, int height) = 0;

	// Render into a a single eye of the VR display
	virtual void onRenderEye(vr::Hmd_Eye nEye, Matrix4 &vpMat) = 0;
	
	// Called by the parent once per frame
	virtual void onUpdate(const Matrix4 &headPose) {}
	
	// Stop all tasks and clean up
	virtual void onShutdown() {}
};

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_OPENVR
