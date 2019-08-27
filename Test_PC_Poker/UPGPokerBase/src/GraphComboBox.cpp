#include "StdAfx.h"
#include "GraphComboBox.h"


#define SPACE_AHEAD_OF_STRING (5)

CGraphComboBox::CGraphComboBox()
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
	m_pPage=NULL;

	m_pItemData=NULL;

}

CGraphComboBox::~CGraphComboBox()
{

	m_vec_data.clear();
}



BOOL CGraphComboBox::Init( CWnd* pParent,
							 NMBASE::GRAPHICGDI::CPage* pPage,
							 CRect *pRect,
							 int nCmdID,
							 NMBASE::GRAPHICGDI::xSprite *pSprite,
							 int nSprID,
							 int nSprCount )
{

	m_pParent =pParent;
	m_pPage = pPage;

	if (!pSprite->IsValidSpr(nSprID))
	{
		return FALSE;
	}

	//버튼의 width, height는?
	int BtnWidth = pSprite->spr[nSprID].xl;
	int BtnHeight = pSprite->spr[nSprID].yl;

	m_rectPosition = *pRect;
	m_rectDropDown = m_rectPosition;

	CPoint PosButton;
	PosButton.x = pRect->right - BtnWidth +1;
	PosButton.y = pRect->top + (int) ( ( pRect->Height() - BtnHeight ) / 2.0f );

	m_nControlID = nCmdID;
	
	m_BtnPullDown.Init(pParent, pPage, PosButton.x, PosButton.y, pSprite, nSprID, 0, nSprCount );
	
	return TRUE;

}



BOOL CGraphComboBox::OnMouseMove( int xp, int yp )
{
	if (m_bShow==FALSE)
		return FALSE;


	if (m_BtnPullDown.OnMouseMove(xp, yp))
		return TRUE;

	if (m_rectPosition.PtInRect(CPoint(xp, yp)))
		return TRUE;

	if (m_bPullDown)
	{
		if (m_rectDropDown.PtInRect(CPoint(xp, yp)))
			return TRUE;

	}

	return FALSE;
}
// BOOL CGraphComboBox::OnMouseLeave( int xp, int yp )
// {
// 	if (m_bShow==FALSE)
// 		return FALSE;
// 
// 	return m_BtnPullDown.OnMouseLeaveWidget(nFlags, point);
// }
BOOL CGraphComboBox::OnLButtonDown( int xp, int yp )
{
	if (m_bShow==FALSE)
		return FALSE;

	if (m_bPullDown)
	{
		int sel  = PtInSelRect(CPoint(xp, yp));
		if (sel>=0)
		{
			int nOldSel = GetCurSel();
			if (SetCurSel(sel))
			{
				if (m_pParent)
					m_pParent->SendMessage(WM_COMMAND, (WPARAM)m_nControlID, (LPARAM)sel);				
			}

			m_bPullDown=FALSE;
			return TRUE;

		}

	}
	else 
	{
		if (m_rectPosition.PtInRect(CPoint(xp, yp)))
		{
			m_BtnPullDown.OnLButtonDown(m_BtnPullDown.m_Xp, m_BtnPullDown.m_Yp);
			m_bPullDown = !m_bPullDown;
			return FALSE;
		}
	}


	if (m_BtnPullDown.OnLButtonDown(xp, yp))
	{
		m_bPullDown = !m_bPullDown;
		return FALSE;
	}

	m_bPullDown=FALSE;




	return FALSE;
}

BOOL CGraphComboBox::OnLButtonUp( int xp, int yp )
{
	if (m_bShow==FALSE)
		return FALSE;

	if (m_BtnPullDown.OnLButtonUp(xp, yp))
		return TRUE;	


	return FALSE;
}

int CGraphComboBox::PtInSelRect(POINT pt)
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


