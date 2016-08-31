#include "StdAfx.h"
#include "MTime.h"

namespace MFun
{
	CMTime::CMTime(void)
	{
		ZeroMemory(&m_stTime,sizeof MSYSTIME);
		ZeroMemory(&m_fileTime, sizeof MFILETIME);
	}

	CMTime::~CMTime(void)
	{
	}

	INT CMTime::MTimeCopy(LPMSYSTIME pDescTime, LPMSYSTIME pSourceTime)
	{
		CopyMemory(pDescTime,pSourceTime,sizeof MSYSTIME);
		return 0;
	}

	INT CMTime::MTimeCopy(LPSYSTEMTIME pDescTime, LPSYSTEMTIME pSourceTime)
	{
		CopyMemory(pDescTime, pSourceTime, sizeof SYSTEMTIME);
		return 0;
	}

	INT CMTime::SystemTime_Diff(SYSTEMTIME &st1, SYSTEMTIME &st2, SYSTEMTIME &st3 )
	{
		INT iRet(0);
		FILETIME ft1;
		SystemTimeToFileTime(&st1, &ft1);
		ULARGE_INTEGER const u1 = { ft1.dwLowDateTime, ft1.dwHighDateTime };
		FILETIME ft2;
		SystemTimeToFileTime(&st2, &ft2);
		ULARGE_INTEGER const u2 = { ft2.dwLowDateTime, ft2.dwHighDateTime };
		ULARGE_INTEGER u3;
		//u3.QuadPart = std::max(u1.QuadPart, u2.QuadPart) - std::min(u1.QuadPart, u2.QuadPart);
		//FILETIME const ft3 = { u3.LowPart, u3.HighPart };
		//FileTimeToSystemTime(&ft3, &st3);
		// FILETIME's starting point is 1601-01-01
		st3.wYear -= 1601;
		st3.wMonth -= 1;
		st3.wDay -= 1;
		st3.wDayOfWeek = 0; // useless for this purpose, make it always 0 so it's always ignored
		return iRet;
	}

	UINT64 CMTime::MTimeSystemTimeInterval(SYSTEMTIME & Time1, SYSTEMTIME & Time2, UINT uSecond)
	{
		UINT64 iRet(0);
		FILETIME ft1;
		SystemTimeToFileTime(&Time1, &ft1);
		FILETIME ft2;
		SystemTimeToFileTime(&Time2, &ft2);
		iRet = MTimeFileTimeInterval(ft1, ft2, uSecond);
		return iRet;
	}

	UINT64 CMTime::MTimeFileTimeInterval(FILETIME & fTime1, FILETIME & fTime2, UINT uSecond)
	{
		UINT64 iRet(0);

		UINT64 i641 = (UINT64)fTime1.dwHighDateTime*(0xFFFFFFFFui64 + 1ui64) + (UINT64)fTime1.dwLowDateTime;
		UINT64 i64a = *((UINT64*)(&fTime1));

		UINT64 i642 = (UINT64)fTime2.dwHighDateTime*(0xFFFFFFFFui64 + 1ui64) + (UINT64)fTime2.dwLowDateTime;
		UINT64 i64b = *((UINT64*)(&fTime2));

		LONG iCmp = ::CompareFileTime(&fTime1, &fTime2);
		switch (iCmp)
		{
		case 1:
			iRet = i641 - i642;
			break;
		case 0:
			iRet = 0;
			break;
		case -1:
			iRet = i642 - i641;
			break;
		default:
			break;
		}

		if (uSecond)
		{
			iRet = iRet / 10000000;
		}
		return iRet;
	}

	INT CMTime::MTimeCompare(LPMSYSTIME pDescTime, LPMSYSTIME pSourceTime)
	{
		INT iRet(0);
		FILETIME FTDesc,FTSource;
		::SystemTimeToFileTime(reinterpret_cast<LPSYSTEMTIME>(pDescTime),&FTDesc );
		if (pSourceTime)
		{
			//用传入时间
			::SystemTimeToFileTime(reinterpret_cast<LPSYSTEMTIME>(pSourceTime),&FTSource );
		}
		else
		{
			//取当前时间
			SYSTEMTIME stTime;
			::GetLocalTime(&stTime);
			::SystemTimeToFileTime(reinterpret_cast<LPSYSTEMTIME>(&stTime),&FTSource );
		}
		iRet = ::CompareFileTime(&FTDesc,&FTSource);
		/*
		Return value Description 
		-1 First file time is earlier than second file time.

		0 First file time is equal to second file time.

		1 First file time is later than second file time.
		*/
		return iRet;
	}

