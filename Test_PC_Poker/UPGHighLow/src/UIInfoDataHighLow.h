// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>


enum ENUM_BUTTON_TYPE_HIGHLOW
{
	EBT_HILO_HIGH,
	EBT_HILO_LOW,
	EBT_HILO_SWING,
};
class CUIInfoDataHighLow : public CUIInfoData
{
public:

	std::map<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>		m_mapHIghLowBtnPos;
	CRect											m_rtLowJokoName;			//�ο� ������ ǥ�� ����
	COLORREF										m_clrLowJokboNameColor;		//�ο����� �̸��� ǥ���Ҷ��� ���ڻ�
	COLORREF										m_clrSwingJokboNameColor;		//�������� �̸��� ǥ���Ҷ��� ���ڻ�


	CPoint											m_ptLowJokboHelpPercentGab;		//�ο����� �ۼ�Ʈ ǥ�� ������
	CPoint											m_ptSwingJokboHelpPercentGab;	//��������(��ī���̻�) �ۼ�Ʈ ǥ�� ������
public:
	CUIInfoDataHighLow();
	virtual ~CUIInfoDataHighLow();

	virtual void Init(void);

	CPoint GetHighLowBtnPos(ENUM_BUTTON_TYPE_HIGHLOW eHighlowPos);

// public:
// 	CPoint	m_CardPosGap[3];		// ī�� ��ġ ���濡 ���� ��ġ�� ��ȭ (���� ī�� ��ġ�� �����Ű�� ������쿡 0�̿� ���� �־� �ش�.)
};