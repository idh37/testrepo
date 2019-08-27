#pragma once

#include "NMButton.h"
#include "skingdidefine.h"


namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMComboBox  : public CNMWidgetCompact
		{
		protected:

			struct COMBODATA
			{
				string	str;
				DWORD	ItemData;
				COLORREF	textcolor;
			};

			int				m_nControlID;
			CWnd*			m_pParent;
			CRect			m_rectPosition;
			CRect			m_rectDropDown;
			BOOL			m_bEnable;
			BOOL			m_bPullDown;


			vector<COMBODATA> m_vec_data;

			CNMButton		m_BtnPullDown;

			int				m_nSel;

			WORD			m_colorBack;
			WORD			m_colorOver;
			COLORREF		m_colorText;
			WORD			m_colorBorder;

			CFont			*m_pFont;

			void			*m_pItemData;

			BOOL			m_bShow;

		public:


			BOOL Initialize( int nControlID,
				CWnd* pParent,
				CRect *pRect,
				char* szSprKey,
				int nSprID,
				int nSprCount );


			int GetCurSel()
			{
				return m_nSel;
			}

			BOOL SetCurSel(int nSel);
			void SetBackColor(WORD color)
			{
				m_colorBack = color;
			}
			void SetBorderColor(WORD color)
			{
				m_colorBorder = color;
			}
			void SetMouseOverColor(WORD color)
			{
				m_colorOver = color;
			}
			void SetFont(CFont *pFont)
			{
				m_pFont = pFont;
			}
			void SetTextColor(COLORREF color)
			{
				m_colorText = color;
			}
			void SetTextColor(int nIndex, COLORREF color);

			int GetCount()
			{
				return (int)m_vec_data.size();
			}

			virtual void Show(BOOL bShow);


			void AddString(const char *pStr, DWORD ItemData=0);

			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );


			virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
			virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );


			BOOL SetItemData(int nIndex, DWORD ItemData);
			DWORD GetItemData(int nIndex);
			void ResetContent();



		public:
			CNMComboBox();
			virtual ~CNMComboBox();

		protected:


			int PtInSelRect(POINT pt);

		};

	} //namespace SKINGDI

}//namespace NMBASE

