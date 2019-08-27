#pragma once

#include "FrameDefine.h"
#include "NMViewBase.h"

namespace NMBASE
{
	namespace FRAME
	{

		class NMFRAME_CLASS CNMLobbyView : public CNMViewBase
		{
		public:
			CNMLobbyView(void);
			~CNMLobbyView(void);

		public:

			// 	virtual void		OnCreate();
			// 	virtual void		OnDestroy();	
			virtual void		OnDraw(){}
			virtual void		OnUpdate(){} 	
			virtual void		OnActiveOn(){}				///< 
			virtual void		OnActiveOff(){}				///< 

		public:
			UTIL::CFunctor			m_cOnClickRefreshBtn;		///< 갱신버튼
			UTIL::CFunctor			m_cOnClickJoinBtn;			///< 입장
			UTIL::CFunctor			m_cOnClickExitGameBtn;		///< 게임 종료(완전 종료)


		};

	}//namespace FRAME

}//namespace NMBASE
