// NMComboBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "./NMWidget/NMComboBox.h"
#include "./NMWidget/NMResourceManager.h"

#define SPACE_AHEAD_OF_STRING (5)

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMComboBox::CNMComboBox()
		{

			m_nSel=-1;
			m_bPullDown=FALSE;
			m_bEnable=FALSE;
			m_pParent=NULL;
			m_nControlID=0;
			m_colorBack=NMBASE::GRAPHICGDI::xSprite::RGB555(200,200,200);
			m_colorOver=NMBASE::GRAPHICGDI::xSprite::RGB555(230,230,230);
			m_colorText=0;
			m_colorBorder=0;
			m_pFont=NULL;
			m_bShow=TRUE;

			m_pItemData=NULL;

		}

		CNMComboBox::~CNMComboBox()
		{

			m_vec_data.clear();
		}



		BOOL CNMComboBox::Initialize( int nControlID,
			CWnd* pParent,
			CRect *pRect,
			char* szSprKey,
			int nSprID,
			int nSprCount )
		{

			m_pParent =pParent;


			NMBASE::GRAPHICGDI::xSprite *pBtnSpr = NMRESSM.GetSprite( szSprKey );


			if (!pBtnSpr->IsValidSpr(nSprID))
			{
				return FALSE;
			}

			//버튼의 width, height는?
			int BtnWidth = pBtnSpr->spr[nSprID].xl;
			int BtnHeight = pBtnSpr->spr[nSprID].yl;

			m_rectPosition = *pRect;
			m_rectDropDown = m_rectPosition;

			CPoint PosButton;
			PosButton.x = pRect->right - BtnWidth +1;
			PosButton.y = pRect->top + (int) ( ( pRect->Height() - BtnHeight ) / 2.0f );

			m_nControlID = nControlID;

			if (!m_BtnPullDown.Initialize(0, pParent, PosButton.x, PosButton.y, szSprKey, nSprID, nSprCount))
			{
				return FALSE;
			}

			return TRUE;

		}



		BOOL CNMComboBox::OnMouseMoveWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)
				return FALSE;


			if (m_BtnPullDown.OnMouseMoveWidget(nFlags, point))
				return TRUE;

			if (m_rectPosition.PtInRect(point))
				return TRUE;

			if (m_bPullDown)
			{
				if (m_rectDropDown.PtInRect(point))
					return TRUE;

			}

			return FALSE;
		}
		BOOL CNMComboBox::OnMouseLeaveWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)
				return FALSE;

			return m_BtnPullDown.OnMouseLeaveWidget(nFlags, point);
		}
		BOOL CNMComboBox::OnLButtonDownWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)
				return FALSE;

			if (m_bPullDown)
			{
				int sel  = PtInSelRect(point);
				if (sel>=0)
				{
					int nOldSel = GetCurSel();
					if (SetCurSel(sel))
					{
						if (m_pParent)
							NMSendMessage(m_pParent, m_nControlID, (WPARAM)sel, (LPARAM)nOldSel);				
					}

					m_bPullDown=FALSE;
					return TRUE;

				}

			}
			else 
			{
				if (m_rectPosition.PtInRect(point))
				{
					m_BtnPullDown.OnLButtonDownWidget(nFlags, CPoint(m_BtnPullDown.GetButtonRect().left, m_BtnPullDown.GetButtonRect().top));
					m_bPullDown = !m_bPullDown;
					return FALSE;
				}
			}


			if (m_BtnPullDown.OnLButtonDownWidget(nFlags, point))
			{
				m_bPullDown = !m_bPullDown;
				return FALSE;
			}

			m_bPullDown=FALSE;




			return FALSE;
		}
		BOOL CNMComboBox::OnLButtonUpWidget( UINT nFlags, CPoint point )
		{
			if (m_bShow==FALSE)
				return FALSE;

			return m_BtnPullDown.OnLButtonUpWidget(nFlags, point);
		}

		int CNMComboBox::PtInSelRect(POINT pt)
		{	

			if (m_bPullDown)
			{
				CRect rt = m_rectPosition;	
				rt.OffsetRect(0, rt.Height());
				int datasize = m_vec_data.size();
				for(int i =0;i< datasize; i++)
				{		
					if (rt.PtInRect(pt))
						return i;

					rt.OffsetRect(0, rt.Height());
				}
			}

			return -1;
		}


		void CNMComboBox::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if (m_bShow==FALSE)
				return;

			//Page.DrawEmptyBox(m_rectPosition.left, m_rectPosition.top, m_rectPosition.right, m_rectPosition.bottom, 0);
			m_BtnPullDown.DrawWidget(Page, DC);

			CFont *pOldFont =NULL;
			if (m_pFont)
				pOldFont = DC.SelectObject(m_pFont);




			if ( GetCurSel() >= 0 && GetCurSel() < (int) m_vec_data.size())
			{
				CString str = m_vec_data[GetCurSel()].str.c_str();
				CRect rectDrawBox = m_rectPosition;
				rectDrawBox.left += SPACE_AHEAD_OF_STRING;				//스트링을 그릴 때 앞쪽에 공간을 준다.

				COLORREF oldColor = DC.SetTextColor(m_colorText);
				DC.DrawText(str,  rectDrawBox, DT_SINGLELINE| DT_VCENTER | DT_LEFT);
				DC.SetTextColor(oldColor);
			}

			if (m_bPullDown)
			{	
				int datasize = (int) m_vec_data.size();

				CRect rt = m_rectPosition;
				rt.OffsetRect(0, rt.Height());






				Page.DrawFillBox(m_rectDropDown.left, m_rectDropDown.top, m_rectDropDown.right, m_rectDropDown.bottom, m_colorBack);
				Page.DrawEmptyBox(m_rectDropDown.left, m_rectDropDown.top, m_rectDropDown.right, m_rectDropDown.bottom, m_colorBorder);



				for(int i =0;i< datasize; i++)
				{			
					POINT pt;
					GetCursorPos(&pt);
					m_pParent->ScreenToClient(&pt);

					if (rt.PtInRect(pt))
						Page.DrawFillBox(rt.left+1, rt.top+1, rt.right-1, rt.bottom-1, m_colorOver);



					CRect rectDrawBox = rt;
					rectDrawBox.left += SPACE_AHEAD_OF_STRING;				//스트링을 그릴 때 앞쪽에 공간을 준다.

					COLORREF oldColor = DC.SetTextColor(m_vec_data[i].textcolor);
					CString str = m_vec_data[i].str.c_str();
					DC.DrawText(str,  rectDrawBox, DT_SINGLELINE| DT_VCENTER | DT_LEFT);
					DC.SetTextColor(oldColor);


					rt.OffsetRect(0, rt.Height());			
				}

			}

			if (pOldFont)
				DC.SelectObject(pOldFont);

		}

		BOOL CNMComboBox::SetCurSel(int nSel)
		{
			if (m_nSel == nSel || nSel<0 )
				return FALSE;
			m_nSel = nSel;
			return TRUE;
		}
		void CNMComboBox::AddString(const char *pStr, DWORD ItemData)
		{
			COMBODATA cd;
			cd.str = pStr;
			cd.ItemData = ItemData;
			cd.textcolor = m_colorText;

			string str = pStr;
			m_vec_data.push_back(cd);



			//펼쳤을때 박스렉트계산
			m_rectDropDown = m_rectPosition;
			m_rectDropDown.top +=	m_rectPosition.Height();
			m_rectDropDown.bottom += m_rectPosition.Height() *  (int) m_vec_data.size();


		}

		BOOL CNMComboBox::SetItemData(int nIndex, DWORD ItemData)
		{
			if (nIndex >= (int)m_vec_data.size() || nIndex<0)
				return FALSE;

			m_vec_data[nIndex].ItemData = ItemData;

			return TRUE;

		}

		DWORD CNMComboBox::GetItemData(int nIndex)
		{

			if (nIndex >= (int)m_vec_data.size() || nIndex<0)
				return NULL;


			return m_vec_data[nIndex].ItemData;
		}


		void CNMComboBox::SetTextColor(int nIndex, COLORREF color)
		{
			if (nIndex >= (int)m_vec_data.size() || nIndex<0)
				return;

			m_vec_data[nIndex].textcolor = color;
		}


		void CNMComboBox::Show(BOOL bShow)
		{
			m_bShow = bShow;

		}

		void CNMComboBox::ResetContent()
		{

			m_vec_data.clear();

			//펼쳤을때 박스렉트계산
			m_rectDropDown = m_rectPosition;
			m_rectDropDown.top +=	m_rectPosition.Height();
			m_rectDropDown.bottom += m_rectPosition.Height() *  (int) m_vec_data.size();	

		}

	} //namespace SKINGDI

}//namespace NMBASE