void CGraphComboBox::Draw( CDC *pDC )
{
	if (m_bShow==FALSE || m_pPage==NULL)
		return;

	//Page.DrawEmptyBox(m_rectPosition.left, m_rectPosition.top, m_rectPosition.right, m_rectPosition.bottom, 0);
	m_BtnPullDown.Draw(pDC);

	CFont *pOldFont =NULL;
	if (m_pFont)
		pOldFont = pDC->SelectObject(m_pFont);




	if ( GetCurSel() >= 0 && GetCurSel() < (int) m_vec_data.size())
	{
		CString str = m_vec_data[GetCurSel()].str.c_str();
		CRect rectDrawBox = m_rectPosition;
		rectDrawBox.left += SPACE_AHEAD_OF_STRING;				//스트링을 그릴 때 앞쪽에 공간을 준다.

		COLORREF oldColor = pDC->SetTextColor(m_colorText);
		pDC->DrawText(str,  rectDrawBox, DT_SINGLELINE| DT_VCENTER | DT_LEFT);
		pDC->SetTextColor(oldColor);
	}

	if (m_bPullDown)
	{	
		int datasize = (int) m_vec_data.size();

		CRect rt = m_rectPosition;
		rt.OffsetRect(0, rt.Height());


		m_pPage->DrawFillBox(m_rectDropDown.left, m_rectDropDown.top, m_rectDropDown.right, m_rectDropDown.bottom, m_colorBack);
		m_pPage->DrawEmptyBox(m_rectDropDown.left, m_rectDropDown.top, m_rectDropDown.right, m_rectDropDown.bottom, m_colorBorder);



		for(int i =0;i< datasize; i++)
		{			
			POINT pt;
			GetCursorPos(&pt);
			m_pParent->ScreenToClient(&pt);

			if (rt.PtInRect(pt))
				m_pPage->DrawFillBox(rt.left+1, rt.top+1, rt.right-1, rt.bottom-1, m_colorOver);



			CRect rectDrawBox = rt;
			rectDrawBox.left += SPACE_AHEAD_OF_STRING;				//스트링을 그릴 때 앞쪽에 공간을 준다.

			COLORREF oldColor = pDC->SetTextColor(m_vec_data[i].textcolor);
			CString str = m_vec_data[i].str.c_str();
			pDC->DrawText(str,  rectDrawBox, DT_SINGLELINE| DT_VCENTER | DT_LEFT);
			pDC->SetTextColor(oldColor);


			rt.OffsetRect(0, rt.Height());			
		}

	}

	if (pOldFont)
		pDC->SelectObject(pOldFont);

}

BOOL CGraphComboBox::SetCurSel(int nSel)
{
	if (m_nSel == nSel || nSel<0 )
		return FALSE;
	m_nSel = nSel;
	return TRUE;
}
void CGraphComboBox::AddString(const char *pStr, DWORD ItemData)
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

BOOL CGraphComboBox::SetItemData(int nIndex, DWORD ItemData)
{
	if (nIndex >= (int)m_vec_data.size() || nIndex<0)
		return FALSE;

	m_vec_data[nIndex].ItemData = ItemData;

	return TRUE;

}

DWORD CGraphComboBox::GetItemData(int nIndex)
{

	if (nIndex >= (int)m_vec_data.size() || nIndex<0)
		return NULL;


	return m_vec_data[nIndex].ItemData;
}


void CGraphComboBox::SetTextColor(int nIndex, COLORREF color)
{
	if (nIndex >= (int)m_vec_data.size() || nIndex<0)
		return;

	m_vec_data[nIndex].textcolor = color;
}


void CGraphComboBox::Show(BOOL bShow)
{
	m_bShow = bShow;

}

void CGraphComboBox::ResetContent()
{

	m_vec_data.clear();

	//펼쳤을때 박스렉트계산
	m_rectDropDown = m_rectPosition;
	m_rectDropDown.top +=	m_rectPosition.Height();
	m_rectDropDown.bottom += m_rectPosition.Height() *  (int) m_vec_data.size();	

}