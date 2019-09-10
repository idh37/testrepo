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
	// [������ ����ȭ ����]
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

	// [������ ����ȭ ����]
	m_Persuasion_ItemIndex = -1;
}

// [5������ ������ �۾�] [2005-11-30]
// ����� ������Ʈ
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

// [5������ ������ �۾�] [2005-12-01]
CString CRewardItemMan::GetRewardItemName( int itemcode )
{
	CString str = _T("???");

	switch( itemcode )
	{		
	case ITEM_SCODE_REWARD_FH:				str.Format( "Ǯ�Ͽ콺����" );	break;
	case ITEM_SCODE_REWARD_FCARD:			str.Format( "��ī������" );		break;
	case ITEM_SCODE_REWARD_SF:				str.Format( "��Ƽ������" );		break;
	case ITEM_SCODE_REWARD_RSF:				str.Format( "��Ƽ������" );		break;	
	case ITEM_SCODE_REWARD_GOLF:			str.Format( "��������" );		break;
	case ITEM_SCODE_REWARD_SECOND:			str.Format( "����������" );		break;
	case ITEM_SCODE_REWARD_THIRD:			str.Format( "��������" );		break;
	case ITEM_SCODE_REWARD_CHANCEAVATA:		str.Format( "�̺�Ʈ����" );		break;
	case ITEM_SCODE_REWARD_38:				str.Format( "38��������" );		break;
	case ITEM_SCODE_REWARD_PARKMUNSU:		str.Format( "����������" );	break;	// ������ ������
	case ITEM_SCODE_REWARD_TANGKILLER:		str.Format( "����������" );		break;	// ������
	case ITEM_SCODE_REWARD_VIP:				str.Format( "VIP����" );		break;	// VIP
	case ITEM_SCODE_REWARD_VVIP:			str.Format( "VVIP����" );		break;	// VVIP
	case ITEM_SCODE_REWARDALLINCLINIC:		str.Format( "����Ŭ����" );		break;	// ����Ŭ����
	case ITEM_SCODE_REWARDJACKPOT:			str.Format( "��������" );		break;	// ��������
	case ITEM_SCODE_REWARDTHANKHOOLA:		str.Format( "��ť�Ƕ�" );		break;	// ��ť�Ƕ�
	case ITEM_SCODE_REWARD2MISSION:			str.Format( "����̼�" );		break;	// ����̼�
	case ITEM_SCORE_REWARDQUADRUPLE:		str.Format( "4��%s����", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);	break;	// 4��������
		// ��� ȸ�� (Bad-Beat) ����
	case ITEM_SCODE_REWARD_BADBEAT_FH:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:
	case ITEM_SCODE_REWARD_BADBEAT_SF:
	case ITEM_SCODE_REWARD_BADBEAT_RSF:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:
		str.Format( "���ȸ��" );	break;	// ���ȸ������
		// VIP ���ȸ�� ����
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:						//�÷���
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:
		str.Format( "VIP ���ȸ��" );	break;	
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())
				str.Format( "VIP ���ȸ��" );	
			else										
				str.Format( "VVIP ���ȸ��" );	
		}
		break;	
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
		str.Format( "VVIP ���ȸ��" );	break;
	}

	return str;
}

