#include "stdafx.h"
#include "CheckBox.h"
#include "../Text/Text.h"
#include "../../tinyxml/tinyxml.h"
#include "../../Utility/MyMime64.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CCheckBox::CCheckBox(void)
:	m_pText(NULL),
	m_szText(0, 0),
	m_strFontName("굴림"),
	m_nFontSize(11),
	m_bFontBold(false),
	m_clrTextColor(RGB(255,255,255)),
	m_strCheckFontName("굴림"),
	m_nCheckFontSize(11),
	m_bCheckFontBold(false),
	m_clrCheckTextColor(RGB(128,128,128)),
	m_strText("")
{
}

CCheckBox::~CCheckBox()
{
}

CObjectBase *CCheckBox::NewObject(void)
{
	return new CCheckBox;
}

CObjectBase *CCheckBox::Copy(CObjectBase *pObject)
{
	CCheckButton::Copy(pObject);

	CCheckBox *pButton = (CCheckBox *)pObject;

	if(m_pText)
	{
		pButton->m_pText = (CText *)m_pText->Clone(true);
		pButton->AddLocalChild(pButton->m_pText);
	}

	pButton->m_strFontName		= m_strFontName;
	pButton->m_nFontSize		= m_nFontSize;
	pButton->m_bFontBold		= m_bFontBold;
	pButton->m_clrTextColor		= m_clrTextColor;
	pButton->m_strCheckFontName	= m_strCheckFontName;
	pButton->m_nCheckFontSize	= m_nCheckFontSize;
	pButton->m_bCheckFontBold	= m_bCheckFontBold;
	pButton->m_clrCheckTextColor= m_clrCheckTextColor;
	pButton->m_strText			= m_strText;

	return pObject;
}

size_t CCheckBox::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);			//m_pText 의 유무
	if(m_pText) nRet += m_pText->GetClipSize(nVersion);
	nRet += sizeof(size_t);			//폰트명 크기
	nRet += m_strFontName.size();	//폰트명
	nRet += sizeof(int);			//m_nFontSize
	nRet += sizeof(bool);			//m_bBold
	nRet += sizeof(COLORREF);		//m_clrTextColor
	nRet += sizeof(size_t);			//체크폰트명 크기
	nRet += m_strCheckFontName.size(); //체크폰트명
	nRet += sizeof(int);			//m_nCheckFontSize
	nRet += sizeof(bool);			//m_bCheckFontBold
	nRet += sizeof(COLORREF);		//m_clrCheckFontColor
	nRet += sizeof(size_t);			//텍스트 크기
	nRet += m_strText.size();		//텍스트
	return nRet;
}

size_t CCheckBox::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	bool bUse = m_pText?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);			//m_pText 의 유무
	if(m_pText)
		nRet += m_pText->SaveClipSize(nVersion, pDest+nRet);
	size_t nSize = m_strFontName.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);			//폰트명 크기
	if(nSize)
	{
		memcpy(pDest+nRet, m_strFontName.c_str(), m_strFontName.size());
		nRet += m_strFontName.size();	//폰트명
	}
	memcpy(pDest+nRet, &m_nFontSize, sizeof(int));
	nRet += sizeof(int);			//m_nFontSize
	memcpy(pDest+nRet, &m_bFontBold, sizeof(bool));
	nRet += sizeof(bool);			//m_bBold
	memcpy(pDest+nRet, &m_clrTextColor, sizeof(COLORREF));
	nRet += sizeof(COLORREF);		//m_clrTextColor
	nSize = m_strCheckFontName.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);			//체크폰트명 크기
	if(nSize)
	{
		memcpy(pDest+nRet, m_strCheckFontName.c_str(), m_strCheckFontName.size());
		nRet += m_strCheckFontName.size(); //체크폰트명
	}
	memcpy(pDest+nRet, &m_nCheckFontSize, sizeof(int));
	nRet += sizeof(int);			//m_nCheckFontSize
	memcpy(pDest+nRet, &m_bCheckFontBold, sizeof(bool));
	nRet += sizeof(bool);			//m_bCheckFontBold
	memcpy(pDest+nRet, &m_clrCheckTextColor, sizeof(COLORREF));
	nRet += sizeof(COLORREF);		//m_clrCheckFontColor
	nSize = m_strText.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);			//텍스트 크기
	if(nSize)
	{
		memcpy(pDest+nRet, m_strText.c_str(), m_strText.size());
		nRet += m_strText.size();		//텍스트
	}
	return nRet;
}

