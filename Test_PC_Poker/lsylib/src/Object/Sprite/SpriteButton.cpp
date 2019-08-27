#include "stdafx.h"
#include "SpriteButton.h"
#include "../Text/TextButton.h"
#include "../ObjectMan.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CSpriteButton::CSpriteButton(void)
:	m_nStatus(EBS_NORMAL),
	m_pSprite(NULL),
	m_nStatusType(EBST_FOURSTATUS),
	m_bUseMultiScene(false),
	m_nRepeatCount(0),
	m_pTextButton(NULL)
{
	InitButtonStatus();
	SetMessage(true);
}

CSpriteButton::~CSpriteButton()
{
}

void CSpriteButton::InitButtonStatus(int nIndex/* = -1*/)
{
	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = nIndex;
	}
	SetStatusIndex();
}

bool CSpriteButton::Create(LPCTSTR lpPath)
{
	bool bAdd = false;
	if(m_pSprite == NULL)
	{
		m_pSprite = new CSprite;
		bAdd = true;
	}

	if(m_pSprite->Create(lpPath) == false)
	{
		delete m_pSprite;
		m_pSprite = NULL;
		return false;
	}

	m_pSprite->SetAfterHide(false);
	m_pSprite->SetRepeatCount(m_nRepeatCount);
	m_pSprite->SetUseMultiScene(m_bUseMultiScene);

	if(bAdd) AddLocalChild(m_pSprite);
	SetLocalRect(m_pSprite->GetImageRect(0));

	int nStartIndex = m_listStatusIndex[EBS_NORMAL];
	int nMaxIndex = m_pSprite->GetTotalNum();

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return true;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_SPRITEBUTTON)?1:2);
	if((m_nStatusType != EBST_FREESTATUS) && (nIndex > nMaxIndex))
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus(0);
		return true;
	}

	switch(m_nStatusType)
	{
	case EBST_FOURSTATUS:
		if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
		{
			nStartIndex = 0;
		}
		SetFourStatusButton(nStartIndex);
		break;
	case EBST_THREESTATUS:
		if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
		{
			nStartIndex = 0;
		}
		SetThreeStatusButton(nStartIndex);
		break;
	case EBST_TWOSTATUS:
		if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
		{
			nStartIndex = 0;
		}
		SetTwoStatusButton(nStartIndex);
		break;
	case EBST_ONESTATUS:
		if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
		{
			nStartIndex = 0;
		}
		SetOneStatusButton(nStartIndex);
		break;
	case EBST_FREESTATUS:
		{
			for(int i=EBS_NORMAL;i<EBS_MAX;++i)
			{
				if(m_listStatusIndex[i] >= nMaxIndex)
				{
					InitButtonStatus(0);
					break;
				}
			}
		}
		break;
	}

	SetStatusIndex();

	return true;
}

std::string CSpriteButton::GetFileName(void)
{
	if(m_pSprite) return m_pSprite->GetFileName();
	return "";
}

CObjectBase *CSpriteButton::NewObject(void)
{
	return new CSpriteButton;
}

CObjectBase *CSpriteButton::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CSpriteButton *pButton = (CSpriteButton *)pObject;

	memcpy(&pButton->m_listStatusIndex, &m_listStatusIndex, sizeof(int) * EBS_MAX);
	pButton->m_nStatus = EBS_NORMAL;
	pButton->m_nStatusType = m_nStatusType;
	pButton->m_bUseMultiScene = m_bUseMultiScene;
	pButton->m_nRepeatCount = m_nRepeatCount;
	if(m_pSprite)
	{
		pButton->m_pSprite = (CSprite *)m_pSprite->Clone(true);
		pButton->AddLocalChild(pButton->m_pSprite);
	}
	if(m_pTextButton)
	{
		pButton->m_pTextButton = (CTextButton *)m_pTextButton->Clone(true);
		pButton->AddLocalChild(pButton->m_pTextButton);
	}
	pButton->SetStatusIndex();

	return pObject;
}

