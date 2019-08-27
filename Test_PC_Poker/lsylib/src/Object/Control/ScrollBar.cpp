#include "stdafx.h"
#include "ScrollBar.h"
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
CScrollBar::CScrollBar(void)
:	m_pUp(NULL),
	m_pDown(NULL),
	m_pThumb(NULL),
	m_ptThumb(0.0f,0.0f),
	m_fCurValue(0.0f),
	m_fMaxValue(0.0f),
	m_fMinValue(0.0f),
	m_rcBound(0.0f,0.0f,0.0f,0.0f),
	m_bIncrease(true),
	m_bHorizontality(false),
	m_nStartIndex(0),
	m_fLen(100.0f),
	m_fChangeValue(1.0f),
	m_fPageValue(10.0f)
{
	SetMessage(true);
	SetUseMouseWheel(true);
}

CScrollBar::~CScrollBar()
{
}

CObjectBase *CScrollBar::NewObject(void)
{
	return new CScrollBar;
}

CObjectBase *CScrollBar::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CScrollBar *pControl				= (CScrollBar *)pObject;
	pControl->m_ptThumb					= m_ptThumb;
	pControl->m_fCurValue				= m_fCurValue;
	pControl->m_fMaxValue				= m_fMaxValue;
	pControl->m_fMinValue				= m_fMinValue;
	pControl->m_rcBound					= m_rcBound;
	pControl->m_bIncrease				= m_bIncrease;
	pControl->m_bHorizontality			= m_bHorizontality;
	pControl->m_nStartIndex				= m_nStartIndex;

	if(m_pUp)
	{
		pControl->m_pUp	= (CButton *)m_pUp->Clone();
		pControl->AddLocalChild(pControl->m_pUp);
	}
	if(m_pDown)
	{
		pControl->m_pDown	= (CButton *)m_pDown->Clone();
		pControl->AddLocalChild(pControl->m_pDown);
	}
	if(m_pThumb)
	{
		pControl->m_pThumb	= (CButton *)m_pThumb->Clone();
		pControl->AddLocalChild(pControl->m_pThumb);
	}

	CheckBound();

	return pObject;
}

size_t CScrollBar::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(CPoint);		//m_ptThumb
	nRet += sizeof(float);		//m_fCurValue
	nRet += sizeof(float);		//m_fMaxValue
	nRet += sizeof(float);		//m_fMinValue
	nRet += sizeof(CRect);		//m_rcBound
	nRet += sizeof(bool);		//m_bIncrease
	nRet += sizeof(bool);		//m_bHorizontality
	nRet += sizeof(size_t);		//파일명 크기
	if(m_pThumb) nRet += m_pThumb->GetFileName().size();	//파일명
	nRet += sizeof(int);		//m_nStartIndex
	nRet += sizeof(float);		//m_fLen
	nRet += sizeof(float);		//m_fChangeValue
	nRet += sizeof(float);		//m_fPageValue
	return nRet;
}

size_t CScrollBar::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_ptThumb, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptThumb
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
		nRet += nSize;	//파일명
	}

	memcpy(pDest+nRet, &m_nStartIndex, sizeof(int));
	nRet += sizeof(int);		//m_nStartIndex
	memcpy(pDest+nRet, &m_fLen, sizeof(float));
	nRet += sizeof(float);		//m_fLen
	memcpy(pDest+nRet, &m_fChangeValue, sizeof(float));
	nRet += sizeof(float);		//m_fChangeValue
	memcpy(pDest+nRet, &m_fPageValue, sizeof(float));
	nRet += sizeof(float);		//m_fPageValue
	return nRet;
}

size_t CScrollBar::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_ptThumb, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptThumb
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
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//파일명 크기

	std::string strFileName;

	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//파일명
	}

	memcpy(&m_nStartIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nStartIndex

	memcpy(&m_fLen, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fLen
	memcpy(&m_fChangeValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fChangeValue
	memcpy(&m_fPageValue, pDest+nRet, sizeof(float));
	nRet += sizeof(float);		//m_fPageValue

	Create(strFileName.c_str());

	return nRet;
}

bool CScrollBar::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetDoubleAttribute("cur",  m_fCurValue);
	xmlInheritance.SetDoubleAttribute("max",  m_fMaxValue);
	xmlInheritance.SetDoubleAttribute("min",  m_fMinValue);
	xmlInheritance.SetAttribute("Increase",  m_bIncrease?1:0);
	xmlInheritance.SetAttribute("Horizontality",  m_bHorizontality?1:0);
	xmlInheritance.SetAttribute("path",  m_pThumb?m_pThumb->GetFileName().c_str():"");
	xmlInheritance.SetAttribute("index",  m_nStartIndex);
	xmlInheritance.SetDoubleAttribute("len",  m_fLen);
	xmlInheritance.SetDoubleAttribute("value",  m_fChangeValue);
	xmlInheritance.SetDoubleAttribute("page",  m_fPageValue);

	pXmlObject->InsertEndChild(xmlInheritance);
	return true;
}

bool CScrollBar::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
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
	GetAttribute(pXmlInheritance, "index", m_nStartIndex);
	GetAttribute(pXmlInheritance, "len", m_fLen);
	GetAttribute(pXmlInheritance, "value", m_fChangeValue);
	GetAttribute(pXmlInheritance, "page", m_fPageValue);

	Create(strTemp.c_str());

	return true;
}

