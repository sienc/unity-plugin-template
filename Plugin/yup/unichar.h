// ========================================================================== //
//
//  LoopThread.h
//  ---
//  Unified char and string types
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "yup.h"

#define UNICHAR_BUFFER_SIZE 1024

BEGIN_NAMESPACE_YUP

#ifdef _UNICODE

	typedef wchar_t uchar;
	typedef std::wstring ustring;
	typedef std::wstringstream ustringstream;
	typedef std::wfstream ufstream;

	static std::wostream & ucout = std::wcout;

	static inline ustring ToUString(const char *str)
	{
		wchar_t buffer[UNICHAR_BUFFER_SIZE];
		size_t ret = mbstowcs(buffer, str, sizeof(buffer));
		if (ret == UNICHAR_BUFFER_SIZE)
			buffer[UNICHAR_BUFFER_SIZE - 1] = '\0';

		return buffer;
	}

	static inline ustring ToUString(const char *format, va_list arg)
	{
		char str[UNICHAR_BUFFER_SIZE];
		vsnprintf(str, UNICHAR_BUFFER_SIZE, format, arg);

		return ToUString(str);
	}

	static inline ustring ToUString(const wchar_t *str)
	{
		return str;
	}

	static inline ustring ToUString(const wchar_t *format, va_list arg)
	{
		wchar_t str[UNICHAR_BUFFER_SIZE];
		vswprintf(str, UNICHAR_BUFFER_SIZE, format, arg);

		return str;
	}

#else

	typedef char uchar;
	typedef std::sstring ustring;
	typedef std::stringstream ustringstream;
	typedef std::fstream ufstream;

	static std::ostream & ucout = std::cout;

	static inline ustring ToUString(const char *str) constexpr
	{
		return str;
	}

	static inline ustring ToUString(const char *format, va_list arg)
	{
		char str[UNICHAR_BUFFER_SIZE];
		vsnprintf(str, UNICHAR_BUFFER_SIZE, format, arg);

		return str;
	}

	static inline ustring ToUString(const wchar_t *str)
	{
		char buffer[UNICHAR_BUFFER_SIZE];
		size_t ret = wcstombs(buffer, str, sizeof(buffer));
		if (ret == UNICHAR_BUFFER_SIZE)
			buffer[UNICHAR_BUFFER_SIZE - 1] = '\0';

		return buffer;
	}

	static inline ustring ToUString(const wchar_t *format, va_list arg)
	{
		wchar_t str[UNICHAR_BUFFER_SIZE];
		vswprintf(str, UNICHAR_BUFFER_SIZE, format, arg);

		return ToUString(str);
	}

#endif // _UNICODE

END_NAMESPACE_YUP