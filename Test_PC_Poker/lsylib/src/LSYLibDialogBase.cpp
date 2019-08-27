#include "stdafx.h"
#include "LSYLibDialogBase.h"
#include "./Object/ObjectMan.h"
#include "./Functor/Message.h"
#include "./Object/Base/ObjectBase.h"
#include "./Object/Base/Window.h"
#include "./Object/Control/Edit.h"
#include "./Object/Control/EditCursor.h"
#include "./Object/Base/DumyImage.h"
#include "./Object/Button/Button.h"

#define DEF_REDRAW_TIMER1 (DEF_REDRAW_TIMER+1)
#define DEF_REDRAW_TIMER2 (DEF_REDRAW_TIMER+2)
#define DEF_REDRAW_TIMER3 (DEF_REDRAW_TIMER+3)

namespace LSY
{
// CLSYLibDialogBase 대화 상자입니다.
IMPLEMENT_DYNAMIC(CLSYLibDialogBase, CDialog)

CLSYLibDialogBase::CLSYLibDialogBase(CWnd* pParent /*=NULL*/)
:	CDialog(CLSYLibDialogBase::IDD, pParent),
	m_pObjectMan(NULL),
	m_bModal(false),
	m_bEventDraw(false),
	m_pReceiveWnd(NULL)
{

}

CLSYLibDialogBase::~CLSYLibDialogBase()
{
	if(m_pObjectMan)
	{
		delete m_pObjectMan;
		m_pObjectMan = NULL;
	}
}

void CLSYLibDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLSYLibDialogBase, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLSYLibDialogBase, CDialog)
END_DISPATCH_MAP()

BOOL CLSYLibDialogBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pObjectMan = new CObjectMan(this, NULL, false, m_bEventDraw);
	CObjectProject *pProject = m_pObjectMan->CreateProject(m_szSize.cx, m_szSize.cy);
	pProject->SetFileName(m_strFileName);
	pProject->SetName(m_strProjectName, true);
	pProject->SetID(m_nID);
	pProject->SetCallCreateObject(Fnt(this, &CLSYLibDialogBase::OnCallCreateObject));
	pProject->LoadXml();
	m_pObjectMan->SetProject(0, m_nID);
	m_pObjectMan->SetUseCustomCursor(true);
	m_pObjectMan->SetCaptionHeight(28);
	m_pObjectMan->SetFPS(20);

	if(!OnCreatedProject())
	{
		OnCancel();
		return FALSE;
	}

	SetRegion();

	m_pParentWnd = GetParent();
	if(m_pParentWnd)
	{
		m_pReceiveWnd = m_pParentWnd;
		CWnd *pParent = m_pParentWnd;
		CWnd *pLastWnd = m_pParentWnd;
		CLSYLibDialogBase *pTempWnd = NULL;
		while(pParent)
		{
			pLastWnd = pParent;
			pTempWnd = dynamic_cast<CLSYLibDialogBase *>(pParent);
			if(pTempWnd && pTempWnd->IsModal())
			{
				break;
			}
			pParent = pParent->GetParent();
		}

		if(pParent == NULL)
		{
			m_pReceiveWnd = pLastWnd;
		}
		else
		{
			m_pReceiveWnd = pParent;
		}
	}

	if(m_bModal)
	{
		SetTimer(DEF_REDRAW_TIMER1, 10, NULL);
		SetTimer(DEF_REDRAW_TIMER2, 10, NULL);
//		SetTimer(DEF_REDRAW_TIMER3, 10, NULL);
	}
	else
	{
		if(m_pParentWnd)
		{
			CLSYLibDialogBase *pTempWnd = dynamic_cast<CLSYLibDialogBase *>(m_pReceiveWnd);
			if(pTempWnd)
			{
				if(pTempWnd->m_pObjectMan) pTempWnd->m_pObjectMan->AddChildObjectMan(m_pObjectMan);
			}
			else
			{
				CObjectMan *pObjectMan = LSY::CObjectMan::GetMainObjectMan();
				if(pObjectMan) pObjectMan->AddChildObjectMan(m_pObjectMan);
			}
		}
		else
		{
			CObjectMan *pObjectMan = LSY::CObjectMan::GetMainObjectMan();
			if(pObjectMan) pObjectMan->AddChildObjectMan(m_pObjectMan);
		}
	}

	ShowWindow(SW_SHOW);
	CenterWindow(m_pParentWnd);
	InvalidateRect(NULL, FALSE);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CLSYLibDialogBase::OnCallCreateObject(CMessage *msg)
{
	CMO_CreateObject *pMsg = (CMO_CreateObject *)msg;
	return (LRESULT)OnCallCreateObject(*pMsg->GetID(), *pMsg->GetName());
}