void CScrollBar::SetEnable(const bool &bEnable)
{
	if(m_bEnable == bEnable) return;
	if(m_pUp)
	{
		m_pUp->SetEnable(bEnable);
		m_pDown->SetEnable(bEnable);
		m_pThumb->SetEnable(bEnable);
	}

	__super::SetEnable(bEnable);
}

bool CScrollBar::Create (LPCTSTR lpPath)  
{
	bool bAdd = false;

	if(m_pUp == NULL)
	{
		m_pUp		= new CButton; 
		m_pDown		= new CButton; 
		m_pThumb	= new CButton;
		bAdd = true;
	}

	if(m_pUp->Create(lpPath) == false)
	{
		delete m_pUp;
		m_pUp = NULL;
		delete m_pDown;
		m_pDown = NULL;
		delete m_pThumb;
		m_pThumb = NULL;
		return false ;
	}

	if(m_pDown->Create(lpPath) == false) 
	{
		delete m_pUp;
		m_pUp = NULL;
		delete m_pDown;
		m_pDown = NULL;
		delete m_pThumb;
		m_pThumb = NULL;
		return false ;
	}

	if(m_pThumb->Create(lpPath) == false) 
	{
		delete m_pUp;
		m_pUp = NULL;
		delete m_pDown;
		m_pDown = NULL;
		delete m_pThumb;
		m_pThumb = NULL;
		return false ;
	}

	if(bAdd)
	{
		AddLocalChild(m_pUp);
		AddLocalChild(m_pDown);
		AddLocalChild(m_pThumb);

		m_pThumb->SetMouseMove(true);
	}

	m_pUp->SetTempEventDraw(m_bEventDraw);
	m_pDown->SetTempEventDraw(m_bEventDraw);
	m_pThumb->SetTempEventDraw(m_bEventDraw);

	m_pUp->SetFourStatusButton(m_nStartIndex);
	m_pDown->SetFourStatusButton(m_nStartIndex+8);
	m_pThumb->SetFourStatusButton(m_nStartIndex+4);

	CheckBound();

	return true; 
}

const std::string CScrollBar::GetFileName(void)
{
	return m_pUp?m_pUp->GetFileName():"";
}

const int CScrollBar::GetTotalNum(void)
{
	return m_pUp?m_pUp->GetTotalNum():0;
}

bool CScrollBar::SetLength(const float &fLen)
{
	if(m_fLen == fLen) return true;
	if(fLen < 0.0f) return true;
	m_fLen = fLen;
	CheckBound();
	return true;
}

bool CScrollBar::SetStartIndex(const int &nStartIndex)
{
	if(m_nStartIndex == nStartIndex) return true;
	if(m_nStartIndex < 0) return false;
	m_nStartIndex = nStartIndex;

	m_pUp->SetFourStatusButton(m_nStartIndex);
	m_pDown->SetFourStatusButton(m_nStartIndex+8);
	m_pThumb->SetFourStatusButton(m_nStartIndex+4);

	CheckBound();
	return true;
}
const int &CScrollBar::GetStartIndex(void)
{
	return m_nStartIndex;
}

LRESULT CScrollBar::OnLocalChildNotify(CMessage *msg)
{
	CObjectBase *pObject = ((CObjectMessage *)msg)->GetObject();
	if(pObject == m_pUp)
	{
		switch(msg->GetID())
		{
		case EM_O_MOUSELCLICK:
			return OnClickUpBtn(msg);
		case EM_O_FOCUSIN:
			return OnFocusInScroll(msg);
		}
	}
	else if(pObject == m_pDown)
	{
		switch(msg->GetID())
		{
		case EM_O_MOUSELCLICK:
			return OnClickDownBtn(msg);
		case EM_O_FOCUSIN:
			return OnFocusInScroll(msg);
		}
	}
	else if(pObject == m_pThumb)
	{
		switch(msg->GetID())
		{
		case EM_O_DRAG_BEGIN:
			return OnThumbBtnProc(msg);
		case EM_O_DRAGING:
			return OnThumbBtnProc(msg);
		case EM_O_DRAG_END:
			return OnThumbBtnProc(msg);
		case EM_O_FOCUSIN:
			return OnFocusInScroll(msg);
		}
	}
	return CObjectBase::OnLocalChildNotify(msg);
}