size_t CCheckBox::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_pText 의 유무
	if(bUse)
	{
		bool bAdd = false;
		if(m_pText == NULL)
		{
			m_pText = new CText;
			bAdd = true;
		}
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += m_pText->LoadClipSize(nVersion, pDest+nRet, pProject, false);

		CheckTextStatus();

		if(bAdd) AddLocalChild(m_pText);

		RepositionTextArea();
	}
	size_t nSize = 0;;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);			//폰트명 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strFontName);
		nRet += nSize;	//폰트명
	}
	memcpy(&m_nFontSize, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nFontSize
	memcpy(&m_bFontBold, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_bBold
	memcpy(&m_clrTextColor, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);		//m_clrTextColor
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);			//체크폰트명 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strCheckFontName);
		nRet += nSize; //체크폰트명
	}
	memcpy(&m_nCheckFontSize, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nCheckFontSize
	memcpy(&m_bCheckFontBold, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_bCheckFontBold
	memcpy(&m_clrCheckTextColor, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);		//m_clrCheckFontColor
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);			//텍스트 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strText);
		nRet += nSize;		//텍스트
	}
	return nRet;
}

bool CCheckBox::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	if(m_pText)
	{
		TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
		m_pText->SaveXml(pXmlAttach);
	}

	CString strTemp;

	CMyMime64 myMime;
	myMime.SetString(m_strFontName,FALSE,TRUE);

	TiXmlElement xmlFont("font");
	xmlFont.SetAttribute("name", myMime.GetString());
	xmlFont.SetAttribute("size", m_nFontSize);
	xmlFont.SetAttribute("bold", m_bFontBold?1:0);
	strTemp.Format("%06X", m_clrTextColor);
	xmlFont.SetAttribute("color", strTemp.GetString());
	pXmlObject->InsertEndChild(xmlFont);

	myMime.SetString(m_strCheckFontName,FALSE,TRUE);

	TiXmlElement xmlCheckFont("checkfont");
	xmlCheckFont.SetAttribute("name", myMime.GetString());
	xmlCheckFont.SetAttribute("size", m_nCheckFontSize);
	xmlCheckFont.SetAttribute("bold", m_bCheckFontBold?1:0);
	strTemp.Format("%06X", m_clrCheckTextColor);
	xmlCheckFont.SetAttribute("color", strTemp.GetString());
	pXmlObject->InsertEndChild(xmlCheckFont);

	myMime.SetString(m_strText,FALSE,TRUE);

	TiXmlElement *pXmlInheritance = (TiXmlElement *)pXmlObject->FirstChild("Inheritance");
	pXmlInheritance->SetAttribute("text",  myMime.GetString());

	return true;
}

