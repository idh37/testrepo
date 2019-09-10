#pragma once

enum TYPE_HL_PROB_BOX			//���̷ο� Ȯ�� �ڽ� ����
{
	TYPE_HL_PROB_BOX_NULL,
	TYPE_HL_PROB_BOX_SWING,			//��ī�� �̻� ��������
	TYPE_HL_PROB_BOX_HIGH,			//��ī�� �̸� ��������
	TYPE_HL_PROB_BOX_LOW,			// �ο� ����
};

class CGameHelperManHighLow :
	public CGameHelperMan
{

	int m_nMyCurJokboIndex_low;
	BOOL m_bOnlySwing;					//������ǥ�����ش�.(��ī���̻�)
public:
	CGameHelperManHighLow(void);
	~CGameHelperManHighLow(void);

	TYPE_HL_PROB_BOX GetProbBoxType(int nIndexRule);


	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, int nMaxRule, COtherUserJokboCheckMan *pOtherUserJokboCheckMan, CJokboProbability *pJokboProbability);
	virtual void DrawProbability( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage );
	virtual void CalculationMyJokbo();
	virtual void Reset();



};
