#include "stdafx.h"
#include "Image.h"
#include <algorithm>
#include "../../tinyxml/tinyxml.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CImage::CImage(void)
:	m_pSprite(NULL),
	m_nIndex(-1),
	m_nTotalNum(0),
	m_pMiddleImage(NULL),
	m_pEndImage(NULL),
	m_nMiddleCount(0),
	m_bHorizontality(false)
{
}

CImage::~CImage()
{
	if(m_pMiddleImage) delete m_pMiddleImage;
	m_pMiddleImage = NULL;

	if(m_pSprite) RM().RemoveSprite(m_pSprite);
	m_pSprite = NULL;

	m_listMiddleImage.clear();
}

bool CImage::Create(LPCTSTR lpPath)
{
	if(m_pSprite)
	{
		if(m_pSprite->GetFilePath() == lpPath) return true;
		else
		{
			RM().RemoveSprite(m_pSprite);
			m_pSprite = NULL;
		}
	}

	m_pSprite = RM().GetSprite(lpPath);
	if(m_pSprite == NULL) return false;
	m_nTotalNum = m_pSprite->GetTotalSpr();

	if(m_nIndex >= m_nTotalNum)
	{
		m_nIndex = 0;
	}
	else if(m_nTotalNum == 0)
	{
		m_nIndex = -1;
	}

	SetIndex(m_nIndex, true);

	return true;
}

bool CImage::Create(NMBASE::GRAPHICGDI::xSprite *pSprite)
{
	if(pSprite == NULL) return false;
	if(pSprite == m_pSprite) return true;

	if(m_pSprite)
	{
		RM().RemoveSprite(m_pSprite);
		m_pSprite = NULL;
	}

	m_pSprite = RM().Clone(pSprite);
	if(m_pSprite == NULL) return false;
	m_nTotalNum = m_pSprite->GetTotalSpr();

	if(m_nIndex >= m_nTotalNum)
	{
		m_nIndex = 0;
	}
	else if(m_nTotalNum == 0)
	{
		m_nIndex = -1;
	}

	SetIndex(m_nIndex, true);

	return true;
}

void CImage::SetIndex(int nIndex, bool bForce/* = false*/)
{
	//강제 셋팅이 아니고 인덱스가 같으면 변경되지 않는다.
	if(bForce == false && m_nIndex == nIndex) return;

	if(m_bTempEventDraw) CheckEventObjectDraw();
	else CheckEventDraw();
	m_nIndex = nIndex;
	if(m_pSprite == NULL) return;
	if(m_nIndex == -1)
	{
		SetLocalRect(CRectZero);
		return;
	}

	CRect rtRect = GetImageRect(nIndex);

	if(m_pMiddleImage && m_nMiddleCount > 0)
	{
		LISTOBJECTITER start = m_listMiddleImage.begin();
		LISTOBJECTITER end = m_listMiddleImage.end();
		CImage *pImage = NULL;
		float fLength = 0.0f;

		if(m_bHorizontality)
		{
			fLength = m_pMiddleImage->GetWidth();
		}
		else
		{
			fLength = m_pMiddleImage->GetHeight();
		}

		for(int i=0;i<m_nMiddleCount;++i)
		{
			if(start == end) break;
			pImage = (CImage *)(*start);
			pImage->SetShow(true);
			++start;

			if(m_bHorizontality)
			{
				pImage->SetLocalPos(rtRect.right, 0);
				rtRect.right += fLength;
			}
			else
			{
				pImage->SetLocalPos(0, rtRect.bottom);
				rtRect.bottom += fLength;
			}
		}
	}

	if(m_pEndImage)
	{
		if(m_bHorizontality)
		{
			m_pEndImage->SetLocalPos(rtRect.right, 0);
			rtRect.right += m_pEndImage->GetWidth();
		}
		else
		{
			m_pEndImage->SetLocalPos(0, rtRect.bottom);
			rtRect.bottom += m_pEndImage->GetHeight();
		}
	}

	SetLocalRect(rtRect);
}

bool CImage::IsPossibilityIndex(const int &nIndex)
{
	if(nIndex >= 0 && nIndex < m_nTotalNum) return true;
	return false;
}

