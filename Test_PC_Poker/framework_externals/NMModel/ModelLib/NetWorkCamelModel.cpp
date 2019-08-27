#include "stdafx.h"
#include "NetworkCamelModel.h"
#include "CommandDefine.h"

namespace NMBASE
{
	namespace MODEL
	{
		CNetWorkCamelModel& GCAMELNET()
		{
			return CNetWorkCamelModel::Get();
		}

		CNetWorkCamelModel& CNetWorkCamelModel::Get()
		{
			static CNetWorkCamelModel sSocketManager;
			return sSocketManager;
		}

		CNetWorkCamelModel::CNetWorkCamelModel()
		{
			UTIL::MM().AddNetMsgH(MSG_SOCKETCAMELINIT, UTIL::Fnt(this, &CNetWorkCamelModel::SocketInit));
		}

		CNetWorkCamelModel::~CNetWorkCamelModel()
		{				
		}

		///< 클라이언트용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)
		BOOL CNetWorkCamelModel::ConnectSocket(char *strIP, UINT nPortNum)
		{
			return SOCKCAMEL::GSOCKCAMELMAN().ConnectSocket(strIP, nPortNum);
		}

		BOOL CNetWorkCamelModel::SendData(void* rMessage)
		{
			Camel::Dar::IDarMsg* pData = (Camel::Dar::IDarMsg*)rMessage;
			return SOCKCAMEL::GSOCKCAMELMAN().Send(*pData);
		}

		LRESULT CNetWorkCamelModel::SocketInit(WPARAM& wParam, LPARAM& lParam)
		{
			HWND hWnd = (HWND)wParam;

			return SOCKCAMEL::GSOCKCAMELMAN().Init(hWnd);
		}

		void CNetWorkCamelModel::Disconnect()
		{
			return SOCKCAMEL::GSOCKCAMELMAN().Disconnect();
		}

		BOOL CNetWorkCamelModel::Reconnect()
		{
			return SOCKCAMEL::GSOCKCAMELMAN().Reconnect();
		}

		LPCTSTR CNetWorkCamelModel::GetServerIP()
		{
			return SOCKCAMEL::GSOCKCAMELMAN().GetIP();
		}
		
		UINT CNetWorkCamelModel::GetServerPort()
		{
			return SOCKCAMEL::GSOCKCAMELMAN().GetPORT();
		}


	}//namespace MODEL
}//namespace NMBASE
