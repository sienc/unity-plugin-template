// ========================================================================== //
//
//  Log.h
//  ---
//  Logging system
//  - Use LogX() macros the same way as printf() to log a formatted message
//  - Use LogPrint() macro to log a raw message
//  - Call Log::SetLogLevel() to filter messages
//  - Call Log::SetLogOutput() to control output channels
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <cstdio>
#include <cstdint>
#include <map>
#include <codecvt>

#include "yup.h"
#include "unichar.h"

#define LogRaw(format, ...)		yup::Log::PrintRaw(format, __VA_ARGS__)

#define LogE(format, ...)		yup::Log::Print(yup::Log::Error, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogW(format, ...)		yup::Log::Print(yup::Log::Warning, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogI(format, ...)		yup::Log::Print(yup::Log::Info, __FILE__, __func__, __LINE__, format, __VA_ARGS__)

#ifndef NDEBUG
#define LogD(format, ...)		yup::Log::Print(yup::Log::Debug, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogV(format, ...)		yup::Log::Print(yup::Log::Verbose, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#else
#define LogD(format, ...)		;
#define LogV(format, ...)		;
#endif // NDEBUG

#define DEFAULT_LOG_OUTPUT		Output::StdOut | Output::DebugWindow

BEGIN_NAMESPACE_YUP

typedef uint8_t LogOutput;

class Log
{
public:
	enum Level
	{
		Error,
		Warning,
		Info,
		Debug,
		Verbose
	};

	enum Output
	{
		None = 0,
		StdOut = 1,
		File = 2,
		DebugWindow = 4,
		All = 0xff
	};

private:

	Level mLevel;
	LogOutput mOutput;

	std::map<Level, ustring> mLevelNames;

	ufstream mLogFile;

private:
	Log(Level level = Level::Verbose)
		: mLevel(level)
	{
		mLevelNames[Level::Error] = TEXT("ERROR");
		mLevelNames[Level::Warning] = TEXT("WARNING");
		mLevelNames[Level::Info] = TEXT("INFO");
		mLevelNames[Level::Debug] = TEXT("DEBUG");
		mLevelNames[Level::Verbose] = TEXT("VERBOSE");

		mOutput = DEFAULT_LOG_OUTPUT;
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

	void inline writeLog(Level level, const char * file, const char * func, int line, const ustring & str);
	void inline write(const ustring & str);

public:
	Log(const Log &) = delete;
	Log & operator=(const Log &) = delete;

	static inline void SetLevel(Level level) { Instance().mLevel = level; }
	static inline void SetOutput(LogOutput output) { Instance().mOutput = output; }
	static inline void AddOutput(LogOutput output) { Instance().mOutput = Instance().mOutput | output; }
	static inline void RemoveOutput(LogOutput output) { Instance().mOutput = Instance().mOutput & ~output; }

	static inline void OpenFile(const ustring & filename);
	static inline void CloseFile();

	static inline void Print(Level level, const char * file, const char * func, int line, const char *format, ...);
	static inline void Print(Level level, const char * file, const char * func, int line, const wchar_t *format, ...);

	static inline void PrintRaw(const char *format, ...);
	static inline void PrintRaw(const wchar_t *format, ...);
};

void Log::writeLog(Level level, const char * file, const char * func, int line, const ustring & str)
{
	ustring fileStr = ToUString(file);

	ustringstream ss;

	ss << "[" << Instance().mLevelNames[level] << "] "
		<< fileStr.substr(fileStr.find_last_of(TEXT("\\/")) + 1, fileStr.length()) << " "
		<< "(" << line << ") => "
		<< func << ": "
		<< str << std::endl;

	write(ss.str());
}

void Log::write(const ustring & str)
{
	if (mOutput & Output::StdOut)
		ucout << str;

	if (mOutput & Output::DebugWindow)
		OutputDebugString(str.c_str());

	if (mOutput & Output::File && mLogFile.is_open())
		mLogFile << str;
}

void Log::OpenFile(const ustring & filename)
{
	Instance().mLogFile.open(filename, ufstream::out);

#ifdef _UNICODE
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	Instance().mLogFile.imbue(utf8_locale);
#endif

	Instance().AddOutput(Output::File);
}

void Log::CloseFile()
{
	if (Instance().mLogFile.is_open())
	{
		Instance().mLogFile.close();
		Instance().RemoveOutput(Output::File);
	}
}


// -------------------------------------------------------------------------- //
//  Narrow version
// -------------------------------------------------------------------------- //

void Log::PrintRaw(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().write(str);
}

void Log::Print(Level level, const char * file, const char * func, int line, const char *format, ...)
{
	if (level > Instance().mLevel)
		return;

	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().writeLog(level, file, func, line, str);
}

// -------------------------------------------------------------------------- //
//  Wide version
// -------------------------------------------------------------------------- //

void Log::PrintRaw(const wchar_t *format, ...)
{
	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().write(str);
}

void Log::Print(Level level, const char * file, const char * func, int line, const wchar_t *format, ...)
{
	if (level > Instance().mLevel)
		return;

	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().writeLog(level, file, func, line, str);
}

END_NAMESPACE_YUP