	INT CMTime::MTimeCompareInterval(LPMSYSTIME pDescTime,DWORD dwInterval , LPMSYSTIME pSourceTime )
	{
		INT iRet(0);
		//ULARGE_INTEGER uliDesc(0),uliSource(0);
		CTime tmLeft(*(LPSYSTEMTIME)pDescTime);
		CTime tmRight;
		if (pSourceTime)
		{
			tmRight = CTime(*(LPSYSTEMTIME)pSourceTime); 
		}
		else
		{
			//
			tmRight = CTime::GetCurrentTime();
		}

		CTimeSpan sp; 
		sp = tmLeft - tmRight;
		if (sp.GetTotalSeconds() > dwInterval)
		{
			iRet = 0;
		}
		else
		{
			iRet = 1;
		}
		return iRet;
	}

	LONGLONG CMTime::MTimeCompareInterval(LPMSYSTIME pDescTime, LPMSYSTIME pSourceTime)
	{
		LONGLONG iRet(0);
		LONGLONG lRet(0);
		//ULARGE_INTEGER uliDesc(0),uliSource(0);
		CTime tmLeft(*(LPSYSTEMTIME)pDescTime);
		CTime tmRight;
		if (pSourceTime)
		{
			tmRight = CTime(*(LPSYSTEMTIME)pSourceTime); 
		}
		else
		{
			//
			tmRight = CTime::GetCurrentTime();
		}

		CTimeSpan sp; 
		sp = tmLeft - tmRight;
		if (sp.GetTotalSeconds())
		{
			iRet = sp.GetTotalSeconds();
		}
		return iRet;
	}

	INT CMTime::GetLocalTime(LPMSYSTIME pTime)
	{
		INT iRet(0);
		if (pTime)
		{
			::GetLocalTime((LPSYSTEMTIME)pTime);
			iRet = 1;
		}
		return iRet;
	}

	INT CMTime::VerifyDate(INT   lMonth,INT   lDay,INT   lYear)   
	{   
		const   unsigned   char   LeapDay[12]   =       
		{   31,   29,   31,   30,   31,   30,   31,   31,   30,   31,   30,   31   };   

		const   unsigned   char   NotLeapDay[12]   =     
		{   31,   28,   31,   30,   31,   30,   31,   31,   30,   31,   30,   31   };   

		if(   (lYear   >   MAXYEAR)   ||   (lYear   <   MINYEAR   )   )   
		{   
			return   FALSE;   
		}   

		if(   (lMonth   >   12)   ||   (lMonth   <=   0)   )   
		{   
			return   FALSE;   
		}   

		if(isLeap(lYear))   
		{   
			/*is   leap   year*/   
			if(   (lDay   >   LeapDay[lMonth-1])   ||   (lDay   <=   0)   )   
			{   
				return   FALSE;   
			}   
		}   
		else   
		{   
			/*Is   Not   leap*/   
			if(   (lDay   >   NotLeapDay[lMonth-1])   ||   (lDay   <=   0)   )   
			{   
				return   FALSE;   
			}   
		}   
		return   TRUE;   
	}   

	INT CMTime::VerifyTime(INT Hour, INT Minute, INT Second)
	{
		if ((Hour > 23) || (Hour < 0))
		{
			return 0;
		}

		if ((Minute > 59) || (Minute < 0))
		{
			return 0;
		}

		if ((Second > 59) || (Second < 0))
		{
			return 0;
		}

		return 1;
	}

	INT CMTime::IsTimeValid(LPMSYSTIME pTime)
	{
		INT iRet(0);
		if (!pTime)
		{
			return iRet;
		}

		if (VerifyDate(pTime->wMonth,pTime->wDay,pTime->wYear) )
		{
			if (VerifyTime(pTime->wHour,pTime->wMinute,pTime->wSecond))
			{
				iRet = 1;
			}
		}

		return iRet;
	}

