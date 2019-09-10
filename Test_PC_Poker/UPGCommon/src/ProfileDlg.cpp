// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProfileDlg.h"
#include "GlobalBase.h"
#include "profiledlgid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProfileDlg::CProfileDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
	ZeroMemory(&m_cUserInfo, sizeof(USERINFO));	
	m_uButtonFlag = 0;
	m_bFirst = true;
}

BEGIN_MESSAGE_MAP(CProfileDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

void CProfileDlg::CheckButtonFlag(void)
{
	m_uButtonFlag = 0;
	
	USERINFO *pInfo  = &m_cUserInfo;
	CPlayer *pMyPlayer = GM().GetMyInfo();
	
	//*** �������� ���¸� üũ�Ѵ�.
	bool bOffline = (pInfo->nGameIndex == -1)?true:false;
	bool bInGameRoom = bOffline?false:(pInfo->nRoomNum == 0)?false:true;
	bool bIsVIPRoom = false;
	bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;
	
	if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
	{
		//�� �������� �ƴҶ��� ��ư�� Ȱ��ȭ �Ѵ�.
		if(!bMyProfile)
		{
			int nPnum = -1;
			int nMyPnum = -1;
			CGameProcess *pGameProcess = GM().GetCurrentProcess();
			if (pGameProcess)
			{
				nPnum = pGameProcess->GetPlayerPNum(pInfo->ID);
				nMyPnum = pGameProcess->GetPlayerPNum(pMyPlayer->UI.ID);
			}

			//���� ���ӹ� �Ȱ� ���� �����̸� ���� �߹��� Ȱ��ȭ �Ѵ�.
			if((nPnum >= 0) && (strcmp(g_RI.ID, pMyPlayer->UI.ID) == 0)) //���� ��ư
				m_uButtonFlag |= UPG_PROFILE_FLAG_BANISHMENT;


			//���� ���ӹ濡�� ���� �����ڴ� ��� ��ư ��Ȱ��
			if(nPnum >= 0 && nPnum < pGameProcess->GetMaxPlayer())
			{
			}
			/* 2018.04.30 ������ ���� : �Ӹ��� ������ ��Ȱ��ȭ
			//������ �Ǵ� ���� ����ڶ�� �Ӹ� Ȱ��ȭ
			else if(nPnum >= pGameProcess->GetMaxPlayer() || pInfo->nRoomNum == 0)
			{
				m_uButtonFlag |= UPG_PROFILE_FLAG_WISPHER;
			}
			*/
		}

		if(bIsVIPRoom) m_uButtonFlag |= UPG_PROFILE_FLAG_VIP;		
	}
	else if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
	{
		//���� �ƴҶ��� ��ư �� VIP�� Ȱ��ȭ �Ѵ�.
		if(!bMyProfile)
		{
			//�����϶� ������ �׻� Ȱ��ȭ
			m_uButtonFlag |= UPG_PROFILE_FLAG_MESSEAGE;
			if(bOffline)
			{
			}
			//�������϶� ä��/�Ӹ� ������ Ȱ��ȭ ����
			else
			{
				//�������� �ƴҶ� ä�ù�ư Ȱ��ȭ
				if(bInGameRoom)
				{
					SMALLROOMINFO* pSRI = GM().FindDataRoomList(m_cUserInfo.nRoomNum);

					if ( GM().IsJoinViewRoom(pSRI) )
						m_uButtonFlag |= UPG_PROFILE_FLAG_SAMEROOM;
				}
				
				/* 2018.04.30 ������ ���� : �Ӹ��� ������ ��Ȱ��ȭ
				m_uButtonFlag |= UPG_PROFILE_FLAG_WISPHER;
				*/
				if (bIsVIPRoom) m_uButtonFlag |= UPG_PROFILE_FLAG_VIP;
			}
		}
	}
	
	// �ΰ��� �κ� ������� �� �������� �ƴҶ� ��ư Ȱ��ȭ
	if(!bMyProfile)
	{
		bool bBuddy = CCommunityManager::Instance()->IsExistBuddy(pInfo->NickName)?true:false;
		bool bBlackBuddy = CCommunityManager::Instance()->IsExistBlackBuddy(pInfo->NickName)?true:false;
		
		if(!bBuddy) m_uButtonFlag |= UPG_PROFILE_FLAG_ADD_BUDDY;
		if(!bBlackBuddy) m_uButtonFlag |= UPG_PROFILE_FLAG_ADD_BLACKLIST;
		
		// Ŭ�� ��� ����
		int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
		int nUserGrade = m_cUserInfo.ClubMemberGrade;
		// ���� ������ ��밡 �̰��� ���¸� Ŭ���ʴ� ��ư Ȱ��ȭ
		if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
			nUserGrade == CLUB_MEMBER_GRADE_NONE)
			m_uButtonFlag |= UPG_PROFILE_FLAG_INVITE_CLUB;
		// ���� Ŭ�� �̰��� �����̰� ��밡 �����϶� Ŭ�� ���Կ�û ��ư Ȱ��ȭ
		else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
			nUserGrade == CLUB_MEMBER_GRADE_BOSS)
			m_uButtonFlag |= UPG_PROFILE_FLAG_INVITE_CLUB;

		// 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: , 4: ���տ�(ī���� ����) 5: ������ 
		// ī�� ��� ����
		nMyGrade = GM().GetMyInfo()->UI.cafeMemberGrade_;
		nUserGrade = m_cUserInfo.cafeMemberGrade_;

		if(nUserGrade == 0 && nMyGrade >= 2)
		{
			m_uButtonFlag |= UPG_PROFILE_FLAG_INVITE_CAFE;
		}
		else if(nUserGrade >= 3 && nMyGrade == 0)
		{
			m_uButtonFlag |= UPG_PROFILE_FLAG_ASK_CAFE;
		}
		else if(nUserGrade == 3 && nMyGrade == 5)
		{
			m_uButtonFlag |= UPG_PROFILE_FLAG_INVITE_UNION;
		}
		else if(nUserGrade == 5 && nMyGrade == 3)
		{
			m_uButtonFlag |= UPG_PROFILE_FLAG_ASK_UNION;
		}
	}
}

