#include "stdafx.h"
#include "ObjectBase.h"
#include <algorithm>
#include "../../Define.h"
#include "../../Functor/Message.h"
#include "../ObjectMan.h"
#include "../Mover/Mover.h"
#include "../Tool/ObjectRect.h"
#include "../../TinyXml/tinyxml.h"
#include "../../Utility/MyMime64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEF_CHECK_CHANGE		if(m_bChange) CheckChange()
#define DEF_CHECK_CHANGE_ALPHA	if(m_bChangeAlpha) CheckChangeAlpha()

namespace LSY
{
CObjectBase::CObjectBase()
:	m_ptPos(CPointZero),
	m_ptLocalPos(CPointZero),
	m_rtRect(CRectZero),
	m_pParent(NULL),
	m_bMessage(false),
	m_fRotation(0.0f),
	m_fScaleX(1.0f),
	m_fScaleY(1.0f),
	m_nEffectType(EOET_NORMAL),
	m_bXFlip(false),
	m_bXFlipRef(false),
	m_bYFlip(false),
	m_bYFlipRef(false),
	m_ptPivot(CPointZero),
	m_fLocalRotation(0.0f),
	m_fLocalScaleX(1.0f),
	m_fLocalScaleY(1.0f),
	m_rtLocalRect(CRectZero),
	m_ptMove(CPointZero),
	m_bShow(true),
	m_bShowRef(true),
	m_bEnable(true),
	m_bEnableRef(true),
	m_bChange(false),
	m_bChangeAlpha(false),
	m_dwDrawOption(0),
	m_ptDraw(CPointZero),
	m_nDrawType(EODT_NORMAL),
	m_nID(_ID_OBJECT_NOTUSE),
	m_strName(""),
	m_bFixed(false),
	m_bDynamic(false),
	m_pClipRect(NULL),
	m_pClipLocal(NULL),
	m_bClipDraw(true),
	m_bFocus(false),
	m_bMouseOver(false),
	m_pProject(NULL),
	m_fLocalAlpha(1.0f),
	m_fAlpha(1.0f),
	m_nAlpha(31),
	m_bLocalChild(false),
	m_bMouseMove(false),
	m_ptDragGab(CPointZero),
	m_bDrag(false),
	m_bUseLogic(false),
	m_rtBound(CRectZero),
	m_fCos(1.0f),
	m_fSin(0.0f),
	m_bDraw(true),
	m_pInterceptSelect(NULL),
	m_bUseMouseWheel(false),
	m_pOriginalObject(NULL),
	m_pObserverObject(NULL),
	m_bLocalClipUseParentPos(false),
	m_nColor(0),
	m_pMover(NULL),
	m_bExpand(true),
	m_pObjectRect(NULL),
	m_bLocalFind(true),
	m_bTracking(false),
	m_bIgnoreParentClip(false),
	m_bIgnoreParentPos(false),
	m_bRemove(false),
	m_bEventDraw(false),
	m_rtOldDraw(CRectZero),
	m_bTempEventDraw(false),
	m_bSendEventDraw(false)
{
	m_itFind = m_listLocalChild.end();
}

CObjectBase::~CObjectBase()
{
	if(m_pProject) m_pProject->OnRemoveObject(this);
	if(m_pParent) m_pParent->SubChild(this);

	if(m_pObjectRect)
	{
		delete m_pObjectRect;
		m_pObjectRect = NULL;
	}

	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();
	LISTOBJECT::iterator next = start;
	while(start != end)
	{
		++next;

		delete *start;

		start = next;
	}

	start = m_listChild.begin();
	end = m_listChild.end();
	next = start;
	while(start != end)
	{
		++next;

		delete *start;

		start = next;
	}

	if(m_pClipRect) delete m_pClipRect;
	m_pClipRect = NULL;

	if(m_pClipLocal) delete m_pClipLocal;
	m_pClipLocal = NULL;

	if(m_pMover)
	{
		delete m_pMover;
		m_pMover = NULL;
	}

	ASSERT((int)m_listChild.size() == 0);
	ASSERT((int)m_listLocalChild.size() == 0);
}

void CObjectBase::SetObserverRect(CObjectRect *pObjectRect)
{
	if(m_pObjectRect == pObjectRect) return;
	m_pObjectRect = pObjectRect;
}

void CObjectBase::SetParent(CObjectBase *pParent)
{
	if(m_pParent == pParent) return;
	if(pParent) OnSetParent(pParent, true);
	else OnSetParent(m_pParent, false);
	m_pParent = pParent;
}

void CObjectBase::SetMover(CMover *pMover)
{
	m_pMover = pMover;

	if(pMover)
	{
		m_pMover = pMover;
		m_pMover->SetParent(this);
		if(m_pProject) m_pMover->Start();
	}
}

void CObjectBase::SetObjectProject(CObjectProject *pProject)
{
	if(m_pProject == pProject) return;

	CObjectMan *pObjectMan = pProject?pProject->GetObjectMan():NULL;
	if(pObjectMan && pObjectMan->IsEventDraw())
	{
		m_bEventDraw = true;
	}
	else
	{
		m_bEventDraw = false;
	}

	if(pProject)
	{
		m_pProject = pProject;
		OnAddSubRootObject(true);
	}
	else
	{
		OnAddSubRootObject(false);
		m_pProject = pProject;
	}

	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->SetObjectProject(pProject);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->SetObjectProject(pProject);
	}
}

void CObjectBase::OnProjectActiveChange(bool bActive)
{
	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->OnProjectActiveChange(bActive);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->OnProjectActiveChange(bActive);
	}
}

void CObjectBase::OnAddSubRootObject(bool bAdd)
{
	if(m_pProject && m_pMover)
	{
		if(bAdd) m_pMover->Start();
		else m_pMover->End(m_pMover->IsSetEndPos());
	}

	if(m_pProject && m_bShow && m_bShowRef)
	{
		if(m_bMessage)
		{
			if(bAdd) m_pProject->AddMouseMessageObject(this);
			else m_pProject->SubMouseMessageObject(this);
		}

		if(m_bUseLogic)
		{
			if(bAdd) m_pProject->AddLogicObject(this);
			else m_pProject->SubLogicObject(this);
		}
	}
}

void CObjectBase::AddChild(CObjectBase *pObject, CObjectBase *pDest/* = NULL*/, bool bBefore/* = true*/)
{
	ASSERT(pObject != NULL && pObject != this);
	ASSERT(pObject != pDest);
	//추가하려는 Object의 Parent가 자신이면 가장 뒤로 이동 시킨다.
	CObjectBase *pParent = pObject->GetParent();
	if(pParent)
	{
		if(pParent == this)
		{
			if(pObject->IsLocalChild())
			{
				m_listLocalChild.remove(pObject);
				pObject->SetLocalChild(false);
			}
			else
			{
				m_listChild.remove(pObject);
			}
			pObject->SetParent(NULL);

			if(pDest)
			{
				LISTOBJECT::iterator findIt = find(m_listChild.begin(), m_listChild.end(), pDest);
				if(findIt == m_listChild.end())
				{
					m_listChild.push_back(pObject);
				}
				else
				{
					if(bBefore)
					{
						m_listChild.insert(findIt, pObject);
					}
					else
					{
						++findIt;
						if(findIt == m_listChild.end())
						{
							m_listChild.push_back(pObject);
						}
						else
						{
							m_listChild.insert(findIt, pObject);
						}
					}
				}
			}
			else m_listChild.push_back(pObject);
			pObject->SetParent(this);
			if(pObject->IsShow() && pObject->IsShowRef() && m_pProject)
				m_pProject->ChangeMouseMessageObject(pObject);

			return;
		}
		else
		{
			pParent->SubChild(pObject);
		}
	}

	if(pDest)
	{
		LISTOBJECT::iterator findIt = find(m_listChild.begin(), m_listChild.end(), pDest);
		if(findIt == m_listChild.end())
		{
			m_listChild.push_back(pObject);
		}
		else
		{
			if(bBefore)
			{
				m_listChild.insert(findIt, pObject);
			}
			else
			{
				++findIt;
				if(findIt == m_listChild.end())
				{
					m_listChild.push_back(pObject);
				}
				else
				{
					m_listChild.insert(findIt, pObject);
				}
			}
		}
	}
	else m_listChild.push_back(pObject);

	pObject->SetParent(this);
	pObject->SetShowRef(m_bShow && m_bShowRef);
	pObject->SetEnableRef(m_bEnable && m_bEnableRef);
	pObject->SetObjectProject(m_pProject);
	pObject->SetChange(true);
	pObject->SetChangeAlpha(true);
}

void CObjectBase::SubChild(CObjectBase *pObject)
{
	ASSERT(pObject != NULL && pObject != this);
	CObjectBase *pParent = pObject->GetParent();
	if(pParent != this) return;

	if(m_pProject) m_pProject->OnRemoveChildObject(pObject);

	if(pObject->IsLocalChild())
	{
		m_listLocalChild.remove(pObject);
		pObject->SetLocalChild(false);
	}
	else m_listChild.remove(pObject);

	pObject->SetParent(NULL);
	pObject->SetObjectProject(NULL);
}

