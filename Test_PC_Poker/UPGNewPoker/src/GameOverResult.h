// GameOverResult.h: interface for the CGameOverResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOVERRESULT_H__1A7405AE_F0A1_4839_B372_8B3C6D58E807__INCLUDED_)
#define AFX_GAMEOVERRESULT_H__1A7405AE_F0A1_4839_B372_8B3C6D58E807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"

class CGameOverResult  
{
public:
	CGameOverResult();
	virtual ~CGameOverResult();

	NMBASE::GRAPHICGDI::CPage				*m_pPage;	
	CCharView			m_cWinnerAV;		//승리자 
	
	GAMEOVERRESULT		m_cGOR;				//결과 데이타
	USERINFO			m_cUI;
	
	BOOL				m_bItemBuyUse;		//수호천사
	INT64				m_i64LostMoney;		//잃은 머니

	BOOL				m_bShow;			//그려라
	BOOL				m_bShowAngel;		//보디가드 보여주기

	int					m_nXp,m_nYp;		//그려질 좌표
	int					m_nShowCnt;			//보여질 시간
	int					m_nCardNo[5];
	int					m_nWinCase;			//1 이면 정상적인 승리 0 이면 기권승
	int					m_nWinnerKind;		//0 승리냐,1패냐,2관전결과냐	
	CString				m_strResultCardName;

	DWORD				m_dwSetStartTime;
	DWORD				m_dwDrawTime;
	int					m_nPNum;
	int					m_nBackIndex;
	int					m_nDelay;

	void Init(NMBASE::GRAPHICGDI::CPage *page);										// 초기
	void Clear();
	void ReSet();
	void OnTimer();
	
	void AngelDraw(CDC *pdc, NMBASE::GRAPHICGDI::CPage *page);
	void PutAngelResult(GAMEOVERRESULT *pgor,USERINFO *ui,int pnum,int wincase = 0 , int delay = 0,BOOL bShow = TRUE);

	INT64 Get_angel_defmoney(const INT64 &lmoney);
	INT64 Get_angel_defmoneyEx(const INT64 &lmoney);
};

#endif // !defined(AFX_GAMEOVERRESULT_H__1A7405AE_F0A1_4839_B372_8B3C6D58E807__INCLUDED_)
