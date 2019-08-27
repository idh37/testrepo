#pragma once

/****************************************************
/                                                   /  
/ How Thread Loading(Please be sure to keep order)	/
/													/
/ 1. Registration load function						/
/ AddTask(Functor);									/
/													/
/ 2. Start a thread									/
/ BeginLoading();									/
/                                                   /
/***************************************************/

#pragma warning(disable: 4786)

#include "UtilDefine.h"
#include "ThreadQueue.h"

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS CStatisticsManager
		{
		public:
			CStatisticsManager(void);
			virtual ~CStatisticsManager(void);

		public:			
			void AddStatisticsURL(LPCTSTR pURL);					// 통계코드 URL을 입력한다.

		public:
			static CStatisticsManager& GetInstance();
			void LoaderThread_Entry();								// 내부클래스에서만 사용함.

			void DeleteFront();
			void Destroy();

		private:
			int SendStatisticData(LPCTSTR szURL);
			bool BeginManager();									// Start a thread

		protected:

			CThreadQueue<CString> m_StatisticsQueue;				// Statistics URL Queue

			CRITICAL_SECTION m_Tasks_cs;							// Critical Section for Access to task array

			unsigned long	m_nStatisticsThreadID;					// Statistics Thread ID

			HANDLE			m_hStatistics;							// Statistics Handle

			BOOL			m_bExit;								// Thread Loop Exit

			//HANDLE			m_hTime;							// Time Handle
		};

		NMUTIL_API CStatisticsManager& gStatisticsMan();

	}//namespace UTIL

}//namespace NMBASE