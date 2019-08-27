#include "stdafx.h"
#include "Combo.h"
#include "../../Functor/Message.h"
#include "../Control/Edit.h"
#include "../Button/Button.h"
#include "../List/TextList.h"
#include "../ObjectMan.h"
#include "../Base/DumyImage.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CCombo::CCombo(void)
:	m_pEdit(NULL),
	m_pList(NULL),
	m_pButton(NULL),
	m_nCurSel(-1),
	m_pRectImage(NULL),
	m_bAttachListShow(true)
{
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

CCombo::~CCombo()
{
	if(m_pList && (m_pList->GetParent() != this))
	{
		CObjectBase *pParent = m_pList->GetParent();
		if(pParent) pParent->SubChild(m_pList);
		delete m_pList;
		m_pList = NULL;
	}
}

bool CCombo::Create(void)
{
	return true;
}

void CCombo::SetText(const std::string &strText)
{
	if(m_pEdit)	m_pEdit->SetText(strText);
}

const std::string CCombo::GetText(void)
{
	return m_pEdit?m_pEdit->GetText():"";
}

//==================================================================================================
//텍스트리스트 관련 함수들
//추가 삭제 관련 함수들
int CCombo::AddText(const std::string &strText)
{
	if(m_pList) return m_pList->AddText(strText);
	return -1;
}

int CCombo::InsertText(const int &nIndex, const std::string &strText)
{
	if(m_pList) return m_pList->InsertText(nIndex, strText);
	return -1;
}


bool CCombo::RemoveText(const std::string &strText)
{
	if(m_pList) return m_pList->RemoveText(strText);
	return false;
}

bool CCombo::RemoveText(const int &nIndex)
{
	if(m_pList) return m_pList->RemoveText(nIndex);
	return false;
}

bool CCombo::RemoveText(const LPARAM &lParam)
{
	if(m_pList) return m_pList->RemoveText(lParam);
	return false;
}

void CCombo::RemoveAll(void)
{
	if(m_pList) m_pList->RemoveAll();
}

//아이템 데이터 관련 함수들
void CCombo::SetItemData(const int &nIndex, const LPARAM &lParam)
{
	if(m_pList) m_pList->SetItemData(nIndex, lParam);
}

const LPARAM CCombo::GetItemData(const int nIndex)
{
	if(m_pList) return m_pList->GetItemData(nIndex);
	return NULL;
}

//아이템의 총 갯수를 구한다.
const int CCombo::GetItemCount(void)
{
	if(m_pList) return m_pList->GetItemCount();
	return 0;
}

//nIndex의 아이템 텍스트를 구한다.
const std::string CCombo::GetItemString(const int &nIndex)
{
	if(m_pList) return m_pList->GetItemString(nIndex);
	return "";
}

void CCombo::SetItemString(const int &nIndex, const std::string &strText)
{
	if(m_pList) m_pList->SetItemString(nIndex, strText);
}

void CCombo::SetItemColor(const int &nIndex, const COLORREF &clrColor)
{
	if(m_pList) m_pList->SetItemColor(nIndex, clrColor);
}

//nIndex 아이템의 화면의 uFormat 위치에 보이도록 셋팅한다.
//uFormat값은 DT_VCENTER, DT_TOP, DT_BOTTOM 이다.
void CCombo::SetVisibleItem(int nIndex, UINT uFormat)
{
	if(m_pList) m_pList->SetVisibleItem(nIndex, uFormat);
}
//nIndex 아이템을 화면에 보이게 한다. 이미 보이면 그래도 두고
//보이는쪽 보다 윗쪽에 있으면 TOP으로 아래에 있으면 Bottom으로 보이도록 한다.
void CCombo::SetVisibleItem(int nIndex)
{	
	if(m_pList) m_pList->SetVisibleItem(nIndex);
}

void CCombo::SetLineScroll(const bool &bLineScroll)
{
	if(m_pList) m_pList->SetLineScroll(bLineScroll);
}

const bool CCombo::IsLineScroll(void)
{
	if(m_pList) return m_pList->IsLineScroll();
	return false;
}

//아이템 선택 관련 함수들
const int &CCombo::GetCurSelectItem(void)
{
	if(m_pList) return m_pList->GetCurSelectItem();
	return m_nCurSel;
}

void CCombo::SetCurSelectItem(const int &nSel)
{
	std::string strText;
	if(m_pList)
	{
		m_pList->SetCurSelectItem(nSel);
		strText = m_pList->GetItemString(nSel);
	}

	if(m_pEdit) m_pEdit->SetText(strText);
}

//라인 높이 관련 함수들
void CCombo::SetLineHeight(const int &nLineHeight)
{
	if(m_pList) m_pList->SetLineHeight(nLineHeight);
}

const int CCombo::GetLineHeight(void)
{
	if(m_pList) return m_pList->GetLineHeight();
	return 0;
}

//nIndex 아이템이 화면에 보이는지 알아보는 함수
const bool CCombo::IsVisualItem(const int &nIndex)
{
	if(m_pList) return m_pList->IsVisualItem(nIndex);
	return false;
}

//화면에 보이는 아이템중 ptPos에 있는 아이템이 어떤것인지 알아보는 함수
const int  CCombo::HitTest(const CPoint &ptPos)
{
	if(m_pList) return m_pList->HitTest(ptPos);
	return -1;
}

//화면에 처음으로 보이는 아이템, 마지막으로 보이는 아이템을 알아내는 함수
const int CCombo::GetFirstVisibleItem(void)
{
	if(m_pList) return m_pList->GetFirstVisibleItem();
	return -1;
}

void CCombo::SetDefaultFindType(bool bUserData)
{
	m_pList->SetDefaultFindType(bUserData);
}

const int CCombo::GetLastVisibleItem(void)
{
	if(m_pList) return m_pList->GetLastVisibleItem();
	return -1;
}

int CCombo::FirstFind(const LPARAM &lUserData)
{
	if(m_pList) return m_pList->FirstFind(lUserData);
	return -1;
}

int CCombo::NextFind(const LPARAM &lUserData)
{
	if(m_pList) return m_pList->NextFind(lUserData);
	return -1;
}

//라인높이를 이용하여 리스트의 크기를 결정하는 함수
//nTotalLine * m_nLineHeight를 높이로 설정한다.
void CCombo::SetTotalHeightUseLineHeight(int nTotalLine)
{
	if(m_pList) return m_pList->SetTotalHeightUseLineHeight(nTotalLine);
}
//==================================================================================================

LRESULT CCombo::OnLocalChildNotify(CMessage *msg)
{
	CObjectBase *pObject = ((CObjectMessage *)msg)->GetObject();
	if(pObject == m_pButton)
	{
		switch(msg->GetID())
		{
		case EM_O_MOUSELCLICK:
			return OnClickButton(msg);
		}
	}

	if(pObject == m_pEdit)
	{
		switch(msg->GetID())
		{
		case EM_O_RETURN:
			return OnEditReturn(msg);
		}
	}

	return CObjectBase::OnLocalChildNotify(msg);
}

void CCombo::AttachEdit(CEdit *pEdit)
{
	ASSERT(m_pEdit == NULL && pEdit != NULL);
	m_pEdit = pEdit;

	AddLocalChild(m_pEdit);
	if(m_pButton) AddLocalChild(m_pButton);
	if(m_pList && (m_pList->GetParent() == this)) AddLocalChild(m_pList);
}

void CCombo::DettachEdit(void)
{
	ASSERT(m_pEdit != NULL);
	SubChild(m_pEdit);
	m_pEdit = NULL;
}

void CCombo::AttachButton(CButton *pButton)
{
	ASSERT(m_pButton == NULL && pButton != NULL);
	m_pButton = pButton;

	AddLocalChild(m_pButton);
	if(m_pList && (m_pList->GetParent() == this)) AddLocalChild(m_pList);
}

LRESULT CCombo::OnClickButton(CMessage *pMsg)
{
	if(IsTool() && !IsFixed()) return TRUE;
	if(m_pList && m_pProject)
	{
		if(m_pProject->GetPopup() == m_pList) m_pProject->SetPopup(NULL, NULL);
		else
		{
			int nIndex = -1;
			int nTotal = m_pList->GetItemCount();
			std::string strOutput = "";
			if(m_pEdit) strOutput = m_pEdit->GetText();

			int i=0;
			for(i=0;i<nTotal;++i)
			{
				if(m_pList->GetItemString(i) == strOutput)
				{
					m_pList->SetCurSelectItem(i);
					if(!m_pList->IsVisualItem(i)) m_pList->SetVisibleItem(i, DT_VCENTER);
					break;
				}
			}

			if(i == nTotal)
			{
				m_pList->SetCurSelectItem(-1);
				m_pList->SetVisibleItem(0, DT_TOP);
			}

			m_pProject->SetPopup(m_pList, NULL);
			m_pList->SetShow(true);
		}
	}
	return TRUE;
}

LRESULT CCombo::OnEditReturn(CMessage *pMsg)
{
	if(m_pProject)
	{
		m_pProject->GetObjectMan()->SetFocus(NULL);

		int nIndex = -1;
		int nTotal = m_pList->GetItemCount();
		std::string strOutput = m_pEdit->GetText();

		int i=0;
		for(i=0;i<nTotal;++i)
		{
			if(m_pList->GetItemString(i) == strOutput)
			{
				m_pList->SetCurSelectItem(i);
				if(!m_pList->IsVisualItem(i)) m_pList->SetVisibleItem(i, DT_VCENTER);
				break;
			}
		}

		if(i == nTotal)
		{
			m_pList->SetCurSelectItem(-1);
			m_pList->SetVisibleItem(0, DT_TOP);
		}
	}
	return TRUE;
}

LRESULT CCombo::OnClickList(CMessage *pMsg)
{
	CMO_MouseLClick *msg = (CMO_MouseLClick *)pMsg;
	if(m_pProject->GetPopup() == m_pList) m_pProject->SetPopup(NULL, NULL);
	return TRUE;
}

void CCombo::DettachButton(void)
{
	ASSERT(m_pButton != NULL);
	SubChild(m_pButton);
	m_pButton = NULL;
}

void CCombo::AttachList(CTextList *pList)
{
	ASSERT(m_pList == NULL && pList != NULL);
	m_pList = pList;
	m_bAttachListShow = m_pList->IsShow();
	m_pList->SetShow(false);
	m_pList->AddHandler(EM_O_SELCHANGE, Fnt(this, &CCombo::OnSelChange));
	m_pList->AddHandler(EM_O_MOUSELCLICK, Fnt(this, &CCombo::OnClickList));

	AddLocalChild(m_pList);
}

void CCombo::DettachList(void)
{
	ASSERT(m_pList != NULL);
	if(m_pProject && (m_pList->GetParent() != this))
	{
		m_pProject->SetPopup(NULL, NULL);
	}

	m_pList->SetShow(m_bAttachListShow);
	SubChild(m_pList);
	m_pList->RemoveHandler(EM_O_SELCHANGE);
	m_pList->RemoveHandler(EM_O_MOUSELCLICK);
	m_pList = NULL;
}

LRESULT CCombo::OnSelChange(CMessage *pMsg)
{
	CMO_SelChange *msg = (CMO_SelChange *)pMsg;
	m_nCurSel = msg->GetValue();
	std::string strText = m_pList?m_pList->GetItemString(m_nCurSel):"";
	if(m_pEdit) m_pEdit->SetText(strText);
	if(m_pProject) m_pProject->SetPopup(NULL, NULL);
	return Dispatch(msg);
}

bool CCombo::OnClosePopup(CObjectBase *pFocusObject)
{
	bool bClose = false;
	//포커스 오브젝트가 없거나, 포커스를 받은 오브젝트가 콤보 오브젝트의 차일드가 아닐경우 창을 닫는다.
	if(pFocusObject)
	{
		if(pFocusObject->IsParent(this) == false) bClose = true;
	}
	else bClose = true;

	if(bClose)
	{
		const CPoint ptPos = m_pList->GetPos();
		const float fScaleX = m_pList->GetScaleX();
		const float fScaleY = m_pList->GetScaleY();
		const float fRotation = m_pList->GetRotation();
		const float fAlpha = m_pList->GetLocalAlpha();
		
		AddLocalChild(m_pList);

		m_pList->SetPos(ptPos);
		m_pList->SetScaleX(fScaleX);
		m_pList->SetScaleY(fScaleY);
		m_pList->SetRotation(fRotation);
		m_pList->SetLocalAlpha(fAlpha);

		m_pList->SetShow(false);
	}

	return bClose;
}

CObjectBase *CCombo::NewObject(void)
{
	return new CCombo;
}

CObjectBase *CCombo::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CCombo *pCombo		= (CCombo *)pObject;

	if(m_pEdit) pCombo->AttachEdit((CEdit *)m_pEdit->Clone(true));
	if(m_pButton) pCombo->AttachButton((CButton *)m_pButton->Clone(true));
	if(m_pList) pCombo->AttachList((CTextList *)m_pList->Clone(true));

	pCombo->m_nCurSel = m_nCurSel;
	pCombo->m_bAttachListShow = m_bAttachListShow;

	return pObject;
}

