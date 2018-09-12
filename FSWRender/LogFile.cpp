/*
 * ------------------------------------------------------------------------
 *  Name:   LogFile.cpp
 *  Desc:   本文件实现了一个日志文件对象。
 *  Author: Yish
 *  Date:   2013/4/26
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "LogFile.h"
#include <time.h>


// 获取日志对象
LogFile& LogFile::GetInstance()
{
	static LogFile obj;
	return obj;
}

/** 析构函数
*/
LogFile::~LogFile()
{
	if( m_pFile )
	{
		LogEvent("Log file closed successfully!");
		fclose(m_pFile);
	}
}

/** 创建一个日志文件
*/
bool LogFile::CreateLog(const char* filename, const char* szHelloMsg)
{
	if( m_pFile )
		fclose(m_pFile);

	m_pFile = fopen(filename, "w");
	if( !m_pFile ) return false;

	fprintf(m_pFile, "%s\n", szHelloMsg);
	fprintf(m_pFile, "Created on %s\n\n", GetTimeStamp());
	return true;
}

/** 写入日志内容
*/
void LogFile::LogEvent(const char* szMsg, ...)
{
	if( !m_pFile ) return;

	char szBuffer[4096];
	va_list args;
	va_start( args,szMsg );
	_vsnprintf( szBuffer,4096,szMsg,args );
	va_end( args );

	fprintf(m_pFile, "[%s] - %s\n", GetTimeStamp(), szBuffer);
}

const char* LogFile::GetTimeStamp() const
{
	static char TIME_STR[40];

	time_t ltime = time(0);
	tm* now = localtime(&ltime);
	_snprintf( TIME_STR,40,"%d/%d/%d %2d:%2d:%2d",now->tm_year+1900,now->tm_mon+1,
		now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec );
	return TIME_STR;
}