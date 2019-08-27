// RewardItemMan.cpp: implementation of the CRewardItemMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RewardItemMan.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CRewardItemMan& CRewardItemMan::GetInstance()
{
	static CRewardItemMan instance;
	return instance;
}

CRewardItemMan& RewardItemMan()
{
	return CRewardItemMan::GetInstance();
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRewardItemMan::CRewardItemMan()
{
	m_Persuasion_ItemIndex = -1;
	// [아이템 유료화 영역]
	m_bUpdateNavi = TRUE;
}

CRewardItemMan::~CRewardItemMan()
{

}

void CRewardItemMan::CloseBuyDlg()
{

	if( RewardItemMan().m_LuckAvataItemDlg.GetSafeHwnd() ){
		RewardItemMan().m_LuckAvataItemDlg.SendMessage(WM_COMMAND , IDCANCEL );
	}

}
void CRewardItemMan::ShowBuyDlg( char* cookidata, int itemcode, int gamecode, SYSTEMTIME& recvtime ,int buyitemcnt)
{
	GM().SendMainWndMessage(UM_CLOSEALLPOPUP);
	
	if( m_LuckAvataItemDlg.GetSafeHwnd() == NULL ){
		m_LuckAvataItemDlg.DoModal(cookidata , recvtime , itemcode );
	}

	// [아이템 유료화 영역]
	m_Persuasion_ItemIndex = -1;
}

// [5광찬스 아이템 작업] [2005-11-30]
// 사랑의 리퀘스트
void CRewardItemMan::RequestFirstPriorityItem( int nGameCode, REWARD_TYPE eRewardType )
{
	REWARD_GAMEITEM_INFO *pRewardGameItemInfo = GetFirstPriorityItem(nGameCode, eRewardType);
	if (pRewardGameItemInfo)
	{

		CSV_ASK_BUYITEM buy_msg;
		buy_msg.Set( pRewardGameItemInfo->ItemCode, pRewardGameItemInfo->GameCode,
			pRewardGameItemInfo->ItemTime );
		NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), buy_msg.pData, buy_msg.GetTotalSize() );
	}
}

// [5광찬스 아이템 작업] [2005-12-01]
CString CRewardItemMan::GetRewardItemName( int itemcode )
{
	CString str = _T("???");

	switch( itemcode )
	{		
	case ITEM_SCODE_REWARD_FH:				str.Format( "풀하우스찬스" );	break;
	case ITEM_SCODE_REWARD_FCARD:			str.Format( "포카드찬스" );		break;
	case ITEM_SCODE_REWARD_SF:				str.Format( "스티플찬스" );		break;
	case ITEM_SCODE_REWARD_RSF:				str.Format( "로티플찬스" );		break;	
	case ITEM_SCODE_REWARD_GOLF:			str.Format( "골프찬스" );		break;
	case ITEM_SCODE_REWARD_SECOND:			str.Format( "세컨드찬스" );		break;
	case ITEM_SCODE_REWARD_THIRD:			str.Format( "서드찬스" );		break;
	case ITEM_SCODE_REWARD_CHANCEAVATA:		str.Format( "이벤트찬스" );		break;
	case ITEM_SCODE_REWARD_38:				str.Format( "38광땡찬스" );		break;
	case ITEM_SCODE_REWARD_PARKMUNSU:		str.Format( "암행어사찬스" );	break;	// 암행어사 아이템
	case ITEM_SCODE_REWARD_TANGKILLER:		str.Format( "땡잡이찬스" );		break;	// 땡잡이
	case ITEM_SCODE_REWARD_VIP:				str.Format( "VIP찬스" );		break;	// VIP
	case ITEM_SCODE_REWARD_VVIP:			str.Format( "VVIP찬스" );		break;	// VVIP
	case ITEM_SCODE_REWARDALLINCLINIC:		str.Format( "올인클리닉" );		break;	// 올인클리닉
	case ITEM_SCODE_REWARDJACKPOT:			str.Format( "잭팟찬스" );		break;	// 잭팟찬스
	case ITEM_SCODE_REWARDTHANKHOOLA:		str.Format( "땡큐훌라" );		break;	// 땡큐훌라
	case ITEM_SCODE_REWARD2MISSION:			str.Format( "더블미션" );		break;	// 더블미션
	case ITEM_SCORE_REWARDQUADRUPLE:		str.Format( "4배%s찬스", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);	break;	// 4배골드찬스
		// 기사 회생 (Bad-Beat) 찬스
	case ITEM_SCODE_REWARD_BADBEAT_FH:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:
	case ITEM_SCODE_REWARD_BADBEAT_SF:
	case ITEM_SCODE_REWARD_BADBEAT_RSF:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:
		str.Format( "기사회생" );	break;	// 기사회생찬스
		// VIP 기사회생 찬스
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:						//플러쉬
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:
		str.Format( "VIP 기사회생" );	break;	
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())
				str.Format( "VIP 기사회생" );	
			else										
				str.Format( "VVIP 기사회생" );	
		}
		break;	
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
		str.Format( "VVIP 기사회생" );	break;
	}

	return str;
}