void CProfileDlg::Init(USERINFO *pUI)
{
	m_pParentWnd = GM().GetMainWnd();
	memcpy(&m_cUserInfo, pUI, sizeof(USERINFO));
	m_bFirst = true;
}

bool CProfileDlg::OnCreatedProject(void)
{
	CheckButtonFlag();

	USERINFO *pInfo  = &m_cUserInfo;
	CPlayer *pMyPlayer = GM().GetMyInfo();
	bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

	LSY::CAvatar *pAvartar = (LSY::CAvatar *)GetObject(ID_USERPROFILE_AVATAR);
	pAvartar->SetAvatar(m_cUserInfo.AvatarInfo);

	LSY::CText *pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_NICKNAME);
	pText->SetText(m_cUserInfo.NickName);

	CString strTemp;
	strTemp.Format("[%s]", m_cUserInfo.ID);
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_ID);
	pText->SetText(strTemp.GetString());
	pText->SetShow(false);

	// ������ �̽�.
	LSY::CButton *pButton = NULL;
//#if defined(_DEBUG)
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_SAMEROOM_ENTER);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_SAMEROOM?true:false);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_SAMEROOM?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	if(bMyProfile)
	{
		pButton->SetShow(false);
	}

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_TOGETHER_PLAYGAME);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_TOGETHER?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
//#endif

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_SEND_MESSEGE);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_MESSEAGE?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	/* �Ӹ� ����
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_SEND_WHISPER);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_WISPHER?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
	*/

	/* Ŭ�� ���� 2018.6
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_INVITE_CLUB);
	// ���� ������ �ƴҰ�� Ŭ�� ���Կ�û ��ư���� ����
	int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
	if (nMyGrade != CLUB_MEMBER_GRADE_BOSS)
		pButton->SetFourStatusButton(45);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_INVITE_CLUB?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
	*/
	// Ŭ�� ������ ��ư ���� 2018.6
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_INVITE_CLUB);
	pButton->SetShow(false);

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_INVITE_CAFE);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_INVITE_CAFE?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_ASK_CAFE);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_ASK_CAFE?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_INVITE_UNION);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_INVITE_UNION?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_ASK_UNION);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_ASK_UNION?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
	
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_ADD_BUDDY);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_ADD_BUDDY?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_ADD_BLACK_BUDDY);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_ADD_BLACKLIST?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	LSY::CTextNumber *pTextNumber = (LSY::CTextNumber *)GetObject(ID_USERPROFILE_TEXTNUMBER);
	//pTextNumber->SetNumber(m_cUserInfo.GetMoney(IsTournament()));
	/*pTextNumber->SetNumber(m_cUserInfo.GetMoney());*/
	pTextNumber->SetNumber(m_cUserInfo.GetRealPMoney());

	int level = GetMoneyGrade(m_cUserInfo.GetTotalMoney());

	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_LEVEL);
	pText->SetText(GetGradeName(level).GetString());

	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_SEX);
	pText->SetText(m_cUserInfo.Sex==0?"��":"��");

	/* ī�� �߰��� ���� ����
	if (m_cUserInfo.nAge<0)	
		strTemp.Format("�����");	
	else
		strTemp.Format("%d��", m_cUserInfo.nAge);	
	*/
	strTemp.Format("");

	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_AGE);
	pText->SetText(strTemp.GetString());
	
	// ���� Ŭ��
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_JOIN_CLUB);
	std::string sClubName = m_cUserInfo.ClubName;
	int nUserGrade = m_cUserInfo.ClubMemberGrade;
	if (nUserGrade == CLUB_MEMBER_GRADE_BOSS)
		sClubName += "(����)";
	pText->SetText(sClubName);

	// ���� ī��
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_JOIN_CAFE);
	std::string sCafeName = "����";
	if (m_cUserInfo.cafeInfo_.cafeID_ > 0)
	{
		sCafeName = m_cUserInfo.cafeInfo_.cafeName_;
	}
	pText->SetText(sCafeName);

	// ���� ����
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_JOIN_UNION);
	std::string sUnionName = "����";
	if (m_cUserInfo.unitedCafeInfo_.cafeID_ > 0)
	{
		sUnionName = m_cUserInfo.unitedCafeInfo_.cafeName_;
	}
	pText->SetText(sUnionName);
	
	LSY::CText *pRoomNumber = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_ROOM_NUMBER);
	
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_GAME);
	CGame *pGame = NULL;
	if(m_cUserInfo.nGameIndex < 0 || m_cUserInfo.nGameIndex >= MAX_GAME)
	{
		strTemp = "��������";
		pRoomNumber->SetShow(false);
	}
	else
	{
		pGame = GM().GetGame((IDX_GAME)m_cUserInfo.nGameIndex);
		if(pGame)
		{
			pText->SetTextColor(pGame->GetGameColor());
			strTemp = pGame->GetGameName();
		}
		else
		{
			strTemp = "��������";
			pRoomNumber->SetShow(false);
		}
	}
	pText->SetText(strTemp.GetString());

	if(pRoomNumber->IsShow())
	{
		if(m_cUserInfo.nRoomNum == 0)
		{
			strTemp = "����";
		}
		else
		{
			strTemp.Format("[%d����]", m_cUserInfo.nRoomNum);
		}

		pRoomNumber->SetText(strTemp.GetString());
	}

	LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_USERPROFILE_IMG_VIP);
	pImage->SetShow(m_uButtonFlag&UPG_PROFILE_FLAG_VIP?true:false);

	LSY::CCombo *pCombo = (LSY::CCombo *)GetObject(ID_USERPROFILE_COMBO_GAME_SCORE);
	if(m_bFirst)
	{
		int nIndex = 0;
		for(int nGame = (int)IDX_GAME_NULL+1;nGame < (int)MAX_GAME;++nGame)
		{
			pGame = GM().GetGame((IDX_GAME)nGame);
			if(pGame)
			{
				nIndex = pCombo->AddText(pGame->GetGameName().GetString());
				pCombo->SetItemData(nIndex, nGame);
				pCombo->SetItemColor(nIndex, pGame->GetGameColor());
			}
		}

		pCombo->SetDefaultFindType(true);
		
		nIndex = pCombo->FirstFind(GM().GetCurrentGameType());
		
		pCombo->SetCurSelectItem(nIndex);
		pCombo->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CProfileDlg::OnSelChange));
	}
	int nIndex = pCombo->GetCurSelectItem();
	IDX_GAME nGame = (IDX_GAME)pCombo->GetItemData(nIndex);

	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_SELF_INTRODUCE);
	pText->SetText(m_cUserInfo.Desc);

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_BANISHMENT);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_BANISHMENT?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_CLOSE1);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_BANISHMENT?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_CLOSE2);
	pButton->SetShow(m_uButtonFlag & UPG_PROFILE_FLAG_BANISHMENT?false:true);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
	
	LSY::CTabControl *pTab = (LSY::CTabControl *)GetObject(ID_USERPROFILE_TAB_SCORE);
	if(m_bFirst)
	{
		pTab->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CProfileDlg::OnTabScore));
		
		// ó�� ������â ������ ���� ȭ������� ���õ� ���� �ǿ� ���� Ȱ��ȭ �Ǵ� ���� �ٸ���.
		int nRoomTabIndex = GM().GetRoomListTabIndex();
		if( nGame == IDX_GAME_HL || nGame == IDX_GAME_HA )
		{
			pTab->SetCurSelFromIndex( 1 );
		}
		else
		{
			// ���� ������̰ų� ���õ� �� ȭ�� ������ �������� ���� ǥ��
			if( g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD || nRoomTabIndex == 2 || nRoomTabIndex == 3 )
				pTab->SetCurSelFromIndex( 0 );
			else										   
				pTab->SetCurSelFromIndex( 1 );
		}
	}
	
	// ���� ������ �������� ���� ���� ����( �� ���� ���� ���� ��ġ ���� ���� )
	SetGameScore(nGame);

	m_bFirst = false;

	return true;
}

