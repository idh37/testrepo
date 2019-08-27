#include "stdafx.h"
#include "ChattingPopupMemu.h"
#include "ChattingPopupMemuID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//alias
//typedef ITaskbarList *LPITaskbarList;

IMPLEMENT_DYNAMIC(CChattingPopupMemu, LSY::CLSYLibWindowsBase)

BEGIN_MESSAGE_MAP(CChattingPopupMemu, LSY::CLSYLibWindowsBase)
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CChattingPopupMemu, LSY::CLSYLibWindowsBase)
END_DISPATCH_MAP()

CChattingPopupMemu::CChattingPopupMemu()
:	LSY::CLSYLibWindowsBase(),
	m_pImage(NULL)
{
}

CChattingPopupMemu::~CChattingPopupMemu()
{
}

bool CChattingPopupMemu::Create(int x, int y, CWnd *pParent)
{
	Init(GetObjectMan(), ID_LOBBYPOPUPMENU, true);
	if(!CLSYLibWindowsBase::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS), "PopupMemu", WS_POPUP | WS_OVERLAPPED | WS_CLIPCHILDREN, x, y, 100, 100, pParent->GetSafeHwnd(), NULL))
	{
		return false;
	}

	SetWindowPos(&CWnd::wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	return true;
}

void CChattingPopupMemu::AddMemu(int nID, LPCTSTR lpText, bool bEnable/* = true*/)
{
	std::list<CMemuData>::iterator start = m_listMenu.begin();
	std::list<CMemuData>::iterator end = m_listMenu.end();

	for(;start != end;++start)
	{
		if(start->m_nID == nID) break;
	}

	ASSERT(start == end);

	CMemuData data;
	data.m_nID		= nID;
	data.m_strText	= lpText;
	data.m_bEnable	= bEnable;

	m_listMenu.push_back(data);
}

int CChattingPopupMemu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nRet = __super::OnCreate(lpCreateStruct);
	if(nRet != -1)
	{
		//작업표시줄 탭 없애기
		//LPITaskbarList	pTaskbar;

		//initializes the Component Object Model(COM)
		//CoInitialize(NULL);
		//We call below function since we only need to create one object
		//CoCreateInstance(CLSID_TaskbarList,0, 
		//	CLSCTX_INPROC_SERVER,IID_ITaskbarList,(void**)&pTaskbar);
		//Below function will initialize the taskbar list object
		//pTaskbar->HrInit();
		//pTaskbar->DeleteTab(GetSafeHwnd());
		//CoUninitialize();
	}

	return nRet;
}

bool CChattingPopupMemu::OnCreatedProject(void)
{
	m_pObjectMan->SetMouseCapture();

	LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_LOBBYPOPUPMENU_IMG_BACK);
	pImage->SetMiddleCount((int)m_listMenu.size());

	m_pImage = (LSY::CImage *)GetObject(ID_LOBBYPOPUPMENU_IMG_OVER);
	m_pImage->SetShow(false);

	LSY::CTextButton *pButton = (LSY::CTextButton *)GetObject(ID_LOBBYPOPUPMENU_TXTBTN_MEMU);
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromLayer(0);
	LSY::CObjectBase *pMainObject = pProject->GetMainObject();

	if(m_listMenu.size() == 0)
	{
		m_pImage->SetShow(false);
		pButton->SetShow(false);
	}
	else
	{
		std::list<CMemuData>::iterator start = m_listMenu.begin();
		std::list<CMemuData>::iterator end = m_listMenu.end();

		LSY::CTextButton *pClone = NULL;
		
		LSY::CPoint ptLocalPos = pButton->GetLocalPos();
		float fMemuHeight = pButton->GetHeight();

		for(int i=0;start != end;++start,++i)
		{
			if(i == 0) pClone = pButton;
			else
			{
				pClone = (LSY::CTextButton *)pButton->Clone();
				pMainObject->AddChild(pClone);
			}

			pClone->SetLocalPos(ptLocalPos.x, ptLocalPos.y + fMemuHeight * (float)i);
			pClone->SetText(start->m_strText);
			pClone->SetID(start->m_nID);
			pClone->SetEnable(start->m_bEnable);

			pClone->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingPopupMemu::OnClickButton));
			pClone->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CChattingPopupMemu::OnMouseOver));
			pClone->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CChattingPopupMemu::OnMouseLeave));
		}
	}

	LSY::CRect rtRect = pImage->GetLocalRect();
	m_pObjectMan->ReSize(rtRect.GetSize());

	GetWindowRect(&m_rtRect);

	return TRUE;
}

LRESULT CChattingPopupMemu::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	CWnd *pParent = GetParent();
	pParent->PostMessage(UPGM_POPUPMEMU_CLOSE, (WPARAM)msg->GetObject()->GetID());

	ShowWindow(SW_HIDE);

	return TRUE;
}

LRESULT CChattingPopupMemu::OnMouseOver(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseOver *msg = (LSY::CMO_MouseOver *)pMsg;
	LSY::CObjectBase *pObject = msg->GetObject();
	if(m_pImage)
	{
		m_pImage->SetShow(true);
		m_pImage->SetLocalPos(pObject->GetLocalPos());
	}
	return TRUE;
}

LRESULT CChattingPopupMemu::OnMouseLeave(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLeave *msg = (LSY::CMO_MouseLeave *)pMsg;
	if(m_pImage) m_pImage->SetShow(false);
	return TRUE;
}

LRESULT CChattingPopupMemu::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if(message == WM_DESTROY) m_pImage = NULL;
	if(message == WM_KILLFOCUS)
	{
		CWnd *pParent = GetParent();
		pParent->PostMessage(UPGM_POPUPMEMU_CLOSE);
		return TRUE;
	}

	LRESULT nRet = LSY::CLSYLibWindowsBase::WindowProc(message, wParam, lParam);

	if(nRet == FALSE)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			{
				CPoint ptPos;
				GetCursorPos(&ptPos);
				if(!m_rtRect.PtInRect(ptPos))
				{
					CWnd *pWnd = WindowFromPoint(ptPos);
					pWnd->ScreenToClient(&ptPos);
					if(pWnd) pWnd->SendMessage(message, wParam, MAKELPARAM(ptPos.x, ptPos.y));

					CWnd *pParent = GetParent();
					pParent->PostMessage(UPGM_POPUPMEMU_CLOSE);
				}
			}
			return TRUE;
		}
	}

	return nRet;
}