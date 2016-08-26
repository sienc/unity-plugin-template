// ========================================================================== //
//
//  Matrice.cpp
//  ---
//  Based on OpenVR utilities
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#include "pathtools.h"

#if defined( _WIN32)
#include <Windows.h>
#include <direct.h>
#include <Shobjidl.h>
#include <KnownFolders.h>
#elif defined OSX
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include "osxfilebridge.h"
#define _S_IFDIR S_IFDIR     // really from tier0/platform.h which we dont have yet
#define _MAX_PATH MAX_PATH   // yet another form of _PATH define we use
#elif defined(LINUX)
#include <dlfcn.h>
#include <stdio.h>
#endif

#include <sys/stat.h>

#include <algorithm>


BEGIN_NAMESPACE_YUP_PATH

/** Returns the path (including filename) to the current executable */
ustring GetExecutablePath()
{
	bool bSuccess = false;
	uchar rchPath[ 1024 ];
	size_t nBuff = sizeof(rchPath);
#if defined( _WIN32 )
	bSuccess = ::GetModuleFileName(NULL, rchPath, (DWORD)nBuff) > 0;
#elif defined OSX
	uint32_t _nBuff = nBuff; 
	bSuccess = _NSGetExecutablePath(rchPath, &_nBuff) == 0;
	rchPath[nBuff-1] = '\0';
#elif defined LINUX
	ssize_t nRead = readlink("/proc/self/exe", rchPath, nBuff-1 );
	if ( nRead != -1 )
	{
		rchPath[ nRead ] = 0;
		bSuccess = true;
	}
	else
	{
		rchPath[ 0 ] = '\0';
	}
#else
	AssertMsg( false, "Implement Plat_GetExecutablePath" );
#endif

	if( bSuccess )
		return rchPath;
	else
		return TEXT("");
}

/** Returns the path of the current working directory */
ustring GetWorkingDirectory()
{
	ustring sPath;
	uchar buf[ 1024 ];
#if defined( _WIN32 )
	sPath = _wgetcwd( buf, sizeof( buf ) );
#else
	sPath = getcwd( buf, sizeof( buf ) );
#endif
	return sPath;
}

/** Sets the path of the current working directory. Returns true if this was successful. */
bool SetWorkingDirectory( const ustring & sPath )
{
	bool bSuccess;
#if defined( _WIN32 )
	bSuccess = 0 == _wchdir( sPath.c_str() );
#else
	bSuccess = 0 == chdir( sPath.c_str() );
#endif
	return bSuccess;
}

ustring GetModulePath()
{
#if defined( _WIN32 )
	uchar path[32768];
	HMODULE hm = NULL;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCSTR) &GetModulePath, 
		&hm))
	{
		int ret = GetLastError();
		fprintf(stderr, "GetModuleHandle returned %d\n", ret);
		return TEXT("");
	}
	GetModuleFileName(hm, path, sizeof(path));
	FreeLibrary( hm );
	return path;
#else
	Dl_info dl_info;
	dladdr((void *)GetModulePath, &dl_info);
	return dl_info.dli_fname;
#endif
}

/** Returns the specified path without its filename */
ustring StripFilename( const ustring & sPath, uchar slash )
{
	if( slash == 0 )
		slash = GetSlash();

	ustring::size_type n = sPath.find_last_of( slash );
	if( n == ustring::npos )
		return sPath;
	else
		return ustring( sPath.begin(), sPath.begin() + n );
}

/** returns just the filename from the provided full or relative path. */
ustring StripDirectory( const ustring & sPath, uchar slash )
{
	if( slash == 0 )
		slash = GetSlash();

	ustring::size_type n = sPath.find_last_of( slash );
	if( n == ustring::npos )
		return sPath;
	else
		return ustring( sPath.begin() + n + 1, sPath.end() );
}

/** returns just the filename with no extension of the provided filename. 
* If there is a path the path is left intact. */
ustring StripExtension( const ustring & sPath )
{
	for( ustring::const_reverse_iterator i = sPath.rbegin(); i != sPath.rend(); i++ )
	{
		if( *i == '.' )
		{
			return ustring( sPath.begin(), i.base() - 1 );
		}

		// if we find a slash there is no extension
		if( *i == '\\' || *i == '/' )
			break;
	}

	// we didn't find an extension
	return sPath;
}

