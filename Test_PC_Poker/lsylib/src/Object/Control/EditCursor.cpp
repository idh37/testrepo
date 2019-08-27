#include "stdafx.h"
#include "EditCursor.h"
#include "../ObjectMan.h"
#include "Edit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CEditCursor::CEditCursor(void)
:	m_pPage(NULL),
	m_bShowCursor(false),
	m_clrCursorColor(RGB(255,255,255)),
	m_dwBlinkTime(500),
	m_bRedraw(true),
	m_pDC(NULL),
	m_szCursor(0,0),
	m_clrTextColor(RGB(0,0,0)),
	m_bCursorSize(false),
	m_dwStartTime(0),
	m_dwCurTime(0)
{
}

CEditCursor::~CEditCursor()
{
	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}

	m_pDC = NULL;
}

void CEditCursor::SetCursorColor(const COLORREF &clrColor)
{
	if(m_clrCursorColor == clrColor) return;
	m_clrCursorColor = clrColor;
	SetRedraw();
}

bool CEditCursor::Create(int nWidth, int nHeight)
{
	if((int)GetWidth() == nWidth && (int)GetHeight() == nHeight) return true;

	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}

	m_pDC = NULL;

	if(nWidth != 0)
	{
		m_pPage = new NMBASE::GRAPHICGDI::CPage;
		m_pPage->Init(nWidth+nWidth%2, nHeight, 16);

		m_pDC = new CDC;
		CDC tmpDC;
		HDC dc = ::GetDC(NULL);
		tmpDC.Attach(dc);

		m_pDC->CreateCompatibleDC(&tmpDC);
		m_pDC->SelectObject(m_pPage->hBmp);
		m_pDC->SetBkMode(TRANSPARENT);

		ReleaseDC(NULL, dc);
	}

	SetLocalRect(CRect(0,0,(float)nWidth,(float)nHeight));
	SetRedraw();
	return true;
}

const CSize &CEditCursor::GetCursorSize(void)
{
	if(!m_bCursorSize)
	{
		m_bCursorSize = true;
		CEdit *pEdit = (CEdit *)GetParent();

		HDC dc = ::GetDC(NULL);
		HGDIOBJ hOldFont = ::SelectObject( dc, pEdit->m_cFontInfo.GetFont()->m_hObject );
		int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, pEdit->m_cFontInfo.m_nSpaceBetweenLetters);

		::GetTextExtentPoint32( dc, "A", 1, &m_szCursor );

		::SelectObject( dc, hOldFont );
		::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

		ReleaseDC(NULL, dc);

		switch(pEdit->m_cFontInfo.m_nDrawType)
		{
		case ETDT_SHADOW:
			++m_szCursor.cy;
			break;
		case ETDT_OUTLINE:
			m_szCursor.cy += 2;
			break;
		}

		m_szCursor.cx = 0;
	}
	return m_szCursor;
}

