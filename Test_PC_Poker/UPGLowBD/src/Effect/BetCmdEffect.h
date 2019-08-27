#pragma once

#include "../AnimationDefine.h"	//UPG작업 추가

namespace effect
{


class CBetCmdEffect : public CBaseObject
{

public:
	CBetCmdEffect(void);
	~CBetCmdEffect(void);


	typedef struct _tagEFFECTPOS
	{
		int nBigPos;
		int nSmallPos;
	}SEFFECTPOS;

public:
	
	BOOL Init(int nPlayerNum, OBJECT_TYPE eAniType, int nEffectType, BOOL bLastBetMax = FALSE);
	void SetSmallCmd();
	void SetDelTime(int ndel)
	{
		if(ndel <=0)
		{
			ndel = 0;
		}
		m_nDestroyDelTime = ndel;
	};

	SEFFECTPOS m_sEffectPos[ _ANF_SEPERATOR1_ ];	// 하프/콜/체크/따당/삥/다이/올인 순서 기록( 5개씩 이동한다.)
	CPoint GetEffectPos(void);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);

	int GetPlayerNumber(){return m_PlayerNumber;}

private:		
	NMBASE::SKINGDI::CNMultiScene *m_pEffectScene;

	int		Xp,Yp;
	int		eXp,eYp;
	int		m_PlayerNumber;			// 플레이어 위치
	int     m_nBetEffectBigPos;		// 큰 이펙트 위치
	int     m_nBetEffectSmallPos;	// 작은 이펙트 위치

	BOOL    bDifferntBetStyle;		// 3인 2인일때 콜 하프 금액

	DWORD	m_nStartTime;
	DWORD	m_nEndTime;	
	int		m_nDestroyDelTime;	

};

}


