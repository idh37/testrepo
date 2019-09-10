// DisplayBoard.h: interface for the CDisplayBoard class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

enum ENUM_DISPLAY_BOARD_TEXT
{
	EDBT_UNKNOWN = 0,
	EDBT_MINUSER,				//1�� ������
	EDBT_OPENCARD_CHOICE,		//7��Ŀ �Ϲݸ��, ����ī�弱��
	EDBT_SELECTCARD,			//ī�带 ������
	EDBT_THROWAWAY_CARD,		//ī�带 ������
	EDBT_AUTOBETTING,			//�ڵ����� ���� ���϶�
	EDBT_2BETTING,				//2������
	EDBT_3BETTING,				//3������
	EDBT_4BETTING,				//4������
	EDBT_5BETTING,				//5������
	EDBT_6BETTING,				//6������
	EDBT_7BETTING,				//7������
	EDBT_HIDDENBETTING,			//������ ���� ����
	EDBT_BETTING_COUNT,			//���� Ƚ��
	EDBT_GAMESTART,				//ù��° ī�� �����Ҷ�
	EDBT_2CARD,					//2��° ī�� ����
	EDBT_3CARD,					//3��° ī�� ����
	EDBT_4CARD,					//4��° ī�� ����
	EDBT_5CARD,					//5��° ī�� ����
	EDBT_6CARD,					//6��° ī�� ����
	EDBT_BOMBCARD,				//��ź ī�� �й�
	EDBT_JOKERCARD,				//��Ŀ ī�� �й�
	EDBT_HIDDENCARD,			//������ ����ī�� �й�
	EDBT_RESULT,				//��� ��½�
	EDBT_RESULT2,				//�������
	EDBT_LIMITTIME,				//�ð� ���� �ʰ� 2ȸ �̻�
	EDBT_BEFORE_MORNING_BETTING,	//��ħ �� ����
	EDBT_MORNING_BETTING,		//��ħ ����
	EDBT_AFTERNOON_BETTING,		//���� ����
	EDBT_NIGHT_BETTING,			//���� ����
	EDBT_MORNING_CUTTING,		//��ħ ����
	EDBT_AFTERNOON_CUTTING,		//���� ����
	EDBT_NIGHT_CUTTING,			//���� ����
	EDBT_SELECT_HLS,			//����/�ο�/���� ����
	EDBT_RESULT_HILOW_1WIN,			//���̷ο� ��� (����1��)
	EDBT_RESULT_HILOW_2WIN,			//���̷ο� ��� (����2��)
	EDBT_FIRST_BETTING,			//ù ��° ���� (����)
	EDBT_FINAL_BETTING,			//������ ���� (����)
	EDBT_FINAL_DEAL,			//"������ ���Դϴ�."
	EDBT_NOTIFY_REMATCH_BY_DRAW,	// ������ ����
	EDBT_NOTIFY_REMATCH_BY_49,	// 49 ����
	EDBT_RESULT_SUTDA,			//���� ��� (OO�� OOO ȹ���ϼ̽��ϴ�.)
	EDBT_SELECTCARD_SUTDA,		//���� �� ����â
	EDBT_NORMAL0_HOOLA,					//�Ƕ�: ���� ������ �޽��� ��������1
	EDBT_NORMAL1_HOOLA,					//�Ƕ�: ���� ������ �޽��� ��������2
	EDBT_NORMAL2_HOOLA,					//�Ƕ�: ���� ������ �޽��� ��������3
	EDBT_MISSION_COMPLETE_HOOLA,		//�Ƕ�: �̼� ����, �ش� ��Ȳ�� �Ǹ� ��� �� ������ ��ü��.
	EDBT_STOPJACKPOT_BELOW_1JO_HOOLA,	//�Ƕ�: ���� ���� ������(1���� ����) �߻� ���ӹ濡�� ����
	EDBT_STOPJACKPOT_UPPER_3JO_HOOLA,	//�Ƕ�: ���� ���� ������(3���� �̻�) �߻� ���ӹ濡�� ����
	EDBT_MAX,
};

enum ENUM_RASE_STYLE
{
	ERS_START = 1,
	ERS_1122 = ERS_START,
	ERS_1222,
	ERS_1123,
	ERS_1223,
	ERS_11223,
	ERS_MAX,
};

class CUIInfoData;

#define MAX_ROUND 5

class UPG_GAMEBASE_EXPORT CDisplayBoard
{
protected:
	struct DrawTextInfo
	{
	COLORREF clrColor;
	CString strOutput;
	CString strFormat;
	};
protected:
	map<int, map<int, DrawTextInfo> >	m_mapText;

	CPoint						m_ptPos;
	CRect						m_rtSize;
	ENUM_DISPLAY_BOARD_TEXT		m_nTextIndex;

	CGameProcess				*m_pGameProcess;
public:
	CDisplayBoard();
	virtual ~CDisplayBoard();

	int GetTextWidth(CDC *pDC);

	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame);
	virtual void SetText(ENUM_DISPLAY_BOARD_TEXT nTextIndex, LPARAM lParam = 0);
	virtual void Draw(CDC *pDC);
	virtual int GetRaseCount(int nRase);
};