#include "StdAfx.h"
#include "UserInfoDetailDlg.h"
#include "../../WidgetID.h"

CUserInfoDetailDlg::CUserInfoDetailDlg()
:	m_pBtnDetailClose(NULL), // �ɹ��� �󼼺��� �ݱ� ��ư
	m_pLevel(NULL), // ��� �ؽ�Ʈ
	m_pInsurance(NULL), // ���� �ؽ�Ʈ
	m_pMembers(NULL), // �ɹ��� �ؽ�Ʈ
	m_pInsuranceGold1(NULL), // ��� ���� �ؽ�Ʈ1
	m_pInsuranceGold2(NULL), // ��� ���� �ؽ�Ʈ2
	m_pBtnInsuranceReceive(NULL), // ���� ���޹ޱ� ��ư
	m_pBtnMembersPreview(NULL), // �ɹ��� ���ú��� ��ư
	m_pBtnInsuranceGoldReceive(NULL), // ��� ���� ���޹ޱ� ��ư
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
	// �ɹ��� �󼼺��� �ݱ� ��ư
	case ID_LOBBY_BTN_MYINFO_DETAIL_CLOSE:
		m_pBtnDetailClose = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	// ��� �ؽ�Ʈ
	case ID_LOBBY_TXT_MYINFO_DETAIL_LEVEL:
		{
			m_pLevel = (LSY::CText *)pObject;
			m_pLevel->SetText(GetGradeName(0).GetString());
		}
		break;
	// ���� �ؽ�Ʈ
	case ID_LOBBY_TXT_MYINFO_DETAIL_INSURANCE:
		{
			m_pInsurance = (LSY::CText *)pObject;
			m_pInsurance->SetText("");
		}
		break;
	// �ɹ��� �ؽ�Ʈ
	case ID_LOBBY_TXT_MYINFO_DETAIL_MEMBERS:
		{
			m_pMembers = (LSY::CText *)pObject;
			m_pMembers->SetText("");
		}
		break;
	// ��� ���� �ؽ�Ʈ1
	case ID_LOBBY_TXT_MYINFO_DETAIL_INSURANCE_GOLD1:
		{
			m_pInsuranceGold1 = (LSY::CText *)pObject;
		}
		break;
	// ��� ���� �ؽ�Ʈ2
	case ID_LOBBY_TXT_MYINFO_DETAIL_INSURANCE_GOLD2:
		{
			m_pInsuranceGold2 = (LSY::CText *)pObject;
		}
		break;
	// ���� ���޹ޱ� ��ư
	case ID_LOBBY_BTN_MYINFO_INSURANCE_RECEIVE:
		{
			m_pBtnInsuranceReceive = (LSY::CButton *)pObject;
			bAddChild = true;
		}
		break;
	// �ɹ��� ���ú��� ��ư
	case ID_LOBBY_BTN_MYINFO_MEMBERS_PREVIEW:
		{
			m_pBtnMembersPreview = (LSY::CButton *)pObject;
			bAddChild = true;
		}
		break;
	// ��� ���� ���޹ޱ� ��ư
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
		// ��� ���� ���޹ޱ� ��ư
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

	//���ú��� /���� �ޱ� ��ư 
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
		strText.Format("�̰���");
	else
		strText.Format("%s���� (��������� %s%s)" , g_GetInsuName(m_pUserInfo->nAIN_InsuranceKind), NMBASE::UTIL::g_MoneyMark(m_pUserInfo->nAIN_Insurance_Money), strChip_Name[(int)ROOM_CHIPKIND_NORMAL]);

	m_pInsurance->SetText(strText.GetString());

	int nPrem = CheckPremiumState(m_pUserInfo);
	if (nPrem==0)
		strText.Format("�̰���");
	else
		strText.Format("%s�����" , g_GetPremName(nPrem, FALSE));

	m_pMembers->SetText(strText.GetString());
}

void CUserInfoDetailDlg::ShowRoomList(bool bShow)
{
	m_pBtnInsuranceReceive->SetEnable(bShow);
	m_pBtnMembersPreview->SetEnable(bShow);
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CUserInfoDetailDlg::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	//�ش� ������ ��Ÿ��ȭ ����� ���� ������ �����ش�.
	case SV_ETCCHIP_INSURANCE_USER_INFO:
		{
			CSV_ETCCHIP_INSURANCE_USER_INFO msg;
			msg.Get(lpData, TotSize);
			
			// ��Ŷ�� ��� ��ȭ�� ���� ������ ���
			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				m_nLevel = *msg.m_Level; //0�̸� �̰��� ����
				m_chArrName = msg.m_Name; //�����
				m_i64SaveMoney = *msg.m_SaveMoney; //���� ������ �ݾ�
				
				// ��� ���� �̰���
				if (m_nLevel == 0)
				{
					m_pInsuranceGold1->SetText("�̰���"); // ��� ���� �ؽ�Ʈ1
					m_pInsuranceGold2->SetText(""); // ��� ���� �ؽ�Ʈ2
				}
				else
				{
					CString csSaveMoney;

					csSaveMoney.Format("(��������� %I64d%s)", m_i64SaveMoney, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);

					m_pInsuranceGold1->SetText(m_chArrName); // ��� ���� �ؽ�Ʈ1
					m_pInsuranceGold2->SetText(csSaveMoney.GetBuffer(0)); // ��� ���� �ؽ�Ʈ2
				}
			}
		}
		break;
	//�ش� ������ ��Ÿ��ȭ ����� ���� ������ �����ش�.(����, ���޵�)
	case SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO:
		{
			CSV_ETCCHIP_INSURANCE_USER_CHANGE_INFO msg;
			msg.Get(lpData, TotSize);

			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				//int *m_ChangeType; //1.����, 2.����
				//INT64 *m_ChangeChip; //�����ݾ�.. �׻� ����ӿ� ��������~!!

				//INT64 *m_CurrentChip; //�� ó�� ���� �����ݾ�
				m_nLevel = *msg.m_FixedLevel; //�� ó������ Ȯ���� ������
				m_chArrName = msg.m_FixedName; //�� ó������ Ȯ���� �����̸�
				m_i64SaveMoney = *msg.m_FixedSaveChip; //�� ó������ ���� �ִ� ������

				// ��� ���� �̰���
				if (m_nLevel == 0)
				{
					m_pInsuranceGold1->SetText("�̰���"); // ��� ���� �ؽ�Ʈ1
					m_pInsuranceGold2->SetText(""); // ��� ���� �ؽ�Ʈ2
				}
				else
				{
					CString csSaveMoney;
					
					csSaveMoney.Format("(���� ����� %I64d%s)", m_i64SaveMoney, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					
					m_pInsuranceGold1->SetText(m_chArrName); // ��� ���� �ؽ�Ʈ1
					m_pInsuranceGold2->SetText(csSaveMoney.GetBuffer(0)); // ��� ���� �ؽ�Ʈ2
				}
			}
		}
		break;
	}
}