	INT CMTime::IsTimeValid(LPSYSTEMTIME pTime)
	{
		INT iRet(0);
		if (!pTime)
		{
			return iRet;
		}

		iRet = IsTimeValid(reinterpret_cast<LPMSYSTIME>(pTime));
		return iRet;
	}


	INT CMTime::SetSystemTime(LPMSYSTIME pSysTime)
	{
		INT iRet(0);
		if (!pSysTime)
		{
			return iRet;
		}

		SYSTEMTIME SystemTime;
		ZeroMemory(&SystemTime,sizeof SYSTEMTIME);
		CopyMemory(&SystemTime,pSysTime,sizeof SYSTEMTIME);
		//时间容错
		if ( IsTimeValid(pSysTime) )
		{
			if (::SetLocalTime(&SystemTime))
			{
				iRet = 1;
			}
		}

		return iRet;
	}

	INT CMTime::SetSystemTime(DWORD dwTime)
	{
		UINT64 uiCurTime, uiBaseTime, uiResult;
		SYSTEMTIME st;

		uiBaseTime = ((UINT64) HIGHTIME << 32) + LOWTIME;

		uiCurTime = (UINT64)dwTime * (UINT64)10000000;
		uiResult = uiBaseTime + uiCurTime;

		::FileTimeToSystemTime((LPFILETIME)&uiResult, &st);

		return ::SetSystemTime(&st);
	}


	INT CMTime::SetTime(LPMSYSTIME pMSysTime)
	{
		INT iRet(0);
		if (pMSysTime)
		{
			if (IsTimeValid(pMSysTime))
			{
				MTimeCopy(&m_stTime,pMSysTime);
				iRet = 1;
			}
		}
		return iRet;
	}

	INT CMTime::SetTime(LPMFILETIME pMFileTime)
	{
		INT iRet(0);
		if (pMFileTime)
		{
			MSYSTIME sTime;
			ZeroMemory(&sTime, sizeof MSYSTIME);
			::FileTimeToSystemTime((LPFILETIME)pMFileTime, (LPSYSTEMTIME)&sTime);
			if (IsTimeValid(&sTime))
			{
				MTimeCopy(&m_stTime, &sTime);
				CopyMemory(&m_fileTime, pMFileTime, sizeof MFILETIME);
				iRet = 1;
			}
		}
		return iRet;
	}

	INT CMTime::SetTime(time_t Seconds, INT isLocal)
	{
		INT iRet(0);
		m_timetTime = Seconds;
		//转换
		tm * p(NULL);
		if (isLocal)
		{
			p = localtime(&m_timetTime);
		}
		else
		{
			p = gmtime(&m_timetTime);
		}

		if (p)
		{
			m_stTime.wYear = p->tm_year + 1900;
			m_stTime.wMonth = p->tm_mon;
			m_stTime.wDay = p->tm_mday;
			m_stTime.wDayOfWeek = p->tm_wday;
			m_stTime.wHour = p->tm_hour;
			m_stTime.wMinute = p->tm_min;
			m_stTime.wSecond = p->tm_sec;
			m_stTime.wMilliseconds = p->tm_yday;	//Day in the year

			iRet = 1;
		}

		return iRet;
	}

	LPMSYSTIME CMTime::GetTime(void)
	{
		return LPMSYSTIME(&m_stTime);
	}

	LPMFILETIME CMTime::GetFileTime(void)
	{
		return LPMFILETIME(&m_fileTime);
	}
	
	//使用外部数据
	INT CMTime::UpdateSystemTime(DWORD dwDeviation,LPMSYSTIME pMSysTime)
	{
		INT iRet(0);
		LONGLONG dwInterval(0);
		dwInterval = MTimeCompareInterval(pMSysTime);
		MSYSTIME MSysTime;
		ZeroMemory(&MSysTime,sizeof MSYSTIME);
		//获取当前系统时间
		if (labs( dwInterval) >= dwDeviation)
		{
			//比较偏差
			//设置时钟
			//返回结果
			if (SetSystemTime(pMSysTime))
			{
				iRet = 1;
			}
		}

		return iRet;
	}

	//使用内部数据
	INT CMTime::UpdateSystemTime(DWORD dwDeviation)
	{
		//获取当前系统时间
		//比较偏差
		//设置时钟
		//返回结果
		return UpdateSystemTime(dwDeviation,&m_stTime);
	}