// [5광찬스 아이템 작업] [2005-11-29]
// 아이템의 우선순위 값을 돌려준다.  ( 이 값을 기준으로 정렬됨 )
int CRewardItemMan::GetRewardItemPreciousLevel( int itemcode )
{
	//리턴되는 값이 낮을수록 우선순위가 높다.
	// 기사 회생 추가.
	switch( itemcode )
	{		
		//훌라
	case ITEM_SCODE_REWARDALLINCLINIC:		return 123;
	case ITEM_SCODE_REWARD2MISSION:			return 122;
	case ITEM_SCODE_REWARDTHANKHOOLA:		return 121;
	case ITEM_SCORE_REWARDQUADRUPLE:		return 120;
	case ITEM_SCODE_REWARDJACKPOT:			return 119;

	case ITEM_SCODE_REWARD_38:				return 118;
	case ITEM_SCODE_REWARD_TANGKILLER:		return 117;
	case ITEM_SCODE_REWARD_PARKMUNSU:		return 116;	
	case ITEM_SCODE_REWARD_THIRD:			return 115;
	case ITEM_SCODE_REWARD_SECOND:			return 114;
	case ITEM_SCODE_REWARD_GOLF:			return 113;
	case ITEM_SCODE_REWARD_FH:				return 112;
	case ITEM_SCODE_REWARD_FCARD:			return 111;
	case ITEM_SCODE_REWARD_SF:				return 110;

		// 기사 회생 (Bad-Beat) 찬스
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:			//6 메이드까지만
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:			//메이드
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:			//서드
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:			//세컨드
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:			//골프
	case ITEM_SCODE_REWARD_BADBEAT_FH:				//풀하우스
		return 108;	//포카드
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:		//포카드
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())return 107;	//포카드
			else										return 106;	//7포커 포카드는 우선 순위 높음
		}
	case ITEM_SCODE_REWARD_BADBEAT_SF:			//스티플
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())return 105;	//포카드
			else										return 104;	//7포커 포카드는 우선 순위 높음
		}
	case ITEM_SCODE_REWARD_BADBEAT_RSF:					return 103;	//로티플
	case ITEM_SCODE_REWARD_RSF:							
		{
			if(IDX_GAME_NP != GM().GetCurrentGameType())return 102;	//뉴포커가 아닌 로티플은 A형
			else										return 109;	//뉴포커 로티플은 B형
		}
	case ITEM_SCODE_REWARD_CHANCEAVATA:					return 101;	//이벤트찬스

	case ITEM_SCODE_REWARD_VIP:							return 7;	//승자 VIP
	
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:						//6 메이드까지만
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:						//메이드
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:						//서드
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:						//세컨드
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:						//골프
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:						//플러쉬
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:							//풀하우스
		return 6;
	case ITEM_SCODE_REWARD_VVIP:						return 4;	//승자 VVIP
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())return 5;	//포카드
			else										return 3;	//7포커 포카드는 우선 순위 높음
		}
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:				return 2;	//스티플
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:				return 1;	//로티플
			//VVIP찬스 우선순위가 가장 높다
	}

	//switch( itemcode )
	//{		
	//	//훌라
	//	case ITEM_SCODE_REWARDALLINCLINIC:		return 116;
	//	case ITEM_SCODE_REWARD2MISSION:			return 115;
	//	case ITEM_SCODE_REWARDTHANKHOOLA:		return 114;
	//	case ITEM_SCORE_REWARDQUADRUPLE:		return 113;
	//	case ITEM_SCODE_REWARDJACKPOT:			return 112;

	//	case ITEM_SCODE_REWARD_38:				return 111;
	//	case ITEM_SCODE_REWARD_TANGKILLER:		return 110;
	//	case ITEM_SCODE_REWARD_PARKMUNSU:		return 109;	
	//	case ITEM_SCODE_REWARD_THIRD:			return 108;
	//	case ITEM_SCODE_REWARD_SECOND:			return 107;
	//	case ITEM_SCODE_REWARD_GOLF:			return 106;
	//	case ITEM_SCODE_REWARD_FH:				return 105;
	//	case ITEM_SCODE_REWARD_FCARD:			return 104;
	//	case ITEM_SCODE_REWARD_SF:				return 103;	
	//	case ITEM_SCODE_REWARD_RSF:				return 102;
	//	case ITEM_SCODE_REWARD_CHANCEAVATA:		return 101;		//이벤트찬스

	//	case ITEM_SCODE_REWARD_VIP:				return 2;	
	//	case ITEM_SCODE_REWARD_VVIP:			return 1;		//VVIP찬스 우선순위가 가장 높다
	//}
	
	return -1;
}