size_t CCombo::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);	//에디트 사용유무
	if(m_pEdit) nRet += m_pEdit->GetClipSize(nVersion);
	nRet += sizeof(bool);	//버튼 사용유무
	if(m_pButton) nRet += m_pButton->GetClipSize(nVersion);
	nRet += sizeof(bool);	//리스트 사용유무
	if(m_pList) nRet += m_pList->GetClipSize(nVersion);
	nRet += sizeof(int);	//m_nCurSel
	nRet += sizeof(bool);	//m_bAttachListShow
	return nRet;
}

size_t CCombo::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);

	bool bUse = m_pEdit?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);	//에디트 사용유무
	if(m_pEdit) nRet += m_pEdit->SaveClipSize(nVersion, pDest+nRet);

	bUse = m_pButton?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);	//버튼 사용유무
	if(m_pButton) nRet += m_pButton->SaveClipSize(nVersion, pDest+nRet);

	bUse = m_pList?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);	//리스트 사용유무
	if(m_pList) nRet += m_pList->SaveClipSize(nVersion, pDest+nRet);

	memcpy(pDest+nRet, &m_nCurSel, sizeof(int));
	nRet += sizeof(int);	//m_nCurSel

	memcpy(pDest+nRet, &m_bAttachListShow, sizeof(bool));
	nRet += sizeof(bool);
	return nRet;
}