void CEditCursor::CheckRedraw(void)
{
	m_bRedraw = false;

	CEdit *pEdit = (CEdit *)GetParent();

	HDC dc = ::GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, pEdit->m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, pEdit->m_cFontInfo.m_nSpaceBetweenLetters);

	if(m_strText == "")
	{
		::GetTextExtentPoint32( dc, "A", 1, &m_szCursor );
		m_szCursor.cx = 0;

		switch(pEdit->m_cFontInfo.m_nDrawType)
		{
		case ETDT_SHADOW:
			++m_szCursor.cy;
			break;
		case ETDT_OUTLINE:
			m_szCursor.cy += 2;
			break;
		}
	}
	else
	{
		if(pEdit->m_bPassword) ::GetTextExtentPoint32( dc, PASSWORD_CHAR, (int)strlen(PASSWORD_CHAR), &m_szCursor );
		else ::GetTextExtentPoint32( dc, m_strText.c_str(), (int)m_strText.length(), &m_szCursor );

		switch(pEdit->m_cFontInfo.m_nDrawType)
		{
		case ETDT_SHADOW:
			++m_szCursor.cx;
			++m_szCursor.cy;
			break;
		case ETDT_OUTLINE:
			m_szCursor.cx += 2;
			m_szCursor.cy += 2;
			break;
		}
	}

	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	ReleaseDC(NULL, dc);

	m_bCursorSize = true;

	if(m_strText == "")
	{
		Create(1, m_szCursor.cy+1);
		m_bRedraw = false;
		SetMove(-1, -1);

		m_pDC->FillSolidRect(&::CRect(0,0,2, m_szCursor.cy+1), RGB(0,0,255));
		m_pDC->FillSolidRect(&::CRect(0,0,1, m_szCursor.cy+1), m_clrCursorColor);
	}
	else
	{
		Create(m_szCursor.cx, m_szCursor.cy+1);
		m_bRedraw = false;
		switch(pEdit->m_cFontInfo.m_nDrawType)
		{
		case ETDT_SHADOW:
			SetMove(1, -1);
			break;
		case ETDT_OUTLINE:
			SetMove(2, -1);
			break;
		default:
			SetMove(0, -1);
			break;
		}
		
		CBrush brush;
		if(m_szCursor.cx%2)
		{
			m_pDC->FillSolidRect(&::CRect(m_szCursor.cx,0,m_szCursor.cx+1,m_szCursor.cy+1), RGB(0,0,255));
		}

		m_pDC->FillSolidRect(&::CRect(0,0,m_szCursor.cx, m_szCursor.cy+1), m_clrCursorColor);
		m_pDC->SelectObject((CFont *)pEdit->m_cFontInfo.GetFont());

		std::string strOutput;
		if(pEdit->m_bPassword) strOutput = PASSWORD_CHAR;
		else strOutput = m_strText;

		switch(pEdit->m_cFontInfo.m_nDrawType)
		{
			case ETDT_OUTLINE:
				m_pDC->SetTextColor(pEdit->m_cFontInfo.m_clrOutline);
				m_pDC->TextOut(1, 1, strOutput.c_str());
				m_pDC->TextOut(0, 2, strOutput.c_str());
				m_pDC->TextOut(2, 2, strOutput.c_str());
				m_pDC->TextOut(1, 3, strOutput.c_str());
				m_pDC->SetTextColor(m_clrTextColor);
				m_pDC->TextOut(1, 2, strOutput.c_str());
				break;
			case ETDT_SHADOW:
				m_pDC->SetTextColor(pEdit->m_cFontInfo.m_clrOutline);
				m_pDC->TextOut(1, 2, strOutput.c_str());
				m_pDC->SetTextColor(m_clrTextColor);
				m_pDC->TextOut(0, 1, strOutput.c_str());
				break;
			default:
				m_pDC->SetTextColor(m_clrTextColor);
				m_pDC->TextOut(0, 1, strOutput.c_str());
				break;
		}
	}

	SetLocalRect(CRect(0,0, (float)(m_szCursor.cx?m_szCursor.cx+1:1), (float)(m_szCursor.cy+1)));
}

void CEditCursor::CheckEventDraw(void)
{
	CheckEventObjectDraw();
	m_bTempEventDraw = true;
}

void CEditCursor::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bRedraw) CheckRedraw();
	__super::Draw(pPage);
}

void CEditCursor::DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bEventDraw)
	{
		if(!m_bClipDraw || !m_bDraw) return;
		CObjectMan *pObjectMan = m_pProject?m_pProject->GetObjectMan():NULL;
		if(pObjectMan) pObjectMan->AddEventObject(this);
		return;
	}

	__super::DrawLocal(pPage);
}