// [5광찬스 아이템 작업] [2005-12-05]
bool RewardItem_GreaterThanSecond( REWARD_GAMEITEM_INFO first, REWARD_GAMEITEM_INFO second )
{
	return ( CRewardItemMan::GetRewardItemPreciousLevel( first.ItemCode ) < 
		     CRewardItemMan::GetRewardItemPreciousLevel( second.ItemCode ) );
}

// [5광찬스 아이템 작업] [2005-11-27]
void CRewardItemMan::AddRewardItem( int itemcode, int gamecode, SYSTEMTIME rctime , SYSTEMTIME expiretime)
{
	int AddItemIndex = -1;

	REWARD_GAMEITEM_INFO info = {0,}; 
	info.ItemCode = itemcode; info.GameCode = gamecode;
	memcpy( &info.ItemTime, &rctime, sizeof(info.ItemTime) );
	memcpy( &info.TimeExpire, &expiretime, sizeof(info.TimeExpire) );

//	CPlayer *pMyInfo = GM().GetMyInfo();

// 	if( (GM().GetCurrentGameType() == IDX_GAME_NP) && (itemcode == ITEM_SCODE_REWARD_LUCKAVATA ))
// 	{		
// 		if( g_Config.nLAOPen == 0 )
// 		{
// 			g_Config.nLAOPen = 1;
// 			CTime time = CTime::GetCurrentTime();	
// 			g_Config.nLADay = time.GetDay();
// 			SaveGameConfig(pMyInfo->UI.ID);
// 			WriteProfile_int(pMyInfo->UI.ID, "LAHour", time.GetHour());
// 		}
// 		else if( !CheckLuckAvatar() )
// 		{
// 			return;
// 		}
// 	}

	// reward item db 08.02.27 
	//똑같은 아이템이 이미 있다면 갱신 없으면 추가 
// 	for(vector<REWARD_GAMEITEM_INFO>::iterator iter = m_RewardItem.begin(); iter!=m_RewardItem.end(); iter++)
// 	{
// 		if ( (*iter).ItemCode == itemcode  && (*iter).GameCode == gamecode)
// 		{
// 			m_RewardItem.erase(iter);
// 			break;
// 		}
// 	}

	ClearItemList(gamecode, GetItemType(itemcode));		// Gamecode, ItemType에 해당되는 찬스아이템을 모두 지운다. (왜냐하면 , 게임별, 찬스타입별 하나의 찬스아이템만 보유할수있다.)

	m_RewardItem.push_back( info );

	sort( m_RewardItem.begin(), m_RewardItem.end(), RewardItem_GreaterThanSecond );
	m_Persuasion_ItemIndex = 0;
}


