#include "stdafx.h"
#include "GlobalInfo.h"
#include "RunparamInfo.h"
#include "StringManager.h"

namespace NMBASE
{
	namespace DATA
	{

		CGlobalInfo* CGlobalInfo::g_pGlobalInfo = NULL;


		CGlobalInfo* GINFO()
		{
			if (!CGlobalInfo::g_pGlobalInfo)
				CGlobalInfo::g_pGlobalInfo = new CGlobalInfo;
			return CGlobalInfo::g_pGlobalInfo;
		}

		CGlobalInfo::CGlobalInfo()
		{	
			m_strGameServerPort = 0;

			///< Runparam
			m_pRunparamInfo = new CRunparamInfo();

			///< 문자열 관리자
			m_pStringManager = new UTIL::CStringManager();		
		}

		CGlobalInfo::~CGlobalInfo()
		{
			if(m_pRunparamInfo)
			{
				delete m_pRunparamInfo;
			}

			if(m_pStringManager)
			{
				delete m_pStringManager;
			}
		}


		COleDateTime CGlobalInfo::GetSyncServerTime()
		{
			// 동기화된 서버 시간을 가져옴 
			return COleDateTime::GetCurrentTime() - COleDateTimeSpan(0, 0, 0, (int)m_cServerTimeGap.GetTotalSeconds());
		}

	}//namespace DATA

}//namespace NMBASE
