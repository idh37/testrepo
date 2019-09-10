// BombCard.h: interface for the CBombCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOMBCARD_H__5B093562_1BCE_4055_9D69_92A9ACD23316__INCLUDED_)
#define AFX_BOMBCARD_H__5B093562_1BCE_4055_9D69_92A9ACD23316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MovementEx.h"

class CBombCard
{
protected:
	enum BOMB_CARD_STATUS
	{
		BCS_NOSHOW = -1,		//���� ȭ�鿡 ������ ������
		BCS_DEALING,			//ó�� ��ź ī�尡 �����Ҷ�
		BCS_BOMBSET_EFFECT,		//������ ������ ��ź ����Ʈ�� ���ö�
		BCS_NOTDEAL_BOMBSET,	//ī�尡 �й�Ǳ��� ��źī�尡 ���É�����
		BCS_DEAL_SHOW,			//��ź ī�� �й�� ȭ���� �߾����� �̵���
		BCS_DEAL_JOKER_EFFECT,	//��ź ī�� �й�� ȭ���� �߾ӿ��� ī������� ��Ŀī���̸� ��Ÿ���� ����Ʈ
		BCS_DEAL_BOMBSET_EFFECT,//��ź ī�� �й�� ȭ���� �߾ӿ��� ī������� ��Ŀī�尡 �ƴҶ� ��Ÿ���� ��ź ����Ʈ
		BCS_DEAL_BOMBRESET,		//�߾ӿ��� ��źī�尡 �ٽ� ���õǷ� ���ư���
		BCS_BOMBRESET,			//��ź ī�尡 ���µǰ� ���϶�
	};

	BOMB_CARD_STATUS	m_nStatus;
	CPoint				m_ptPos;	// ī�� ��ġ
	int					m_nCardNo;	// ī�� �ѹ�

	CMovementEx			m_cMove;	// �̵�

	int					m_nTotalFrame;
	DWORD				m_dwStartTime;
	DWORD				m_dwTimeGab;
public:
	CBombCard();
	virtual ~CBombCard();	
	
	void Clear();
	void ForceSet(int nCardNo);

	void OnSetBombCard(DWORD dwTimeGab);
	void OnShowBombCard(DWORD dwTimeGab);
	void OnShowJokerBombCard(void);

	void Draw(NMBASE::GRAPHICGDI::CPage *pPage);
	void SetBombCard();
	void SetOPenBombCard(char cardno);
	void RePtBombCard(void);
	void OnTimer();
};

#endif // !defined(AFX_BOMBCARD_H__5B093562_1BCE_4055_9D69_92A9ACD23316__INCLUDED_)
