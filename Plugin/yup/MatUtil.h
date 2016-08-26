// ========================================================================== //
//
//  matutil.h
//  ---
//  Utility functions for matrix handling
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_OPENVR
#ifdef YUP_INCLUDE_OPENCV

#include <openvr.h>
#include <opencv2/core.hpp>

#include "yup.h"

using vr::HmdMatrix34_t;
using vr::HmdMatrix44_t;
using vr::HmdVector3_t;

using cv::Point3f;
using cv::Point2f;

BEGIN_NAMESPACE_YUP

typedef cv::Mat_<float> Matf;

static inline void OpenVRMat2CVMat(const HmdMatrix34_t &matPose, Matf &outMat)
{
	if (outMat.rows != 4 && outMat.cols != 4)
		outMat.create(4, 4);

	for (int i = 0; i < 3; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = matPose.m[i][0];
		rowi[1] = matPose.m[i][1];
		rowi[2] = matPose.m[i][2];
		rowi[3] = matPose.m[i][3];
	}

	outMat(3, 0) = 0.0f;
	outMat(3, 1) = 0.0f;
	outMat(3, 2) = 0.0f;
	outMat(3, 3) = 1.0f;
}

static inline Matf OpenVRMat2CVMat(const HmdMatrix34_t &matPose)
{
	Matf outMat(4, 4);

	for (int i = 0; i < 3; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = matPose.m[i][0];
		rowi[1] = matPose.m[i][1];
		rowi[2] = matPose.m[i][2];
		rowi[3] = matPose.m[i][3];
	}

	outMat(3, 0) = 0.0f;
	outMat(3, 1) = 0.0f;
	outMat(3, 2) = 0.0f;
	outMat(3, 3) = 1.0f;

	return outMat;
}

static inline void OpenVRMat2CVMat(const HmdMatrix44_t &mat44, Matf &outMat)
{
	if (outMat.rows != 4 && outMat.cols != 4)
		outMat.create(4, 4);

	for (int i = 0; i < 4; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = mat44.m[i][0];
		rowi[1] = mat44.m[i][1];
		rowi[2] = mat44.m[i][2];
		rowi[3] = mat44.m[i][3];
	}
}

static inline Matf OpenVRMat2CVMat(const HmdMatrix44_t &mat44)
{
	Matf outMat(4, 4);

	for (int i = 0; i < 4; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = mat44.m[i][0];
		rowi[1] = mat44.m[i][1];
		rowi[2] = mat44.m[i][2];
		rowi[3] = mat44.m[i][3];
	}

	return outMat;
}

static inline Matf pt2mat3(const Point3f &pt)
{
	Matf mat(3, 1);

	mat(0) = pt.x;
	mat(1) = pt.y;
	mat(2) = pt.z;

	return mat;
}

static inline Matf pt2mat4(const Point3f &pt)
{
	Matf mat(4, 1);

	mat(0) = pt.x;
	mat(1) = pt.y;
	mat(2) = pt.z;
	mat(3) = 1;

	return mat;
}

static inline Point3f vec32pt(const HmdVector3_t vec)
{
	return Point3f(vec.v[0], vec.v[1], vec.v[2]);
}

static inline Point3f mat2pt3(const Matf mat)
{
	return Point3f(mat(0), mat(1), mat(2));
}

END_NAMESPACE_YUP

#endif // YUP_INCLUDE_OPENVR
#endif // YUP_INCLUDE_OPENCV