#include "stdafx.h"
#include "Group.h"
#include "../../Functor/Message.h"
#include "../Button/Radio.h"
#include "./DumyImage.h"
#include "../ObjectMan.h"
#include <algorithm>
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CGroup::CGroup(void)
:	m_pRectImage(NULL)
{
	if(IsTool() && GetType() != EOT_TAB)
	{
		m_pRectImage = new CDumyImage;
		m_pRectImage->SetLocalAlpha(0.5f);
		AddLocalChild(m_pRectImage);
	}
}

CGroup::~CGroup()
{
}

CObjectBase *CGroup::NewObject(void)
{
	return new CGroup;
}

CObjectBase *CGroup::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CGroup *pGroup			= (CGroup *)pObject;
	if(m_listRadio.size() != 0)
	{
		int nIndex = GetCurSelIndex();

		std::list<CRadio *>::iterator start = m_listRadio.begin();
		std::list<CRadio *>::iterator end = m_listRadio.end();
		CRadio *pRadio = NULL;
		for(;start != end;++start)
		{
			pRadio = (CRadio *)(*start)->Clone(true);
			pGroup->AttachRadio(pRadio);
		}

		pGroup->SetCurSelFromIndex(nIndex);
	}

	return pObject;
}

LRESULT CGroup::OnLocalChildNotify(CMessage *msg)
{
	if(msg->GetID() == EM_O_CHECKCHANGE)
	{
		Dispatch(msg);

		CMO_CheckChange *pMsg = (CMO_CheckChange *)msg;
		CRadio *pRadio = (CRadio *)pMsg->GetObject(); 

		if(pMsg->GetValue())
		{
			CMO_SelChange msg(this, GetRadioToIndex(pRadio));
			Dispatch(&msg);
		}
	}

	return __super::OnLocalChildNotify(msg);
}

void CGroup::ClearRadioCheck(CRadio *pException, const bool &bSendMsg/* = false*/)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();
	CRadio *pRadio = NULL;
	for(;start != end;++start)
	{
		pRadio = (CRadio *)(*start);
		if(pRadio != pException)
		{
			pRadio->SetCheck(false, bSendMsg);
			AddLocalChild(pRadio);
		}
	}
	AddLocalChild(pException);
}

void CGroup::AttachRadio(CRadio *pRadio, int nIndex/* = -1*/)
{
	ASSERT(pRadio != NULL);

	CRadio *pCur = NULL;
	if(m_listRadio.size() == 0)
	{
		pRadio->SetCheck(true);
	}
	else
	{
		pCur = GetCurSel();
		if(pCur == NULL)
		{
			if(pRadio->IsCheck())
			{
			}
			else
			{
				pCur = *m_listRadio.begin();
				pCur->SetCheck(true);
			}
		}
		else
		{
			pRadio->SetCheck(false);
		}
	}

	if(nIndex < 0) m_listRadio.push_back(pRadio);
	else
	{
		std::list<CRadio *>::iterator start = m_listRadio.begin();
		std::list<CRadio *>::iterator end = m_listRadio.end();
		for(int i=0;start != end;++start, ++i)
		{
			if(i==nIndex) m_listRadio.insert(start, pRadio);
		}
	}

	AddLocalChild(pRadio);
	if(pCur) AddLocalChild(pCur);
}

void CGroup::DettachRadio(CRadio *pRadio)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();
	std::list<CRadio *>::iterator findIt = find(start, end, pRadio);
	ASSERT(findIt != end);
	if(findIt == end) return;
	m_listRadio.remove(pRadio);
	SubChild(pRadio);
}

void CGroup::DettachAllRadio(void)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();

	for(;start != end;++start)
	{
		SubChild(*start);
	}

	m_listRadio.clear();
}

CRadio *CGroup::GetCurSel(void)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();

	for(;start != end;++start)
	{
		if((*start)->IsCheck()) return *start;
	}

	return NULL;
}

const int CGroup::GetCurSelIndex(void)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();
	int i=0;
	for(;start != end;++start)
	{
		if((*start)->IsCheck()) return i;
		else ++i;
	}

	return -1;
}

const int CGroup::GetCurSelID(void)
{
	CRadio *pRadio = GetCurSel();
	if(pRadio) return pRadio->GetID();
	return -1;
}

CRadio *CGroup::GetRadioFromIndex(const int &nIndex)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();
	for(int i=0;start != end;++start,++i)
	{
		if(i == nIndex) return *start;
	}
	return NULL;
}

const int CGroup::GetRadioToIndex(CRadio *pRadio)
{
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();
	for(int i=0;start != end;++start,++i)
	{
		if(*start == pRadio) return i;
	}
	return -1;
}

void CGroup::SetCurSelFromID(const int &nID)
{
	if(GetCurSelID() == nID) return;
	if(nID < 0) return;

	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();

	for(;start != end;++start)
	{
		if((*start)->GetID() == nID)
		{
			SetCurSelFromObject(*start);
			return;
		}
	}
}

