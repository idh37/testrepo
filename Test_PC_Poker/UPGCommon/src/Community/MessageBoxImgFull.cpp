#include "stdafx.h"
#include "MessageBoxImgFull.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessageBoxImgFull::CMessageBoxImgFull()
:	m_pTab(NULL)
{
}

void CMessageBoxImgFull::OnMouseOver(void)
{
	m_pTab->OnMouseOver();
	__super::OnMouseOver();
}

void CMessageBoxImgFull::OnMouseLeave(void)
{
	m_pTab->OnMouseLeave();
	__super::OnMouseLeave();
}

bool CMessageBoxImgFull::OnLButtonDown(const LSY::CPoint &ptPos)
{
	m_pTab->OnLButtonDown(ptPos);
	return __super::OnLButtonDown(ptPos);
}

bool CMessageBoxImgFull::OnLButtonUP(const LSY::CPoint &ptPos)
{
	m_pTab->OnLButtonUP(ptPos);
	LSY::CObjectProject *pProject = GetObjectProject();
	LSY::CObjectMan *pOblectMan = pProject?pProject->GetObjectMan():NULL;
	if(pOblectMan)
	{
		LSY::CObjectBase *pSelectObject = pOblectMan->GetSelectObject();
		if(pSelectObject == this)
		{
			LSY::CObjectBase *pFocusObject = pOblectMan->IsInObject((LSY::CPoint &)ptPos, true, NULL);
			if(pFocusObject == m_pTab)
			{
				m_pTab->OnMouseLClick(ptPos);
			}
		}
	}
	return __super::OnLButtonUP(ptPos);
}

bool CMessageBoxImgFull::OnLButtonDbClick(const LSY::CPoint &ptPos)
{
	m_pTab->OnLButtonDbClick(ptPos);
	return __super::OnLButtonDbClick(ptPos);
}

void CMessageBoxImgFull::OnMouseLClick(const LSY::CPoint &ptPos)
{
	m_pTab->OnMouseLClick(ptPos);
	__super::OnMouseLClick(ptPos);
}