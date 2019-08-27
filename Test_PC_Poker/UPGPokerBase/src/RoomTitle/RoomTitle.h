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
	NMBASE::SKINGDI::CNText		*m_pRoomInfoMinimumMoney;	//입장제한 머니 표시
	NMBASE::SKINGDI::CNText		*m_pRoomInfoRuleType;		//초이스|1000억/아침전F등 룸의 포커룰 정보 표시
	NMBASE::SKINGDI::CNText		*m_pRoomInfoPokerKind;		//맞포커에서 깜깜이/7포커 표시
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