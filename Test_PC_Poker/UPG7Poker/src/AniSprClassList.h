// AniSprClassList.h: interface for the AniSprClassList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_)
#define AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "Player7Poker.h"
#include "AnimationDefine.h"	//UPG작업 추가


class CBettingEffect : public CBaseObject
{
	typedef struct _tagEFFECTPOS
	{
		int nBigPos;
		int nSmallPos;
	}SEFFECTPOS;

public:
	CBettingEffect();
	virtual ~CBettingEffect();

	BOOL Init(int nPlayerNum, ENUM_ANIFILEID eAniType, int nEffectType, BOOL bLastBetMax = FALSE);
	void SetSmallCmd();
	void SetDelTime(int ndel)
	{
		if(ndel <=0)
		{
			ndel = 0;
		}
		m_nDestroyDelTime = ndel;
	};

	SEFFECTPOS m_sEffectPos[ ANF_BET_RAISE_X3 - ANF_BET_CMD ];	// 하프/콜/체크/따당/삥/다이/올인 순서 기록( 5개씩 이동한다.)

	CPoint GetEffectPos(void);
	//void MakeVIPJackpotPoint(int nPoint);

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

	int		m_nSpriteIndex;
	
};


// RoyalST(0), RoyalST&Jackpot(1), BackST(2), ST(3), 4card(4)
class CJokboEffect : public CBaseObject
{
public:	
	int		m_nDelTime;
	BOOL	m_bJokboShow;	
	int		m_nMoneyDeck[30];			// 스프라이트 인덱스
	int		m_nTotalMoneyIndex;
	GAMERESULTDATA m_sResultData;	
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;
private:
	int m_nSprindex;
	DWORD m_dwWinDrawTime;

public:
	CJokboEffect();
	virtual ~CJokboEffect();

	void Init( GAMERESULTDATA *data = NULL);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);	
};


//-----------------------(공통 관전자 이미지 etc.spr mc = 0) -----------------------//	

//ANF_SDLG_GRADE
class CGradeSpr : public CBaseObject
{
public:

	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;	//응모권 증가 효과
	DWORD m_dwDelay;	

	NMBASE::GRAPHICGDI::CPage		*m_pPage;

	CGradeSpr();
	virtual ~CGradeSpr();

	BOOL Init(int index , NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};



//채팅창 찬스 메세지 지급 받을때 같이 보여준다.
class CChance_Msg_EffectSpr :public CBaseObject
{
public:
	int nItemCode;

	char	m_ChanceItemText[MAX_PATH];
	int		nDelTime;
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;

	CChance_Msg_EffectSpr();
	virtual ~CChance_Msg_EffectSpr();

	void Init(int itemcode);	

	void get_ChanceText(int itemCode);
	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);	
};



class CMadeEffect :public CBaseObject
{
public:

	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;
	DWORD nDelay;	

	CMadeEffect();
	virtual ~CMadeEffect();

	BOOL Init(int nCardCnt);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);

protected:
	NMBASE::GRAPHICGDI::xSprite* m_pSpr;
	int m_nCardCnt;

};
#endif // !defined(AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_)