void CEditCursor::DrawMy(NMBASE::GRAPHICGDI::CPage *pPage)
{
	__super::DrawMy(pPage);
	if(m_pPage == NULL || m_bShowCursor == false) return;
	m_bTempEventDraw = false;
	int nOption = (int)m_dwDrawOption;
	switch(m_nDrawType)
	{
	case EODT_ROTATION:
	case EODT_SCALEROTATION:
		pPage->RotateZoomSprAuto((int)m_ptDraw.x, (int)m_ptDraw.y, m_pPage, (int)m_rtRect.GetWidth(), (int)m_rtRect.GetHeight(), m_fDrawRotaion, nOption, m_nAlpha, m_nColor);
		break;
	case EODT_SCALE:
		pPage->ZoomSprAuto((int)m_ptDraw.x, (int)m_ptDraw.y, m_pPage, (int)m_rtRect.GetWidth(), (int)m_rtRect.GetHeight(), nOption, m_nAlpha, m_nColor);
		break;
	case EODT_NORMAL:
		pPage->PutSprAuto((int)m_ptDraw.x, (int)m_ptDraw.y, m_pPage, nOption, m_nAlpha, m_nColor);
		break;
	}
}

void CEditCursor::SetShow(const bool &bShow)
{
	SetUseLogic(bShow);
	CObjectBase::SetShow(bShow);
}

CObjectBase *CEditCursor::NewObject(void)
{
	return new CEditCursor;
}

CObjectBase *CEditCursor::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CEditCursor *pEditCursor = (CEditCursor *)pObject;
	return pObject;
}

void CEditCursor::SetText(const std::string &strText)
{
	if(m_strText == strText) return;
	m_strText = strText;

	CEdit *pEdit = (CEdit *)GetParent();
	HDC dc = ::GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, pEdit->m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, pEdit->m_cFontInfo.m_nSpaceBetweenLetters);

	if(pEdit && pEdit->m_cFontInfo.GetFont())
	{
		if(m_strText == "")
		{
			::GetTextExtentPoint32( dc, "A", 1, &m_szCursor );

			switch(pEdit->m_cFontInfo.m_nDrawType)
			{
			case ETDT_SHADOW:
				++m_szCursor.cy;
				break;
			case ETDT_OUTLINE:
				m_szCursor.cy += 2;
				break;
			}

			m_szCursor.cx = 0;
		}
		else
		{
			if(pEdit->m_bPassword) ::GetTextExtentPoint32( dc, PASSWORD_CHAR, (int)strlen(PASSWORD_CHAR), &m_szCursor );
			else ::GetTextExtentPoint32( dc, m_strText.c_str(), (int)m_strText.length(), &m_szCursor );

			switch(pEdit->m_cFontInfo.m_nDrawType)
			{
			case ETDT_SHADOW:
				++m_szCursor.cx;
				++m_szCursor.cy;
				break;
			case ETDT_OUTLINE:
				m_szCursor.cx += 2;
				m_szCursor.cy += 2;
				break;
			}
		}
	}

	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	ReleaseDC(NULL, dc);

	SetRedraw();
}

void CEditCursor::SetLogicStartTime(const DWORD &dwStartTime)
{
	__super::SetLogicStartTime(dwStartTime);
	m_dwStartTime = 0;
	m_dwCurTime = 0;
}

void CEditCursor::Logic(const DWORD &dwTime)
{
	bool bOldShowCursor = m_bShowCursor;
	m_dwCurTime = dwTime;
	m_bShowCursor = ((dwTime - m_dwStartTime)/m_dwBlinkTime)%2?false:true;
	if(bOldShowCursor != m_bShowCursor)
	{
		CheckEventDraw();
	}
}

void CEditCursor::SetTextColor(const COLORREF &clrColor)
{
	if(m_clrTextColor == clrColor) return;
	m_clrTextColor = clrColor;
	SetRedraw();
}

void CEditCursor::SetRedraw(void)
{
	if(m_bRedraw) return;
	m_bRedraw = true;
	CheckEventDraw();
}

void CEditCursor::ResetTimer(void)
{
	m_dwStartTime = m_dwCurTime;
	if(!m_bShowCursor) CheckEventDraw();
	m_bShowCursor = true;
}
} //namespace LSY