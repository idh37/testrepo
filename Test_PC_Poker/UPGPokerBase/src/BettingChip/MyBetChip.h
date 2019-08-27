// MyBetChip.h: interface for the CMyBetChip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBETCHIP_H__6B5FA6B5_5539_4E64_9A89_44FBC10BEC1C__INCLUDED_)
#define AFX_MYBETCHIP_H__6B5FA6B5_5539_4E64_9A89_44FBC10BEC1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../shared/CardDefine.h"

class NMBASE::GRAPHICGDI::CPage;
class CUIInfoData;
class NMBASE::GRAPHICGDI::xSprite;

class UPG_GAMEBASE_EXPORT CMyBetChip  
{
private:
	NMBASE::GRAPHICGDI::CPage		*m_pPage;
	NMBASE::GRAPHICGDI::xSprite		*m_pChipSpr;				// 칩 스프라이트
	MYCHIPDATA	m_listMyChipData[EndChip];	// 칩 데이타
	CUIInfoData	*m_pUIInfoData;

	CPoint		m_ptChipStart;				// 그려질 칩 위치 좌표

	bool		m_bChipShow;				// 그려질 여부
	CPoint		m_ptPos;					// 그려질 위치
	CPoint		m_ptStart;					// 이동 시작 위치
	bool		m_bMoveStart;				// 이동 시작을 알림 
	bool		m_bMoveEnd;					// 이동 끝을 알림 TRUE 일때 토탈 배팅을 셋한다.
	int			m_nPNum;
public:
	CMyBetChip();
	virtual ~CMyBetChip();

	// 칩 초기화시에 NMBASE::GRAPHICGDI::CPage, 칩 스프라이트 포인터, 플레이어 위치를 입력해 준다.
	virtual void Init(NMBASE::GRAPHICGDI::CPage* pPage, NMBASE::GRAPHICGDI::xSprite* pChipSpr, int nPNum, CUIInfoData *pUInfoData);
	virtual void ReSet();									// 초기 

	// 칩을 표시할 공간 때문에 상위 3자리만 가지고 값을 채움 Ex: 130억1345만..  이면 130억만 값 셋팅
	// 배팅 금액을 받아 m_ChipDeck에 값을  넣는다 Ex : 130억이면 _100Y = 1,_10Y = 3
	BOOL IsMyChipShow() 
	{
		return m_bChipShow;
	}
	virtual void SetMoneyChip(INT64 Betmoney);				
	virtual void OnMyChipTimer(DWORD dwCurTime);			// 갱신되는 정보 처리 	
	virtual void SetMyMoveChip();

	virtual void OnMyMoveChipProcessEx(DWORD dwCurTime);					// 칩이동 처리 

	virtual void OnMyChipDraw();							// 칩 그리기
	virtual void OnMyChipDraw(MYCHIPDATA *data);			// 칩 그리기
	
	virtual void SetMyChipData(MYCHIPDATA &data, int index, DWORD dwStartTime, DWORD dwMoveTime);
	virtual void OnEnterRoom(void);
};

#endif // !defined(AFX_MYBETCHIP_H__6B5FA6B5_5539_4E64_9A89_44FBC10BEC1C__INCLUDED_)
