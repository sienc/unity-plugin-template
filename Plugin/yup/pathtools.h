//========= Copyright Valve Corporation ============//
#pragma once

#include <string>

#ifndef StdString
#ifdef _UNICODE
typedef std::wstring StdString;
typedef wchar_t StdChar;
#else
typedef std::string StdString;
typedef char StdChar;
#endif // _UNICODE
#endif // !StdString

/** Returns the path (including filename) to the current executable */
StdString Path_GetExecutablePath();

/** Returns the path of the current working directory */
StdString Path_GetWorkingDirectory();

/** Sets the path of the current working directory. Returns true if this was successful. */
bool Path_SetWorkingDirectory( const StdString & sPath );

/** returns the path (including filename) of the current shared lib or DLL */
StdString Path_GetModulePath();

/** Returns the specified path without its filename.
* If slash is unspecified the native path separator of the current platform
* will be used. */
StdString Path_StripFilename( const StdString & sPath, StdChar slash = 0 );

/** returns just the filename from the provided full or relative path. */
StdString Path_StripDirectory( const StdString & sPath, StdChar slash = 0 );

/** returns just the filename with no extension of the provided filename. 
* If there is a path the path is left intact. */
StdString Path_StripExtension( const StdString & sPath );

// YHC: get file extension
StdString Path_GetExtension(const StdString & sPath);

/** Returns true if the path is absolute */
bool Path_IsAbsolute( const StdString & sPath );

/** Makes an absolute path from a relative path and a base path */
StdString Path_MakeAbsolute( const StdString & sRelativePath, const StdString & sBasePath, StdChar slash = 0 );

/** Fixes the directory separators for the current platform.
* If slash is unspecified the native path separator of the current platform
* will be used. */
StdString Path_FixSlashes( const StdString & sPath, StdChar slash = 0 );

/** Returns the path separator for the current platform */
StdChar Path_GetSlash();

/** Jams two paths together with the right kind of slash */
StdString Path_Join( const StdString & first, const StdString & second, StdChar slash = 0 );
StdString Path_Join( const StdString & first, const StdString & second, const StdString & third, StdChar slash = 0 );
StdString Path_Join( const StdString & first, const StdString & second, const StdString & third, const StdString &fourth, StdChar slash = 0 );
StdString Path_Join( 
	const StdString & first, 
	const StdString & second, 
	const StdString & third, 
	const StdString & fourth, 
	const StdString & fifth, 
	StdChar slash = 0 );


/** Removes redundant <dir>/.. elements in the path. Returns an empty path if the 
* specified path has a broken number of directories for its number of ..s.
* If slash is unspecified the native path separator of the current platform
* will be used. */
StdString Path_Compact( const StdString & sRawPath, StdChar slash = 0 );

/** returns true if the specified path exists and is a directory */
bool Path_IsDirectory( const StdString & sPath );

/** Returns the path to the current DLL or exe */
StdString GetThisModulePath();

/** returns true if the the path exists */
bool Path_Exists( const StdString & sPath );

/** Helper functions to find parent directories or subdirectories of parent directories */
StdString Path_FindParentDirectoryRecursively( const StdString &strStartDirectory, const StdString &strDirectoryName );
StdString Path_FindParentSubDirectoryRecursively( const StdString &strStartDirectory, const StdString &strDirectoryName );

/** Path operations to read or write text/binary files */
unsigned char * Path_ReadBinaryFile( const StdString &strFilename, int *pSize );
StdString Path_ReadTextFile( const StdString &strFilename );
bool Path_WriteStringToTextFile( const StdString &strFilename, const StdChar *pchData );

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
