// GameViewBase.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameViewBase.h"
#include "GlobalBase.h"
#include "Game.h"

// ### [ Frame++ ] ###
void CALLBACK EventTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	CGameViewBase *pGameViewBase = GM().GetCurrentGameView();
	if(pGameViewBase && pGameViewBase->GetSafeHwnd()) 
	{
		static MSG wq_msg;
		if(::PeekMessage(&wq_msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE) != 0) 
		{ 
			// ������ ����Ʈ�� Ÿ�̸� �޽����� ������ ó������ ���� ����� �׳� ���� 
			if(wq_msg.hwnd == pGameViewBase->GetSafeHwnd() && wq_msg.wParam == GAMEVIEW_TIMER)
			{
				return;
			}
		}

		pGameViewBase->PostMessage(WM_TIMER, GAMEVIEW_TIMER, 0);
	}
} 

// CGameViewBase

IMPLEMENT_DYNAMIC(CGameViewBase, CWnd)

CGameViewBase::CGameViewBase(CGame *pGame)
:	m_pGame(pGame)
{
	m_ReFresh_FPS		= REFRESH_FPS;
	m_hEventTimer		= 0;
	m_pDC				= NULL;
	//������ ���� ����
	m_bExitSubscript	= FALSE;
}

CGameViewBase::~CGameViewBase()
{
	StopTimer();
}


BEGIN_MESSAGE_MAP(CGameViewBase, CWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CGameViewBase �޽��� ó�����Դϴ�.
void CGameViewBase::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(nIDEvent==GAMEVIEW_TIMER)
	{
		if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
		{
			CGameProcess *pProc = GM().GetCurrentProcess();
			if (pProc==NULL) 
				return;

			pProc->OnTimer();

			int skipframe = m_syncfps.FrameSync(FALSE); // ����ȭ

			// ��ŵ�� �����Ӹ�ŭ ������ ����			
			for(int i=0; i<skipframe; i++) 
			{
				pProc->OnTimer();				
			}

			if(skipframe <=0  )
			{
				Invalidate(FALSE); 							
			}

			NMBASE::SOUND::g_Sound.RegularCall();
		}


		
	}
		
	CWnd::OnTimer(nIDEvent);
}

int CGameViewBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �׷��� ���̺귯�� �ʱ�ȭ
	Page.Init(GetWidth(), GetHeight(), 16);
	Randomize();

	m_pDC = GetDC();

	return 0;
}

void CGameViewBase::StartTimer(void)
{
	if(m_hEventTimer)
		return;

	m_syncfps.Init(m_ReFresh_FPS);
	TIMECAPS caps; memset( &caps, 0, sizeof(caps) );
	timeGetDevCaps( &caps, sizeof(caps) ); 	
	m_hEventTimer = timeSetEvent((int)(1000.0f/m_ReFresh_FPS), (int)(1000.0f/m_ReFresh_FPS), EventTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 
}

void CGameViewBase::StopTimer(void)
{
	if(m_hEventTimer == 0) return;
	timeKillEvent(m_hEventTimer);
	m_hEventTimer = 0;
}


