#pragma once

#include "FrameDefine.h"
#include "Delegate/Functor.h"
#include "NMViewBase.h"

namespace NMBASE
{
	namespace FRAME
	{

		class NMFRAME_CLASS CNMMainFrmView : public CFrameWnd, public CNMViewBase
		{
		public:
			CNMMainFrmView(void);
			CNMMainFrmView(int nWidth, int nHeight);
			virtual ~CNMMainFrmView(void);

			DECLARE_DYNAMIC(CNMMainFrmView)

			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			virtual void OnTimer( UINT nIDEvent );


			virtual void SetMoveCaptionRect();			///< (필수)이동 가능한 캡션 영역 지정


			virtual void		OnDraw(){}					///< 
			virtual void		OnUpdate(){} 				///< 
			virtual void		OnActiveOn(){}				///< 
			virtual void		OnActiveOff(){}				///< 

			virtual void		OnDialogToPaint(){}			///< Modal Dialog쪽에서 부모의 화면 갱신을 위해 호출해 주는 함수

		public:
			UTIL::CFunctor			m_cOnClickExitBtn;		///< 종료버튼	
			UTIL::CFunctor			m_cOnClickMiniBtn;		///< MiniMize버튼	
			UTIL::CFunctor			m_cOnClickRestoreBtn;	///< Restore버튼
			UTIL::CFunctor			m_cOnClickOptionBtn;	///< 옵션버튼
			UTIL::CFunctor			m_cOnClickAltF4KeyDown;	///< Alt + F4 KeyDown


		private:
			int			m_nWinWidth;
			int			m_nWinHeight;

			DWORD		m_dwDragFlags;		///< Window Drag Flags
			POINT		m_ptClickPos;		///< 커서의 위치

			CRect		m_rcMoveCaption;	///< 이동할 캡션바

		public:
			afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnDestroy();
			afx_msg void OnClose();		
			DECLARE_MESSAGE_MAP()
		};

	}//namespace FRAME

}//namespace NMBASE
