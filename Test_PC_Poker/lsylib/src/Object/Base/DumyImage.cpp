#include "stdafx.h"
#include "DumyImage.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CDumyImage::CDumyImage(void)
:	m_pPage(NULL),
	m_pDC(NULL),
	m_rtPageRect(CRectZero)
{
}

CDumyImage::~CDumyImage()
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

bool CDumyImage::Create(int nWidth, int nHeight)
{
	if(m_rtPageRect.GetWidth() == (float)nWidth && m_rtPageRect.GetHeight() == (float)nHeight) return false;

	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}
	m_pDC = NULL;

	if(nWidth == 0 || nHeight == 0)
	{
		m_rtPageRect = CRectZero;
	}
	else
	{
		m_pPage = new NMBASE::GRAPHICGDI::CPage;
		m_pPage->Init(nWidth + nWidth%2, nHeight, 16);
		m_rtPageRect = CRect(0.0f,0.0f, (float)nWidth, (float)nHeight);

		CDC TempDC;
		HDC dc = ::GetDC(NULL);
		TempDC.Attach(dc);

		m_pDC = new CDC;
		m_pDC->CreateCompatibleDC(&TempDC);
		m_pDC->SelectObject(m_pPage->hBmp);
		m_pDC->SetBkMode(TRANSPARENT);
		::ReleaseDC(NULL, dc);
	}

	SetLocalRect(m_rtPageRect);
	return true;
}

void CDumyImage::DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bTempEventDraw)
	{
		if(!m_bClipDraw || !m_bDraw) return;
		CObjectMan *pObjectMan = m_pProject?m_pProject->GetObjectMan():NULL;
		if(pObjectMan) pObjectMan->AddEventObject(this);
		return;
	}
	__super::DrawLocal(pPage);
}

void CDumyImage::DrawMy(NMBASE::GRAPHICGDI::CPage *pPage)
{
	__super::DrawMy(pPage);
	if(m_pPage == NULL) return;
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

CObjectBase *CDumyImage::NewObject(void)
{
	return new CDumyImage;
}

CObjectBase *CDumyImage::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CDumyImage *pImage = (CDumyImage *)pObject;

	if(m_pPage)
	{
		pImage->m_pPage = new NMBASE::GRAPHICGDI::CPage;
		int nWidth			= (int)m_rtPageRect.right;
		int nHeight			= (int)m_rtPageRect.bottom;
		pImage->m_pPage->Init(nWidth + nWidth%2, nHeight, 16);
	}

	if(m_pDC)
	{
		pImage->m_pDC = new CDC;
		CDC tmpDC;
		HDC dc = ::GetDC(NULL);
		tmpDC.Attach(dc);

		pImage->m_pDC->CreateCompatibleDC(&tmpDC);
		pImage->m_pDC->SelectObject(pImage->m_pPage->hBmp);
		pImage->m_pDC->SetBkMode(TRANSPARENT);

		tmpDC.DeleteDC();
		::ReleaseDC(NULL, dc);
	}

	pImage->m_rtPageRect = m_rtPageRect;
	pImage->m_bTempEventDraw = m_bTempEventDraw;

	return pObject;
}

void CDumyImage::ClearColor(const COLORREF &clrColor)
{
	if(m_pDC == NULL) return;
	::CRect rtRect = m_rtPageRect.GetRect();
	m_pDC->FillSolidRect(&rtRect, clrColor);
	if(rtRect.Width()%2)
	{
		rtRect.left = rtRect.right;
		rtRect.right = rtRect.left+1;
		m_pDC->FillSolidRect(&rtRect, RGB(0,0,255));
	}
}

void CDumyImage::DrawHLine(const CPoint &ptPos, const float &fLen, const COLORREF &clrColor)
{
	WORD wColor = NMBASE::GRAPHICGDI::GetRGBmix(GetRValue(clrColor), GetGValue(clrColor), GetBValue(clrColor));
	if(m_pPage) m_pPage->HLine((int)ptPos.x, (int)ptPos.y, (int)fLen, wColor, FALSE);
}

void CDumyImage::DrawVLine(const CPoint &ptPos, const float &fLen, const COLORREF &clrColor)
{
	WORD wColor = NMBASE::GRAPHICGDI::GetRGBmix(GetRValue(clrColor), GetGValue(clrColor), GetBValue(clrColor));
	if(m_pPage) m_pPage->VLine((int)ptPos.x, (int)ptPos.y, (int)fLen, wColor, FALSE);
}
} //namespace LSY