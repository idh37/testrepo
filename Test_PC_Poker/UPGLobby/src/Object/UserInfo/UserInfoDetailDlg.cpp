#include "StdAfx.h"
#include "UserInfoDetailDlg.h"
#include "../../WidgetID.h"

CUserInfoDetailDlg::CUserInfoDetailDlg()
:	m_pBtnDetailClose(NULL), // 맴버스 상세보기 닫기 버튼
	m_pLevel(NULL), // 등급 텍스트
	m_pInsurance(NULL), // 보험 텍스트
	m_pMembers(NULL), // 맴버스 텍스트
	m_pInsuranceGold1(NULL), // 골드 보험 텍스트1
	m_pInsuranceGold2(NULL), // 골드 보험 텍스트2
	m_pBtnInsuranceReceive(NULL), // 보험 지급받기 버튼
	m_pBtnMembersPreview(NULL), // 맴버스 혜택보기 버튼
	m_pBtnInsuranceGoldReceive(NULL), // 골드 보험 지급받기 버튼
	m_pUserInfo(NULL)
{
}

CUserInfoDetailDlg::~CUserInfoDetailDlg()
{
}

void CUserInfoDetailDlg::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	// 맴버스 상세보기 닫기 버튼
	case ID_LOBBY_BTN_MYINFO_DETAIL_CLOSE:
		m_pBtnDetailClose = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	// 등급 텍스트
	case ID_LOBBY_TXT_MYINFO_DETAIL_LEVEL:
		{
			m_pLevel = (LSY::CText *)pObject;
			m_pLevel->SetText(GetGradeName(0).GetString());
		}
		break;
	// 보험 텍스트
	case ID_LOBBY_TXT_MYINFO_DETAIL_INSURANCE:
		{
			m_pInsurance = (LSY::CText *)pObject;
			m_pInsurance->SetText("");
		}
		break;
	// 맴버스 텍스트
	case ID_LOBBY_TXT_MYINFO_DETAIL_MEMBERS:
		{
			m_pMembers = (LSY::CText *)pObject;
			m_pMembers->SetText("");
		}
		break;
	// 골드 보험 텍스트1
	case ID_LOBBY_TXT_MYINFO_DETAIL_INSURANCE_GOLD1:
		{
			m_pInsuranceGold1 = (LSY::CText *)pObject;
		}
		break;
	// 골드 보험 텍스트2
	case ID_LOBBY_TXT_MYINFO_DETAIL_INSURANCE_GOLD2:
		{
			m_pInsuranceGold2 = (LSY::CText *)pObject;
		}
		break;
	// 보험 지급받기 버튼
	case ID_LOBBY_BTN_MYINFO_INSURANCE_RECEIVE:
		{
			m_pBtnInsuranceReceive = (LSY::CButton *)pObject;
			bAddChild = true;
		}
		break;
	// 맴버스 혜택보기 버튼
	case ID_LOBBY_BTN_MYINFO_MEMBERS_PREVIEW:
		{
			m_pBtnMembersPreview = (LSY::CButton *)pObject;
			bAddChild = true;
		}
		break;
	// 골드 보험 지급받기 버튼
	case ID_LOBBY_BTN_MYINFO_INSURANCE_RECEIVE_GOLD:
		{
			m_pBtnInsuranceGoldReceive = (LSY::CButton *)pObject;
			bAddChild = true;
		}
		break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CUserInfoDetailDlg::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

LRESULT CUserInfoDetailDlg::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();

	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_MYINFO_DETAIL_CLOSE:
		SetShow(false);
		break;
	case ID_LOBBY_BTN_MYINFO_INSURANCE_RECEIVE:
		{
			SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_PERSONAL_AREA_GET_MONEY);
			pWnd->SendMessage(LMV_BUTTON_ID_JOIN_INSURANCE);
		}
		break;
		// 골드 보험 지급받기 버튼
	case ID_LOBBY_BTN_MYINFO_INSURANCE_RECEIVE_GOLD:
		{
			SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_PERSONAL_AREA_GET_MONEY);
			pWnd->SendMessage(LMV_BUTTON_ID_JOIN_INSURANCE_GOLD);
		}
		break;
	case ID_LOBBY_BTN_MYINFO_MEMBERS_PREVIEW:
		if(m_pUserInfo)
		{
			int nPremCode= UPGCheckPremiumState(m_pUserInfo);

			if (nPremCode == CODE_NEW_FAMILY_NONE)
				SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_PERSONAL_AREA_SHOW_BENEFIT);
			else
				SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_PERSONAL_AREA_GET_BENEFIT);

			pWnd->SendMessage(LMV_BUTTON_ID_FAMILY_ADVANTAGE, (WPARAM)nPremCode);
		}
		break;
	}
	return TRUE;
}

