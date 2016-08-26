// ========================================================================== //
//
//  YH's Utilities for Prototyping
//
//  Version: v0.1.0
//  Updated: 2015-08-05
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //
//  |   |   |  4|   |   |   |  8|   |   |   | 12|   |   |   | 16|   |   |   | 20
// -------------------------------------------------------------------------- //

#pragma once

#define BEGIN_NAMESPACE_YUP namespace yup {
#define END_NAMESPACE_YUP }

#define BEGIN_NAMESPACE_YUP_GL namespace yup { namespace gl {
#define END_NAMESPACE_YUP_GL } }

#define BEGIN_NAMESPACE_YUP_PATH namespace yup { namespace path {
#define END_NAMESPACE_YUP_PATH } }

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
