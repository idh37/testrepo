// DisplayBoard.cpp: implementation of the CDisplayBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DisplayBoard.h"
#include "../UIInfoData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDisplayBoard::CDisplayBoard()
{
	m_nTextIndex = EDBT_UNKNOWN;
}

CDisplayBoard::~CDisplayBoard()
{
}

void CDisplayBoard::SetText(ENUM_DISPLAY_BOARD_TEXT nTextIndex, LPARAM lParam /*=0*/)
{
	m_nTextIndex = nTextIndex;
	switch(nTextIndex)
	{
	case EDBT_2BETTING:
	case EDBT_3BETTING:
	case EDBT_4BETTING :
	case EDBT_5BETTING :
	case EDBT_6BETTING :
	case EDBT_7BETTING :
	case EDBT_HIDDENBETTING :
	case EDBT_BEFORE_MORNING_BETTING:
	case EDBT_MORNING_BETTING:
	case EDBT_AFTERNOON_BETTING:
	case EDBT_NIGHT_BETTING:
		{
			int nRase = 0;

			if(nTextIndex >= EDBT_BEFORE_MORNING_BETTING)
			{
				nRase = nTextIndex - EDBT_BEFORE_MORNING_BETTING + 1;
			}
			else
			{
				nRase = nTextIndex - EDBT_4BETTING + 1;
				if((nTextIndex == EDBT_HIDDENBETTING) && (m_pGameProcess->GetGameType() != IDX_GAME_NP)) nRase--;
			}

			m_mapText[nTextIndex][0].strOutput = m_mapText[nTextIndex][0].strFormat;
		}
		break;
	case EDBT_BOMBCARD:
		m_mapText[EDBT_BOMBCARD][0].strOutput.Format(m_mapText[EDBT_BOMBCARD][0].strFormat, (char *)lParam);
		break;
	case EDBT_RESULT:
		{
			GAMEOVERRESULT *pGOR = (GAMEOVERRESULT *)lParam;
			CString strWinMoney;
			CString strJokbo = "기권승";
			int wp = m_pGameProcess->GetPNum_ByServPN(pGOR->nNowWinner[0]);
			CPlayer *pPlayer = (CPlayer *)m_pGameProcess->GetPlayer(wp);

			if(pGOR->bOpen)
			{
				MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)wp, (LPARAM)&strJokbo);
			}

			if(GM().GetCurrentGameType() == IDX_GAME_BD)
			{
				strWinMoney = (CString)NMBASE::UTIL::g_MoneyMark(pGOR->Ui[pGOR->nNowWinner[0]].nResultMoney, 2);
			}
			else
			{
				strWinMoney = (CString)NMBASE::UTIL::g_MoneyMark(pGOR->nWinMoney, 2);
			}

			strWinMoney.Append(strChip_Name[g_RI.ChipKind]);

			if(pGOR->nWinnerNum > 1)
			{
				nTextIndex = EDBT_RESULT2;	
				m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, pGOR->nWinnerNum);
				m_mapText[nTextIndex][1].strOutput = strJokbo;
			}
			else
			{
				m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, pPlayer->UI.NickName);
				m_mapText[nTextIndex][1].strOutput = strWinMoney;
			}
		}
		break;
	case EDBT_RESULT_HILOW_1WIN:	//하이로우 승자 1명
		{
			GAMEOVERRESULT *pGOR = (GAMEOVERRESULT *)lParam;			

			CString strJokbo = "기권";			
			if(pGOR->bOpen)
			{
				switch(pGOR->WinCase)
				{
				case 1:  strJokbo = "하이"; break;
				case 2:  strJokbo = "로우"; break;
				case 3:  strJokbo = "스윙"; break;
				}				
			}

			int nWinnerIndex = pGOR->nNowWinner[IDX_HL_1ST_WINNER];
			if (nWinnerIndex<0 || nWinnerIndex>=MAX_PLAYER)
				break;

			//OOOO님이 OO승
			m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, pGOR->Ui[nWinnerIndex].NickName, strJokbo);
			//획득머니
			m_mapText[nTextIndex][1].strOutput = (CString)NMBASE::UTIL::g_MoneyMark(pGOR->Ui[nWinnerIndex].nResultMoney, 2);
		} 
		break;

	case EDBT_RESULT_HILOW_2WIN:	//하이로우 승자 2명
		{
			GAMEOVERRESULT *pGOR = (GAMEOVERRESULT *)lParam;
			CString strWinMoney;



			int nHighWinner = pGOR->nNowWinner[IDX_HL_1ST_WINNER];
			int nLowWinner = pGOR->nNowWinner[IDX_HL_2ND_WINNER];
			if (nHighWinner<0 || nHighWinner>=MAX_PLAYER)
				break;
			if (nLowWinner<0 || nLowWinner>=MAX_PLAYER)
				break;

			int nHighPNum = m_pGameProcess->GetPNum_ByServPN(nHighWinner);
			int nLowPNum = m_pGameProcess->GetPNum_ByServPN(nLowWinner);
			if (nHighPNum<0 || nLowPNum<0)
				break;

			CString strHighJokbo, strLowJokbo;
			MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nHighPNum, (LPARAM)&strHighJokbo);
			MM().Call(UPG_CMK_GET_LOW_JOKBO_NAME, (WPARAM)nLowPNum, (LPARAM)&strLowJokbo);


			//"%s님 %s 하이승/ %s님 %s 로우승";