void CObjectBase::AddLocalChild(CObjectBase *pObject, CObjectBase *pDest/* = NULL*/, bool bBefore/* = true*/)
{
	ASSERT(pObject != NULL && pObject != this);
	ASSERT(pObject != pDest);
	CObjectBase *pParent = pObject->GetParent();
	if(pParent)
	{
		if(pParent == this)
		{
			if(pObject->IsLocalChild())
			{
				m_listLocalChild.remove(pObject);
				pObject->SetLocalChild(false);
			}
			else
			{
				m_listChild.remove(pObject);
			}

			pObject->SetParent(NULL);
			pObject->SetLocalChild(true);

			if(pDest)
			{
				LISTOBJECT::iterator findIt = find(m_listLocalChild.begin(), m_listLocalChild.end(), pDest);
				if(findIt == m_listLocalChild.end())
				{
					m_listLocalChild.push_back(pObject);
				}
				else
				{
					if(bBefore)
					{
						m_listLocalChild.insert(findIt, pObject);
					}
					else
					{
						++findIt;
						if(findIt == m_listLocalChild.end())
						{
							m_listLocalChild.push_back(pObject);
						}
						else
						{
							m_listLocalChild.insert(findIt, pObject);
						}
					}
				}
			}
			else m_listLocalChild.push_back(pObject);
			pObject->SetParent(this);
			if(pObject->IsShow() && pObject->IsShowRef() && m_pProject)
				m_pProject->ChangeMouseMessageObject(pObject);

			return;
		}
		else
		{
			pParent->SubChild(pObject);
		}
	}

	if(pDest)
	{
		LISTOBJECT::iterator findIt = find(m_listLocalChild.begin(), m_listLocalChild.end(), pDest);
		if(findIt == m_listLocalChild.end())
		{
			m_listLocalChild.push_back(pObject);
		}
		else
		{
			if(bBefore)
			{
				m_listLocalChild.insert(findIt, pObject);
			}
			else
			{
				++findIt;
				if(findIt == m_listLocalChild.end())
				{
					m_listLocalChild.push_back(pObject);
				}
				else
				{
					m_listLocalChild.insert(findIt, pObject);
				}
			}
		}
	}
	else m_listLocalChild.push_back(pObject);

	pObject->SetLocalChild(true);
	pObject->SetParent(this);
	pObject->SetEnableRef(m_bEnable && m_bEnableRef);
	pObject->SetShowRef(m_bShow && m_bShowRef);
	pObject->SetObjectProject(m_pProject);

	pObject->SetChange(true);
	pObject->SetChangeAlpha(true);
}

const bool CObjectBase::IsChild(CObjectBase *pObject)
{
	if(pObject == NULL) return false;

	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();
	for(;start != end;++start)
	{
		if(*start == pObject) return true;
		if((*start)->IsChild(pObject)) return true;
	}

	start = m_listChild.begin();
	end = m_listChild.end();
	for(;start != end;++start)
	{
		if(*start == pObject) return true;
		if((*start)->IsChild(pObject)) return true;
	}

	return false;
}

LRESULT CObjectBase::Dispatch(CMessage *msg)
{
	//Local Child일 경우 Parent로 모든 Notify를 전달한다.
	if(m_bLocalChild) return m_pParent->OnLocalChildNotify(msg);
	//감시하고 있는 Observer Object가 있을 경우 해당 Object로 모든 Notify를 전달한다.
	if(m_pObserverObject)
	{
		m_pObserverObject->OnObserverNotify(msg);
	}
	//Clone시 원본 Dispatch를 사용하게 되어 있었다면 모든 Notify를 원본 Object로 전달한다.
	if(m_pOriginalObject) return m_pOriginalObject->Dispatch(msg);
	return m_cMessage.Call(msg);
}

LRESULT CObjectBase::OnLocalChildNotify(CMessage *msg)
{
	CMO_LocalChildNotify msg2(this, (CObjectMessage *)msg);
	return m_cMessage.Call(&msg2);
}

bool CObjectBase::OnDragBegin(const CPoint &ptPos)
{
	m_ptDragGab = GetPos() - ptPos;
	m_bDrag = true;

	CMO_DragBegin msg(this, ptPos);
	Dispatch(&msg);

	return true;
}

bool CObjectBase::OnDraging(const CPoint &ptPos)
{
	if(m_bMouseMove)
	{
		DEF_CHECK_CHANGE;

		//새로운 위치
		CPoint ptNewPos = ptPos + m_ptDragGab;
		CPoint ptGab = ptNewPos - m_ptPos;

		CRect rtRect = m_rtRect + ptGab;
		CSize szSize = m_pProject->GetObjectMan()->GetSize();

		if(rtRect.left < 0.0f)
		{
			ptNewPos.x = ptNewPos.x - rtRect.left;
		}
		else if(rtRect.right > (float)szSize.cx)
		{
			ptNewPos.x = ptNewPos.x - (rtRect.right - (float)szSize.cx);
		}

		if(rtRect.top < 0.0f)
		{
			ptNewPos.y = ptNewPos.y - rtRect.top;
		}
		else if(rtRect.bottom > (float)szSize.cy)
		{
			ptNewPos.y = ptNewPos.y - (rtRect.bottom - (float)szSize.cy);
		}

		SetPos(ptNewPos);
	}

	CMO_Draging msg(this, ptPos);
	Dispatch(&msg);

	return true;
}

bool CObjectBase::OnDragEnd(const CPoint &ptPos)
{
	if(m_bMouseMove) SetPos(ptPos + m_ptDragGab);

	m_ptDragGab = CPointZero;
	m_bDrag = false;

	CMO_DragEnd msg(this, ptPos);
	Dispatch(&msg);

	return true;
}

