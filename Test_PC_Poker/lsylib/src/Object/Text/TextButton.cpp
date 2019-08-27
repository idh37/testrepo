#include "stdafx.h"
#include "TextButton.h"
#include "../../tinyxml/tinyxml.h"
#include "../Base/DumyImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CTextButton::CTextButton(void)
:	m_ptDownGap(CPointZero),
	m_nStatus(EBS_NORMAL)
{
	m_listStatusColor.resize(8);
	m_listStatusColor[EBS_NORMAL]	= RGB(255,255,255);
	m_listStatusColor[EBS_OVER]		= RGB(255,255,255);
	m_listStatusColor[EBS_DOWN]		= RGB(255,255,255);
	m_listStatusColor[EBS_DISABLE]	= RGB(255,255,255);
	m_listStatusColor[EBS_SNORMAL]	= RGB(255,255,255);
	m_listStatusColor[EBS_SOVER]	= RGB(255,255,255);
	m_listStatusColor[EBS_SDOWN]	= RGB(255,255,255);
	m_listStatusColor[EBS_SDISABLE] = RGB(255,255,255);
	SetMessage(true);
}

CTextButton::~CTextButton()
{
}

CObjectBase *CTextButton::NewObject(void)
{
	return new CTextButton;
}

CObjectBase *CTextButton::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CTextButton *pButton			= (CTextButton *)pObject;
	pButton->m_listStatusColor		= m_listStatusColor;
	pButton->m_ptDownGap			= m_ptDownGap;
	pButton->SetStatusColor();
	pButton->SetRedraw();
	return pObject;
}

bool CTextButton::Create(int nWidth, int nHeight)
{
	bool bRet = __super::Create(nWidth, nHeight);
	SetStatusColor();
	return bRet;
}

void CTextButton::SetEnable(const bool &bEnable)
{
	__super::SetEnable(bEnable);

	if(!m_bEnable) SetButtonStatus(EBS_DISABLE);
	else if(m_bMouseOver) SetButtonStatus(EBS_OVER);
	else SetButtonStatus(EBS_NORMAL);
}

void CTextButton::OnMouseOver(void)
{
	__super::OnMouseOver();
	if(m_bEnable) SetButtonStatus(EBS_OVER);
}

void CTextButton::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	if(m_bEnable) SetButtonStatus(EBS_NORMAL);
}

bool CTextButton::OnLButtonDown(const CPoint &ptPos)
{
	if(m_bEnable) SetButtonStatus(EBS_DOWN);
	return __super::OnLButtonDown(ptPos);
}

bool CTextButton::OnLButtonDbClick(const CPoint &ptPos)
{
	if(m_bEnable) SetButtonStatus(EBS_DOWN);
	return __super::OnLButtonDbClick(ptPos);
}

bool CTextButton::OnLButtonUP(const CPoint &ptPos)
{
	if(m_bEnable)
	{
		if(m_bMouseOver) SetButtonStatus(EBS_OVER);
		else SetButtonStatus(EBS_NORMAL);
	}
	return __super::OnLButtonUP(ptPos);
}

void CTextButton::SetStatusColor(void)
{
	if(m_pBack)
	{
		if(m_nStatus == EBS_DOWN) m_pBack->SetLocalPos(m_ptDownGap);
		else m_pBack->SetLocalPos(CPointZero);
	}

	if(m_cFontInfo.m_clrColor == m_listStatusColor[m_nStatus]) return;
	m_cFontInfo.m_clrColor = m_listStatusColor[m_nStatus];
	SetRedraw();
}

void CTextButton::SetStatusColor(const ENUM_BUTTON_STATUS &nStatus, const COLORREF &clrColor)
{
	if(nStatus >= EBS_MAX) return;
	if(m_listStatusColor[nStatus] == clrColor) return;
	m_listStatusColor[nStatus] = clrColor;
	if(GetButtonStatus() == nStatus) SetStatusColor();
}

const COLORREF CTextButton::GetStatusColor(const ENUM_BUTTON_STATUS &nStatus)
{
	if(nStatus >= EBS_MAX) return RGB(255,255,255);
	return m_listStatusColor[nStatus];
}

void CTextButton::SetDownGap(const CPoint &ptPos)
{
	if(m_ptDownGap == ptPos) return;
	m_ptDownGap = ptPos;
	SetStatusColor();
}