size_t CSpriteButton::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet +=	sizeof(size_t);							//파일명 크기
	if(m_pSprite) nRet += m_pSprite->GetFileName().size();	//파일명
	nRet += sizeof(int) * EBS_MAX;							//m_listStatusIndex
	nRet += sizeof(ENUM_BUTTON_STATUS_TYPE);				//m_nStatusType
	nRet += sizeof(bool);									//m_bUseMultiScene
	nRet += sizeof(int);									//m_nRepeatCount
	nRet += sizeof(bool);									//m_pTextButton의 존재 여부
	if(m_pTextButton)
	{
		nRet += m_pTextButton->GetClipSize(nVersion);
	}
	return nRet;
}

size_t CSpriteButton::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pSprite) nSize = m_pSprite->GetFileName().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet +=	sizeof(size_t);							//파일명 크기
	if(m_pSprite)
	{
		memcpy(pDest+nRet, m_pSprite->GetFileName().c_str(), m_pSprite->GetFileName().size());
		nRet += m_pSprite->GetFileName().size();	//파일명
	}
	memcpy(pDest+nRet, m_listStatusIndex, sizeof(int)*EBS_MAX);
	nRet += sizeof(int) * EBS_MAX;							//m_listStatusIndex
	memcpy(pDest+nRet, &m_nStatusType, sizeof(ENUM_BUTTON_STATUS_TYPE));
	nRet += sizeof(ENUM_BUTTON_STATUS_TYPE);				//m_nStatusType
	memcpy(pDest+nRet, &m_bUseMultiScene, sizeof(bool));
	nRet += sizeof(bool);				//m_bUseMultiScene
	memcpy(pDest+nRet, &m_nRepeatCount, sizeof(int));
	nRet += sizeof(int);				//m_nRepeatCount

	bool bUse = m_pTextButton?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);

	if(bUse) nRet += m_pTextButton->SaveClipSize(nVersion, pDest+nRet);
	return nRet;
}

size_t CSpriteButton::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	std::string strFileName;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet +=	sizeof(size_t);							//파일명 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//파일명
	}
	memcpy(m_listStatusIndex, pDest+nRet, sizeof(int)*EBS_MAX);
	nRet += sizeof(int) * EBS_MAX;							//m_listStatusIndex
	memcpy(&m_nStatusType, pDest+nRet, sizeof(ENUM_BUTTON_STATUS_TYPE));
	nRet += sizeof(ENUM_BUTTON_STATUS_TYPE);				//m_nStatusType
	memcpy(&m_bUseMultiScene, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);									//m_bUseMultiScene
	memcpy(&m_nRepeatCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);									//m_nRepeatCount

	Create(strFileName.c_str());

	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_pTextButton 사용유무
	if(bUse)
	{
		CTextButton *pButton = new CTextButton;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pButton->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachTextButton(pButton);
	}


	return nRet;
}

bool CSpriteButton::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("path",  m_pSprite?m_pSprite->GetFileName().c_str():"");
	xmlInheritance.SetAttribute("usemultiscene",  m_bUseMultiScene?1:0);
	xmlInheritance.SetAttribute("repeat",  m_nRepeatCount);
	xmlInheritance.SetAttribute("index0",  m_listStatusIndex[EBS_NORMAL]);
	xmlInheritance.SetAttribute("index1",  m_listStatusIndex[EBS_OVER]);
	xmlInheritance.SetAttribute("index2",  m_listStatusIndex[EBS_DOWN]);
	xmlInheritance.SetAttribute("index3",  m_listStatusIndex[EBS_DISABLE]);
	xmlInheritance.SetAttribute("index4",  m_listStatusIndex[EBS_SNORMAL]);
	xmlInheritance.SetAttribute("index5",  m_listStatusIndex[EBS_SOVER]);
	xmlInheritance.SetAttribute("index6",  m_listStatusIndex[EBS_SDOWN]);
	xmlInheritance.SetAttribute("index7",  m_listStatusIndex[EBS_SDISABLE]);
	xmlInheritance.SetAttribute("type",  (int)m_nStatusType);
	pXmlObject->InsertEndChild(xmlInheritance);

	TiXmlElement xmlAttach("attach");
	if(m_pTextButton) m_pTextButton->SaveXml(&xmlAttach);
	pXmlObject->InsertEndChild(xmlAttach);

	return true;
}

