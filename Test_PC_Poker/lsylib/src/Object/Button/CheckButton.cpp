#include "stdafx.h"
#include "CheckButton.h"
#include "../../Functor/Message.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CCheckButton::CCheckButton(void)
:	m_bCheck(false)
{
}

CCheckButton::~CCheckButton()
{
}

CObjectBase *CCheckButton::NewObject(void)
{
	return new CCheckButton;
}

CObjectBase *CCheckButton::Copy(CObjectBase *pObject)
{
	CButton::Copy(pObject);

	CCheckButton *pButton = (CCheckButton *)pObject;
	pButton->m_bCheck = m_bCheck;
	pButton->CheckStatus();
	return pObject;
}

size_t CCheckButton::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);		//m_bCheck
	return nRet;
}

size_t CCheckButton::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_bCheck, sizeof(bool));
	nRet += sizeof(bool);		//m_bCheck
	return nRet;
}

size_t CCheckButton::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_bCheck, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bCheck
	CheckStatus();
	return nRet;
}

bool CCheckButton::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement *pXmlInheritance = (TiXmlElement *)pXmlObject->FirstChild("Inheritance");
	pXmlInheritance->SetAttribute("check",  m_bCheck?1:0);
	return true;
}

bool CCheckButton::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "check", m_bCheck);
	CheckStatus();

	return true;
}

void CCheckButton::CheckStatus(void)
{
	ENUM_BUTTON_STATUS nStatus = m_nStatus;
	if(m_bCheck)
	{
		if(nStatus < EBS_SNORMAL)	nStatus = (ENUM_BUTTON_STATUS)((int)nStatus + (int)EBS_SNORMAL);
	}
	else
	{
		if(nStatus >= EBS_SNORMAL) nStatus = (ENUM_BUTTON_STATUS)((int)nStatus - (int)EBS_SNORMAL);
	}

	SetButtonStatus(nStatus);
}

void CCheckButton::SetEnable(const bool &bEnable)
{
	CButton::SetEnable(bEnable);
	CheckStatus();
}

void CCheckButton::SetEnableRef(const bool &bEnableRef)
{
	CButton::SetEnableRef(bEnableRef);
	CheckStatus();
}

void CCheckButton::OnMouseOver(void)
{
	CButton::OnMouseOver();
	CheckStatus();
}

void CCheckButton::OnMouseLeave(void)
{
	CButton::OnMouseLeave();
	CheckStatus();
}

bool CCheckButton::OnLButtonDown(const CPoint &ptPos)
{
	bool bRet = CButton::OnLButtonDown(ptPos);
	CheckStatus();
	return bRet;
}

bool CCheckButton::OnLButtonDbClick(const CPoint &ptPos)
{
	bool bRet = CButton::OnLButtonDbClick(ptPos);
	CheckStatus();
	return bRet;
}

bool CCheckButton::OnLButtonUP(const CPoint &ptPos)
{
	bool bRet = CButton::OnLButtonUP(ptPos);
	CheckStatus();
	return bRet;
}

void CCheckButton::OnMouseLClick(const CPoint &ptPos)
{
	SetCheck(IsCheck()?false:true, true);
	CButton::OnMouseLClick(ptPos);
}

bool CCheckButton::SetCheck(const bool &bCheck, const bool &bSendMsg/* = false*/)
{
	if(m_bCheck == bCheck) return false;
	m_bCheck = bCheck;
	CheckStatus();

	if(bSendMsg)
	{
		CMO_CheckChange msg(this, m_bCheck);
		Dispatch(&msg);
	}
	return true;
}

void CCheckButton::CopyStatus(int nGab)
{
	for(int i=EBS_NORMAL;i<EBS_SNORMAL;++i)
	{
		m_listStatusIndex[i+EBS_SNORMAL] = m_listStatusIndex[i] + nGab;
	}
	SetStatusIndex();
}

void CCheckButton::SetFourStatusButton(int nStartIndex)
{
	CButton::SetFourStatusButton(nStartIndex);
	if(m_nStatusType == EBST_FOURSTATUS) CopyStatus(4);
}

void CCheckButton::SetThreeStatusButton(int nStartIndex)
{
	CButton::SetThreeStatusButton(nStartIndex);
	if(m_nStatusType == EBST_THREESTATUS) CopyStatus(3);
}

void CCheckButton::SetTwoStatusButton(int nStartIndex)
{
	CButton::SetTwoStatusButton(nStartIndex);
	if(m_nStatusType == EBST_TWOSTATUS) CopyStatus(2);
}

void CCheckButton::SetOneStatusButton(int nStartIndex)
{
	CButton::SetOneStatusButton(nStartIndex);
	if(m_nStatusType == EBST_ONESTATUS) CopyStatus(1);
}

void CCheckButton::SetFreeStatusButton(int *plistStatusIndex)
{
	int nMaxIndex = m_pImage?m_pImage->GetTotalNum():0;
	m_nStatusType = EBST_FREESTATUS;

	if(nMaxIndex == 0)
	{
		InitButtonStatus();
		return;
	}

	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		if(plistStatusIndex[i] >= nMaxIndex)
		{
			InitButtonStatus(0);
			return;
		}
	}

	for(int i = EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = plistStatusIndex[i];
	}
	SetStatusIndex();
}
} //namespace LSY