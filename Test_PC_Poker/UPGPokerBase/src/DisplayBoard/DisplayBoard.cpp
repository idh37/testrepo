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
			CString strJokbo = "��ǽ�";
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
	case EDBT_RESULT_HILOW_1WIN:	//���̷ο� ���� 1��
		{
			GAMEOVERRESULT *pGOR = (GAMEOVERRESULT *)lParam;			

			CString strJokbo = "���";			
			if(pGOR->bOpen)
			{
				switch(pGOR->WinCase)
				{
				case 1:  strJokbo = "����"; break;
				case 2:  strJokbo = "�ο�"; break;
				case 3:  strJokbo = "����"; break;
				}				
			}

			int nWinnerIndex = pGOR->nNowWinner[IDX_HL_1ST_WINNER];
			if (nWinnerIndex<0 || nWinnerIndex>=MAX_PLAYER)
				break;

			//OOOO���� OO��
			m_mapText[nTextIndex][0].strOutput.Format(m_mapText[nTextIndex][0].strFormat, pGOR->Ui[nWinnerIndex].NickName, strJokbo);
			//ȹ��Ӵ�
			m_mapText[nTextIndex][1].strOutput = (CString)NMBASE::UTIL::g_MoneyMark(pGOR->Ui[nWinnerIndex].nResultMoney, 2);
		} 
		break;

	case EDBT_RESULT_HILOW_2WIN:	//���̷ο� ���� 2��
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


			//"%s�� %s ���̽�/ %s�� %s �ο��";
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
			m_mapText[nTextIndex][0].strOutput.Format("%s �Բ��� �̼ǿ� �����ϼ̽��ϴ�.",strNickName.GetString());			
		}
		break;
	case EDBT_STOPJACKPOT_BELOW_1JO_HOOLA:
		{
			CString strNickName;		//���� ��÷�� �г���
			CString strJackpotMoney;	//���� ��÷ �ݾ�

			jackpot::JP_WIN* pAlram = (jackpot::JP_WIN*) lParam;

			//%s��, ���̼���! (146,68,153)
			strNickName.Format("%s",pAlram->JData.szNickName);
			COLORREF clrText = RGB( 146, 68, 153 );
			m_mapText[nTextIndex][0].clrColor = clrText;
			m_mapText[nTextIndex][0].strOutput.Format("%s��, ���̼���! ",strNickName.GetString());	

			//%s(155,156,81)XXX��
			strJackpotMoney = NMBASE::UTIL::g_MakeShortMoneyStr(pAlram->JData.llBonusPMoney);
			clrText = RGB( 155,156,81 );
			m_mapText[nTextIndex][1].clrColor = clrText;
			m_mapText[nTextIndex][1].strOutput.Format("%s",strJackpotMoney.GetString());	

			//ȹ�� (146,68,153)
			clrText = RGB( 146,68,153 );
			m_mapText[nTextIndex][2].clrColor = clrText;
			m_mapText[nTextIndex][2].strOutput.Format(" ȹ��");
			
		}
		break;
	case EDBT_STOPJACKPOT_UPPER_3JO_HOOLA:
		{
			CString strNickName;		//���� ��÷�� �г���
			CString strJackpotMoney;	//���� ��÷ �ݾ�

			jackpot::JP_WIN* pAlram = (jackpot::JP_WIN*) lParam;

			//%s��, ���̼���! (146,68,153)
			strNickName.Format("%s",pAlram->JData.szNickName);
			COLORREF clrText = RGB( 146, 68, 153 );
			m_mapText[nTextIndex][0].clrColor = clrText;
			m_mapText[nTextIndex][0].strOutput.Format("%s��, ���̼���! ",strNickName.GetString());	

			//%s(155,156,81)XXX��
			strJackpotMoney = NMBASE::UTIL::g_MakeShortMoneyStr(pAlram->JData.llBonusPMoney);
			clrText = RGB( 155,156,81 );
			m_mapText[nTextIndex][1].clrColor = clrText;
			m_mapText[nTextIndex][1].strOutput.Format("%s",strJackpotMoney.GetString());	

			//ȹ��(146,68,153)
			clrText = RGB( 146,68,153 );
			m_mapText[nTextIndex][2].clrColor = clrText;
			m_mapText[nTextIndex][2].strOutput.Format(" ȹ��");
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
	mapTemp[0].strOutput = "���� ������ ���⿡ ǥ�õ˴ϴ�.";
	m_mapText[EDBT_UNKNOWN] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "�ּ� 2����� ������ ����˴ϴ�.";
	m_mapText[EDBT_MINUSER] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ī�带 �����ϼ���";
	m_mapText[EDBT_OPENCARD_CHOICE] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "����ī��� ������ī�带 �����ϼ���.";
	m_mapText[EDBT_SELECTCARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "����ī�带 �����ϼ���.";
	m_mapText[EDBT_THROWAWAY_CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "�ڵ������� �������Դϴ�.";
	m_mapText[EDBT_AUTOBETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "2�� ������ �������Դϴ�.";
	m_mapText[EDBT_2BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "3�� ������ �������Դϴ�.";
	m_mapText[EDBT_3BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "4�� ������ �������Դϴ�.";
	m_mapText[EDBT_4BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "5�� ������ �������Դϴ�.";
	m_mapText[EDBT_5BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "6�� ������ �������Դϴ�.";
	m_mapText[EDBT_6BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "7�� ������ �������Դϴ�.";
	m_mapText[EDBT_7BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "������ ���� ������ �������Դϴ�.";
	m_mapText[EDBT_HIDDENBETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%d�� ������ �� �ֽ��ϴ�.";
	m_mapText[EDBT_BETTING_COUNT] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ���۵Ǿ����ϴ�.";
	m_mapText[EDBT_GAMESTART] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "2��° ī���Դϴ�.";
	m_mapText[EDBT_2CARD] = mapTemp;


	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "3��° ī���Դϴ�.";
	m_mapText[EDBT_3CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "4��° ī���Դϴ�.";
	m_mapText[EDBT_4CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "5��° ī���Դϴ�.";
	m_mapText[EDBT_5CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "6��° ī���Դϴ�.";
	m_mapText[EDBT_6CARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "�̹����� %s ��źī�尡 �й�˴ϴ�";
	m_mapText[EDBT_BOMBCARD] = mapTemp;

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "�̹����� ��Ŀī�尡 �й�˴ϴ�.";
	m_mapText[EDBT_JOKERCARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ���� ī���Դϴ�.";
	m_mapText[EDBT_HIDDENCARD] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s �� �¸�, ";

	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayMoney;
	mapTemp[1].strOutput = "�Ӵ�";

	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = " ȹ���ϼ̽��ϴ�.";

	m_mapText[EDBT_RESULT] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%d���� ";

	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayJokbo;
	mapTemp[1].strOutput = "����";

	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = "�� ���� ����ϼ̽��ϴ�.";

	m_mapText[EDBT_RESULT2] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "���ѽð��� 10���Դϴ�. 2�� �̻� ���ѽð��� �ѱ�� ������ ����˴ϴ�.";
	m_mapText[EDBT_LIMITTIME] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "��ħ �� ������ �������Դϴ�.";
	m_mapText[EDBT_BEFORE_MORNING_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "��ħ ������ �������Դϴ�.";
	m_mapText[EDBT_MORNING_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "���� ������ �������Դϴ�.";
	m_mapText[EDBT_AFTERNOON_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "������ ���� ������ �������Դϴ�.";
	m_mapText[EDBT_NIGHT_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "��ħ ������ �������Դϴ�. �ٲ� ī�带 �����ϼ���";
	m_mapText[EDBT_MORNING_CUTTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "���� ������ �������Դϴ�. �ٲ� ī�带 �����ϼ���";
	m_mapText[EDBT_AFTERNOON_CUTTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ���� ������ �������Դϴ�. �ٲ� ī�带 �����ϼ���";
	m_mapText[EDBT_NIGHT_CUTTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "����,�ο�,������ �����ϼ���.";
	m_mapText[EDBT_SELECT_HLS] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s�� %s ��, ";
	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayMoney;
	mapTemp[1].strFormat = "%I64d";
	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = " ȹ���ϼ̽��ϴ�.";
	m_mapText[EDBT_RESULT_HILOW_1WIN] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s�� ";
	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayJokbo;
	mapTemp[1].strFormat = "%s";
	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strFormat = " ���̽� / %s�� ";
	mapTemp[3].clrColor = pUIInfoData->m_clrDisplayJokbo;
	mapTemp[3].strFormat = "%s";
	mapTemp[4].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[4].strFormat = " �ο��";
	m_mapText[EDBT_RESULT_HILOW_2WIN] = mapTemp;

	mapTemp.clear();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///���� ���� �޼���
	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "ù ��° ������ ���� ���Դϴ�.";
	m_mapText[EDBT_FIRST_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ������ ���� ���Դϴ�.";
	m_mapText[EDBT_FINAL_BETTING] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ���Դϴ�.";
	m_mapText[EDBT_FINAL_DEAL] = mapTemp;

	mapTemp.clear();


	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "������ ���⸦ �����մϴ�.";
	m_mapText[EDBT_NOTIFY_REMATCH_BY_DRAW] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "�籸 ���⸦ �����մϴ�.";
	m_mapText[EDBT_NOTIFY_REMATCH_BY_49] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strFormat = "%s �� ";
	mapTemp[1].clrColor = pUIInfoData->m_clrDisplayMoney;
	mapTemp[1].strFormat = "%s";
	mapTemp[1].strFormat.Append(strChip_Name[g_RI.ChipKind]);
	mapTemp[2].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[2].strOutput = " ȹ���ϼ̽��ϴ�.";
	m_mapText[EDBT_RESULT_SUTDA] = mapTemp;

	mapTemp.clear();


	mapTemp[0].clrColor = pUIInfoData->m_clrDisplayNormal;
	mapTemp[0].strOutput = "�и� �������ּ���.";
	m_mapText[EDBT_SELECTCARD_SUTDA] = mapTemp;

	mapTemp.clear();


	// �Ƕ� ������ �޽���
	COLORREF clrText = RGB( 146, 68, 153 );
	COLORREF clrMoney = RGB( 155, 156, 81 );
	
	mapTemp[0].clrColor = clrText;
	mapTemp[0].strOutput = "�����Ƕ�� �������� �����Ͽ� ������ ���� �� �ֽ��ϴ�.";
	m_mapText[EDBT_NORMAL0_HOOLA] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = clrText;
	mapTemp[0].strOutput = "�ޱ� ī�尡 0���� �Ǹ� ������ �ڵ� ����˴ϴ�.";
	m_mapText[EDBT_NORMAL1_HOOLA] = mapTemp;

	mapTemp.clear();

	mapTemp[0].clrColor = clrText;
	mapTemp[0].strOutput = "�����ϱ⿡�� ���� ���ڸ� ������ ����� ���� �˴ϴ�.";
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