void CScrollBar::CheckBound(void)
{
	if ( m_bHorizontality )
	{
		float fMinLen = m_pUp->GetWidth() + m_pDown->GetWidth() + m_pThumb->GetWidth();
		if(fMinLen > m_fLen) m_fLen = fMinLen;

		SetLocalRect(CRect(0,0,m_fLen,m_pUp->GetHeight()));

		m_rcBound.left = m_pDown->GetWidth();
		m_rcBound.right = m_fLen - m_pUp->GetWidth() - m_pThumb->GetWidth();
		m_rcBound.top = 0.0f;
		m_rcBound.bottom = 0.0f;

		m_pUp->SetLocalPos(m_fLen - m_pUp->GetWidth(), 0);
		m_pDown->SetLocalPos(0, 0);
	}
	else
	{
		float fMinLen = m_pUp->GetHeight() + m_pDown->GetHeight() + m_pThumb->GetHeight();
		if(fMinLen > m_fLen) m_fLen = fMinLen;

		SetLocalRect(CRect(0,0,m_pUp->GetWidth(),m_fLen));

		m_rcBound.left = 0.0f;
		m_rcBound.right = 0.0f;
		m_rcBound.top = m_pUp->GetHeight();
		m_rcBound.bottom = m_fLen - m_pDown->GetHeight() - m_pThumb->GetHeight();
		m_pUp->SetLocalPos(0, 0);
		m_pDown->SetLocalPos(0, m_fLen - m_pDown->GetHeight());
	}

	SetCurValue(m_fCurValue);
}

void CScrollBar::SetRangeValue(const float &fMin, const float &fMax) 
{
	m_fMinValue = fMin ; 
	m_fMaxValue = (fMax>fMin)?fMax:fMin; 

	if( m_fCurValue < m_fMinValue )
		m_fCurValue = m_fMinValue;

	if( m_fCurValue > m_fMaxValue )
		m_fCurValue = m_fMaxValue;

	SetCurValue(m_fCurValue);
}

void CScrollBar::SetCurValue(const float &fCur, const bool &bSetButtonPos/* = true*/)
{
	m_fCurValue = fCur;
	if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
	if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;

	if(bSetButtonPos && m_pThumb)
	{
		float fRate = 0.0f;
		
		if(m_fMinValue != m_fMaxValue) fRate = (m_fCurValue - m_fMinValue) / (m_fMaxValue - m_fMinValue);

		if ( m_bIncrease ) 
		{
			if ( m_bHorizontality )
			{
				m_ptThumb = CPoint(m_rcBound.left + m_rcBound.GetWidth() * fRate, m_rcBound.top);
			}
			else
			{
				m_ptThumb = CPoint(m_rcBound.left, (float)((int)(m_rcBound.bottom - m_rcBound.GetHeight() * fRate)));
			}
		}
		else
		{
			if ( m_bHorizontality ) 
			{
				m_ptThumb = CPoint((float)((int)(m_rcBound.right - m_rcBound.GetWidth() * fRate)), m_rcBound.top);
			}
			else
			{
				m_ptThumb = CPoint(m_rcBound.left, m_rcBound.top + m_rcBound.GetHeight() * fRate);
			}
		}

		m_pThumb->SetLocalPos(m_ptThumb.x, m_ptThumb.y);
	}
}

void CScrollBar::SetHorizontality(const bool &bHorizontality) 
{
	if(m_bHorizontality == bHorizontality) return;
	m_bHorizontality = bHorizontality;
	SetCurValue(m_fCurValue);
	CheckBound();
}

void CScrollBar::SetIncrease(const bool &bIncrease)
{
	if(m_bIncrease == bIncrease) return;
	m_bIncrease = bIncrease;
	SetCurValue(m_fCurValue);
	CheckBound();
}