bool CCheckBox::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
	if(pXmlAttach)
	{
		CObjectBase *pChildObject = NULL;
		TiXmlElement *pXmlChildObject = pXmlAttach->FirstChildElement("Object");
		std::string strType = "";
		while(pXmlChildObject)
		{
			strType = pXmlChildObject->Attribute("Type");
			pChildObject = NULL;
			switch(GetStringToObjectType(strType))
			{
			case EOT_TEXTBUTTON:
				break;
			case EOT_TEXT:
				{
					pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);

					if(m_pText)
					{
						delete m_pText;
						m_pText = NULL;
					}

					m_pText = (CText *)pChildObject;

					CheckTextStatus();
					AddLocalChild(m_pText);
					RepositionTextArea();

					pChildObject->OnCreated();
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
	}

	TiXmlElement *pXmlFont = pXmlObject->FirstChildElement("font");
	GetAttributeMime64(pXmlFont, "name", m_strFontName);
	GetAttribute(pXmlFont, "size", m_nFontSize);
	GetAttribute(pXmlFont, "bold", m_bFontBold);
	GetAttributeColor(pXmlFont, "color", m_clrTextColor);

	TiXmlElement *pXmlCheckFont = pXmlObject->FirstChildElement("checkfont");
	GetAttributeMime64(pXmlCheckFont, "name", m_strCheckFontName);
	GetAttribute(pXmlCheckFont, "size", m_nCheckFontSize);
	GetAttribute(pXmlCheckFont, "bold", m_bCheckFontBold);
	GetAttributeColor(pXmlCheckFont, "color", m_clrCheckTextColor);

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttributeMime64(pXmlInheritance, "text", m_strText);

	return true;
}

bool CCheckBox::Create(LPCTSTR lpPath)
{
	bool bAdd = false;
	if(m_pImage == NULL) bAdd = true;
	
	if(CCheckButton::Create(lpPath) == false) return false;

	if(bAdd && m_pText) AddLocalChild(m_pText);

	RepositionTextArea();
	return true;
}

bool CCheckBox::CreateTextArea(int nWidth, int nHeight)
{
	if(m_szText.cx == nWidth && m_szText.cy == nHeight) return true;
	m_szText.SetSize(nWidth, nHeight);

	if(m_pText)
	{
		SubChild(m_pText);
		delete m_pText;
		m_pText = NULL;
	}

	bool bAdd = false;

	if(m_pText == NULL)
	{
		m_pText = new CText;
		bAdd = true;
	}

	if(m_pText->Create(nWidth, nHeight) == false)
	{
		delete m_pText;
		m_pText = NULL;
		return false;
	}

	m_pText->SetFormat(DT_VCENTER | DT_LEFT | DT_SINGLELINE);

	CheckTextStatus();

	if(bAdd)
	{
		AddLocalChild(m_pText);
	}

	RepositionTextArea();
	return true;
}

void CCheckBox::CheckTextStatus(void)
{
	if(m_pText == NULL) return;
	if(!IsCheck())
	{
		m_pText->SetFont(m_strFontName, m_nFontSize, m_bFontBold);
		m_pText->SetTextColor(m_clrTextColor);
	}
	else
	{
		m_pText->SetFont(m_strCheckFontName, m_nCheckFontSize, m_bCheckFontBold);
		m_pText->SetTextColor(m_clrCheckTextColor);
	}
}

bool CCheckBox::SetCheck(const bool &bCheck, const bool &bSendMsg/* = false*/)
{
	if(CCheckButton::SetCheck(bCheck, bSendMsg))
	{
		CheckTextStatus();
		return true;
	}

	return false;
}

void CCheckBox::RepositionTextArea(void)
{
	if(m_pText)
	{
		if(m_pImage)
		{
			CRect rtRect;
			rtRect.right = m_pImage->GetWidth ( ) + m_pText->GetWidth ( ) + 5.0f;

			CPoint ptLocal;

			if(m_pImage->GetHeight() > m_pText->GetHeight())
			{
				ptLocal.x = m_pImage->GetWidth() + 5.0f;
				ptLocal.y = (m_pImage->GetHeight() - m_pText->GetHeight())/2.0f;

				m_pText->SetLocalPos(ptLocal);
				m_pImage->SetLocalPos(CPointZero);

				rtRect.bottom = m_pImage->GetHeight();
			}
			else
			{
				ptLocal.x = m_pImage->GetWidth ( ) + 5.0f;
				ptLocal.y = 0;
				m_pText->SetLocalPos(ptLocal);

				ptLocal.x = 0;
				ptLocal.y = (m_pText->GetHeight() - m_pImage->GetHeight())/2.0f;
				m_pImage->SetLocalPos(ptLocal);
				rtRect.bottom = m_pText->GetHeight();
			}

			SetLocalRect(rtRect);
		}
		else
		{
			m_pText->SetLocalPos(CPointZero);
			SetLocalRect(m_pText->GetLocalRect());
		}
	}
	else if(m_pImage)
	{
		m_pImage->SetLocalPos(CPointZero);
		SetLocalRect(m_pImage->GetLocalRect());
	}
}

void CCheckBox::SetStatusIndex(void)
{
	if(m_pImage)
	{
		m_pImage->SetIndex(m_listStatusIndex[m_nStatus]);
		if(m_pText == NULL) SetLocalRect(m_pImage->GetLocalRect());
	}
}

void CCheckBox::SetText(const std::string &strText)
{
	if(m_strText == strText) return;
	if(m_pText) m_pText->SetText(strText);
	m_strText = strText;
}

void CCheckBox::SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold)
{
	if(m_strFontName == strFontName && m_nFontSize == nFontSize && m_bFontBold == bBold) return;
	m_strFontName = strFontName;
	m_nFontSize = nFontSize;
	m_bFontBold = bBold;
	if(!IsCheck() && m_pText) m_pText->SetFont(strFontName, nFontSize, bBold);
}

void CCheckBox::SetTextColor(COLORREF clrColor)
{
	if(m_clrTextColor == clrColor) return;
	m_clrTextColor = clrColor;
	if(!IsCheck() && m_pText) m_pText->SetTextColor(clrColor);
}

void CCheckBox::SetCheckFont(const std::string &strFontName, const int &nFontSize, const bool &bBold)
{
	if(m_strCheckFontName == strFontName && m_nCheckFontSize == nFontSize && m_bCheckFontBold == bBold) return;
	m_strCheckFontName = strFontName;
	m_nCheckFontSize = nFontSize;
	m_bCheckFontBold = bBold;
	if(IsCheck() && m_pText) m_pText->SetFont(strFontName, nFontSize, bBold);
}

void CCheckBox::SetCheckTextColor(COLORREF clrColor)
{
	if(m_clrCheckTextColor == clrColor) return;
	m_clrCheckTextColor = clrColor;
	if(IsCheck() && m_pText) m_pText->SetTextColor(clrColor);
}
} //namespace LSY