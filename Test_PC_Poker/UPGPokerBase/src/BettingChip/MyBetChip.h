// MyBetChip.h: interface for the CMyBetChip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBETCHIP_H__6B5FA6B5_5539_4E64_9A89_44FBC10BEC1C__INCLUDED_)
#define AFX_MYBETCHIP_H__6B5FA6B5_5539_4E64_9A89_44FBC10BEC1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../shared/CardDefine.h"

class NMBASE::GRAPHICGDI::CPage;
class CUIInfoData;
class NMBASE::GRAPHICGDI::xSprite;

class UPG_GAMEBASE_EXPORT CMyBetChip  
{
private:
	NMBASE::GRAPHICGDI::CPage		*m_pPage;
	NMBASE::GRAPHICGDI::xSprite		*m_pChipSpr;				// Ĩ ��������Ʈ
	MYCHIPDATA	m_listMyChipData[EndChip];	// Ĩ ����Ÿ
	CUIInfoData	*m_pUIInfoData;

	CPoint		m_ptChipStart;				// �׷��� Ĩ ��ġ ��ǥ

	bool		m_bChipShow;				// �׷��� ����
	CPoint		m_ptPos;					// �׷��� ��ġ
	CPoint		m_ptStart;					// �̵� ���� ��ġ
	bool		m_bMoveStart;				// �̵� ������ �˸� 
	bool		m_bMoveEnd;					// �̵� ���� �˸� TRUE �϶� ��Ż ������ ���Ѵ�.
	int			m_nPNum;
public:
	CMyBetChip();
	virtual ~CMyBetChip();

	// Ĩ �ʱ�ȭ�ÿ� NMBASE::GRAPHICGDI::CPage, Ĩ ��������Ʈ ������, �÷��̾� ��ġ�� �Է��� �ش�.
	virtual void Init(NMBASE::GRAPHICGDI::CPage* pPage, NMBASE::GRAPHICGDI::xSprite* pChipSpr, int nPNum, CUIInfoData *pUInfoData);
	virtual void ReSet();									// �ʱ� 

	// Ĩ�� ǥ���� ���� ������ ���� 3�ڸ��� ������ ���� ä�� Ex: 130��1345��..  �̸� 130�︸ �� ����
	// ���� �ݾ��� �޾� m_ChipDeck�� ����  �ִ´� Ex : 130���̸� _100Y = 1,_10Y = 3
	BOOL IsMyChipShow() 
	{
		return m_bChipShow;
	}
	virtual void SetMoneyChip(INT64 Betmoney);				
	virtual void OnMyChipTimer(DWORD dwCurTime);			// ���ŵǴ� ���� ó�� 	
	virtual void SetMyMoveChip();

	virtual void OnMyMoveChipProcessEx(DWORD dwCurTime);					// Ĩ�̵� ó�� 

	virtual void OnMyChipDraw();							// Ĩ �׸���
	virtual void OnMyChipDraw(MYCHIPDATA *data);			// Ĩ �׸���
	
	virtual void SetMyChipData(MYCHIPDATA &data, int index, DWORD dwStartTime, DWORD dwMoveTime);
	virtual void OnEnterRoom(void);
};

#endif // !defined(AFX_MYBETCHIP_H__6B5FA6B5_5539_4E64_9A89_44FBC10BEC1C__INCLUDED_)
