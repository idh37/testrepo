#include "stdafx.h"
#include "Button.h"
#include "../Text/TextButton.h"
#include "../../tinyxml/tinyxml.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CButton::CButton(void)
:	m_nStatus(EBS_NORMAL),
	m_pImage(NULL),
	m_nStatusType(EBST_FOURSTATUS),
	m_pTextButton(NULL)
{
	InitButtonStatus();
	SetMessage(true);
}

CButton::~CButton()
{
}

void CButton::InitButtonStatus(int nIndex/* = -1*/)
{
	for(int i=EBS_NORMAL;i<EBS_MAX;++i)
	{
		m_listStatusIndex[i] = nIndex;
	}
	SetStatusIndex();
}

bool CButton::Create(LPCTSTR lpPath)
{
	bool bAdd = false;
	if(m_pImage == NULL)
	{
		m_pImage = new CImage;
		bAdd = true;
	}

	if(m_pImage->Create(lpPath) == false)
	{
		delete m_pImage;
		m_pImage = NULL;
		return false;
	}

	if(bAdd) AddLocalChild(m_pImage);
	SetLocalRect(m_pImage->GetLocalRect());

	int nStartIndex = m_listStatusIndex[EBS_NORMAL];
	int nMaxIndex = m_pImage->GetTotalNum();

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return true;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_BUTTON)?1:2);
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

std::string CButton::GetFileName(void)
{
	if(m_pImage) return m_pImage->GetFileName();
	return "";
}

CObjectBase *CButton::NewObject(void)
{
	return new CButton;
}

CObjectBase *CButton::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CButton *pButton = (CButton *)pObject;

	memcpy(&pButton->m_listStatusIndex, &m_listStatusIndex, sizeof(int) * EBS_MAX);
	pButton->m_nStatus = EBS_NORMAL;
	pButton->m_nStatusType = m_nStatusType;
	if(m_pImage)
	{
		pButton->m_pImage = (CImage *)m_pImage->Clone(true);
		pButton->AddLocalChild(pButton->m_pImage);
	}
	if(m_pTextButton)
	{
		pButton->m_pTextButton = (CTextButton *)m_pTextButton->Clone(true);
		pButton->AddLocalChild(pButton->m_pTextButton);
	}
	pButton->SetStatusIndex();

	return pObject;
}

size_t CButton::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet +=	sizeof(size_t);							//파일명 크기
	if(m_pImage) nRet += m_pImage->GetFileName().size();	//파일명
	nRet += sizeof(int) * EBS_MAX;							//m_listStatusIndex
	nRet += sizeof(ENUM_BUTTON_STATUS_TYPE);				//m_nStatusType
	nRet += sizeof(bool);									//m_pTextButton의 존재 여부
	if(m_pTextButton)
	{
		nRet += m_pTextButton->GetClipSize(nVersion);
	}
	return nRet;
}

size_t CButton::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pImage) nSize = m_pImage->GetFileName().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet +=	sizeof(size_t);							//파일명 크기
	if(nSize)
	{
		memcpy(pDest+nRet, m_pImage->GetFileName().c_str(), nSize);
		nRet += nSize;	//파일명
	}
	memcpy(pDest+nRet, m_listStatusIndex, sizeof(int)*EBS_MAX);
	nRet += sizeof(int) * EBS_MAX;							//m_listStatusIndex
	memcpy(pDest+nRet, &m_nStatusType, sizeof(ENUM_BUTTON_STATUS_TYPE));
	nRet += sizeof(ENUM_BUTTON_STATUS_TYPE);				//m_nStatusType

	bool bUse = m_pTextButton?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);

	if(bUse) nRet += m_pTextButton->SaveClipSize(nVersion, pDest+nRet);
	return nRet;
}

size_t CButton::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
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

bool CButton::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("path",  m_pImage?m_pImage->GetFileName().c_str():"");
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

bool CButton::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	int nType = 0;
	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "path", strTemp);
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
	std::string strType = "";
	if(pXmlAttach)
	{
		CObjectBase *pChildObject = NULL;
		TiXmlElement *pXmlChildObject = pXmlAttach->FirstChildElement("Object");
		while(pXmlChildObject)
		{
			pChildObject = NULL;
			strType = pXmlChildObject->Attribute("Type");
			switch(GetStringToObjectType(strType))
			{
			case EOT_TEXTBUTTON:
				pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
				AttachTextButton((CTextButton *)pChildObject);
				break;
			case EOT_TEXT:
				if(GetType() == EOT_CHECKBOX || GetType() == EOT_RADIO)
					break;
			default:
				ASSERT(FALSE);
				break;
			}
			if(pChildObject)
			{
				OnAttachChild(pChildObject);
				pChildObject->OnCreated();
			}
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
	}

	return true;
}

void CButton::SetStatusIndex(void)
{
	if(m_pImage)
	{
		m_pImage->SetIndex(m_listStatusIndex[m_nStatus]);
		SetLocalRect(m_pImage->GetLocalRect());
	}
}

void CButton::SetButtonStatusIndex(ENUM_BUTTON_STATUS nStatus, int nIndex)
{
	if(m_nStatusType != EBST_FREESTATUS) return;
	if(m_listStatusIndex[nStatus] == nIndex) return;
	m_listStatusIndex[nStatus] = nIndex;
	SetStatusIndex();
}

