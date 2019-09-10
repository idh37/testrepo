// GameOverResult.cpp: implementation of the CGameOverResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameOverResult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MIN_MONEYLIMIT1 144000000000      // 1440��
#define MAX_MONEYLIMIT1 360000000000 	 // 3600��
CGameOverResult::CGameOverResult()
{
}

CGameOverResult::~CGameOverResult()
{
	
}

void CGameOverResult::Init(NMBASE::GRAPHICGDI::CPage *page)										// �ʱ�
{	
	ReSet();
	m_nXp= 326;
	m_nYp=177;			//�׷��� ��ǥ

}
void CGameOverResult::Clear()
{
	ReSet();
}

void CGameOverResult::ReSet()
{	
	m_bShow=0;				//�׷���
	m_bItemBuyUse =0;		//��ȣõ��
	m_i64LostMoney=0;		//���� �Ӵ�			
	
	m_nShowCnt = 0;			//������ �ð�	
	m_nWinCase = 0;			//1 �̸� �������� �¸� 0 �̸� ��ǽ�
	m_nWinnerKind = 0;		//0 �¸���,1�г�,2���������	
	m_strResultCardName = "";

	m_dwSetStartTime = 0;
	m_nPNum = 0;
	m_nBackIndex = 0;
	m_dwDrawTime = 0;

	m_nDelay = 0;

	ZeroMemory(&m_cUI,sizeof(m_cUI));
	ZeroMemory(&m_cGOR,sizeof(GAMEOVERRESULT));	
	ZeroMemory(m_nCardNo,sizeof(m_nCardNo));
}

void CGameOverResult::OnTimer()
{
	if(m_nDelay > 0)
	{
		m_nDelay--;
		if(m_nDelay == 0)
		{
			// �����϶��� ����� �������� �ʴ´�.
			if(GAME()->m_i64JackPotActive <= 0)
			{
				if( m_bShowAngel == FALSE )
				{
					m_bShow = FALSE;
				}
				else
				{
					m_bShow = TRUE;
				}
				
				m_dwSetStartTime = timeGetTime();
				m_dwDrawTime = m_dwSetStartTime;
			}			
			
			GAME()->ReSetGameOverFlg();
			return;
		}
		return;
	}

	if(m_bShow == FALSE)return;

	DWORD CurTime = timeGetTime();
	
	CurTime = CurTime - m_dwSetStartTime; 
	if(CurTime >= 1000){
		m_dwSetStartTime = timeGetTime();
		m_nShowCnt --;		
	}

	if( m_nShowCnt <= 0 ){
		ReSet();
	}

	if(GAME()->m_bGameStart || g_RI.State == 1 || GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{		
		ReSet();
	}
}

void CGameOverResult::PutAngelResult(GAMEOVERRESULT *pgor,USERINFO *ui,int pnum,int wincase, int delay,BOOL bShow)
{
	m_nWinCase = wincase;

	m_nDelay = delay;
	m_bShowAngel = bShow;

	memcpy(&m_cUI, ui, sizeof(m_cUI));
	memcpy(&m_cGOR, pgor, sizeof(GAMEOVERRESULT));

	m_nPNum = pnum;
	
	if(m_nWinCase == 1){
		m_strResultCardName = GAME()->GetJokboName(pnum);
	}	
	
	// ��� �׸� �б�
	if( g_ObserverMan.IsMyObserver() ) {		
		m_nWinnerKind = 1;			
	}
	else if(GAME()->GetPlayerNP(0)->ServPNum == g_pGameView->m_cPlayerDrawManager.GetWinnerPNum()) {	
		m_nWinnerKind = 0;				
	}
	else {		
		
		int pnum = GAME()->GetServPNum_ByPN(0);
		INT64 tmpmoney = 0;
		if(pnum != -1) {
			tmpmoney = m_cGOR.Ui[pnum].nResultMoney;
		}
	}	

	m_dwSetStartTime = 0;	
	m_nShowCnt = 4;
}

INT64 CGameOverResult::Get_angel_defmoney(const INT64 &lmoney)
{	
	if(lmoney<=MIN_MONEYLIMIT1)	  return MIN_MONEYLIMIT1;
	else if(lmoney>=MAX_MONEYLIMIT1) return MAX_MONEYLIMIT1;
	else return lmoney;
}

INT64 CGameOverResult::Get_angel_defmoneyEx(const INT64 &lmoney)
{
	return (INT64)0;	
}

void CGameOverResult::AngelDraw(CDC *pdc , NMBASE::GRAPHICGDI::CPage *page)
{
	if(pdc == NULL || page == NULL)return;	

	if(m_bShow == FALSE)return;
}