/** returns just the filename with no extension of the provided filename.
* If there is a path the path is left intact. */
ustring GetExtension(const ustring & sPath)
{
	for (ustring::const_reverse_iterator i = sPath.rbegin(); i != sPath.rend(); i++)
	{
		if (*i == '.')
		{
			return ustring(i.base(), sPath.end());
		}

		// if we find a slash there is no extension
		if (*i == '\\' || *i == '/')
			break;
	}

	// we didn't find an extension
	return TEXT("");
}

bool IsAbsolute( const ustring & sPath )
{
	if( sPath.empty() )
		return false;

	if( sPath.find( ':' ) != ustring::npos )
		return true;

	if( sPath[0] == '\\' || sPath[0] == '/' )
		return true;

	return false;
}


/** Makes an absolute path from a relative path and a base path */
ustring MakeAbsolute( const ustring & sRelativePath, const ustring & sBasePath, uchar slash )
{
	if( slash == 0 )
		slash = GetSlash();

	if( IsAbsolute( sRelativePath ) )
		return sRelativePath;
	else
	{
		if( !IsAbsolute( sBasePath ) )
			return TEXT("");

		ustring sCompacted = Compact( Join( sBasePath, sRelativePath, slash ), slash );
		if( IsAbsolute( sCompacted ) )
			return sCompacted;
		else
			return TEXT("");
	}
}


/** Fixes the directory separators for the current platform */
ustring FixSlashes( const ustring & sPath, uchar slash )
{
	if( slash == 0 )
		slash = GetSlash();

	ustring sFixed = sPath;
	for( ustring::iterator i = sFixed.begin(); i != sFixed.end(); i++ )
	{
		if( *i == '/' || *i == '\\' )
			*i = slash;
	}

	return sFixed;
}


uchar GetSlash()
{
#if defined(_WIN32)
	return '\\';
#else
	return '/';
#endif
}

/** Jams two paths together with the right kind of slash */
ustring Join( const ustring & first, const ustring & second, uchar slash )
{
	if( slash == 0 )
		slash = GetSlash();

	// only insert a slash if we don't already have one
	ustring::size_type nLen = first.length();
#if defined(_WIN32)
	if( first.back() == '\\' || first.back() == '/' )
	    nLen--;
#else
	uchar last_uchar = first[first.length()-1];
	if (last_uchar == '\\' || last_uchar == '/')
	    nLen--;
#endif

	return first.substr( 0, nLen ) + ustring( 1, slash ) + second;
}


ustring Join( const ustring & first, const ustring & second, const ustring & third, uchar slash )
{
	return Join( Join( first, second, slash ), third, slash );
}

ustring Join( const ustring & first, const ustring & second, const ustring & third, const ustring &fourth, uchar slash )
{
	return Join( Join( Join( first, second, slash ), third, slash ), fourth, slash );
}

ustring Join( 
	const ustring & first, 
	const ustring & second, 
	const ustring & third, 
	const ustring & fourth, 
	const ustring & fifth, 
	uchar slash )
{
	return Join( Join( Join( Join( first, second, slash ), third, slash ), fourth, slash ), fifth, slash );
}

