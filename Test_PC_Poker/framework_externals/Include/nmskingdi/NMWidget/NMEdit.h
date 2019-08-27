#pragma once

#include "NMWidgetManager.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		//////////////////////////////////////////////////////////////////////////////
		// CNMCommonEdit
		const int MAX_TEXT_LEN = 256;

		class NMSKINGDI_CLASS CNMCommonEdit : public CEdit
		{
			DECLARE_DYNAMIC(CNMCommonEdit)

		public:
			CNMCommonEdit();
			virtual ~CNMCommonEdit();
			BOOL Initialize( INMWidget* pParent );
			BOOL CheckEasterEgg( char* szBuf );
			BOOL CheckPapering( char* szBuf );
			void GraspData();

			// Generated message map functions
		protected:
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

			//{{AFX_MSG(CNMCommonEdit)
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
			afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			//}}AFX_MSG

		protected:
			DECLARE_MESSAGE_MAP()

		protected:
			enum { HISTORY_COUNT = 10, };
			CString			m_strHistory[HISTORY_COUNT];
			int				m_nNowHistory;

		private:
			INMWidget*		m_pParent;

		private:
			NMBASE::UTIL::CNoChat			m_PaperingChat;
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMEdit
		class NMSKINGDI_CLASS CNMEdit : public CNMWidgetCompact
		{
		public:
			CNMEdit();
			virtual ~CNMEdit();

			BOOL Initialize( int nControlID, CWnd* pParent, CRect rectEdit );
			void SetPosition( int nPosX, int nPosY );
			void SetTextFont( CFont* pFont );
			void SetTextColor( COLORREF crText );
			void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
			BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );

			void GraspData();
			void EnterMessage();

			BOOL GetFocus();
			void SetFocus();

			BOOL EnableWindow(BOOL bEnable);
			void SetWindowText(LPCTSTR lpszString);
			BOOL IsWindowEnabled();

			void SetDefaultText(LPCTSTR lpszString);
			void ClearText();

			void SetLimitText(int nLimitTextCnt);
			void SetStyleNumber(BOOL bNumber);

		protected:
			BOOL			m_bNumber;
			int				m_nLimitTextCnt;
			int				m_nControlID;
			CWnd*			m_pParent;
			CRect			m_rectPosition;

			COLORREF		m_crText;

			CNMCommonEdit	m_CommonEdit;
			CString			m_strCurrentText;

			CString			m_strDefalutText;	//에디트 창을 클릭하기전까지 노출되는 디폴트 스트링
		};

	} //namespace SKINGDI

}//namespace NMBASE