void CProfileDlg::SetGameScore(IDX_GAME nGame)
{
	LSY::CText *pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_GAME_SCORE);
	if(nGame == IDX_GAME_NULL)
	{
		pText->SetShow(false);
		return;
	}
	pText->SetShow(true);
	
	CString str;
	// ���� �� ����
	LSY::CTabControl *pTab = (LSY::CTabControl *)GetObject(ID_USERPROFILE_TAB_SCORE);

	if( nGame == IDX_GAME_HL || nGame == IDX_GAME_HA )
	{
		LSY::CRadio *pRadio = (LSY::CRadio*) pTab->GetAttachRadioList().front();
		if( pRadio->GetID() == ID_USERPROFILE_RDO_TABSCORE_GOLD ) pRadio->SetEnable( FALSE );	

		pTab->SetCurSelFromIndex( 1 );
	}
	else
	{
		LSY::CRadio *pRadio = (LSY::CRadio*) pTab->GetAttachRadioList().front();
		if( pRadio->GetID() == ID_USERPROFILE_RDO_TABSCORE_GOLD ) pRadio->SetEnable( TRUE );
	}
		
	// Ȱ��ȭ�� �ǿ� ���� ���� ����
	if( pTab->GetCurSelIndex() == 0 )
	{
		str.Format("%d�� %d�� %d��(%d%%)", 
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetTotalNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetWinNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetLoseNum(nGame)+m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetFoldNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetWinPro(nGame));
	}
	else
	{
		str.Format("%d�� %d�� %d��(%d%%)", 
			m_cUserInfo.AllGameRecord[ PlayableChipType::NORMAL ].GetTotalNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::NORMAL ].GetWinNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::NORMAL ].GetLoseNum(nGame)+m_cUserInfo.AllGameRecord[ PlayableChipType::NORMAL ].GetFoldNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::NORMAL ].GetWinPro(nGame));
	}

	pText->SetText(str.GetString());
}

