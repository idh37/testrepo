#include "stdafx.h"
#include "SlideBar.h"
#include "../Base/Image.h"
#include "../Button/Button.h"
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
CSlideBar::CSlideBar(void)
:	m_pBar1(NULL),
	m_pBar2(NULL),
	m_pThumb(NULL),
	m_ptThumb(0.0f,0.0f),
	m_ptThumbPivot(0.0f,0.0f),
	m_fThumbLeftTopWidth(0.0f),
	m_fThumbRightBottomWidth(0.0f),
	m_fCurValue(0.0f),
	m_fMaxValue(0.0f),
	m_fMinValue(0.0f),
	m_rcBound(0.0f,0.0f,0.0f,0.0f),
	m_bIncrease(true),
	m_bHorizontality(true),
	m_nStartIndex(0)
{
	SetMessage(true);
}

CSlideBar::~CSlideBar()
{
}

CObjectBase *CSlideBar::NewObject(void)
{
	return new CSlideBar;
}

CObjectBase *CSlideBar::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CSlideBar *pSlide					= (CSlideBar *)pObject;
	pSlide->m_ptThumb					= m_ptThumb;
	pSlide->m_ptThumbPivot				= m_ptThumbPivot;
	pSlide->m_fThumbLeftTopWidth		= m_fThumbLeftTopWidth;
	pSlide->m_fThumbRightBottomWidth	= m_fThumbRightBottomWidth;
	pSlide->m_fCurValue					= m_fCurValue;
	pSlide->m_fMaxValue					= m_fMaxValue;
	pSlide->m_fMinValue					= m_fMinValue;
	pSlide->m_rcBound					= m_rcBound;
	pSlide->m_bIncrease					= m_bIncrease;
	pSlide->m_bHorizontality			= m_bHorizontality;
	pSlide->m_nStartIndex				= m_nStartIndex;

	if(m_pBar1)
	{
		pSlide->m_pBar1	= (CImage *)m_pBar1->Clone();
		pSlide->AddLocalChild(pSlide->m_pBar1);
	}
	if(m_pBar2)
	{
		pSlide->m_pBar2	= (CImage *)m_pBar2->Clone();
		pSlide->AddLocalChild(pSlide->m_pBar2);
	}
	if(m_pThumb)
	{
		pSlide->m_pThumb	= (CButton *)m_pThumb->Clone();
		pSlide->AddLocalChild(pSlide->m_pThumb);
	}

	return pObject;
}

size_t CSlideBar::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(CPoint);		//m_ptThumb
	nRet += sizeof(CPoint);		//m_ptThumbPivot
	nRet += sizeof(float);		//m_fThumbLeftTopWidth
	nRet += sizeof(float);		//m_fThumbRightBottomWidth
	nRet += sizeof(float);		//m_fCurValue
	nRet += sizeof(float);		//m_fMaxValue
	nRet += sizeof(float);		//m_fMinValue
	nRet += sizeof(CRect);		//m_rcBound
	nRet += sizeof(bool);		//m_bIncrease
	nRet += sizeof(bool);		//m_bHorizontality
	nRet += sizeof(size_t);		//파일명 크기
	if(m_pThumb) nRet += m_pThumb->GetFileName().size(); //파일명
	nRet += sizeof(int);		//m_nStartIndex
	return nRet;
}

size_t CSlideBar::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_ptThumb, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptThumb
	memcpy(pDest+nRet, &m_ptThumbPivot, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptThumbPivot
	memcpy(pDest+nRet, &m_fThumbLeftTopWidth, sizeof(float));
	nRet += sizeof(float);		//m_fThumbLeftTopWidth
	memcpy(pDest+nRet, &m_fThumbRightBottomWidth, sizeof(float));
	nRet += sizeof(float);		//m_fThumbRightBottomWidth
	memcpy(pDest+nRet, &m_fCurValue, sizeof(float));
	nRet += sizeof(float);		//m_fCurValue
	memcpy(pDest+nRet, &m_fMaxValue, sizeof(float));
	nRet += sizeof(float);		//m_fMaxValue
	memcpy(pDest+nRet, &m_fMinValue, sizeof(float));
	nRet += sizeof(float);		//m_fMinValue
	memcpy(pDest+nRet, &m_rcBound, sizeof(CRect));
	nRet += sizeof(CRect);		//m_rcBound
	memcpy(pDest+nRet, &m_bIncrease, sizeof(bool));
	nRet += sizeof(bool);		//m_bIncrease
	memcpy(pDest+nRet, &m_bHorizontality, sizeof(bool));
	nRet += sizeof(bool);		//m_bHorizontality

	size_t nSize = 0;
	if(m_pThumb) nSize = m_pThumb->GetFileName().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기

	if(m_pThumb)
	{
		memcpy(pDest+nRet, m_pThumb->GetFileName().c_str(), nSize);
		nRet += nSize; //파일명
	}
	memcpy(pDest+nRet, &m_nStartIndex, sizeof(int));
	nRet += sizeof(int);		//m_nStartIndex
	return nRet;
}

