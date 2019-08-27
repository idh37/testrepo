#include "stdafx.h"
#include "LSYLibWindowBase.h"
#include "./Object/ObjectMan.h"
#include "./Functor/Message.h"
#include "./Object/Base/ObjectBase.h"
#include "./Object/Base/Window.h"
#include "./Object/Control/Edit.h"
#include "./Object/Control/EditCursor.h"
#include "./Object/Base/DumyImage.h"
#include "./Object/Button/Button.h"
#include "./LSYLibDialogBase.h"

namespace LSY
{
IMPLEMENT_DYNAMIC(CLSYLibWindowsBase, CWnd)

CLSYLibWindowsBase::CLSYLibWindowsBase()
:	CWnd(),
	m_pObjectMan(NULL),
	m_bEventDraw(true),
	m_pReceiveWnd(NULL)
{

}

CLSYLibWindowsBase::~CLSYLibWindowsBase()
{
	if(m_pObjectMan)
	{
		delete m_pObjectMan;
		m_pObjectMan = NULL;
	}
}

BEGIN_MESSAGE_MAP(CLSYLibWindowsBase, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLSYLibWindowsBase, CWnd)
END_DISPATCH_MAP()

int CLSYLibWindowsBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1) return -1;
	
	m_pObjectMan = new CObjectMan(this, NULL, false, m_bEventDraw);
	CObjectProject *pProject = m_pObjectMan->CreateProject(m_szSize.cx, m_szSize.cy);
	pProject->SetFileName(m_strFileName);
	pProject->SetName(m_strProjectName, true);
	pProject->SetID(m_nID);
	pProject->SetCallCreateObject(Fnt(this, &CLSYLibWindowsBase::OnCallCreateObject));
	pProject->LoadXml();
	m_pObjectMan->SetProject(0, m_nID);
	m_pObjectMan->SetUseCustomCursor(true);
	m_pObjectMan->SetCaptionHeight(0);
	m_pObjectMan->SetFPS(20);

	if(GetParent())
	{
		m_pReceiveWnd = GetParent();
		CWnd *pParent = m_pReceiveWnd;
		CWnd *pLastWnd = m_pReceiveWnd;
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

		pTempWnd = dynamic_cast<CLSYLibDialogBase *>(m_pReceiveWnd);
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

	if(!OnCreatedProject()) return -1;

	SetRegion();
	InvalidateRect(NULL, FALSE);

	return 0;
}

LRESULT CLSYLibWindowsBase::OnCallCreateObject(CMessage *msg)
{
	CMO_CreateObject *pMsg = (CMO_CreateObject *)msg;
	return (LRESULT)OnCallCreateObject(*pMsg->GetID(), *pMsg->GetName());
}

CObjectBase *CLSYLibWindowsBase::GetObject(const int &nID)
{
	if(m_pObjectMan == NULL) return NULL;
	CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	if(pProject == NULL) return NULL;
	return pProject->GetObject(nID);
}

CObjectBase *CLSYLibWindowsBase::GetObject(const std::string &strName)
{
	if(m_pObjectMan == NULL) return NULL;
	CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	if(pProject == NULL) return NULL;
	return pProject->GetObject(strName);
}

void CLSYLibWindowsBase::Init(CObjectMan *pMainObjectMan, int nID, bool bEventDraw/* = true*/)
{
	ASSERT(pMainObjectMan != NULL);
	CObjectProject *pProject = pMainObjectMan->GetProjectFromID(nID);
	m_strFileName = pProject->GetFileName();
	m_strProjectName = pProject->GetName();
	m_nID = nID;
	m_szSize.cx = pProject->GetWidth();
	m_szSize.cy = pProject->GetHeight();
	m_bEventDraw = bEventDraw;
}

CObjectBase *CLSYLibWindowsBase::OnCallCreateObject(const int &nID, const std::string &strName)
{
	return NULL;
}

bool CLSYLibWindowsBase::OnCreatedProject(void)
{
	return true;
}

BOOL CLSYLibWindowsBase::PreTranslateMessage(MSG* pMsg)
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
			case VK_ESCAPE:
				OnCancel();
				return TRUE;
			}
			break;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CLSYLibWindowsBase::OnCancel()
{
	PostMessage(WM_CLOSE);
}

LRESULT CLSYLibWindowsBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(message == LSY_UM_EVENT_DRAW && m_pObjectMan && m_pObjectMan->IsFirstDraw())
			return TRUE;

	if(m_pObjectMan && m_pObjectMan->WindowsProc(message, wParam, lParam))
		return TRUE;

	return CWnd::WindowProc(message, wParam, lParam);
}

void CLSYLibWindowsBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_pObjectMan)
		m_pObjectMan->BitBlt(&dc);
}

BOOL CLSYLibWindowsBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
}

void CLSYLibWindowsBase::SetFPS(float fps)
{
	if(m_pObjectMan) m_pObjectMan->SetFPS(fps);
}

void CLSYLibWindowsBase::OnDestroy()
{
	CWnd::OnDestroy();

	if(m_pObjectMan)
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
		else
		{
			CObjectMan *pObjectMan = LSY::CObjectMan::GetMainObjectMan();
			if(pObjectMan) pObjectMan->SubChildObjectMan(m_pObjectMan);
		}

		delete m_pObjectMan;
		m_pObjectMan = NULL;
	}
}

bool CLSYLibWindowsBase::SetRegion(void)
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