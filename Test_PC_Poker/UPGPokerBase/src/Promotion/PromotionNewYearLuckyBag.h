#pragma once

#include "PromotionBase.h"
#include "PromotionManager.h"
#include "../ChattingWindowMan/ChattingWindowMan.h"
#include "../UIInfoData.h"

#define UM_LB_BTN01 (WM_USER + 201801)
#define UM_LB_BTN02 (WM_USER + 201802)
#define UM_LB_BTN03 (WM_USER + 201803)
#define UM_LB_BTN04 (WM_USER + 201804)
#define UM_LB_BTN05 (WM_USER + 201805)

class CChattingWindowMan;

class UPG_GAMEBASE_EXPORT CPromotionNewYearLuckyBag : public CPromotionBase
{
protected:
	// ��� �׸�
	NMBASE::GRAPHICGDI::xSprite m_sprBack;
	CEffectBase m_cEffect_back;
	NMBASE::GRAPHICGDI::xSprite m_sprChicken;
	CEffectBase m_cEffect_chicken;
	
	// �����ϱ� ��ư
	NMBASE::GRAPHICGDI::xSprite m_sprBtn;
	NMBASE::SKINGDI::CGraphButton m_btn01;
	NMBASE::SKINGDI::CGraphButton m_btn02;
	NMBASE::SKINGDI::CGraphButton m_btn03;
	NMBASE::SKINGDI::CGraphButton m_btn04;
	
	// �����ϱ� ��ư ���� - 0: none, 1: over, 2: click
	int m_nBtnState01;
	int m_nBtnState02;
	int m_nBtnState03;
	int m_nBtnState04;
	
	// ���� Ƚ�� �ؽ�Ʈ
	NMBASE::SKINGDI::CNText *m_pNum01;
	NMBASE::SKINGDI::CNText *m_pNum02;
	NMBASE::SKINGDI::CNText *m_pNum03;
	NMBASE::SKINGDI::CNText *m_pNum04;
	NMBASE::SKINGDI::CNText *m_pNum05;
	
	// ���� ���ָӴ� ���� �ؽ�Ʈ
	NMBASE::SKINGDI::CNText *m_pBagNum01;
	NMBASE::SKINGDI::CNText *m_pBagNum02;
	NMBASE::SKINGDI::CNText *m_pBagNum03;
	NMBASE::SKINGDI::CNText *m_pBagNum04;
	
	// ��� mouse on ����
	bool m_isOnBtn_bag;
	bool m_isOnBtn_goldBag;
	bool m_isOnBtn_btn;	// �����ϱ� ��ư
	bool m_isOnBtn_chicken;
	
	DWORD m_dwRewardStartTime;
	DWORD m_dwRewardStartTime_gold;
	
	//// �������� �޾ƿ��� ������
	// ���ָӴ� ���� ����
	int m_CurrentGameCount; //���� �������� �Ǽ�. ������ ���޵Ǹ� 0���� �ʱ�ȭ �ȴ�.
	//int m_TodayRewardCount; //���� ���޹��� ����Ƚ��(�ʱ�ȭ �ð��� �Ǹ� 0���� �ʱ�ȭ �ȴ�.)
	int m_NeedGameCount; //���� ������ �ޱ� ���� �ʿ��� ����Ƚ��
	//int m_TodayRewardLimtCount; //���� ���޹��� �� �ִ� ����Ƚ��
	int m_TicketCount; //����ǵ� ����ϱ� ������ �����Ҽ� �ִ� ��ȭ�� ����
	int m_TicketLimitCount; //����ǵ� ������ �� �ִ� �ִ�ġ
	// Ȳ�� ���ָӴ� ���� ����
	int m_CurrentGameCount_gold; //���� �������� �Ǽ�. ������ ���޵Ǹ� 0���� �ʱ�ȭ �ȴ�.
	//int m_TodayRewardCount_gold; //���� ���޹��� ����Ƚ��(�ʱ�ȭ �ð��� �Ǹ� 0���� �ʱ�ȭ �ȴ�.)
	int m_NeedGameCount_gold; //���� ������ �ޱ� ���� �ʿ��� ����Ƚ��
	//int m_TodayRewardLimtCount_gold; //���� ���޹��� �� �ִ� ����Ƚ��
	int m_TicketCount_gold; //����ǵ� ����ϱ� ������ �����Ҽ� �ִ� ��ȭ�� ����
	int m_TicketLimitCount_gold; //����ǵ� ������ �� �ִ� �ִ�ġ
	// ġŲŸ�� �̺�Ʈ ���� ����
	int m_CurrentGameCount_chicken; //���� �������� �Ǽ�. ������ ���޵Ǹ� 0���� �ʱ�ȭ �ȴ�.
	int m_TodayRewardCount_chicken;
	
