// ========================================================================== //
//
//  VRManager.h
//  ---
//  OpenVR wrapper class
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

#include <vector>

#include "yup.h"
#include "inc_sdl.h"
#include "Renderable.h"
#include "Matrices.h"

#include "VRRenderModel.h"

#ifdef WIN32
#define INT2VOIDPTR(val)  ((const void *)(uint64_t)val)
#else
#define INT2VOIDPTR(val)  ((const void *)(uint32_t)val)
#endif

BEGIN_NAMESPACE_YUP

using namespace gl;

struct ControllerState
{
	int id = 0;
	Matrix4 pose;
	bool buttonPressed = false;

	ControllerState(int _id, const Matrix4 &_pose, bool _buttonPressed)
	{
		id = _id;
		pose = _pose;
		buttonPressed = _buttonPressed;
	}
};

/////////////////////////////////////////////////
//
// VR System
//
/////////////////////////////////////////////////
class VRManager
{
private:
	int mState;

	Renderable * mScene = nullptr;
	bool mIsGLReady = false;
	bool mShutdown = false;

	bool mShowControllers = false;

	int mControllerId = 0;
	bool mIsControllerButtonDown = false;

	bool mIsHmdConnected = false;

	int mDisplayWidth = 0;
	int mDisplayHeight = 0;
	float mDisplayFov = 46.4f; // 28mm by default
	float mNearClip = 0.1f;
	float mFarClip = 30.0f;

public:
	VRManager();
	~VRManager();

	bool init();
	bool initGL();
	void shutdown();
	void shutdownGL();

	void setScene(Renderable *scene) { mScene = scene; }
	void RenderFrame();
	void HandleInput();

	void UpdateHMDMatrixPose();

	const Matrix4 & getHMDPose() const { return m_mat4HMDPose; }
	const Matrix4 & getControllerPose() const { return m_mat4ControllerPose; }

	bool isSystemReady() const { return (m_pHMD != nullptr); }
	bool isGLReady() const { return mIsGLReady; }
	bool isRenderReady() const { return isSystemReady() && isGLReady(); }

	void showControllers(bool show) { mShowControllers = show; }

	const vr::TrackedDevicePose_t & getTrackedDevicePose(int deviceIndex) const { return m_rTrackedDevicePose[deviceIndex]; }

	void setDisplaySize(int width, int height) { mDisplayWidth = width; mDisplayHeight = height; }

	void SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
	VRRenderModel *FindOrLoadRenderModel(const char *pchRenderModelName);

private:
	void ProcessVREvent(const vr::VREvent_t & event);

	void SetupRenderModels();
	bool SetupStereoRenderTargets();
	void SetupCameras();

	void RenderStereoTargets();
	void RenderScene(vr::Hmd_Eye nEye);


	Matrix4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	Matrix4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	Matrix4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);

	Matrix4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);

	bool CreateAllShaders();

	void RenderDisplay();
	void DrawControllers();

	inline uint64_t ButtonMaskFromId(vr::EVRButtonId id) { return 1ull << id; }

	///////////////////////////////////////////////////
	// VR Rendering - Code from OpenVR sample
	///////////////////////////////////////////////////

private:
	vr::IVRSystem *m_pHMD;
	vr::IVRRenderModels *m_pRenderModels;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];


private:
	// OpenGL bookkeeping
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

	float m_fNearClip;
	float m_fFarClip;

	Matrix4 m_mat4HMDPose;
	Matrix4 m_mat4eyePosLeft;
	Matrix4 m_mat4eyePosRight;

	Matrix4 m_mat4ControllerPose;

	Matrix4 m_mat4ProjectionCenter;
	Matrix4 m_mat4ProjectionLeft;
	Matrix4 m_mat4ProjectionRight;

	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
	};
	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc);

	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	// RenderModels
	std::vector< VRRenderModel * > m_vecRenderModels;
	VRRenderModel *m_rTrackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];

	GLuint m_glControllerVertBuffer;
	GLuint m_unControllerVAO;
	unsigned int m_uiControllerVertcount;

	GLuint m_unControllerTransformProgramID;
	GLuint m_unRenderModelProgramID;

	GLint m_nControllerMatrixLocation;
	GLint m_nRenderModelMatrixLocation;

};

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_OPENVR
