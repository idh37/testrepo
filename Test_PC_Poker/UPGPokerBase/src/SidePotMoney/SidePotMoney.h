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
	//������ ���̵� �� ���� ����
	class USER_SIDEPOT
	{
	protected:
		bool			bUse;				//���̵� ���� �߻��ߴ��� ����
		bool			bFold;				//���� ���θ� ��Ÿ����.
		map<int, INT64>	mapSidePotMoney;	//���̿� �ڽ��� ������ �ݾ�
		INT64			i64ResultMoney;		//�ٴڿ��� �������� �ݾ�(������ �� �ݾ��� ����)
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

	virtual bool IsOpponentAllFold(int nPNum);					//nPNum�� ������ ������� ��� Fold�ߴ����� ����
	virtual bool IsUserParticipation(int nPNum, int nPot);		//nPNum �ڸ��� �ִ� ����ڰ� nPot��° �̿� ���� �ߴ��� ���θ� �����Ѵ�.
	virtual LIST_SAME_PLAYER GetWinner(int nPot);						//nPot��° ���� ���� ��ȣ�� ���Ѵ�.

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual USER_SIDEPOT &GetUserSidePot(int nPnum){return m_mapUserSidePot[nPnum];}
	virtual MAP_RANKING &GetRanking(void){return m_mapRanking;}
};