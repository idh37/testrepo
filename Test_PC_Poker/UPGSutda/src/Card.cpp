// Card.cpp: implementation of the CCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Card.h"
#include "GlobalGame.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define CARDMOVESPD_DEFAULT (30)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCard::CCard()
{
	pPage = NULL;
	m_pHwatuNSpr = NULL;
	ZeroMemory(&Event, sizeof(CARDEVENT_SUTDA));
	ZeroMemory(&Event2, sizeof(CARDEVENT_SUTDA));


	Reset();
}

CCard::~CCard()
{

}

void CCard::Init(NMBASE::GRAPHICGDI::CPage *ppage, NMBASE::GRAPHICGDI::xSprite *pHwatuNSpr)
{	
	Reset();
	pPage = ppage;
	CardNo = 0;
	Who = 0;
	m_pHwatuNSpr = pHwatuNSpr;


	//## ��Ȯ�� ##//
	m_xl = m_pHwatuNSpr->spr->xl;
	m_yl = m_pHwatuNSpr->spr->yl;

	m_subval_x = 0;
	m_subval_y = 0;

	//ī�尡 ���� Ŀ���� ũ�� 
	m_subval_x = (int)((float)m_pHwatuNSpr->spr->xl / 5.0f);
	m_subval_y = (int)((float)m_pHwatuNSpr->spr->yl / 5.0f);

	//ī�尡 Ŀ���� �ִ����
	m_Max_Growth_X = (int)(m_pHwatuNSpr->spr->xl * 1.5f);
	m_Max_Growth_Y = (int)(m_pHwatuNSpr->spr->yl * 1.5f);
	////////////////////////////////////////////////////
}

void CCard::SetCard(int cardnum)
{
	Reset();
	//[��������]
// 	if(g_RI.Rule != 2)
// 	{
		if(cardnum<0 || cardnum>20) cardnum = 0;
		CardNo = cardnum;
		Kind = cardnum/2 + (cardnum%2);
// 	}
// 	else
// 	{
// 		if(cardnum<0 || cardnum>40) cardnum = 0;
// 		CardNo = cardnum;
// 		Kind = (cardnum + 3)/4;
// 	}
}

// ����� ī�带 �����Ѵ�
void CCard::CardValue(int cardnum)
{
// 	if(g_RI.Rule != 2)
// 	{
		if(cardnum<0 || cardnum>20) cardnum = 0;
		CardNo = cardnum;
		Kind = cardnum/2 + (cardnum%2);
// 	}
// 	else
// 	{
// 		if(cardnum<0 || cardnum>40) cardnum = 0;
// 		CardNo = cardnum;
// 		Kind = (cardnum + 3)/4;
// 	}
}

void CCard::Reset()
{
	CardNo = 0;
	//Kind = 0;
	ZeroMemory(&Event, sizeof(CARDEVENT_SUTDA));
	ZeroMemory(&Event2, sizeof(CARDEVENT_SUTDA));
	Who  =  0;
	ShowMode = 0;

	bNotifyEvent = FALSE;
	NotifyString.Format("");

	MoveStartSnd = NULL;
	MoveEndSnd = NULL;

	bShow = FALSE;
	bFront = FALSE;
	bMove = FALSE;
	bEvent = FALSE;
	bEvent2 = FALSE;
	bEndMove = FALSE;

	Xp = Yp = 0;
	MoveLen = 0;
	SXp = SYp = 0;
	TXp = TYp = 0;

	CXp = CYp = 0; // Ŭ������� ���콺 ������

	MoveDelay = 0;
	EndMoveDelay = 0;
	MoveSpeed = 25;  

	bTwincle = FALSE;
	TwincleCnt = 0;

	bRedTwincle = FALSE;
	RedTwincleCnt = 0;
//----------------------------------
	bFrontCall = FALSE;
	m_PNum = -1;
	bMadeDraw = FALSE;
	nMadeCnt = 0;

	// �߰�.
	m_growth_delay = 0;
	
	//ī��ִϸ��̼��߰�
	m_bActive = FALSE;
	m_dwActiveTime = 0;
}

void CCard::SetAnimation( int r_kind)
{
	bShow = FALSE;
	m_bActive = TRUE;
	m_dwActiveTime = timeGetTime();
}

void CCard::SetEvent(int eventcode, int option, int option2)
{
	ZeroMemory(&Event, sizeof(CARDEVENT_SUTDA));
	bEvent = TRUE;
	Event.CardNo = CardNo;
	Event.EventCode = eventcode;
	Event.Option = option;
	Event.Option2 = option2;
}

void CCard::SetEvent2(int eventcode, int option, int option2)
{
	ZeroMemory(&Event2, sizeof(CARDEVENT_SUTDA));
	bEvent2 = TRUE;
	Event2.CardNo = CardNo;
	Event2.EventCode = eventcode;
	Event2.Option = option;
	Event2.Option2 = option2;
}

