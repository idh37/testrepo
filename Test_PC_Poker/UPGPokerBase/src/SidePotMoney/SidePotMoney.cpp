// DisplayBoard.cpp: implementation of the CDisplayBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <set>
#include "SidePotMoney.h"
#include "../UIInfoData.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSidePotMoney::CSidePotMoney()
{
	m_nMaxPlayer = 0;
	m_bExpand = false;

	m_pUIInfoData = NULL;
	m_pGameViewBase = NULL;
	m_nUsePlayer = 0;
	m_bGameOver = false;
	m_pResultTotalMoney = NULL;
	m_nShowResultNumber = 0;
}

CSidePotMoney::~CSidePotMoney()
{
	if(m_listResult_VS.size())
	{
		vector<NMBASE::SKINGDI::CNText *>::iterator start = m_listResult_VS.begin();
		vector<NMBASE::SKINGDI::CNText *>::iterator end = m_listResult_VS.end();

		for(;start != end;start++)
		{
			delete *start;
		}
	}

	m_listResult_VS.clear();

	if(m_listResult_Money.size())
	{
		vector<NMBASE::SKINGDI::CNText *>::iterator start = m_listResult_Money.begin();
		vector<NMBASE::SKINGDI::CNText *>::iterator end = m_listResult_Money.end();

		for(;start != end;start++)
		{
			delete *start;
		}
	}

	m_listResult_Money.clear();

	if(m_pResultTotalMoney) delete m_pResultTotalMoney;
	m_pResultTotalMoney = NULL;
}

void CSidePotMoney::LoadImage(LPCTSTR lpBettingDisplayboardPath)
{
	AddLoadImageList(&m_sprSidePot, ".\\PokerCommon\\data\\side_betting.spr", true);
	AddLoadImageList(&m_sprDisplayBoard, lpBettingDisplayboardPath, true);
	AddLoadImageList(&m_sprResult, ".\\PokerCommon\\data\\sp_sidebet.spr", true);
}

