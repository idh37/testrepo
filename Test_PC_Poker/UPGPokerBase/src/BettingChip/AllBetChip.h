// AllBetChip.h: interface for the CAllBetChip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLBETCHIP_H__C13D68C6_5C8F_4E97_8EFD_E0DDBFF4EB99__INCLUDED_)
#define AFX_ALLBETCHIP_H__C13D68C6_5C8F_4E97_8EFD_E0DDBFF4EB99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../shared/CardDefine.h"


// 7��Ŀ, ����Ŀ �ڵ� ���̰� �־� ����� �ּ�ó���� �ϰڽ��ϴ�. (���߿� ���� ���� ���Ѿ� ��)
// NMBASE::SOUND::g_Sound.PlayWav(SND54);	// �� ���ñݾ� �̵�
// NMBASE::SOUND::g_Sound.PlayWav(SND56);	// ����� ī�� �̵�	

class NMBASE::GRAPHICGDI::CPage;
class CUIInfoData;
class CSidePotMoney;

class UPG_GAMEBASE_EXPORT CAllBetChip  
{
protected:
	enum ENUM_ALLBETCHIP_MOVESTATS
	{
	EABCMS_UNKNOWN = -1,
	EABCMS_CENTERDRAW = 0,		//Ĩ�� �߾ӿ� �̵����� �ʰ� ����
	EABCMS_WAIT,				//��� ǥ���� Ĩ�� �̵��� �Ǳ����� ��ٸ�
	EABCMS_MOVE,				//Ĩ�� �̵�
	EABCMS_FADEOUT,				//�̵��� ������ Ĩ�� ������� ��
	EABCMS_END,					//Ĩ�� ������� �̵��� ������.
	};

	struct USER_GETMONEY_DATA
	{
	int		nPNum;
	INT64	i64Money;
	INT64	i64ResultMoney;		//��� �Ӵ�, ������ ���ų� ���� �ݾ�
	CPoint	ptTarget;
	CPoint	ptStart;
	CPoint	ptCur;
	};

	ENUM_ALLBETCHIP_MOVESTATS	m_nStatus;
	list<USER_GETMONEY_DATA>	m_listUserGetMoneyData;
	list<int>					m_listDrawOrder;

	CHIPDATA		m_listChipData[EndChip];			// Ĩ ����Ÿ

	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	NMBASE::GRAPHICGDI::xSprite*		m_pChipSpr;							// Ĩ ��������Ʈ
	CUIInfoData		*m_pUIInfoData;
	CGameProcess	*m_pGameProcess;

	NMBASE::GRAPHICGDI::xSprite			m_sprSideMoney;

	int				m_nChipHeight;
	DWORD			m_dwMoveStart;
	DWORD			m_dwMoveTime;
	DWORD			m_dwFadeOutTime;
	INT64			m_i64Money;
	int				m_nAlpha;
	int				m_nGoldIndex;

	DWORD			m_dwMoveStartTime;
public:
	CAllBetChip();
	virtual ~CAllBetChip();

	void LoadImage(void);
	// Page, UPGPokerBase�� UIInfo, GameProcess, // ��� ���� ��������Ʈ, Ĩ ��������Ʈ
	void Init(NMBASE::GRAPHICGDI::CPage* page, CUIInfoData* pUIInfoData, CGameProcess* pGameProcess, NMBASE::GRAPHICGDI::xSprite* pChipSpr); // ���� �ʱ�ȭ �Լ�
	void Reset();								// ���� 

	void OnTimer();
	void OnMove(DWORD dwCurTime);
	void OnFadeOut(DWORD dwCurTime);

	void SetMoney(INT64 tmoney);					// �� ���øӴ� ����
	void SetWinner(CSidePotMoney *pSidePotResult);	// ���� ����(�̵��Ѵ�)
		
	void DrawBottom();								// ���� ��ο�
	void DrawTop();									// ���� ��ο�
protected:
	void DrawCenterChip(void);
	void DrawMoveAllChip(void);
	void DrawMoveChip(USER_GETMONEY_DATA &data);
};

#endif // !defined(AFX_ALLBETCHIP_H__C13D68C6_5C8F_4E97_8EFD_E0DDBFF4EB99__INCLUDED_)
