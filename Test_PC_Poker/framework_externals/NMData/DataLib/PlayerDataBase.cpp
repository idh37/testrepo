#include "StdAfx.h"
#include "PlayerDataBase.h"

namespace NMBASE
{
	namespace DATA
	{

		CPlayerDataBase::CPlayerDataBase(void)
		{
			Clear();

			m_pAvatar = new AVATAR::CAvatar;
		}

		CPlayerDataBase::~CPlayerDataBase(void)
		{
			if(m_pAvatar)
			{
				delete m_pAvatar; 
			}
		}

		void CPlayerDataBase::Clear()
		{
			//	ZeroMemory(&m_sUserInfo, sizeof(m_sUserInfo));

			m_nServerPlayerNum = 0;
			m_nPlayerNum=0;
			m_nSndFxKind=0;
			m_nMyIpNumber=-1;
			//	m_n64RoundPrevMoney=0;// 종환복구
			m_nGameJoinState = 0;

			// 종환복구
			//ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));

			Reset();
		}
		void CPlayerDataBase::Reset()
		{
			m_nGameJoinState = 0;
			m_nGamePlayState = 0;
			//	m_bDie = 0;// 종환복구
		}

		void CPlayerDataBase::SetPlayerNum(int nNum)
		{
			m_nPlayerNum = nNum;
		}

		//
		// void CPlayerDataBase::SetNewPlayer(USERINFO *pInput)
		// {
		// 	ASSERT(pInput != NULL);
		// 	memcpy(&m_sUserInfo, pInput, sizeof(USERINFO));
		// }

		// 종환복구
		// void CPlayerDataBase::SetChangeRoomInfo(CHANGE_USERINFO_ROOM &changeInfo)
		// {
		// 	memcpy(&m_cRoomChangeInfo, &changeInfo, sizeof(CHANGE_USERINFO_ROOM));
		// }

	}//namespace DATA

}//namespace NMBASE
