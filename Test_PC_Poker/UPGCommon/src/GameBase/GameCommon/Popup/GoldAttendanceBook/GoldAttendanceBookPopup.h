#pragma once

#include "GoldAttendanceBookSub/GoldAttendanceBookSubPopup.h"

class UPG_COMMON_EXPORT CGoldAttendanceBookPopup : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_TIMER_TYPE
	{
		TIMER_CARD_OUT,
		TIMER_SUCCESS_ANI1,
		TIMER_SUCCESS_ANI2,
		TIMER_SUCCESS_DLG,
		TIMER_FAIL_ANI1,
		TIMER_FAIL_ANI2,
		TIMER_FAIL_DLG,
		TIMER_GAME_RESULT,
		TIMER_WINNER_NOTI,
		TIMER_WINNER_NOTI_END,
	};
private:
	LSY::CSprite *m_pSprBack;
	LSY::CSprite *m_pSprOdd;
	LSY::CSprite *m_pSprEven;
	LSY::CSprite *m_pSprCard;
	LSY::CSprite *m_pSprSuccess;
	LSY::CSprite *m_pSprNotice;
	LSY::CText *m_pTxtWinNum;
	LSY::CText *m_pTxtGold;
	LSY::CText *m_pTxtWinGold;
	LSY::CImage *m_pImgCard;
	LSY::CButton *m_pBtnOdd;
	LSY::CButton *m_pBtnEven;
	LSY::CButton *m_pBtnClose;
	
	// �������� �޾ƿ��� ��
	int m_Round_Cur; //�ܰ� 1 ~ 8���� ����
	INT64 m_CurrentReward; //���ݱ��� Ȯ���� ����ݾ�(���ý���, Ȥ�� �������⸦ �ϸ� �ް� �Ǵ� �����)
	INT64 m_WinReward; //�̹� ���忡�� ���ü����� �ްԵǴ� ����ݾ��̴�.
	int m_BetTime; //�������ѽð��̴�(��), ���� -1�� ���� ������ ���ٴ� �ǹ�
	
	bool m_BetSuccess; //��������
	int m_Round; //������ ����
	int m_CardNum; //�̹��� ����� �ο��� ī���ȣ�̴�.(������Ŀ ī�� �ε����� ������)
	INT64 m_RoundReward; //������ ���忡 ���ü����� ȹ���ϰԵǴ� �����
	
	//char *m_UserID; //���̵� Ȥ�� �г��� -> ���� �ʿ� �����Ƿ� �ϴ� �н�
	int m_WinRound; //��÷ ����
	INT64 m_RewardMoney; //��÷��
	int m_RoundWinner; //�ش� ���� ��÷�� ��
	
	// 
	bool m_bContinue; // ���� ���� ���� ����
	bool m_bInProgress; // ���� ���� ������ ����
	
	// Ŭ����
	CGoldAttendanceBookSubPopup m_popGoldAttendanceBookSub;
	
public:
	CGoldAttendanceBookPopup(CWnd* pParent = NULL);
	~CGoldAttendanceBookPopup();
	
	DECLARE_MESSAGE_MAP()
public:
	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	//virtual BOOL OnInitDialog(); // LSY Object �ε� ��
	virtual bool OnCreatedProject(); // LSY Object �ε� ��
	virtual void OnCancel();
	// �޽�����
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// �̺�Ʈ
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);
	LRESULT OnMouseMove(LSY::CMessage *pMsg);
	LRESULT OnLButtonDown(LSY::CMessage *pMsg);
	LRESULT OnLButtonUp(LSY::CMessage *pMsg);
	
	// ����� ����
	void InitLSYControl(); // LSY Control ����
	void InitLSYControlToNULL(); // LSY Control NULL ������ �ʱ�ȭ
	void SetData(int nRound, bool bShowNoti); // ������ ����
	void CloseDlg(bool bContinue);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
	void UserBetResult(); // ���� ���� ���
	void Wait(DWORD dwMillisecond);
	int GetPokerSprIndex(int nCardNum);
	void GetNumberWithComma(CString parm_data, CString &parm_string);
	void GetNumberWithComma2(INT64 nNum, char cBuffer[]);
	void CardOut(); // ī�� ����
};