CObjectBase *CLSYLibDialogBase::GetObject(const int &nID)
{
	if(m_pObjectMan == NULL) return NULL;
	CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	if(pProject == NULL) return NULL;
	return pProject->GetObject(nID);
}

CObjectBase *CLSYLibDialogBase::GetObject(const std::string &strName)
{
	if(m_pObjectMan == NULL) return NULL;
	CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	if(pProject == NULL) return NULL;
	return pProject->GetObject(strName);
}

INT_PTR CLSYLibDialogBase::DoModal(CObjectMan *pMainObjectMan, int nID, bool bEventDraw/* = true*/, CWnd* pParent/* = NULL*/)
{
	ASSERT(pMainObjectMan != NULL);
	CObjectProject *pProject = pMainObjectMan->GetProjectFromID(nID);
	m_strFileName = pProject->GetFileName();
	m_strProjectName = pProject->GetName();
	m_nID = nID;
	m_szSize.cx = pProject->GetWidth();
	m_szSize.cy = pProject->GetHeight();
	m_bModal = true;
	m_bEventDraw = bEventDraw;

	m_pParentWnd = pParent;
	if(m_pParentWnd)
		m_pParentWnd->SendMessageToDescendants(WM_CANCELMODE);
	else
	{
		CWnd *pLastWnd = NULL;
		CWnd *pWnd = pMainObjectMan->GetMainObjectMan()->GetWnd();

		while(pWnd)
		{
			pLastWnd = pWnd;
			pWnd = pWnd->GetParent();
		}

		if(pLastWnd) pLastWnd->SendMessageToDescendants(WM_CANCELMODE);
	}
	return CDialog::DoModal();
}

BOOL CLSYLibDialogBase::Create(CObjectMan *pMainObjectMan, int nID, CWnd *pParent/* = NULL*/, bool bEventDraw/* = true*/)
{
	ASSERT(pMainObjectMan != NULL);
	CObjectProject *pProject = pMainObjectMan->GetProjectFromID(nID);
	m_strFileName = pProject->GetFileName();
	m_strProjectName = pProject->GetName();
	m_nID = nID;
	m_szSize.cx = pProject->GetWidth();
	m_szSize.cy = pProject->GetHeight();
	m_bModal = false;
	m_bEventDraw = bEventDraw;
	m_pParentWnd = pParent;
	return CDialog::Create(CLSYLibDialogBase::IDD, pParent);
}

CObjectBase *CLSYLibDialogBase::OnCallCreateObject(const int &nID, const std::string &strName)
{
	return NULL;
}

bool CLSYLibDialogBase::OnCreatedProject(void)
{
	return true;
}

LRESULT CLSYLibDialogBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_pObjectMan && m_pObjectMan->WindowsProc(message, wParam, lParam))
		return TRUE;

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CLSYLibDialogBase::PreTranslateMessage(MSG* pMsg)
{
	if(m_pObjectMan)
	{
		switch(pMsg->message)
		{
		case WM_SYSKEYDOWN:
			switch(pMsg->wParam)
			{
			case VK_F4:
				if(GetKeyState(VK_LMENU) & 0xff00)
				{
					OnCancel();
					return TRUE;
				}
				break;
			}
			break;
		case WM_KEYDOWN:
			switch(pMsg->wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_HOME:
			case VK_END:
			case VK_DELETE:
			case VK_UP:
			case VK_DOWN:
			case VK_BACK:
			case VK_RETURN:
				return FALSE;
			case VK_ESCAPE:
				OnCancel();
				return TRUE;
			}
			break;
		case WM_CHAR:
			return FALSE;
		case LSY_UM_EVENT_DRAW:
			if(m_pObjectMan->IsFirstDraw())
				return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CLSYLibDialogBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_pObjectMan)
		m_pObjectMan->BitBlt(&dc);
}

BOOL CLSYLibDialogBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
}


void CLSYLibDialogBase::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case DEF_REDRAW_TIMER1:
	case DEF_REDRAW_TIMER2:
	case DEF_REDRAW_TIMER3:
		if(!m_bModal)
			return;

		if (m_pObjectMan)
			m_pObjectMan->AllDraw();

		if ( LSY::CObjectMan::GetMainObjectMan() )
			LSY::CObjectMan::GetMainObjectMan()->AllDraw();

// 		if(m_pReceiveWnd)
// 		{
// 			// 부모윈도우의 화면 갱신을 위해 호출을 해준다.
// 			m_pReceiveWnd->SendMessage(WM_TIMER, DEF_REDRAW_TIMER, 0);
// 		}

		NMBASE::UTIL::MM().ProcPostMessage();

		NMBASE::UTIL::MM().Call("NMApp.ProcessModalDialog");

		break;

