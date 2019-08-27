#include "stdafx.h"
#include "SpriteCheckButton.h"
#include "../../Functor/Message.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CSpriteCheckButton::CSpriteCheckButton(void)
:	m_bCheck(false)
{
}

CSpriteCheckButton::~CSpriteCheckButton()
{
}

CObjectBase *CSpriteCheckButton::NewObject(void)
{
	return new CSpriteCheckButton;
}

CObjectBase *CSpriteCheckButton::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CSpriteCheckButton *pButton = (CSpriteCheckButton *)pObject;
	pButton->m_bCheck = m_bCheck;
	pButton->CheckStatus();
	return pObject;
}

size_t CSpriteCheckButton::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);		//m_bCheck
	return nRet;
}

size_t CSpriteCheckButton::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_bCheck, sizeof(bool));
	nRet += sizeof(bool);		//m_bCheck
	return nRet;
}

size_t CSpriteCheckButton::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_bCheck, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bCheck
	CheckStatus();
	return nRet;
}

bool CSpriteCheckButton::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement *pXmlInheritance = (TiXmlElement *)pXmlObject->FirstChild("Inheritance");
	pXmlInheritance->SetAttribute("check",  m_bCheck?1:0);
	return true;
}

bool CSpriteCheckButton::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "check", m_bCheck);
	CheckStatus();

	return true;
}

void CSpriteCheckButton::CheckStatus(void)
{
	ENUM_BUTTON_STATUS nStatus = m_nStatus;
	if(m_bCheck)
	{
		if(nStatus < EBS_SNORMAL)	nStatus = (ENUM_BUTTON_STATUS)((int)m_nStatus + (int)EBS_SNORMAL);
	}
	else
	{
		if(nStatus >= EBS_SNORMAL) nStatus = (ENUM_BUTTON_STATUS)((int)m_nStatus - (int)EBS_SNORMAL);
	}

	SetButtonStatus(nStatus);
}

void CSpriteCheckButton::SetEnable(const bool &bEnable)
{
	__super::SetEnable(bEnable);
	CheckStatus();
}

void CSpriteCheckButton::OnMouseOver(void)
{
	__super::OnMouseOver();
	CheckStatus();
}

void CSpriteCheckButton::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	CheckStatus();
}

bool CSpriteCheckButton::OnLButtonDown(const CPoint &ptPos)
{
	bool bRet = __super::OnLButtonDown(ptPos);
	CheckStatus();
	return bRet;
}

bool CSpriteCheckButton::OnLButtonDbClick(const CPoint &ptPos)
{
	bool bRet = __super::OnLButtonDbClick(ptPos);
	CheckStatus();
	return bRet;
}

bool CSpriteCheckButton::OnLButtonUP(const CPoint &ptPos)
{
	bool bRet = __super::OnLButtonUP(ptPos);
	CheckStatus();
	return bRet;
}

void CSpriteCheckButton::OnMouseLClick(const CPoint &ptPos)
{
	SetCheck(IsCheck()?false:true);
	__super::OnMouseLClick(ptPos);
}

bool CSpriteCheckButton::SetCheck(bool bCheck)
{
	if(m_bCheck == bCheck) return false;
	m_bCheck = bCheck;
	CheckStatus();

	CMO_CheckChange msg(this, m_bCheck);
	Dispatch(&msg);
	return true;
}

void CSpriteCheckButton::CopyStatus(int nGab)
{
	for(int i=EBS_NORMAL;i<EBS_SNORMAL;++i)
	{
		m_listStatusIndex[i+EBS_SNORMAL] = m_listStatusIndex[i] + nGab;
	}
	SetStatusIndex();
}

void CSpriteCheckButton::SetFourStatusButton(int nStartIndex)
{
	__super::SetFourStatusButton(nStartIndex);
	if(m_nStatusType == EBST_FOURSTATUS) CopyStatus(4);
}

void CSpriteCheckButton::SetThreeStatusButton(int nStartIndex)
{
	__super::SetThreeStatusButton(nStartIndex);
	if(m_nStatusType == EBST_THREESTATUS) CopyStatus(3);
}

void CSpriteCheckButton::SetTwoStatusButton(int nStartIndex)
{
	__super::SetTwoStatusButton(nStartIndex);
	if(m_nStatusType == EBST_TWOSTATUS) CopyStatus(2);
}

void CSpriteCheckButton::SetOneStatusButton(int nStartIndex)
{
	__super::SetOneStatusButton(nStartIndex);
	if(m_nStatusType == EBST_ONESTATUS) CopyStatus(1);
}

void CSpriteCheckButton::SetFreeStatusButton(int *plistStatusIndex)
{
	int nMaxIndex = m_pSprite?m_pSprite->GetTotalNum():0;
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