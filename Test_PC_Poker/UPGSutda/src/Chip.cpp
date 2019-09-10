// Chip.cpp: implementation of the CChip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalGame.h"
#include "Chip.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChip::CChip()
{
	m_pChipSpr = NULL;
	pPage = NULL;
	pBatPage = NULL;
	Reset();
}

CChip::~CChip()
{

}

void CChip::Init(NMBASE::GRAPHICGDI::CPage *ppage, NMBASE::GRAPHICGDI::CPage *pbatpage, NMBASE::GRAPHICGDI::xSprite *pChipSpr)
{
	Reset();
	bLive = FALSE;
	pPage = ppage;
	pBatPage = pbatpage;
	ChipNo = 0;
	Kind = 0;
	SprNo = 0;
	TwincleCnt = 0;

	m_pChipSpr = pChipSpr;
}

void CChip::SetChip(int chipno, int kind)
{
	Reset();
	if(chipno<0 || chipno >= MAX_CHIPCNT) //7
	{
		return;
	}
	if(kind < 0 || kind >= 3)
	{
		return;
	}

	ChipNo = chipno;
	Kind = kind;
	bLive = TRUE;
	SprNo = ChipNo*2+Kind;
	//bLive = TRUE;
}

void CChip::Reset()
{
	ChipNo = 0;

	MoveStartSnd = NULL;
	MoveEndSnd = NULL;

	bLive = FALSE;
	ChipNo = 0;
	Kind = 0;
	SprNo = 0;

	bTwincle = FALSE;
	TwincleCnt = 0;
	
	bMove = FALSE;

	Xp = Yp = 0;
	MoveLen = 0;
	SXp = SYp = 0;
	TXp = TYp = 0;
	MoveDelay = 0;
	EndMoveDelay = 0;
	MoveSpeed = 10; //MoveSpeed = 40;
}

void CChip::SetPos(int tx, int ty)
{
	SXp = TXp = Xp = tx;
	SYp = TXp = Yp = ty;
	MoveLen = 0;
}

void CChip::SetMoveStartSnd(char *sndfilename)
{
	MoveStartSnd = sndfilename;
}

void CChip::SetMoveEndSnd(char *sndfilename)
{
	MoveEndSnd = sndfilename;
}

void CChip::SetMove(int tx, int ty, int sdelay, int edelay, int speed)
{
	SXp = Xp; SYp = Yp;
	TXp = tx; TYp = ty;
	bMove = TRUE;
	MoveDelay = sdelay;
	EndMoveDelay = edelay;
	if(speed == -1) MoveSpeed = 20;	//40
	else MoveSpeed = speed;
	MoveLen = 0;
}

#define RADIAN 0.0174532888f

void CChip::OnTimer()
{
	if(bLive==FALSE) return;
	if(MoveDelay>0) MoveDelay--;

	if(bMove && MoveDelay==0)
	{
		// �̵��� ������ ���¶��
		if(Xp != TXp || Yp != TYp)
		{
			int w = TXp-SXp;
			int h = TYp-SYp;
			int TarLen = (int)sqrt( (double)(abs(w*w) + abs(h*h)) );

			// ���� �Ÿ��� 1/2 �����϶����� ���ӿ ����
			if(MoveLen > TarLen/2)
			{
				int halflen = max(1,TarLen/2);
				int leftlen = TarLen - MoveLen;

				int movdot = (int)(MoveSpeed*((float)leftlen/halflen))+1;
				MoveLen += movdot;
			}
			else MoveLen += MoveSpeed;

			if(MoveLen >= TarLen) MoveLen = TarLen;

			TwincleCnt = (int)((float)MoveLen/TarLen*100.0f*0.31f);
			if(TwincleCnt > 31) TwincleCnt=31;


			// ���� ���ϱ�
//			double al = (double)abs(h)/(double)abs(w);
			int dx=0, dy=0;

			if(w==0 && h==0) { dx=0; dy=0; }
			else if(w==0) { dx = 0; dy = MoveLen; }
			else if(h==0) { dy = 0; dx = MoveLen; }
			else
			{
				// ���� ���ϱ�
				double al = (double)abs(h)/(double)abs(w);
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
			bLive = FALSE;

			if(EndMoveDelay==0)
			{
				bMove=FALSE;	//332 115
				pBatPage->PutSprAuto(Xp-(332), Yp-(115), m_pChipSpr, SprNo);
				//pBatPage->PutSprAuto(Xp-(340), Yp-(115), &ChipSpr, SprNo);
			}
			else if(EndMoveDelay>0) EndMoveDelay--;
		}
	}
}

void CChip::Draw()
{
	if(pPage == NULL) return;
	if(ChipNo < 0) return;
	if(bLive==FALSE) return;

	int sprno = SprNo;
	int xp = Xp;
	int yp = Yp;

	if(TwincleCnt==0) return;
	else if(TwincleCnt<31) pPage->PutSprAuto(xp, yp, m_pChipSpr, sprno, GB, TwincleCnt, NMBASE::GRAPHICGDI::GetRGBmix(31,31,31));
	else pPage->PutSprAuto(xp, yp, m_pChipSpr, sprno);
}