// 	case DEF_REDRAW_TIMER:
// 		if(m_bModal) return;
// 		if(m_pObjectMan) m_pObjectMan->AllDraw();
// 		return;
	}

	CDialog::OnTimer(nIDEvent);
}

void CLSYLibDialogBase::SetFPS(float fps)
{
	if(m_pObjectMan) m_pObjectMan->SetFPS(fps);
}

void CLSYLibDialogBase::SetCaptionHeight(int nCaptionHeight)
{
	if(m_pObjectMan) m_pObjectMan->SetCaptionHeight(nCaptionHeight);
}

void CLSYLibDialogBase::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_pObjectMan)
	{
		if(!m_bModal)
		{
			if(m_pParentWnd)
			{
				if(m_pReceiveWnd)
				{
					CLSYLibDialogBase *pTempWnd = dynamic_cast<CLSYLibDialogBase *>(m_pReceiveWnd);
					if(pTempWnd)
					{
						if(pTempWnd->m_pObjectMan) pTempWnd->m_pObjectMan->SubChildObjectMan(m_pObjectMan);
					}
					else
					{
						CObjectMan *pObjectMan = LSY::CObjectMan::GetMainObjectMan();
						if(pObjectMan) pObjectMan->SubChildObjectMan(m_pObjectMan);	
					}
				}
			}
			else
			{
				CObjectMan *pObjectMan = LSY::CObjectMan::GetMainObjectMan();
				if(pObjectMan) pObjectMan->SubChildObjectMan(m_pObjectMan);				
			}
		}

		delete m_pObjectMan;
		m_pObjectMan = NULL;
	}

	KillTimer(DEF_REDRAW_TIMER1);
	KillTimer(DEF_REDRAW_TIMER2);
	KillTimer(DEF_REDRAW_TIMER3);
}

