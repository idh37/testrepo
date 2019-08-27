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
	
	//*** 오프라인 상태를 체크한다.
	bool bOffline = (pInfo->nGameIndex == -1)?true:false;
	bool bInGameRoom = bOffline?false:(pInfo->nRoomNum == 0)?false:true;
	bool bIsVIPRoom = false;
	bool bMyProfile = (strcmp(pInfo->ID, pMyPlayer->UI.ID)==0)?true:false;
	
	if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
	{
		//내 프로필이 아닐때만 버튼을 활성화 한다.
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

			//같은 게임방 안고 내가 방장이면 강제 추방을 활성화 한다.
			if((nPnum >= 0) && (strcmp(g_RI.ID, pMyPlayer->UI.ID) == 0)) //강퇴 버튼
				m_uButtonFlag |= UPG_PROFILE_FLAG_BANISHMENT;


			//같은 게임방에의 게임 참여자는 모든 버튼 비활성
			if(nPnum >= 0 && nPnum < pGameProcess->GetMaxPlayer())
			{
			}
			/* 2018.04.30 프로필 정리 : 귓말은 언제나 비활성화
			//관전자 또는 대기실 사용자라면 귓말 활성화
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
		//내가 아닐때만 버튼 및 VIP를 활성화 한다.
		if(!bMyProfile)
		{
			//대기실일때 쪽지는 항상 활성화
			m_uButtonFlag |= UPG_PROFILE_FLAG_MESSEAGE;
			if(bOffline)
			{
			}
			//접속중일때 채팅/귓말 보내기 활성화 결정
			else
			{
				//게임중이 아닐때 채팅버튼 활성화
				if(bInGameRoom)
				{
					SMALLROOMINFO* pSRI = GM().FindDataRoomList(m_cUserInfo.nRoomNum);

					if ( GM().IsJoinViewRoom(pSRI) )
						m_uButtonFlag |= UPG_PROFILE_FLAG_SAMEROOM;
				}
				
				/* 2018.04.30 프로필 정리 : 귓말은 언제나 비활성화
				m_uButtonFlag |= UPG_PROFILE_FLAG_WISPHER;
				*/
				if (bIsVIPRoom) m_uButtonFlag |= UPG_PROFILE_FLAG_VIP;
			}
		}
	}
	
	// 인게임 로비 상관업이 내 프로필이 아닐때 버튼 활성화
	if(!bMyProfile)
	{
		bool bBuddy = CCommunityManager::Instance()->IsExistBuddy(pInfo->NickName)?true:false;
		bool bBlackBuddy = CCommunityManager::Instance()->IsExistBlackBuddy(pInfo->NickName)?true:false;
		
		if(!bBuddy) m_uButtonFlag |= UPG_PROFILE_FLAG_ADD_BUDDY;
		if(!bBlackBuddy) m_uButtonFlag |= UPG_PROFILE_FLAG_ADD_BLACKLIST;
		
		// 클럽 등급 세팅
		int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
		int nUserGrade = m_cUserInfo.ClubMemberGrade;
		// 내가 보스고 상대가 미가입 상태면 클럽초대 버튼 활성화
		if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
			nUserGrade == CLUB_MEMBER_GRADE_NONE)
			m_uButtonFlag |= UPG_PROFILE_FLAG_INVITE_CLUB;
		// 내가 클럽 미가입 상태이고 상대가 보스일때 클럽 가입요청 버튼 활성화
		else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
			nUserGrade == CLUB_MEMBER_GRADE_BOSS)
			m_uButtonFlag |= UPG_PROFILE_FLAG_INVITE_CLUB;

		// 0: 비회원 1: 준회원 2: 정회원 3: , 4: 연합원(카페장 포함) 5: 연합장 
		// 카페 등급 세팅
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

	// 규제안 이슈.
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

	/* 귓말 삭제
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_SEND_WHISPER);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_WISPHER?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
	*/

	/* 클럽 삭제 2018.6
	pButton = (LSY::CButton *)GetObject(ID_USERPROFILE_BTN_INVITE_CLUB);
	// 내가 보스가 아닐경우 클럽 가입요청 버튼으로 변경
	int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
	if (nMyGrade != CLUB_MEMBER_GRADE_BOSS)
		pButton->SetFourStatusButton(45);
	pButton->SetEnable(m_uButtonFlag & UPG_PROFILE_FLAG_INVITE_CLUB?true:false);
	if(m_bFirst) pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CProfileDlg::OnClickButton));
	*/
	// 클럽 삭제로 버튼 숨김 2018.6
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
	pText->SetText(m_cUserInfo.Sex==0?"여":"남");

	/* 카페 추가로 나이 삭제
	if (m_cUserInfo.nAge<0)	
		strTemp.Format("비공개");	
	else
		strTemp.Format("%d세", m_cUserInfo.nAge);	
	*/
	strTemp.Format("");

	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_AGE);
	pText->SetText(strTemp.GetString());
	
	// 가입 클럽
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_JOIN_CLUB);
	std::string sClubName = m_cUserInfo.ClubName;
	int nUserGrade = m_cUserInfo.ClubMemberGrade;
	if (nUserGrade == CLUB_MEMBER_GRADE_BOSS)
		sClubName += "(보스)";
	pText->SetText(sClubName);

	// 가입 카페
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_JOIN_CAFE);
	std::string sCafeName = "없음";
	if (m_cUserInfo.cafeInfo_.cafeID_ > 0)
	{
		sCafeName = m_cUserInfo.cafeInfo_.cafeName_;
	}
	pText->SetText(sCafeName);

	// 가입 연합
	pText = (LSY::CText *)GetObject(ID_USERPROFILE_TXT_JOIN_UNION);
	std::string sUnionName = "없음";
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
		strTemp = "오프라인";
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
			strTemp = "오프라인";
			pRoomNumber->SetShow(false);
		}
	}
	pText->SetText(strTemp.GetString());

	if(pRoomNumber->IsShow())
	{
		if(m_cUserInfo.nRoomNum == 0)
		{
			strTemp = "대기실";
		}
		else
		{
			strTemp.Format("[%d번방]", m_cUserInfo.nRoomNum);
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
		
		// 처음 프로필창 생성시 현재 화폐단위나 선택된 게임 탭에 따라 활성화 되는 탭이 다르다.
		int nRoomTabIndex = GM().GetRoomListTabIndex();
		if( nGame == IDX_GAME_HL || nGame == IDX_GAME_HA )
		{
			pTab->SetCurSelFromIndex( 1 );
		}
		else
		{
			// 현재 사용중이거나 선택된 룸 화폐 정보를 바탕으로 정보 표기
			if( g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD || nRoomTabIndex == 2 || nRoomTabIndex == 3 )
				pTab->SetCurSelFromIndex( 0 );
			else										   
				pTab->SetCurSelFromIndex( 1 );
		}
	}
	
	// 현재 정보를 바탕으로 게임 전적 셋팅( 탭 생성 보다 위로 위치 변경 금지 )
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
	// 전적 탭 관련
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
		
	// 활성화된 탭에 따라 정보 세팅
	if( pTab->GetCurSelIndex() == 0 )
	{
		str.Format("%d전 %d승 %d패(%d%%)", 
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetTotalNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetWinNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetLoseNum(nGame)+m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetFoldNum(nGame),
			m_cUserInfo.AllGameRecord[ PlayableChipType::GOLD ].GetWinPro(nGame));
	}
	else
	{
		str.Format("%d전 %d승 %d패(%d%%)", 
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
	//강퇴 버튼
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
				if(dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "공정한 경기를 진행하기 위하여\n잭팟매치 진행 전까지 강제추방이 불가합니다.\n고객님의 양해 부탁드립니다.") == IDOK)
				{
					OnOK();				
				}
				//m_uButtonFlag=((m_uButtonFlag)^((m_uButtonFlag)&(UPG_PROFILE_FLAG_BANISHMENT)));
			}
			else
			{
				CMsgBoxDlg dlg(this);
				if(dlg.DoModal(CMsgBoxDlg::EMBT_QUESTION, "강제 추방 하시겠습니까?") == IDOK)
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
// 				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 쪽지를 보낼 수 없습니다.");
// 			}
		}
		break;
		/* 귓말 삭제
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
	case ID_USERPROFILE_BTN_SAMEROOM_ENTER:		//같은 방 입장
		SendUILog_Basic(CLSTATS_UINO_POKER_PROFILE_JOIN_FRIENDROOM);
	case ID_USERPROFILE_BTN_TOGETHER_PLAYGAME:  //같이 게임하기
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
	// 채팅하기 -> 초대/가입요청 으로 변경
	case ID_USERPROFILE_BTN_INVITE_CLUB:
		{
			// 혹시 나중에 채팅하기 관련 작업이 있을 수 있어 주석을 남겨둠..
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
			//SendCreateChatting(&listInviteID, "커뮤니티 서버와 연결되어 있지 않아 대화창을 열수 없습니다.");
			
			int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
			int nUserGrade = m_cUserInfo.ClubMemberGrade;
			// 초대(내가 보스고 상대가 미가입 상태)
			if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
				nUserGrade == CLUB_MEMBER_GRADE_NONE)
			{
				CCL_CLUB_INVITE msg;
				
				msg.Set(m_cUserInfo.ID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
			// 가입요청(내가 클럽 미가입 상태이고 상대가 보스일때)
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
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 연결되어 있지 않아 내친구 목록에 추가할 수 없습니다.");
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
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 연결되어 있지 않아 블랙리스트 목록에 추가할 수 없습니다.");
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
			if (*msg.joinResult_ == CafeSystem::JoinResult::SUCCESS)// 가입 신청 완료
			{
				str.Format( "가입 신청이 완료 되었습니다." );
			}
			else if(*msg.joinResult_ == CafeSystem::JoinResult::ALREADY_MEMBER)// 이미 멤버임. (가입된 상태)
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::CAFE)
				{
					str.Format( "이미 카페에 가입된 회원입니다." );
				}
				else if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "이미 연합에 가입된 회원입니다." );
				}
			}
			else if(*msg.joinResult_ == CafeSystem::JoinResult::EXCESS_DAY_LIMIT_COUNT)// 1일 가입요청 건수 제한 초과.
			{
				str.Format( "1일 10회까지만 가입 요청이 가능합니다." );
			}			
			else if(*msg.joinResult_ == CafeSystem::JoinResult::ALREADY_ASKED)// 이미 가입 요청을 한 상태.
			{
				str.Format( "이미 가입을 요청한 상태입니다." );
			}
			else if(*msg.joinResult_ == CafeSystem::JoinResult::FULL_MEMBER_COUNT)// 이미 멤버가 다 찼음.
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::CAFE)
				{
					str.Format( "카페 준회원 보유 초과로 더 이상 가입 요청할 수 없습니다." );
				}
				else if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "카페 보유 수 초과로 더 이상 가입 요청할 수 없습니다." );
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
			if (*msg.inviteResult_ == CafeSystem::InviteResult::SUCCESS) // 초대 완료
			{
				str.Format( "정상적으로 초대 완료하였습니다." );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::NOT_EXIST_INVITE_MEMBER) // 초대할 대상이 없음.
			{
				str.Format( "해당 사용자가 존재하지 않습니다." );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::EXCESS_DAY_LIMIT_COUNT) // 1일 초대 건수 제한 초과.
			{
				if(GM().GetMyInfo()->UI.cafeMemberGrade_ >= CafeSystem::MemberGrade::CAFE_BOSS)
				{
					str.Format( "1일 초대 10건 제한을 초과하였습니다. 익일 00시 부터 초대가 가능합니다." );
				}
				else
				{
					str.Format( "1일 초대 5건 제한을 초과하였습니다. 익일 00시 부터 초대가 가능합니다." );
				}
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::YOURE_NOT_CAFE_BOSS) // 카페장이 아님.
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "연합초대는 카페장에게만 가능합니다." );
				}
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::ALREADY_MEMBER) // 이미 멤버임. (가입된 상태)
			{
				if(*msg.cafeKind_==CafeSystem::CafeKind::CAFE)
				{
					str.Format( "해당 사용자가 카페에 가입되어 있습니다." );
				}
				else if(*msg.cafeKind_==CafeSystem::CafeKind::UNITED_CAFE)
				{
					str.Format( "해당 사용자가 연합에 가입되어 있습니다." );
				}
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::FULL_MEMBER_COUNT) // 이미 멤버가 다 찼음.
			{
				str.Format( "카페원이 다 있는 상태입니다. 초대를 하기 위해서는 카페원이 탈퇴하거나 VIP등급이 상향되어야 합니다" );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::NOW_UPDATING_VIP_GRADE) // 월초 VIP 등급 갱신중일 경우.
			{
				str.Format( "VIP 등급 갱신 중에는 카페원을 초대할 수 없습니다. 갱신 완료 이후 다시 시도해주시기 바랍니다" );
			}
			else if(*msg.inviteResult_ == CafeSystem::InviteResult::ALREADY_ASKED) // 이미 초대 요청을 한 상태.
			{
				str.Format( "이미 초대를 한 상태입니다." );
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