bool CSpriteButton::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	int nType = 0;
	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "path", strTemp);
	GetAttribute(pXmlInheritance, "usemultiscene", m_bUseMultiScene);
	GetAttribute(pXmlInheritance, "repeat", m_nRepeatCount);
	GetAttribute(pXmlInheritance, "index0", m_listStatusIndex[EBS_NORMAL]);
	GetAttribute(pXmlInheritance, "index1", m_listStatusIndex[EBS_OVER]);
	GetAttribute(pXmlInheritance, "index2", m_listStatusIndex[EBS_DOWN]);
	GetAttribute(pXmlInheritance, "index3", m_listStatusIndex[EBS_DISABLE]);
	GetAttribute(pXmlInheritance, "index4", m_listStatusIndex[EBS_SNORMAL]);
	GetAttribute(pXmlInheritance, "index5", m_listStatusIndex[EBS_SOVER]);
	GetAttribute(pXmlInheritance, "index6", m_listStatusIndex[EBS_SDOWN]);
	GetAttribute(pXmlInheritance, "index7", m_listStatusIndex[EBS_SDISABLE]);
	GetAttribute(pXmlInheritance, "type", nType);
	m_nStatusType = (ENUM_BUTTON_STATUS_TYPE)nType;

	Create(strTemp.c_str());

	TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
	if(pXmlAttach)
	{
		CObjectBase *pChildObject = NULL;
		TiXmlElement *pXmlChildObject = pXmlAttach->FirstChildElement("Object");
		while(pXmlChildObject)
		{
			pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
			switch(pChildObject->GetType())
			{
			case EOT_TEXTBUTTON:
				AttachTextButton((CTextButton *)pChildObject);
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			if(pChildObject) OnAttachChild(pChildObject);
			pChildObject->OnCreated();
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
	}

	return true;
}

void CSpriteButton::SetStatusIndex(void)
{
	if(m_pSprite)
	{
		m_pSprite->SetPlayIndex(m_listStatusIndex[m_nStatus]);
		if(m_pProject && m_bShow && m_bShowRef) m_pSprite->StartAnimation();
		SetLocalRect(m_pSprite->GetImageRect(0));
	}
}

void CSpriteButton::SetButtonStatus(const ENUM_BUTTON_STATUS &nStatus)
{
	if(m_nStatus == nStatus) return;
	m_nStatus = nStatus;
	if(m_pTextButton) m_pTextButton->SetButtonStatus(nStatus);
	SetStatusIndex();
}

void CSpriteButton::SetButtonStatusIndex(ENUM_BUTTON_STATUS nStatus, int nIndex)
{
	if(m_nStatusType != EBST_FREESTATUS) return;
	if(m_listStatusIndex[nStatus] == nIndex) return;
	m_listStatusIndex[nStatus] = nIndex;
	SetStatusIndex();
}

int CSpriteButton::GetButtonStatusIndex(ENUM_BUTTON_STATUS nStatus)
{
	return m_listStatusIndex[nStatus];
}

void CSpriteButton::GetButtonStatusIndex(int *pListIndex)
{
	memcpy(pListIndex, m_listStatusIndex, sizeof(int) * EBS_MAX);
}

void CSpriteButton::SetEnable(const bool &bEnable)
{
	CObjectBase::SetEnable(bEnable);

	if(!m_bEnable) SetButtonStatus(EBS_DISABLE);
	else if(m_bMouseOver) SetButtonStatus(EBS_OVER);
	else SetButtonStatus(EBS_NORMAL);

	SetStatusIndex();
}

void CSpriteButton::OnMouseOver(void)
{
	CObjectBase::OnMouseOver();

	if(m_bEnable) SetButtonStatus(EBS_OVER);
	SetStatusIndex();
}

void CSpriteButton::OnMouseLeave(void)
{
	CObjectBase::OnMouseLeave();
	if(m_bEnable) SetButtonStatus(EBS_NORMAL);
	SetStatusIndex();
}

bool CSpriteButton::OnLButtonDown(const CPoint &ptPos)
{
	if(m_bEnable) SetButtonStatus(EBS_DOWN);
	SetStatusIndex();
	return CObjectBase::OnLButtonDown(ptPos);
}

bool CSpriteButton::OnLButtonDbClick(const CPoint &ptPos)
{
	if(m_bEnable) SetButtonStatus(EBS_DOWN);
	SetStatusIndex();
	return CObjectBase::OnLButtonDbClick(ptPos);
}

bool CSpriteButton::OnLButtonUP(const CPoint &ptPos)
{
	if(m_bEnable)
	{
		if(m_bMouseOver) SetButtonStatus(EBS_OVER);
		else SetButtonStatus(EBS_NORMAL);
	}
	SetStatusIndex();
	return CObjectBase::OnLButtonUP(ptPos);
}

void CSpriteButton::SetFourStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pSprite?m_pSprite->GetTotalNum():0;
	m_nStatusType = EBST_FOURSTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_SPRITEBUTTON)?1:2);
	if(nIndex > nMaxIndex)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus(0);
		return;
	}

	if(nStartIndex + nIndex > nMaxIndex)
	{
		nStartIndex = 0;
	}

	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = -1;
	}

	m_listStatusIndex[EBS_NORMAL] = nStartIndex;
	m_listStatusIndex[EBS_OVER] = nStartIndex+1;
	m_listStatusIndex[EBS_DOWN] = nStartIndex+2;
	m_listStatusIndex[EBS_DISABLE] = nStartIndex+3;
	SetStatusIndex();
}

