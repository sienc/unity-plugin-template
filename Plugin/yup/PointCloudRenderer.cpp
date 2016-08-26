// ========================================================================== //
//
//  PointCloudRenderer.cpp
//  ---
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#ifdef YUP_INCLUDE_OPENVR

#include "PointCloudRenderer.h"
#include "ShaderCollection.h"

BEGIN_NAMESPACE_YUP_GL

//============================================================================//
// PointCloudRenderer
//============================================================================//

void PointCloudRenderer::onRenderEye(vr::Hmd_Eye nEye, Matrix4 & vpMat)
{
	switch (mRenderMode)
	{
	case Pos:
		Shader.Pos.use();
		{
			Shader.Pos.MvpMatrix = vpMat;
			VertexArray::drawPoints();
		}
		Shader.Pos.unuse();
		break;

	case Depth:
		Shader.Depth.use();
		{
			Shader.Depth.MvpMatrix = vpMat;
			VertexArray::drawPoints();
		}
		Shader.Depth.unuse();
		break;

	case Solid:
	default:
		Shader.Solid.use();
		{
			Shader.Solid.MvpMatrix = vpMat;
			Shader.Solid.Color = mColor;
			VertexArray::drawPoints();
		}
		Shader.Solid.unuse();
	}

	Shader.Solid.use();
	{
		Shader.Pos.MvpMatrix = vpMat;
		VertexArray::drawPoints();
	}
	Shader.Pos.unuse();
}

//============================================================================//
// PointCloudRenderer
//============================================================================//

void RgbPointCloudRenderer::onRenderEye(vr::Hmd_Eye nEye, Matrix4 & vpMat)
{
	Shader.VColor.use();
	{
		Shader.VColor.MvpMatrix = vpMat;
		VertexArray::drawPoints();
	}
	Shader.VColor.unuse();
}


END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_OPENVR
