// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

class CUIInfoDataSutda : public CUIInfoData
{
public:
	CUIInfoDataSutda();
	virtual ~CUIInfoDataSutda();

	virtual void Init(void);


	std::vector<CPoint>		m_listParkmoonsuPos;				//�� �÷��̾��� ������ ����Ʈ ��ġ�� ������ �ִ� ����Ʈ
	std::vector<CPoint>		m_listTangKillerPos;				//�� �÷��̾��� ������ ����Ʈ ��ġ�� ������ �ִ� ����Ʈ
	std::vector<CPoint>		m_listResultStampPos;				//�� �÷��̾��� ����� ����(��, �籸��, ���� ��) ����Ʈ ��ġ�� ������ �ִ� ����Ʈ
	std::vector<CPoint>		m_listPenaltyMoneyPos;				//�� �÷��̾��� ����� �����Ӵ� ǥ�� ��ġ�� ������ �ִ� ����Ʈ
	std::vector<CPoint>		m_listChipStartPos;					//�� �÷��̾��� ���ý� Ĩ ������ ������ġ�� ������ �ִ� ����Ʈ
	std::vector<CRect>		m_listJokboNameRect;				//�� �÷��̾��� ����� �����̸� ǥ�� ��ġ�� ������ �ִ� ����Ʈ


	CPoint		m_ptRematchNotice;								//���� �˸�â �ִϸ��̼� ��ġ

	CPoint		m_ptBetBoard;									//ȭ�� �߰� Ĩ ���ÿ��� ��ġ

// public:
// 	CPoint	m_CardPosGap[3];		// ī�� ��ġ ���濡 ���� ��ġ�� ��ȭ (���� ī�� ��ġ�� �����Ű�� ������쿡 0�̿� ���� �־� �ش�.)
};