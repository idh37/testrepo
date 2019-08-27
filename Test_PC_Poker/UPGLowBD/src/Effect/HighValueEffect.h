#pragma once

#include "../AnimationDefine.h"	//UPG작업 추가

namespace effect
{

class CHighValueEffect  :public CBaseObject
{
public:	
	BOOL	bShowValue;	
	int		nMoneyDeck[30];			// 스프라이트 인덱스
	int		nTotalMoneyIndex;
	GAMERESULTDATA m_stResultData;	
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;
	NMBASE::SKINGDI::CNMultiScene *m_pMultiSceneJackpot;
	NMBASE::GRAPHICGDI::xSprite *pSpr;

	CRect	m_rtstrID;				// 유저rect
	CRect	m_rtstrJokbo;		    // 족보rect
	CRect   m_rtstrMoney;			// 머니rect
	std::string m_strMoney;			// 머니

	CRect	m_rtJackPot;
	std::string m_strJackPotMoney;  // 잭팟 머니

	bool    m_bJackpot;

	DWORD   m_dwWinDrawTime;
	DWORD   m_dwCurTime;

	CHighValueEffect();
	virtual ~CHighValueEffect();

	void Init( GAMERESULTDATA *data = NULL, bool bJackpPot = false );

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);	
};

}

