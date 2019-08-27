// GameViewBase.cpp : 구현 파일입니다.
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
			// 이전에 포스트된 타이머 메시지가 아직도 처리되지 않은 경우라면 그냥 리턴 
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
	//나가기 예약 상태
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

// CGameViewBase 메시지 처리기입니다.
void CGameViewBase::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==GAMEVIEW_TIMER)
	{
		if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
		{
			CGameProcess *pProc = GM().GetCurrentProcess();
			if (pProc==NULL) 
				return;

			pProc->OnTimer();

			int skipframe = m_syncfps.FrameSync(FALSE); // 동기화

			// 스킵된 프레임만큼 프레임 진행			
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

	// 그래픽 라이브러리 초기화
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


