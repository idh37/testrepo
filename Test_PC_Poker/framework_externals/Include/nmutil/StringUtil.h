#pragma once

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{
		NMUTIL_API LPCTSTR g_MakeCommaMoney(INT64 nMoney);
		NMUTIL_API int     g_NumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4);
		NMUTIL_API LPCTSTR g_MoneyMark(INT64 nMoney, int nUnit = 5);
		NMUTIL_API LPCTSTR g_HistoryMoneyMark(INT64 nMoney);
		NMUTIL_API LPCTSTR g_HistoryMoneyMarkEx(INT64 nMoney);
		NMUTIL_API LPCTSTR g_ProfileMoneyMark(INT64 nMoney);
		NMUTIL_API LPCTSTR g_HigherMonetaryunit(INT64 nMoney);
		NMUTIL_API LPCTSTR g_MakeShortMoneyStr(INT64 nMoney);		//머니스트링이 길면 짧게 만든다.
		NMUTIL_API LPCTSTR g_MakeShortMoneyStrEx(INT64 nMoney);		//머니스트링이 길면 짧게 만든다
		NMUTIL_API LPCTSTR g_MakeShortMoneyStrText(INT64 nMoney);		//머니스트링이 길면 짧게 만든다
		NMUTIL_API LPCTSTR g_NumberToHangul( INT64 number );

		NMUTIL_API char*   g_NumberToString(int  nNumber);			// 숫자를 문자열로 리턴
		NMUTIL_API char*   g_NumberToString(unsigned int nNumber);	// 숫자를 문자열로 리턴



	}//namespace UTIL

}//namespace NMBASE