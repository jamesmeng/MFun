//*************************************************************************
//类    名：CMTime
//功能描述：时间基类封装
//命名空间：MFun
//作    者：蒙进
//日    期：2010-05-20
//修 改 人：
//日    期：
//版    本：version 1.0
//文 件 名：MTime.h
//库    名：M系列基础类库
//*************************************************************************
#pragma once

#include <time.h>
#include "MString.h"

namespace MFun
{
#pragma pack(push, 1) //保存对齐状态
	typedef struct _MSYSTIME 
	{ 
		WORD wYear; 
		WORD wMonth; 
		WORD wDayOfWeek; 
		WORD wDay; 
		WORD wHour; 
		WORD wMinute; 
		WORD wSecond; 
		WORD wMilliseconds; 
	} MSYSTIME, *LPMSYSTIME;

	typedef struct _MFILETIME
	{
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	} MFILETIME, *LPMFILETIME;
	//设置内存对齐方式
#pragma pack(pop)//恢复对齐状态


#define HIGHTIME		21968699 // 21968708 // Jan 1, 1900 FILETIME.highTime
#define LOWTIME			4259332096 // 1604626432 // Jan 1, 1900 FILETIME.lowtime
#define MAXYEAR			2030
#define MINYEAR			1900
#define   isLeap(y)     (   ((((y)   %   4)   ==   0)   &&   (((y)   %   100)   !=   0))   ||   (((y)   %   400)   ==   0)   )   

	class CMTime
	{
	public:
		CMTime(void);
		~CMTime(void);
		MFun::MSYSTIME	m_stTime;
		MFun::MFILETIME	m_fileTime;
		CMString	m_strTime;
		INT MTimeCopy(LPMSYSTIME pDescTime, LPMSYSTIME pSourceTime);
		INT MTimeCopy(LPSYSTEMTIME pDescTime, LPSYSTEMTIME pSourceTime);
		INT MTimeCompare(LPMSYSTIME pDescTime, LPMSYSTIME pSourceTime = NULL);
		INT MTimeCompareInterval(LPMSYSTIME pDescTime, DWORD dwInterval , LPMSYSTIME pSourceTime = NULL);
		LONGLONG MTimeCompareInterval(LPMSYSTIME pDescTime, LPMSYSTIME pSourceTime = NULL);
		INT GetLocalTime(LPMSYSTIME pTime);
		INT VerifyDate(INT   lMonth,INT   lDay,INT   lYear);
		INT IsTimeValid(LPMSYSTIME pTime);
		INT IsTimeValid(LPSYSTEMTIME pTime);
		INT SetSystemTime(LPMSYSTIME pSysTime);
		INT SetSystemTime(DWORD dwTime);
		INT VerifyTime(INT Hour, INT Minute, INT Second);
		INT SetTime(LPMSYSTIME pMSysTime);
		INT SetTime(LPMFILETIME pMFileTime);
		LPMSYSTIME GetTime(void);
		LPMFILETIME GetFileTime(void);
		INT UpdateSystemTime(DWORD dwDeviation = 1);
		INT UpdateSystemTime(DWORD dwDeviation ,LPMSYSTIME pMSysTime );
		LPMSYSTIME GetNowTime(void);
		WORD Year(void);
		WORD Month(void);
		WORD Day(void);
		WORD DayOfWeek(void);
		WORD Hour(void);
		WORD Minute(void);
		WORD Second(void);
		WORD Milliseconds(void);

		WORD Year(WORD Data);
		WORD Month(WORD Data);
		WORD Day(WORD Data);
		WORD DayOfWeek(WORD Data);
		WORD Hour(WORD Data);
		WORD Minute(WORD Data);
		WORD Second(WORD Data);
		WORD Milliseconds(WORD Data);
		INT SetTime(time_t Seconds, INT isLocal = 1);
		time_t m_timetTime;
		INT SetTime(CMTime * pMTime);
		INT SetTime(LPCTSTR  pTime);
		CString & GetTimeStr();

		INT SystemTime_Diff(SYSTEMTIME &st1, SYSTEMTIME &st2, SYSTEMTIME &st3);
		UINT64 MTimeFileTimeInterval(FILETIME & fTime1, FILETIME & fTime2, UINT uSecond = 1);
		UINT64 MTimeSystemTimeInterval(SYSTEMTIME & Time1, SYSTEMTIME & Time2, UINT uSecond = 1);
	};

}
//参考
// #ifndef _TM_DEFINED
// struct tm {
// 	int tm_sec;     /* seconds after the minute - [0,59] */
// 	int tm_min;     /* minutes after the hour - [0,59] */
// 	int tm_hour;    /* hours since midnight - [0,23] */
// 	int tm_mday;    /* day of the month - [1,31] */
// 	int tm_mon;     /* months since January - [0,11] */
// 	int tm_year;    /* years since 1900 */
// 	int tm_wday;    /* days since Sunday - [0,6] */
// 	int tm_yday;    /* days since January 1 - [0,365] */
// 	int tm_isdst;   /* daylight savings time flag */
// };
// #define _TM_DEFINED
// #endif