LRESULT CProfileDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	int nObjID = msg->GetObject()->GetID();
	switch(nObjID)
	{
	//���� ��ư
	case ID_USERPROFILE_BTN_BANISHMENT:
		{
			if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
			{
				OnOK();
				break;
			}
			if (true==GM().IsNotBanishment())
			{
				CMsgBoxDlg dlg(this);
				if(dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "������ ��⸦ �����ϱ� ���Ͽ�\n���̸�ġ ���� ������ �����߹��� �Ұ��մϴ�.\n������ ���� ��Ź�帳�ϴ�.") == IDOK)
				{
					OnOK();				
				}
				//m_uButtonFlag=((m_uButtonFlag)^((m_uButtonFlag)&(UPG_PROFILE_FLAG_BANISHMENT)));
			}
			else
			{
				CMsgBoxDlg dlg(this);
				if(dlg.DoModal(CMsgBoxDlg::EMBT_QUESTION, "���� �߹� �Ͻðڽ��ϱ�?") == IDOK)
				{
					BanishKickOut(m_cUserInfo.ID);
					OnOK();				
				}
			}
		}
		break;
	case ID_USERPROFILE_BTN_SEND_MESSEGE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
				{
					USERINFO *pInfo  = &m_cUserInfo;
					CPlayer *pMyPlayer = GM().GetMyInfo();
					bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

					if (bMyProfile == false)
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_SEND_MEMO);
				}
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			OnOK();
			//if(CCommunityManager::Instance()->IsConnect())
			{
				ShowSendMessageDialog( m_cUserInfo.NickName );
			}
