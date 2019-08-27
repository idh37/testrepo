// DisplayBoard.h: interface for the CDisplayBoard class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

using namespace std;

#define UM_SIDEPOT_ON			(WM_USER+8100)
#define UM_SIDEPOT_OFF			(WM_USER+8101)

class CUIInfoData;
class CGameViewBase;

typedef map<int, INT64>				LIST_SAME_PLAYER;
typedef map<int, LIST_SAME_PLAYER>	MAP_RANKING;

class UPG_GAMEBASE_EXPORT CSidePotMoney
{
protected:
	//유저별 사이드 팟 적용 상태
	class USER_SIDEPOT
	{
	protected:
		bool			bUse;				//사이드 팟이 발생했는지 여부
		bool			bFold;				//다이 여부를 나타낸다.
		map<int, INT64>	mapSidePotMoney;	//각팟에 자신이 투입한 금액
		INT64			i64ResultMoney;		//바닥에서 가져가는 금액(가진이 건 금액을 포함)
	public:
		USER_SIDEPOT():bUse(false),bFold(false){}
		virtual ~USER_SIDEPOT(){}

		void Clear(void){bUse = false; bFold = false; mapSidePotMoney.clear(); i64ResultMoney = 0;}
		map<int, INT64>& GetUserSidePotMoney()
		{
			return mapSidePotMoney;
		}
		bool IsUse()
		{
			return bUse;
		}
		bool IsFold()
		{
			return bFold;
		}

		friend class CSidePotMoney;
		friend class CAllBetChip;
	};

	vector<INT64>	m_listSidePot;
	int				m_nMaxPot;
	bool			m_bExpand;

	NMBASE::GRAPHICGDI::xSprite			m_sprSidePot;
	NMBASE::GRAPHICGDI::xSprite			m_sprDisplayBoard;
	NMBASE::GRAPHICGDI::xSprite			m_sprResult;
	
	NMBASE::SKINGDI::CGraphButton    m_btnSidePotOn;
	NMBASE::SKINGDI::CGraphButton    m_btnSidePotOff;

	int				m_nMaxPlayer;
	CUIInfoData		*m_pUIInfoData;
	CGameProcess	*m_pGameProcess;
	CGameViewBase	*m_pGameViewBase;

	vector<NMBASE::SKINGDI::CNText *> m_listResult_VS;
	vector<NMBASE::SKINGDI::CNText *> m_listResult_Money;
	NMBASE::SKINGDI::CNText			 *m_pResultTotalMoney;

	map<int, USER_SIDEPOT>	m_mapUserSidePot;
	int						m_nUsePlayer;
	bool					m_bGameOver;
	MAP_RANKING				m_mapRanking;
	int						m_nShowResultNumber;
public:
	CSidePotMoney();
	virtual ~CSidePotMoney();

	virtual void LoadImage(LPCTSTR lpBettingDisplayboardPath);
	virtual void Init(int nMaxPlayer, CUIInfoData* pInfoData, CGameViewBase *pGameView);
	virtual void SetGameOver(MAP_RANKING &mapRanking);

	virtual void Reset(void);
	virtual void SetExpand(bool bExpand);
	virtual void Check(void);
	virtual void DrawBottom(CDC *pDC);
	virtual void DrawTop(CDC *pDC);

	virtual bool IsOpponentAllFold(int nPNum);					//nPNum을 제외한 상대편이 모두 Fold했는지를 조사
	virtual bool IsUserParticipation(int nPNum, int nPot);		//nPNum 자리에 있는 사용자가 nPot번째 팟에 참여 했는지 여부를 조사한다.
	virtual LIST_SAME_PLAYER GetWinner(int nPot);						//nPot번째 팟의 승자 번호를 구한다.

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual USER_SIDEPOT &GetUserSidePot(int nPnum){return m_mapUserSidePot[nPnum];}
	virtual MAP_RANKING &GetRanking(void){return m_mapRanking;}
};