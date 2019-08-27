// AllBetChip.h: interface for the CAllBetChip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLBETCHIP_H__C13D68C6_5C8F_4E97_8EFD_E0DDBFF4EB99__INCLUDED_)
#define AFX_ALLBETCHIP_H__C13D68C6_5C8F_4E97_8EFD_E0DDBFF4EB99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../shared/CardDefine.h"


// 7포커, 뉴포커 코드 차이가 있어 현재는 주석처리를 하겠습니다. (나중에 원상 복귀 시켜야 함)
// NMBASE::SOUND::g_Sound.PlayWav(SND54);	// 총 배팅금액 이동
// NMBASE::SOUND::g_Sound.PlayWav(SND56);	// 결과때 카드 이동	

class NMBASE::GRAPHICGDI::CPage;
class CUIInfoData;
class CSidePotMoney;

class UPG_GAMEBASE_EXPORT CAllBetChip  
{
protected:
	enum ENUM_ALLBETCHIP_MOVESTATS
	{
	EABCMS_UNKNOWN = -1,
	EABCMS_CENTERDRAW = 0,		//칩이 중앙에 이동하지 않고 보임
	EABCMS_WAIT,				//결과 표시중 칩의 이동이 되기전에 기다림
	EABCMS_MOVE,				//칩의 이동
	EABCMS_FADEOUT,				//이동이 끝나고 칩이 사라지는 중
	EABCMS_END,					//칩이 사라지고 이동이 끝났다.
	};

	struct USER_GETMONEY_DATA
	{
	int		nPNum;
	INT64	i64Money;
	INT64	i64ResultMoney;		//결과 머니, 실제로 따거나 잃은 금액
	CPoint	ptTarget;
	CPoint	ptStart;
	CPoint	ptCur;
	};

	ENUM_ALLBETCHIP_MOVESTATS	m_nStatus;
	list<USER_GETMONEY_DATA>	m_listUserGetMoneyData;
	list<int>					m_listDrawOrder;

	CHIPDATA		m_listChipData[EndChip];			// 칩 데이타

	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	NMBASE::GRAPHICGDI::xSprite*		m_pChipSpr;							// 칩 스프라이트
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
	// Page, UPGPokerBase의 UIInfo, GameProcess, // 결과 숫자 스프라이트, 칩 스프라이트
	void Init(NMBASE::GRAPHICGDI::CPage* page, CUIInfoData* pUIInfoData, CGameProcess* pGameProcess, NMBASE::GRAPHICGDI::xSprite* pChipSpr); // 최초 초기화 함수
	void Reset();								// 리셋 

	void OnTimer();
	void OnMove(DWORD dwCurTime);
	void OnFadeOut(DWORD dwCurTime);

	void SetMoney(INT64 tmoney);					// 총 배팅머니 셋팅
	void SetWinner(CSidePotMoney *pSidePotResult);	// 승자 셋팅(이동한다)
		
	void DrawBottom();								// 배팅 드로우
	void DrawTop();									// 배팅 드로우
protected:
	void DrawCenterChip(void);
	void DrawMoveAllChip(void);
	void DrawMoveChip(USER_GETMONEY_DATA &data);
};

#endif // !defined(AFX_ALLBETCHIP_H__C13D68C6_5C8F_4E97_8EFD_E0DDBFF4EB99__INCLUDED_)
