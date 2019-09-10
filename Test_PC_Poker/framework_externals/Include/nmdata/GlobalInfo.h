/**    
@file    GlobalInfo.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   ���ӿ��� �ʿ��� ������ �����ϴ� Ŭ����
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
			static CGlobalInfo* g_pGlobalInfo;											///< GINFO()�� ����� �� �ֵ��� �ϱ� ���� ������.

		public:

			CRunparamInfo* GetRunParm(){return m_pRunparamInfo;}	
			UTIL::CStringManager* GetStringManager(){return m_pStringManager;}


			LPCTSTR GetGameServerIP(){return (LPCTSTR)m_strGameServerIP;}
			void SetGameServerIP(LPCTSTR pIP){m_strGameServerIP = pIP;}

			UINT GetGameServerPort(){return m_strGameServerPort;}
			void SetGameServerPort(UINT nPort){m_strGameServerPort = nPort;}

			COleDateTime GetSyncServerTime();											///< ����ȭ�� ���� �ð��� ������ 

			void SetServerTimeGap(COleDateTimeSpan cTime){m_cServerTimeGap = cTime;}	///< ������ Ŭ���̾�Ʈ�� �ð���
			const COleDateTimeSpan GetServerTimeGap(){return m_cServerTimeGap;}


			LPCTSTR	GetChannelNotice(){return (LPCTSTR)m_strChannelNotice;}				///< ä�� ���� ����
			void	SetChannelNotice(LPCTSTR pNotice){m_strChannelNotice = pNotice;}	///< ä�� ���� ����

			DWORD GetStartTimeTick(){return m_dwStartTick;}				
			void  SetStartTimeTick(DWORD dwTime){m_dwStartTick = dwTime;}

			void  SetMasterServerName(LPCTSTR pName){m_strMasterServerName = (CString)pName;}
			char* GetMasterServerName(){return m_strMasterServerName.GetBuffer();}

			void  SetGameServerName(LPCTSTR pName){m_strGameServerName = (CString)pName;}
			char* GetGameServerName(){return m_strGameServerName.GetBuffer();}

		protected:

			CRunparamInfo*		m_pRunparamInfo;			///< Runparam����
			UTIL::CStringManager*		m_pStringManager;			///< ���ڿ� ������

			COleDateTimeSpan	m_cServerTimeGap;			///< ������ Ŭ���̾�Ʈ�� �ð���

			DWORD				m_dwStartTick;				///< �������� �ð� ���� ��
			DWORD				m_dwDebugStartTick;			///< �������� �ð� ���� ��

			CString				m_strChannelNotice;			///< ä�� ���� ����

			int					m_nMasterServerCode;		///< �����ͼ��� �ڵ�

			CString				m_strGameServerIP;			///< ���� ������ IP
			UINT				m_strGameServerPort;		///< ���� ������ Port

			CString				m_strMasterServerName;		///< ������ ���� �̸�
			CString				m_strGameServerName;		///< ���� ����  �̸�
		};


		NMDATA_API CGlobalInfo* GINFO();

	}//namespace DATA

}//namespace NMBASE