void CCard::ClearEvent()
{
	ZeroMemory(&Event, sizeof(CARDEVENT_SUTDA));
	bEvent = FALSE;
}

void CCard::ClearEvent2()
{
	ZeroMemory(&Event2, sizeof(CARDEVENT_SUTDA));
	bEvent2 = FALSE;
}


void CCard::SetNotifyEvent(char *pstr, COLORREF color)
{
	bNotifyEvent = TRUE;
	NotifyStringColor = color;
	NotifyString.Format("%s", pstr);
}

void CCard::ClearNotifyEvent()
{
	bNotifyEvent = FALSE;
	NotifyStringColor = 0;
	NotifyString.Format("");
}

void CCard::SetMoveStartSnd(char* sndfilename)
{
	MoveStartSnd = sndfilename;
}

void CCard::SetMoveEndSnd(char* sndfilename)
{
	MoveEndSnd = sndfilename;
}

//##���̵� ǥ�ø� ���ش�.
void CCard::SetMadeDraw(int cnt)
{
	nMadeCnt = cnt;		
	bMadeDraw = TRUE;
}

void CCard::SetPos(int tx, int ty)
{
	SXp = TXp = Xp = tx;
	SYp = TYp = Yp = ty;
	MoveLen = 0;
}

void CCard::SetMovePos( int mx, int my)
{
	SetMove( mx, my);
}

//##ī�� �̵�
void CCard::SetMove(int tx, int ty, int sdelay, int edelay, int speed, int front, char* startsound, char* endsound)
{
	SXp = Xp; SYp = Yp;
	TXp = tx; TYp = ty;

	bMove = TRUE;
	bEndMove = FALSE;

	MoveDelay = sdelay;
	EndMoveDelay = edelay;
	MoveLen = 0;
	
	if(speed == -1) MoveSpeed = CARDMOVESPD_DEFAULT;    //Game.CardSpeed;
	else MoveSpeed = speed;

	if(front==1) bFrontCall = TRUE;
	
	MoveStartSnd = startsound;	//ī�尡 �����϶� ���带 �־��ش�. 
	MoveEndSnd = endsound;		//ī�尡 ���������� ���带 �־��ش�. 
			
	bShow = TRUE;				//ī�尡 �̵��ϸ� �����ش� 
}

#define RADIAN 0.0174532888f

