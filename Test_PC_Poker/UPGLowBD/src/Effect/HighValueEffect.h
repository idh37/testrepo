#pragma once

#include "../AnimationDefine.h"	//UPG�۾� �߰�

namespace effect
{

class CHighValueEffect  :public CBaseObject
{
public:	
	BOOL	bShowValue;	
	int		nMoneyDeck[30];			// ��������Ʈ �ε���
	int		nTotalMoneyIndex;
	GAMERESULTDATA m_stResultData;	
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;
	NMBASE::SKINGDI::CNMultiScene *m_pMultiSceneJackpot;
	NMBASE::GRAPHICGDI::xSprite *pSpr;

	CRect	m_rtstrID;				// ����rect
	CRect	m_rtstrJokbo;		    // ����rect
	CRect   m_rtstrMoney;			// �Ӵ�rect
	std::string m_strMoney;			// �Ӵ�

	CRect	m_rtJackPot;
	std::string m_strJackPotMoney;  // ���� �Ӵ�

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