// 			m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, 
// 				pGOR->Ui[nHighWinner].NickName,
// 				strHighJokbo,
// 				pGOR->Ui[nLowWinner].NickName,
// 				strLowJokbo
// 				);			


			m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, pGOR->Ui[nHighWinner].NickName);
			m_mapText[nTextIndex][1].strOutput.Format(m_mapText[nTextIndex][1].strFormat, strHighJokbo);
			m_mapText[nTextIndex][2].strOutput.Format(m_mapText[nTextIndex][2].strFormat, pGOR->Ui[nLowWinner].NickName);
			m_mapText[nTextIndex][3].strOutput.Format(m_mapText[nTextIndex][3].strFormat, strLowJokbo);
			m_mapText[nTextIndex][4].strOutput.Format(m_mapText[nTextIndex][4].strFormat);


		}
		break;

	case EDBT_RESULT_SUTDA:
		{
			GAMEOVERRESULT *pGOR = (GAMEOVERRESULT *)lParam;
			CString strWinMoney;

			int nWinnerServPnum = pGOR->nNowWinner[IDX_SP_WINNER];			
			if (nWinnerServPnum<0 || nWinnerServPnum>=MAX_PLAYER)
				break;	

			strWinMoney = (CString)NMBASE::UTIL::g_MoneyMark(pGOR->Ui[nWinnerServPnum].nResultMoney, 2);

			strWinMoney.Append(strChip_Name[g_RI.ChipKind]);

			m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, pGOR->Ui[nWinnerServPnum].NickName);
			m_mapText[nTextIndex][1].strOutput = strWinMoney;
		
		} break;
	case EDBT_MISSION_COMPLETE_HOOLA:
		{
			CString strNickName;
			strNickName.Format("%s",(char*) lParam);
			COLORREF clrText = RGB( 146, 68, 153 );
			m_mapText[nTextIndex][0].clrColor = clrText;
			m_mapText[nTextIndex][0].strOutput.Format("%s 님께서 미션에 성공하셨습니다.",strNickName.GetString());			
		}
		break;
	case EDBT_STOPJACKPOT_BELOW_1JO_HOOLA:
		{
			CString strNickName;		//잭팟 당첨자 닉네임
			CString strJackpotMoney;	//잭팟 당첨 금액

			jackpot::JP_WIN* pAlram = (jackpot::JP_WIN*) lParam;

			//%s님, 잭팟성공! (146,68,153)
			strNickName.Format("%s",pAlram->JData.szNickName);
			COLORREF clrText = RGB( 146, 68, 153 );
			m_mapText[nTextIndex][0].clrColor = clrText;
			m_mapText[nTextIndex][0].strOutput.Format("%s님, 잭팟성공! ",strNickName.GetString());	

			//%s(155,156,81)XXX조
			strJackpotMoney = NMBASE::UTIL::g_MakeShortMoneyStr(pAlram->JData.llBonusPMoney);
			clrText = RGB( 155,156,81 );
			m_mapText[nTextIndex][1].clrColor = clrText;
			m_mapText[nTextIndex][1].strOutput.Format("%s",strJackpotMoney.GetString());	

			//획득 (146,68,153)
			clrText = RGB( 146,68,153 );
			m_mapText[nTextIndex][2].clrColor = clrText;
			m_mapText[nTextIndex][2].strOutput.Format(" 획득");
			
		}
		break;
	case EDBT_STOPJACKPOT_UPPER_3JO_HOOLA:
		{
			CString strNickName;		//잭팟 당첨자 닉네임
			CString strJackpotMoney;	//잭팟 당첨 금액

			jackpot::JP_WIN* pAlram = (jackpot::JP_WIN*) lParam;

			//%s님, 잭팟성공! (146,68,153)
			strNickName.Format("%s",pAlram->JData.szNickName);
			COLORREF clrText = RGB( 146, 68, 153 );
			m_mapText[nTextIndex][0].clrColor = clrText;
			m_mapText[nTextIndex][0].strOutput.Format("%s님, 잭팟성공! ",strNickName.GetString());	

			//%s(155,156,81)XXX조
			strJackpotMoney = NMBASE::UTIL::g_MakeShortMoneyStr(pAlram->JData.llBonusPMoney);
			clrText = RGB( 155,156,81 );
			m_mapText[nTextIndex][1].clrColor = clrText;
			m_mapText[nTextIndex][1].strOutput.Format("%s",strJackpotMoney.GetString());	

			//획득(146,68,153)
			clrText = RGB( 146,68,153 );
			m_mapText[nTextIndex][2].clrColor = clrText;
			m_mapText[nTextIndex][2].strOutput.Format(" 획득");
		}
		break;
	default:
		break;
	}
}