void CSpriteButton::SetThreeStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pSprite?m_pSprite->GetTotalNum():0;
	m_nStatusType = EBST_THREESTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_SPRITEBUTTON)?1:2);
	if(nIndex > nMaxIndex)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus(0);
		return;
	}

	if(nStartIndex + nIndex > nMaxIndex)
	{
		nStartIndex = 0;
	}

	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = -1;
	}

	m_listStatusIndex[EBS_NORMAL] = nStartIndex;
	m_listStatusIndex[EBS_OVER] = nStartIndex;
	m_listStatusIndex[EBS_DOWN] = nStartIndex+1;
	m_listStatusIndex[EBS_DISABLE] = nStartIndex+2;
	SetStatusIndex();
}

void CSpriteButton::SetTwoStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pSprite?m_pSprite->GetTotalNum():0;
	m_nStatusType = EBST_TWOSTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_SPRITEBUTTON)?1:2);
	if(nIndex > nMaxIndex)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus(0);
		return;
	}

	if(nStartIndex + nIndex > nMaxIndex)
	{
		nStartIndex = 0;
	}

	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = -1;
	}

	m_listStatusIndex[EBS_NORMAL]	= nStartIndex;
	m_listStatusIndex[EBS_OVER]		= nStartIndex;
	m_listStatusIndex[EBS_DOWN]		= nStartIndex+1;
	m_listStatusIndex[EBS_DISABLE]	= nStartIndex;
	SetStatusIndex();
}

void CSpriteButton::SetOneStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pSprite?m_pSprite->GetTotalNum():0;
	m_nStatusType = EBST_ONESTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_SPRITEBUTTON)?1:2);
	if(nIndex > nMaxIndex)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus(0);
		return;
	}

	if(nStartIndex + nIndex > nMaxIndex)
	{
		nStartIndex = 0;
	}

	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = -1;
	}

	m_listStatusIndex[EBS_NORMAL]	= nStartIndex;
	m_listStatusIndex[EBS_OVER]		= nStartIndex;
	m_listStatusIndex[EBS_DOWN]		= nStartIndex;
	m_listStatusIndex[EBS_DISABLE]	= nStartIndex;
	SetStatusIndex();
}

void CSpriteButton::SetFreeStatusButton(int *plistStatusIndex)
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

	for(int i = EBS_NORMAL;i<=EBS_DISABLE;++i)
	{
		m_listStatusIndex[i] = plistStatusIndex[i];
	}
	SetStatusIndex();
}