/** Removes redundant <dir>/.. elements in the path. Returns an empty path if the 
* specified path has a broken number of directories for its number of ..s */
ustring Compact( const ustring & sRawPath, uchar slash )
{
	if( slash == 0 )
		slash = GetSlash();

	ustring sPath = FixSlashes( sRawPath, slash );
	ustring sSlashString( 1, slash );

	// strip out all /./
	for( ustring::size_type i = 0; (i + 3) < sPath.length();  )
	{
		if( sPath[ i ] == slash && sPath[ i+1 ] == '.' && sPath[ i+2 ] == slash )
		{
			sPath.replace( i, 3, sSlashString );
		}
		else
		{
			++i;
		}
	}


	// get rid of trailing /. but leave the path separator
	if( sPath.length() > 2 )
	{
		ustring::size_type len = sPath.length();
		if( sPath[ len-1 ] == '.'  && sPath[ len-2 ] == slash )
		{
		  // sPath.pop_back();
		  sPath[len-1] = 0;  // for now, at least
		}
	}

	// get rid of leading ./ 
	if( sPath.length() > 2 )
	{
		if( sPath[ 0 ] == '.'  && sPath[ 1 ] == slash )
		{
			sPath.replace( 0, 2, TEXT(""));
		}
	}

	// each time we encounter .. back up until we've found the previous directory name
	// then get rid of both
	ustring::size_type i = 0;
	while( i < sPath.length() )
	{
		if( i > 0 && sPath.length() - i >= 2 
			&& sPath[i] == '.'
			&& sPath[i+1] == '.'
			&& ( i + 2 == sPath.length() || sPath[ i+2 ] == slash )
			&& sPath[ i-1 ] == slash )
		{
			// check if we've hit the start of the string and have a bogus path
			if( i == 1 )
				return TEXT("");
			
			// find the separator before i-1
			ustring::size_type iDirStart = i-2;
			while( iDirStart > 0 && sPath[ iDirStart - 1 ] != slash )
				--iDirStart;

			// remove everything from iDirStart to i+2
			sPath.replace( iDirStart, (i - iDirStart) + 3, TEXT(""));

			// start over
			i = 0;
		}
		else
		{
			++i;
		}
	}

	return sPath;
}

#define MAX_UNICODE_PATH			32768
#define MAX_UNICODE_PATH_IN_UTF8	( MAX_UNICODE_PATH * 4 )

/** Returns the path to the current DLL or exe */
ustring GetThisModulePath()
{
	// gets the path of vrclient.dll itself
#ifdef WIN32
	HMODULE hmodule = NULL;

	::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCTSTR>(GetThisModulePath), &hmodule);
 
	uchar *pchPath = new uchar[MAX_UNICODE_PATH];
	::GetModuleFileName(hmodule, pchPath, MAX_UNICODE_PATH);
	ustring sPath = pchPath;
	delete[] pchPath;
	return sPath;

	//wchar_t *pwchPath = new wchar_t[MAX_UNICODE_PATH];
	//char *pchPath = new char[ MAX_UNICODE_PATH_IN_UTF8 ];
	//::GetModuleFileNameW( hmodule, pwchPath, MAX_UNICODE_PATH );
	//WideCharToMultiByte( CP_UTF8, 0, pwchPath, -1, pchPath, MAX_UNICODE_IN_UTF8, NULL, NULL );
	//delete[] pwchPath;

	//ustring sPath = pchPath;
	//delete [] pchPath;
	//return sPath;

#elif defined( OSX ) || defined( LINUX )
	// get the addr of a function in vrclient.so and then ask the dlopen system about it
	Dl_info info;
	dladdr( (void *)GetThisModulePath, &info );
	return info.dli_fname;
#endif

}