bool CLSYLibDialogBase::SetRegion(void)
{
	if(m_pObjectMan == NULL) return false;
	CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	if(pProject == NULL) return false;
	CWindows *pMainObject = (CWindows *)pProject->GetMainObject();
	if(pMainObject == NULL) return false;
	NMBASE::GRAPHICGDI::xSprite	*pSprite = pMainObject->GetSprite();
	int nIndex = pMainObject->GetIndex();
	if(pSprite == NULL || nIndex < 0 || nIndex >= pSprite->GetTotalSpr()) return false;
	NMBASE::GRAPHICGDI::SPRITE30 *pImage = &(pSprite->spr[nIndex]);
	if(pImage == NULL) return false;

	RECT rtInclude;
	SetRectEmpty(&rtInclude);

	HRGN hRgn = NULL;
	DWORD dwMaxRect = 100;

	RGNDATA *pData = (RGNDATA *) new BYTE[sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect)];
	pData->rdh.dwSize = sizeof(RGNDATAHEADER);
	pData->rdh.iType = RDH_RECTANGLES;
	pData->rdh.nCount = pData->rdh.nRgnSize = 0;
	SetRect(&pData->rdh.rcBound, pImage->xl + 1, pImage->yl + 1, 0, 0);
	
	RECT rt;
	RECT rtIntersect;

	//압축 상태
	if(pImage->compress)
	{
		int  x1, x2, width, y;
		short cnt;

		WORD *pColor = pImage->data + pImage->yl * 2;
		for(y=0;y<pImage->yl;++y) 
		{
			cnt = *pColor;      // 패턴 개수 얻기
			++pColor;
			x1 = x2 = 0;
			while(cnt--)
			{
				x1 = x2;
				//투명 통과
				width = (*(WORD*)pColor);
				x2 = x1 + width;
				++pColor;

				if(width > 0)
				{
					SetRect(&rt, x1, y, x2+1, y + 1);
					IntersectRect(&rtIntersect, &rt, &rtInclude);

					LPBYTE pr = (LPBYTE)pData->Buffer;
					pr += sizeof(RECT) * pData->rdh.nCount;

					if(IsRectEmpty(&rtIntersect)) rtIntersect = rt;

					*((LPRECT)pr) = rtIntersect;

					if (rtIntersect.left < pData->rdh.rcBound.left)		
						pData->rdh.rcBound.left = rtIntersect.left;
					if (rtIntersect.top < pData->rdh.rcBound.top)		
						pData->rdh.rcBound.top = rtIntersect.top;
					if (rtIntersect.right > pData->rdh.rcBound.right)	
						pData->rdh.rcBound.right = rtIntersect.right;
					if (rtIntersect.bottom > pData->rdh.rcBound.bottom)	
						pData->rdh.rcBound.bottom = pData->rdh.rcBound.bottom;
					pData->rdh.nCount++;

					if (pData->rdh.nCount == dwMaxRect)
					{
						HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect), pData);
						if (hRgn)
						{
							CombineRgn(hRgn, hRgn, h, RGN_OR);
							DeleteObject(h);
						}
						else hRgn = h;

						pData->rdh.nCount = 0;
						SetRect(&pData->rdh.rcBound, pImage->xl + 1, pImage->yl + 1, 0, 0);
					}
				}

				x1 = x2;

				//이미지 그리기
				width = (*(WORD*)pColor);
				++pColor;
				x2 = x1 + width;

				pColor += width;
			}
		}
	}
	//비 압축 상태
	else
	{
		WORD wColorKey = pImage->colorkey;
		WORD *pColor = pImage->data;
		int x1, x2;

		for(int y=0; y < pImage->yl; ++y)
		{
			x1 = x2 = -1;
			for(int x=0; x< pImage->xl; ++x)
			{
				//칼라키인 부분을 구한다.
				if(*pColor == wColorKey)
				{
					if(x1 == -1) x1 = x2 = x;
					else x2 = x;
				}
				//칼라키가 아닐경우 칼라키가 아닌 부분을 구한다.
				else
				{
					//만약 x1이 -1이 아니면 이미 이미지 영역을 지나온 것이므로
					//리전을 설정한다.
					if(x1 != -1)
					{
						SetRect(&rt, x1, y, x2+1, y + 1);
						IntersectRect(&rtIntersect, &rt, &rtInclude);

						LPBYTE pr = (LPBYTE)pData->Buffer;
						pr += sizeof(RECT) * pData->rdh.nCount;

						if(IsRectEmpty(&rtIntersect)) rtIntersect = rt;

						*((LPRECT)pr) = rtIntersect;

						if (rtIntersect.left < pData->rdh.rcBound.left)		
							pData->rdh.rcBound.left = rtIntersect.left;
						if (rtIntersect.top < pData->rdh.rcBound.top)		
							pData->rdh.rcBound.top = rtIntersect.top;
						if (rtIntersect.right > pData->rdh.rcBound.right)	
							pData->rdh.rcBound.right = rtIntersect.right;
						if (rtIntersect.bottom > pData->rdh.rcBound.bottom)	
							pData->rdh.rcBound.bottom = pData->rdh.rcBound.bottom;
						pData->rdh.nCount++;

						if (pData->rdh.nCount == dwMaxRect)
						{
							HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect), pData);
							if (hRgn)
							{
								CombineRgn(hRgn, hRgn, h, RGN_OR);
								DeleteObject(h);
							}
							else hRgn = h;

							pData->rdh.nCount = 0;
							SetRect(&pData->rdh.rcBound, pImage->xl + 1, pImage->yl + 1, 0, 0);
						}
					}

					x1 = x2 = -1;
				}

				++pColor;
			}

			if(x1 != -1)
			{
				SetRect(&rt, x1, y, x2+1, y + 1);
				IntersectRect(&rtIntersect, &rt, &rtInclude);

				LPBYTE pr = (LPBYTE)pData->Buffer;
				pr += sizeof(RECT) * pData->rdh.nCount;

				if(IsRectEmpty(&rtIntersect)) rtIntersect = rt;

				*((LPRECT)pr) = rtIntersect;

				if (rtIntersect.left < pData->rdh.rcBound.left)		
					pData->rdh.rcBound.left = rtIntersect.left;
				if (rtIntersect.top < pData->rdh.rcBound.top)		
					pData->rdh.rcBound.top = rtIntersect.top;
				if (rtIntersect.right > pData->rdh.rcBound.right)	
					pData->rdh.rcBound.right = rtIntersect.right;
				if (rtIntersect.bottom > pData->rdh.rcBound.bottom)	
					pData->rdh.rcBound.bottom = pData->rdh.rcBound.bottom;
				pData->rdh.nCount++;

				if (pData->rdh.nCount == dwMaxRect)
				{
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else hRgn = h;

					pData->rdh.nCount = 0;
					SetRect(&pData->rdh.rcBound, pImage->xl + 1, pImage->yl + 1, 0, 0);
				}
			}
		}
	}

	if (pData->rdh.nCount)
	{
		HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * pData->rdh.nCount), pData);
		if (hRgn)
		{
			CombineRgn(hRgn, hRgn, h, RGN_OR);
			DeleteObject(h);


		}
		else hRgn = h;

		if (hRgn)
		{
			HRGN h = CreateRectRgn(0, 0, pImage->xl, pImage->yl);
			CombineRgn(hRgn, hRgn, h, RGN_XOR);
			DeleteObject(h);
		}
	}
	delete (LPBYTE)pData;

	/// 윈도우 세팅 
	BOOL bRet = FALSE;
	if (GetObjectType(hRgn)) 
	{
		bRet = SetWindowRgn(hRgn, TRUE);
		DeleteObject(hRgn);
	}

	return true;
}
}