// [5광찬스 아이템 작업] [2005-11-27]
void CRewardItemMan::ShowRewardItemState(int nGamecode)
{
	sort( m_RewardItem.begin(), m_RewardItem.end(), RewardItem_GreaterThanSecond );

#ifdef _BETAVER
	return;
#endif

	if (m_Persuasion_ItemIndex>=0)
	{
		REWARD_GAMEITEM_INFO *pRewardGameItemInfo= GetFirstPriorityItem(nGamecode);
		if (pRewardGameItemInfo)
		{
			CSV_ASK_BUYITEM buy_msg;
			buy_msg.Set( pRewardGameItemInfo->ItemCode, pRewardGameItemInfo->GameCode,
				pRewardGameItemInfo->ItemTime );

			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), buy_msg.pData, buy_msg.GetTotalSize() );
			
		}


	}

// 	
// 	CString str;
// 	// 서버에서 구매 기회를 보내주면 확인후 쿠키값 을 서버에 보낸다. (서버에서 사라고 한넘이 맞는 체크)
// 	if( m_Persuasion_ItemIndex >= 0 && m_Persuasion_ItemIndex < static_cast<int>(m_RewardItem.size()) && 
// 		m_RewardItem[m_Persuasion_ItemIndex].GameCode > 0 && m_RewardItem[m_Persuasion_ItemIndex].ItemCode > 0 )
// 	{
// 		CSV_ASK_BUYITEM buy_msg;
// 		buy_msg.Set( m_RewardItem[m_Persuasion_ItemIndex].ItemCode, m_RewardItem[m_Persuasion_ItemIndex].GameCode,
// 					 m_RewardItem[m_Persuasion_ItemIndex].ItemTime );
// 
// 		NMBASE::SOCK::GSOCKMAN().SendData( g_MainSrvSID, buy_msg.pData, buy_msg.GetTotalSize() );
// 	}
}

// [5광찬스 아이템 작업] [2005-11-29]
void CRewardItemMan::ClearItem( int itemcode, SYSTEMTIME recvtime )
{
	vector<REWARD_GAMEITEM_INFO>::iterator it = m_RewardItem.begin();
	for( it; it != m_RewardItem.end(); it++ )
	{
		if( it->ItemCode == itemcode &&
			memcmp( &(it->ItemTime), &recvtime, sizeof(SYSTEMTIME)) == 0 )
		{
			// 해당 아이템이 존재!! 이 아이템은 삭제해도 된다.
			m_RewardItem.erase( it );
			break;
		}
	}
	// [아이템 유료화 영역]
	if ( m_RewardItem.empty() )
	{
		m_Persuasion_ItemIndex = -1;
		m_bUpdateNavi = TRUE;
	}
//	ShowRewardItemState();
}


// 구매여부 결과
void CRewardItemMan::CheckRewardItemResult( int PurchaseResult, int ItemCode, SYSTEMTIME&RecvTime, INT64 Money )
{
	CPlayer *pMyInfo = GM().GetMyInfo();
	if( PurchaseResult == REWARDITEM_RESULT::ITEM_PURCHASED )
	{		
		// 구매했으므로 해당 아이템을 지운다.
		ClearItem( ItemCode, RecvTime );
		ShowChanceBtn();
	
		if( pMyInfo->UI.GetMoney() < Money )
		{
			CString str;					
			str.Format( "[%s]님께서는 [%s] 아이템을 사용하여 %s이(가) 충전되셨습니다.\n", 
				pMyInfo->UI.NickName, GetRewardItemName( ItemCode ).GetString() , ((CString)NMBASE::UTIL::g_MoneyMark(Money - pMyInfo->UI.GetMoney())).GetString() );
			AddLobbyChatViewEdit( str.GetString(), RGB(0,44,93) );
		}
	}
	
	CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(0);
	pPlayer->UI.SetMoney(Money);
	pMyInfo->UI.SetMoney(Money);
}