void CScrollBar::CheckThumb(const CPoint &ptPos)
{
	CPoint ptLocal = ptPos;

	if ( ptLocal.x < m_rcBound.left ) ptLocal.x = m_rcBound.left ; 
	else if ( ptLocal.x > m_rcBound.right ) ptLocal.x = m_rcBound.right ; 

	if ( ptLocal.y < m_rcBound.top ) ptLocal.y = m_rcBound.top ; 
	else if ( ptLocal.y > m_rcBound.bottom ) ptLocal.y = m_rcBound.bottom ; 

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

LRESULT CScrollBar::OnThumbBtnProc(CMessage *pMsg)
{
	CPoint ptPos = m_pThumb->GetLocalPos();
	CheckThumb(ptPos);
	return TRUE;
}

LRESULT CScrollBar::OnClickUpBtn(CMessage *pMsg)
{
	float fOldValue = m_fCurValue;
	if(m_bIncrease)
	{
		m_fCurValue += m_fChangeValue;
		if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;
	}
	else 
	{
		m_fCurValue -= m_fChangeValue;
		if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
	}

	if(fOldValue != m_fCurValue)
	{
		CMO_ClickUp msg1(this);
		if(Dispatch(&msg1) == FALSE)
		{
			CMO_CurValueChange msg(this, m_fCurValue);
			Dispatch(&msg);

			SetCurValue(m_fCurValue, true);
		}
	}
	return TRUE;
}

LRESULT CScrollBar::OnClickDownBtn(CMessage *pMsg)
{
	float fOldValue = m_fCurValue;

	if(m_bIncrease)
	{
		m_fCurValue -= m_fChangeValue;
		if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
	}
	else 
	{
		m_fCurValue += m_fChangeValue;
		if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;
	}

	if(fOldValue != m_fCurValue)
	{
		CMO_ClickDown msg1(this);
		if(Dispatch(&msg1) == FALSE)
		{
			CMO_CurValueChange msg(this, m_fCurValue);
			Dispatch(&msg);

			SetCurValue(m_fCurValue, true);
		}
	}
	return TRUE;
}

void CScrollBar::SetChangeValue(const float &fChangeValue)
{
	ASSERT(fChangeValue > 0.0f); 
	m_fChangeValue = fChangeValue;
}

void CScrollBar::SetPageValue(const float &fPageValue)
{
	ASSERT(fPageValue > 0.0f); 
	m_fPageValue = fPageValue;
}

void CScrollBar::OnMouseLClick(const CPoint &ptPos)
{
	if(m_pThumb)
	{
		CPoint ptLocalMouse = GetGlobalToLocalPos(ptPos);
		CPoint ptLocalThumb = m_pThumb->GetLocalPos() - m_ptPivot;

		float fOldValue = m_fCurValue;
		bool bUp = true;

		if(m_bHorizontality)
		{
			if(m_bIncrease)
			{
				if(ptLocalMouse.x < ptLocalThumb.x)
				{
					m_fCurValue -= m_fPageValue;
					if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
					bUp = false;
				}
				else
				{
					m_fCurValue += m_fPageValue;
					if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;
				}
			}
			else
			{
				if(ptLocalMouse.x < ptLocalThumb.x)
				{
					m_fCurValue += m_fPageValue;
					if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;
					bUp = false;
				}
				else
				{
					m_fCurValue -= m_fPageValue;
					if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
				}
			}
		}
		else
		{
			if(m_bIncrease)
			{
				if(ptLocalMouse.y < ptLocalThumb.y)
				{
					m_fCurValue += m_fPageValue;
					if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;
				}
				else
				{
					m_fCurValue -= m_fPageValue;
					if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
					bUp = false;
				}
			}
			else
			{
				if(ptLocalMouse.y < ptLocalThumb.y)
				{
					m_fCurValue -= m_fPageValue;
					if(m_fCurValue < m_fMinValue) m_fCurValue = m_fMinValue;
				}
				else
				{
					m_fCurValue += m_fPageValue;
					if(m_fCurValue > m_fMaxValue) m_fCurValue = m_fMaxValue;
					bUp = false;
				}
			}
		}

		if(fOldValue != m_fCurValue)
		{
			BOOL bRet = FALSE;
			if(bUp)
			{
				CMO_ClickPageUp msg(this);
				bRet = Dispatch(&msg);
			}
			else
			{
				CMO_ClickPageDown msg(this);
				bRet = Dispatch(&msg);
			}

			if(bRet == FALSE)
			{
				CMO_CurValueChange msg(this, m_fCurValue);
				Dispatch(&msg);

				SetCurValue(m_fCurValue, true);
			}
		}
	}

	CObjectBase::OnMouseLClick(ptPos);
}

bool CScrollBar::OnMouseWheel(const int &nKey, const short &zDelta)
{
	if( zDelta > 0 )
		OnClickUpBtn(NULL); 
	else
		OnClickDownBtn(NULL);

	return CObjectBase::OnMouseWheel(nKey,zDelta);
}

LRESULT CScrollBar::OnFocusInScroll(CMessage *pMsg)
{
	return (LRESULT)this;
}

void CScrollBar::SetObjectProject(CObjectProject *pProject)
{
	__super::SetObjectProject(pProject);

	//if(m_pUp)
	//{
	//	m_pUp->SetTempEventDraw(m_bEventDraw);
	//	m_pDown->SetTempEventDraw(m_bEventDraw);
	//	m_pThumb->SetTempEventDraw(m_bEventDraw);
	//}
}
} //namespace LSY