#include "stdafx.h"
#include "TabControl.h"
#include "../Base/Image.h"
#include "../Button/Radio.h"
#include "../../Functor/Message.h"
#include "../Base/DumyImage.h"
#include "../../tinyxml/tinyxml.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CTabControl::CTabControl(void)
:	m_pImage(NULL),
	m_nIndex(-1),
	m_nIndexGap(0),
	m_pRectImage(NULL)
{
	SetMessage(true);

	if(IsTool())
	{
		m_pRectImage = new CDumyImage;
		m_pRectImage->SetLocalAlpha(0.5f);
		m_pRectImage->Create(25, 25);
		m_pRectImage->ClearColor(RGB(128, 128, 20));
		SetLocalRect(m_pRectImage->GetLocalRect());
		AddLocalChild(m_pRectImage);
	}
}

CTabControl::~CTabControl()
{
}

bool CTabControl::Create(LPCTSTR lpPath)
{
	bool bAdd = false;
	if(lpPath != NULL && lpPath[0] != NULL)
	{
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

		int nCurSel = GetCurSelIndex();
		if(nCurSel < 0) nCurSel = 0;
		int nImageIndex = m_nIndex + nCurSel*m_nIndexGap;
		if(nImageIndex < 0) nImageIndex = 0;
		else if(nImageIndex >= m_pImage->GetTotalNum()-1) nImageIndex = m_pImage->GetTotalNum() - 1;

		m_pImage->SetIndex(nImageIndex);
		SetLocalRect(m_pImage->GetLocalRect());

		if(bAdd)
		{
			CObjectBase *pAfterObject = NULL;
			if(m_listLocalChild.begin() != m_listLocalChild.end())
				pAfterObject = *m_listLocalChild.begin();
			AddLocalChild(m_pImage, pAfterObject);
		}
	}
	else
	{
		if(m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
	}

	if(m_pImage)
	{
		delete m_pRectImage;
		m_pRectImage = NULL;
	}
	else if(IsTool() && m_pRectImage == NULL)
	{
		m_pRectImage = new CDumyImage;
		m_pRectImage->SetLocalAlpha(0.5f);
		m_pRectImage->Create(25, 25);
		m_pRectImage->ClearColor(RGB(128, 128, 20));
		SetLocalRect(m_pRectImage->GetLocalRect());
		AddLocalChild(m_pRectImage);
	}

	return true;
}

void CTabControl::SetIndex(const int &nIndex)
{
	if(GetIndex() == nIndex) return;
	m_nIndex = nIndex;
	if(m_pImage)
	{
		int nCurSel = GetCurSelIndex();
		if(nCurSel < 0) nCurSel = 0;
		int nImageIndex = m_nIndex + nCurSel*m_nIndexGap;
		if(nImageIndex < 0) nImageIndex = 0;
		else if(nImageIndex >= m_pImage->GetTotalNum()-1) nImageIndex = m_pImage->GetTotalNum() - 1;

		m_pImage->SetIndex(nImageIndex);
		SetLocalRect(m_pImage->GetLocalRect());
	}
}

void CTabControl::SetCurSelFromObject(CRadio *pRadio)
{
	__super::SetCurSelFromObject(pRadio);
	if(m_pImage)
	{
		int nCurSel = GetCurSelIndex();
		int nIndex = m_nIndex + nCurSel*m_nIndexGap;
		if(nIndex < 0) nIndex = 0;
		else if(nIndex >= m_pImage->GetTotalNum()-1) nIndex = m_pImage->GetTotalNum() - 1;
		m_pImage->SetIndex(nIndex);
		SetLocalRect(m_pImage->GetLocalRect());
	}
}

void CTabControl::ClearRadioCheck(CRadio *pException, const bool &bSendMsg/* = false*/)
{
	__super::ClearRadioCheck(pException, bSendMsg);
	if(m_pImage)
	{
		int nCurSel = GetRadioToIndex(pException);
		int nIndex = m_nIndex + nCurSel*m_nIndexGap;
		if(nIndex < 0) nIndex = 0;
		else if(nIndex >= m_pImage->GetTotalNum()-1) nIndex = m_pImage->GetTotalNum() - 1;
		m_pImage->SetIndex(nIndex);
		SetLocalRect(m_pImage->GetLocalRect());
	}
}

const int CTabControl::GetIndex(void)
{
	return m_nIndex;
}

void CTabControl::SetIndexGap(const int &nIndexGap)
{
	if(GetIndexGap() == nIndexGap) return;
	m_nIndexGap = nIndexGap;
	if(m_pImage)
	{
		int nCurSel = GetCurSelIndex();
		if(nCurSel < 0) nCurSel = 0;
		int nImageIndex = m_nIndex + nCurSel*m_nIndexGap;
		if(nImageIndex < 0) nImageIndex = 0;
		else if(nImageIndex >= m_pImage->GetTotalNum()-1) nImageIndex = m_pImage->GetTotalNum() - 1;

		m_pImage->SetIndex(nImageIndex);
		SetLocalRect(m_pImage->GetLocalRect());
	}
}

const int CTabControl::GetIndexGap(void)
{
	return m_nIndexGap;
}

const std::string CTabControl::GetFileName(void)
{
	return m_pImage?m_pImage->GetFileName():"";
}

const int CTabControl::GetTotalNum(void)
{
	return m_pImage?m_pImage->GetTotalNum():0;
}

CObjectBase *CTabControl::NewObject(void)
{
	return new CTabControl;
}

CObjectBase *CTabControl::Copy(CObjectBase *pObject)
{
	CGroup::Copy(pObject);

	CTabControl *pText				= (CTabControl *)pObject;
	if(m_pImage)
	{
		pText->m_pImage	= (CImage *)m_pImage->Clone();
		pText->AddLocalChild(pText->m_pImage);
	}
	pText->m_nIndexGap = m_nIndexGap;
	pText->SetIndex(m_nIndex);
	return pObject;
}

size_t CTabControl::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);		//파일명 크기
	if(m_pImage) nRet += m_pImage->GetFileName().size(); //파일명
	nRet += sizeof(int);		//m_nIndex;
	nRet += sizeof(int);		//m_nIndexGap;
	return nRet;
}

size_t CTabControl::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pImage) nSize = m_pImage->GetFileName().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기

	if(m_pImage)
	{
		memcpy(pDest+nRet, m_pImage->GetFileName().c_str(), nSize);
		nRet += nSize; //파일명
	}
	memcpy(pDest+nRet, &m_nIndex, sizeof(int));
	nRet += sizeof(int);		//m_nIndex;
	memcpy(pDest+nRet, &m_nIndexGap, sizeof(int));
	nRet += sizeof(int);		//m_nIndexGap;
	return nRet;
}

size_t CTabControl::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기

	std::string  strFileName;

	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize; //파일명
	}
	memcpy(&m_nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nIndex;
	memcpy(&m_nIndexGap, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nIndexGap;

	Create(strFileName.c_str());

	return nRet;
}

bool CTabControl::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("path",  m_pImage?m_pImage->GetFileName().c_str():"");
	xmlInheritance.SetAttribute("index",  m_nIndex);
	xmlInheritance.SetAttribute("indexgap",  m_nIndexGap);
	pXmlObject->InsertEndChild(xmlInheritance);

	return true;
}

bool CTabControl::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "path", strTemp);
	GetAttribute(pXmlInheritance, "index", m_nIndex);
	if(pXmlInheritance->Attribute("indexgap")) GetAttribute(pXmlInheritance, "indexgap", m_nIndexGap);

	Create(strTemp.c_str());

	return true;
}
} //namespace LSY