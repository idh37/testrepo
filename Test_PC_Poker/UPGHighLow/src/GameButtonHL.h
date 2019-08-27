#pragma once


#define IDM_HIGH			(WM_USER+7001)
#define IDM_LOW				(WM_USER+7002)	
#define IDM_SWING			(WM_USER+7003)


class CGameButtonHL : public CGameViewButtonMan
{
public:

	NMBASE::SKINGDI::CGraphButton	m_btnHigh, m_btnLow, m_btnSwing;

	NMBASE::SKINGDI::CGraphButtonList	m_ctrlHilowBtnList;	//하이, 로우, 스윙 버튼


	CGameButtonHL();
	virtual ~CGameButtonHL();

public:

	void Init( CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChatWndMan );
	

	virtual void ClearFlag();
	virtual void InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType );	
	virtual void DrawBtn(CDC *pDC);
	virtual BOOL OnLButtonDown(int x , int y);
	virtual BOOL OnLButtonUp(int x , int y);
	virtual BOOL OnMouseMove(int x , int y);

	void SetHilowSelBtnShowAll( bool bShow ); // 모든 하이/로우/스윙 선택 버튼 보이기
	void ShowWincaseBtn_HL(bool bShow,bool bEnableOnlySwingBtn);
	void EnableWincaseBtn_HL(bool bEnable);
	

private:

};