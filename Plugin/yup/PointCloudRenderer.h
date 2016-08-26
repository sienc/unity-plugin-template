// ========================================================================== //
//
//  PointCloudRenderer.h
//  ---
//  A Renderable that renders point cloud
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_OPENVR

#include <vector>

#ifdef YUP_INCLUDE_OPENCV
#include <opencv2/core.hpp>
#endif // YUP_INCLUDE_OPENCV

#include "Renderable.h"
#include "VertexArray.h"

BEGIN_NAMESPACE_YUP_GL

class PointCloudRenderer : public Renderable, public SimpleVertexArray
{
public:
	enum RenderMode {
		Solid,
		Pos,
		Depth
	};

private:
	RenderMode mRenderMode = RenderMode::Solid;
	Vector4 mColor;

public:
	PointCloudRenderer() : mColor(0, 1, 0, 1) {}
	PointCloudRenderer(const Vector4 & color) : mColor(color) {}
	~PointCloudRenderer() {}

	void setRenderMode(const RenderMode & mode) { mRenderMode = mode; }
	void setColor(const Vector4 & color) { mColor = color; }

	// Renderable override
	virtual bool onInit() override { return VertexArray::init(); }
	virtual void onRender(Matrix4 &vpMat, int width, int height) override { onRenderEye(vr::Hmd_Eye::Eye_Left, vpMat); }
	virtual void onRenderEye(vr::Hmd_Eye nEye, Matrix4 &vpMat) override;
	virtual void onUpdate(const Matrix4 &headPose) override { VertexArray::update(); }
	virtual void onShutdown() override { VertexArray::shutdown(); }
};


class RgbPointCloudRenderer : public Renderable, public RgbVertexArray
{
public:
	RgbPointCloudRenderer() {}
	~RgbPointCloudRenderer() {}

	// Renderable override
	virtual bool onInit() override { return VertexArray::init(); }
	virtual void onRender(Matrix4 &vpMat, int width, int height) override { onRenderEye(vr::Hmd_Eye::Eye_Left, vpMat); }
	virtual void onRenderEye(vr::Hmd_Eye nEye, Matrix4 &vpMat) override;
	virtual void onUpdate(const Matrix4 &headPose) override { VertexArray::update(); }
	virtual void onShutdown() override { VertexArray::shutdown(); }
};

END_NAMESPACE_YUP_GL

#endif // YUP_INCLUDE_OPENVR
