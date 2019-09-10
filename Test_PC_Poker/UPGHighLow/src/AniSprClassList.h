// AniSprClassList.h: interface for the AniSprClassList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_)
#define AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "PlayerHighLow.h"
#include "AnimationDefine.h"	//UPG�۾� �߰�


enum EHIGHLOWSWINGCASE;


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

	SEFFECTPOS m_sEffectPos[ANF_BET_RAISE_X3 - ANF_BET_CMD];	// ����/��/üũ/����/��/����/���� ���� ���( 5���� �̵��Ѵ�.)

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);

	int GetPlayerNumber(){return m_PlayerNumber;}
	CPoint GetEffectPos(void);

private:		
	NMBASE::SKINGDI::CNMultiScene *m_pEffectScene;

	int		Xp,Yp;
	int		eXp,eYp;
	int		m_PlayerNumber;			// �÷��̾� ��ġ
	int     m_nBetEffectBigPos;		// ū ����Ʈ ��ġ
	int     m_nBetEffectSmallPos;	// ���� ����Ʈ ��ġ

	BOOL    bDifferntBetStyle;		// 3�� 2���϶� �� ���� �ݾ�

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
	int		m_nMoneyDeck[30];			// ��������Ʈ �ε���
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


//-----------------------(���� ������ �̹��� etc.spr mc = 0) -----------------------//	

//ANF_SDLG_GRADE
class CGradeSpr : public CBaseObject
{
public:

	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;	//����� ���� ȿ��
	DWORD m_dwDelay;	

	NMBASE::GRAPHICGDI::CPage		*m_pPage;

	CGradeSpr();
	virtual ~CGradeSpr();

	BOOL Init(int index , NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};



//ä��â ���� �޼��� ���� ������ ���� �����ش�.
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



//-----------------------(���̷ο�) (�߰��ڵ�)-----------------------//	

//ANF_ASK_SELECTWINNERCASE,	//���� , �ο� ���� ���� �϶�,
class CSelectWinnerCaseEft_HI  :public CBaseObject
{
public:		

	DWORD	nDelayTime;	
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;

	CSelectWinnerCaseEft_HI();
	virtual ~CSelectWinnerCaseEft_HI();

	//bAskSelect = TRUE		���̷ο� ���� �϶�
	//bAskSelect = FALSE	���̷ο� ������ �Ϸ� �ߴ�.
	void Init( int pnum , BOOL bAskSelect );

	virtual void DrawObject(CDC *pDC);
	virtual void Destroy();
	virtual int  ProcessObject();
};


//ANF_RESULTSELECTCASE_HI,	//��� ó�� �� ���� , �ο� ���� ���� ȿ��,
class CResultSelectCaseEft_HI  :public CBaseObject
{
public:		

	DWORD	nDelayTime;	
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;

	CResultSelectCaseEft_HI();
	virtual ~CResultSelectCaseEft_HI();

	//resultcase = 1	����
	//resultcase = 2	�ο�
	//resultcase = 3	����
	void Init( int pnum , EHIGHLOWSWINGCASE eResultCase );

	virtual void DrawObject(CDC *pDC);
	virtual void Destroy();
	virtual int  ProcessObject();
};

#endif // !defined(AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_)
