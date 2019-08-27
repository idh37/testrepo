#include "stdafx.h"
#include "ProgressiveBar.h"
#include "../Base/Image.h"
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
CProgressiveBar::CProgressiveBar(void)
:	m_pBar(NULL),
	m_fCurValue(0.0f),
	m_fMaxValue(0.0f),
	m_fMinValue(0.0f),
	m_bIncrease(true),
	m_bHorizontality(true),
	m_nIndex(-1)
{
}

CProgressiveBar::~CProgressiveBar()
{
}

CObjectBase *CProgressiveBar::NewObject(void)
{
	return new CProgressiveBar;
}

CObjectBase *CProgressiveBar::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CProgressiveBar *pControl			= (CProgressiveBar *)pObject;
	pControl->m_fCurValue				= m_fCurValue;
	pControl->m_fMaxValue				= m_fMaxValue;
	pControl->m_fMinValue				= m_fMinValue;
	pControl->m_bIncrease				= m_bIncrease;
	pControl->m_bHorizontality			= m_bHorizontality;

	if(m_pBar)
	{
		pControl->m_pBar	= (CImage *)m_pBar->Clone();
		pControl->AddLocalChild(pControl->m_pBar);
	}

	pControl->SetIndex(m_nIndex);

	return pObject;
}

size_t CProgressiveBar::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(float);		//m_fCurValue
	nRet += sizeof(float);		//m_fMaxValue
	nRet += sizeof(float);		//m_fMinValue
	nRet += sizeof(bool);		//m_bIncrease
	nRet += sizeof(bool);		//m_bHorizontality
	nRet += sizeof(size_t);		//이미지명 크기
	if(m_pBar) nRet += m_pBar->GetFileName().size();	//이미지명
	nRet += sizeof(int);		//m_nIndex
	return nRet;
}

size_t CProgressiveBar::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_fCurValue, sizeof(float));
	nRet += sizeof(float);		//m_fCurValue
	memcpy(pDest+nRet, &m_fMaxValue, sizeof(float));
	nRet += sizeof(float);		//m_fMaxValue
	memcpy(pDest+nRet, &m_fMinValue, sizeof(float));
	nRet += sizeof(float);		//m_fMinValue
	memcpy(pDest+nRet, &m_bIncrease, sizeof(bool));
	nRet += sizeof(bool);		//m_bIncrease
	memcpy(pDest+nRet, &m_bHorizontality, sizeof(bool));
	nRet += sizeof(bool);		//m_bHorizontality

	size_t nSize = 0;
	if(m_pBar) nSize = m_pBar->GetFileName().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//이미지명 크기
	if(m_pBar)
	{
		memcpy(pDest+nRet, m_pBar->GetFileName().c_str(), nSize);
		nRet += nSize;	//이미지명
	}

	memcpy(pDest+nRet, &m_nIndex, sizeof(int));
	nRet += sizeof(int);		//m_nIndex
	return nRet;
}

