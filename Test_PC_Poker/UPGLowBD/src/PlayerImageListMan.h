// PlayerImageListMan.h: interface for the CPlayerImageListMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERIMAGELISTMAN_H__D7D7AF70_BD3A_4643_BF11_2C54FB3CC038__INCLUDED_)
#define AFX_PLAYERIMAGELISTMAN_H__D7D7AF70_BD3A_4643_BF11_2C54FB3CC038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//뉴도신지수 2006.3.2
class CJekiChargeEffect  :public CBaseObject
{
public:
	
	NMBASE::GRAPHICGDI::xSprite *m_pSpr;
	int PNum;
	int	nSaveCnt;//남은 기회 인덱스
	INT64 nChargeMoney;	
	int nOptionKind;
	DWORD nStartTime;
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;
	
	CJekiChargeEffect();
	virtual ~CJekiChargeEffect();

	void Init(INT64 ChargeMoney, int SaveCnt,int pnum , NMBASE::GRAPHICGDI::xSprite *pSpr = NULL ,int optionkind = 0);
	void ChargeDrawText(CDC *pDC);

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

class CCuttingInfo_BD  :public CBaseObject
{
public:		

	DWORD	nDelayTime;	
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;

	CCuttingInfo_BD();
	virtual ~CCuttingInfo_BD();


	void Init( int nPNum, int nCutCnt );

	virtual void DrawObject(CDC *pDC);
	virtual void Destroy();
	virtual int  ProcessObject();
};

class CBlindBet_BD  :public CBaseObject
{
public:		
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;

	CBlindBet_BD();
	virtual ~CBlindBet_BD();

	void Init( int nPNum );

	virtual void DrawObject(CDC *pDC);
	virtual void Destroy();
	virtual int  ProcessObject();
};

#endif // !defined(AFX_PLAYERIMAGELISTMAN_H__D7D7AF70_BD3A_4643_BF11_2C54FB3CC038__INCLUDED_)
