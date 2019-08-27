// AniFileListMan.h: interface for the CAniFileListMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIFILELISTMAN_H__65DB3EB5_02E7_44B4_AE8E_550D0B8EC014__INCLUDED_)
#define AFX_ANIFILELISTMAN_H__65DB3EB5_02E7_44B4_AE8E_550D0B8EC014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <../src/BaseObject.h>

class CBetCmdAni  :public CBaseObject
{
public:
	int		m_nPNum;

	int		m_nBetCmd;
	BOOL	m_bSmallCmd;

	DWORD	m_dwStartTime;
	DWORD	m_dwEndTime;
	int		m_nDelTime;

	CBetCmdAni();
	virtual ~CBetCmdAni();

	BOOL Init(int pnum,ENUM_ANIFILE enumkind,int betcmd, bool bHighlight);
	CPoint GetEffectPos(void);

	void SetSmallCmd();
	void SetDelTime(int ndel)
	{
		if (ndel <= 0)
		{
			ndel = 0;
		}

		m_nDelTime = ndel;
	};

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};

//ANF_SDLG_GRADE
class CGradeSpr :public CBaseObject
{
public:
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;	//응모권 증가 효과
	int			m_nDelay;	
	
	NMBASE::GRAPHICGDI::CPage		*m_pPage;
	
	CGradeSpr();
	virtual ~CGradeSpr();
	
	BOOL Init(int index , NMBASE::GRAPHICGDI::CPage *pPage);
	
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

#endif // !defined(AFX_ANIFILELISTMAN_H__65DB3EB5_02E7_44B4_AE8E_550D0B8EC014__INCLUDED_)
