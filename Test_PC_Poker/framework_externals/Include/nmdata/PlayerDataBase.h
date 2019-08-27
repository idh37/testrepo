/**    
@file    Avatar.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   게임에 입장한 Player 정보(게임에서는 클래스를 상속 받아 사용)
*/

#pragma once

#include "DataDefine.h"

namespace NMBASE
{
	namespace DATA
	{

		class AVATAR::CAvatar;

		class NMDATA_CLASS CPlayerDataBase
		{
		public:
			CPlayerDataBase(void);
			virtual ~CPlayerDataBase(void);

		public:
			virtual void Clear();
			virtual	void SetPlayerNum(int nNum);
			//	virtual void SetNewPlayer(USERINFO *pInput);		// 유저정보를 세팅한다.(서버에서 받음)
			virtual	void Reset();

			// 종환복구
			//virtual void SetChangeRoomInfo(CHANGE_USERINFO_ROOM &changeInfo);
			//virtual CHANGE_USERINFO_ROOM *GetChangeRoomInfo(void){return &m_cRoomChangeInfo;}

		public:
			int					m_nServerPlayerNum;			///< 서버 기준의 플레이어 번호
			int					m_nPlayerNum;				///< 플레이어 번호
			AVATAR::CAvatar*			m_pAvatar;					///< 아바타 클래스

			int					m_nGameJoinState;			///< 게임 참여 상태(0:불참, 1:참여)
			int					m_nGamePlayState;			///< 플레이 상태(0:게임종료, 1:게임중)

			int					m_nSndFxKind;				///< 
			int					m_nMyIpNumber;				///< 자신의 IP번호

			///< 종환복구
			//	INT64				m_n64RoundPrevMoney;		///< 판시작머니
			//	BOOL				m_bDie;						///< 

		protected:
			//CHANGE_USERINFO_ROOM m_cRoomChangeInfo;		///< 종환복구

		};

	}//namespace DATA

}//namespace NMBASE