size_t CCombo::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);

	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//에디트 사용유무
	if(bUse)
	{
		CEdit *pEdit = new CEdit;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pEdit->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachEdit(pEdit);
	}

	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//버튼 사용유무
	if(bUse)
	{
		CButton *pButton = new CButton;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pButton->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachButton(pButton);
	}

	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//리스트 사용유무
	if(bUse)
	{
		CTextList *pList = new CTextList;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pList->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachList(pList);
	}

	memcpy(&m_nCurSel, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nCurSel

	memcpy(&m_bAttachListShow, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);

	if(IsTool()) SetMessage(true);

	return nRet;
}

bool CCombo::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlAttach("attach");
	xmlAttach.SetAttribute("listshow", m_bAttachListShow?1:0);
	if(m_pEdit) m_pEdit->SaveXml(&xmlAttach);
	if(m_pButton) m_pButton->SaveXml(&xmlAttach);
	if(m_pList) m_pList->SaveXml(&xmlAttach);
	pXmlObject->InsertEndChild(xmlAttach);
	return true;
}

bool CCombo::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
	if(pXmlAttach->Attribute("listshow")) GetAttribute(pXmlAttach, "listshow", m_bAttachListShow);

	CObjectBase *pChildObject = NULL;
	TiXmlElement *pXmlChildObject = pXmlAttach->FirstChildElement("Object");
	while(pXmlChildObject)
	{
		pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
		switch(pChildObject->GetType())
		{
		case EOT_EDIT:
			AttachEdit((CEdit *)pChildObject);
			break;
		case EOT_BUTTON:
			AttachButton((CButton *)pChildObject);
			break;
		case EOT_TEXTLIST:
			AttachList((CTextList *)pChildObject);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		OnAttachChild(pChildObject);
		pChildObject->OnCreated();
		pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
	}

	if(IsTool()) SetMessage(true);

	return true;
}
} //namespace LSY