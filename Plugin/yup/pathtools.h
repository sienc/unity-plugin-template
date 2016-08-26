// ========================================================================== //
//
//  Matrice.h
//  ---
//  Based on OpenVR utilities
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include "unichar.h"

BEGIN_NAMESPACE_YUP_PATH

using yup::uchar;
using yup::ustring;

/** Returns the path (including filename) to the current executable */
ustring GetExecutablePath();

/** Returns the path of the current working directory */
ustring GetWorkingDirectory();

/** Sets the path of the current working directory. Returns true if this was successful. */
bool SetWorkingDirectory( const ustring & sPath );

/** returns the path (including filename) of the current shared lib or DLL */
ustring GetModulePath();

/** Returns the specified path without its filename.
* If slash is unspecified the native path separator of the current platform
* will be used. */
ustring StripFilename( const ustring & sPath, uchar slash = 0 );

/** returns just the filename from the provided full or relative path. */
ustring StripDirectory( const ustring & sPath, uchar slash = 0 );

/** returns just the filename with no extension of the provided filename. 
* If there is a path the path is left intact. */
ustring StripExtension( const ustring & sPath );

// YHC: get file extension
ustring GetExtension(const ustring & sPath);

/** Returns true if the path is absolute */
bool IsAbsolute( const ustring & sPath );

/** Makes an absolute path from a relative path and a base path */
ustring MakeAbsolute( const ustring & sRelativePath, const ustring & sBasePath, uchar slash = 0 );

/** Fixes the directory separators for the current platform.
* If slash is unspecified the native path separator of the current platform
* will be used. */
ustring FixSlashes( const ustring & sPath, uchar slash = 0 );

/** Returns the path separator for the current platform */
uchar GetSlash();

/** Jams two paths together with the right kind of slash */
ustring Join( const ustring & first, const ustring & second, uchar slash = 0 );
ustring Join( const ustring & first, const ustring & second, const ustring & third, uchar slash = 0 );
ustring Join( const ustring & first, const ustring & second, const ustring & third, const ustring &fourth, uchar slash = 0 );
ustring Join( 
	const ustring & first, 
	const ustring & second, 
	const ustring & third, 
	const ustring & fourth, 
	const ustring & fifth, 
	uchar slash = 0 );


/** Removes redundant <dir>/.. elements in the path. Returns an empty path if the 
* specified path has a broken number of directories for its number of ..s.
* If slash is unspecified the native path separator of the current platform
* will be used. */
ustring Compact( const ustring & sRawPath, uchar slash = 0 );

/** returns true if the specified path exists and is a directory */
bool IsDirectory( const ustring & sPath );

/** Returns the path to the current DLL or exe */
ustring GetThisModulePath();

/** returns true if the the path exists */
bool Exists( const ustring & sPath );

/** Helper functions to find parent directories or subdirectories of parent directories */
ustring FindParentDirectoryRecursively( const ustring &strStartDirectory, const ustring &strDirectoryName );
ustring FindParentSubDirectoryRecursively( const ustring &strStartDirectory, const ustring &strDirectoryName );

/** Path operations to read or write text/binary files */
unsigned char * ReadBinaryFile( const ustring &strFilename, int *pSize );
ustring ReadTextFile( const ustring &strFilename );
bool WriteStringToTextFile( const ustring &strFilename, const uchar *pchData );

//-----------------------------------------------------------------------------
#if defined(WIN32)
#define DYNAMIC_LIB_EXT	".dll"
#ifdef _WIN64
#define PLATSUBDIR	"win64"
#else
#define PLATSUBDIR	"win32"
#endif
#elif defined(OSX)
#define DYNAMIC_LIB_EXT	".dylib"
#define PLATSUBDIR	"osx32"
#elif defined(LINUX)
#define DYNAMIC_LIB_EXT	".so"
#define PLATSUBDIR	"linux32"
#else
//#warning "Unknown platform for PLATSUBDIR"
#define PLATSUBDIR	"unknown_platform"
#endif

END_NAMESPACE_YUP_PATH