size_t CSlideBar::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_ptThumb, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptThumb
	memcpy(&m_ptThumbPivot, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptThumbPivot
	memcpy(&m_fThumbLeftTopWidth, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fThumbLeftTopWidth
	memcpy(&m_fThumbRightBottomWidth, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fThumbRightBottomWidth
	memcpy(&m_fCurValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fCurValue
	memcpy(&m_fMaxValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fMaxValue
	memcpy(&m_fMinValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fMinValue
	memcpy(&m_rcBound, pDest+nRet, sizeof(CRect));
	nRet += sizeof(CRect);		//m_rcBound
	memcpy(&m_bIncrease, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bIncrease
	memcpy(&m_bHorizontality, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bHorizontality

	size_t nSize = 0;
	std::string strFileName;

	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기

	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize; //파일명
	}
	memcpy(&m_nStartIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nStartIndex

	Create(strFileName.c_str());

	return nRet;
}

bool CSlideBar::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetDoubleAttribute("x",  m_ptThumbPivot.x);
	xmlInheritance.SetDoubleAttribute("y",  m_ptThumbPivot.y);
	xmlInheritance.SetDoubleAttribute("lefttop",  m_fThumbLeftTopWidth);
	xmlInheritance.SetDoubleAttribute("rightbottom",  m_fThumbRightBottomWidth);
	xmlInheritance.SetDoubleAttribute("cur",  m_fCurValue);
	xmlInheritance.SetDoubleAttribute("max",  m_fMaxValue);
	xmlInheritance.SetDoubleAttribute("min",  m_fMinValue);
	xmlInheritance.SetAttribute("Increase",  m_bIncrease?1:0);
	xmlInheritance.SetAttribute("Horizontality",  m_bHorizontality?1:0);
	xmlInheritance.SetAttribute("path",  m_pThumb?m_pThumb->GetFileName().c_str():"");
	xmlInheritance.SetAttribute("index",  m_nStartIndex);

	pXmlObject->InsertEndChild(xmlInheritance);

	return true;
}

bool CSlideBar::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "x", m_ptThumbPivot.x);
	GetAttribute(pXmlInheritance, "y", m_ptThumbPivot.y);
	GetAttribute(pXmlInheritance, "lefttop", m_fThumbLeftTopWidth);
	GetAttribute(pXmlInheritance, "rightbottom", m_fThumbRightBottomWidth);
	GetAttribute(pXmlInheritance, "cur", m_fCurValue);
	GetAttribute(pXmlInheritance, "max", m_fMaxValue);
	GetAttribute(pXmlInheritance, "min", m_fMinValue);
	GetAttribute(pXmlInheritance, "Increase", m_bIncrease);
	GetAttribute(pXmlInheritance, "Horizontality", m_bHorizontality);
	GetAttribute(pXmlInheritance, "path", strTemp);
	GetAttribute(pXmlInheritance, "index", m_nStartIndex);

	Create(strTemp.c_str());

	return true;
}

bool CSlideBar::Create (LPCTSTR lpPath)  
{
	bool bAddBar = false;

	if(m_pBar1 == NULL)
	{
		m_pBar1 = new CImage;
		m_pBar2 = new CImage;
		m_pThumb = new CButton;
		bAddBar = true;
	}

	m_pBar1->SetTempEventDraw(m_bEventDraw);
	m_pBar2->SetTempEventDraw(m_bEventDraw);
	m_pThumb->SetTempEventDraw(m_bEventDraw);

	if(m_pBar1->Create(lpPath) == false)
	{
		delete m_pBar1;
		delete m_pBar2;
		delete m_pThumb;
		m_pBar1 = NULL;
		m_pBar2 = NULL;
		m_pThumb = NULL;
		return false ;
	}

	if(m_pBar2->Create(lpPath) == false) 
	{
		delete m_pBar1;
		delete m_pBar2;
		delete m_pThumb;
		m_pBar1 = NULL;
		m_pBar2 = NULL;
		m_pThumb = NULL;
		return false ;
	}

	if(m_pThumb->Create(lpPath) == false) 
	{
		delete m_pBar1;
		delete m_pBar2;
		delete m_pThumb;
		m_pBar1 = NULL;
		m_pBar2 = NULL;
		m_pThumb = NULL;
		return false ;
	}

	if(bAddBar)
	{
		AddLocalChild(m_pBar1);
		AddLocalChild(m_pBar2);
		AddLocalChild(m_pThumb);
	}

	if(IsEnable()) m_pBar1->SetIndex(m_nStartIndex);
	else m_pBar1->SetIndex(m_nStartIndex+2);

	m_pBar2->SetIndex(m_nStartIndex+1);
	m_pThumb->SetFourStatusButton(m_nStartIndex+3);

	m_pBar1->SetLocalPos( 0, 0 );
	m_pBar2->SetLocalPos( 0, 0 );

	SetLocalRect(m_pBar1->GetLocalRect());

	CRect rect( 0, 0, 0, 0 ); 
	m_pBar1->SetLocalClipRect(rect);

	rect.SetRect(0, 0, m_pBar1->GetWidth(), m_pBar1->GetHeight()); 
	m_pBar2->SetLocalClipRect(rect);

	m_pThumb->SetMouseMove(true);
	m_pThumb->SetPivot(m_ptThumbPivot);

	SetCurValue(m_fCurValue);

	CheckBound();

	return true ; 
}

const int CSlideBar::GetThumbWidth(void)
{
	return m_pThumb?(int)m_pThumb->GetWidth():0;
}

const int CSlideBar::GetThumbHeight(void)
{
	return m_pThumb?(int)m_pThumb->GetHeight():0;
}

void CSlideBar::SetThumbPivot(const CPoint &ptPivot)
{
	if(m_ptThumbPivot == ptPivot) return;
	m_ptThumbPivot = ptPivot;
	if(m_pThumb) m_pThumb->SetPivot(m_ptThumbPivot);
}

const std::string CSlideBar::GetFileName(void)
{
	return m_pBar1?m_pBar1->GetFileName():"";
}

const int CSlideBar::GetTotalNum(void)
{
	return m_pBar1?m_pBar1->GetTotalNum():0;
}

void CSlideBar::SetStartIndex(const int &nStartIndex)
{
	if(m_nStartIndex == nStartIndex) return;

	m_nStartIndex = nStartIndex;

	if(m_pBar1 == NULL) return;

	if(IsEnable()) m_pBar1->SetIndex(m_nStartIndex);
	else m_pBar1->SetIndex(m_nStartIndex+2);

	m_pBar2->SetIndex(m_nStartIndex+1);
	m_pThumb->SetFourStatusButton(m_nStartIndex+3);

	SetLocalRect(m_pBar1->GetLocalRect());

	SetCurValue(m_fCurValue);
	CheckBound();
}

const int &CSlideBar::GetStartIndex(void)
{
	return m_nStartIndex;
}

LRESULT CSlideBar::OnLocalChildNotify(CMessage *msg)
{
	CObjectBase *pObject = ((CObjectMessage *)msg)->GetObject();
	if(pObject == m_pThumb)
	{
		switch(msg->GetID())
		{
		case EM_O_DRAG_BEGIN:
		case EM_O_DRAGING:
		case EM_O_DRAG_END:
			return OnThumbBtnProc(msg);
		}
	}

	return CObjectBase::OnLocalChildNotify(msg);
}

void CSlideBar::CheckBound(void)
{
	if ( m_bHorizontality )
	{
		m_rcBound.left = m_fThumbLeftTopWidth;
		m_rcBound.right = GetWidth() - m_fThumbRightBottomWidth;
		m_rcBound.top = 0.0f;
		m_rcBound.bottom = 0.0f;
	}
	else
	{
		m_rcBound.left = 0.0f;
		m_rcBound.right = 0.0f;
		m_rcBound.top = m_fThumbLeftTopWidth;
		m_rcBound.bottom = GetHeight() - m_fThumbRightBottomWidth;
	}
}

void CSlideBar::SetRangeValue(const float &fMin, const float &fMax) 
{
	m_fMinValue = fMin ; 
	m_fMaxValue = fMax ; 

	if( m_fCurValue < m_fMinValue )
		m_fCurValue = m_fMinValue;

	if( m_fCurValue > m_fMaxValue )
		m_fCurValue = m_fMaxValue;

	SetCurValue(m_fCurValue);
}

void CSlideBar::SetCurValue(const float &fCur, const bool &bSetButtonPos/* = true*/)
{
	m_fCurValue = fCur ; 

	float fRate = 1.0f;
	if(m_fMaxValue != m_fMinValue) fRate = (m_fCurValue - m_fMinValue) / (m_fMaxValue - m_fMinValue) ; 
	CRect rect ( 0, 0, m_pBar1->GetWidth(), m_pBar1->GetHeight() ) ; 

	if ( m_bIncrease ) 
	{
		if ( m_bHorizontality )
		{
			rect.right = m_fThumbLeftTopWidth + (rect.right - (m_fThumbLeftTopWidth + m_fThumbRightBottomWidth)) * fRate;
			m_ptThumb = CPoint(rect.right, 0.0f);
		}
		else
		{
			rect.top = rect.bottom - m_fThumbRightBottomWidth - (rect.bottom - (m_fThumbLeftTopWidth + m_fThumbRightBottomWidth)) * fRate ; 
			rect.top = float((int)rect.top);
			m_ptThumb = CPoint(0.0f, rect.top);
		}
	}
	else
	{
		if ( m_bHorizontality ) 
		{
			rect.left = rect.right - m_fThumbRightBottomWidth - (rect.right - (m_fThumbLeftTopWidth + m_fThumbRightBottomWidth)) * fRate;
			rect.left = float((int)rect.left);
			m_ptThumb = CPoint(rect.left, 0.0f);
		}
		else
		{
			rect.bottom = m_fThumbLeftTopWidth + (rect.bottom - (m_fThumbLeftTopWidth + m_fThumbRightBottomWidth)) * fRate;
			m_ptThumb = CPoint(0.0f, rect.bottom);
		}
	}

	if(bSetButtonPos && m_pThumb) m_pThumb->SetLocalPos(m_ptThumb.x, m_ptThumb.y);
	if(m_pBar1) m_pBar1->SetLocalClipRect ( rect ) ; 
}

void CSlideBar::SetThumbLeftTopWidth(const float &fThumbWidth)
{
	m_fThumbLeftTopWidth = fThumbWidth;
	SetCurValue(m_fCurValue);
	CheckBound();
}

void CSlideBar::SetThumbRightBottomWidth(const float &fThumbWidth)
{
	m_fThumbRightBottomWidth = fThumbWidth;
	SetCurValue(m_fCurValue);
	CheckBound();
}

void CSlideBar::SetHorizontality(const bool &bHorizontality) 
{
	if(m_bHorizontality == bHorizontality) return;
	m_bHorizontality = bHorizontality;
	SetCurValue(m_fCurValue);
	CheckBound();
}

void CSlideBar::SetIncrease(const bool &bIncrease)
{
	if(m_bIncrease == bIncrease) return;
	m_bIncrease = bIncrease;
	SetCurValue(m_fCurValue);
	CheckBound();
}

void CSlideBar::SetEnable(const bool &bEnable)
{
	CObjectBase::SetEnable(bEnable);

	if(m_pBar1)
	{
		if(bEnable) m_pBar1->SetIndex(m_nStartIndex);
		else m_pBar1->SetIndex(m_nStartIndex+2);
	}
}

void CSlideBar::CheckThumb(const CPoint &ptPos)
{
	CPoint ptLocal = ptPos;

	if ( ptLocal.x < m_rcBound.left ) 
	{
		ptLocal.x = m_rcBound.left ; 
	}
	else if ( ptLocal.x > m_rcBound.right ) 
	{
		ptLocal.x = m_rcBound.right ; 
	}

	if ( ptLocal.y < m_rcBound.top ) 
	{
		ptLocal.y = m_rcBound.top ; 
	}
	else if ( ptLocal.y > m_rcBound.bottom )
	{
		ptLocal.y = m_rcBound.bottom ; 
	}

	if(m_pThumb) m_pThumb->SetLocalPos ( ptLocal ) ;
	float fRate = 0.0f;

	if ( m_bIncrease )
	{
		if(m_bHorizontality)
		{
			fRate = (ptLocal.x - m_rcBound.left) / (m_rcBound.right-m_rcBound.left);
		}
		else
		{
			fRate = (m_rcBound.bottom - ptLocal.y) / (m_rcBound.bottom-m_rcBound.top) ; 
		}
	}
	else
	{
		if(m_bHorizontality)
		{
			fRate = (m_rcBound.right - ptLocal.x) / (m_rcBound.right-m_rcBound.left);
		}
		else
		{
			fRate = (ptLocal.y-m_rcBound.top) / (m_rcBound.bottom-m_rcBound.top) ; 
		}
	}

	float fCurValue = m_fMinValue + (m_fMaxValue - m_fMinValue) * fRate ;
	if(fCurValue != m_fCurValue)
	{
		SetCurValue(fCurValue, false);

		CMO_CurValueChange msg(this, m_fCurValue);
		Dispatch(&msg);
	}
}

LRESULT CSlideBar::OnThumbBtnProc(CMessage *pMsg)
{
	CPoint ptPos = m_pThumb->GetLocalPos();
	CheckThumb(ptPos);
	return TRUE;
}

bool CSlideBar::OnDragBegin(const CPoint &ptPos)
{
	CObjectBase::OnDragBegin(ptPos);
	OnBarDrag(ptPos);
	return true;
}

bool CSlideBar::OnDraging(const CPoint &ptPos)
{
	CObjectBase::OnDraging(ptPos);
	OnBarDrag(ptPos);
	return true;
}

bool CSlideBar::OnDragEnd(const CPoint &ptPos)
{
	CObjectBase::OnDragEnd(ptPos);
	OnBarDrag(ptPos);
	return true;
}

const float &CSlideBar::OnBarDrag(const CPoint &ptPos)
{
	CPoint ptLocal = GetGlobalToLocalPos(ptPos);

	m_pThumb->SetLocalPos(ptLocal.x, ptLocal.y);
	CheckThumb(ptLocal);

	return m_fCurValue;
}

void CSlideBar::SetObjectProject(CObjectProject *pProject)
{
	__super::SetObjectProject(pProject);

	if(m_pBar1)
	{
		m_pBar1->SetTempEventDraw(m_bEventDraw);
		m_pBar2->SetTempEventDraw(m_bEventDraw);
		m_pThumb->SetTempEventDraw(m_bEventDraw);
	}
}

bool CSlideBar::EventDraw(CObjectMan *pObjectMan, NMBASE::GRAPHICGDI::CPage *pPage)
{
	CImage *pImage = m_pThumb->GetImage();
	if(pImage->IsSendEventDraw())
	{
		CRect &rtBar = m_pBar1->GetRect();
		pObjectMan->EventBackToFrontBitBlt(&rtBar);

		if(m_pBar1->IsShow() && m_pBar1->IsShowRef())	m_pBar1->CObjectBase::DrawLocal(pPage);
		else m_pBar1->ClearSendEventDraw();

		if(m_pBar2->IsShow() && m_pBar2->IsShowRef())	m_pBar2->CObjectBase::DrawLocal(pPage);
		else m_pBar2->ClearSendEventDraw();

		pObjectMan->EventBitBlt(&rtBar);

		CRect rtOldDraw = pImage->GetOldDrawRect();

		pImage->Draw(pPage);

		CRect &rtDraw = pImage->GetRect();

		if(rtOldDraw != rtDraw)
		{
			pObjectMan->EventBackToFrontBitBlt(&rtOldDraw);
			pObjectMan->EventBackToFrontBitBlt(&rtDraw);

			if(pImage->IsShow() && pImage->IsShowRef())	pImage->CObjectBase::DrawLocal(pPage);
			else pImage->ClearSendEventDraw();

			pObjectMan->EventBitBlt(&rtOldDraw);
			pObjectMan->EventBitBlt(&rtDraw);
		}
		else
		{
			pObjectMan->EventBackToFrontBitBlt(&rtDraw);

			if(pImage->IsShow() && pImage->IsShowRef())	pImage->CObjectBase::DrawLocal(pPage);
			else pImage->ClearSendEventDraw();

			pObjectMan->EventBitBlt(&rtDraw);
		}

		return true;
	}
	return false;
}
} //namespace LSY