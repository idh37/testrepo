#include "StdAfx.h"
#include "ChipBoard.h"
#include "GlobalGame.h"

CChipBoard::CChipBoard(void)
{

	m_pSprBack = NULL;
	m_pMainPage = NULL;
}

CChipBoard::~CChipBoard(void)
{

}

void CChipBoard::LoadImage()
{
	//스프라이트 읽기
	AddLoadImageList(&m_sprChip,".\\Sutda\\data\\Chip.spr", true);

}


void CChipBoard::Reset()
{
	if (m_pSprBack==NULL)
		return;

	// 배팅 칩 게임판을 지운다
	m_BoardPage.PutSprAuto(0, 0, m_pSprBack, 1);	

	for(int i=0; i<MAX_CHIP; i++)
		m_Chip[i].Reset();


	ZeroMemory(m_AllinBetting, sizeof(m_AllinBetting));

}


void CChipBoard::Init(NMBASE::GRAPHICGDI::CPage *pMainPage, NMBASE::GRAPHICGDI::xSprite* pSprBack)
{
	m_pSprBack = pSprBack;
	m_pMainPage = pMainPage;

	const int nSprNo_Board = 1;
	m_BoardPage.Init(pSprBack->spr[nSprNo_Board].xl, pSprBack->spr[nSprNo_Board].yl, 16);		//320, 288,


	for(int i=0; i<MAX_CHIP; i++) 
		m_Chip[i].Init(pMainPage, &m_BoardPage, &m_sprChip);

	Reset();

}

void CChipBoard::OnTimer()
{
	int i=0;
	for(i=0; i<MAX_CHIP; i++)
	{
		m_Chip[i].OnTimer();
	}

}

void CChipBoard::Draw_Bottom()
{
	if (m_pMainPage==NULL)
		return;

	m_pMainPage->PutFromPage(g_cUIData.m_ptBetBoard.x, g_cUIData.m_ptBetBoard.y, &m_BoardPage);

}


void CChipBoard::Draw_Top()
{
	
	int i=0;
	//## 움직이는 칩 그리기 ##//
	for(i=0; i<MAX_CHIP; i++)
		m_Chip[i].Draw();



}


//이미 던져진 칩을 그린다.
void CChipBoard::PreDrawCastedChip(INT64 llTotalBet)	
{	
	if (llTotalBet<=0)
		return;


	///////////////////////////////////////////////////////////////////////////

	int i;
	int cnt[MAX_CHIPCNT]={0,};
		

	INT64 llTempMoney = llTotalBet;
	for(int i=0; i<MAX_CHIPCNT; i++) 	
	{
		if (llTempMoney<=0)
			break;

		int nLastUnit = (int) (llTempMoney%10);
		cnt[i]=nLastUnit;
		llTempMoney = llTempMoney / 10;
		
	}


	
	for(;;)
	{
		int zcnt = 0;
		for(i=0; i<MAX_CHIPCNT; i++) 
		{
			if(cnt[i] > 0) {
				cnt[i]--;
				int kind = abs(rand()%2);

				//int tx = rand()%210;
				//int ty = rand()%210;

				int stx = 370;	     //위치
				int sty = 146;
				int w = 200;		 //지름 280;
				int radius = w/2;
				int erad = radius-20;// 효과

				int cx = stx + radius;
				int cy = sty + radius;
				
				int tx, ty;

				for(;;)
				{
					tx = stx+rand()%w;
					ty = sty+rand()%w;

					int xp = tx - cx;
					int yp = ty - cy;

					if(xp*xp + yp*yp < erad*erad) {
						break;
					}

					if(xp*xp + yp*yp < radius*radius) {
						if(rand()%5 == 0) break;
					}
				}

					tx = tx-(332); // 작은 페이지 좌표로BETPAGE_X
					ty = ty-(115); //BETPAGE_Y
					//tx = tx-(235); // 작은 페이지 좌표로BETPAGE_X
					//ty = ty-(27); //BETPAGE_Y

					int sprno = i*2+kind;
					m_BoardPage.PutSprAuto(tx, ty, &m_sprChip, sprno);
			}
			else zcnt++;//7 칩 제한 걸기
		}
		if(zcnt>=MAX_CHIPCNT) break;
	}
}