// [5������ ������ �۾�] [2005-11-29]
// �������� �켱���� ���� �����ش�.  ( �� ���� �������� ���ĵ� )
int CRewardItemMan::GetRewardItemPreciousLevel( int itemcode )
{
	//���ϵǴ� ���� �������� �켱������ ����.
	// ��� ȸ�� �߰�.
	switch( itemcode )
	{		
		//�Ƕ�
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

		// ��� ȸ�� (Bad-Beat) ����
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:			//6 ���̵������
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:			//���̵�
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:			//����
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:			//������
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:			//����
	case ITEM_SCODE_REWARD_BADBEAT_FH:				//Ǯ�Ͽ콺
		return 108;	//��ī��
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:		//��ī��
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())return 107;	//��ī��
			else										return 106;	//7��Ŀ ��ī��� �켱 ���� ����
		}
	case ITEM_SCODE_REWARD_BADBEAT_SF:			//��Ƽ��
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())return 105;	//��ī��
			else										return 104;	//7��Ŀ ��ī��� �켱 ���� ����
		}
	case ITEM_SCODE_REWARD_BADBEAT_RSF:					return 103;	//��Ƽ��
	case ITEM_SCODE_REWARD_RSF:							
		{
			if(IDX_GAME_NP != GM().GetCurrentGameType())return 102;	//����Ŀ�� �ƴ� ��Ƽ���� A��
			else										return 109;	//����Ŀ ��Ƽ���� B��
		}
	case ITEM_SCODE_REWARD_CHANCEAVATA:					return 101;	//�̺�Ʈ����

	case ITEM_SCODE_REWARD_VIP:							return 7;	//���� VIP
	
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:						//6 ���̵������
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:						//���̵�
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:						//����
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:						//������
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:						//����
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:						//�÷���
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:							//Ǯ�Ͽ콺
		return 6;
	case ITEM_SCODE_REWARD_VVIP:						return 4;	//���� VVIP
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())return 5;	//��ī��
			else										return 3;	//7��Ŀ ��ī��� �켱 ���� ����
		}
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:				return 2;	//��Ƽ��
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:				return 1;	//��Ƽ��
			//VVIP���� �켱������ ���� ����
	}

	//switch( itemcode )
	//{		
	//	//�Ƕ�
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
	//	case ITEM_SCODE_REWARD_CHANCEAVATA:		return 101;		//�̺�Ʈ����

	//	case ITEM_SCODE_REWARD_VIP:				return 2;	
	//	case ITEM_SCODE_REWARD_VVIP:			return 1;		//VVIP���� �켱������ ���� ����
	//}
	
	return -1;
}

// [5������ ������ �۾�] [2005-12-05]
bool RewardItem_GreaterThanSecond( REWARD_GAMEITEM_INFO first, REWARD_GAMEITEM_INFO second )
{
	return ( CRewardItemMan::GetRewardItemPreciousLevel( first.ItemCode ) < 
		     CRewardItemMan::GetRewardItemPreciousLevel( second.ItemCode ) );
}

// [5������ ������ �۾�] [2005-11-27]
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
	//�Ȱ��� �������� �̹� �ִٸ� ���� ������ �߰� 
// 	for(vector<REWARD_GAMEITEM_INFO>::iterator iter = m_RewardItem.begin(); iter!=m_RewardItem.end(); iter++)
// 	{
// 		if ( (*iter).ItemCode == itemcode  && (*iter).GameCode == gamecode)
// 		{
// 			m_RewardItem.erase(iter);
// 			break;
// 		}
// 	}

	ClearItemList(gamecode, GetItemType(itemcode));		// Gamecode, ItemType�� �ش�Ǵ� ������������ ��� �����. (�ֳ��ϸ� , ���Ӻ�, ����Ÿ�Ժ� �ϳ��� ���������۸� �����Ҽ��ִ�.)

	m_RewardItem.push_back( info );

	sort( m_RewardItem.begin(), m_RewardItem.end(), RewardItem_GreaterThanSecond );
	m_Persuasion_ItemIndex = 0;
}


// [5������ ������ �۾�] [2005-11-27]
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
// 	// �������� ���� ��ȸ�� �����ָ� Ȯ���� ��Ű�� �� ������ ������. (�������� ���� �ѳ��� �´� üũ)
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

// [5������ ������ �۾�] [2005-11-29]
void CRewardItemMan::ClearItem( int itemcode, SYSTEMTIME recvtime )
{
	vector<REWARD_GAMEITEM_INFO>::iterator it = m_RewardItem.begin();
	for( it; it != m_RewardItem.end(); it++ )
	{
		if( it->ItemCode == itemcode &&
			memcmp( &(it->ItemTime), &recvtime, sizeof(SYSTEMTIME)) == 0 )
		{
			// �ش� �������� ����!! �� �������� �����ص� �ȴ�.
			m_RewardItem.erase( it );
			break;
		}
	}
	// [������ ����ȭ ����]
	if ( m_RewardItem.empty() )
	{
		m_Persuasion_ItemIndex = -1;
		m_bUpdateNavi = TRUE;
	}
//	ShowRewardItemState();
}