void CRewardItemMan::AllowBuyItem( int itemcode, int gamecode, SYSTEMTIME rctime , SYSTEMTIME expiretime, int nViewitemcode ) 
{		
	AddRewardItem( itemcode, gamecode, rctime , expiretime);
	
	//이펙트
	CGameViewBase *pGameViewBase = GM().GetCurrentGameView();
	pGameViewBase->ShowEffect_AllowChancePurchase(nViewitemcode);

	//VIP 찬스아바타 11.08.30
	if (nViewitemcode == ITEM_SCODE_REWARD_VVIP)
	{
		CString strMsg;
		strMsg.Format("-------------------------------------\n   [%s]님, VVIP 찬스 구매 획득\n ## 일반:4조 / 골드:4.5조 / 다이아몬드:5조 ##\n-------------------------------------\n",GM().GetMyInfo()->UI.NickName);
		AddGameInfoViewEdit(strMsg, RGB(0,250,250));	
	
	}else if (nViewitemcode == ITEM_SCODE_REWARD_VIP)
	{
		CString strMsg;
		strMsg.Format("-------------------------------------\n   [%s]님, VIP 찬스 구매 획득\n ## 일반:3.7조 / 골드:4조 / 다이아몬드:4.5조  ##\n-------------------------------------\n",GM().GetMyInfo()->UI.NickName);
		AddGameInfoViewEdit(strMsg, RGB(0,250,250));
	
	}else if (GetItemType(nViewitemcode) == REWARD_TYPE_REGEN)
	{
		CString str ;
		CString strMsg;
		switch (itemcode)
		{
		case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:						//6 메이드까지만
		case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:						//메이드
		case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:						//서드
		case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:						//세컨드
		case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:						//골프
		case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:							//풀하우스
		case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:						//플러쉬
			str = "VIP기사회생 찬스 구매 획득\n # 일반:3.9조 / 골드 : 4.2조 / 다이아몬드 : 4.7조 #" ;
			break ;
		case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
			{
				if(IDX_GAME_SP != GM().GetCurrentGameType())str = "VIP기사회생 찬스 구매 획득\n # 일반:3.9조 / 골드 : 4.2조 / 다이아몬드 : 4.7조 #" ;	//포카드
				else										str = "VVIP기사회생 찬스 구매 획득\n # 일반:4.2조 / 골드 : 4.7조 / 다이아몬드 : 5.3조 #" ;
			}
			break ;
		case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
		case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
			str = "VVIP기사회생 찬스 구매 획득\n # 일반:4.2조 / 골드 : 4.7조 / 다이아몬드 : 5.3조 #" ;
			break ;
		default:
			strMsg.Format("-------------------------------------\n   [%s]님, 기사회생 구입자격 획득\n-------------------------------------\n",GM().GetMyInfo()->UI.NickName);
			AddGameInfoViewEdit(strMsg, RGB(255,167,63));
			return;
		}
		strMsg.Format("-------------------------------------\n   [%s]님, %s\n-------------------------------------\n", GM().GetMyInfo()->UI.NickName, str.GetString());
		AddGameInfoViewEdit(strMsg, RGB(255,167,63));
	
	}else
	{
		CString strMsg;
		strMsg.Format( "◈ [%s]님 %s \n구입자격 획득.\n", GM().GetMyInfo()->UI.NickName, GetRewardItemName( nViewitemcode ));
		AddGameInfoViewEdit(strMsg, RGB(143, 141, 102));
	}
}

