#include "stdafx.h"
#include "SpriteWindow.h"
#include "../../Functor/Message.h"
#include "../../tinyxml/tinyxml.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CSpriteWindows::CSpriteWindows(void)
:	m_pPopupObject(NULL),
	m_bModal(NULL)
{
	SetMessage(true);
}

CSpriteWindows::~CSpriteWindows()
{
	if(m_pPopupObject)
	{
		SubChild(m_pPopupObject);
		m_pPopupObject = NULL;
	}
}

bool CSpriteWindows::Create(LPCTSTR strPath)
{
	bool bRet = __super::Create(strPath);
	if(!bRet) return false;
	if(m_pSprite->GetTotalSpr() != 0)
	{
		SetLocalRect(CRect(0,0,(float)m_pSprite->spr[0].xl, (float)m_pSprite->spr[0].yl));
	}
	else
	{
		SetLocalRect(CRectZero);
	}
	return true;
}

void CSpriteWindows::SetShow(const bool &bShow)
{
	if(m_bShow == bShow) return;
	__super::SetShow(bShow);
	if(m_bShow && m_bShowRef && m_pProject)
		StartAnimation();
}

void CSpriteWindows::SetShowRef(const bool &bShowRef)
{
	if(m_bShowRef == bShowRef) return;
	__super::SetShowRef(bShowRef);
	if(m_bShow && m_bShowRef && m_pProject)
		StartAnimation();
}

void CSpriteWindows::OnAddSubRootObject(bool bAdd)
{
	__super::OnAddSubRootObject(bAdd);

	if(m_pProject && m_bShow && m_bShowRef)
	{
		if(bAdd) StartAnimation();
	}
}

CObjectBase *CSpriteWindows::NewObject(void)
{
	return new CSpriteWindows;
}

CObjectBase *CSpriteWindows::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CSpriteWindows *pWindows			= (CSpriteWindows *)pObject;
	pWindows->m_bModal = m_bModal;
	return pObject;
}

size_t CSpriteWindows::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);		//m_bModal
	return nRet;
}

size_t CSpriteWindows::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_bModal, sizeof(bool));
	nRet += sizeof(bool);		//m_bModal
	return nRet;
}

size_t CSpriteWindows::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_bModal, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bModal
	return nRet;
}

bool CSpriteWindows::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	pXmlInheritance->SetAttribute("modal", m_bModal?1:0);
	return true;
}

bool CSpriteWindows::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "modal", m_bModal);

	return true;
}

void CSpriteWindows::SetPopup(CObjectBase *pPopup)
{
	ASSERT(m_pPopupObject == NULL);
	m_pPopupObject = pPopup;
	if(m_pPopupObject)
	{
		const CPoint ptPos = m_pPopupObject->GetPos();
		const float fScaleX = m_pPopupObject->GetScaleX();
		const float fScaleY = m_pPopupObject->GetScaleY();
		const float fRotation = m_pPopupObject->GetRotation();
		const float fAlpha = m_pPopupObject->GetLocalAlpha();

		AddChild(m_pPopupObject);

		m_pPopupObject->SetPos(ptPos);
		m_pPopupObject->SetScaleX(fScaleX);
		m_pPopupObject->SetScaleY(fScaleY);
		m_pPopupObject->SetRotation(fRotation);
		m_pPopupObject->SetLocalAlpha(fAlpha);
	}
}

void CSpriteWindows::SubChild(CObjectBase *pObject)
{
	if(m_pPopupObject == pObject) m_pPopupObject = NULL;
	__super::SubChild(pObject);
}

bool CSpriteWindows::IsIn(IN CPoint &ptPos, OUT bool &bIn)
{
	bool bRet = CObjectBase::IsIn(ptPos, bIn);
	if(bRet == false && bIn == false && m_bModal && !IsTool()) bIn = true;
	return bRet;
}

void CSpriteWindows::SetModal(const bool &bModal)
{
	if(m_bModal == bModal) return;
	m_bModal = bModal;
}
} //namespace LSY