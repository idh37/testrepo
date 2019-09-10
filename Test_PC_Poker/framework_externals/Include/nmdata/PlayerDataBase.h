/**    
@file    Avatar.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   ���ӿ� ������ Player ����(���ӿ����� Ŭ������ ��� �޾� ���)
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
			//	virtual void SetNewPlayer(USERINFO *pInput);		// ���������� �����Ѵ�.(�������� ����)
			virtual	void Reset();

			// ��ȯ����
			//virtual void SetChangeRoomInfo(CHANGE_USERINFO_ROOM &changeInfo);
			//virtual CHANGE_USERINFO_ROOM *GetChangeRoomInfo(void){return &m_cRoomChangeInfo;}

		public:
			int					m_nServerPlayerNum;			///< ���� ������ �÷��̾� ��ȣ
			int					m_nPlayerNum;				///< �÷��̾� ��ȣ
			AVATAR::CAvatar*			m_pAvatar;					///< �ƹ�Ÿ Ŭ����

			int					m_nGameJoinState;			///< ���� ���� ����(0:����, 1:����)
			int					m_nGamePlayState;			///< �÷��� ����(0:��������, 1:������)

			int					m_nSndFxKind;				///< 
			int					m_nMyIpNumber;				///< �ڽ��� IP��ȣ

			///< ��ȯ����
			//	INT64				m_n64RoundPrevMoney;		///< �ǽ��۸Ӵ�
			//	BOOL				m_bDie;						///< 

		protected:
			//CHANGE_USERINFO_ROOM m_cRoomChangeInfo;		///< ��ȯ����

		};

	}//namespace DATA

}//namespace NMBASE