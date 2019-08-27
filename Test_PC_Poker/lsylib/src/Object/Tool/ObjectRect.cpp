#include "stdafx.h"
#include "ObjectRect.h"
#include "../Base/DumyImage.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CObjectRect::CObjectRect(void)
:	m_pLeft(NULL),
	m_pRight(NULL),
	m_pTop(NULL),
	m_pBottom(NULL),
	m_pLeftTop(NULL),
	m_pRightTop(NULL),
	m_pLeftBottom(NULL),
	m_pRightBottom(NULL),
	m_pObserver(NULL),
	m_bObserverChange(false)
{
	m_pLeft			= new CDumyImage;
	m_pRight		= new CDumyImage;
	m_pTop			= new CDumyImage;
	m_pBottom		= new CDumyImage;
	m_pLeftTop		= new CDumyImage;
	m_pRightTop		= new CDumyImage;
	m_pLeftBottom	= new CDumyImage;
	m_pRightBottom	= new CDumyImage;

	AddLocalChild(m_pLeft);
	AddLocalChild(m_pRight);
	AddLocalChild(m_pTop);
	AddLocalChild(m_pBottom);
	AddLocalChild(m_pLeftTop);
	AddLocalChild(m_pRightTop);
	AddLocalChild(m_pLeftBottom);
	AddLocalChild(m_pRightBottom);

	m_pLeftTop->Create(3, 3);
	m_pLeftTop->ClearColor(RGB(255,255,255));
	m_pLeftTop->DrawHLine(CPoint(1,1), 2,RGB(0,0,0));
	m_pLeftTop->DrawVLine(CPoint(1,1), 2,RGB(0,0,0));

	m_pRightTop->Create(3, 3);
	m_pRightTop->ClearColor(RGB(255,255,255));
	m_pRightTop->DrawHLine(CPoint(0,1), 2,RGB(0,0,0));
	m_pRightTop->DrawVLine(CPoint(1,1), 2,RGB(0,0,0));

	m_pLeftBottom->Create(3, 3);
	m_pLeftBottom->ClearColor(RGB(255,255,255));
	m_pLeftBottom->DrawVLine(CPoint(1,0), 2,RGB(0,0,0));
	m_pLeftBottom->DrawHLine(CPoint(1,1), 2,RGB(0,0,0));

	m_pRightBottom->Create(3, 3);
	m_pRightBottom->ClearColor(RGB(255,255,255));
	m_pRightBottom->DrawVLine(CPoint(1,0), 2,RGB(0,0,0));
	m_pRightBottom->DrawHLine(CPoint(0,1), 2,RGB(0,0,0));
}

CObjectRect::~CObjectRect()
{
	if(m_pObserver)
	{
		m_pObserver->SetObserverRect(NULL);
	}

	m_pObserver = NULL;
}

bool CObjectRect::Create(CObjectBase *pObject)
{
	ASSERT(pObject != NULL);

	m_pObserver = pObject;
	m_pObserver->SetObserverRect(this);
	SetLocalRect(m_pObserver->GetLocalRect());
	SetOserverChange(true);
	return true;
}

void CObjectRect::SetOserverChange(const bool &bChange)
{
	m_bObserverChange = bChange;
}

void CObjectRect::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bObserverChange) CheckObserver();

	__super::Draw(pPage);
}

void CObjectRect::CheckObserver(void)
{
	CRect rtRect = m_pObserver->GetRect();
	CPoint ptPos = m_pObserver->GetPos();

	const float fWidth = rtRect.GetWidth();
	const float fHeight = rtRect.GetHeight();

	if(m_pLeft->GetHeight() != fHeight)
	{
		m_pLeft->Create(3, (int)fHeight);
		m_pLeft->ClearColor(RGB(255,255,255));
		m_pLeft->DrawVLine(CPoint(1,0), fHeight, RGB(0,0,0));

		m_pRight->Create(3, (int)fHeight);
		m_pRight->ClearColor(RGB(255,255,255));
		m_pRight->DrawVLine(CPoint(1,0), fHeight, RGB(0,0,0));
	}

	if(m_pTop->GetWidth() != fWidth)
	{
		m_pTop->Create((int)fWidth, 3);
		m_pTop->ClearColor(RGB(255,255,255));
		m_pTop->DrawHLine(CPoint(0,1), fWidth, RGB(0,0,0));

		m_pBottom->Create((int)fWidth, 3);
		m_pBottom->ClearColor(RGB(255,255,255));
		m_pBottom->DrawHLine(CPoint(0,1), fWidth, RGB(0,0,0));
	}

	m_pLeftTop->SetLocalPos(-3, -3);
	m_pTop->SetLocalPos(0,-3);
	m_pLeft->SetLocalPos(-3,0);
	m_pRightTop->SetLocalPos(fWidth,-3);
	m_pRight->SetLocalPos(fWidth,0);
	m_pLeftBottom->SetLocalPos(-3,fHeight);
	m_pRightBottom->SetLocalPos(fWidth,fHeight);
	m_pBottom->SetLocalPos(0,fHeight);

	CPoint ptPivot(ptPos.x - rtRect.left, ptPos.y - rtRect.top);

	SetPos(ptPos);
	SetRotation(m_pObserver->GetRotation());
	SetPivot(ptPivot);
	SetLocalRect(CRect(0,0,rtRect.GetWidth(),rtRect.GetHeight()));

	SetOserverChange(false);
}

CObjectBase *CObjectRect::NewObject(void)
{
	return new CObjectRect;
}

CObjectBase *CObjectRect::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CObjectRect *pImage = (CObjectRect *)pObject;
	return pObject;
}
} //namespace LSY