bool CObjectBase::OnLButtonDown(const CPoint &ptPos)
{
	OnDragBegin(ptPos);

	CMO_LButtonDown msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnLButtonUP(const CPoint &ptPos)
{
	if(m_bDrag)	OnDragEnd(ptPos);

	CMO_LButtonUp msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnLButtonDbClick(const CPoint &ptPos)
{
	CMO_LButtonDbClick msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnRButtonDown(const CPoint &ptPos)
{
	CMO_RButtonDown msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnRButtonUp(const CPoint &ptPos)
{
	CMO_RButtonUp msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnRButtonDbClick(const CPoint &ptPos)
{
	CMO_RButtonDbClick msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnMButtonDown(const CPoint &ptPos)
{
	CMO_MButtonDown msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnMButtonUp(const CPoint &ptPos)
{
	CMO_MButtonDown msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnMButtonDbClick(const CPoint &ptPos)
{
	CMO_MButtonDbClick msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

bool CObjectBase::OnMouseMove(const CPoint &ptPos)
{
	if(m_bDrag)
	{
		OnDraging(ptPos);
	}

	CMO_MouseMove msg(this, ptPos);
	Dispatch(&msg);
	return true;
}

CObjectBase *CObjectBase::GetWheelFocusObject(void)
{
	if(m_bMessage && m_bUseMouseWheel)
	{
		return this;
	}

	if(m_pParent) return m_pParent->GetWheelFocusObject();

	return NULL;
}

bool CObjectBase::OnMouseWheel(const int &nKey, const short &zDelta)
{
	if(m_bMessage && m_bUseMouseWheel)
	{
		CMO_MouseWheel msg(this, nKey, zDelta);
		Dispatch(&msg);
		return true;
	}
	
	return false;
}

void CObjectBase::OnMouseOver(void)
{
	CMO_MouseOver msg(this);
	Dispatch(&msg);
	m_bMouseOver = true;
}

void CObjectBase::OnMouseLeave(void)
{
	CMO_MouseLeave msg(this);
	Dispatch(&msg);
	m_bMouseOver = false;
}

void CObjectBase::OnMouseLClick(const CPoint &ptPos)
{
	CMO_MouseLClick msg(this, ptPos);
	Dispatch(&msg);
}

void CObjectBase::OnMouseRClick(const CPoint &ptPos)
{
	CMO_MouseRClick msg(this, ptPos);
	Dispatch(&msg);
}

void CObjectBase::OnMouseMClick(const CPoint &ptPos)
{
	CMO_MouseMClick msg(this, ptPos);
	Dispatch(&msg);
}

//포커스를 잃는 Object를 받는다.
CObjectBase *CObjectBase::OnFocusIn(CObjectBase *pObject)
{
	CMO_FocusIn msg(this, pObject);
	CObjectBase *pObjectRet = (CObjectBase *)Dispatch(&msg);
	if(pObjectRet == NULL)
	{
		m_bFocus = true;
		return this;
	}

	return pObjectRet->OnFocusIn(pObject);
}

//포커를 얻는 Object를 받는다.
void CObjectBase::OnFocusOut(CObjectBase *pObject)
{
	m_bFocus = false;
	CMO_FocusOut msg(this, pObject);
	Dispatch(&msg);
}

bool CObjectBase::OnClickOutside(CObjectBase *pFocus)
{
	CMO_ClickOutside msg(this, pFocus);
	Dispatch(&msg);
	return true;
}

void CObjectBase::OnKeyDown(const int &nKey)
{
	CMO_KeyDown msg(this, nKey);
	Dispatch(&msg);
}

void CObjectBase::OnKeyUp(const int &nKey)
{
	CMO_KeyUp msg(this, nKey);
	Dispatch(&msg);
}

void CObjectBase::OnTabDown(void)
{
	CMO_TabDown msg(this);
	Dispatch(&msg);
}

void CObjectBase::OnReturn(void)
{
	CMO_Return msg(this);
	Dispatch(&msg);
}

BOOL CObjectBase::OnKeyProc(const int &nMessage, const WPARAM &wParam, const LPARAM &lParam)
{
	CMO_KeyMessageProc msg(this, nMessage, wParam, lParam);
	return Dispatch(&msg);
}

void CObjectBase::SetIgnoreParentClip(bool bIgnoreClip)
{
	if(m_bIgnoreParentClip == bIgnoreClip) return;
	m_bIgnoreParentClip = bIgnoreClip;
	SetChange(true);
}

void CObjectBase::SetIgnoreParentPos(bool bIgnorePos)
{
	if(m_bIgnoreParentPos == bIgnorePos) return;
	m_bIgnoreParentPos = bIgnorePos;
	SetChange(true);
}

void CObjectBase::Logic(const DWORD &dwTime)
{
}

void CObjectBase::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	DEF_CHECK_CHANGE;
	DEF_CHECK_CHANGE_ALPHA;

	DrawLocal(pPage);
	DrawLocalChild(pPage);
	DrawChild(pPage);
}

void CObjectBase::DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(!m_bClipDraw || !m_bDraw) return;
	m_bSendEventDraw = false;
	if(m_pClipRect)
	{
		pPage->EnableTempClip(TRUE);
		pPage->SetTempClip((int)m_pClipRect->left, (int)m_pClipRect->top, (int)m_pClipRect->right, (int)m_pClipRect->bottom);
		DrawMy(pPage);
		pPage->EnableTempClip(FALSE);
	}
	else
	{
		DrawMy(pPage);
	}
}

void CObjectBase::DrawLocalChild(NMBASE::GRAPHICGDI::CPage *pPage)
{
	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		if((*start)->m_bShow && (*start)->m_bShowRef)
			(*start)->Draw(pPage);
	}
}

void CObjectBase::DrawChild(NMBASE::GRAPHICGDI::CPage *pPage)
{
	LISTOBJECT::iterator start = m_listChild.begin();
		LISTOBJECT::iterator end = m_listChild.end();

	for(;start != end;++start)
	{
		if((*start)->m_bShow && (*start)->m_bShowRef)
			(*start)->Draw(pPage);
	}
}

void CObjectBase::DrawMy(NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CObjectBase::SetShowRef(const bool &bShowRef)
{
	if(m_bShowRef == bShowRef) return;

	bool bOldShow = m_bShow && m_bShowRef;

	m_bShowRef = bShowRef;

	if(bOldShow != (m_bShow && m_bShowRef))
	{
		CheckEventDraw();
		if(m_pProject)
		{
			if(m_bMessage)
			{
				if(m_bShow && bShowRef) m_pProject->AddMouseMessageObject(this);
				else m_pProject->SubMouseMessageObject(this);
			}

			if(m_bUseLogic)
			{
				if(m_bShow && bShowRef) m_pProject->AddLogicObject(this);
				else m_pProject->SubLogicObject(this);
			}
		}
	}

	SetChildShowRef();
}

void CObjectBase::SetChildShowRef(void)
{
	bool bShowRef = m_bShowRef && m_bShow;
	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->SetShowRef(bShowRef);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->SetShowRef(bShowRef);
	}
}

void CObjectBase::SetShow(const bool &bShow)
{
	if(m_bShow == bShow) return;

	bool bOldShow = m_bShow && m_bShowRef;

	m_bShow = bShow;

	if(bOldShow != (m_bShow && m_bShowRef))
	{
		CheckEventDraw();
		if(m_pProject)
		{
			if(m_bMessage)
			{
				if(bShow && m_bShowRef) m_pProject->AddMouseMessageObject(this);
				else m_pProject->SubMouseMessageObject(this);
			}

			if(m_bUseLogic)
			{
				if(bShow && m_bShowRef) m_pProject->AddLogicObject(this);
				else m_pProject->SubLogicObject(this);
			}
		}
	}

	SetChildShowRef();
}

void CObjectBase::SetEnableRef(const bool &bEnableRef)
{
	if(m_bEnableRef == bEnableRef) return;

	m_bEnableRef = bEnableRef;
	SetChildEnableRef();
}

void CObjectBase::SetChildEnableRef(void)
{
	bool bEnableRef = m_bEnableRef && m_bEnable;
	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->SetEnableRef(bEnableRef);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->SetEnableRef(bEnableRef);
	}
}

void CObjectBase::SetEnable(const bool &bEnable)
{
	if(m_bEnable == bEnable) return;

	m_bEnable = bEnable;
	SetChildEnableRef();
}

void CObjectBase::CheckEventDraw(void)
{
	if(!m_bEventDraw) return;
	CObjectMan *pObjectMan = m_pProject?m_pProject->GetObjectMan():NULL;
	if(pObjectMan) pObjectMan->SetEventDraw();
}

void CObjectBase::CheckEventObjectDraw(void)
{
	if(!m_bEventDraw) return;
	if(m_bSendEventDraw) return;

	m_bSendEventDraw = true;
	CObjectMan *pObjectMan = m_pProject?m_pProject->GetObjectMan():NULL;
	CWnd *pWnd = pObjectMan?pObjectMan->GetWnd():NULL;
	if(pWnd) pWnd->PostMessage(LSY_UM_EVENT_DRAW, NULL, (LPARAM)this);
}

void CObjectBase::SetChange(const bool &bChange)
{
	if(m_bChange == bChange) return;

	m_bChange = bChange;

	if(m_bChange)
	{
		CheckEventDraw();

		LISTOBJECT::iterator start = m_listLocalChild.begin();
		LISTOBJECT::iterator end = m_listLocalChild.end();

		for(;start != end;++start)
		{
			(*start)->SetChange(true);
		}

		start = m_listChild.begin();
		end = m_listChild.end();

		for(;start != end;++start)
		{
			(*start)->SetChange(true);
		}
	}
}

void CObjectBase::SetChangeAlpha(const bool &bChangeAlpha)
{
	if(m_bChangeAlpha == bChangeAlpha) return;

	m_bChangeAlpha = bChangeAlpha;
	if(m_bChangeAlpha)
	{
		CheckEventDraw();

		LISTOBJECT::iterator start = m_listLocalChild.begin();
		LISTOBJECT::iterator end = m_listLocalChild.end();

		for(;start != end;++start)
		{
			(*start)->SetChangeAlpha(true);
		}

		start = m_listChild.begin();
		end = m_listChild.end();

		for(;start != end;++start)
		{
			(*start)->SetChangeAlpha(true);
		}
	}
}

const float CObjectBase::GetWidth(void)
{
	return m_rtLocalRect.GetWidth();
}

const float CObjectBase::GetHeight(void)
{
	return m_rtLocalRect.GetHeight();
}

bool CObjectBase::IsIn(IN CPoint &ptPos, OUT bool &bIn)
{
	CPoint ptTemp = ptPos;

	DEF_CHECK_CHANGE;
	if(m_pClipRect && !m_pClipRect->PtInRect(ptPos))
	{
		return false;
	}

	if(m_fRotation != 0.0f)
	{
		CPoint ptPivot = m_ptPos;

		ptTemp.x = ptPivot.x + m_fCos * (ptPos.x - ptPivot.x) + m_fSin * (ptPos.y - ptPivot.y);
		ptTemp.y = ptPivot.y - m_fSin * (ptPos.x - ptPivot.x) + m_fCos * (ptPos.y - ptPivot.y);		
	}

	bIn = m_rtRect.PtInRect(ptTemp);
	if(m_bEnable && m_bEnableRef && bIn)
	{
		return true;
	}
	return false;
}

void CObjectBase::SetLocalPos(const CPoint &ptPos)
{
	if(m_ptLocalPos == ptPos) return;

	m_ptLocalPos = ptPos;
	SetChange(true);
}

void CObjectBase::SetLocalPos(const float &fX, const float &fY)
{
	SetLocalPos(CPoint(fX, fY));
}

const CPoint &CObjectBase::GetLocalPos(void)
{
	return m_ptLocalPos;
}

void CObjectBase::SetLocalScaleX(const float &fScaleX)
{
	if(m_fLocalScaleX == fScaleX) return;
	m_fLocalScaleX = fScaleX;
	SetChange(true);
}

void CObjectBase::SetLocalScaleY(const float &fScaleY)
{
	if(m_fLocalScaleY == fScaleY) return;
	m_fLocalScaleY = fScaleY;
	SetChange(true);
}

void CObjectBase::SetLocalScale(const float &fScale)
{
	if(m_fLocalScaleX == fScale && m_fLocalScaleY == fScale) return;
	m_fLocalScaleX = m_fLocalScaleY = fScale;
	SetChange(true);
}

const float &CObjectBase::GetLocalScaleX(void)
{
	return m_fLocalScaleX;
}

const float &CObjectBase::GetLocalScaleY(void)
{
	return m_fLocalScaleY;
}

void CObjectBase::SetScaleX(const float &fScaleX)
{
	if(GetScaleX() == fScaleX) return;
	if(m_pParent && (m_pParent->GetScaleX() != 0.0f)) m_fLocalScaleX = fScaleX / m_pParent->GetScaleX();
	else m_fLocalScaleX = fScaleX;
	SetChange(true);
}

void CObjectBase::SetScaleY(const float &fScaleY)
{
	if(GetScaleY() == fScaleY) return;
	if(m_pParent && (m_pParent->GetScaleY() != 0.0f)) m_fLocalScaleY = fScaleY / m_pParent->GetScaleY();
	else m_fLocalScaleY = fScaleY;
	SetChange(true);
}

const float &CObjectBase::GetScaleX(void)
{
	DEF_CHECK_CHANGE;
	return m_fScaleX;
}

const float &CObjectBase::GetScaleY(void)
{
	DEF_CHECK_CHANGE;
	return m_fScaleY;
}

void CObjectBase::SetLocalRotation(const float &fRotation)
{
	if(m_fLocalRotation == fRotation) return;
	m_fLocalRotation = fRotation;
	SetChange(true);
}

const float &CObjectBase::GetLocalRotation(void)
{
	return m_fLocalRotation;
}

void CObjectBase::SetRotation(const float &fRotation)
{
	if(fRotation == GetRotation()) return;
	if(m_pParent) m_fLocalRotation = fRotation - m_pParent->GetRotation();
	else m_fLocalRotation = fRotation;
	SetChange(true);
}

const float &CObjectBase::GetRotation(void)
{
	DEF_CHECK_CHANGE;
	return m_fRotation;
}

void CObjectBase::SetPivot(const CPoint &ptPos)
{
	if(ptPos == m_ptPivot) return;
	m_ptPivot = ptPos;
	SetChange(true);
}

void CObjectBase::SetPivot(const float &fX, const float &fY)
{
	SetPivot(CPoint(fX, fY));
}

const CPoint &CObjectBase::GetPivot(void)
{
	return m_ptPivot;
}

void CObjectBase::SetMove(const CPoint &ptPos)
{
	if(ptPos == m_ptMove) return;
	m_ptMove = ptPos;
	SetChange(true);
}

void CObjectBase::SetMove(const float &fX, const float &fY)
{
	SetMove(CPoint(fX, fY));
}

const CPoint &CObjectBase::GetMove(void)
{
	DEF_CHECK_CHANGE;
	return m_ptMove;
}

void CObjectBase::SetLocalRect(const CRect &rtRect)
{
	if(rtRect == m_rtLocalRect) return;
	m_rtLocalRect = rtRect;
	SetChange(true);

	const CPoint &ptPoint = GetPos();
	const CPoint &ptPivot = GetPivot();
	const float &fScaleX = GetScaleX();
	const float &fScaleY = GetScaleY();

	m_rtRect.left = m_ptPos.x - ptPivot.x * fScaleX;
	m_rtRect.top = m_ptPos.y - ptPivot.y * fScaleY;
	m_rtRect.right = m_rtRect.left + m_rtLocalRect.GetWidth() * fScaleX;
	m_rtRect.bottom = m_rtRect.top + m_rtLocalRect.GetHeight() * fScaleY;
}

CRect &CObjectBase::GetLocalRect(void)
{
	return m_rtLocalRect;
}

CRect &CObjectBase::GetRect(void)
{
	DEF_CHECK_CHANGE;
	return m_rtRect;
}

void CObjectBase::SetXFlip(const bool &bXFlip)
{
	if(m_bXFlip == bXFlip) return;
	m_bXFlip = bXFlip;
	SetChildXFlipRef();
	SetChange(true);
}

void CObjectBase::SetXFlipRef(const bool &bXFlipRef)
{
	if(m_bXFlipRef == bXFlipRef) return;
	m_bXFlipRef = bXFlipRef;
	SetChildXFlipRef();
	SetChange(true);
}

void CObjectBase::SetChildXFlipRef(void)
{
	bool bXFlipRef = false;

	if((m_bXFlipRef && !m_bXFlip) || (!m_bXFlipRef && m_bXFlip))
		bXFlipRef = true;

	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->SetXFlipRef(bXFlipRef);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->SetXFlipRef(bXFlipRef);
	}
}

const bool &CObjectBase::IsXFlip(void)
{
	return m_bXFlip;
}

void CObjectBase::SetYFlip(const bool &bYFlip)
{
	if(m_bYFlip == bYFlip) return;
	m_bYFlip = bYFlip;
	SetChildYFlipRef();
	SetChange(true);
}

void CObjectBase::SetYFlipRef(const bool &bYFlipRef)
{
	if(m_bYFlipRef == bYFlipRef) return;
	m_bYFlipRef = bYFlipRef;
	SetChildYFlipRef();
	SetChange(true);
}

void CObjectBase::SetChildYFlipRef(void)
{
	bool bYFlipRef = false;

	if((m_bYFlipRef && !m_bYFlip) || (!m_bYFlipRef && m_bYFlip))
		bYFlipRef = true;

	LISTOBJECT::iterator start = m_listLocalChild.begin();
	LISTOBJECT::iterator end = m_listLocalChild.end();

	for(;start != end;++start)
	{
		(*start)->SetYFlipRef(bYFlipRef);
	}

	start = m_listChild.begin();
	end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->SetYFlipRef(bYFlipRef);
	}
}

const bool &CObjectBase::IsYFlip(void)
{
	return m_bYFlip;
}

void CObjectBase::SetMessage(const bool &bMessage)
{
	if(m_bMessage == bMessage) return;
	m_bMessage = bMessage;

	if(m_pProject && m_bShow && m_bShowRef)
	{
		if(m_bMessage) m_pProject->AddMouseMessageObject(this);
		else m_pProject->SubMouseMessageObject(this);
	}
}

const bool &CObjectBase::IsMessage(void)
{
	return m_bMessage;
}

void CObjectBase::SetUseLogic(const bool &bUseLogic)
{
	if(m_bUseLogic == bUseLogic) return;
	m_bUseLogic = bUseLogic;

	if(m_pProject && m_bShow && m_bShowRef)
	{
		if(m_bUseLogic) m_pProject->AddLogicObject(this);
		else m_pProject->SubLogicObject(this);
	}
}

const bool &CObjectBase::IsUseLogic(void)
{
	return m_bUseLogic;
}

bool CObjectBase::CheckChange(void)
{
	if(m_pObjectRect)
		m_pObjectRect->SetOserverChange(true);

	bool bXFlip = false;
	if((m_bXFlip && !m_bXFlipRef) || (!m_bXFlip && m_bXFlipRef)) bXFlip = true;

	bool bYFlip = false;
	if((m_bYFlip && !m_bYFlipRef) || (!m_bYFlip && m_bYFlipRef)) bYFlip = true;

	if(m_pParent && !m_bIgnoreParentPos)
	{
		m_bDraw = false;

		const CPoint &ptParent = m_pParent->GetPos();
		const CPoint &ptParentPivot = m_pParent->GetPivot();
		const float &fParentRotation = m_pParent->GetRotation();
		const float &fParentScaleX = m_pParent->GetScaleX();
		const float &fParentScaleY = m_pParent->GetScaleY();

		m_fScaleX = m_fLocalScaleX * fParentScaleX;
		m_fScaleY = m_fLocalScaleY * fParentScaleY;
		m_fRotation = m_fLocalRotation + fParentRotation;

		CPoint ptLocal = m_ptLocalPos - ptParentPivot + m_ptMove;
		if(m_bXFlipRef) ptLocal.x *= -1.0f;
		if(m_bYFlipRef) ptLocal.y *= -1.0f;

		if(m_fRotation == 0.0f)
		{
			m_fCos = 1.0f;
			m_fSin = 0.0f;
		}
		else
		{
			m_fCos = _cos(m_fRotation);
			m_fSin = _sin(m_fRotation);
		}

		if(fParentRotation == 0.0f)
		{
			m_ptPos.x = ptParent.x + ptLocal.x * fParentScaleX;
			m_ptPos.y = ptParent.y + ptLocal.y * fParentScaleY;
		}
		else
		{
			float fCos = m_pParent->GetCos();
			float fSin = m_pParent->GetSin();

			m_ptPos.x = ptParent.x + ptLocal.x * fParentScaleX * fCos - ptLocal.y * fParentScaleY * fSin;
			m_ptPos.y = ptParent.y + ptLocal.x * fParentScaleX * fSin + ptLocal.y * fParentScaleY * fCos;
		}
	}
	else
	{
		m_ptPos = m_ptLocalPos + m_ptMove;
		m_fScaleX = m_fLocalScaleX;
		m_fScaleY = m_fLocalScaleY;
		m_fRotation = m_fLocalRotation;
	}

	DWORD dwTemp = ~(LR | UD);
	m_dwDrawOption &= dwTemp;
	if(bXFlip)
	{
		m_rtRect.right = m_ptPos.x + m_ptPivot.x * m_fScaleX;
		m_rtRect.left = m_rtRect.right - m_rtLocalRect.GetWidth() * m_fScaleX;
		m_dwDrawOption |= LR;
	}
	else
	{
		m_rtRect.left = m_ptPos.x - m_ptPivot.x * m_fScaleX;
		m_rtRect.right = m_rtRect.left + m_rtLocalRect.GetWidth() * m_fScaleX;
	}

	if(bYFlip)
	{
		m_rtRect.bottom = m_ptPos.y + m_ptPivot.y * m_fScaleY;
		m_rtRect.top = m_rtRect.bottom - m_rtLocalRect.GetHeight() * m_fScaleY;
		m_dwDrawOption |= UD;
	}
	else
	{
		m_rtRect.top = m_ptPos.y - m_ptPivot.y * m_fScaleY;
		m_rtRect.bottom = m_rtRect.top + m_rtLocalRect.GetHeight() * m_fScaleY;
	}

	m_rtBound = m_rtRect.GetBound(m_fRotation);

	m_ptDraw = CPoint(m_rtRect.left, m_rtRect.top);
	float fTemp = m_fRotation / 1.57079632f;
	if(fTemp == (float)((int)fTemp)) m_fDrawRotaion = -90.0f * fTemp;
	else m_fDrawRotaion = m_fRotation * _PI_180D * -1.0f;

	if(m_fRotation != 0.0f)
	{
		if(m_fScaleX != 1.0f || m_fScaleY != 1.0f) m_nDrawType = EODT_SCALEROTATION;
		else m_nDrawType = EODT_ROTATION;

		if(m_ptPivot.x != 0.0f || m_ptPivot.y != 0.0f)
		{
			m_ptDraw.x = m_ptDraw.x + m_ptPivot.x * m_fScaleX - (m_ptPivot.x * m_fScaleX * m_fCos - m_ptPivot.y * m_fScaleY * m_fSin);
			m_ptDraw.y = m_ptDraw.y + m_ptPivot.y * m_fScaleY - (m_ptPivot.x * m_fScaleX * m_fSin + m_ptPivot.y * m_fScaleY * m_fCos);
		}
	}
	else if(m_fScaleX != 1.0f || m_fScaleY != 1.0f) m_nDrawType = EODT_SCALE;
	else m_nDrawType = EODT_NORMAL;

	if(m_pClipRect) delete m_pClipRect;
	m_pClipRect = NULL;
	m_bClipDraw = true;

	if((m_pParent == NULL) || (m_pParent && m_pParent->IsClipDraw()) || m_bIgnoreParentClip)
	{
		CRect *pClipParent = NULL;
		CRect rtClipLocal;

		if(m_pParent) pClipParent = m_pParent->GetClipRect();
		if(m_pClipLocal)
		{
			if(m_pParent && m_bLocalClipUseParentPos) rtClipLocal = *m_pClipLocal + m_pParent->GetPos();
			else rtClipLocal = *m_pClipLocal + m_ptPos;
		}

		if(m_bIgnoreParentClip)
		{
			if(m_pClipLocal)
			{
				m_pClipRect = new CRect(rtClipLocal);
			}
		}
		else 
		{
			if((pClipParent == NULL) && (m_pClipLocal == NULL))
			{
			}
			else if(pClipParent && m_pClipLocal)
			{
				m_pClipRect = new CRect;
				m_bClipDraw = m_pClipRect->IntersectRect(pClipParent, &rtClipLocal);
			}
			else if(m_pClipLocal)
			{
				m_pClipRect = new CRect(rtClipLocal);
			}
			else
			{
				m_pClipRect = new CRect(*pClipParent);
			}
		}
	}
	else
	{
		m_bClipDraw = false;
	}

	if(m_bClipDraw == false)
	{
	}
	else if(m_pClipRect)
	{
		CRect rtRect;
		m_bDraw = rtRect.IntersectRect(m_pClipRect, &m_rtBound);
	}
	else if(m_pProject)
	{
		const CSize &szSize = m_pProject->GetObjectMan()->GetSize();
		CRect rtWindow(0, 0, (float)szSize.cx, (float)szSize.cy);
		CRect rtRect;
		m_bDraw = rtRect.IntersectRect(&rtWindow, &m_rtBound);
	}

	SetChange(false);
	return true;
}

bool CObjectBase::CheckChangeAlpha(void)
{
	if(m_pParent)
	{
		const float &fAlpha = m_pParent->GetAlpha();
		m_fAlpha = m_fLocalAlpha * fAlpha;
	}
	else
	{
		m_fAlpha = m_fLocalAlpha;
	}

	m_nAlpha = (int)((float)31 * m_fAlpha);

	if(m_fAlpha == 1.0f) m_dwDrawOption &= ~((DWORD)GB);
	else if(!(m_dwDrawOption & GB || m_dwDrawOption & LE || m_dwDrawOption & DE || m_dwDrawOption & AB))
	{
		m_dwDrawOption |= GB;
	}

	SetChangeAlpha(false);
	return true;
}

void CObjectBase::SetEffect(ENUM_OBJECTEFFECTTYPE nEffect)
{
	if(m_nEffectType == nEffect) return;
	m_nEffectType = nEffect;
	m_dwDrawOption &= ~(0x03fc);
	DWORD dwTemp = 0;
	switch(m_nEffectType)
	{
	case EOET_NORMAL:
		dwTemp = 0;
		break;
	case EOET_HALFALPHA:
		dwTemp = HB;
		break;
	case EOET_STEPALPHA:
		dwTemp = GB;
		break;
	case EOET_GLOW:
		dwTemp = GLOW;
		break;
	case EOET_GLAYSCALE:
		dwTemp = GRAYSCL;
		break;
	case EOET_MONO:
		dwTemp = MONO;
		break;
	case EOET_ANTI:
		dwTemp = ANTI;
		break;
	case EOET_LIGHT:
		dwTemp = LE;
		break;
	case EOET_DARKEN:
		dwTemp = DE;
		break;
	case EOET_ALPHA:
		dwTemp = AB;
		break;
	}
	m_dwDrawOption |= dwTemp;
}

//Alpha 관련 함수들, 값은 0~1 사이 값이다
void CObjectBase::SetLocalAlpha(const float &fAlpha)
{
	if(m_fLocalAlpha == fAlpha) return;
	m_fLocalAlpha = fAlpha;
	SetChangeAlpha(true);
}

const float &CObjectBase::GetLocalAlpha(void)
{
	return m_fLocalAlpha;
}

const float &CObjectBase::GetAlpha(void)
{
	DEF_CHECK_CHANGE_ALPHA;
	return m_fAlpha;
}

void CObjectBase::SetPos(const CPoint &ptPos)
{
	if(ptPos == GetPos()) return;

	if(m_pParent && !m_bIgnoreParentPos)
	{
		const CPoint &ptParent = m_pParent->GetPos();
		const CPoint &ptParentPivot = m_pParent->GetPivot();
		const float &fParentRotation = m_pParent->GetRotation();
		const float &fParentScaleX = m_pParent->GetScaleX();
		const float &fParentScaleY = m_pParent->GetScaleY();

		CPoint ptTemp = ptPos - ptParent;

		if(fParentRotation == 0.0f)
		{
			m_ptLocalPos = ptTemp;
		}
		else
		{
			float fCos = m_pParent->GetCos();
			float fSin = m_pParent->GetSin();

			m_ptLocalPos.x = ptTemp.x * fCos + ptTemp.y * fSin;
			m_ptLocalPos.y = ptTemp.y * fCos - ptTemp.x * fSin;
		}

		m_ptLocalPos.x /= fParentScaleX;
		m_ptLocalPos.y /= fParentScaleY;

		m_ptLocalPos = m_ptLocalPos + ptParentPivot - m_ptMove;
	}
	else
	{
		m_ptLocalPos = ptPos - m_ptMove;
	}
	SetChange(true);
}

CPoint CObjectBase::GetGlobalToLocalPos(const CPoint &ptPos)
{
	DEF_CHECK_CHANGE;
	CPoint ptLocal = ptPos - m_ptPos;

	if(m_fRotation != 0.0f)
	{
		CPoint ptTemp;
		ptTemp.x = ptLocal.x * m_fCos + ptLocal.y * m_fSin;
		ptTemp.y = ptLocal.y * m_fCos - ptLocal.x * m_fSin;
		ptLocal = ptTemp;
	}

	ptLocal.x /= m_fScaleX;
	ptLocal.y /= m_fScaleY;

	return ptLocal;
}

CPoint CObjectBase::GetLocalToGlobalPos(const CPoint &ptPos)
{
	DEF_CHECK_CHANGE;
	CPoint ptGlobal = ptPos;

	if(m_fRotation != 0.0f)
	{
		ptGlobal.x = m_ptPos.x + ptPos.x * m_fScaleX * m_fCos - ptPos.y * m_fScaleY * m_fSin;
		ptGlobal.y = m_ptPos.y + ptPos.x * m_fScaleX * m_fSin + ptPos.y * m_fScaleY * m_fCos;
	}
	else
	{
		ptGlobal.x = m_ptPos.x + ptPos.x * m_fScaleX;
		ptGlobal.y = m_ptPos.y + ptPos.y * m_fScaleY;
	}

	return ptGlobal;
}

void CObjectBase::SetPos(const float &fX, const float &fY)
{
	SetPos(CPoint(fX, fY));
}

const CPoint &CObjectBase::GetPos(void)
{
	//상태값이 변경되어 위치를 새로 계산하여야 할때
	DEF_CHECK_CHANGE;
	return m_ptPos;
}

void CObjectBase::SetLocalClipRect(const CRect &rtClip)
{
	if((m_pClipLocal && (*m_pClipLocal == rtClip)) 
		|| ((m_pClipLocal == NULL) && (rtClip == CRectZero))) return;

	if(rtClip != CRectZero)
	{
		if(m_pClipLocal)
		{
			*m_pClipLocal = rtClip;	
		}
		else
		{
			m_pClipLocal = new CRect(rtClip);
		}
	}
	else
	{
		if(m_pClipLocal) delete m_pClipLocal;
		m_pClipLocal = NULL;
	}

	SetChange(true);
}

void CObjectBase::SetLocalClipUseParentPos(const bool &bLocalClipUseParentPos)
{
	if(m_bLocalClipUseParentPos == bLocalClipUseParentPos) return;
	m_bLocalClipUseParentPos = bLocalClipUseParentPos;
	SetChange(true);
}

void CObjectBase::SetGlobalClipRect(const CRect &rtClip)
{
	if((m_pClipLocal == NULL) && (rtClip == CRectZero)) return;
	DEF_CHECK_CHANGE;
	SetLocalClipRect(rtClip - m_ptPos);
}

CRect *CObjectBase::GetClipRect(void)
{
	DEF_CHECK_CHANGE;
	return m_pClipRect;
}

const bool &CObjectBase::IsClipDraw(void)
{
	DEF_CHECK_CHANGE;
	return m_bClipDraw;
}

void CObjectBase::OnSetParent(CObjectBase *pParent, bool bSet)
{
}

size_t CObjectBase::GetClipSize(int &nVersion)
{
	size_t nRet = sizeof(ENUM_OBJECTTYPE);	//Object 종류
	nRet += sizeof(size_t);					//Object 이름의 크기
	nRet += m_strName.size();				//Object 이름
	nRet += sizeof(int);					//m_nID
	nRet += sizeof(CPoint);					//m_ptLocalPos
	nRet += sizeof(CPoint);					//m_ptPivot
	nRet += sizeof(float);					//m_fLocalRotation
	nRet += sizeof(float);					//m_fLocalScaleX
	nRet += sizeof(float);					//m_fLocalScaleY
	nRet += sizeof(float);					//m_fLocalAlpha
	nRet += sizeof(int);					//m_nColor
	nRet += sizeof(CRect);					//m_rtLocalRect
	nRet += sizeof(bool);					//m_bMessage
	nRet += sizeof(bool);					//m_bShow
	nRet += sizeof(bool);					//m_bEnable
	nRet += sizeof(ENUM_OBJECTEFFECTTYPE);	//m_nEffectType
	nRet += sizeof(bool);					//m_bXFlip
	nRet += sizeof(bool);					//m_bYFlip
	nRet += sizeof(bool);					//m_bFixed
	nRet += sizeof(bool);					//m_bDynamic
	nRet += sizeof(bool);					//m_bUseMouseMove
	nRet += sizeof(bool);					//m_bUseMouseWheel
	nRet += sizeof(bool);					//m_bLocalClipUseParentPos
	nRet += sizeof(bool);					//m_bUseLogic
	nRet += sizeof(bool);					//m_bExpand
	nRet += sizeof(bool);					//m_bIgnoreParentClip
	nRet += sizeof(bool);					//m_bIgnoreParentPos
	nRet += sizeof(bool);					//bUseClipLocal
	if(m_pClipLocal)
	{
		nRet += sizeof(CRect);				//m_pClipLocal
	}

	size_t nSize = m_listUserData.size();
	nRet += sizeof(size_t);					//m_listUserData의 크기
	nRet += nSize * sizeof(int);
	nRet += nSize * sizeof(DWORD_PTR);

	nRet += GetInheritanceClipSize(nVersion);

	nRet += sizeof(size_t);					//Child의 갯수
	LISTOBJECT::iterator start = m_listChild.begin();
	LISTOBJECT::iterator end = m_listChild.end();

	for(;start != end;++start)
	{
		nRet += (*start)->GetClipSize(nVersion);
	}

	return nRet;
}

size_t CObjectBase::SaveClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = 0;
	ENUM_OBJECTTYPE nType = GetType();
	memcpy(pDest+nRet, &nType, sizeof(ENUM_OBJECTTYPE));
	nRet += sizeof(ENUM_OBJECTTYPE);		//Object 종류

	size_t nSize = m_strName.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);
	if(nSize)
	{
		memcpy(pDest+nRet, m_strName.c_str(), nSize);
		nRet += nSize;
	}

	memcpy(pDest+nRet, &m_nID, sizeof(int));
	nRet += sizeof(int);

	memcpy(pDest+nRet, &m_ptLocalPos, sizeof(CPoint));
	nRet += sizeof(CPoint);					//m_ptLocalPos
	memcpy(pDest+nRet, &m_ptPivot, sizeof(CPoint));
	nRet += sizeof(CPoint);					//m_ptPivot
	memcpy(pDest+nRet, &m_fLocalRotation, sizeof(float));
	nRet += sizeof(float);					//m_fLocalRotation
	memcpy(pDest+nRet, &m_fLocalScaleX, sizeof(float));
	nRet += sizeof(float);					//m_fLocalScaleX
	memcpy(pDest+nRet, &m_fLocalScaleY, sizeof(float));
	nRet += sizeof(float);					//m_fLocalScaleY
	memcpy(pDest+nRet, &m_fLocalAlpha, sizeof(float));
	nRet += sizeof(float);					//m_fLocalAlpha
	memcpy(pDest+nRet, &m_nColor, sizeof(int));
	nRet += sizeof(int);					//m_nColor
	memcpy(pDest+nRet, &m_rtLocalRect, sizeof(CRect));
	nRet += sizeof(CRect);					//m_rtLocalRect
	memcpy(pDest+nRet, &m_bMessage, sizeof(bool));
	nRet += sizeof(bool);					//m_bMessage
	memcpy(pDest+nRet, &m_bShow, sizeof(bool));
	nRet += sizeof(bool);					//m_bShow
	memcpy(pDest+nRet, &m_bEnable, sizeof(bool));
	nRet += sizeof(bool);					//m_bEnable
	memcpy(pDest+nRet, &m_nEffectType, sizeof(ENUM_OBJECTEFFECTTYPE));
	nRet += sizeof(ENUM_OBJECTEFFECTTYPE);	//m_nEffectType
	memcpy(pDest+nRet, &m_bXFlip, sizeof(bool));
	nRet += sizeof(bool);					//m_bXFlip
	memcpy(pDest+nRet, &m_bYFlip, sizeof(bool));
	nRet += sizeof(bool);					//m_bYFlip
	memcpy(pDest+nRet, &m_bFixed, sizeof(bool));
	nRet += sizeof(bool);					//m_bFixed
	memcpy(pDest+nRet, &m_bDynamic, sizeof(bool));
	nRet += sizeof(bool);					//m_bDynamic
	memcpy(pDest+nRet, &m_bMouseMove, sizeof(bool));
	nRet += sizeof(bool);					//m_bUseMouseMove
	memcpy(pDest+nRet, &m_bUseMouseWheel, sizeof(bool));
	nRet += sizeof(bool);					//m_bUseMouseWheel
	memcpy(pDest+nRet, &m_bLocalClipUseParentPos, sizeof(bool));
	nRet += sizeof(bool);					//m_bLocalClipUseParentPos
	memcpy(pDest+nRet, &m_bUseLogic, sizeof(bool));
	nRet += sizeof(bool);					//m_bUseLogic
	memcpy(pDest+nRet, &m_bExpand, sizeof(bool));
	nRet += sizeof(bool);					//m_bExpand
	memcpy(pDest+nRet, &m_bIgnoreParentClip, sizeof(bool));
	nRet += sizeof(bool);					//m_bIgnoreParentClip
	memcpy(pDest+nRet, &m_bIgnoreParentPos, sizeof(bool));
	nRet += sizeof(bool);					//m_bIgnoreParentPos

	bool bUseClipLocal = m_pClipLocal?true:false;
	memcpy(pDest+nRet, &bUseClipLocal, sizeof(bool));
	nRet += sizeof(bool);					//bUseClipLocal
	if(m_pClipLocal)
	{
		memcpy(pDest+nRet, m_pClipLocal, sizeof(CRect));
		nRet += sizeof(CRect);				//m_pClipLocal
	}

	nSize = m_listUserData.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);

	std::map<int, DWORD_PTR>::iterator start_ud = m_listUserData.begin();
	std::map<int, DWORD_PTR>::iterator end_ud = m_listUserData.end();
	for(;start_ud != end_ud;++start_ud)
	{
		memcpy(pDest+nRet, &(start_ud->first), sizeof(int));
		nRet += sizeof(int);
		memcpy(pDest+nRet, &(start_ud->second), sizeof(DWORD_PTR));
		nRet += sizeof(DWORD_PTR);
	}

	nRet += SaveInheritanceClipSize(nVersion, pDest+nRet);

	nSize = m_listChild.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);					//Child의 갯수

	LISTOBJECT::iterator start = m_listChild.begin();
	LISTOBJECT::iterator end = m_listChild.end();
	for(;start != end;++start)
	{
		nRet += (*start)->SaveClipSize(nVersion, pDest+nRet);
	}

	return nRet;
}

