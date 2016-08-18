#pragma once
#include <cstdio>
#include <cstdint>
#include <map>
#include <codecvt>

#include "yup.h"
#include "unichar.h"

#define LogPrint(format, ...)	yup::Log::Print(format, __VA_ARGS__)

#define LogE(format, ...)		yup::Log::PrintLog(yup::LL_ERROR, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogW(format, ...)		yup::Log::PrintLog(yup::LL_WARN, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogI(format, ...)		yup::Log::PrintLog(yup::LL_INFO, __FILE__, __func__, __LINE__, format, __VA_ARGS__)

#ifndef NDEBUG
#define LogD(format, ...)		yup::Log::PrintLog(yup::LL_DEBUG, __FILE__, __func__, __LINE__, format, __VA_ARGS__);
#define LogV(format, ...)		yup::Log::PrintLog(yup::LL_VERBOSE, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#else
#define LogD(format, ...)		;
#define LogV(format, ...)		;
#endif // NDEBUG

BEGIN_NAMESPACE_YUP

enum LogLevel
{
	LL_ERROR,
	LL_WARN,
	LL_INFO,
	LL_DEBUG,
	LL_VERBOSE
};

typedef uint8_t LogOutput;
#define LO_NONE 0
#define LO_STDOUT 1
#define LO_FILE 2
#define LO_DEBUGWINDOW 4
#define LO_ALL 0xFF

class Log
{
private:
	LogLevel mLogLevel;
	LogOutput mLogOutput;

	std::map<LogLevel, ustring> mLogLevelNames;

	ufstream mLogFile;

private:
	Log(LogLevel level = LL_VERBOSE)
		: mLogLevel(level)
	{
		mLogLevelNames[LL_ERROR] = TEXT("ERROR");
		mLogLevelNames[LL_WARN] = TEXT("WARN");
		mLogLevelNames[LL_INFO] = TEXT("INFO");
		mLogLevelNames[LL_DEBUG] = TEXT("DEBUG");
		mLogLevelNames[LL_VERBOSE] = TEXT("VERBOSE");

		mLogOutput = LO_STDOUT | LO_DEBUGWINDOW;
	}

	~Log() {
		if (mLogFile.is_open())
			mLogFile.close();
	}

	// singleton implementation
	static inline Log & Instance() {
		static Log instance;
		return instance;
	}

	void inline outputLog(LogLevel level, const char * file, const char * func, int line, const ustring & str);
	void inline output(const ustring & str);

public:
	Log(const Log &) = delete;
	Log & operator=(const Log &) = delete;

	static inline void SetLogLevel(LogLevel level) { Instance().mLogLevel = level; }
	static inline void SetLogOutput(LogOutput output) { Instance().mLogOutput = output; }
	static inline void AddLogOutput(LogOutput output) { Instance().mLogOutput = Instance().mLogOutput | output; }
	static inline void RemoveLogOutput(LogOutput output) { Instance().mLogOutput = Instance().mLogOutput & ~output; }

	static inline void OpenLogFile(const ustring & filename);
	static inline void CloseLogFile();

	static inline void PrintLog(LogLevel level, const char * file, const char * func, int line, const char *format, ...);
	static inline void PrintLog(LogLevel level, const char * file, const char * func, int line, const wchar_t *format, ...);

	static inline void Print(const char *format, ...);
	static inline void Print(const wchar_t *format, ...);
};

void Log::outputLog(LogLevel level, const char * file, const char * func, int line, const ustring & str)
{
	ustring fileStr = ToUString(file);

	ustringstream ss;

	ss << "[" << Instance().mLogLevelNames[level] << "] "
		<< fileStr.substr(fileStr.find_last_of(TEXT("\\/")) + 1, fileStr.length()) << " "
		<< "(" << line << ") => "
		<< func << ": "
		<< str << std::endl;

	output(ss.str());
}

void Log::output(const ustring & str)
{
	if (mLogOutput & LO_STDOUT)
		ucout << str;

	if (mLogOutput & LO_DEBUGWINDOW)
		OutputDebugString(str.c_str());

	if (mLogOutput & LO_FILE && mLogFile.is_open())
		mLogFile << str;
}

void Log::OpenLogFile(const ustring & filename)
{
	Instance().mLogFile.open(filename, ufstream::out);

#ifdef _UNICODE
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	Instance().mLogFile.imbue(utf8_locale);
#endif

	Instance().AddLogOutput(LO_FILE);
}

void Log::CloseLogFile()
{
	if (Instance().mLogFile.is_open())
	{
		Instance().mLogFile.close();
		Instance().RemoveLogOutput(LO_FILE);
	}
}


////////////////////////////////////////////////////////////////////////////////
// - Narrow version
////////////////////////////////////////////////////////////////////////////////

void Log::Print(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().output(str);
}

void Log::PrintLog(LogLevel level, const char * file, const char * func, int line, const char *format, ...)
{
	if (level > Instance().mLogLevel)
		return;

	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().outputLog(level, file, func, line, str);
}

////////////////////////////////////////////////////////////////////////////////
// - Wide version
////////////////////////////////////////////////////////////////////////////////

void Log::Print(const wchar_t *format, ...)
{
	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().output(str);
}

void Log::PrintLog(LogLevel level, const char * file, const char * func, int line, const wchar_t *format, ...)
{
	if (level > Instance().mLogLevel)
		return;

	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().outputLog(level, file, func, line, str);
}

END_NAMESPACE_YUP