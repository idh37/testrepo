/**    
@file    GameManager.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   �������� ������ �����ϴ� �޴���( ���� �ʿ������ ������ GlobalInfo������ ���Ѵ�.)
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
		@brief	�������� ������ �����ϴ� Ŭ����
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

			///< Virtual �Լ���(���ӿ��� �������̵��ؼ� ���)
			virtual void AddView(CNMViewBase *pGame){}				///< View�� �߰��Ѵ�.	
			virtual void AddGame(CNMViewBase *pGame){}				///< ������ �߰��Ѵ�.	

			CNMViewBase *GetView(string strGameName);				///< ���� ����

			virtual void Release();									///< 	

			virtual void BadUserReport();							///< �ҷ����� ����Ʈ

			virtual void SetCurrentGame(LPCTSTR pGameName);			///< ���� ������ �����Ѵ�.

			virtual void SetMyPlayerInfo(CPlayerDataBase *pPlayer);		///< �÷��̾� ������ �����Ѵ�.


			void	OnDraw();
			void	OnUpdate();

		protected:

			// GameName, GameView
			typedef map<string, CNMViewBase*>	GAMEMAP;
			typedef	GAMEMAP::iterator			GAMEMAP_IT;
			GAMEMAP					m_mapGamelist;				///< ���� ����Ʈ(���� ���� ������ �� �� �ִ�)

			//	CNMViewBase*			m_pCurrentGame;				///< ���� ����
			CString					m_strCurrentGameName;		///< ���� ���� ���� �̸�

			HANDLE					m_hMutex;					///< ���ؽ�	

			UTIL::CPlayTimeWarning*		m_pPlayTimeWarning;			///< ��ð� ���� �÷��� ��� �޽��� ���

			EGAMELOCATION			m_eGameLocation;			///< ���� ���� ��ġ

			//	CPlayerDataBase			*m_pMyPlayerInfo;			///< �ڽ��� ����

			// ��ȯ����
			//CDataUserList			m_DataUserList;				///< ���� ����Ʈ
			//CDataRoomList			m_DataRoomList;				///< �� ����Ʈ
			//CChannelList			m_cChannelList;				///< ä�� ����Ʈ

		};

	}//namespace FRAME
}//namespace NMBASE