// 			else
// 			{
// 				ShowMainMessageDlg("Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ������ ���� �� �����ϴ�.");
// 			}
		}
		break;
		/* �Ӹ� ����
	case ID_USERPROFILE_BTN_SEND_WHISPER:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
				{
					USERINFO *pInfo  = &m_cUserInfo;
					CPlayer *pMyPlayer = GM().GetMyInfo();
					bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

					if (bMyProfile == false)
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_SEND_WHISPER);
				}
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			SMALLUSERINFO userinfo;
			strcpy(userinfo.NickName, m_cUserInfo.NickName);
			strcpy(userinfo.ID, m_cUserInfo.ID);
			userinfo.llUserKey = m_cUserInfo.llUserKey;
			ShowWhisperDialog(&userinfo, this);
		}
		break;
		*/
	case ID_USERPROFILE_BTN_SAMEROOM_ENTER:		//���� �� ����
		SendUILog_Basic(CLSTATS_UINO_POKER_PROFILE_JOIN_FRIENDROOM);
	case ID_USERPROFILE_BTN_TOGETHER_PLAYGAME:  //���� �����ϱ�
		if (GM().IsGetLimitMoney(GM().GetCurrentGameArray()) && !GM().GetEnableOver())
		{
			OnOK();
			MM().Call(UPG_CMK_SHOW_RESTRICTION_DIALOG);
		}
		else
		{
			if(MM().Call(UPG_CMK_LOBBY_PLAY_TOGETHER, (WPARAM)(&m_cUserInfo)))
				OnOK();
		}
		break;
	case ID_USERPROFILE_BTN_CLOSE1:
	case ID_USERPROFILE_BTN_CLOSE2:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (GM().GetProfileOpenKind() == EPOK_LOBBY_MY_AVATAR)
				{
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_CLOSE);
				}
				else if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
				{
					USERINFO *pInfo  = &m_cUserInfo;
					CPlayer *pMyPlayer = GM().GetMyInfo();
					bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

					if (bMyProfile)
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_CLOSE);
					else
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_CLOSE);
				}
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			OnCancel();
		}
		break;
	// ä���ϱ� -> �ʴ�/���Կ�û ���� ����
	case ID_USERPROFILE_BTN_INVITE_CLUB:
		{
			// Ȥ�� ���߿� ä���ϱ� ���� �۾��� ���� �� �־� �ּ��� ���ܵ�..
			//#ifdef UPGRADE_10210619_ADD_CLICKLOG
			//	if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
			//	{
			//		USERINFO *pInfo  = &m_cUserInfo;
			//		CPlayer *pMyPlayer = GM().GetMyInfo();
			//		bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;


			//		if (bMyProfile == false)
			//			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_TALK);
			//	}
			//#endif //UPGRADE_10210619_ADD_CLICKLOG
			
			//OnOK();
			//std::list<std::string> listInviteID;
			//listInviteID.push_back(m_cUserInfo.ID);
			//SendCreateChatting(&listInviteID, "Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ��ȭâ�� ���� �����ϴ�.");
			
			int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
			int nUserGrade = m_cUserInfo.ClubMemberGrade;
			// �ʴ�(���� ������ ��밡 �̰��� ����)
			if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
				nUserGrade == CLUB_MEMBER_GRADE_NONE)
			{
				CCL_CLUB_INVITE msg;
				
				msg.Set(m_cUserInfo.ID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
			// ���Կ�û(���� Ŭ�� �̰��� �����̰� ��밡 �����϶�)
			else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
				nUserGrade == CLUB_MEMBER_GRADE_BOSS)
			{
				CCL_CLUB_JOIN msg;
				
				msg.Set(m_cUserInfo.ClubCode);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
		}
		break;
	case ID_USERPROFILE_BTN_INVITE_CAFE:
		{
			CCL_CAFE_INVITE msg;
			msg.Set(CafeSystem::CafeKind::CAFE, GM().GetMyInfo()->UI.cafeInfo_.cafeID_, m_cUserInfo.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_USERPROFILE_BTN_ASK_CAFE:
		{
			CCL_CAFE_ASK_JOIN msg;
			msg.Set(CafeSystem::CafeKind::CAFE, m_cUserInfo.cafeInfo_.cafeID_);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_USERPROFILE_BTN_INVITE_UNION:
		{
			CCL_CAFE_INVITE msg;
			msg.Set(CafeSystem::CafeKind::UNITED_CAFE, GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_, m_cUserInfo.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_USERPROFILE_BTN_ASK_UNION:
		{
			CCL_CAFE_ASK_JOIN msg;
			msg.Set(CafeSystem::CafeKind::UNITED_CAFE, m_cUserInfo.unitedCafeInfo_.cafeID_);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_USERPROFILE_BTN_ADD_BUDDY:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
				{
					USERINFO *pInfo  = &m_cUserInfo;
					CPlayer *pMyPlayer = GM().GetMyInfo();
					bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

					if (bMyProfile == false)
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_ADD_FRIEND);
				}
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBuddyAddDlg dlg;

			if(dlg.DoModal(m_cUserInfo.NickName, this) == IDCANCEL)
			{
				break;
			}

			if(!CCommunityManager::Instance()->IsConnect())
			{
				LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_ADD_BUDDY);
				pButton->SetEnable(false);

				CMsgBoxDlg dlg(this);
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ��ģ�� ��Ͽ� �߰��� �� �����ϴ�.");
			}
		}
		break;
	case ID_USERPROFILE_BTN_ADD_BLACK_BUDDY:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
				{
					USERINFO *pInfo  = &m_cUserInfo;
					CPlayer *pMyPlayer = GM().GetMyInfo();
					bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

					if (bMyProfile == false)
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_ADD_BLACKLIST);
				}
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBlackBuddyAddDlg dlg;

			if(dlg.DoModal(CBlackBuddyAddDlg::BBADT_QUESTION, "", this) == IDCANCEL)
			{
				break;
			}

			if(CCommunityManager::Instance()->IsConnect())
			{
				LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_ADD_BLACK_BUDDY);
				pButton->SetEnable(false);

				CCommunityManager::Instance()->SendAddBlackBuddy(m_cUserInfo.ID);
			}
			else
			{
				CMsgBoxDlg dlg(this);
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ������Ʈ ��Ͽ� �߰��� �� �����ϴ�.");
			}
		}
		break;
	}
	return TRUE;
}

