/*
 * ------------------------------------------------------------------------
 *  Name:   LogFile.h
 *  Desc:   ���ļ�������һ����־�ļ��ࡣ
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

	// ������־�ļ�
	bool CreateLog(const char* filename, const char* szHelloMsg);

	// д��
	void LogEvent(const char* szMsg, ...);

	// ��ȡ��־�ļ��Ķ���
	static LogFile& GetInstance();

protected:
	FILE* m_pFile;

	// ˽�й��캯��
	LogFile() : m_pFile(NULL) {}

	// ��ȡ��ǰʱ���
	const char* GetTimeStamp() const;
};

// 
#define FSW_LOG(str, ...) LogFile::GetInstance().LogEvent(str, __VA_ARGS__)

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __LOGFILE_H__