#include "stdafx.h"
#include "Radio.h"
#include "../Base/Group.h"
#include "../ObjectMan.h"
#include "../base/ObjectBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CRadio::CRadio(void)
:	m_bTab(false)
{
}

CRadio::~CRadio()
{
}

CObjectBase *CRadio::NewObject(void)
{
	return new CRadio;
}

void CRadio::OnSetParent(CObjectBase *pParent, bool bSet)
{
	if(bSet && pParent && (pParent->GetType() == EOT_TAB) && IsLocalChild())
	{
		m_bTab = true;
		SetChildShowRef();
	}
	else if(m_bTab)
	{
		m_bTab = false;
	}
}

CObjectBase *CRadio::Copy(CObjectBase *pObject)
{
	CCheckButton::Copy(pObject);

	CRadio *pButton = (CRadio *)pObject;
	return pObject;
}

void CRadio::OnMouseLClick(const CPoint &ptPos)
{
	SetCheck(true, true);
	CButton::OnMouseLClick(ptPos);
}

bool CRadio::SetCheck(const bool &bCheck, const bool &bSendMsg/* = false*/)
{
	if(IsCheck() == bCheck) return false;
	if(m_bLocalChild && bCheck && ((m_pParent->GetType() == EOT_GROUP) || (m_pParent->GetType() == EOT_TAB)))
	{
		CGroup *pGroup = (CGroup *)m_pParent;
		pGroup->ClearRadioCheck(this, bSendMsg);
	}
	bool bRet = __super::SetCheck(bCheck, bSendMsg);
	if(m_bTab) SetChildShowRef();
	return bRet;
}

void CRadio::DrawChild(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bTab && !IsCheck()) return;
	__super::DrawChild(pPage);
}

void CRadio::SetChildShowRef(void)
{
	bool bShowRef = m_bShowRef && m_bShow;
	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->SetShowRef(bShowRef);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	if(m_bTab) bShowRef = m_bShow && m_bShowRef && m_bCheck;

	for(;start != end;++start)
	{
		(*start)->SetShowRef(bShowRef);
	}
}

void CRadio::AddChild(CObjectBase *pObject, CObjectBase *pBeforeObject/* = NULL*/, bool bBefore/* = true*/)
{
	__super::AddChild(pObject, pBeforeObject, bBefore);
	if(m_bTab)
	{
		bool bShowRef = m_bShow && m_bShowRef && m_bCheck;
		pObject->SetShowRef(bShowRef);
	}
}

bool CRadio::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);
	if(m_bTab) SetChildShowRef();
	return true;
}
} //namespace LSY