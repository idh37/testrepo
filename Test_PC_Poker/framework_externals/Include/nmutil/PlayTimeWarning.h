/**    
@file    PlayTimeWarning.h
@date    2011/2/15
@author  PJH(gamking@cj.com)
@brief   ��ð� ���� ������ ���� ��� �޽���
*/

#pragma once

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		class NMUTIL_CLASS CPlayTimeWarning  
		{
		public:
			CPlayTimeWarning();
			virtual ~CPlayTimeWarning();

		public:
			void Login();
			void EnterTheGame();
			void OnTimer();

		private:
			int m_nFirstPrint;
			DWORD m_dwLoginTime;
			int m_nLastPrintTime;

			CString m_strNotice[2];

		private:
			void PrintPerHour( int nHour );
		};

	}//namespace UTIL

}//namespace NMBASE