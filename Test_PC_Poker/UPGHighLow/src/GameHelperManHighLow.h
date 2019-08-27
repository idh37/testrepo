#pragma once

enum TYPE_HL_PROB_BOX			//하이로우 확률 박스 종류
{
	TYPE_HL_PROB_BOX_NULL,
	TYPE_HL_PROB_BOX_SWING,			//포카드 이상 스윙족보
	TYPE_HL_PROB_BOX_HIGH,			//포카드 미만 하이족보
	TYPE_HL_PROB_BOX_LOW,			// 로우 족보
};

class CGameHelperManHighLow :
	public CGameHelperMan
{

	int m_nMyCurJokboIndex_low;
	BOOL m_bOnlySwing;					//스윙만표시해준다.(포카드이상)
public:
	CGameHelperManHighLow(void);
	~CGameHelperManHighLow(void);

	TYPE_HL_PROB_BOX GetProbBoxType(int nIndexRule);


	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, int nMaxRule, COtherUserJokboCheckMan *pOtherUserJokboCheckMan, CJokboProbability *pJokboProbability);
	virtual void DrawProbability( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage );
	virtual void CalculationMyJokbo();
	virtual void Reset();



};