const int CSpriteButton::GetTotalNum(void)
{
	return m_pSprite?m_pSprite->GetTotalNum():0;
}

void CSpriteButton::SetUseMultiScene(const bool &bUseMultiSecne)
{
	if(m_bUseMultiScene == bUseMultiSecne) return;
	m_bUseMultiScene = bUseMultiSecne;
	if(m_pSprite)
	{
		m_pSprite->SetUseMultiScene(m_bUseMultiScene);

		int nStartIndex = m_listStatusIndex[EBS_NORMAL];
		int nMaxIndex = m_pSprite->GetTotalNum();

		if(nMaxIndex == 0)
		{
			m_nStatusType = EBST_FREESTATUS;
			InitButtonStatus();
			return;
		}

		int nIndex = m_nStatusType * ((GetType() == EOT_SPRITEBUTTON)?1:2);
		if((m_nStatusType != EBST_FREESTATUS) && (nIndex > nMaxIndex))
		{
			m_nStatusType = EBST_FREESTATUS;
			InitButtonStatus(0);
			return;
		}

		switch(m_nStatusType)
		{
		case EBST_FOURSTATUS:
			if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
			{
				nStartIndex = 0;
			}
			SetFourStatusButton(nStartIndex);
			break;
		case EBST_THREESTATUS:
			if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
			{
				nStartIndex = 0;
			}
			SetThreeStatusButton(nStartIndex);
			break;
		case EBST_TWOSTATUS:
			if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
			{
				nStartIndex = 0;
			}
			SetTwoStatusButton(nStartIndex);
			break;
		case EBST_ONESTATUS:
			if(nStartIndex + nIndex > nMaxIndex || nStartIndex == -1)
			{
				nStartIndex = 0;
			}
			SetOneStatusButton(nStartIndex);
			break;
		case EBST_FREESTATUS:
			{
				for(int i=EBS_NORMAL;i<EBS_MAX;++i)
				{
					if(m_listStatusIndex[i] >= nMaxIndex)
					{
						InitButtonStatus(0);
						break;
					}
				}
			}
			break;
		}

		SetStatusIndex();
	}
}

void CSpriteButton::SetRepeatCount(const int &nRepeatCount)
{
	if(m_nRepeatCount == nRepeatCount) return;
	m_nRepeatCount = nRepeatCount;
	if(m_pSprite)
	{
		m_pSprite->SetRepeatCount(nRepeatCount);
		if(m_pProject && m_bShow && m_bShowRef) m_pSprite->StartAnimation();
	}
}

void CSpriteButton::AttachTextButton(CTextButton *pButton)
{
	ASSERT(m_pTextButton == NULL);
	m_pTextButton = pButton;
	m_pTextButton->SetMessage(false);
	AddLocalChild(m_pTextButton);
	m_pTextButton->SetButtonStatus(m_nStatus);
}

void CSpriteButton::DettachTextButton(void)
{
	ASSERT(m_pTextButton != NULL);
	m_pTextButton->SetMessage(true);
	if(m_pTextButton->IsEnable()) m_pTextButton->SetButtonStatus(EBS_NORMAL);
	else m_pTextButton->SetButtonStatus(EBS_DISABLE);
	SubChild(m_pTextButton);
	m_pTextButton = NULL;
}

void CSpriteButton::SetShow(const bool &bShow)
{
	__super::SetShow(bShow);
	if(m_pSprite)
	{
		if(m_pParent && m_bShow && m_bShowRef)
		{
			if(!m_pSprite->IsAnimation()) m_pSprite->StartAnimation();
		}
		else 
		{
			if(m_pSprite->IsAnimation()) m_pSprite->StopAnimation();
		}
	}
}

void CSpriteButton::SetShowRef(const bool &bShowRef)
{
	__super::SetShowRef(bShowRef);
	if(m_pSprite)
	{
		if(m_pParent && m_bShow && m_bShowRef)
		{
			if(!m_pSprite->IsAnimation()) m_pSprite->StartAnimation();
		}
		else 
		{
			if(m_pSprite->IsAnimation()) m_pSprite->StopAnimation();
		}
	}
}
} //namespace LSY