	LPMSYSTIME CMTime::GetNowTime(void)
	{
		if (CMTime::GetLocalTime(&m_stTime))
		{
			//
			return LPMSYSTIME(&m_stTime);
		}

		return LPMSYSTIME(NULL);
	}

	WORD CMTime::Year(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wYear;
		}
		return wRet;
	}

	WORD CMTime::Month(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wMonth;
		}
		return wRet;
	}

	WORD CMTime::Day(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wDay;
		}
		return wRet;
	}

	WORD CMTime::DayOfWeek(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wDayOfWeek;
		}
		return wRet;
	}

	WORD CMTime::Hour(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wHour;
		}
		return wRet;
	}

	WORD CMTime::Minute(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wMinute;
		}
		return wRet;
	}

	WORD CMTime::Second(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wSecond;
		}
		return wRet;
	}

	WORD CMTime::Milliseconds(void)
	{
		WORD wRet(0xFFFF);
		if (IsTimeValid(&m_stTime))
		{
			wRet = m_stTime.wMilliseconds;
		}
		return wRet;
	}

	WORD CMTime::Year(WORD Data)
	{
		m_stTime.wYear = Data;
		return m_stTime.wYear;
	}

	WORD CMTime::Month(WORD Data)
	{
		m_stTime.wMonth = Data;
		return m_stTime.wMonth;
	}

	WORD CMTime::Day(WORD Data)
	{
		m_stTime.wDay = Data;
		return m_stTime.wDay;
	}

	WORD CMTime::DayOfWeek(WORD Data)
	{
		m_stTime.wDayOfWeek = Data;
		return m_stTime.wDayOfWeek;
	}

	WORD CMTime::Hour(WORD Data)
	{
		m_stTime.wHour = Data;
		return m_stTime.wHour;
	}

	WORD CMTime::Minute(WORD Data)
	{
		m_stTime.wMinute = Data;
		return m_stTime.wMinute;
	}

	WORD CMTime::Second(WORD Data)
	{
		m_stTime.wSecond = Data;
		return m_stTime.wSecond;
	}

	WORD CMTime::Milliseconds(WORD Data)
	{
		m_stTime.wMilliseconds = Data;
		return m_stTime.wMilliseconds;
	}


	INT CMTime::SetTime(CMTime * pMTime)
	{
		INT iRet(0);
		if (pMTime)
		{
			iRet = SetTime(&pMTime->m_stTime);
		}
		return iRet;
	}

	INT CMTime::SetTime(LPCTSTR  pTime)
	{
		INT iRet(0);
		if (pTime)
		{
			COleDateTime T1;
			iRet = T1.ParseDateTime(pTime);
			T1.GetAsSystemTime( *(LPSYSTEMTIME)&m_stTime);
			//iRet = SetTime(&pMTime->m_stTime);
			USES_CONVERSION;
			DATE m_dt;
			_bstr_t bstrDate = pTime;
			HRESULT hr;// = VarDateFromStr(bstrDate, LANG_USER_DEFAULT, 0, &m_dt);
			if (FAILED(hr = VarDateFromStr(bstrDate, LANG_USER_DEFAULT, 0, &m_dt)))
			{
				if (hr == DISP_E_TYPEMISMATCH)
				{
					// Can't convert string to date, set 0 and invalidate
					m_dt = 0;
					return iRet;
				}
				else if (hr == DISP_E_OVERFLOW)
				{
					// Can't convert string to date, set -1 and invalidate
					m_dt = -1;
					return iRet;
				}
				else
				{
					// Can't convert string to date, set -1 and invalidate
					m_dt = -1;
					return iRet;
				}
			}

			iRet = TRUE;
			_variant_t vTime;
			vTime = m_dt;
			//SYSTEMTIME stTmp;
			VariantTimeToSystemTime(vTime, (LPSYSTEMTIME)&m_stTime);
		}
		return iRet;
	}

	CString & CMTime::GetTimeStr()
	{
		m_strTime.asCString().Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_stTime.wYear, m_stTime.wMonth, m_stTime.wDay, m_stTime.wHour, m_stTime.wMinute, m_stTime.wSecond);
		return m_strTime.asCString();
	}

}