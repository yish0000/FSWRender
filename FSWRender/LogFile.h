/*
 * ------------------------------------------------------------------------
 *  Name:   LogFile.h
 *  Desc:   本文件定义了一个日志文件类。
 *  Author: Yish
 *  Date:   2013/4/26
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __LOGFILE_H__
#define __LOGFILE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"

///////////////////////////////////////////////////////////////////////////


class LogFile
{
public:
	~LogFile();

	// 创建日志文件
	bool CreateLog(const char* filename, const char* szHelloMsg);

	// 写入
	void LogEvent(const char* szMsg, ...);

	// 获取日志文件的对象
	static LogFile& GetInstance();

protected:
	FILE* m_pFile;

	// 私有构造函数
	LogFile() : m_pFile(NULL) {}

	// 获取当前时间戳
	const char* GetTimeStamp() const;
};

// 
#define FSW_LOG(str, ...) LogFile::GetInstance().LogEvent(str, __VA_ARGS__)

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __LOGFILE_H__