size_t CTextButton::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(COLORREF);					//m_clrNormal
	nRet += sizeof(COLORREF);					//m_clrOver
	nRet += sizeof(COLORREF);					//m_clrDown
	nRet += sizeof(COLORREF);					//m_clrDisable
	nRet += sizeof(COLORREF);					//m_clrSNormal
	nRet += sizeof(COLORREF);					//m_clrSOver
	nRet += sizeof(COLORREF);					//m_clrSDown
	nRet += sizeof(COLORREF);					//m_clrSDisable
	nRet += sizeof(CPoint);						//m_ptDownGap
	return nRet;
}

size_t CTextButton::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_listStatusColor[EBS_NORMAL], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrNormal
	memcpy(pDest+nRet, &m_listStatusColor[EBS_OVER], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrOver
	memcpy(pDest+nRet, &m_listStatusColor[EBS_DOWN], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrDown
	memcpy(pDest+nRet, &m_listStatusColor[EBS_DISABLE], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrDisable
	memcpy(pDest+nRet, &m_listStatusColor[EBS_SNORMAL], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSNormal
	memcpy(pDest+nRet, &m_listStatusColor[EBS_SOVER], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSOver
	memcpy(pDest+nRet, &m_listStatusColor[EBS_SDOWN], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSDown
	memcpy(pDest+nRet, &m_listStatusColor[EBS_SDISABLE], sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSDisable
	memcpy(pDest+nRet, &m_ptDownGap, sizeof(CPoint));
	nRet += sizeof(CPoint);						//m_ptDownGap
	return nRet;
}

size_t CTextButton::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_listStatusColor[EBS_NORMAL], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrNormal
	memcpy(&m_listStatusColor[EBS_OVER], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrOver
	memcpy(&m_listStatusColor[EBS_DOWN], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrDown
	memcpy(&m_listStatusColor[EBS_DISABLE], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrDisable
	memcpy(&m_listStatusColor[EBS_SNORMAL], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSNormal
	memcpy(&m_listStatusColor[EBS_SOVER], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSOver
	memcpy(&m_listStatusColor[EBS_SDOWN], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSDown
	memcpy(&m_listStatusColor[EBS_SDISABLE], pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSDisable
	memcpy(&m_ptDownGap, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);						//m_ptDownGap

	SetStatusColor();

	return nRet;
}

bool CTextButton::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlButton("button");

	CString strTemp;
	strTemp.Format("%06X", m_listStatusColor[EBS_NORMAL]);
	xmlButton.SetAttribute("normal", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_OVER]);
	xmlButton.SetAttribute("over", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_DOWN]);
	xmlButton.SetAttribute("down", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_DISABLE]);
	xmlButton.SetAttribute("disable", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_SNORMAL]);
	xmlButton.SetAttribute("snormal", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_SOVER]);
	xmlButton.SetAttribute("sover", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_SDOWN]);
	xmlButton.SetAttribute("sdown", strTemp.GetString());
	strTemp.Format("%06X", m_listStatusColor[EBS_SDISABLE]);
	xmlButton.SetAttribute("sdisable", strTemp.GetString());

	xmlButton.SetDoubleAttribute("GapX", m_ptDownGap.x);
	xmlButton.SetDoubleAttribute("GapY", m_ptDownGap.y);

	pXmlObject->InsertEndChild(xmlButton);

	return true;
}

bool CTextButton::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *xmlButton = pXmlObject->FirstChildElement("button");

	GetAttributeColor(xmlButton, "normal", m_listStatusColor[EBS_NORMAL]);
	GetAttributeColor(xmlButton, "over", m_listStatusColor[EBS_OVER]);
	GetAttributeColor(xmlButton, "down", m_listStatusColor[EBS_DOWN]);
	GetAttributeColor(xmlButton, "disable", m_listStatusColor[EBS_DISABLE]);

	if(xmlButton->Attribute("snormal"))
	{
		GetAttributeColor(xmlButton, "snormal", m_listStatusColor[EBS_SNORMAL]);
		GetAttributeColor(xmlButton, "sover", m_listStatusColor[EBS_SOVER]);
		GetAttributeColor(xmlButton, "sdown", m_listStatusColor[EBS_SDOWN]);
		GetAttributeColor(xmlButton, "sdisable", m_listStatusColor[EBS_SDISABLE]);
	}

	GetAttribute(xmlButton, "GapX", m_ptDownGap.x);
	GetAttribute(xmlButton, "GapY", m_ptDownGap.y);

	SetStatusColor();

	return true;
}

void CTextButton::SetButtonStatus(const ENUM_BUTTON_STATUS &nStatus)
{
	if(m_nStatus == nStatus) return;
	m_nStatus = nStatus;
	SetStatusColor();
}
} //namespace LSY