void CSidePotMoney::Init(int nMaxPlayer, CUIInfoData* pInfoData, CGameViewBase *pGameView)
{
	m_pUIInfoData = pInfoData;
	m_nMaxPlayer  = nMaxPlayer;

	m_pGameViewBase = pGameView;
	m_pGameProcess = pGameView->GetGame()->GetGameProcess();

	m_btnSidePotOn.Init(m_pGameViewBase,  &m_pGameViewBase->Page, m_pUIInfoData->m_listButtonPos[EBT_SIDEPOT].x, m_pUIInfoData->m_listButtonPos[EBT_SIDEPOT].y, &m_sprSidePot, 0, UM_SIDEPOT_ON);
	m_btnSidePotOff.Init(m_pGameViewBase, &m_pGameViewBase->Page, m_pUIInfoData->m_listButtonPos[EBT_SIDEPOT].x, m_pUIInfoData->m_listButtonPos[EBT_SIDEPOT].y, &m_sprSidePot, 1, UM_SIDEPOT_OFF);

	m_listSidePot.resize(m_nMaxPlayer);

	CRect rtRect = m_pUIInfoData->m_rtSidePotResultVS;
	rtRect.OffsetRect(m_pUIInfoData->m_ptSidePotResult + m_pUIInfoData->m_ptSidePotResultVSGab);

	NMBASE::SKINGDI::CNText *pText = NULL;
	for(int i=0;i < m_nMaxPlayer ;i++)
	{
		pText = new NMBASE::SKINGDI::CNText(rtRect, "", i, &m_pGameViewBase->Page, m_pGameViewBase->m_pDC);
		pText->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		pText->SetColor(RGB(198,198,198));
		pText->SetOutLine(RGB(16,16,16));
		pText->SetAlign(DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rtRect += CPoint(0, m_pUIInfoData->m_nSidePotResultGab);
		m_listResult_VS.push_back(pText);
	}

	rtRect = m_pUIInfoData->m_rtSidePotResultMoney;
	rtRect.OffsetRect(m_pUIInfoData->m_ptSidePotResult + m_pUIInfoData->m_ptSidePotResultMoneyGab);

	for(int i=0;i < m_nMaxPlayer ;i++)
	{
		pText = new NMBASE::SKINGDI::CNText(rtRect, "", 5+i, &m_pGameViewBase->Page, m_pGameViewBase->m_pDC);
		pText->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		pText->SetColor(RGB(140,132,90));
		pText->SetOutLine(RGB(16,16,16));
		pText->SetAlign(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

		rtRect += CPoint(0, m_pUIInfoData->m_nSidePotResultGab);
		m_listResult_Money.push_back(pText);
	}

	rtRect = m_pUIInfoData->m_rtSidePotResultTotalMoney;
	rtRect.OffsetRect(m_pUIInfoData->m_ptSidePotResult + m_pUIInfoData->m_ptSidePotResultTotalMoneyGab);

	m_pResultTotalMoney = new NMBASE::SKINGDI::CNText(rtRect, "", 11, &m_pGameViewBase->Page, m_pGameViewBase->m_pDC);
	m_pResultTotalMoney->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	m_pResultTotalMoney->SetColor(RGB(181,173,107));
	m_pResultTotalMoney->SetOutLine(RGB(16,16,16));
	m_pResultTotalMoney->SetAlign(DT_SINGLELINE | DT_TOP | DT_RIGHT);
}

void CSidePotMoney::Reset(void)
{
	m_nMaxPot	= 0;	
	m_nUsePlayer = 0;
	m_bGameOver = false;
	m_nShowResultNumber = 0;

	for(int i = 0; i < m_nMaxPlayer; ++i)
	{
		m_listSidePot[i] = (INT64)0;
		m_mapUserSidePot[i].Clear();
	}
}

void CSidePotMoney::SetGameOver(MAP_RANKING &mapRanking)
{
	m_bGameOver = true;
	m_mapRanking = mapRanking;
	m_nShowResultNumber = 0;

	CPlayer *pPlayer = NULL;
	for(int nPNum=0;nPNum<m_nMaxPlayer;nPNum++)
	{
		USER_SIDEPOT &mySidePotInfo = m_mapUserSidePot[nPNum];
		pPlayer = GM().GetCurrentProcess()->GetPlayer(nPNum);
		if(mySidePotInfo.mapSidePotMoney.size())
		{
			map<int, INT64>::iterator start, end;
			INT64 i64Result = 0;
			INT64 nGetMoney = 0;
			CString strOutput;

			//내가 기권했을 경우, 내가 건 머니는 모두 잃었다.
			if(mySidePotInfo.bFold)
			{
				start = mySidePotInfo.mapSidePotMoney.begin();
				end = mySidePotInfo.mapSidePotMoney.end();

				for(;start != end;start++)
				{
					if(nPNum == 0)
					{
						m_listResult_VS[start->first]->SetText("기권패");
						strOutput.Format("-%s",NMBASE::UTIL::g_MoneyMark(start->second, 2));
						m_listResult_Money[start->first]->SetText(strOutput);
						m_nShowResultNumber++;
					}

					i64Result -= start->second;
				}

				mySidePotInfo.i64ResultMoney = 0;
			}
			//모든 유저가 Fold 했을때 모든 머니는 내가 갖는다.
			else if(IsOpponentAllFold(nPNum))
			{
				start = mySidePotInfo.mapSidePotMoney.begin();
				end = mySidePotInfo.mapSidePotMoney.end();

				vector<INT64>::iterator start_total = m_listSidePot.begin();
				vector<INT64>::iterator end_total = m_listSidePot.end();

				for(int i=0;(start_total != end_total) && (*start_total != 0);start_total++,i++ )
				{
					mySidePotInfo.i64ResultMoney += *start_total;

					//내가 걸지 않는 팟의 모든 머니는 내가 딴 금액이다.
					if(start == end)
					{
						nGetMoney = *start_total;
					}
					//내가 건 팟에서는 내가 건 머니를 뺀 금액이 내가 딴 금액이다.
					else
					{
						nGetMoney = (*start_total - start->second);
						start++;
					}

					if((nPNum == 0) && i < m_nUsePlayer)
					{
						m_listResult_VS[i]->SetText("기권승");
						strOutput.Format("+%s",NMBASE::UTIL::g_MoneyMark(nGetMoney, 2));
						m_listResult_Money[i]->SetText(strOutput);
						m_nShowResultNumber++;
					}
					i64Result += nGetMoney;
				}
			}
			//끝까지 베팅했고 나 이외에 남은 유저가 있을 경우
			else
			{
				start = mySidePotInfo.mapSidePotMoney.begin();
				end = mySidePotInfo.mapSidePotMoney.end();
				LIST_SAME_PLAYER listWinner;
				//내가 참여한 팟에서 각각의 승패를 판정하고, 획득 또는 잃은 머니를 계산한다.
				for(;start != end;start++)
				{
					listWinner = GetWinner(start->first);
					ASSERT((int)listWinner.size() != 0);

					//승자에 내가 포함되어 있으면
					if(listWinner.find(nPNum) != listWinner.end())
					{
						if(start->first <= m_nUsePlayer)
						{
							mySidePotInfo.i64ResultMoney += (INT64)(m_listSidePot[start->first] / (INT64)listWinner.size());
							nGetMoney = (INT64)(m_listSidePot[start->first] / (INT64)listWinner.size()) - start->second;
							if(nPNum == 0)
							{
								m_listResult_VS[start->first]->SetText("승");
								strOutput.Format("+%s",NMBASE::UTIL::g_MoneyMark(nGetMoney, 2));
							}
						}
						else continue;
					}
					//승자가 다른 사람이면
					else
					{
						nGetMoney = -1 * start->second;
						if(nPNum == 0)
						{
							m_listResult_VS[start->first]->SetText("패");
							strOutput.Format("%s",NMBASE::UTIL::g_MoneyMark(nGetMoney, 2));
						}
					}

					if(nPNum == 0)
					{
						m_listResult_Money[start->first]->SetText(strOutput);
						m_nShowResultNumber++;
					}

					i64Result += nGetMoney;
				}
			}

			if(nPNum == 0)
			{
				strOutput = (CString)NMBASE::UTIL::g_MoneyMark(i64Result, 2);
				if(i64Result > 0)
				{
					strOutput.Insert(0,'+');
				}

				m_pResultTotalMoney->SetText(strOutput);
			}
		}
		else
		{
			mySidePotInfo.i64ResultMoney = 0;
		}
	}
}

void CSidePotMoney::SetExpand(bool bExpand)
{
	m_bExpand = bExpand;
	if(bExpand)
	{
		m_btnSidePotOff.Enable(TRUE);
		m_btnSidePotOn.Enable(FALSE);
	}
	else
	{
		m_btnSidePotOff.Enable(FALSE);
		m_btnSidePotOn.Enable(TRUE);
	}
}

void CSidePotMoney::Check(void)
{	
	int i = 0;
	for(i = 0; i < m_nMaxPlayer; ++i)
	{
		m_listSidePot[i] = (INT64)0;
	}

	//사이트 팟의 시작 금액을 구한다.
	//시작 금액은 오름차순으로 정리된다.
	std::set<INT64> listPot;
	listPot.insert((INT64)0);
	for(i = 0; i < m_nMaxPlayer; ++i)
	{
		//사이드 팟의 갯수를 구한다.
		//참여한 사용자 중 총베팅금액과 실제베팅 금액이 다르면 사이드 머니가 발생하였음을 의미한다.
		if(g_Poker.RU[i].bInUse)
		{
			//기권한 사용자는 모두 1Pot에 머니가 투입된다.
			if(g_Poker.RU[i].nRealBat != g_Poker.RU[i].nTotBat)
				listPot.insert(g_Poker.RU[i].nRealBat);
		}
	}

	std::set<INT64>::reverse_iterator rstart, rpre, rend;
	m_nMaxPot = (int)listPot.size() - 1;
	bool bFirst = true;

	//해당 유저가 각 Pot에 넣을 금액을 연산한다.
	int pn = 0;
	CGameProcess *pProcess = m_pGameViewBase->GetGame()->GetGameProcess();
	m_nUsePlayer = 0;
	INT64 nPotMoney = 0;

	for(i = 0; i < m_nMaxPlayer; ++i)
	{
		pn = pProcess->GetPNum_ByServPN(i);
		m_mapUserSidePot[pn].Clear();
		m_mapUserSidePot[pn].bUse	= g_Poker.RU[i].bInUse?true:false;
		m_mapUserSidePot[pn].bFold  = g_Poker.RU[i].bFold?true:false;

		if(g_Poker.RU[i].bInUse)
		{
			m_nUsePlayer++;
			//만약 실제 베팅금액이랑 총 배팅금액이 같으면 아직
			//자신는 사이드 배팅중이 아니다
			if(g_Poker.RU[i].nRealBat == g_Poker.RU[i].nTotBat)
			{
				m_mapUserSidePot[pn].bUse = false;
			}

			rstart = listPot.rbegin();
			rend   = listPot.rend();
			bFirst = true;
			for(int nPot = m_nMaxPot; rstart != rend; rstart++, nPot--)
			{
				//해당 유저가 가장 마지막에 넣은 Pot을 찾는다.
				if(bFirst && (*rstart < g_Poker.RU[i].nRealBat))
				{
					//해당 유저가 가장 마지막 Pot에 투입한 금액을 구한다.(실제 베팅 금액 - 마지막 Pot 시작 머니)
					nPotMoney = g_Poker.RU[i].nRealBat - *rstart;
					m_listSidePot[nPot] += nPotMoney;
					rpre = rstart;
					//이전 Pot에 투입한 금액은 현재 Pot의 금액 - 이전 Pot의 금액을 저해준기만 하면 된다.
					bFirst = false;
					m_mapUserSidePot[pn].mapSidePotMoney[nPot] = nPotMoney;
				}
				else if(!bFirst)
				{
					//다음 Pot 시작 금액과 현재 Pot 시작 금액의 차액 만큼을 현재 Pot에 추가해준다.
					nPotMoney = *rpre - *rstart;
					m_listSidePot[nPot] += nPotMoney;
					m_mapUserSidePot[pn].mapSidePotMoney[nPot] = nPotMoney;
					rpre = rstart;
				}
			}
		}
	}

	//마지막 팟에 들어가있는 머니가 0이면 마지막 팟은 제거해 준다.
	if(m_nMaxPot > 0 && ((m_listSidePot[m_nMaxPot] == 0) || (m_nMaxPot == m_nMaxPlayer-1))) 
	{
		for(i = 0; i < m_nMaxPlayer; ++i)
		{
			m_mapUserSidePot[i].mapSidePotMoney.erase(m_nMaxPot);
		}

		m_nMaxPot--;
	}
}

void CSidePotMoney::DrawBottom(CDC *pDC)
{
	return;

	//0번 팟만 존재한다면 그리지 않는다.
	if(m_nMaxPot < 1)
	{
		return;
	}

	//확장일때 그리기
	if(m_bExpand)
	{
		CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

		int nHeight = m_sprSidePot.spr[8].yl;
		int nX = m_pUIInfoData->m_listButtonPos[EBT_SIDEPOT].x;
		int nY = m_pUIInfoData->m_listButtonPos[EBT_SIDEPOT].y - nHeight * (m_nMaxPot + 1);
		
		m_btnSidePotOff.SetButtonPos(nX, nY);
		m_btnSidePotOff.Draw(pDC, AB, 31);

		nY += m_sprSidePot.spr[4].yl;

		CRect rtRect(0,0,34,15);
		COLORREF clrOld = pDC->GetTextColor();
		CString strOutput;

		for(int i = 0; i <= m_nMaxPot; ++i)
		{
			m_pGameViewBase->Page.PutSprAuto(nX, nY, &m_sprSidePot, 8, AB);
			
			//머니 그리기
			rtRect = m_pUIInfoData->m_rtSidePotMoneyName;
			rtRect.OffsetRect(CPoint(nX, nY) + m_pUIInfoData->m_ptSidePotMoneyNameGab);
			pDC->SetTextColor(m_pUIInfoData->m_clrSidePotMoneyName);

			strOutput.Format("%d번 Pot", i+1);
			pDC->DrawText(strOutput, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

			rtRect = m_pUIInfoData->m_rtSidePotMoney;
			rtRect.OffsetRect(CPoint(nX, nY) + m_pUIInfoData->m_ptSidePotMoneyGab);
			pDC->SetTextColor(m_pUIInfoData->m_clrSidePotMoney);
			strOutput = (CString)NMBASE::UTIL::g_MoneyMark(m_listSidePot[i]);
			pDC->DrawText(strOutput, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

			nY += nHeight;
		}

		pDC->SetTextColor(clrOld);
		pDC->SelectObject(pOldFont);
	}
	//확장이 아닐때 그리기
	else
	{
		m_btnSidePotOn.Draw(pDC, AB, 31);
	}
}

bool CSidePotMoney::IsOpponentAllFold(int nPNum)
{
	for(int i=0;i<m_nMaxPlayer;i++)
	{
		if(i == nPNum) continue;
		if(m_mapUserSidePot[i].mapSidePotMoney.size() && !m_mapUserSidePot[i].bFold) return false;
	}

	return true;
}

bool CSidePotMoney::IsUserParticipation(int nPNum, int nPot)
{
	map<int, USER_SIDEPOT>::iterator findIt = m_mapUserSidePot.find(nPNum);
	if(findIt == m_mapUserSidePot.end()) return false;
	map<int, INT64>::iterator find_pot = m_mapUserSidePot[nPNum].mapSidePotMoney.find(nPot);
	if(find_pot == m_mapUserSidePot[nPNum].mapSidePotMoney.end()) return false;
	return true;
}

LIST_SAME_PLAYER CSidePotMoney::GetWinner(int nPot)
{
	MAP_RANKING::iterator start = m_mapRanking.begin();
	MAP_RANKING::iterator end = m_mapRanking.end();

	LIST_SAME_PLAYER listRet;
	LIST_SAME_PLAYER::iterator start_pnum, end_pnum;

	for(;start != end;start++)
	{
		listRet.clear();

		start_pnum = start->second.begin();
		end_pnum = start->second.end();

		for(;start_pnum != end_pnum;start_pnum++)
		{
			if(IsUserParticipation(start_pnum->first, nPot))
				listRet.insert(*start_pnum);
		}


		if(listRet.size()) break;
	}

	return listRet;
}

void CSidePotMoney::DrawTop(CDC *pDC)
{
	if(m_nMaxPot < 1)
	{
		return;
	}

	if(m_bGameOver)
	{
		return;
		USER_SIDEPOT &mySidePotInfo = m_mapUserSidePot[0];
		CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(0);
		if(mySidePotInfo.mapSidePotMoney.size() && (pPlayer->UI.ID[0] != 0) && (pPlayer->JoinState == 1))
		{
			m_pGameViewBase->Page.PutSprAuto(m_pUIInfoData->m_ptSidePotResult.x, m_pUIInfoData->m_ptSidePotResult.y, &m_sprResult, 0);
			for(int i=0;i<m_nShowResultNumber;i++)
			{
				m_listResult_VS[i]->DrawText(pDC);
				m_listResult_Money[i]->DrawText(pDC);
			}

			m_pResultTotalMoney->DrawText(pDC);
		}
	}
	else
	{
		int nPlayIndex = 0;
		CPlayer *pPlayer = NULL;
		for(int i = 0; i < m_nMaxPlayer; ++i)
		{
			nPlayIndex = i;
			pPlayer = GM().GetCurrentProcess()->GetPlayer(i);
			if(m_mapUserSidePot[i].bUse && !m_mapUserSidePot[i].bFold && (pPlayer->UI.ID[0] != 0) && (pPlayer->JoinState == 1))
			{
				m_pGameViewBase->Page.DrawMultiScene(0, 0, &m_sprDisplayBoard, nPlayIndex, 0);
			}
		}
	}
}

BOOL CSidePotMoney::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(m_nMaxPot < 1) 
	{
		return FALSE;
	}

	switch(wParam)
	{
		case UM_SIDEPOT_ON:
			SetExpand(true);
			return TRUE;

		case UM_SIDEPOT_OFF:
			SetExpand(false);
			return TRUE;

		default:
			break;
	}

	return FALSE;
}

BOOL CSidePotMoney::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_nMaxPot < 1)
	{
		return FALSE;
	}

	int mxp, myp;
	mxp = LOWORD(lParam);
	myp = HIWORD(lParam);
	switch(message)
	{
		case WM_LBUTTONDOWN:
			{
				if (m_btnSidePotOn.OnLButtonDown(mxp,myp))	return TRUE;
				if (m_btnSidePotOff.OnLButtonDown(mxp,myp))	return TRUE;
			}
			break;
		case WM_LBUTTONUP:
			{
				if (m_btnSidePotOn.OnLButtonUp(mxp,myp))	return TRUE;
				if (m_btnSidePotOff.OnLButtonUp(mxp,myp))	return TRUE;
			}
			break;
		case WM_MOUSEMOVE:
			{
				m_btnSidePotOn.OnMouseMove(mxp,myp);
				m_btnSidePotOff.OnMouseMove(mxp,myp);
			}
			break;

		default:
			break;
	}

	return FALSE;
}