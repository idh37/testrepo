// VSModePlayRoundResultMan.h: interface for the CVSModePlayRoundResultMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSMODEPLAYROUNDRESULTMAN_H__2663E0FE_429E_42F2_BB3C_008B6ACFA440__INCLUDED_)
#define AFX_VSMODEPLAYROUNDRESULTMAN_H__2663E0FE_429E_42F2_BB3C_008B6ACFA440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IDM_VSHIDEBTN			(WM_USER+9007) 

class CVSModePlayRoundResultMan  
{
public:
	CVSModePlayRoundResultMan();
	virtual ~CVSModePlayRoundResultMan();


	DWORD nStartTime;
	DWORD nEndTime;

	xSprite *pSpr;
	NMBASE::SKINGDI::CGraphButton m_HideBtn;

	DWORD nDelay;
	int nWinNum;
	int nLooseNum;
	int nRoundNum;

	int nDrawMode;//0 나오는 효과 1 들어가는 효과
	BOOL bShowRound;
	void Init();
	void Clear();
	void SetRoundInfo( int win ,int loose ,int round);
	void VSModePlayRoundResult_Draw(CDC *pDC);

	// 마우스 관련
	void VSRoundResltWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void ReSetHideButtonState();
};

#endif // !defined(AFX_VSMODEPLAYROUNDRESULTMAN_H__2663E0FE_429E_42F2_BB3C_008B6ACFA440__INCLUDED_)