void CDisplayBoard::Init(CUIInfoData *pUIInfoData, CGame *pGame)
{
	m_rtSize = pUIInfoData->m_rtDisplayBoard;
	m_ptPos = pUIInfoData->m_ptDisplayBoard;
	m_pGameProcess = pGame->GetGameProcess();

	map<int, DrawTextInfo> mapTemp;

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "게임 정보가 여기에 표시됩니다.";
	m_mapText[EDBT_UNKNOWN] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "최소 2명부터 게임이 진행됩니다.";
	m_mapText[EDBT_MINUSER] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "오픈할 카드를 선택하세요";
	m_mapText[EDBT_OPENCARD_CHOICE] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "버릴카드와 오픈할카드를 선택하세요.";
	m_mapText[EDBT_SELECTCARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "버릴카드를 선택하세요.";
	m_mapText[EDBT_THROWAWAY_CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "자동베팅이 진행중입니다.";
	m_mapText[EDBT_AUTOBETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "2구 베팅이 진행중입니다.";
	m_mapText[EDBT_2BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "3구 베팅이 진행중입니다.";
	m_mapText[EDBT_3BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "4구 베팅이 진행중입니다.";
	m_mapText[EDBT_4BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "5구 베팅이 진행중입니다.";
	m_mapText[EDBT_5BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "6구 베팅이 진행중입니다.";
	m_mapText[EDBT_6BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "7구 베팅이 진행중입니다.";
	m_mapText[EDBT_7BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "마지막 히든 베팅이 진행중입니다.";
	m_mapText[EDBT_HIDDENBETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%d번 베팅할 수 있습니다.";
	m_mapText[EDBT_BETTING_COUNT] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "게임이 시작되었습니다.";
	m_mapText[EDBT_GAMESTART] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "2번째 카드입니다.";
	m_mapText[EDBT_2CARD] = mapTemp;


	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "3번째 카드입니다.";
	m_mapText[EDBT_3CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "4번째 카드입니다.";
	m_mapText[EDBT_4CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "5번째 카드입니다.";
	m_mapText[EDBT_5CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "6번째 카드입니다.";
	m_mapText[EDBT_6CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "이번판은 %s 폭탄카드가 분배됩니다";
	m_mapText[EDBT_BOMBCARD] = mapTemp;

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "이번판은 조커카드가 분배됩니다.";
	m_mapText[EDBT_JOKERCARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "마지막 히든 카드입니다.";
	m_mapText[EDBT_HIDDENCARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s 님 승리, ";

	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayMoney;
	mapTemp[1].strOutput = "머니";

	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = " 획득하셨습니다.";

	m_mapText[EDBT_RESULT] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%d명이 ";

	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayJokbo;
	mapTemp[1].strOutput = "족보";

	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = "로 공동 우승하셨습니다.";

	m_mapText[EDBT_RESULT2] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "제한시간은 10초입니다. 2번 이상 제한시간을 넘기면 강제로 퇴장됩니다.";
	m_mapText[EDBT_LIMITTIME] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "아침 전 베팅이 진행중입니다.";
	m_mapText[EDBT_BEFORE_MORNING_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "아침 베팅이 진행중입니다.";
	m_mapText[EDBT_MORNING_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "점심 베팅이 진행중입니다.";
	m_mapText[EDBT_AFTERNOON_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "마지막 저녁 베팅이 진행중입니다.";
	m_mapText[EDBT_NIGHT_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "아침 컷팅이 진행중입니다. 바꿀 카드를 선택하세요";
	m_mapText[EDBT_MORNING_CUTTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "점심 컷팅이 진행중입니다. 바꿀 카드를 선택하세요";
	m_mapText[EDBT_AFTERNOON_CUTTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "마지막 저녁 컷팅이 진행중입니다. 바꿀 카드를 선택하세요";
	m_mapText[EDBT_NIGHT_CUTTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "하이,로우,스윙을 선택하세요.";
	m_mapText[EDBT_SELECT_HLS] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s님 %s 승, ";
	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayMoney;
	mapTemp[1].strFormat = "%I64d";
	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = " 획득하셨습니다.";
	m_mapText[EDBT_RESULT_HILOW_1WIN] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s님 ";
	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayJokbo;
	mapTemp[1].strFormat = "%s";
	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strFormat = " 하이승 / %s님 ";
	mapTemp[3].clrColor = pUIInfoData->m_clrDisplayJokbo;
	mapTemp[3].strFormat = "%s";
	mapTemp[4].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[4].strFormat = " 로우승";
	m_mapText[EDBT_RESULT_HILOW_2WIN] = mapTemp;

	mapTemp.clear();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///섯다 전용 메세지
	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "첫 번째 베팅이 진행 중입니다.";
	m_mapText[EDBT_FIRST_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "마지막 베팅이 진행 중입니다.";
	m_mapText[EDBT_FINAL_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "마지막 패입니다.";
	m_mapText[EDBT_FINAL_DEAL] = mapTemp;

	mapTemp.clear();


	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "동일패 재경기를 진행합니다.";
	m_mapText[EDBT_NOTIFY_REMATCH_BY_DRAW] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "사구 재경기를 진행합니다.";
	m_mapText[EDBT_NOTIFY_REMATCH_BY_49] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s 님 ";
	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayMoney;
	mapTemp[1].strFormat = "%s";
	mapTemp[1].strFormat.Append(strChip_Name[g_RI.ChipKind]);
	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = " 획득하셨습니다.";
	m_mapText[EDBT_RESULT_SUTDA] = mapTemp;

	mapTemp.clear();


	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "패를 선택해주세요.";
	m_mapText[EDBT_SELECTCARD_SUTDA] = mapTemp;

	mapTemp.clear();


	// 훌라 전광판 메시지
	COLORREF clrText = RGB( 146, 68, 153 );
	COLORREF clrMoney = RGB( 155, 156, 81 );
	
	mapTemp[0].clrColor = clrText;
	mapTemp[0].strOutput = "스톱훌라는 언제든지 스톱하여 게임을 끝낼 수 있습니다.";
	m_mapText[EDBT_NORMAL0_HOOLA] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = clrText;
	mapTemp[0].strOutput = "받기 카드가 0장이 되면 게임이 자동 종료됩니다.";
	m_mapText[EDBT_NORMAL1_HOOLA] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = clrText;
	mapTemp[0].strOutput = "선정하기에서 높은 숫자를 선택한 사람이 선이 됩니다.";
	m_mapText[EDBT_NORMAL2_HOOLA] = mapTemp;

	mapTemp.clear();

	SetText(EDBT_UNKNOWN);
}

int CDisplayBoard::GetTextWidth(CDC *pDC)
{
	map<int, DrawTextInfo> &mapTemp = m_mapText[m_nTextIndex];

	int nWidth = 0;
	map<int, DrawTextInfo>::iterator start = mapTemp.begin();
	map<int, DrawTextInfo>::iterator end = mapTemp.end();

	CSize szSize;
	for(;start != end;start++)
	{
		CString &strOutput = start->second.strOutput;
		::GetTextExtentPoint( pDC->m_hDC, (LPCTSTR)strOutput, strOutput.GetLength(), &szSize );
		nWidth += szSize.cx;
	}

	return nWidth;
}

void CDisplayBoard::Draw(CDC *pDC)
{
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

	CRect rtTitle = m_rtSize;
	rtTitle.OffsetRect(m_ptPos);

	int nWidth = GetTextWidth(pDC);
	int nStartX = m_ptPos.x + (m_rtSize.Width() - nWidth)/2;

	map<int, DrawTextInfo> &mapTemp = m_mapText[m_nTextIndex];
	map<int, DrawTextInfo>::iterator start = mapTemp.begin();
	map<int, DrawTextInfo>::iterator end = mapTemp.end();

	CSize szSize;
	for(;start != end;start++)
	{
		pDC->SetTextColor(start->second.clrColor);
		if(nStartX > m_ptPos.x)
			rtTitle.left = nStartX;

		CString &strOutput = start->second.strOutput;
		pDC->DrawText(strOutput, &rtTitle, DT_SINGLELINE | DT_VCENTER);

		::GetTextExtentPoint( pDC->m_hDC, (LPCTSTR)strOutput, strOutput.GetLength(), &szSize );
		nStartX += szSize.cx;
	}

	pDC->SelectObject(pOldFont);
}

int CDisplayBoard::GetRaseCount(int nRase)
{
	return g_RI.arrRaiseStyleDesc[nRase];
}