size_t CProgressiveBar::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	std::string strFileName;

	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_fCurValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fCurValue
	memcpy(&m_fMaxValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fMaxValue
	memcpy(&m_fMinValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fMinValue
	memcpy(&m_bIncrease, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bIncrease
	memcpy(&m_bHorizontality, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bHorizontality

	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//이미지명 크기
	if(nSize)
	{
		
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//이미지명
	}

	memcpy(&m_nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nIndex

	Create(strFileName.c_str());

	if(IsTool()) SetMessage(true);

	return nRet;
}

bool CProgressiveBar::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetDoubleAttribute("cur",  m_fCurValue);
	xmlInheritance.SetDoubleAttribute("max",  m_fMaxValue);
	xmlInheritance.SetDoubleAttribute("min",  m_fMinValue);
	xmlInheritance.SetAttribute("Increase",  m_bIncrease?1:0);
	xmlInheritance.SetAttribute("Horizontality",  m_bHorizontality?1:0);
	xmlInheritance.SetAttribute("path",  m_pBar?m_pBar->GetFileName().c_str():"");
	xmlInheritance.SetAttribute("index",  m_nIndex);

	pXmlObject->InsertEndChild(xmlInheritance);

	return true;
}

bool CProgressiveBar::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "cur", m_fCurValue);
	GetAttribute(pXmlInheritance, "max", m_fMaxValue);
	GetAttribute(pXmlInheritance, "min", m_fMinValue);
	GetAttribute(pXmlInheritance, "Increase", m_bIncrease);
	GetAttribute(pXmlInheritance, "Horizontality", m_bHorizontality);
	GetAttribute(pXmlInheritance, "path", strTemp);
	GetAttribute(pXmlInheritance, "index", m_nIndex);

	Create(strTemp.c_str());

	if(IsTool()) SetMessage(true);

	return true;
}

bool CProgressiveBar::Create (LPCTSTR lpPath)  
{
	bool bAddBar = false;

	if(m_pBar== NULL)
	{
		m_pBar = new CImage; 
		bAddBar = true;
	}

	if(m_pBar->Create(lpPath) == false)
	{
		delete m_pBar;
		m_pBar = NULL;
		return false ;
	}

	if(m_pBar->GetTotalNum() == 0) SetIndex(-1);
	else if(m_pBar->GetTotalNum() <= m_nIndex)
	{
		m_nIndex = 0;
		SetIndex(0);
	}
	else
	{
		m_pBar->SetIndex(m_nIndex);
	}

	SetLocalRect(m_pBar->GetLocalRect());
	SetCurValue(m_fCurValue);

	m_pBar->SetTempEventDraw(m_bEventDraw);

	if(bAddBar) AddLocalChild(m_pBar);
	m_pBar->SetLocalPos( 0, 0 );

	return true ; 
}

void CProgressiveBar::SetIndex(int nIndex)
{
	if(m_nIndex == nIndex) return;
	ASSERT(nIndex >= -1 && GetTotalNum() > nIndex);
	m_nIndex = nIndex;
	if(m_pBar)
	{
		m_pBar->SetIndex(nIndex);
		SetLocalRect(m_pBar->GetLocalRect());
		SetCurValue(m_fCurValue);
	}
}

const int CProgressiveBar::GetTotalNum(void)
{
	return m_pBar?m_pBar->GetTotalNum():0;
}

const std::string CProgressiveBar::GetFileName(void)
{
	return m_pBar?m_pBar->GetFileName():"";
}

const int CProgressiveBar::GetIndex(void)
{
	return m_pBar?m_pBar->GetIndex():-1;
}

void CProgressiveBar::SetRangeValue(const float &fMin, const float &fMax) 
{
	m_fMinValue = fMin ; 
	m_fMaxValue = fMax ; 

	if( m_fCurValue < m_fMinValue )
		m_fCurValue = m_fMinValue;

	if( m_fCurValue > m_fMaxValue )
		m_fCurValue = m_fMaxValue;

	SetCurValue(m_fCurValue);
}

void CProgressiveBar::SetCurValue(const float &fCur)
{
	if(fCur < m_fMinValue) m_fCurValue = m_fMinValue;
	else if(fCur > m_fMaxValue) m_fCurValue = m_fMaxValue;
	else m_fCurValue = fCur ; 

	float fRate = 0.0f;
	if(m_fMaxValue != m_fMinValue) fRate = (m_fCurValue - m_fMinValue) / (m_fMaxValue - m_fMinValue) ; 
	CRect rect ( 0, 0, m_pBar->GetWidth(), m_pBar->GetHeight() ) ; 

	if ( m_bIncrease ) 
	{
		if ( m_bHorizontality )
		{
			rect.right *= fRate;
		}
		else
		{
			rect.top = rect.bottom - rect.bottom * fRate ; 
			rect.top = float((int)rect.top);
		}
	}
	else
	{
		if ( m_bHorizontality ) 
		{
			rect.left = rect.right - rect.right * fRate;
			rect.left = float((int)rect.left);
		}
		else
		{
			rect.bottom *= fRate;
		}
	}

	if(m_pBar)
	{
		CRect *pClipRect = m_pBar->GetLocalClipRect();
		bool bSet = true;
		if(pClipRect && *pClipRect == rect)
		{
			bSet = false;
		}

		if(bSet) 
		{
			m_pBar->SetLocalClipRect ( rect );
			m_pBar->CheckEventObjectDraw();
		}
	}
}

void CProgressiveBar::SetHorizontality(const bool &bHorizontality) 
{
	if(m_bHorizontality == bHorizontality) return;
	m_bHorizontality = bHorizontality;
	SetCurValue(m_fCurValue);
}

void CProgressiveBar::SetIncrease(const bool &bIncrease)
{
	if(m_bIncrease == bIncrease) return;
	m_bIncrease = bIncrease;
	SetCurValue(m_fCurValue);
}

void CProgressiveBar::SetObjectProject(CObjectProject *pProject)
{
	__super::SetObjectProject(pProject);
	if(m_pBar) m_pBar->SetTempEventDraw(m_bEventDraw);
}
} //namespace LSY