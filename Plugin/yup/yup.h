#pragma once

//  |   |   |  4|   |   |   |  8|   |   |   | 12|   |   |   | 16|   |   |   | 20

//============================================================================//
//
// YH's Utilities for Prototyping
//
// Version: v0.0.1
// Updated: 2015-08-05
//
//============================================================================//

#define BEGIN_NAMESPACE_YUP namespace yup {
#define END_NAMESPACE_YUP }

#define BEGIN_NAMESPACE_YUP_GL namespace yup { namespace gl {
#define END_NAMESPACE_YUP_GL } }

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