int CButton::GetButtonStatusIndex(ENUM_BUTTON_STATUS nStatus)
{
	return m_listStatusIndex[nStatus];
}

void CButton::GetButtonStatusIndex(int *pListIndex)
{
	memcpy(pListIndex, m_listStatusIndex, sizeof(int) * EBS_MAX);
}

void CButton::SetButtonStatus(const ENUM_BUTTON_STATUS &nStatus)
{
	if(m_nStatus == nStatus) return;
	m_nStatus = nStatus;
	if(m_pTextButton) m_pTextButton->SetButtonStatus(nStatus);
	SetStatusIndex();
}

void CButton::SetEnable(const bool &bEnable)
{
	__super::SetEnable(bEnable);

	if(!m_bEnable || !m_bEnableRef) SetButtonStatus(EBS_DISABLE);
	else if(m_bMouseOver) SetButtonStatus(EBS_OVER);
	else SetButtonStatus(EBS_NORMAL);
}

void CButton::SetEnableRef(const bool &bEnableRef)
{
	__super::SetEnableRef(bEnableRef);

	if(!m_bEnable || !m_bEnableRef) SetButtonStatus(EBS_DISABLE);
	else if(m_bMouseOver) SetButtonStatus(EBS_OVER);
	else SetButtonStatus(EBS_NORMAL);
}

void CButton::OnMouseOver(void)
{
	__super::OnMouseOver();

	if(m_bEnable) SetButtonStatus(EBS_OVER);
}

void CButton::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	if(m_bEnable) SetButtonStatus(EBS_NORMAL);
}

bool CButton::OnLButtonDown(const CPoint &ptPos)
{
	if(m_bEnable) SetButtonStatus(EBS_DOWN);
	return __super::OnLButtonDown(ptPos);
}

bool CButton::OnLButtonDbClick(const CPoint &ptPos)
{
	if(m_bEnable) SetButtonStatus(EBS_DOWN);
	return __super::OnLButtonDbClick(ptPos);
}

bool CButton::OnLButtonUP(const CPoint &ptPos)
{
	if(m_bEnable)
	{
		if(m_bMouseOver) SetButtonStatus(EBS_OVER);
		else SetButtonStatus(EBS_NORMAL);
	}
	return __super::OnLButtonUP(ptPos);
}

void CButton::SetFourStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pImage?m_pImage->GetTotalNum():0;
	m_nStatusType = EBST_FOURSTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_BUTTON)?1:2);
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

void CButton::SetThreeStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pImage?m_pImage->GetTotalNum():0;
	m_nStatusType = EBST_THREESTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_BUTTON)?1:2);
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

void CButton::SetTwoStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pImage?m_pImage->GetTotalNum():0;
	m_nStatusType = EBST_TWOSTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_BUTTON)?1:2);
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

void CButton::SetOneStatusButton(int nStartIndex)
{
	int nMaxIndex = m_pImage?m_pImage->GetTotalNum():0;
	m_nStatusType = EBST_ONESTATUS;

	if(nMaxIndex == 0)
	{
		m_nStatusType = EBST_FREESTATUS;
		InitButtonStatus();
		return;
	}

	int nIndex = m_nStatusType * ((GetType() == EOT_BUTTON)?1:2);
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

void CButton::SetFreeStatusButton(int *plistStatusIndex)
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

	for(int i = EBS_NORMAL;i<=EBS_DISABLE;++i)
	{
		m_listStatusIndex[i] = plistStatusIndex[i];
	}
	SetStatusIndex();
}

const int CButton::GetTotalNum(void)
{
	return m_pImage?m_pImage->GetTotalNum():0;
}

void CButton::AttachTextButton(CTextButton *pButton)
{
	ASSERT(m_pTextButton == NULL);
	m_pTextButton = pButton;
	m_pTextButton->SetMessage(false);
	AddLocalChild(m_pTextButton);
	m_pTextButton->SetButtonStatus(m_nStatus);
}

void CButton::DettachTextButton(void)
{
	ASSERT(m_pTextButton != NULL);
	m_pTextButton->SetMessage(true);
	if(m_pTextButton->IsEnable()) m_pTextButton->SetButtonStatus(EBS_NORMAL);
	else m_pTextButton->SetButtonStatus(EBS_DISABLE);
	SubChild(m_pTextButton);
	m_pTextButton = NULL;
}

const CRect &CButton::GetImageRect(void)
{
	if(m_pImage) return m_pImage->GetLocalRect();
	return GetLocalRect();
}

void CButton::CheckEventDraw(void)
{
	if(m_bTempEventDraw) m_pImage->CheckEventObjectDraw();
	else __super::CheckEventDraw();
}

void CButton::SetTempEventDraw(const bool &bEventDraw)
{
	__super::SetTempEventDraw(bEventDraw);
	if(m_pImage) m_pImage->SetTempEventDraw(bEventDraw);
}

void CButton::ForceChangStatusNomal()
{
	SetButtonStatus(EBS_NORMAL);
}

void CButton::ForceChangStatusOver()
{
	SetButtonStatus(EBS_OVER);
}

} //namespace LSY