	// ���ָӴ� ����
	int m_TodayRewardCount; //���� ���޹��� ����Ƚ��(�ʱ�ȭ �ð��� �Ǹ� 0���� �ʱ�ȭ �ȴ�.)
	int m_TodayRewardLimtCount; //���� ���޹��� �� �ִ� ����Ƚ��
	char *m_RewardType; //����Ÿ�� = GM.���ӸӴ�, GD���, CP.����, DC.���α�
	INT64 m_RewardAmount; //����ݾ�(���� ������ �Ѱ� ���޵Ǿ��ٸ� ���⿡ 1�� ��)
	char *m_RewardAddInfo; //���� ���� �߰� ����, ���� �������� ���޵Ǿ��ٸ� ������ȣ�� ��.
	SYSTEMTIME m_ExpiredTime; //���� ���� �� �ִ� ������. ���ӸӴϳ� ���� ��� ���� ��ǰ�̶�� �ǹ� ����
	// Ȳ�� ���ָӴ� ����
	int m_TodayRewardCount_gold; //���� ���޹��� ����Ƚ��(�ʱ�ȭ �ð��� �Ǹ� 0���� �ʱ�ȭ �ȴ�.)
	int m_TodayRewardLimtCount_gold; //���� ���޹��� �� �ִ� ����Ƚ��
	char *m_RewardType_gold; //����Ÿ�� = GM.���ӸӴ�, GD���, CP.����, DC.���α�
	INT64 m_RewardAmount_gold; //����ݾ�(���� ������ �Ѱ� ���޵Ǿ��ٸ� ���⿡ 1�� ��)
	char *m_RewardAddInfo_gold; //���� ���� �߰� ����, ���� �������� ���޵Ǿ��ٸ� ������ȣ�� ��.
	SYSTEMTIME m_ExpiredTime_gold; //���� ���� �� �ִ� ������. ���ӸӴϳ� ���� ��� ���� ��ǰ�̶�� �ǹ� ����
	// ġŲ ����
	DWORD m_dwChickenTimeStartTime;
	DWORD m_dwChickenRewardStartTime; //���� ��������
	
	//ġŲ Ÿ������ ����
	BOOL m_bTimeForChicken;
	BOOL m_bTimeForNewYear;
	bool m_bPlayReward;
	bool m_bPlayReward_gold;
	bool m_bGoldReward;
	
public:
	// ������, �Ҹ���
	CPromotionNewYearLuckyBag(CPromotionManager *pManager, int nType);
	virtual ~CPromotionNewYearLuckyBag(void);
	
	// ����
	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, CChattingWindowMan *pChatMan);
	
	virtual BOOL OnLButtonUpTop(int &x, int &y);
	virtual BOOL OnMouseMoveTop(int &x, int &y);
	
	virtual BOOL OnEventTabLButtonDown(int &x, int &y);
	virtual BOOL OnEventTabLButtonUp(int &x, int &y);
	virtual BOOL OnEventTabMouseMove(int &x, int &y);
	
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);
	
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);
	
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	
protected:
	//// ����� ����
	// �ΰ��� ���� �Ʒ��� ��� ǥ��
	void DrawBanner(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, int nLuckyBagNum, int nGoldLuckyBagNum);
	BOOL DrawBannerReward(NMBASE::GRAPHICGDI::CPage *pPage);
	BOOL DrawBannerReward_Gold(NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawChickenBanner(NMBASE::GRAPHICGDI::CPage *pPage);
	
	bool CheckRoom();
	void GetNumberWithComma(int nNum, char cBuffer[]);
};
