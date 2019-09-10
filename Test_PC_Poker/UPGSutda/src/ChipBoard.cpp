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
	//��������Ʈ �б�
	AddLoadImageList(&m_sprChip,".\\Sutda\\data\\Chip.spr", true);

}


void CChipBoard::Reset()
{
	if (m_pSprBack==NULL)
		return;

	// ���� Ĩ �������� �����
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
	//## �����̴� Ĩ �׸��� ##//
	for(i=0; i<MAX_CHIP; i++)
		m_Chip[i].Draw();



}


//�̹� ������ Ĩ�� �׸���.
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

				int stx = 370;	     //��ġ
				int sty = 146;
				int w = 200;		 //���� 280;
				int radius = w/2;
				int erad = radius-20;// ȿ��

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

					tx = tx-(332); // ���� ������ ��ǥ��BETPAGE_X
					ty = ty-(115); //BETPAGE_Y
					//tx = tx-(235); // ���� ������ ��ǥ��BETPAGE_X
					//ty = ty-(27); //BETPAGE_Y

					int sprno = i*2+kind;
					m_BoardPage.PutSprAuto(tx, ty, &m_sprChip, sprno);
			}
			else zcnt++;//7 Ĩ ���� �ɱ�
		}
		if(zcnt>=MAX_CHIPCNT) break;
	}
}

void CChipBoard::CastChip(int pnum, INT64 nMarble, int sdelay, INT64 llCurMoney)	//Ĩ������ ��ġ 
{
	if(pnum < 0 || pnum >= MAX_PLAYER) 
		return;

	SetBetEft(nMarble, llCurMoney);

	INT64 marble = nMarble;
	int cn[MAX_CHIPCNT]={0,};

	//cn = SetCastChipCnt( nMarble );

	//���ڸ��� �����ΰ� ����Ѵ�. 
	INT64 sumarble = nMarble;
	int sucnt = 0;
	
	//������ ���ε����� ���� ����, �������� ���������� �ѹ��� ���´�.
	//if( 0 >= Play[pnum].UI.PMoney - nMarble && Play[pnum].UI.PMoney > 0)
	if( llCurMoney <= 0)
	{
		//ù���ι��ýø� �������� ������.
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
	
		//���� �ڸ����� ������ �ѷ��ش�. 
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
					int stx = 370;	     //��ġ
					int sty = 146;
					int w = 200;		 //���� 280;
					int radius = w/2;
					int erad = radius-20;// ȿ��
					
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
					
					//	tx = tx-(500); // ���� ������ ��ǥ��
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



//���ÿ� ���� ȿ���� ����Ĩ�Ҹ�
void CChipBoard::SetBetEft( INT64 r_money , INT64 llCurMoney)
{
	//���ι��ý� ���̼Ҹ�
	if( 0 >=  llCurMoney - r_money)
	{
		NMBASE::SOUND::g_Sound.PlayWav( SND09 );
	}
	else
	{
		if(r_money > 0 &&  r_money < 1000000)	//�鸸���̸��� ���� ���ʱ���������  
		{
			NMBASE::SOUND::g_Sound.PlayWav( SND06 );
		}
		else if( r_money >= 1000000 && r_money < 10000000000)	//�鸸~���(���̸�)
		{
			NMBASE::SOUND::g_Sound.PlayWav( SND07 );
		}
		else if( r_money >= 10000000000)	//1����̻� �ݱ��Ҹ�
		{
			NMBASE::SOUND::g_Sound.PlayWav( SND08 );
		}
	}
}