const CRect CImage::GetImageRect(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < GetTotalNum());
	return CRect(0,0,(float)m_pSprite->spr[nIndex].xl,(float)m_pSprite->spr[nIndex].yl);
}

const int &CImage::GetTotalNum(void)
{
	return m_nTotalNum;
}

void CImage::DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bTempEventDraw)
	{
		if(!m_bClipDraw || !m_bDraw) return;
		CObjectMan *pObjectMan = m_pProject?m_pProject->GetObjectMan():NULL;
		if(pObjectMan) pObjectMan->AddEventObject(this);
		return;
	}
	__super::DrawLocal(pPage);
}

void CImage::DrawMy(NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_rtOldDraw = m_rtRect;
	__super::DrawMy(pPage);
	if(m_nIndex < 0 || pPage == NULL || m_pSprite == NULL) return;

	int nOption = (int)m_dwDrawOption;
	if((m_pSprite->spr[m_nIndex].PixelFormat == 5558) && !(nOption & GRAYSCL))
	{
		nOption &= ~(HB | GB);
		nOption |= AB;
	}

	switch(m_nDrawType)
	{
	case EODT_ROTATION:
	case EODT_SCALEROTATION:
		pPage->RotateZoomSprAuto((int)m_ptDraw.x, (int)m_ptDraw.y, m_pSprite, m_nIndex, (int)m_rtRect.GetWidth(), (int)m_rtRect.GetHeight(), m_fDrawRotaion, nOption, m_nAlpha, m_nColor);
		break;
	case EODT_SCALE:
		pPage->ZoomSprAuto((int)m_ptDraw.x, (int)m_ptDraw.y, m_pSprite, m_nIndex, (int)m_rtRect.GetWidth(), (int)m_rtRect.GetHeight(), nOption, m_nAlpha, m_nColor);
		break;
	case EODT_NORMAL:
		pPage->PutSprAuto((int)m_ptDraw.x, (int)m_ptDraw.y, m_pSprite, m_nIndex, nOption, m_nAlpha, m_nColor);
		break;
	}
}

CObjectBase *CImage::NewObject(void)
{
	return new CImage;
}

CObjectBase *CImage::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CImage *pImage = (CImage *)pObject;
	//이전에 읽어들인 이미지가 있다면 지운다.
	if(pImage->m_pSprite) RM().RemoveSprite(pImage->m_pSprite);
	//새로운 이미지를 복사해 온다.
	if(m_pSprite) pImage->m_pSprite = RM().Clone(m_pSprite);
	pImage->m_nIndex = m_nIndex;
	pImage->m_nTotalNum = m_nTotalNum;

	pImage->m_nMiddleCount = m_nMiddleCount;
	pImage->m_bHorizontality = m_bHorizontality;
	if(m_pMiddleImage)
	{
		CImage *pImage = (CImage *)m_pMiddleImage->Clone(true);
		AttachMiddle(pImage);
	}
	if(m_pEndImage)
	{
		CImage *pImage = (CImage *)m_pEndImage->Clone(true);
		AttachEnd(pImage);
	}

	return pObject;
}

std::string CImage::GetFileName(void)
{
	if(m_pSprite) return m_pSprite->GetFilePath();
	return "";
}

size_t CImage::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);			//이름크기
	if(m_pSprite) nRet += m_pSprite->GetFilePath().size();	//이름
	nRet += sizeof(int);			//m_nIndex
	nRet += sizeof(int);			//m_nTotalNum
	nRet += sizeof(int);			//m_nMiddleCount
	nRet += sizeof(bool);			//m_bHorizontality
	nRet += sizeof(bool);			//m_pMiddleImage 사용유무
	if(m_pMiddleImage) nRet += m_pMiddleImage->GetClipSize(nVersion);
	nRet += sizeof(bool);			//m_pEndImage 사용유무
	if(m_pEndImage) nRet += m_pEndImage->GetClipSize(nVersion);
	return nRet;
}