void CChipBoard::CastChip(int pnum, INT64 nMarble, int sdelay, INT64 llCurMoney)	//칩던져질 위치 
{
	if(pnum < 0 || pnum >= MAX_PLAYER) 
		return;

	SetBetEft(nMarble, llCurMoney);

	INT64 marble = nMarble;
	int cn[MAX_CHIPCNT]={0,};

	//cn = SetCastChipCnt( nMarble );

	//몇자리의 숫자인가 계산한다. 
	INT64 sumarble = nMarble;
	int sucnt = 0;
	
	//유저가 올인됐을땐 올인 배팅, 집문서는 돈이있을때 한번만 나온다.
	//if( 0 >= Play[pnum].UI.PMoney - nMarble && Play[pnum].UI.PMoney > 0)
	if( llCurMoney <= 0)
	{
		//첫올인배팅시만 집문서가 나간다.
		if( !m_AllinBetting[pnum] )
		{
			cn[MAX_CHIPCNT-1] = 1;
			m_AllinBetting[pnum] = TRUE;
		}
		
	}
	else 
	{
		for( int j = 0; j < MAX_CHIPCNT - 2; j++)
		{
			INT64 pp = sumarble/10;
			if( pp > 0)
			{
				sumarble = pp;
				sucnt++;
			}
			else break;
		}		
	
		//같은 자릿수의 갯수만 뿌려준다. 
		cn[sucnt]=(int)sumarble;
	}

	
	int sx=0, sy=0;
	

	sx = g_cUIData.m_listChipStartPos[pnum].x;
	sy = g_cUIData.m_listChipStartPos[pnum].y;

	int i=0;
	int nowchip=0;
	for( i=MAX_CHIPCNT-1; i>=0; i--)//6
	{
		for(int j=0; j<cn[i]; j++)
		{
			int n=0;
			for(n=nowchip; n<MAX_CHIP; n++)
			{
				nowchip = n;
				if(m_Chip[n].bLive == FALSE)
				{
					int kind = abs(rand()%2);
					//					int tx = 306+rand()%160;
					//					int ty = 157+rand()%190;
					//					int tx = 225+g_CX+rand()%160;
					//---		
					int stx = 370;	     //위치
					int sty = 146;
					int w = 200;		 //지름 280;
					int radius = w/2;
					int erad = radius-20;// 효과
					
					int cx = stx + radius;
					int cy = sty + radius;
					
					int tx, ty;
					
					for(;;)
					{
						tx = stx+rand()%w;
						ty = sty+rand()%w;
						
						int xp = tx - cx;
						int yp = ty - cy;
						
						if(xp*xp + yp*yp < erad*erad) {
							break;
						}
						
						if(xp*xp + yp*yp < radius*radius) {
							if(rand()%5 == 0) break;
						}
					}
					
					//	tx = tx-(500); // 작은 페이지 좌표로
					//	ty = ty-(270); //
				
				//---
				//int tx = 200+g_CX+rand()%210;
				//int ty = 120+g_CY+rand()%210;

				m_Chip[n].SetChip(i, kind);
				m_Chip[n].SetPos(sx, sy);
	//			tx = 530; ty = 330;
				m_Chip[n].SetMove(tx, ty, sdelay);
				break;
				}
				
			}
			if(n == MAX_CHIP) return;
		}		
	}
}



//배팅에 따른 효과음 배팅칩소리
void CChipBoard::SetBetEft( INT64 r_money , INT64 llCurMoney)
{
	//올인배팅시 종이소리
	if( 0 >=  llCurMoney - r_money)
	{
		NMBASE::SOUND::g_Sound.PlayWav( SND09 );
	}
	else
	{
		if(r_money > 0 &&  r_money < 1000000)	//백만원미만은 동전 구십구만원까지  
		{
			NMBASE::SOUND::g_Sound.PlayWav( SND06 );
		}
		else if( r_money >= 1000000 && r_money < 10000000000)	//백만~백억(백억미만)
		{
			NMBASE::SOUND::g_Sound.PlayWav( SND07 );
		}
		else if( r_money >= 10000000000)	//1백억이상 금괴소리
		{
			NMBASE::SOUND::g_Sound.PlayWav( SND08 );
		}
	}
}