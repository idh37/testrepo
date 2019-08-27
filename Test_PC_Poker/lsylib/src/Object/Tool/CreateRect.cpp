#include "stdafx.h"
#include "CreateRect.h"
#include "../Base/DumyImage.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CCreateRect::CCreateRect(void)
:	m_pLeft(NULL),
	m_pRight(NULL),
	m_pTop(NULL),
	m_pBottom(NULL),
	m_pBack(NULL),
	m_ptStartPos(CPointZero),
	m_ptEndPos(CPointZero)
{
	m_pLeft			= new CDumyImage;
	m_pRight		= new CDumyImage;
	m_pTop			= new CDumyImage;
	m_pBottom		= new CDumyImage;
	m_pBack			= new CDumyImage;

	AddLocalChild(m_pBack);
	AddLocalChild(m_pLeft);
	AddLocalChild(m_pRight);
	AddLocalChild(m_pTop);
	AddLocalChild(m_pBottom);

	m_pBack->SetLocalAlpha(0.5f);

	m_pBack->SetShow(false);
	m_pLeft->SetShow(false);
	m_pRight->SetShow(false);
	m_pTop->SetShow(false);
	m_pBottom->SetShow(false);
}

CCreateRect::~CCreateRect()
{
}

void CCreateRect::Create(const CPoint &ptPos)
{
	m_ptStartPos = ptPos;
	m_ptEndPos = ptPos;
	m_rtCreateRect.SetLeftTop(ptPos);
	m_rtCreateRect.SetRightBottom(ptPos);
}

void CCreateRect::SetEndPos(const CPoint &ptPos)
{
	if(ptPos == m_ptEndPos) return;
	m_ptEndPos = ptPos;
	bool bShow = (m_ptEndPos == m_ptStartPos)?false:true;

	m_pBack->SetShow(bShow);
	m_pLeft->SetShow(bShow);
	m_pRight->SetShow(bShow);
	m_pTop->SetShow(bShow);
	m_pBottom->SetShow(bShow);

	if(bShow == false) return;

	m_rtCreateRect.left = (m_ptStartPos.x < m_ptEndPos.x)?m_ptStartPos.x:m_ptEndPos.x;
	m_rtCreateRect.right = (m_ptStartPos.x < m_ptEndPos.x)?m_ptEndPos.x:m_ptStartPos.x;
	m_rtCreateRect.top = (m_ptStartPos.y < m_ptEndPos.y)?m_ptStartPos.y:m_ptEndPos.y;
	m_rtCreateRect.bottom = (m_ptStartPos.y < m_ptEndPos.y)?m_ptEndPos.y:m_ptStartPos.y;

	if(m_rtCreateRect.GetWidth() != 0.0f && m_rtCreateRect.GetHeight() != 0.0f)
	{
		m_pBack->Create((int)m_rtCreateRect.GetWidth(), (int)m_rtCreateRect.GetHeight());
		m_pBack->ClearColor(RGB(153,217,234));
	}
	else m_pBack->SetShow(false);

	if(m_rtCreateRect.GetHeight() != 0.0f)
	{
		m_pLeft->Create(1, (int)m_rtCreateRect.GetHeight());
		m_pLeft->SetLocalPos(0,0);
		m_pLeft->ClearColor(RGB(153,217,234));

		m_pRight->Create(1, (int)m_rtCreateRect.GetHeight());
		m_pRight->SetLocalPos(m_rtCreateRect.GetWidth()-1.0f, 0);
		m_pRight->ClearColor(RGB(153,217,234));
	}
	else
	{
		m_pLeft->SetShow(false);
		m_pRight->SetShow(false);
	}

	if(m_rtCreateRect.GetWidth() != 1.0f)
	{
		m_pTop->Create((int)m_rtCreateRect.GetWidth(), 1);
		m_pTop->SetLocalPos(0,0);
		m_pTop->ClearColor(RGB(153,217,234));

		m_pBottom->Create((int)m_rtCreateRect.GetWidth(), 1);
		m_pBottom->SetLocalPos(0, m_rtCreateRect.GetHeight()-1.0f);
		m_pBottom->ClearColor(RGB(153,217,234));
	}
	else
	{
		m_pTop->SetShow(false);
		m_pBottom->SetShow(false);
	}

	SetPos(m_rtCreateRect.GetLeftTop());
}

CObjectBase *CCreateRect::NewObject(void)
{
	return new CCreateRect;
}

CObjectBase *CCreateRect::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CCreateRect *pImage = (CCreateRect *)pObject;
	return pObject;
}
} //namespace LSY