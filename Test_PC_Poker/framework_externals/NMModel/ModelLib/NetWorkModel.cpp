#include "stdafx.h"
#include "NetworkModel.h"
#include "CommandDefine.h"

namespace NMBASE
{
	namespace MODEL
	{

		static SOCK::CSockCmnd m_cSockCmnd;	// 소켓 메세지 처리 클래스

		CNetWorkModel& GNET()
		{
			return CNetWorkModel::Get();
		}

		CNetWorkModel& CNetWorkModel::Get()
		{
			static CNetWorkModel sSocketManager;
			return sSocketManager;
		}

		CNetWorkModel::CNetWorkModel()
		{
			//m_pSockCmnd = NULL;	// 소켓 메세지 처리 클래스

			UTIL::MM().AddNetMsgH(MSG_SOCKETINIT, UTIL::Fnt(this, &CNetWorkModel::SocketInit));
		}

		CNetWorkModel::~CNetWorkModel()
		{	
			// 	if(m_pSockCmnd)
			// 	{
			// 		delete m_pSockCmnd;
			// 	}
		}

		///< 클라이언트용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)
		LPCTSTR CNetWorkModel::ConnectSocket(char *strIP, UINT nPortNum)
		{
			return SOCK::GSOCKMAN().ConnectSocket(strIP, nPortNum);
		}

		///< 소켓으로부터 데이터를 받는다
		int CNetWorkModel::ReceiveData(char* szSockName)
		{
			return SOCK::GSOCKMAN().ReceiveData(szSockName);
		}

		BOOL CNetWorkModel::SendData(char* szSockName, void *pdata, int size)
		{
			return SOCK::GSOCKMAN().SendData(szSockName, pdata, size);
		}

		BOOL CNetWorkModel::PostCloseMsg(char* szSockName)
		{
			return SOCK::GSOCKMAN().PostCloseMsg(szSockName);
		}

		///< 소켓 클래스ID로 소켓 삭제 - Overrided
		BOOL CNetWorkModel::CloseDataSocket(const char* szSockName)
		{
			return SOCK::GSOCKMAN().CloseDataSocket(szSockName);
		}

		void CNetWorkModel::Destroy()
		{
			return SOCK::GSOCKMAN().Destroy();
		}

		char* CNetWorkModel::GetHostIP()
		{
			return SOCK::GSOCKMAN().GetHostIP();
		}

		char* CNetWorkModel::GetHostDomain()
		{
			return SOCK::GSOCKMAN().GetHostDomain();
		}

		char* CNetWorkModel::GetHostPortStr()
		{
			return SOCK::GSOCKMAN().GetHostPortStr();
		}


		LRESULT CNetWorkModel::SocketInit(WPARAM& wParam, LPARAM& lParam)
		{
			/*m_pSockCmnd = new CSockCmnd();

			if(NULL == m_pSockCmnd)
			{
			return FALSE;
			}*/

			HWND hWnd = (HWND)wParam;

			return m_cSockCmnd.Init(CWnd::FromHandle(hWnd), &SOCK::GSOCKMAN());
		}

		LPCTSTR CNetWorkModel::GetServerIP(char* szSockName)
		{
			return SOCK::GSOCKMAN().GetServerIP(szSockName);
		}
		
		UINT CNetWorkModel::GetServerPort(char* szSockName)
		{
			return SOCK::GSOCKMAN().GetServerPort(szSockName);
		}


	}//namespace MODEL
}//namespace NMBASE
