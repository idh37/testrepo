#include "stdafx.h"
#include "Window.h"
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
CWindows::CWindows(void)
:	m_pPopupObject(NULL),
	m_bModal(NULL)
{
	SetMessage(true);
}

CWindows::~CWindows()
{
	if(m_pPopupObject)
	{
		SubChild(m_pPopupObject);
		m_pPopupObject = NULL;
	}
}

CObjectBase *CWindows::NewObject(void)
{
	return new CWindows;
}

CObjectBase *CWindows::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CWindows *pWindows			= (CWindows *)pObject;
	pWindows->m_bModal = m_bModal;
	return pObject;
}

size_t CWindows::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);		//m_bModal
	return nRet;
}

size_t CWindows::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_bModal, sizeof(bool));
	nRet += sizeof(bool);		//m_bModal
	return nRet;
}

size_t CWindows::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_bModal, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bModal
	return nRet;
}

bool CWindows::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("modal", m_bModal?1:0);
	pXmlObject->InsertEndChild(xmlInheritance);
	return true;
}

bool CWindows::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	TiXmlElement *pXmlFile = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlFile, "modal", m_bModal);

	return true;
}

void CWindows::SetPopup(CObjectBase *pPopup)
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

void CWindows::SubChild(CObjectBase *pObject)
{
	if(m_pPopupObject == pObject) m_pPopupObject = NULL;
	__super::SubChild(pObject);
}

bool CWindows::IsIn(IN CPoint &ptPos, OUT bool &bIn)
{
	bool bRet = __super::IsIn(ptPos, bIn);
	if(bRet == false && bIn == false && m_bModal && !IsTool()) bIn = true;
	return bRet;
}

void CWindows::SetModal(const bool &bModal)
{
	if(m_bModal == bModal) return;
	m_bModal = bModal;
}
} //namespace LSY