void CCard::OnTimer()
{
	//���̵�ǥ�� ī��Ʈ
	if(nMadeCnt > 0) nMadeCnt--;
	
	if(TwincleCnt > 3) TwincleCnt-=3;
	else TwincleCnt = 0;

	if(MoveDelay>0) MoveDelay--;
	if(MoveDelay==0 && ShowMode==1) { bShow = TRUE; ShowMode=0; }
	if(bMove && MoveDelay==0)
	{
		//////////////////////////////////////////////////////////////////////////
		//## ��Ȯ�� ##//
		if( m_growth_delay > 0 ) m_growth_delay--;

		if( m_bResvGrowth && m_growth_delay <= 0)
		{
			if( m_xl < m_Max_Growth_X )
			{
				m_xl += m_subval_x;
				if( m_xl > m_Max_Growth_X )	m_xl = m_Max_Growth_X;
			}

			if( m_yl < m_Max_Growth_Y )
			{
				m_yl += m_subval_y;
				if( m_yl > m_Max_Growth_Y ) m_yl = m_Max_Growth_Y;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		
		// �̵� ���� ���� ���
		if(MoveStartSnd != NULL)
		{
			NMBASE::SOUND::g_Sound.PlayWav(MoveStartSnd);
			MoveStartSnd=NULL;
		}

		// �̵��� ������ ���¶��
		if(Xp != TXp || Yp != TYp)
		{

			//## ��Ȯ�� ##//
			m_xl = m_pHwatuNSpr->spr->xl;
			m_yl = m_pHwatuNSpr->spr->yl;
			m_bResvGrowth = FALSE;

			int w = TXp-SXp;
			int h = TYp-SYp;
			int TarLen = (int)sqrt(  (double)   ( abs(w*w) + abs(h*h) )   );
		
			MoveLen += MoveSpeed;

			if(MoveLen >= TarLen) MoveLen = TarLen;

			// ���� ���ϱ�
			double al = (double)abs(h)/(double)abs(w);
			int dx=0, dy=0;

			if(w==0 && h==0) { dx=0; dy=0; }
			else if(w==0) { dx = 0; dy = MoveLen; }
			else if(h==0) { dy = 0; dx = MoveLen; }
			else
			{
				double rad = atan(al);
				dx = (int)(cos(rad) * (double)MoveLen);
				dy = (int)(sin(rad) * (double)MoveLen);
			}

			if(w<0 && h>=0) dx *= -1;				// 2��и�
			if(w<0 && h<0) { dx *= -1; dy *= -1; }	// 3��и�
			if(w>=0 && h<0) dy *= -1;				// 4��и�

			Xp = SXp + dx;
			Yp = SYp + dy;

			// ��ǥ �Ÿ� �̻� �̵��Ͽ��ٸ� ��ǥ�������� ���� �̵�
			if(MoveLen >= TarLen) { Xp=TXp; Yp=TYp; }
		}

		if(Xp == TXp && Yp == TYp)
		{
			MoveLen = 0;

			if(bTwincle)
			{
				TwincleCnt = 20;
				bTwincle = FALSE;
			}

			// �˸� �̺�Ʈ�� ������ ó��
			if(bNotifyEvent)
			{
				//Game.OnNotifyEvent((char*)NotifyString.operator LPCTSTR(), NotifyStringColor);
				ClearNotifyEvent();
			}

			// �̵� ���� ���� ���
			if(MoveEndSnd != NULL)
			{
				NMBASE::SOUND::g_Sound.PlayWav(MoveEndSnd);
				//PlaySound(MoveEndSnd, NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
				MoveEndSnd=NULL;
			}

			if(EndMoveDelay==0)
			{
				bMove=FALSE;
				bEndMove = TRUE;

				// �̺�Ʈ�� �ִٸ� �̺�Ʈ�� ó��
				if(bEvent)
				{
					g_GAMEPROCESS()->OnSutdaCardEvent(&Event);
					ClearEvent();
				}

				if(bEvent2)
				{
					if( bMove )
						SetEvent( Event2.EventCode, Event2.Option, Event2.Option2 );
					else 
						g_GAMEPROCESS()->OnSutdaCardEvent(&Event2);

					//Game.OnCARDEVENT_SUTDA(&Event2);
					ClearEvent2();
				}

				if(bFrontCall) bFront = TRUE; 
			
				//ī�尡 �����԰� ���ÿ� ����������� ī�带 �޾Ҵ��� �˻��Ѵ�.
				if( g_GAMEPROCESS()->IsSutdaCardMoving() == FALSE )
				{
					///$$$$$
// 					CCL_ENDWORK msg;
// 					msg.Set( 1, g_MyInfo.UI.ID, g_RI.RoomNum );
// 					SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());		
				}

			}
			else if(EndMoveDelay>0) EndMoveDelay--;
		}
	}
}

void CCard::SetShow( BOOL r_show )
{
	bShow = r_show;
}

void CCard::SetFront( BOOL r_front )
{
	bFront = r_front;
}

void CCard::CardDraw(BOOL bFold, BOOL beft)
{
	if(pPage == NULL) return;
	if(bShow == FALSE) return;
	if(CardNo < 0) return;

	int sprno = (bFront)? CardNo : 0;
	int xp = Xp;
	int yp = Yp;

	//�������ϰ�� Ȳ�ݻ�ī��� �ٲ��ش�.
// 	if( sprno == 0 && g_RoomJackPot.m_JackPotActive )
// 	{
// 		sprno = 42;
// 	}

	if( beft )
	{
		//$$$$$
		//pPage->PutSprAuto(xp, yp, &Game.m_AniQue.m_AniSpr[ANIMATION_WINLOSS - ANIMIN], sprno);
	}
	else if(bRedTwincle && bFold == FALSE)
	{
		RedTwincleCnt++;
		if(RedTwincleCnt<=5) pPage->PutSprAuto(xp, yp, m_pHwatuNSpr, sprno, GLOW, 18, NMBASE::GRAPHICGDI::xSprite::RGB555(31,11,0));
		else pPage->PutSprAuto(xp, yp, m_pHwatuNSpr, sprno);
		if(RedTwincleCnt>10) RedTwincleCnt = 0;
	}
	else
	{
		if(TwincleCnt>0) pPage->PutSprAuto(xp, yp, m_pHwatuNSpr, sprno, GLOW, 31-TwincleCnt, NMBASE::GRAPHICGDI::xSprite::RGB555(31,31,31));
		else if(bFold==TRUE && bFront==FALSE)
		{
			pPage->PutSprAuto(xp, yp, m_pHwatuNSpr, sprno, GLOW, 22, NMBASE::GRAPHICGDI::xSprite::RGB555(0,0,0));
		}
		else pPage->PutSprAuto(xp, yp, m_pHwatuNSpr, sprno);
	}
	
}

// Ŭ������� ���콺 �������� ������ �ִ´�
void CCard::SetClickPos(int x, int y)
{	
	CXp = x;
	CYp = y;
}

//## ��Ȯ�� ##//
void CCard::ReserveGrowth(int growth_delay)	//?????
{
//	if( g_pMainDlg->CFG.AniEff == EFFKIND_NONE || g_pMainDlg->CFG.nCardEff == CARDEFFKIND_NONE ) return;

	m_bResvGrowth = TRUE;

	// �߰�
	m_growth_delay = growth_delay;

	m_xl = m_pHwatuNSpr->spr->xl;
	m_yl = m_pHwatuNSpr->spr->yl;
}
