#pragma once

#include "NMWidgetManager.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		enum CHAT_DATA_ATTRI
		{
			ATTRI_CHAT,
			ATTRI_SYSTEM,
			ATTRI_SERVER,
		};

		/////////////////////////////////////////////////////////////////////////////
		// CNMCommonRichEdit window
		class NMSKINGDI_CLASS CNMCommonRichEdit : public CRichEditCtrl
		{
		public:
			CNMCommonRichEdit();
			virtual ~CNMCommonRichEdit();

			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CNMCommonRichEdit)
		protected:
			//}}AFX_VIRTUAL

			// Implementation
		public:
			BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
			BOOL Initialize( INMWidget* pParent );
			//	void AddText( CString* pstrText, COLORREF crText, int nAttri = ATTRI_CHAT );
			void AddText( const char* szText, COLORREF crText, int nAttri = ATTRI_CHAT );
			void ClearAllData();

			// Generated message map functions
		protected:
			//{{AFX_MSG(CNMCommonRichEdit)
			afx_msg void OnSetFocus( CWnd* pOldWnd );
			//}}AFX_MSG

			DECLARE_MESSAGE_MAP()

		private:
			INMWidget*	m_pParent;
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMChatWidget
		class NMSKINGDI_CLASS CNMChatWidget : public CNMWidgetComposite
		{
		public:
			CNMChatWidget();
			virtual ~CNMChatWidget();

			virtual BOOL Initialize( int nControlID, CWnd* pParent, CRect rectEdit,
				int nLinePerPage, int nLinePerLimit );
			virtual void SetPosition( int nPosX, int nPosY );
			virtual void SetFont( CFont* pFont );
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );

			virtual void InsertData( CString* pstrText, COLORREF crText = RGB(82,82,82), int nAttri = ATTRI_CHAT );
			virtual void InsertData( const char* szText, COLORREF crText = RGB(82,82,82), int nAttri = ATTRI_CHAT );
			virtual void ClearAllData();

			virtual void SetCurrentLine( int nLine );




		protected:
			int					m_nControlID;
			CWnd*				m_pParent;
			CRect				m_rectPosition;

			int					m_nLinePerPage;
			int					m_nLinePerLimit;
			int					m_nCurrentLine;

			CFont*				m_pFont;
			CNMCommonRichEdit	m_CommonRichEdit;
		};


	} //namespace SKINGDI

}//namespace NMBASE