void  CUserInfoDetailDlg::SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin)
{
	m_pUserInfo = pMyInfo;

	//혜택보기 /혜택 받기 버튼 
	int nPremCode= UPGCheckPremiumState(pMyInfo);
	if (nPremCode == CODE_NEW_FAMILY_NONE )
	{
		m_pBtnMembersPreview->SetFourStatusButton(4);
	}
	else
	{
		m_pBtnMembersPreview->SetFourStatusButton(8);
	}

	CString strText;

	m_pLevel->SetText(GetGradeName(m_pUserInfo->nIcon).GetString());

	if (m_pUserInfo->nAIN_InsuranceKind==0)
		strText.Format("미가입");
	else
		strText.Format("%s보험 (누적보험금 %s%s)" , g_GetInsuName(m_pUserInfo->nAIN_InsuranceKind), NMBASE::UTIL::g_MoneyMark(m_pUserInfo->nAIN_Insurance_Money), strChip_Name[(int)ROOM_CHIPKIND_NORMAL]);

	m_pInsurance->SetText(strText.GetString());

	int nPrem = CheckPremiumState(m_pUserInfo);
	if (nPrem==0)
		strText.Format("미가입");
	else
		strText.Format("%s멤버스" , g_GetPremName(nPrem, FALSE));

	m_pMembers->SetText(strText.GetString());
}

void CUserInfoDetailDlg::ShowRoomList(bool bShow)
{
	m_pBtnInsuranceReceive->SetEnable(bShow);
	m_pBtnMembersPreview->SetEnable(bShow);
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CUserInfoDetailDlg::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	//해당 유저의 기타재화 보험금 가입 정보를 보내준다.
	case SV_ETCCHIP_INSURANCE_USER_INFO:
		{
			CSV_ETCCHIP_INSURANCE_USER_INFO msg;
			msg.Get(lpData, TotSize);
			
			// 패킷이 골드 재화에 대한 정보일 경우
			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				m_nLevel = *msg.m_Level; //0이면 미가입 상태
				m_chArrName = msg.m_Name; //보험명
				m_i64SaveMoney = *msg.m_SaveMoney; //누적 적립된 금액
				
				// 골드 보험 미가입
				if (m_nLevel == 0)
				{
					m_pInsuranceGold1->SetText("미가입"); // 골드 보험 텍스트1
					m_pInsuranceGold2->SetText(""); // 골드 보험 텍스트2
				}
				else
				{
					CString csSaveMoney;

					csSaveMoney.Format("(누적보험금 %I64d%s)", m_i64SaveMoney, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);

					m_pInsuranceGold1->SetText(m_chArrName); // 골드 보험 텍스트1
					m_pInsuranceGold2->SetText(csSaveMoney.GetBuffer(0)); // 골드 보험 텍스트2
				}
			}
		}
		break;
	//해당 유저의 기타재화 보험금 변동 내역을 보내준다.(가입, 지급등)
	case SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO:
		{
			CSV_ETCCHIP_INSURANCE_USER_CHANGE_INFO msg;
			msg.Get(lpData, TotSize);

			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				//int *m_ChangeType; //1.가입, 2.지급
				//INT64 *m_ChangeChip; //변동금액.. 항상 양수임에 주의하자~!!

				//INT64 *m_CurrentChip; //위 처리 이후 보유금액
				m_nLevel = *msg.m_FixedLevel; //위 처리이후 확정된 보험등급
				m_chArrName = msg.m_FixedName; //위 처리이후 확정된 보험이름
				m_i64SaveMoney = *msg.m_FixedSaveChip; //위 처리이후 남아 있는 적립금

				// 골드 보험 미가입
				if (m_nLevel == 0)
				{
					m_pInsuranceGold1->SetText("미가입"); // 골드 보험 텍스트1
					m_pInsuranceGold2->SetText(""); // 골드 보험 텍스트2
				}
				else
				{
					CString csSaveMoney;
					
					csSaveMoney.Format("(누적 보험금 %I64d%s)", m_i64SaveMoney, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					
					m_pInsuranceGold1->SetText(m_chArrName); // 골드 보험 텍스트1
					m_pInsuranceGold2->SetText(csSaveMoney.GetBuffer(0)); // 골드 보험 텍스트2
				}
			}
		}
		break;
	}
}
