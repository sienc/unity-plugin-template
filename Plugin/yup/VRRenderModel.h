// ========================================================================== //
//
//  VRRenderModel.h
//  ---
//  OpenVR render models
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
#include <string>

#include "yup.h"
#include "inc_sdl.h"

BEGIN_NAMESPACE_YUP

class VRRenderModel
{
public:
	VRRenderModel(const std::string & sRenderModelName);
	~VRRenderModel();

	bool BInit(const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture);
	void Cleanup();
	void Draw();
	const std::string & GetName() const { return m_sModelName; }

private:
	GLuint m_glVertBuffer;
	GLuint m_glIndexBuffer;
	GLuint m_glVertArray;
	GLuint m_glTexture;
	GLsizei m_unVertexCount;
	std::string m_sModelName;
};

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_OPENVR
