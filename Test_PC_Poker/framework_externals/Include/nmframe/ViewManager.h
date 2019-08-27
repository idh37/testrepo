/**    
@file    GameManager.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   여러개의 게임을 관리하는 메니저( 봐서 필요없을것 같으면 GlobalInfo쪽으로 합한다.)
*/

#pragma once

#include "FrameDefine.h"
#include "CommonDefine.h"

#include <map>

//#include "DataRoomList.h"
//#include "DataUserList.h"

using namespace std;


namespace NMBASE
{
	namespace FRAME
	{

		class CNMViewBase;
		class UTIL::CPlayTimeWarning;
		class CPlayerDataBase;


		/**      
		@brief	여러개의 게임을 관리하는 클래스
		*/
		class NMFRAME_CLASS CViewManager
		{

		public:
			CViewManager(void);
			virtual ~CViewManager(void);

			//public:
			//	static	CViewManager& Get();

		public:	

			//	CPlayerDataBase* GetMyInfo(void){return m_pMyPlayerInfo;}

			///< Virtual 함수들(게임에서 오버라이딩해서 사용)
			virtual void AddView(CNMViewBase *pGame){}				///< View를 추가한다.	
			virtual void AddGame(CNMViewBase *pGame){}				///< 게임을 추가한다.	

			CNMViewBase *GetView(string strGameName);				///< 현재 게임

			virtual void Release();									///< 	

			virtual void BadUserReport();							///< 불량유저 리포트

			virtual void SetCurrentGame(LPCTSTR pGameName);			///< 현재 게임을 세팅한다.

			virtual void SetMyPlayerInfo(CPlayerDataBase *pPlayer);		///< 플레이어 정보를 세팅한다.


			void	OnDraw();
			void	OnUpdate();

		protected:

			// GameName, GameView
			typedef map<string, CNMViewBase*>	GAMEMAP;
			typedef	GAMEMAP::iterator			GAMEMAP_IT;
			GAMEMAP					m_mapGamelist;				///< 게임 리스트(여러 개의 게임이 들어갈 수 있다)

			//	CNMViewBase*			m_pCurrentGame;				///< 현재 게임
			CString					m_strCurrentGameName;		///< 현재 선택 게임 이름

			HANDLE					m_hMutex;					///< 뮤텍스	

			UTIL::CPlayTimeWarning*		m_pPlayTimeWarning;			///< 장시간 게임 플레이 경고 메시지 출력

			EGAMELOCATION			m_eGameLocation;			///< 게임 현재 위치

			//	CPlayerDataBase			*m_pMyPlayerInfo;			///< 자신의 정보

			// 종환복구
			//CDataUserList			m_DataUserList;				///< 유저 리스트
			//CDataRoomList			m_DataRoomList;				///< 룸 리스트
			//CChannelList			m_cChannelList;				///< 채널 리스트

		};

	}//namespace FRAME
}//namespace NMBASE