int CRewardItemMan::get_Item_GameCode(int item )
{
	for(vector<REWARD_GAMEITEM_INFO>::iterator iter = m_RewardItem.begin(); iter!=m_RewardItem.end(); iter++)
	{
		if ( (*iter).ItemCode == item )
		{
			return (*iter).GameCode;
		}
	}

	return -1;
}


REWARD_GAMEITEM_INFO *CRewardItemMan::GetFirstPriorityItem(int gamecode, REWARD_TYPE eRewardType)	//대기실에 노출될 게임별 최고우선순위 찬스아이템 가져오기
{

	RemoveExpiredItem();		//

	for(vector<REWARD_GAMEITEM_INFO>::iterator iter = m_RewardItem.begin(); iter!=m_RewardItem.end(); iter++)
	{
		if ( (*iter).GameCode == gamecode )
		{
			if(GetItemType((*iter).ItemCode) == eRewardType)
			{
				return &(*iter);
			}		
		}
	}
	return NULL;
}

REWARD_GAMEITEM_INFO *CRewardItemMan::GetFirstPriorityItem(int gamecode)	//대기실에 노출될 게임별 최고우선순위 찬스아이템 가져오기
{

	RemoveExpiredItem();		//

	for(vector<REWARD_GAMEITEM_INFO>::iterator iter = m_RewardItem.begin(); iter!=m_RewardItem.end(); iter++)
	{
		if ( (*iter).GameCode == gamecode )
		{
			return &(*iter);				
		}
	}
	return NULL;
}

CRewardItemMan::REWARD_TYPE CRewardItemMan::GetItemType(int itemcode)
{
	switch (itemcode)
	{
	case ITEM_SCODE_REWARDALLINCLINIC:
	case ITEM_SCODE_REWARD2MISSION:
	case ITEM_SCODE_REWARDTHANKHOOLA:
	case ITEM_SCODE_REWARDJACKPOT:
	case ITEM_SCORE_REWARDQUADRUPLE:
	case ITEM_SCODE_REWARD_38:				
	case ITEM_SCODE_REWARD_TANGKILLER:		
	case ITEM_SCODE_REWARD_PARKMUNSU:
	case ITEM_SCODE_REWARD_LUCKAVATA:
	case ITEM_SCODE_REWARD_THIRD:			
	case ITEM_SCODE_REWARD_SECOND:			
	case ITEM_SCODE_REWARD_GOLF:			
	case ITEM_SCODE_REWARD_FH:				
	case ITEM_SCODE_REWARD_FCARD:			
	case ITEM_SCODE_REWARD_SF:					
	case ITEM_SCODE_REWARD_RSF:				
	case ITEM_SCODE_REWARD_CHANCEAVATA:		
	case ITEM_SCODE_REWARD_VVIP:
	case ITEM_SCODE_REWARD_VIP:
		return REWARD_TYPE_MADE;
	case ITEM_SCODE_REWARD_BADBEAT_FH:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:
	case ITEM_SCODE_REWARD_BADBEAT_SF:
	case ITEM_SCODE_REWARD_BADBEAT_RSF:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:
		return REWARD_TYPE_REGEN;
	}
	return REWARD_TYPE_MADE;
}


void CRewardItemMan::RemoveExpiredItem()
{

	COleDateTime now = g_GetSyncTime();

	vector<REWARD_GAMEITEM_INFO>::iterator it = m_RewardItem.begin();
	while( it != m_RewardItem.end() )
	{
		COleDateTime expiretime  = it->TimeExpire;
		if ( now >= expiretime)
		{
			it = m_RewardItem.erase( it );

		}
		else
		{
			 it++;
		}		
	}
}

void CRewardItemMan::ClearItemList( int gamecode, REWARD_TYPE eRewardType )
{
	vector<REWARD_GAMEITEM_INFO>::iterator it = m_RewardItem.begin();

	while( it != m_RewardItem.end() )
	{
		if( (GetItemType(it->ItemCode) == eRewardType) && it->GameCode == gamecode)
		{	
			it = m_RewardItem.erase( it );
		}
		else			 
		{
			it ++;
		}			
	}
}