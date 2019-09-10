#pragma once

class CUIInfoData;

class UPG_GAMEBASE_EXPORT CRoomTitle
{
protected:
	CUIInfoData	*m_pUIInfoData;
	CString		m_strRoomTitle;

	NMBASE::SKINGDI::CNText		*m_pRoomTitle;
	IDX_GAME	m_nGameType;

	NMBASE::GRAPHICGDI::xSprite		m_sprRoomInfo;
	NMBASE::SKINGDI::CNText		*m_pRoomInfoMinimumMoney;	//�������� �Ӵ� ǥ��
	NMBASE::SKINGDI::CNText		*m_pRoomInfoRuleType;		//���̽�|1000��/��ħ��F�� ���� ��Ŀ�� ���� ǥ��
	NMBASE::SKINGDI::CNText		*m_pRoomInfoPokerKind;		//����Ŀ���� ������/7��Ŀ ǥ��
public:
	CRoomTitle();
	virtual ~CRoomTitle();

	virtual void LoadImage(IDX_GAME nGame);

	virtual void Init(CUIInfoData *pUIInfoData, CGameViewBase *pGameView);
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void MakeTitle(void);
public:
	char* GetRoomInfoMininumMoney();
	char* GetRoomInfoRuleType();
};