LRESULT CProfileDlg::OnSelChange(LSY::CMessage *pMsg)
{
	LSY::CMO_SelChange *msg = (LSY::CMO_SelChange *)pMsg;
	const int &nIndex = msg->GetValue();
	LSY::CTextList *pTextList = (LSY::CTextList *)msg->GetObject();
	IDX_GAME nGame = (IDX_GAME)pTextList->GetItemData(nIndex);
	SetGameScore(nGame);

	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		if (GM().GetProfileOpenKind() == EPOK_LOBBY_MY_AVATAR)
		{
			if (nGame == IDX_GAME_SP)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_GAMERECORD_7POKER);
			else if (nGame == IDX_GAME_BD)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_GAMERECORD_LOWBD);
			else if (nGame == IDX_GAME_NP)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_GAMERECORD_NEWPOKER);
			else if (nGame == IDX_GAME_HL)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_GAMERECORD_HIGHLOW);
			else if (nGame == IDX_GAME_SD)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_GAMERECORD_SUTDA);
			else if (nGame == IDX_GAME_HA)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR_GAMERECORD_HOOLA);
		}
		else if (GM().GetProfileOpenKind() == EPOK_ROOM_CLICK_AVATAR)
		{
			USERINFO *pInfo  = &m_cUserInfo;
			CPlayer *pMyPlayer = GM().GetMyInfo();
			bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;

			if (bMyProfile)
			{
				if (nGame == IDX_GAME_SP)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_GAMERECORD_7POKER);
				else if (nGame == IDX_GAME_BD)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_GAMERECORD_LOWBD);
				else if (nGame == IDX_GAME_NP)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_GAMERECORD_NEWPOKER);
				else if (nGame == IDX_GAME_HL)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_GAMERECORD_HIGHLOW);
				else if (nGame == IDX_GAME_SD)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_GAMERECORD_SUTDA);
				else if (nGame == IDX_GAME_HA)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE_GAMERECORD_HOOLA);
			}
			else
			{
				if (nGame == IDX_GAME_SP)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_GAMERECORD_7POKER);
				else if (nGame == IDX_GAME_BD)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_GAMERECORD_LOWBD);
				else if (nGame == IDX_GAME_NP)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_GAMERECORD_NEWPOKER);
				else if (nGame == IDX_GAME_HL)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_GAMERECORD_HIGHLOW);
				else if (nGame == IDX_GAME_SD)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_GAMERECORD_SUTDA);
				else if (nGame == IDX_GAME_HA)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE_GAMERECORD_HOOLA);
			}
		}
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}	
LRESULT CProfileDlg::OnTabScore(LSY::CMessage *pMsg)
{
 	LSY::CCombo *pCombo = (LSY::CCombo *)GetObject(ID_USERPROFILE_COMBO_GAME_SCORE);
	int nIndex = pCombo->GetCurSelectItem();

	LSY::CTextList *pTextList = (LSY::CTextList *)pCombo->GetList();
	IDX_GAME nGame = (IDX_GAME)pTextList->GetItemData(nIndex);

	SetGameScore(nGame);

	return TRUE;
}