// ���ſ��� ���
void CRewardItemMan::CheckRewardItemResult( int PurchaseResult, int ItemCode, SYSTEMTIME&RecvTime, INT64 Money )
{
	CPlayer *pMyInfo = GM().GetMyInfo();
	if( PurchaseResult == REWARDITEM_RESULT::ITEM_PURCHASED )
	{		
		// ���������Ƿ� �ش� �������� �����.
		ClearItem( ItemCode, RecvTime );
		ShowChanceBtn();
	
		if( pMyInfo->UI.GetMoney() < Money )
		{
			CString str;					
			str.Format( "[%s]�Բ����� [%s] �������� ����Ͽ� %s��(��) �����Ǽ̽��ϴ�.\n", 
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
	
	//����Ʈ
	CGameViewBase *pGameViewBase = GM().GetCurrentGameView();
	pGameViewBase->ShowEffect_AllowChancePurchase(nViewitemcode);

	//VIP �����ƹ�Ÿ 11.08.30
	if (nViewitemcode == ITEM_SCODE_REWARD_VVIP)
	{
		CString strMsg;
		strMsg.Format("-------------------------------------\n   [%s]��, VVIP ���� ���� ȹ��\n ## �Ϲ�:4�� / ���:4.5�� / ���̾Ƹ��:5�� ##\n-------------------------------------\n",GM().GetMyInfo()->UI.NickName);
		AddGameInfoViewEdit(strMsg, RGB(0,250,250));	
	
	}else if (nViewitemcode == ITEM_SCODE_REWARD_VIP)
	{
		CString strMsg;
		strMsg.Format("-------------------------------------\n   [%s]��, VIP ���� ���� ȹ��\n ## �Ϲ�:3.7�� / ���:4�� / ���̾Ƹ��:4.5��  ##\n-------------------------------------\n",GM().GetMyInfo()->UI.NickName);
		AddGameInfoViewEdit(strMsg, RGB(0,250,250));
	
	}else if (GetItemType(nViewitemcode) == REWARD_TYPE_REGEN)
	{
		CString str ;
		CString strMsg;
		switch (itemcode)
		{
		case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:						//6 ���̵������
		case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:						//���̵�
		case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:						//����
		case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:						//������
		case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:						//����
		case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:							//Ǯ�Ͽ콺
		case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:						//�÷���
			str = "VIP���ȸ�� ���� ���� ȹ��\n # �Ϲ�:3.9�� / ��� : 4.2�� / ���̾Ƹ�� : 4.7�� #" ;
			break ;
		case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
			{
				if(IDX_GAME_SP != GM().GetCurrentGameType())str = "VIP���ȸ�� ���� ���� ȹ��\n # �Ϲ�:3.9�� / ��� : 4.2�� / ���̾Ƹ�� : 4.7�� #" ;	//��ī��
				else										str = "VVIP���ȸ�� ���� ���� ȹ��\n # �Ϲ�:4.2�� / ��� : 4.7�� / ���̾Ƹ�� : 5.3�� #" ;
			}
			break ;
		case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
		case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
			str = "VVIP���ȸ�� ���� ���� ȹ��\n # �Ϲ�:4.2�� / ��� : 4.7�� / ���̾Ƹ�� : 5.3�� #" ;
			break ;
		default:
			strMsg.Format("-------------------------------------\n   [%s]��, ���ȸ�� �����ڰ� ȹ��\n-------------------------------------\n",GM().GetMyInfo()->UI.NickName);
			AddGameInfoViewEdit(strMsg, RGB(255,167,63));
			return;
		}
		strMsg.Format("-------------------------------------\n   [%s]��, %s\n-------------------------------------\n", GM().GetMyInfo()->UI.NickName, str.GetString());
		AddGameInfoViewEdit(strMsg, RGB(255,167,63));
	
	}else
	{
		CString strMsg;
		strMsg.Format( "�� [%s]�� %s \n�����ڰ� ȹ��.\n", GM().GetMyInfo()->UI.NickName, GetRewardItemName( nViewitemcode ));
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


REWARD_GAMEITEM_INFO *CRewardItemMan::GetFirstPriorityItem(int gamecode, REWARD_TYPE eRewardType)	//���ǿ� ����� ���Ӻ� �ְ�켱���� ���������� ��������
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

REWARD_GAMEITEM_INFO *CRewardItemMan::GetFirstPriorityItem(int gamecode)	//���ǿ� ����� ���Ӻ� �ְ�켱���� ���������� ��������
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