size_t CObjectBase::LoadClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = 0;
	size_t nSize = 0;
	bool bSet = false;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strName);
		nRet += nSize;
	}
	memcpy(&m_nID, pDest+nRet, sizeof(int));
	nRet += sizeof(int);
	memcpy(&m_ptLocalPos, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);					//m_ptLocalPos
	memcpy(&m_ptPivot, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);					//m_ptPivot
	memcpy(&m_fLocalRotation, pDest+nRet, sizeof(float));
	nRet += sizeof(float);					//m_fLocalRotation
	memcpy(&m_fLocalScaleX, pDest+nRet, sizeof(float));
	nRet += sizeof(float);					//m_fLocalScaleX
	memcpy(&m_fLocalScaleY, pDest+nRet, sizeof(float));
	nRet += sizeof(float);					//m_fLocalScaleY
	memcpy(&m_fLocalAlpha, pDest+nRet, sizeof(float));
	nRet += sizeof(float);					//m_fLocalAlpha
	memcpy(&m_nColor, pDest+nRet, sizeof(int));
	nRet += sizeof(int);					//m_nColor
	memcpy(&m_rtLocalRect, pDest+nRet, sizeof(CRect));
	nRet += sizeof(CRect);					//m_rtLocalRect
	memcpy(&bSet, pDest+nRet, sizeof(bool));
	SetMessage(bSet);
	nRet += sizeof(bool);					//m_bMessage
	memcpy(&bSet, pDest+nRet, sizeof(bool));
	SetShow(bSet);
	nRet += sizeof(bool);					//m_bShow
	memcpy(&bSet, pDest+nRet, sizeof(bool));
	SetEnable(bSet);
	nRet += sizeof(bool);					//m_bEnable
	memcpy(&m_nEffectType, pDest+nRet, sizeof(ENUM_OBJECTEFFECTTYPE));
	nRet += sizeof(ENUM_OBJECTEFFECTTYPE);	//m_nEffectType
	memcpy(&m_bXFlip, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bXFlip
	memcpy(&m_bYFlip, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bYFlip
	memcpy(&m_bFixed, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bFixed
	memcpy(&m_bDynamic, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bDynamic
	memcpy(&m_bMouseMove, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bUseMouseMove
	memcpy(&bSet, pDest+nRet, sizeof(bool));
	SetUseMouseWheel(bSet);
	nRet += sizeof(bool);					//m_bUseMouseWheel
	memcpy(&m_bLocalClipUseParentPos, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bLocalClipUseParentPos
	memcpy(&bSet, pDest+nRet, sizeof(bool));
	SetUseLogic(bSet);
	nRet += sizeof(bool);					//m_bUseLogic
	memcpy(&m_bExpand, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bExpand
	memcpy(&m_bIgnoreParentClip, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bIgnoreParentClip
	memcpy(&m_bIgnoreParentPos, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bIgnoreParentPos

	bool bUseClipLocal = false;
	memcpy(&bUseClipLocal, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//bUseClipLocal
	if(bUseClipLocal)
	{
		if(m_pClipLocal == NULL) m_pClipLocal = new CRect;
		memcpy(m_pClipLocal, pDest+nRet, sizeof(CRect));
		nRet += sizeof(CRect);				//m_pClipLocal
	}

	nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);					//Child의 갯수
	int nUDType = 0;
	DWORD_PTR dwUDData = 0;
	m_listUserData.clear();
	for(int i=0;i<(int)nSize;++i)
	{
		memcpy(&nUDType, pDest+nRet, sizeof(int));
		nRet += sizeof(int);
		memcpy(&dwUDData, pDest+nRet, sizeof(DWORD_PTR));
		nRet += sizeof(DWORD_PTR);
		m_listUserData[nUDType] = dwUDData;
	}

	nRet += LoadInheritanceClipSize(nVersion, pDest+nRet, pProject, bSetID);

	nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);					//Child의 갯수

	ENUM_OBJECTTYPE nObjectType = EOT_UNKNOWN;
	CObjectBase *pObject = NULL;

	for(int i=0;i<(int)nSize;++i)
	{
		memcpy(&nObjectType, pDest+nRet, sizeof(ENUM_OBJECTTYPE));
		nRet += sizeof(ENUM_OBJECTTYPE);
		pObject = CreateNewObject(nObjectType);
		nRet += pObject->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);

		AddChild(pObject);

		pObject->OnCreated();
	}

	if(bSetID)
	{
		if(pProject->IsExistID(m_nID)) SetID(pProject->MakeID());
		SetName(pProject->MakeName(m_strName));
		pProject->AddObject(this);
	}
	return nRet;
}

CObjectBase *CObjectBase::Clone(const bool &bChildClone/* = false*/, const bool &bUseOriginalObjectFunctor/* = false*/, const bool &bCloneObserverChild/* = false*/, CObjectBase *pObserverObject/* = NULL*/, const bool &bFirst/* = true*/)
{
	CObjectBase *pObject = NewObject();

	//bUseOriginalObjectFunctor는 원본에 있는 펑터를 사용해서 복사복의 메시지를 전달한다
	if(bUseOriginalObjectFunctor) pObject->m_pOriginalObject = this;
	//복사되어지는 Object의 모든 메시지를 감시하는 Observer Object를 사용한다.
	if(bCloneObserverChild)
	{
		//옵저버가 없다면 자기 자신을 옵저버로 복사되는 하위 Object에 등록한다.
		if(pObserverObject == NULL) pObserverObject = pObject;
		//옵저버가 잇다면 옵저버를 등록한다.
		else pObject->m_pObserverObject = pObserverObject;
	}

	Copy(pObject);

	if(bChildClone)
	{
		pObject->OnCreateChildStart();
		LISTOBJECT::iterator start = m_listChild.begin();
		LISTOBJECT::iterator end = m_listChild.end();

		CObjectBase *pCloneChild = NULL;
		for(;start != end;++start)
		{
			pCloneChild = (*start)->Clone(true, bUseOriginalObjectFunctor, bCloneObserverChild, pObserverObject, false);
			pObject->AddChild(pCloneChild);
			pObject->OnCreateChild(pCloneChild);
			pCloneChild->OnCreated();
		}
		pObject->OnCreateChildEnd();
	}

	if(bFirst) pObject->OnCreated();

	return pObject;
}

bool CObjectBase::SaveXml(TiXmlNode *pParentNode)
{
	TiXmlElement xmlObject("Object");

	std::string strTemp;
	GetObjectTypeToString(GetType(), strTemp);

	//기본 데이터 저장
	xmlObject.SetAttribute("Type", strTemp.c_str());
	xmlObject.SetAttribute("Name", m_strName.c_str());
	xmlObject.SetAttribute("ID", m_nID);

	//로컬 데이터 저장
	TiXmlElement xmlLocal("Local");
	xmlLocal.SetDoubleAttribute("PosX", m_ptLocalPos.x);
	xmlLocal.SetDoubleAttribute("PosY", m_ptLocalPos.y);
	xmlLocal.SetDoubleAttribute("PivotX", m_ptPivot.y);
	xmlLocal.SetDoubleAttribute("PivotY", m_ptPivot.y);
	xmlLocal.SetDoubleAttribute("Rotation", m_fLocalRotation);
	xmlLocal.SetDoubleAttribute("ScaleX", m_fLocalScaleX);
	xmlLocal.SetDoubleAttribute("ScaleY", m_fLocalScaleY);
	xmlLocal.SetDoubleAttribute("left", m_rtLocalRect.left);
	xmlLocal.SetDoubleAttribute("top", m_rtLocalRect.top);
	xmlLocal.SetDoubleAttribute("right", m_rtLocalRect.right);
	xmlLocal.SetDoubleAttribute("bottom", m_rtLocalRect.bottom);
	xmlLocal.SetAttribute("Ignore", m_bIgnoreParentPos?1:0);
	xmlObject.InsertEndChild(xmlLocal);

	//기본 상태 데이터 저장
	TiXmlElement xmlCondition("condition");

	bool bUse = GetUserData(OUDT_MESSAGE)?true:false;
	xmlCondition.SetAttribute("message", bUse?1:0);

	bUse = GetUserData(OUDT_ENABLE)?true:false;
	xmlCondition.SetAttribute("enable", bUse?1:0);

	bUse = GetUserData(OUDT_USEMOUSEWHEEL)?true:false;
	xmlCondition.SetAttribute("mousewheel", bUse?1:0);

	bUse = GetUserData(OUDT_USELOGIC)?true:false;
	xmlCondition.SetAttribute("logic", bUse?1:0);
	xmlCondition.SetAttribute("expand", m_bExpand?1:0);

	xmlCondition.SetAttribute("show", m_bShow?1:0);
	xmlCondition.SetAttribute("dynamic", m_bDynamic?1:0);
	xmlCondition.SetAttribute("mousemove", m_bMouseMove?1:0);
	xmlCondition.SetAttribute("localclipuseparentpos", m_bLocalClipUseParentPos?1:0);
	xmlCondition.SetAttribute("fixed", m_bFixed?1:0);
	xmlCondition.SetAttribute("xflip", m_bXFlip?1:0);
	xmlCondition.SetAttribute("yflip", m_bYFlip?1:0);
	xmlObject.InsertEndChild(xmlCondition);

	//이펙트 관련 데이터
	TiXmlElement xmlEffect("effect");
	GetEffectTypeToString(m_nEffectType, strTemp);
	xmlEffect.SetAttribute("name", strTemp.c_str());
	xmlEffect.SetDoubleAttribute("alpha", m_fLocalAlpha);
	xmlEffect.SetAttribute("color", m_nColor);
	xmlObject.InsertEndChild(xmlEffect);

	TiXmlElement xmlClipLocal("cliplocal");
	xmlClipLocal.SetAttribute("use", m_pClipLocal?1:0);
	xmlClipLocal.SetDoubleAttribute("left", m_pClipLocal?m_pClipLocal->left:0.0f);
	xmlClipLocal.SetDoubleAttribute("top", m_pClipLocal?m_pClipLocal->top:0.0f);
	xmlClipLocal.SetDoubleAttribute("right", m_pClipLocal?m_pClipLocal->right:0.0f);
	xmlClipLocal.SetDoubleAttribute("bottom", m_pClipLocal?m_pClipLocal->bottom:0.0f);
	xmlClipLocal.SetAttribute("Ignore", m_bIgnoreParentClip?1:0);
	xmlObject.InsertEndChild(xmlClipLocal);

	SaveInheritanceXml(&xmlObject);

	TiXmlElement xmlObjectList("ObjectList");

	LISTOBJECT::const_iterator start = m_listChild.begin();
	LISTOBJECT::const_iterator end = m_listChild.end();

	for(;start != end;++start)
	{
		(*start)->SaveXml(&xmlObjectList);
	}

	xmlObject.InsertEndChild(xmlObjectList);

	pParentNode->InsertEndChild(xmlObject);
	return true;
}

bool CObjectBase::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	return true;
}

bool CObjectBase::LoadXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	std::string strTemp;
	bool bSet = false;

	TiXmlElement *pXmlLocal = pXmlObject->FirstChildElement("Local");

	GetAttribute(pXmlLocal, "PosX", m_ptLocalPos.x);
	GetAttribute(pXmlLocal, "PosY", m_ptLocalPos.y);
	GetAttribute(pXmlLocal, "PivotX", m_ptPivot.x);
	GetAttribute(pXmlLocal, "PivotY", m_ptPivot.y);
	GetAttribute(pXmlLocal, "ScaleX", m_fLocalScaleX);
	GetAttribute(pXmlLocal, "ScaleY", m_fLocalScaleY);
	GetAttribute(pXmlLocal, "left", m_rtLocalRect.left);
	GetAttribute(pXmlLocal, "top", m_rtLocalRect.top);
	GetAttribute(pXmlLocal, "right", m_rtLocalRect.right);
	GetAttribute(pXmlLocal, "bottom", m_rtLocalRect.bottom);
	if(pXmlLocal->Attribute("Ignore")) GetAttribute(pXmlLocal, "Ignore", m_bIgnoreParentPos);

	TiXmlElement *pXmlCondition = pXmlObject->FirstChildElement("condition");
	if(IsTool())
	{
		GetAttribute(pXmlCondition, "message", bSet);
		SetUserData(OUDT_MESSAGE, (DWORD_PTR)(bSet?1:0));
		GetAttribute(pXmlCondition, "enable", bSet);
		SetUserData(OUDT_ENABLE, (DWORD_PTR)(bSet?1:0));
		GetAttribute(pXmlCondition, "mousewheel", bSet);
		SetUserData(OUDT_USEMOUSEWHEEL, (DWORD_PTR)(bSet?1:0));
		GetAttribute(pXmlCondition, "logic", bSet);
		SetUserData(OUDT_USELOGIC, (DWORD_PTR)(bSet?1:0));
	}
	else
	{
		GetAttribute(pXmlCondition, "message", bSet);
		SetMessage(bSet);
		GetAttribute(pXmlCondition, "enable", bSet);
		SetEnable(bSet);
		GetAttribute(pXmlCondition, "mousewheel", bSet);
		SetUseMouseWheel(bSet);
		GetAttribute(pXmlCondition, "logic", bSet);
		SetUseLogic(bSet);
	}

	if(pXmlCondition->Attribute("expand"))
		GetAttribute(pXmlCondition, "expand", m_bExpand);

	GetAttribute(pXmlCondition, "show", bSet);
	SetShow(bSet);
	GetAttribute(pXmlCondition, "dynamic", m_bDynamic);
	GetAttribute(pXmlCondition, "mousemove", m_bMouseMove);
	GetAttribute(pXmlCondition, "localclipuseparentpos", m_bLocalClipUseParentPos);
	GetAttribute(pXmlCondition, "fixed", m_bFixed);
	GetAttribute(pXmlCondition, "xflip", m_bXFlip);
	GetAttribute(pXmlCondition, "yflip", m_bYFlip);

	TiXmlElement *pXmlEffect = pXmlObject->FirstChildElement("effect");
	GetAttribute(pXmlEffect, "name", strTemp);
	m_nEffectType = GetStringToEffectType(strTemp);
	GetAttribute(pXmlEffect, "alpha", m_fLocalAlpha);
	GetAttribute(pXmlEffect, "color", m_nColor);

	TiXmlElement *pXmlClipLocal = pXmlObject->FirstChildElement("cliplocal");
	GetAttribute(pXmlClipLocal, "use", bSet);
	if(bSet)
	{
		if(m_pClipLocal == NULL) m_pClipLocal = new CRect;
		GetAttribute(pXmlClipLocal, "left", m_pClipLocal->left);
		GetAttribute(pXmlClipLocal, "top", m_pClipLocal->top);
		GetAttribute(pXmlClipLocal, "right", m_pClipLocal->right);
		GetAttribute(pXmlClipLocal, "bottom", m_pClipLocal->bottom);
	}
	if(pXmlClipLocal->Attribute("Ignore")) GetAttribute(pXmlClipLocal, "Ignore", m_bIgnoreParentClip);

	LoadInheritanceXml(nVersion, pXmlObject, pProject);

	CObjectBase *pChildObject = NULL;
	TiXmlElement *pObjectList = pXmlObject->FirstChildElement("ObjectList");

	TiXmlElement *pXmlChildObject = pObjectList->FirstChildElement("Object");
	OnCreateChildStart();
	while(pXmlChildObject)
	{
		pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
		AddChild(pChildObject, NULL, true);
		OnCreateChild(pChildObject);
		pChildObject->OnCreated();
		pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
	}
	OnCreateChildEnd();
	return true;
}

bool CObjectBase::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	return true;
}

CObjectBase *CObjectBase::NewObject(void)
{
	return new CObjectBase;
}

CObjectBase *CObjectBase::Copy(CObjectBase *pObject)
{
	ASSERT(pObject != NULL);
	//로컬값만 복사한다. 글로벌 값은 새로 연산하도록 놔둔다.
	pObject->m_ptLocalPos		= m_ptLocalPos;
	pObject->m_fLocalRotation	= m_fLocalRotation;
	pObject->m_fLocalScaleX		= m_fLocalScaleX;
	pObject->m_fLocalScaleY		= m_fLocalScaleY;
	pObject->SetLocalRect(m_rtLocalRect);
	pObject->m_fLocalAlpha		= m_fLocalAlpha;
	pObject->m_nColor			= m_nColor;

	//m_ptMove, m_cMessage, m_pParent는 복사하지 않는다.

	pObject->m_bMessage			= m_bMessage;
	pObject->m_bShow			= m_bShow;
	pObject->m_bEnable			= m_bEnable;
	pObject->m_nEffectType		= m_nEffectType;
	pObject->m_bXFlip			= m_bXFlip;
	pObject->m_bYFlip			= m_bYFlip;
	pObject->m_bFixed			= m_bFixed;
	pObject->m_bDynamic			= m_bDynamic;
	pObject->m_bMouseMove		= m_bMouseMove;
	pObject->m_bUseMouseWheel	= m_bUseMouseWheel;
	pObject->m_bUseLogic		= m_bUseLogic;
	pObject->m_bLocalClipUseParentPos = m_bLocalClipUseParentPos;
	pObject->m_listUserData		= m_listUserData;
	pObject->m_ptPivot			= m_ptPivot;

	pObject->m_bExpand			= m_bExpand;
	pObject->m_nID				= m_nID;
	pObject->m_strName			= m_strName;
	pObject->m_bIgnoreParentClip	= m_bIgnoreParentClip;
	pObject->m_bIgnoreParentPos		= m_bIgnoreParentPos;
	pObject->m_bEventDraw		= m_bEventDraw;

	if(m_pClipLocal) pObject->m_pClipLocal = new CRect(*m_pClipLocal);
	return pObject;
}

bool CObjectBase::IsParent(CObjectBase *pParent)
{
	ASSERT(pParent != NULL);
	if(pParent == this) return true;

	if(m_pParent)
	{
		if(m_pParent == pParent) return true;
		else return m_pParent->IsParent(pParent);
	}
	
	return false;
}

LSY::CObjectLocalData CObjectBase::GetLocalData(void)
{
	CObjectLocalData cData;
	cData.m_ptLocalPos		= m_ptLocalPos;
	cData.m_fLocalAlpha		= m_fLocalAlpha;
	cData.m_fLocalRotation	= m_fLocalRotation;
	cData.m_fLocalScaleX	= m_fLocalScaleX;
	cData.m_fLocalScaleY	= m_fLocalScaleY;
	return cData;
}

void CObjectBase::SetLocalData(CObjectLocalData &cData)
{
	SetLocalPos(cData.m_ptLocalPos);
	SetLocalAlpha(cData.m_fLocalAlpha);
	SetLocalRotation(cData.m_fLocalRotation);
	SetLocalScaleX(cData.m_fLocalScaleX);
	SetLocalScaleY(cData.m_fLocalScaleY);
}

bool CObjectBase::OnClosePopup(CObjectBase *pFocusObject)
{
	return false;
}

CObjectBase *CObjectBase::GetWindowsParent(void)
{
	switch(GetType())
	{
	case EOT_WINDOWS: 
	case EOT_SPRITEWINDOWS:
		return this;
	}
	if(m_pParent) return m_pParent->GetWindowsParent();
	return NULL;
}

void CObjectBase::SetUserData(int nID, DWORD_PTR dwData)
{
	m_listUserData[nID] = dwData;
}

DWORD_PTR CObjectBase::GetUserData(int nID)
{
	std::map<int, DWORD_PTR>::iterator findIt = m_listUserData.find(nID);
	if(findIt == m_listUserData.end()) return NULL;
	return findIt->second;
}

CObjectBase *CObjectBase::GetFirstChildFromID(const int &nID)
{
	m_bLocalFind = true;
	m_itFind = m_listLocalChild.begin();
	LISTOBJECTITER end = m_listLocalChild.end();
	CObjectBase *pRet = NULL;
	for(;m_itFind != end;++m_itFind)
	{
		if(nID == _ID_OBJECT_NOTUSE)
		{
			if((*m_itFind)->GetID() != _ID_OBJECT_NOTUSE)	return *m_itFind;
		}
		else
		{
			if((*m_itFind)->GetID() == nID) return *m_itFind;
		}

		pRet = (*m_itFind)->GetFirstChildFromID(nID);
		if(pRet) return pRet;
	}

	m_bLocalFind = false;
	m_itFind = m_listChild.begin();
	end = m_listChild.end();
	for(;m_itFind != end;++m_itFind)
	{
		if(nID == _ID_OBJECT_NOTUSE)
		{
			if((*m_itFind)->GetID() != _ID_OBJECT_NOTUSE)	return *m_itFind;
		}
		else
		{
			if((*m_itFind)->GetID() == nID) return *m_itFind;
		}

		pRet = (*m_itFind)->GetFirstChildFromID(nID);
		if(pRet) return pRet;
	}

	return NULL;
}

CObjectBase *CObjectBase::GetNextChildFromID(const int &nID)
{
	CObjectBase *pRet = NULL;
	bool bFirst = true;
	if(m_bLocalFind)
	{
		LISTOBJECTITER end = m_listLocalChild.end();
		for(;m_itFind != end;++m_itFind)
		{
			if(nID == _ID_OBJECT_NOTUSE)
			{
				if((*m_itFind)->GetID() != _ID_OBJECT_NOTUSE)	return *m_itFind;
			}
			else
			{
				if((*m_itFind)->GetID() == nID) return *m_itFind;
			}

			if(bFirst)
			{
				pRet = (*m_itFind)->GetNextChildFromID(nID);
				bFirst = false;
			}
			else
			{
				pRet = (*m_itFind)->GetFirstChildFromID(nID);
			}
			if(pRet) return pRet;
		}

		m_bLocalFind = false;
		m_itFind = m_listChild.begin();
		end = m_listChild.end();
		for(;m_itFind != end;++m_itFind)
		{
			if(nID == _ID_OBJECT_NOTUSE)
			{
				if((*m_itFind)->GetID() != _ID_OBJECT_NOTUSE)	return *m_itFind;
			}
			else
			{
				if((*m_itFind)->GetID() == nID) return *m_itFind;
			}

			if(bFirst)
			{
				pRet = (*m_itFind)->GetNextChildFromID(nID);
				bFirst = false;
			}
			else
			{
				pRet = (*m_itFind)->GetFirstChildFromID(nID);
			}
			if(pRet) return pRet;
		}
	}
	else
	{
		m_itFind = m_listChild.begin();
		LISTOBJECTITER end = m_listChild.end();
		for(;m_itFind != end;++m_itFind)
		{
			if(nID == _ID_OBJECT_NOTUSE)
			{
				if((*m_itFind)->GetID() != _ID_OBJECT_NOTUSE)	return *m_itFind;
			}
			else
			{
				if((*m_itFind)->GetID() == nID) return *m_itFind;
			}

			if(bFirst)
			{
				pRet = (*m_itFind)->GetNextChildFromID(nID);
				bFirst = false;
			}
			else
			{
				pRet = (*m_itFind)->GetFirstChildFromID(nID);
			}
			if(pRet) return pRet;
		}
	}
	return NULL;
}

CObjectBase *CObjectBase::GetFirstChildFromName(const std::string &strName)
{
	m_bLocalFind = true;
	m_itFind = m_listLocalChild.begin();
	LISTOBJECTITER end = m_listLocalChild.end();
	CObjectBase *pRet = NULL;
	for(;m_itFind != end;++m_itFind)
	{
		if(strName == "")
		{
			if((*m_itFind)->GetName() != "")	return *m_itFind;
		}
		else
		{
			if((*m_itFind)->GetName() == strName) return *m_itFind;
		}

		pRet = (*m_itFind)->GetFirstChildFromName(strName);
		if(pRet) return pRet;
	}

	m_bLocalFind = false;
	m_itFind = m_listChild.begin();
	end = m_listChild.end();
	for(;m_itFind != end;++m_itFind)
	{
		if(strName == "")
		{
			if((*m_itFind)->GetName() != "")	return *m_itFind;
		}
		else
		{
			if((*m_itFind)->GetName() == strName) return *m_itFind;
		}

		pRet = (*m_itFind)->GetFirstChildFromName(strName);
		if(pRet) return pRet;
	}

	return NULL;
}

CObjectBase *CObjectBase::GetNextChildFromName(const std::string &strName)
{
	CObjectBase *pRet = NULL;
	bool bFirst = true;
	if(m_bLocalFind)
	{
		LISTOBJECTITER end = m_listLocalChild.end();
		for(;m_itFind != end;++m_itFind)
		{
			if(strName == "")
			{
				if((*m_itFind)->GetName() != "")	return *m_itFind;
			}
			else
			{
				if((*m_itFind)->GetName() == strName) return *m_itFind;
			}

			if(bFirst)
			{
				pRet = (*m_itFind)->GetNextChildFromName(strName);
				bFirst = false;
			}
			else
			{
				pRet = (*m_itFind)->GetFirstChildFromName(strName);
			}
			if(pRet) return pRet;
		}

		m_bLocalFind = false;
		m_itFind = m_listChild.begin();
		end = m_listChild.end();
		for(;m_itFind != end;++m_itFind)
		{
			if(strName == "")
			{
				if((*m_itFind)->GetName() != "")	return *m_itFind;
			}
			else
			{
				if((*m_itFind)->GetName() == strName) return *m_itFind;
			}

			if(bFirst)
			{
				pRet = (*m_itFind)->GetNextChildFromName(strName);
				bFirst = false;
			}
			else
			{
				pRet = (*m_itFind)->GetFirstChildFromName(strName);
			}
			if(pRet) return pRet;
		}
	}
	else
	{
		m_itFind = m_listChild.begin();
		LISTOBJECTITER end = m_listChild.end();
		for(;m_itFind != end;++m_itFind)
		{
			if(strName == "")
			{
				if((*m_itFind)->GetName() != "")	return *m_itFind;
			}
			else
			{
				if((*m_itFind)->GetName() == strName) return *m_itFind;
			}

			if(bFirst)
			{
				pRet = (*m_itFind)->GetNextChildFromName(strName);
				bFirst = false;
			}
			else
			{
				pRet = (*m_itFind)->GetFirstChildFromName(strName);
			}
			if(pRet) return pRet;
		}
	}
	return NULL;
}

void CObjectBase::SetTracking(const bool &bTracking)
{
	if(m_bTracking == bTracking) return;
	if(bTracking)
	{
		if(m_pProject == NULL) return;
		m_pProject->GetObjectMan()->SetTracking(this);
	}

	m_bTracking = bTracking;
}

void CObjectBase::RemoveTracking(void)
{
	if(m_bTracking == false) return;
	m_bTracking = false;
	if(m_pParent == NULL) return;
	m_pProject->GetObjectMan()->RemoveTracking(this);
}

void CObjectBase::SetRemove(void)
{
	if(m_pProject && !m_bRemove)
	{
		m_bRemove = true;
		m_pProject->AddRemove(this);
	}
}
} //namespace LSY