size_t CImage::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pSprite) nSize = m_pSprite->GetFilePath().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);			//이름크기
	if(m_pSprite)
	{
		memcpy(pDest+nRet, m_pSprite->GetFilePath().c_str(), nSize);
		nRet += nSize;	//이름
	}
	memcpy(pDest+nRet, &m_nIndex, sizeof(int));
	nRet += sizeof(int);			//m_nIndex
	memcpy(pDest+nRet, &m_nTotalNum, sizeof(int));
	nRet += sizeof(int);			//m_nTotalNum
	memcpy(pDest+nRet, &m_nMiddleCount, sizeof(int));
	nRet += sizeof(int);			//m_nMiddleCount
	memcpy(pDest+nRet, &m_bHorizontality, sizeof(bool));
	nRet += sizeof(bool);			//m_bHorizontality
	
	bool bUse = m_pMiddleImage?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);			//m_pMiddleImage 사용유무
	if(bUse) nRet += m_pMiddleImage->SaveClipSize(nVersion, pDest+nRet);

	bUse = m_pEndImage?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);			//m_pEndImage 사용유무
	if(bUse) nRet += m_pEndImage->SaveClipSize(nVersion, pDest+nRet);
	return nRet;
}

size_t CImage::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	std::string strFileName;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);			//이름크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//이름
	}
	memcpy(&m_nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nIndex
	memcpy(&m_nTotalNum, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nTotalNum

	memcpy(&m_nMiddleCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nMiddleCount
	memcpy(&m_bHorizontality, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_bHorizontality

	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_pMiddleImage 사용유무
	if(bUse)
	{
		CImage *pImage = new CImage;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pImage->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachMiddle(pImage);
	}

	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_pEndImage 사용유무
	if(bUse)
	{
		CImage *pImage = new CImage;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pImage->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachEnd(pImage);
	}

	Create(strFileName.c_str());

	if(IsTool()) SetMessage(true);

	return nRet;
}

bool CImage::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlFile("file");
	xmlFile.SetAttribute("path", m_pSprite?m_pSprite->GetFilePath().c_str():"");
	xmlFile.SetAttribute("index", m_nIndex);
	xmlFile.SetAttribute("middlecount", m_nMiddleCount);
	xmlFile.SetAttribute("horizontality", m_bHorizontality?1:0);
	pXmlObject->InsertEndChild(xmlFile);

	TiXmlElement xmlAttach_middle("attach");
	if(m_pMiddleImage) m_pMiddleImage->SaveXml(&xmlAttach_middle);
	pXmlObject->InsertEndChild(xmlAttach_middle);

	TiXmlElement xmlAttach_end("attachadd");
	if(m_pEndImage) m_pEndImage->SaveXml(&xmlAttach_end);
	pXmlObject->InsertEndChild(xmlAttach_end);

	return true;
}

bool CImage::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	TiXmlElement *pXmlFile = pXmlObject->FirstChildElement("file");
	GetAttribute(pXmlFile, "path", strTemp);
	GetAttribute(pXmlFile, "index", m_nIndex);
	if(pXmlFile->Attribute("middlecount")) GetAttribute(pXmlFile, "middlecount", m_nMiddleCount);
	if(pXmlFile->Attribute("horizontality")) GetAttribute(pXmlFile, "horizontality", m_bHorizontality);

	TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
	CObjectBase *pChildObject = NULL;
	TiXmlElement *pXmlChildObject = NULL;
	if(pXmlAttach)
	{
		pXmlChildObject = pXmlAttach->FirstChildElement("Object");
		while(pXmlChildObject)
		{
			pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
			switch(pChildObject->GetType())
			{
			case EOT_IMAGE:
				AttachMiddle((CImage *)pChildObject);
				break;
			}
			OnAttachChild(pChildObject);
			pChildObject->OnCreated();
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
	}

	pXmlAttach = pXmlObject->FirstChildElement("attachadd");
	if(pXmlAttach)
	{
		pXmlChildObject = pXmlAttach->FirstChildElement("Object");
		while(pXmlChildObject)
		{
			pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
			switch(pChildObject->GetType())
			{
			case EOT_IMAGE:
				AttachEnd((CImage *)pChildObject);
				break;
			}
			OnAttachChild(pChildObject);
			pChildObject->OnCreated();
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
	}

	Create(strTemp.c_str());

	if(IsTool()) SetMessage(true);

	return true;
}

void CImage::AttachMiddle(CImage *pImage)
{
	ASSERT(m_pMiddleImage == NULL);
	m_pMiddleImage = pImage;
	if(m_pMiddleImage->GetParent())
		m_pMiddleImage->GetParent()->SubChild(m_pMiddleImage);
	MakeMiddleEndImage();
}

void CImage::DettachMiddle(void)
{
	ASSERT(m_pMiddleImage != NULL);
	CImage *pImage = m_pMiddleImage;
	m_pMiddleImage = NULL;
	MakeMiddleEndImage();
	pImage->SetShow(true);
}

void CImage::AttachEnd(CImage *pImage)
{
	ASSERT(m_pEndImage == NULL);
	m_pEndImage = pImage;
	AddLocalChild(m_pEndImage);
	SetIndex(m_nIndex, true);
}

void CImage::DettachEnd(void)
{
	ASSERT(m_pEndImage != NULL);
	SubChild(m_pEndImage);
	m_pEndImage = NULL;
	SetIndex(m_nIndex, true);
}

void CImage::SetMiddleCount(const int &nMiddleCount)
{
	if(m_nMiddleCount == nMiddleCount) return;
	m_nMiddleCount = nMiddleCount;
	MakeMiddleEndImage();
}

void CImage::SetHorizontality(const bool &bHorizontality)
{
	if(m_bHorizontality == bHorizontality) return;
	m_bHorizontality = bHorizontality;
	MakeMiddleEndImage();
}

void CImage::ReleaseMiddleEndImage(void)
{
	LISTOBJECTITER start = m_listMiddleImage.begin();
	LISTOBJECTITER end = m_listMiddleImage.end();
	for(;start != end;++start)
	{
		delete *start;		
	}
	m_listMiddleImage.clear();
}

void CImage::MakeMiddleEndImage(void)
{
	ReleaseMiddleEndImage();

	CRect rtRect = CRectZero;
	if(m_pSprite && m_nIndex >= 0) rtRect = GetImageRect(m_nIndex);

	if(m_pMiddleImage && m_nMiddleCount > 0)
	{
		CImage *pImage = NULL;
		float fLength = 0.0f;

		if(m_bHorizontality)
		{
			fLength = m_pMiddleImage->GetWidth();
		}
		else
		{
			fLength = m_pMiddleImage->GetHeight();
		}
		for(int i=0;i<m_nMiddleCount;++i)
		{
			pImage = (LSY::CImage *)m_pMiddleImage->Clone(true, true, false, NULL);
			AddLocalChild(pImage);
			m_listMiddleImage.push_back(pImage);
	
			if(m_bHorizontality)
			{
				pImage->SetLocalPos(rtRect.right, 0);
				rtRect.right += fLength;
			}
			else
			{
				pImage->SetLocalPos(0, rtRect.bottom);
				rtRect.bottom += fLength;
			}
		}
	}

	if(m_pEndImage)
	{
		if(m_bHorizontality)
		{
			m_pEndImage->SetLocalPos(rtRect.right, 0);
			rtRect.right += m_pEndImage->GetWidth();
		}
		else
		{
			m_pEndImage->SetLocalPos(0, rtRect.bottom);
			rtRect.bottom += m_pEndImage->GetHeight();
		}
		AddLocalChild(m_pEndImage);
	}

	SetLocalRect(rtRect);
}

CImage *CImage::GetMiddleImage(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_nMiddleCount) return NULL;

	LISTOBJECT::iterator start = m_listMiddleImage.begin();
	LISTOBJECT::iterator end = m_listMiddleImage.end();
	for(int i=0;start != end;++start,++i)
	{
		if(i == nIndex) return (CImage *)(*start);
	}

	return NULL;
}

int CImage::GetMiddleImageToIndex(CImage *pImage)
{
	if(pImage == NULL) return -1;

	LISTOBJECT::iterator start = m_listMiddleImage.begin();
	LISTOBJECT::iterator end = m_listMiddleImage.end();
	for(int i=0;start != end;++start,++i)
	{
		if(*start == pImage) return i;
	}
	return -1;
}

void CImage::CheckEventDraw(void)
{
	if(m_bTempEventDraw)
	{
	}
	else
	{
		__super::CheckEventDraw();
	}
}
} //namespace LSY