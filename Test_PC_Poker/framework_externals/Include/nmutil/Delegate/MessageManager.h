/**    
@file    MessageManager.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   메시지 관리자(Command, Functor)쌍으로 관리 된다.
@brief   메모리는 소멸자에서 자동 클리어 된다.
*/

#pragma once

#include "../UtilDefine.h"
#include "Delegate.h"
#include "IComponent.h"
#include <map>
#include <string>
#include <vector>
#include <queue>

namespace NMBASE
{
	namespace UTIL
	{

		class NMUTIL_CLASS CMessageManager : public IComponent
		{
		protected:
			CRITICAL_SECTION	m_cLock;

		public:

			///< 일반 메시지
			BOOL AddMsgH		(std::string stCommand, IFunctor* pFunctor);
			BOOL AppendMsgH		(std::string stCommand, IFunctor* pFunctor);
			BOOL OverrideMsgH	(std::string stCommand, IFunctor* pFunctor);
			LRESULT	Call		(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);
			LRESULT	CallDefault	(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);

			BOOL RemoveMsgH		   (std::string stCommand);
			BOOL RemoveAppendMsgH  (std::string stCommand, IFunctor* pFunctor);	// Appand한 메시지 중 선택한 삭제한다.
			BOOL RemoveOverrideMsgH(std::string stCommand, IFunctor* pFunctor);	// Override메시지를 삭제한다.

			///< 네트웍 메시지
			BOOL AddNetMsgH		( std::string stCommand, IFunctor* pFunctor);
			BOOL AppendNetMsgH	( std::string stCommand, IFunctor* pFunctor);
			BOOL OverrideNetMsgH( std::string stCommand, IFunctor* pFunctor);	
			BOOL RemoveNetMsgH  ( std::string stCommand);
			LRESULT	 NetCall	( std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);

			BOOL AddNetMsgH		( unsigned int nCommand, IFunctor* pFunctor);
			BOOL AppendNetMsgH	( unsigned int nCommand, IFunctor* pFunctor);
			BOOL OverrideNetMsgH( unsigned int nCommand, IFunctor* pFunctor);	
			BOOL RemoveNetMsgH  ( unsigned int nCommand);
			LRESULT	 NetCall	( unsigned int nCommand, WPARAM wParam = 0, LPARAM lParam = 0);

			// Post Message [7/25/2011 guesty]
			void ProcPostMessage(void);
			void PostCall(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);

			// Component 관리
			BOOL AttachComponent(IComponent* pComponet);
			BOOL DetachComponent(IComponent* pComponet);
			void DetachAll();

		public:
			SFINFOS	Info(std::string stCommand);
			void	Clear();
			void    DumpAllHandlers(BOOL bSort);

			void    ClearAllCommand();					///< Command를 모두 삭제한다.

		public:
			static	CMessageManager& Get();

		private:
			CDelegate*	FindMsgDelegate( std::string stCommand);
			CDelegate*	FindNetDelegate( std::string stCommand);

			CDelegate*	FindNetDelegate( unsigned int nCommand);

		private:
			typedef	std::map< std::string, CDelegate>	MAP;
			typedef	MAP::iterator						IT;

			MAP	m_mapMessage;
			MAP	m_mapNetWork;

			typedef	std::map< unsigned int, CDelegate>	MAPNUMBER;
			typedef	MAPNUMBER::iterator					MAPNUMBERIT;

			MAPNUMBER	m_mapNetWorkNumber;

			typedef	std::list<IComponent*>  COMPONENTS;
			typedef COMPONENTS::iterator	COMPONENT_IT;;
			COMPONENTS						m_cComponentList;

			// Post Message [7/25/2011 guesty]
			struct POSTMSG
			{
				std::string stCommand;
				WPARAM wParam;
				LPARAM lParam;
			};

			typedef std::deque<POSTMSG>	QUEUE;
			QUEUE m_queMessage;
				
			// singletone
		public:
			//static CMessageManager& GetInstance() 
			//{
			//	if(destroyed) 
			//	{
			//		new(pInst) CMessageManager;
			//		atexit(killPhoenix);	// 종료될때 호출되는 예약 함수
			//		destroyed = false;
			//	} 
			//	else if(pInst == 0) 
			//	{
			//		create();
			//	}

			//	return *pInst;
			//}
		private:
			CMessageManager();
			CMessageManager(const CMessageManager & other);
			~CMessageManager();

			/*static void create() 
			{
			static CMessageManager inst;
			pInst = &inst;
			}

			static void killPhoenix()
			{
			pInst->~CMessageManager();
			}*/

		public:

			//static bool destroyed;
			//static CMessageManager* pInst;
		};


		NMUTIL_CLASS CMessageManager& MM();

	}//namespace UTIL
}//namespace NMBASE