void CProfileDlg::OnChangeUser(SMALLUSERINFO *pSUI)
{
	if (GetSafeHwnd()==NULL) 
		return;

	if (strcmp(pSUI->ID, m_cUserInfo.ID)==0)
	{
		m_cUserInfo.llUserKey = pSUI->llUserKey;
		m_cUserInfo.nGameIndex = pSUI->nGameIndex;
		m_cUserInfo.SetMoney(pSUI->PMoney);
		m_cUserInfo.cPosition = pSUI->Position;
		m_cUserInfo.nRoomNum = pSUI->RoomNum;
		OnCreatedProject();
	}
}


void CProfileDlg::HandleResponse(int Signal, int TotSize, char *lpData)
{
	CString str;
	switch(Signal)
	{
	case SV_CAFE_ASK_JOIN:
		{
			CSV_CAFE_ASK_JOIN msg;
			msg.Get(lpData, TotSize);
			if (*msg.joinResult_ == CafeSystem::JoinResult::SUCCESS)// ���� ��û �Ϸ�
			{
				str.Format( "���� ��û�� �Ϸ� �Ǿ����ϴ�." );
			}
			else if(*msg.joinResult_ == CafeSystem::JoinResult::ALREADY_MEMBER)// �̹� �����. (���Ե� ����)
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::CAFE)
				{
					str.Format( "�̹� ī�信 ���Ե� ȸ���Դϴ�." );
				}
				else if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "�̹� ���տ� ���Ե� ȸ���Դϴ�." );
				}
			}
			else if(*msg.joinResult_ == CafeSystem::JoinResult::EXCESS_DAY_LIMIT_COUNT)// 1�� ���Կ�û �Ǽ� ���� �ʰ�.
			{
				str.Format( "1�� 10ȸ������ ���� ��û�� �����մϴ�." );
			}			
			else if(*msg.joinResult_ == CafeSystem::JoinResult::ALREADY_ASKED)// �̹� ���� ��û�� �� ����.
			{
				str.Format( "�̹� ������ ��û�� �����Դϴ�." );
			}
			else if(*msg.joinResult_ == CafeSystem::JoinResult::FULL_MEMBER_COUNT)// �̹� ����� �� á��.
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::CAFE)
				{
					str.Format( "ī�� ��ȸ�� ���� �ʰ��� �� �̻� ���� ��û�� �� �����ϴ�." );
				}
				else if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "ī�� ���� �� �ʰ��� �� �̻� ���� ��û�� �� �����ϴ�." );
				}
			}	
			else
			{
				break;
			}
			ShowMainMessageDlg(str);
		}
		break;
	case SV_CAFE_INVITE:
		{
			CSV_CAFE_INVITE msg;
			msg.Get(lpData, TotSize);
			if (*msg.inviteResult_ == CafeSystem::InviteResult::SUCCESS) // �ʴ� �Ϸ�
			{
				str.Format( "���������� �ʴ� �Ϸ��Ͽ����ϴ�." );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::NOT_EXIST_INVITE_MEMBER) // �ʴ��� ����� ����.
			{
				str.Format( "�ش� ����ڰ� �������� �ʽ��ϴ�." );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::EXCESS_DAY_LIMIT_COUNT) // 1�� �ʴ� �Ǽ� ���� �ʰ�.
			{
				if(GM().GetMyInfo()->UI.cafeMemberGrade_ >= CafeSystem::MemberGrade::CAFE_BOSS)
				{
					str.Format( "1�� �ʴ� 10�� ������ �ʰ��Ͽ����ϴ�. ���� 00�� ���� �ʴ밡 �����մϴ�." );
				}
				else
				{
					str.Format( "1�� �ʴ� 5�� ������ �ʰ��Ͽ����ϴ�. ���� 00�� ���� �ʴ밡 �����մϴ�." );
				}
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::YOURE_NOT_CAFE_BOSS) // ī������ �ƴ�.
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "�����ʴ�� ī���忡�Ը� �����մϴ�." );
				}
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::ALREADY_MEMBER) // �̹� �����. (���Ե� ����)
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::CAFE)
				{
					str.Format( "�ش� ����ڰ� ī�信 ���ԵǾ� �ֽ��ϴ�." );
				}
				else if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "�ش� ����ڰ� ���տ� ���ԵǾ� �ֽ��ϴ�." );
				}
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::FULL_MEMBER_COUNT) // �̹� ����� �� á��.
			{
				str.Format( "ī����� �� �ִ� �����Դϴ�. �ʴ븦 �ϱ� ���ؼ��� ī����� Ż���ϰų� VIP����� ����Ǿ�� �մϴ�" );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::NOW_UPDATING_VIP_GRADE) // ���� VIP ��� �������� ���.
			{
				str.Format( "VIP ��� ���� �߿��� ī����� �ʴ��� �� �����ϴ�. ���� �Ϸ� ���� �ٽ� �õ����ֽñ� �ٶ��ϴ�" );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::ALREADY_ASKED) // �̹� �ʴ� ��û�� �� ����.
			{
				str.Format( "�̹� �ʴ븦 �� �����Դϴ�." );
			}
			else
			{
				break;
			}
			ShowMainMessageDlg(str);
		}
		break;
	}
}

INT_PTR CProfileDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_USERPROFILE);
}