/** returns true if the specified path exists and is a directory */
bool IsDirectory( const ustring & sPath )
{
	ustring sFixedPath = FixSlashes( sPath );
	if( sFixedPath.empty() )
		return false;
	uchar cLast = sFixedPath[ sFixedPath.length() - 1 ];
	if( cLast == '/' || cLast == '\\' )
		sFixedPath.erase( sFixedPath.end() - 1, sFixedPath.end() );

	// see if the specified path actually exists.
	struct _stat buf;
	if (_wstat( sFixedPath.c_str(), &buf ) == -1)
	{
		return false;
	}

#if defined(LINUX)
	return S_ISDIR( buf.st_mode );
#else
	return ( buf.st_mode & _S_IFDIR ) != 0;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns true if the the path exists
//-----------------------------------------------------------------------------
bool Exists( const ustring & sPath )
{
	ustring sFixedPath = FixSlashes( sPath );
	if( sFixedPath.empty() )
		return false;

	struct _stat buf;
	if ( _wstat( sFixedPath.c_str(), &buf ) == -1)
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: helper to find a directory upstream from a given path
//-----------------------------------------------------------------------------
ustring FindParentDirectoryRecursively( const ustring &strStartDirectory, const ustring &strDirectoryName )
{
	ustring strFoundPath = TEXT("");
	ustring strCurrentPath = FixSlashes( strStartDirectory );
	if ( strCurrentPath.length() == 0 )
		return TEXT("");

	bool bExists = Exists( strCurrentPath );
	ustring strCurrentDirectoryName = StripDirectory( strCurrentPath );
	if ( bExists && _wcsicmp( strCurrentDirectoryName.c_str(), strDirectoryName.c_str() ) == 0 )
		return strCurrentPath;

	while( bExists && strCurrentPath.length() != 0 )
	{
		strCurrentPath = StripFilename( strCurrentPath );
		strCurrentDirectoryName = StripDirectory( strCurrentPath );
		bExists = Exists( strCurrentPath );
		if ( bExists && _wcsicmp( strCurrentDirectoryName.c_str(), strDirectoryName.c_str() ) == 0 )
			return strCurrentPath;
	}

	return TEXT("");
}


//-----------------------------------------------------------------------------
// Purpose: helper to find a subdirectory upstream from a given path
//-----------------------------------------------------------------------------
ustring FindParentSubDirectoryRecursively( const ustring &strStartDirectory, const ustring &strDirectoryName )
{
	ustring strFoundPath = TEXT("");
	ustring strCurrentPath = FixSlashes( strStartDirectory );
	if ( strCurrentPath.length() == 0 )
		return TEXT("");

	bool bExists = Exists( strCurrentPath );
	while( bExists && strCurrentPath.length() != 0 )
	{
		strCurrentPath = StripFilename( strCurrentPath );
		bExists = Exists( strCurrentPath );

		if( Exists( Join( strCurrentPath, strDirectoryName ) ) )
		{
			strFoundPath = Join( strCurrentPath, strDirectoryName );
			break;
		}
	}
	return strFoundPath;
}


//-----------------------------------------------------------------------------
// Purpose: reading and writing files in the vortex directory
//-----------------------------------------------------------------------------
unsigned char * ReadBinaryFile( const ustring &strFilename, int *pSize )
{
	FILE *f;
#if defined( POSIX )
	f = fopen( strFilename.c_str(), "rb" );
#else
	errno_t err = _wfopen_s(&f, strFilename.c_str(), TEXT("rb"));
	if ( err != 0 )
	{
		f = NULL;
	}
#endif
	
	unsigned char* buf = NULL;

	if ( f != NULL )
	{
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);

		buf = new unsigned char[size];
		if (buf && fread(buf, size, 1, f) == 1)
		{
			if (pSize)
				*pSize = size;
		}
		else
		{
			delete[] buf;
			buf = 0;
		}

		fclose(f);
	}

	return buf;
}


ustring ReadTextFile( const ustring &strFilename )
{
	// doing it this way seems backwards, but I don't
	// see an easy way to do this with C/C++ style IO
	// that isn't worse...
	int size;
	unsigned char* buf = ReadBinaryFile( strFilename, &size );
	if (!buf)
		return TEXT("");

	// convert CRLF -> LF
	int outsize = 1;
	for (int i=1; i < size; i++)
	{
		if (buf[i] == '\n' && buf[i-1] == '\r') // CRLF
			buf[outsize-1] = '\n'; // ->LF
		else
			buf[outsize++] = buf[i]; // just copy
	}

	ustring ret((uchar *)buf, (uchar *)(buf + outsize));
	delete[] buf;
	return ret;
}


bool WriteStringToTextFile( const ustring &strFilename, const uchar *pchData )
{
	FILE *f;
#if defined( POSIX )
	f = fopen( strFilename.c_str(), "w" );
#else
	errno_t err = _wfopen_s(&f, strFilename.c_str(), TEXT("w"));
	if ( err != 0 )
	{
		f = NULL;
	}
#endif
	
	bool ok = false;

	if ( f != NULL )
	{
		ok = fputws( pchData, f) >= 0;
		fclose(f);
	}

	return ok;
}

END_NAMESPACE_YUP_PATH