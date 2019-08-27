/**    
@file    GlobalInfo.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   게임에서 필요한 정보를 관리하는 클래스
*/

#pragma once

#include "DataDefine.h"
#include <string>
#include <map>

namespace NMBASE
{
	namespace DATA
	{

		class CRunparamInfo;
		class UTIL::CStringManager;

		class NMDATA_CLASS CGlobalInfo
		{
		public:
			CGlobalInfo();
			virtual ~CGlobalInfo();

		public:
			static CGlobalInfo* g_pGlobalInfo;											///< GINFO()를 사용할 수 있도록 하기 위해 선언함.

		public:

			CRunparamInfo* GetRunParm(){return m_pRunparamInfo;}	
			UTIL::CStringManager* GetStringManager(){return m_pStringManager;}


			LPCTSTR GetGameServerIP(){return (LPCTSTR)m_strGameServerIP;}
			void SetGameServerIP(LPCTSTR pIP){m_strGameServerIP = pIP;}

			UINT GetGameServerPort(){return m_strGameServerPort;}
			void SetGameServerPort(UINT nPort){m_strGameServerPort = nPort;}

			COleDateTime GetSyncServerTime();											///< 동기화된 서버 시간을 가져옴 

			void SetServerTimeGap(COleDateTimeSpan cTime){m_cServerTimeGap = cTime;}	///< 서버와 클라이언트의 시간차
			const COleDateTimeSpan GetServerTimeGap(){return m_cServerTimeGap;}


			LPCTSTR	GetChannelNotice(){return (LPCTSTR)m_strChannelNotice;}				///< 채널 공지 내용
			void	SetChannelNotice(LPCTSTR pNotice){m_strChannelNotice = pNotice;}	///< 채널 공지 내용

			DWORD GetStartTimeTick(){return m_dwStartTick;}				
			void  SetStartTimeTick(DWORD dwTime){m_dwStartTick = dwTime;}

			void  SetMasterServerName(LPCTSTR pName){m_strMasterServerName = (CString)pName;}
			char* GetMasterServerName(){return m_strMasterServerName.GetBuffer();}

			void  SetGameServerName(LPCTSTR pName){m_strGameServerName = (CString)pName;}
			char* GetGameServerName(){return m_strGameServerName.GetBuffer();}

		protected:

			CRunparamInfo*		m_pRunparamInfo;			///< Runparam정보
			UTIL::CStringManager*		m_pStringManager;			///< 문자열 관리자

			COleDateTimeSpan	m_cServerTimeGap;			///< 서버와 클라이언트의 시간차

			DWORD				m_dwStartTick;				///< 서버와의 시간 설정 값
			DWORD				m_dwDebugStartTick;			///< 서버와의 시간 설정 값

			CString				m_strChannelNotice;			///< 채널 공지 내용

			int					m_nMasterServerCode;		///< 마스터서버 코드

			CString				m_strGameServerIP;			///< 게임 서버의 IP
			UINT				m_strGameServerPort;		///< 게임 서버의 Port

			CString				m_strMasterServerName;		///< 마스터 서버 이름
			CString				m_strGameServerName;		///< 게임 서버  이름
		};


		NMDATA_API CGlobalInfo* GINFO();

	}//namespace DATA

}//namespace NMBASE