void CGroup::SetCurSelFromIndex(const int &nIndex)
{
	if(GetCurSelIndex() == nIndex) return;
	if(nIndex < 0) return;

	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();

	int i=0;
	for(;start != end;++start)
	{
		if(i == nIndex)
		{
			SetCurSelFromObject(*start);
			return;
		}
		else i++;
	}
}

void CGroup::SetCurSelFromObject(CRadio *pRadio)
{
	if(pRadio == NULL) return;

	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();

	for(;start != end;++start)
	{
		if(*start == pRadio)
		{
			(*start)->SetCheck(true);
		}
		else
		{
			(*start)->SetCheck(false);
			AddLocalChild(*start);
		}
	}

	AddLocalChild(pRadio);
}

void CGroup::SetLocalRect(const CRect &rtRect)
{
	if(rtRect == m_rtLocalRect) return;
	if(IsTool() && GetType() != EOT_TAB)
	{
		CRect rtTemp = rtRect;
		if(rtTemp.GetWidth() == 0.0f || rtTemp.GetHeight() == 0.0f)
		{
			m_pRectImage->SetShow(false);
		}
		else
		{
			m_pRectImage->Create((int)rtTemp.GetWidth(), (int)rtTemp.GetHeight());
			m_pRectImage->ClearColor(RGB(128, 128, 20));
			m_pRectImage->SetShow(true);
		}
	}
	CObjectBase::SetLocalRect(rtRect);
}

size_t CGroup::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);	//전체 Attach된 라디오 버튼의 갯수
	std::list<CRadio *>::iterator start = m_listRadio.begin();
	std::list<CRadio *>::iterator end = m_listRadio.end();
	for(;start != end;++start)
	{
		nRet += (*start)->GetClipSize(nVersion);
	}
	nRet += sizeof(int);	//현재 선택된 라디오 번튼의 인덱스
	return nRet;
}

size_t CGroup::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = m_listRadio.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);
	if(nSize != 0)
	{
		std::list<CRadio *>::iterator start = m_listRadio.begin();
		std::list<CRadio *>::iterator end = m_listRadio.end();
		for(;start != end;++start)
		{
			nRet += (*start)->SaveClipSize(nVersion, pDest+nRet);
		}
		int nIndex = GetCurSelIndex();
		memcpy(pDest+nRet, &nIndex, sizeof(int));
		nRet += sizeof(int);	//현재 선택된 라디오 번튼의 인덱스
	}
	return nRet;
}

size_t CGroup::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);

	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);
	if(nSize != 0)
	{
		CRadio *pRadio = NULL;
		for(int i=0;i<(int)nSize;++i)
		{
			pRadio = new CRadio;
			nRet += sizeof(ENUM_OBJECTTYPE);
			nRet += pRadio->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
			AttachRadio(pRadio);
		}
		int nIndex = 0;
		memcpy(&nIndex, pDest+nRet, sizeof(int));
		nRet += sizeof(int);	//현재 선택된 라디오 번튼의 인덱스

		SetCurSelFromIndex(nIndex);
	}

	if(IsTool())
	{
		if(GetType() != EOT_TAB)
		{
			CRect rtTemp = m_rtLocalRect;
			if(rtTemp.GetWidth() == 0.0f || rtTemp.GetHeight() == 0.0f)
			{
				m_pRectImage->SetShow(false);
			}
			else
			{
				m_pRectImage->Create((int)rtTemp.GetWidth(), (int)rtTemp.GetHeight());
				m_pRectImage->ClearColor(RGB(128, 128, 20));
				m_pRectImage->SetShow(true);
			}
		}

		SetMessage(true);
	}	
	return nRet;
}

bool CGroup::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	if(m_listRadio.size() != 0)
	{
		int nIndex = GetCurSelIndex();
		TiXmlElement xmlAttach("attach");
		xmlAttach.SetAttribute("cursel", nIndex);
		std::list<CRadio *>::iterator start = m_listRadio.begin();
		std::list<CRadio *>::iterator end = m_listRadio.end();
		for(;start != end;++start)
		{
			(*start)->SaveXml(&xmlAttach);
		}
		pXmlObject->InsertEndChild(xmlAttach);
	}
	return true;
}

bool CGroup::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);
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
			case EOT_RADIO:
				AttachRadio((CRadio *)pChildObject);
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			OnAttachChild(pChildObject);
			pChildObject->OnCreated();
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
		int nIndex = 0;
		GetAttribute(pXmlAttach, "cursel", nIndex);
		SetCurSelFromIndex(nIndex);
	}

	if(IsTool())
	{
		SetMessage(true);
		if(GetType() != EOT_TAB)
		{
			CRect rtTemp = m_rtLocalRect;
			if(rtTemp.GetWidth() == 0.0f || rtTemp.GetHeight() == 0.0f)
			{
				m_pRectImage->SetShow(false);
			}
			else
			{
				m_pRectImage->Create((int)rtTemp.GetWidth(), (int)rtTemp.GetHeight());
				m_pRectImage->ClearColor(RGB(128, 128, 20));
				m_pRectImage->SetShow(true);
			}
		}
	}

	return true;
}
} //namespace LSY