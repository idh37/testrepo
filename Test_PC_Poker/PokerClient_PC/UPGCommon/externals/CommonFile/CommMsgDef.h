#ifndef __COMMMSGDEF_H__ 
#define __COMMMSGDEF_H__ 

#include "CommMsg.h"
#include "CommMsgIdHeader.h"
#include "PromotionMsgHeader.h"

#pragma warning (disable:4996)
////////////////////////////////////////////////////////////////////////////
//                 ������ ������ �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

/////////// ������ üũ��
class CMS_CHECKVERSION : public CCommMsg
{
public:

	int *Ver;

	void Set(int ver) 
	{
		SetHeader(MS_CHECKVERSION);
		PushData(_PVAR(Ver), &ver, sizeof(*Ver));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Ver), sizeof(*Ver));
	}
};

/////////// ���� ���� �α��� ��û ����
class CMS_ASK_SERVERLOGIN : public CCommMsg
{
public:

	Ms_ServerLoginInfo *LI;
	Ms_ChannelInfo *pArrayCI;

	void Set( Ms_ServerLoginInfo *pLI, std::vector< Ms_ChannelInfo >& rArrayRef ) 
 	{
 		SetHeader( MS_ASK_SERVERLOGIN );
 		PushData( _PVAR( LI ), pLI, sizeof( *LI ));

		size_t size = ( size_t )LI->SvInfo.TotChan;
		for ( size_t i = 0; i < size; ++i )
		{
			PushData(_PVAR( pArrayCI ), &rArrayRef[ i ], sizeof( Ms_ChannelInfo ));
		}
 	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(LI), sizeof(*LI));
		TakeData(_PVAR(pArrayCI), sizeof(Ms_ChannelInfo) * LI->SvInfo.TotChan);
	}
};

/////////// �׷����˽� ���尡���� ����IP���� ����(���Ӽ��� ���ӿ�û����)
class CMS_GROUP_INSPECTION_ADMININFO : public CCommMsg
{
public:

	int* m_nIPInfoArraySize;
	Ms_GroupInspectionAdminIPInfo *m_pArrayIPInfo;

	void Set( int nIPInfoArraySize, std::vector<Ms_GroupInspectionAdminIPInfo>& rArrayRef ) 
	{
		SetHeader( MS_GROUP_INSPECTION_ADMININFO );
		PushData( _PVAR( m_nIPInfoArraySize ), &nIPInfoArraySize, sizeof( *m_nIPInfoArraySize));

		size_t size = ( size_t )nIPInfoArraySize;
		for ( size_t i = 0; i < size; ++i )
		{
			PushData(_PVAR( m_pArrayIPInfo ), &rArrayRef[ i ], sizeof( Ms_GroupInspectionAdminIPInfo ));
		}
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nIPInfoArraySize), sizeof(*m_nIPInfoArraySize));
		TakeData(_PVAR(m_pArrayIPInfo), sizeof(Ms_GroupInspectionAdminIPInfo) * (*m_nIPInfoArraySize));
	}
};

/////////// ���� ���� �α����� �����
class CMS_ACCEPT_SERVERLOGIN : public CCommMsg
{
public:

	Ms_ServerInfo *SI;
	Ms_ChannelInfo *ArrayCI;
	int *TotGCI;
	Ms_ChannelInfo *ArrayGCI;

	void Set(Ms_ServerInfo *pSI, std::vector< Ms_ChannelInfo >& rArrayCI , int totgci, std::vector< Ms_ChannelInfo >& rArrayGCI ) 
	{
		SetHeader(MS_ACCEPT_SERVERLOGIN);
		PushData(_PVAR(SI), pSI, sizeof(*SI));
		size_t size = rArrayCI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayCI ), &rArrayCI[ i ], sizeof( Ms_ChannelInfo ));
		}
		PushData(_PVAR(TotGCI), &totgci, sizeof(*TotGCI));

		size = rArrayGCI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayGCI ), &rArrayGCI[ i ], sizeof( Ms_ChannelInfo ));
		}
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SI), sizeof(*SI));
		TakeData(_PVAR(ArrayCI), sizeof(Ms_ChannelInfo) * SI->TotChan);
		TakeData(_PVAR(TotGCI), sizeof(*TotGCI));
		TakeData(_PVAR(ArrayGCI), sizeof(Ms_ChannelInfo) * (*TotGCI));
	}
};

/////////// ���Ӽ��� �α����� �ź���(�ź� ���� �ڵ带 ����)
class CMS_REFUSE_SERVERLOGIN : public CCommMsg
{
public:

	int *Code;
	int *l_StrMsg;
	char *StrMsg;
	void Set(int code, char* strmsg) 
	{
		int l_strmsg = 0;
		if(strmsg) l_strmsg = strlen(strmsg);
	
		SetHeader(MS_REFUSE_SERVERLOGIN);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushData(_PVAR(l_StrMsg), &l_strmsg, sizeof(*l_StrMsg));
		PushData(_PVAR(StrMsg), strmsg, l_strmsg);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));
		TakeData(_PVAR(l_StrMsg), sizeof(*l_StrMsg));
		TakeData(_PVAR(StrMsg), *l_StrMsg);
	}
};

/////////// ���� ���� ���� ���� ����
class CMS_SERVERSTATUS : public CCommMsg
{
public:

	Ms_ServerStatus	*SS;

	void Set(Ms_ServerStatus *pSS) 
	{
		SetHeader(MS_SERVERSTATUS);
		PushData(_PVAR(SS), pSS, sizeof(*SS));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SS), sizeof(*SS));
	}
};

/////////// ������ ���� ���� ����
class CMS_MASTERSTATUS : public CCommMsg
{
public:

	Ms_MasterStatus	*MS;
	Ms_GroupInfo *ArrayGI;

	void Set( Ms_MasterStatus *pMS, std::vector<Ms_GroupInfo>& rArrayGI ) 
	{
		SetHeader(MS_MASTERSTATUS);
		PushData(_PVAR(MS), pMS, sizeof(*MS));

		size_t size = rArrayGI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayGI ), &rArrayGI[ i ], sizeof( Ms_GroupInfo ));
		}
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(MS), sizeof(*MS));
		TakeData(_PVAR(ArrayGI), sizeof(Ms_GroupInfo) * MS->TotGroup);
	}
};

/////////// ���Ӽ��� ���� ����
class CMS_CONTROLCMD : public CCommMsg
{
public:

	int *Code;			// Ŀ�ǵ� �ڵ�
	VSTRING vstrCmd;	// Ŀ�ǵ� ��Ʈ��

	void Set(int code, char* strmsg) 
	{
		SetHeader(MS_CONTROLCMD);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushVSTR(vstrCmd, strmsg);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));
		TakeVSTR(vstrCmd);
	}

};

/////////// ���� ����͸� �α�(from ���Ӽ���)
class CMS_STATUSLOG : public CCommMsg
{
public:

	int *Kind;			// ����
	VSTRING vstrLog;	// �α� ��Ʈ��

	void Set(int kind, char* strlog) 
	{
		SetHeader(MS_STATUSLOG);
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushVSTR(vstrLog, strlog);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeVSTR(vstrLog);
	}
};

/////////// ����� ���� ����Ʈ(���� �α��ν� ������Ʈ��)
class CMS_USERINFOLIST : public CCommMsg
{
public:

	int			*TotUser;
	Ms_UserInfo *ArrayUI;

	void Set( std::vector<Ms_UserInfo>& rArrayUI ) 
	{
		int totuser = rArrayUI.size();
		SetHeader( MS_USERINFOLIST );
		PushData( _PVAR( TotUser ), &totuser, sizeof( *TotUser ));

		size_t size = rArrayUI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayUI ), &rArrayUI[ i ], sizeof( Ms_UserInfo ));
		}
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotUser), sizeof(*TotUser));
		TakeData(_PVAR(ArrayUI), sizeof(Ms_UserInfo) * (*TotUser));
	}
};

/////////// ä�� ������Ʈ ����
class CMS_CHANUPDATE : public CCommMsg
{
public:

	Ms_ChanUpdateInfo *CI;
	Ms_ChannelInfo *ArrayNewCI;
	Ms_SmallChanInfo *ArrayModSCI;
	Ms_SmallChanInfo *ArrayDelSCI;

	void Set(Ms_ChanUpdateInfo *pCI, std::vector< Ms_ChannelInfo >& rArrayNewCI, std::vector< Ms_SmallChanInfo >& rArrayModSCI, std::vector< Ms_SmallChanInfo >& rArrayDelSCI) 
	{
		SetHeader(MS_CHANUPDATE);
		PushData(_PVAR(CI), pCI, sizeof(*CI));

		size_t size = rArrayNewCI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayNewCI ), &rArrayNewCI[ i ], sizeof( Ms_ChannelInfo ));
		}

		size = rArrayModSCI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayModSCI ), &rArrayModSCI[ i ], sizeof( Ms_SmallChanInfo ));
		}

		size = rArrayDelSCI.size();
		for ( size_t i = 0; i < size; ++i )
		{
			PushData( _PVAR( ArrayDelSCI ), &rArrayDelSCI[ i ], sizeof( Ms_SmallChanInfo ));
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CI), sizeof(*CI));
		TakeData(_PVAR(ArrayNewCI), sizeof(Ms_ChannelInfo) * CI->TotNewChan);
		TakeData(_PVAR(ArrayModSCI), sizeof(Ms_SmallChanInfo) * CI->TotModChan);
		TakeData(_PVAR(ArrayDelSCI), sizeof(Ms_SmallChanInfo) * CI->TotDelChan);
	}
};

/////////// ����� ä�� ���� ����
class CMS_USERCHANGECHAN : public CCommMsg
{
public:

	Ms_UserChangeInfo *CI;
	void Set(Ms_UserChangeInfo *pCI) 
	{
		SetHeader(MS_USERCHANGECHAN);
		PushData(_PVAR(CI), pCI, sizeof(*CI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CI), sizeof(*CI));
	}
};

/////////// ����� ���� ���� ����
class CMS_DISCONNECTUSER : public CCommMsg
{
public:

	Ms_DisconnectUserInfo *DU;
	void Set(Ms_DisconnectUserInfo *pDU) 
	{
		SetHeader(MS_DISCONNECTUSER);
		PushData(_PVAR(DU), pDU, sizeof(*DU));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(DU), sizeof(*DU));
	}
};

/////////// ����� ���� üũ ����
class CMS_CHECKUSER : public CCommMsg
{
public:

	Ms_CheckUserInfo *CU;
	void Set(Ms_CheckUserInfo *pCU) 
	{
		SetHeader(MS_CHECKUSER);
		PushData(_PVAR(CU), pCU, sizeof(*CU));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CU), sizeof(*CU));
	}
};

/////////// ����ڰ� �ٸ� �������� �̵� ��û
class CMS_ASK_MOVEGAME : public CCommMsg
{
public:
	Ms_AskMoveGame *MG;
	USERINFO *UI;
	ORGINFO	*OI;
	USERINFO2 *UI2;
	GAMEITEM *ArrayGI;
	int*    nArraySize;
	promotion::UNION_PROMOTION_INFO* pArray;
	int* m_Count;
	UINT64* m_ArrayRoomKey;

	////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
	ETC_MOVE_INFO *m_EtcMoveInfo; //�� ������ �ܿ� �����̵��� ������ �Űܾ� �� �����͵��� �����Ѵ�.
	//�� ����ü�� ����Ǹ� �����ͼ��������� �ݵ�� ��������� ��~!!

	void Set(Ms_AskMoveGame *pMG, USERINFO *pUI, ORGINFO *pOI, USERINFO2 *pUI2, GAMEITEM *pArrayGI, std::vector< promotion::UNION_PROMOTION_INFO >& rArry, ETC_MOVE_INFO EtcMoveInfo, std::vector<UINT64>& vecllRoomKey) 
	{
		SetHeader(MS_ASK_MOVEGAME);

		PushData(_PVAR(MG), pMG, sizeof(*MG));
		PushData(_PVAR(UI), pUI, sizeof(*UI));
		PushData(_PVAR(OI), pOI, sizeof(*OI));
		PushData(_PVAR(UI2), pUI2, sizeof(*UI2));
		PushData(_PVAR(ArrayGI), pArrayGI, sizeof(GAMEITEM) * pMG->TotGameItem);
		int nSize = static_cast< int >( rArry.size() );
		PushData(_PVAR( nArraySize ), &nSize, sizeof( *nArraySize ));
		for ( int i = 0; i < nSize; i++ )
		{
			PushData(_PVAR( pArray ), &rArry[ i ], sizeof( *pArray ));
		}

		////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
		PushData(_PVAR(m_EtcMoveInfo), &EtcMoveInfo, sizeof(*m_EtcMoveInfo));
		//////////////////////////////////////////////////

		// �Ƕ� 2�η� ��ġ����ŷ ����ȭ
		int nCount = vecllRoomKey.size();
		PushData ( _PVAR(m_Count), &nCount, sizeof(*m_Count) );
		for ( int i = 0; i < nCount; i++)
		{
			PushData( _PVAR(m_ArrayRoomKey), &vecllRoomKey[i], sizeof(UINT64));
		}

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(MG), sizeof(*MG));
		TakeData(_PVAR(UI), sizeof(*UI));
		TakeData(_PVAR(OI), sizeof(*OI));
		TakeData(_PVAR(UI2), sizeof(*UI2));
		TakeData(_PVAR(ArrayGI), sizeof(GAMEITEM) * MG->TotGameItem);
		TakeData(_PVAR( nArraySize ), sizeof(*nArraySize));
		TakeData(_PVAR( pArray ), sizeof( *pArray ) * ( *nArraySize ) );

		////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
		TakeData(_PVAR(m_EtcMoveInfo), sizeof(*m_EtcMoveInfo));
		///////////////////////////////////////////////////////

		// �Ƕ� 2�η� ��ġ����ŷ ����ȭ
		TakeData( _PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayRoomKey), sizeof(UINT64) * (*m_Count));
	}
};

/////////// ������ �ٸ� ���� ���� ����
class CMS_RESULT_MOVEGAME : public CCommMsg
{
public:
	Ms_ResultMoveGame* RG;
	void Set(Ms_ResultMoveGame *pRG) 
	{
		SetHeader(MS_RESULT_MOVEGAME);
		PushData(_PVAR(RG), pRG, sizeof(*RG));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RG), sizeof(*RG));
	}
};

/////////// ����ڰ� �ٸ� ä�η� �̵� ��û
class CMS_ASK_MOVECHAN : public CCommMsg
{
public:

	Ms_AskMoveChan *MC;
	USERINFO *UI;
	ORGINFO	*OI;
	USERINFO2 *UI2;
	GAMEITEM *ArrayGI;
	int*    nArraySize;
	promotion::UNION_PROMOTION_INFO* pArray; 

	////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
	ETC_MOVE_INFO *m_EtcMoveInfo; //�� ������ �ܿ� �����̵��� ������ �Űܾ� �� �����͵��� �����Ѵ�.
	//�� ����ü�� ����Ǹ� �����ͼ��������� �ݵ�� ��������� ��~!!

	void Set(Ms_AskMoveChan *pMC, USERINFO *pUI, ORGINFO *pOI, USERINFO2 *pUI2, GAMEITEM *pArrayGI, std::vector< promotion::UNION_PROMOTION_INFO >& rArry, ETC_MOVE_INFO EtcMoveInfo) 
	{
		SetHeader(MS_ASK_MOVECHAN);
	
		PushData(_PVAR(MC), pMC, sizeof(*MC));
		PushData(_PVAR(UI), pUI, sizeof(*UI));
		PushData(_PVAR(OI), pOI, sizeof(*OI));
		PushData(_PVAR(UI2), pUI2, sizeof(*UI2));
		PushData(_PVAR(ArrayGI), pArrayGI, sizeof(GAMEITEM) * pMC->TotGameItem);
		
		int nSize = static_cast< int >( rArry.size() );
		PushData(_PVAR( nArraySize ), &nSize, sizeof( *nArraySize ));
		for ( int i = 0; i < nSize; i++ )
		{
			PushData(_PVAR( pArray ), &rArry[ i ], sizeof( *pArray ));
		}

		////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
		PushData(_PVAR(m_EtcMoveInfo), &EtcMoveInfo, sizeof(*m_EtcMoveInfo));
		//////////////////////////////////////////////////
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	
		TakeData(_PVAR(MC), sizeof(*MC));
		TakeData(_PVAR(UI), sizeof(*UI));
		TakeData(_PVAR(OI), sizeof(*OI));
		TakeData(_PVAR(UI2), sizeof(*UI2));
		TakeData(_PVAR(ArrayGI), sizeof(GAMEITEM) * MC->TotGameItem);

		TakeData(_PVAR( nArraySize ), sizeof(*nArraySize));
		TakeData(_PVAR( pArray ), sizeof( *pArray ) * ( *nArraySize ) );

		////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
		TakeData(_PVAR(m_EtcMoveInfo), sizeof(*m_EtcMoveInfo));
		///////////////////////////////////////////////////////
	}
};

/////////// ����ڰ� �ٸ� ä�η� �̵� ��û ���
class CMS_RESULT_MOVECHAN : public CCommMsg
{
public:

	Ms_ResultMoveChan *RM;
	void Set(Ms_ResultMoveChan *pRM) 
	{
		SetHeader(MS_RESULT_MOVECHAN);
		PushData(_PVAR(RM), pRM, sizeof(*RM));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RM), sizeof(*RM));
	}
};

/////////// ����� ã�� ��û
class CMS_ASK_FINDUSER : public CCommMsg
{
public:

	int  *ClientUNum;	// �Ƿ��� UNum
	char *ClientID;		// �Ƿ��� ID
	char *TargetNickName;		// ã�� ��� NickName
	void Set(int client_unum, char* client_id, char* target_nickname) 
	{
		char strClientId[16]={0,};
		if(strlen(client_id) < 16) strcpy(strClientId, client_id);
		char strTargetNickName[20]={0,};
		if(strlen(target_nickname) < 20) strcpy(strTargetNickName, target_nickname);
	
		SetHeader(MS_ASK_FINDUSER);
		PushData(_PVAR(ClientUNum), &client_unum, sizeof(*ClientUNum));
		PushData(_PVAR(ClientID), strClientId, 16);
		PushData(_PVAR(TargetNickName), strTargetNickName, 20);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ClientUNum), sizeof(*ClientUNum));
		TakeData(_PVAR(ClientID), 16);
		TakeData(_PVAR(TargetNickName), 20);
	}
};

/////////// ���̵� ã�� ���
class CMS_RESULT_FINDUSER : public CCommMsg
{
public:

	int  *ClientUNum;	// �Ƿ��� UNum
	char *ClientID;		// �Ƿ��� ID
	RESULTFINDID *RF;	// ã�� ���
	void Set(int client_unum, char* client_id, RESULTFINDID *pRF) 
	{
		char strClientId[16]={0,};
		if(strlen(client_id) < 16) strcpy(strClientId, client_id);
	
		SetHeader(MS_RESULT_FINDUSER);
		PushData(_PVAR(ClientUNum), &client_unum, sizeof(*ClientUNum));
		PushData(_PVAR(ClientID), strClientId, 16);
		PushData(_PVAR(RF), pRF, sizeof(RESULTFINDID));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ClientUNum), sizeof(*ClientUNum));
		TakeData(_PVAR(ClientID), 16);
		TakeData(_PVAR(RF), sizeof(RESULTFINDID));
	}
};

/////////// ������ ���Ӽ��� ���� ��û(From Ŭ���̾�Ʈ)
class CMS_ASK_DESTSERVERINFO : public CCommMsg
{
public:
	int *Group;
	void Set(int group) 
	{
		SetHeader(MS_ASK_DESTSERVERINFO);
		PushData(_PVAR(Group), &group, sizeof(*Group));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Group), sizeof(*Group));
	}
};

/////////// ���Ӽ��� ���� ��û �ź�(To Ŭ���̾�Ʈ)
class CMS_REFUSE_DESTSERVERINFO : public CCommMsg
{
public:
	int *Code;
	int *l_StrMsg;
	char *StrMsg;
	void Set(int code, char* strmsg) 
	{
		int l_strmsg = 0;
		if(strmsg) l_strmsg = strlen(strmsg);
	
		SetHeader(MS_REFUSE_DESTSERVERINFO);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushData(_PVAR(l_StrMsg), &l_strmsg, sizeof(*l_StrMsg));
		PushData(_PVAR(StrMsg), strmsg, l_strmsg);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));
		TakeData(_PVAR(l_StrMsg), sizeof(*l_StrMsg));
		TakeData(_PVAR(StrMsg), *l_StrMsg);
	}
};

/////////// ������ ���Ӽ��� ����(To Ŭ���̾�Ʈ)
class CMS_DESTSERVERINFO : public CCommMsg
{
public:

	Ms_DestServerInfo *DI;
	void Set(Ms_DestServerInfo *pDI) 
	{
		SetHeader(MS_DESTSERVERINFO);
		PushData(_PVAR(DI), pDI, sizeof(*DI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(DI), sizeof(*DI));
	}
};


/*
class CMS_OFFER_RESTING_PROMOTION_MONEY : public CCommMsg
{
public:
	PROMOTIONDATA_LOG* WinnerInfo;

	void Set(PROMOTIONDATA_LOG* pWinnerInfo) 
	{
		SetHeader(MS_OFFER_RESTING_PROMOTION_MONEY);
		PushData(_PVAR(WinnerInfo), pWinnerInfo, sizeof(PROMOTIONDATA_LOG));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(WinnerInfo), sizeof(PROMOTIONDATA_LOG));
	}
};
*/

class CMS_GROUP_INSPECTION_INFO : public CCommMsg
{
public:
	Ms_GroupInfo* m_stGroupInfo;

	void Set(Ms_GroupInfo& rGroupInfo) 
	{
		SetHeader(MS_GROUP_INSPECTION_INFO);
		PushData(_PVAR(m_stGroupInfo), &rGroupInfo, sizeof(*m_stGroupInfo));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_stGroupInfo), sizeof(*m_stGroupInfo));
	}
};

////////////////////////////////////////////////////////////////////////////
// ��ʸ�Ʈ ���� ����
////////////////////////////////////////////////////////////////////////////
class CMS_OPENTOURNAMENT_NOTI : public CCommMsg
{
public:
	int* nInning;
	__int64* tStartingTime;
	void Set(const int NInning, const __int64 TStartingTime)
	{
		SetHeader(MS_OPENTOURNAMENT_NOTI);
		PushData(_PVAR(nInning), &NInning, sizeof(*nInning));
		PushData(_PVAR(tStartingTime), &TStartingTime, sizeof(*tStartingTime));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nInning), sizeof(*nInning));
		TakeData(_PVAR(tStartingTime), sizeof(*tStartingTime));
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//                      ������ �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

/////////// ������ üũ��
class CSV_CHECKVERSION : public CCommMsg
{
public:

	int *Ver;
	int *PreVersion;
	void Set(int ver, int prversion) 
	{
		SetHeader(SV_CHECKVERSION);
		PushData(_PVAR(Ver), &ver, sizeof(*Ver));
		PushData(_PVAR(PreVersion), &prversion, sizeof(*PreVersion));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Ver), sizeof(*Ver));
		TakeData(_PVAR(PreVersion), sizeof(*PreVersion));
	}
};

/////////// ���� ����	[���� ������ �۾�]
class CSV_SERVERINFO : public CCommMsg
{
public:

	SERVERINFO *SI;
	void Set(SERVERINFO *pLI) 
	{
		SetHeader(SV_SERVERINFO);
		PushData(_PVAR(SI), pLI, sizeof(SERVERINFO));
		//Encode();
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SI), sizeof(SERVERINFO));
	}
};



// [SSO �۾�] - �߰�
/////////// �α��� ��û ����(ID, Password, Game Version)
class CSV_ASK_LOGIN : public CCommMsg
{
public:

	ASK_LOGININFO *pAL;
	char *AuthCookie;
	char *DataCookie;
	char *CPCookie;
	FOBIS_Common *FOBISCommon;

	void Set(ASK_LOGININFO *pal, char* szAuthCookie, char* szDataCookie, char *szCPCookie, FOBIS_Common *FOBISCom) 
	{
		SetHeader(SV_ASK_LOGIN);
		PushData(_PVAR(pAL), pal, sizeof(ASK_LOGININFO));
		PushData(_PVAR(AuthCookie), szAuthCookie, pal->AuthCookieLen);
		PushData(_PVAR(DataCookie), szDataCookie, pal->DataCookieLen);
		PushData(_PVAR(CPCookie), szCPCookie, pal->CPCookieLen);
		PushData(_PVAR(FOBISCommon), FOBISCom, sizeof(FOBIS_Common));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pAL), sizeof(ASK_LOGININFO));
		TakeData(_PVAR(AuthCookie), pAL->AuthCookieLen);
		TakeData(_PVAR(DataCookie), pAL->DataCookieLen);
		TakeData(_PVAR(CPCookie), pAL->CPCookieLen);
		TakeData(_PVAR(FOBISCommon), sizeof(FOBIS_Common));
	}
};

/////////// �α����� �ź���(�ź� ���� �ڵ带 ����)	[SSO �۾�] - ��ü
class CSV_REFUSE_LOGIN : public CCommMsg
{
public:

	int *Code;
	int *nRetAuthType;
	int *l_StrMsg;
	char *StrMsg;
	void Set(int code, int nAuthtype, char* strmsg) 
	{
		int l_strmsg = 0;
		if(strmsg) l_strmsg = strlen(strmsg);
	
		SetHeader(SV_REFUSE_LOGIN);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushData(_PVAR(nRetAuthType), &nAuthtype, sizeof(*nRetAuthType));
		PushData(_PVAR(l_StrMsg), &l_strmsg, sizeof(*l_StrMsg));
		PushData(_PVAR(StrMsg), strmsg, l_strmsg);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));
		TakeData(_PVAR(nRetAuthType), sizeof(*nRetAuthType));
		TakeData(_PVAR(l_StrMsg), sizeof(*l_StrMsg));
		TakeData(_PVAR(StrMsg), *l_StrMsg);		
	}
};


/////////// �α����� �����(����� ������ ����)
class CSV_ACCEPT_LOGIN : public CCommMsg
{
public:

	LOGININFO *LI;
	void Set(LOGININFO *pLI) 
	{
		SetHeader(SV_ACCEPT_LOGIN);
		PushData(_PVAR(LI), pLI, sizeof(LOGININFO));
		//Encode();
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(LI), sizeof(LOGININFO));
	}
};



/////////// �α׾ƿ��� ��û
class CSV_ASK_LOGOUT : public CCommMsg
{
public:

	int *UNum;
	char *ID;
	void Set(int unum, char* id) 
	{
		char strId[16]={0,};
		if(strlen(id) < 16) strcpy(strId, id);
		SetHeader(SV_ASK_LOGOUT);
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UNum), sizeof(*UNum));		
		TakeData(_PVAR(ID), 16);
	}
};

/////////// �α׾ƿ��� ���
class CSV_ACCEPT_LOGOUT : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(SV_ACCEPT_LOGOUT);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

// ��������(���̷ο�, ����, �Ƕ� �� ������, ������Ʈ, �α׼���)���� ȣȯ���� ���ؼ� ���ܵ�. ���� �����ؾ���.
/////////// ��Ʈ��Ʈ
class CSV_HEARTBEAT : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(SV_HEARTBEAT);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

/////////// ��Ʈ��Ʈ����Ʈ üũ��
class CSV_HEARTBEATGHOSTCHECK : public CCommMsg
{
public:
	UINT64*	m_llTick;			// �������� ���� �ð�

	void Set(UINT64 llTick) 
	{
		SetHeader(SV_HEARTBEATGHOSTCHECK);
		PushData(_PVAR(m_llTick), &llTick, sizeof(*m_llTick));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_llTick), sizeof(*m_llTick));
	}
};

/////////// �ٸ� ������ ������ ä�� ����
class CSV_DESTCHANNELINFO : public CCommMsg
{
public:
	char *IP;
	int  *Port;
	CONNECT_MOVECHANINFO *MI;
	void Set(char* ip, int* port, CONNECT_MOVECHANINFO *pMI) 
	{
		char strIP[20]={0,};
		if(strlen(ip) < 19) strcpy(strIP, ip);
	
		SetHeader(SV_DESTCHANNELINFO);
		PushData(_PVAR(IP), strIP, 20);
		PushData(_PVAR(Port), port, sizeof(*Port));
		PushData(_PVAR(MI), pMI, sizeof(*MI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(IP), 20);
		TakeData(_PVAR(Port), sizeof(*Port));
		TakeData(_PVAR(MI), sizeof(*MI));
	}
};

/////////// �ٸ��������� ä�� �̵����� ���� ��û
class CSV_CONNECT_MOVECHAN : public CCommMsg
{
public:
	CONNECT_MOVECHANINFO *MI;
	void Set(CONNECT_MOVECHANINFO *pMI) 
	{
		SetHeader(SV_CONNECT_MOVECHAN);
		PushData(_PVAR(MI), pMI, sizeof(*MI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(MI), sizeof(*MI));
	}
};

/////////// �ٸ� ������ ������ ä�� ����
class CSV_DESTGAMEINFO : public CCommMsg
{
public:
	int*	m_nGameCode;
	char*	m_szIP;
	int*	m_nPort;
	CONNECT_MOVECHANINFO *MI;
	void Set(int nGameCode, char* ip, int* port, CONNECT_MOVECHANINFO *pMI) 
	{
		char strIP[20]={0,};
		if(strlen(ip) < 19) strcpy(strIP, ip);

		SetHeader(SV_DESTGAMEINFO);
		PushData(_PVAR(m_nGameCode), &nGameCode, sizeof(*m_nGameCode));
		PushData(_PVAR(m_szIP), strIP, 20);
		PushData(_PVAR(m_nPort), port, sizeof(*m_nPort));
		PushData(_PVAR(MI), pMI, sizeof(*MI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameCode), sizeof(*m_nGameCode));
		TakeData(_PVAR(m_szIP), 20);
		TakeData(_PVAR(m_nPort), sizeof(*m_nPort));
		TakeData(_PVAR(MI), sizeof(*MI));
	}
};

/////////// �ٸ��������� ���� �̵����� ���� ��û
class CSV_CONNECT_MOVEGAME : public CCommMsg
{
public:
	int*	m_nGameCode;
	CONNECT_MOVECHANINFO *MI;

	void Set(int nGameCode, CONNECT_MOVECHANINFO *pMI) 
	{
		SetHeader(SV_CONNECT_MOVEGAME);
		PushData(_PVAR(m_nGameCode), &nGameCode, sizeof(*m_nGameCode));
		PushData(_PVAR(MI), pMI, sizeof(*MI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameCode), sizeof(*m_nGameCode));
		TakeData(_PVAR(MI), sizeof(*MI));
	}
};

/////////// ���� ������ ���
class CSV_ACCEPT_MOVEGAME : public CCommMsg
{
public:
	int*	m_nGameCode;
	ENTERCHANINFO *EC;
	void Set(int nGameCode, ENTERCHANINFO *pEC) 
	{
		SetHeader(SV_ACCEPT_MOVEGAME);
		PushData(_PVAR(m_nGameCode), &nGameCode, sizeof(*m_nGameCode));
		PushData(_PVAR(EC), pEC, sizeof(ENTERCHANINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameCode), sizeof(*m_nGameCode));
		TakeData(_PVAR(EC), sizeof(ENTERCHANINFO));
	}
};


/////////// ä�� ������ ��û
class CSV_ASK_ENTERCHAN : public CCommMsg
{
public:

	int *GroupNo;
	int *ChanNo;
	void Set(int group, int channo) 
	{
		SetHeader(SV_ASK_ENTERCHAN);
		PushData(_PVAR(GroupNo), &group, sizeof(*GroupNo));
		PushData(_PVAR(ChanNo), &channo, sizeof(*ChanNo));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(GroupNo), sizeof(*GroupNo));
		TakeData(_PVAR(ChanNo), sizeof(*ChanNo));
	}
};

/////////// ä�� ������ ���
class CSV_ACCEPT_ENTERCHAN : public CCommMsg
{
public:

	ENTERCHANINFO *EC;
	void Set(ENTERCHANINFO *pEC) 
	{
		SetHeader(SV_ACCEPT_ENTERCHAN);
		PushData(_PVAR(EC), pEC, sizeof(ENTERCHANINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(EC), sizeof(ENTERCHANINFO));
	}
};

/////////// ���̵� ã�� ��û

class CSV_ASK_FINDID : public CCommMsg
{
public:

	char *NickName;
	void Set(char* nickname) 
	{
		char strId[20]={0,};
		if(strlen(nickname) < 20) strcpy(strId, nickname);
		SetHeader(SV_ASK_FINDID);
		PushData(_PVAR(NickName), strId, 20);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(NickName), 20);
	}
};

/////////// ���̵� ã�� ���
class CSV_RESULT_FINDID : public CCommMsg
{
public:
	USERINFO	*m_pUI;
	bool			*m_bFound;
	void Set(USERINFO* pUI, bool bFound) 
	{
		SetHeader(SV_RESULT_FINDID);
		PushData(_PVAR(m_pUI), pUI, sizeof(*m_pUI));
		PushData(_PVAR(m_bFound), &bFound, sizeof(*m_bFound));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_pUI), sizeof(*m_pUI));
		TakeData(_PVAR(m_bFound), sizeof(*m_bFound));
	}
};

class CSV_ASK_BADUSER : public CCommMsg
{
public:

	BOOL *bAnnounce;	// �Ű� �˸� ����
	int *Type;			//�Ű�����	//## 112�ű� �߰� ##//
	int *RNum;			// ���ȣ(0�̸� ����)
	int *UNum;			// �Ű��� ������ȣ
	char *ID;			// �Ű��� ���̵�
	char *TargetID;			// �Ű���� ���̵�
	char *TargetNickName;		// �Ű���� �г���
	char *SubmitReason;	// �Ű�����
	int *l_StrWatchID;	// ���� ���̵� ��Ʈ���� ����
	char *WatchID;		// ���� ���̵� ��Ʈ��
	int *l_StrChat;		// �Ű��� ä�� ��Ʈ���� ����
	char *StrChat;		// �Ű��� ä�� ��Ʈ��
	//void Set(BOOL announce, int type, int rnum, int unum, char* id, char *tarnickname, char *reason, char *watchid, char *chat ) 
	void Set(BOOL announce, int ntype, int rnum, int unum, char* id, char* tarid, char *tarnickname, char *reason, char *watchid, char *chat) 
	{
		char strId[16]={0,};
		char strTargetID[16]={0,};
		char strTargetNickName[20]={0,};
		char strReason[64]={0,};
		CString szWatchID, szChat;
		szWatchID.Format("%s", watchid);
		szChat.Format("%s", chat);
		int l_strwatchid = szWatchID.GetLength();
		int l_strchat = szChat.GetLength();
	
		SetHeader(SV_ASK_BADUSER);
	
		if(strlen(id) < 16) strcpy(strId, id);
		if(strlen(tarid) < 16 )	strncpy( strTargetID, tarid, 16 );
		if(strlen(tarnickname) < 20) strcpy(strTargetNickName, tarnickname);
		if(strlen(reason) < 64) strcpy(strReason, reason);
	
		PushData(_PVAR(bAnnounce), &announce, sizeof(*bAnnounce));
		PushData(_PVAR(Type), &ntype, sizeof(*Type));	//�߰� 
		PushData(_PVAR(RNum), &rnum, sizeof(*RNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(TargetID), strTargetID, 16);
		PushData(_PVAR(TargetNickName), strTargetNickName, 20);
		PushData(_PVAR(SubmitReason), strReason, 64);
		PushData(_PVAR(l_StrWatchID), &l_strwatchid, sizeof(l_strwatchid));
		PushData(_PVAR(WatchID), (void*)szWatchID.operator LPCTSTR(), l_strwatchid);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(bAnnounce), sizeof(*bAnnounce));
		TakeData(_PVAR(Type), sizeof(*Type));	
		TakeData(_PVAR(RNum), sizeof(*RNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(TargetID), 16);
		TakeData(_PVAR(TargetNickName), 20);
		TakeData(_PVAR(SubmitReason), 64);
		TakeData(_PVAR(l_StrWatchID), sizeof(*l_StrWatchID));
		TakeData(_PVAR(WatchID), *l_StrWatchID);
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
	}
};

/////////// ��ü ä�� ����� �䱸�Ѵ�
class CSV_ASK_ALLCHANINFO : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(SV_ASK_ALLCHANINFO);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

/////////// ��ü ä�� ����� �����ش�
class CSV_ALLCHANINFO : public CCommMsg
{
public:

	int *TotGroup;
	int *TotChan;
	GROUPINFO *ArrayGI;
	CHANNELINFO *ArrayCI;

	void Set( std::vector< GROUPINFO >& rvtGI, std::vector< CHANNELINFO >& rvtCI )
	{
		SetHeader(SV_ALLCHANINFO);
		
		size_t nTotalgroup = rvtGI.size();
		PushData(_PVAR(TotGroup), &nTotalgroup, sizeof(*TotGroup));

		size_t nTotalchan = rvtCI.size();
		PushData(_PVAR(TotChan), &nTotalchan, sizeof(*TotChan));

		for ( size_t i = 0; i < nTotalgroup; i++ )
		{
			PushData(_PVAR(ArrayGI), &rvtGI[ i ], sizeof( GROUPINFO ) );
		}

		for (  size_t i = 0; i < nTotalchan; i++ )
		{
			PushData(_PVAR(ArrayCI), &rvtCI[ i ], sizeof( CHANNELINFO ) );
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotGroup), sizeof(*TotGroup));
		TakeData(_PVAR(TotChan), sizeof(*TotChan));
		TakeData(_PVAR(ArrayGI), sizeof(GROUPINFO)*(*TotGroup));
		TakeData(_PVAR(ArrayCI), sizeof(CHANNELINFO)*(*TotChan));
	}
};

/////////// ����� ����� �䱸�Ѵ�
class CSV_ASK_ALLUSERINFO : public CCommMsg
{
public:
	int* m_nGameIndex;			// -1: ��ü, 0:7��Ŀ, 1:�ο�ٵ���, 2:����Ŀ, 3:����Ŀ, 4:���̷ο�
	MONEYRANGE* m_stMoneyRange;	// ����� ����� ��û�� �Ӵϱ���

	void Set(int nGameIndex, MONEYRANGE stMoneyRange) 
	{
		SetHeader(SV_ASK_ALLUSERINFO);
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex));
		PushData(_PVAR(m_stMoneyRange), &stMoneyRange, sizeof(*m_stMoneyRange));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex));
		TakeData(_PVAR(m_stMoneyRange), sizeof(*m_stMoneyRange));
	}
};

/////////// ����� ����� �����ش�
class CSV_ALLUSERINFO : public CCommMsg
{
public:
	int* m_nGameIndex;			// -1: ��ü, 0:7��Ŀ, 1:�ο�ٵ���, 2:����Ŀ, 3:����Ŀ, 4:���̷ο�
	MONEYRANGE* m_stMoneyRange;	// ����� �����  �Ӵϱ���
	int *TotNum;
	SMALLUSERINFO *ArraySUI;

	// 500 ������ ����� ����� ��� �����ֱ�
	int* m_nTotalSendCnt;		// �� ���� ��Ŷ����
	int* m_nCurSendCnt;		// ���� ������ �ִ� ��Ŷ�ε���	
	void Set(int nGameIndex, MONEYRANGE stMoneyRange, int totnum, std::vector<SMALLUSERINFO>& rvecSUI, int nTotalSendCnt, int nCurSendCnt)
	{
		SetHeader(SV_ALLUSERINFO);
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex));
		PushData(_PVAR(m_stMoneyRange), &stMoneyRange, sizeof(*m_stMoneyRange));
		PushData(_PVAR(TotNum), &totnum, sizeof(*TotNum));
		int nIndex = (nCurSendCnt - 1) * 500;
		for ( int i = 0 ; i < totnum ; ++i, ++nIndex)
		{
			PushData(_PVAR(ArraySUI), &rvecSUI[nIndex], sizeof(SMALLUSERINFO));
		}
		PushData(_PVAR(m_nTotalSendCnt), &nTotalSendCnt, sizeof(*m_nTotalSendCnt));
		PushData(_PVAR(m_nCurSendCnt), &nCurSendCnt, sizeof(*m_nCurSendCnt));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex));
		TakeData(_PVAR(m_stMoneyRange), sizeof(*m_stMoneyRange));
		TakeData(_PVAR(TotNum), sizeof(*TotNum));
		TakeData(_PVAR(ArraySUI), sizeof(SMALLUSERINFO)*(*TotNum));
		TakeData(_PVAR(m_nTotalSendCnt), sizeof(*m_nTotalSendCnt));
		TakeData(_PVAR(m_nCurSendCnt), sizeof(*m_nCurSendCnt));
	}
};

//////// �ʴ��� ���������� ��û�Ѵ�.
class CCL_ASK_INVITEUSERLIST : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(CL_ASK_INVITEUSERLIST);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_INVITEUSERLIST : public CCommMsg
{
public:
	int *TotNum;
	SMALLUSERINFO *ArraySUI;
	void Set(int totnum, std::vector<SMALLUSERINFO>& rvecSUI) 
	{
		SetHeader(SV_INVITEUSERLIST);
		PushData(_PVAR(TotNum), &totnum, sizeof(*TotNum));
		for ( size_t i = 0 ; i < rvecSUI.size() ; ++i)
		{
			PushData(_PVAR(ArraySUI), &rvecSUI[i], sizeof(SMALLUSERINFO));
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum));
		TakeData(_PVAR(ArraySUI), sizeof(SMALLUSERINFO)*(*TotNum));
	}
};

/////////// Ư�� ������� �� ������ �䱸�Ѵ�
class CSV_ASK_DETAILUSERINFO : public CCommMsg
{
public:

	int *UNum;
	// �г��� �۾�
	//char *ID;
	char *NickName;
	// �������
	void Set(int unum, char* nickname) 
	{
		char strNickName[20]={0,};
		// �г��� �۾�
		if(strlen(nickname) < 20) strcpy(strNickName, nickname);
		// �������

		SetHeader(SV_ASK_DETAILUSERINFO);
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));

		// �г��� �۾�
		PushData(_PVAR(NickName), strNickName, 20);
		// �������

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UNum), sizeof(*UNum));

		// �г��� �۾�
		TakeData(_PVAR(NickName), 20);
		// �������

	}
};

/////////// Ư�� ����� ������ �䱸�Ѵ�
class CSV_ASK_USERINFO : public CCommMsg
{
public:

	UINT64	*m_llUserKey;		// IPS User Key
	char *m_szID;
	char *m_szUniqNo;

	void Set(UINT64 llUserKey, char* id, const char* szUniqNo) 
	{
		char strId[16]={0,};
		if(strlen(id) < 16) strcpy(strId, id);

		char strUniqNo[16]={0,};
		if(strlen(szUniqNo) < 16) strcpy(strUniqNo, szUniqNo);

		SetHeader(SV_ASK_USERINFO);
		PushData(_PVAR(m_llUserKey), &llUserKey, sizeof(*m_llUserKey));
		PushData(_PVAR(m_szID), strId, 16);
		PushData(_PVAR(m_szUniqNo), strUniqNo, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_llUserKey), sizeof(*m_llUserKey));
		TakeData(_PVAR(m_szID), 16);
		TakeData(_PVAR(m_szUniqNo), 16);
	}
};

/////////// Ư�� ����� ������ ������
class CSV_USERINFO : public CCommMsg
{
public:

	USERINFO *UI;
	void Set(USERINFO *pUI) 
	{
		SetHeader(SV_USERINFO);
		PushData(_PVAR(UI), pUI, sizeof(*UI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UI), sizeof(*UI));
	}
};

/////////// ���ο� ����ڰ� ���������� �˸���
class CSV_NEWUSER : public CCommMsg
{
public:

	SMALLUSERINFO *SUI;
	void Set(SMALLUSERINFO* pSUI) 
	{
		SetHeader(SV_NEWUSER);
		PushData(_PVAR(SUI), pSUI, sizeof(SMALLUSERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SUI), sizeof(*SUI));
	}
};

/////////// ����ڰ� ������ ���������� �˸���
class CSV_OUTUSER : public CCommMsg
{
public:

	SMALLUSERINFO *SUI;
	void Set(SMALLUSERINFO* pSUI) 
	{
		SetHeader(SV_OUTUSER);
		PushData(_PVAR(SUI), pSUI, sizeof(SMALLUSERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SUI), sizeof(*SUI));
	}
};

/////////// �ٸ� ������� ���� ������ ������
class CSV_CHANGEUSER : public CCommMsg
{
public:

	SMALLUSERINFO *SUI;
	void Set(SMALLUSERINFO* pSUI) 
	{
		SetHeader(SV_CHANGEUSER);
		PushData(_PVAR(SUI), pSUI, sizeof(SMALLUSERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SUI), sizeof(*SUI));
	}
};

/////////// �� ����� �䱸�Ѵ�
class CSV_ASK_ALLROOMINFO : public CCommMsg
{
public:
	int* m_nGameIndex;				// ����� ����� ��û�� �����ڵ�

	void Set(int nGameIndex) 
	{
		SetHeader(SV_ASK_ALLROOMINFO);
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex));
	}
};

/////////// �� ����� �����ش�
class CSV_ALLROOMINFO : public CCommMsg
{
public:
	int* m_nGameIndex;			// ����� ����� ��û�� �����ڵ�
	int *TotNum;
	SMALLROOMINFO *ArrayRI;

	// 500 ������ ����� ����� ��� �����ֱ�
	int* m_nTotalSendCnt;		// �� ���� ��Ŷ����
	int* m_nCurSendCnt;		// ���� ������ �ִ� ��Ŷ�ε���	

	void Set(int nGameIndex, int totnum, std::vector<SMALLROOMINFO>& rvecRI, int nTotalSendCnt, int nCurSendCnt) 
	{
		SetHeader(SV_ALLROOMINFO);
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex));
		PushData(_PVAR(TotNum), &totnum, sizeof(*TotNum));

		int nIndex = (nCurSendCnt - 1) * 500;
		for (int i = 0 ; i < totnum ; ++i, ++nIndex)
		{
			PushData(_PVAR(ArrayRI), &rvecRI[nIndex], sizeof(SMALLROOMINFO));
		}
		PushData(_PVAR(m_nTotalSendCnt), &nTotalSendCnt, sizeof(*m_nTotalSendCnt));
		PushData(_PVAR(m_nCurSendCnt), &nCurSendCnt, sizeof(*m_nCurSendCnt));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex));
		TakeData(_PVAR(TotNum), sizeof(*TotNum));
		TakeData(_PVAR(ArrayRI), sizeof(SMALLROOMINFO)*(*TotNum));		
		TakeData(_PVAR(m_nTotalSendCnt), sizeof(*m_nTotalSendCnt));
		TakeData(_PVAR(m_nCurSendCnt), sizeof(*m_nCurSendCnt));
	}
};

/////////// �� ������ �䱸
class CSV_ASK_ROOMINFO : public CCommMsg
{
public:
	enum RequestKind
	{
		DisplayRoomInfo = 0,	// ������ ǥ�ÿ뵵
		EnterRoomInfo = 1,		// ������ ��û�뵵
		SearchRoomInfo = 2,		// ��˻� ��û�뵵
		EnterCafeRoomInfo = 3,		// ī��/���չ� ���� ��û�뵵
	};

	int *RoomNum;
	char*	m_cKind;		// ������ ��û����(Ŭ���̾�Ʈ���� ��ü������ ���)
	void Set(int roomnum, char cKind)
	{
		SetHeader(SV_ASK_ROOMINFO);
		PushData(_PVAR(RoomNum), &roomnum, sizeof(*RoomNum));
		PushData(_PVAR(m_cKind), &cKind, sizeof(*m_cKind));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(m_cKind), sizeof(*m_cKind));
	}
};

/////////// �� ����
class CSV_ROOMINFO : public CCommMsg
{
public:

	int *ErrCode;
	char* m_cKind;		// ������ ��û����(Ŭ���̾�Ʈ���� ��ü������ ���)
	GAMEINFO *GI;
	void Set(int errcode, char cKind, GAMEINFO *pGI)
	{
		SetHeader(SV_ROOMINFO);
		PushData(_PVAR(ErrCode), &errcode, sizeof(*ErrCode));
		PushData(_PVAR(m_cKind), &cKind, sizeof(*m_cKind));
		PushData(_PVAR(GI), pGI, sizeof(*GI));
		//Encode();
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ErrCode), sizeof(*ErrCode));
		TakeData(_PVAR(m_cKind), sizeof(*m_cKind));
		TakeData(_PVAR(GI), sizeof(*GI));
	}
};

/////////// ĳ���� �ٲٱ⸦ ��û
class CSV_ASK_CHANGECHAR : public CCommMsg
{
public:

	int *UNum;
	char *ID;
	int *CharNum;
	void Set(int unum, char *id, int charnum) 
	{
		char strID[16];
		strncpy(strID, id, 15);
		SetHeader(SV_ASK_CHANGECHAR);
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strID, 16);
		PushData(_PVAR(CharNum), &charnum, sizeof(*CharNum));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(CharNum), sizeof(*CharNum));
	}
};

/////////// ĳ���� �ٲٱ⸦ ��û ���
class CSV_ACCEPT_CHANGECHAR : public CCommMsg
{
public:

	int *CharNum;
	void Set(int charnum) 
	{
		SetHeader(SV_ACCEPT_CHANGECHAR);
		PushData(_PVAR(CharNum), &charnum, sizeof(*CharNum));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CharNum), sizeof(*CharNum));
	}
};

/////////// �� ���� �䱸
class CSV_ASK_CREATEROOM : public CCommMsg
{
public:

	ROOMINFO *RI;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	bool*	m_bDeafultCreateRoom;
	void Set(ROOMINFO* pRI, int sndfxkind, bool bDeafultCreateRoom = false) 
	{
		SetHeader(SV_ASK_CREATEROOM);
		PushData(_PVAR(RI), pRI, sizeof(ROOMINFO));
		PushData(_PVAR(SndFxKind), &sndfxkind, sizeof(*SndFxKind));
		PushData(_PVAR(m_bDeafultCreateRoom), &bDeafultCreateRoom, sizeof(*m_bDeafultCreateRoom));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RI), sizeof(ROOMINFO));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));
		TakeData(_PVAR(m_bDeafultCreateRoom), sizeof(*m_bDeafultCreateRoom));
	}
};

/////////// �� ���� �ź�
class CSV_REFUSE_CREATEROOM : public CCommMsg
{
public:
	enum ERR_CREATEROOM
	{
		ERR_ROOM_NOFOUND = 0,
		ERR_ROOMIDX_ERR = 1,	
		ERR_NOT_ENOUGH_MONEY = 5,
		ERR_INVALID_GAMEMODE = 6,
		ERR_CEATEROOM_PLAYTIMEOVER = 7,
		ERR_CEATEROOM_DAILYLOSSLIMIT = 8,

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
		ERR_CEATEROOM_GIVEUP_MAXUSER_OVER = 101, //��ǹ� �ִ� �ο��� ��������
		/////////////////////////////////////////

		ERR_FAIL = 999,
		ERR_SUCCESS_ = 1001,
	};

	int *Code;
	void Set(int code) 
	{
		SetHeader(SV_REFUSE_CREATEROOM);
		PushData(_PVAR(Code), &code, sizeof(*Code));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));
	}
};

/////////// �� ���� ���
class CSV_ACCEPT_CREATEROOM : public CCommMsg
{
public:

	ROOMINFO *RI;
	void Set(ROOMINFO* pRI) 
	{
		SetHeader(SV_ACCEPT_CREATEROOM);
		PushData(_PVAR(RI), pRI, sizeof(*RI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RI), sizeof(*RI));
	}
};

/////////// �� ������ ��û
class CSV_ASK_ENTERROOM : public CCommMsg
{
public:

	int *RoomNum;
	UINT64*	m_llRoomKey;			// IPS ��Ű
	int *UNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;
	char *Pass;
	char *Observer;
	int *m_ChipKind;////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	
	void Set(int rnum, UINT64 llRoomKey, int unum, int sndKind, char *id, char *pass=NULL, char observer = 0, int ChipKind = 0) // �߰�
	{
		char strid[16]={0,};
		char strpass[5]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		if(pass != NULL) strncpy(strpass, pass, 4);
		SetHeader(SV_ASK_ENTERROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(m_llRoomKey), &llRoomKey, sizeof(*m_llRoomKey));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // �߰�
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(Pass), strpass, 5);
		PushData(_PVAR(Observer), &observer, sizeof(char));
		PushData(_PVAR(m_ChipKind), &ChipKind, sizeof(*m_ChipKind));////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(m_llRoomKey), sizeof(*m_llRoomKey));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Pass), 5);
		TakeData(_PVAR(Observer), sizeof(char));
		TakeData(_PVAR(m_ChipKind), sizeof(*m_ChipKind));////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	}
};

/////////// ������ �㰡��
class CSV_ACCEPT_ENTERROOM : public CCommMsg
{
public:

	ENTERROOMINFO *EI;
	void Set(ENTERROOMINFO *pEI) //, int fieldnum, BLOCKFIELDINFO *pbf) 
	{
		SetHeader(SV_ACCEPT_ENTERROOM);
		PushData(_PVAR(EI), pEI, sizeof(ENTERROOMINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(EI), sizeof(ENTERROOMINFO));

	}
};



/////////// ���� ��û�� �ź���
class CSV_REFUSE_ENTERROOM : public CCommMsg
{
public:
	int *Code;				// -10 = �����̵��� ������
	INT64 *LimitMoney;

	int* m_nGroupCode;	// �ű� �׷��ڵ�
	int* m_nChanCode;		// �ű� ä���ڵ�

	////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
	int *m_ChipKind; //enum ROOM_CHIPKIND �� ���ǵ� ���� ��
	/////////////////////////////////////////

	void Set(int code, INT64 i64LimitMoney, int nGroupCode, int nChanCode, int ChipKind)  // 0:�ο��ʰ� 1:��й�ȣ�� Ʋ��
	{
		SetHeader(SV_REFUSE_ENTERROOM);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushData(_PVAR(m_nGroupCode), &nGroupCode, sizeof(*m_nGroupCode));
		PushData(_PVAR(m_nChanCode), &nChanCode, sizeof(*m_nChanCode));
		PushData(_PVAR(m_ChipKind), &ChipKind, sizeof(*m_ChipKind));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));	
		TakeData(_PVAR(LimitMoney), sizeof(*LimitMoney));
		TakeData(_PVAR(m_nGroupCode), sizeof(*m_nGroupCode));
		TakeData(_PVAR(m_nChanCode), sizeof(*m_nChanCode));
		TakeData(_PVAR(m_ChipKind), sizeof(*m_ChipKind));
	}
};

/////////// ���ο� ���� �����Ǿ���
class CSV_NEWROOM : public CCommMsg
{
public:

	SMALLROOMINFO *SRI;
	void Set(SMALLROOMINFO* pSRI) 
	{
		SetHeader(SV_NEWROOM);
		PushData(_PVAR(SRI), pSRI, sizeof(*SRI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SRI), sizeof(*SRI));
	}
};

/////////// ���� �������
class CSV_OUTROOM : public CCommMsg
{
public:

	SMALLROOMINFO *SRI;
	void Set(SMALLROOMINFO* pSRI) 
	{
		SetHeader(SV_OUTROOM);
		PushData(_PVAR(SRI), pSRI, sizeof(*SRI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SRI), sizeof(*SRI));
	}
};

/////////// �� ������ ������
class CSV_CHANGEROOM : public CCommMsg
{
public:

	SMALLROOMINFO *SRI;
	void Set(SMALLROOMINFO* pSRI) 
	{
		SetHeader(SV_CHANGEROOM);		
		PushData(_PVAR(SRI), pSRI, sizeof(*SRI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SRI), sizeof(*SRI));
	}
};

/////////// �濡 ����ڰ� �����Ͽ���
class CSV_USERENTERROOM : public CCommMsg
{
public:

	int *RoomNum;
	USERINFO *UI;
	INT64 *llMaxLossMoney;	// �ִ� �ս� ���� �Ӵ� 
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	void Set(int rnum, USERINFO *pUI, INT64 max_loss, IPARRINDEX *pIPA, int sndKind) 
	{
		SetHeader(SV_USERENTERROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UI), pUI, sizeof(USERINFO));
		PushData(_PVAR(llMaxLossMoney), &max_loss, sizeof(*llMaxLossMoney));
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // �߰�

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UI), sizeof(USERINFO));
		TakeData(_PVAR(llMaxLossMoney), sizeof(*llMaxLossMoney));
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
	}
};

/////////// ����ڰ� �濡�� ������
class CSV_USEROUTROOM : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	void Set(int rnum, int unum, char *id, IPARRINDEX *pIPA) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_USEROUTROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};

/////////// ����ڰ� �濡�� �����⸦ ��û
class CSV_ASK_OUTROOM : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	char *Reason;
	void Set(int rnum, int unum, char *id, char Reason_param) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ASK_OUTROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(Reason), &Reason_param, sizeof(*Reason));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Reason), sizeof(*Reason));
	}
};

/////////// ����ڰ� �濡�� �����⸦ �㰡
class CSV_ACCEPT_OUTROOM : public CCommMsg
{
public:
	char *Reason;		//���������� (enum REASON_OUTROOM ����)

	void Set(char Reason_param) 
	{
		SetHeader(SV_ACCEPT_OUTROOM);
		PushData(_PVAR(Reason), &Reason_param, sizeof(*Reason));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Reason), sizeof(*Reason));
	}
};

/////////// ���� ����� ������ �����
class CSV_CHANGEMYINFO : public CCommMsg
{
public:

	CHANGEMYINFO *pCMI;
	void Set(CHANGEMYINFO *pcmi) 
	{
		SetHeader(SV_CHANGEMYINFO);
		PushData(_PVAR(pCMI), pcmi, sizeof(CHANGEMYINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pCMI), sizeof(CHANGEMYINFO));
	}
};

/////////// ���������� ����(������� �����ӿ� �������� ���)
class CSV_ABNORMALOUTROOM : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(SV_ABNORMALOUTROOM);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

////////// ���� ���� �ʴ�â ���� ���� �˸�
class CSV_LOBYOPEN : public CCommMsg
{
public:

	BOOL *bLobyOpen;
	void Set(BOOL blobyopen) 
	{
		SetHeader(SV_LOBYOPEN);
		PushData(_PVAR(bLobyOpen), &blobyopen, sizeof(*bLobyOpen));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(bLobyOpen), sizeof(*bLobyOpen));
	}
};

/////////// ��û �޼����� ����
class CSV_ASK_INVITE : public CCommMsg
{
public:

	UINT64*	m_llTargetUserKey;			// ��� ���� Ű
	char*		m_szTargetNickName;		// ��� ���̵�
	SMALLROOMINFO*	m_pSmallRoomInfo;
	USERINFO*	m_pUserInfo;		// ���� ����� ����
	int*			m_nStrPasswdLen;		// ���н����� ����
	char*			m_szStrPasswd;			// ���н�����
	void Set(UINT64 llTargetUserKey, const char* szTargetNickName, SMALLROOMINFO *pSmallRoomInfo, USERINFO *pUserInfo,  const char *szPass) 
	{
		char strTargetNickName[20]={0,};
		if(strlen(szTargetNickName) < 20) strcpy(strTargetNickName, szTargetNickName);
		CString strPasswd;
		strPasswd.Format("%s", szPass);
		int l_strPasswd = strPasswd.GetLength() + 1;
	
		SetHeader(SV_ASK_INVITE);
		PushData(_PVAR(m_llTargetUserKey), &llTargetUserKey, sizeof(*m_llTargetUserKey));
		PushData(_PVAR(m_szTargetNickName), strTargetNickName, 20);
		PushData(_PVAR(m_pSmallRoomInfo), pSmallRoomInfo, sizeof(*m_pSmallRoomInfo));		
		PushData(_PVAR(m_pUserInfo), pUserInfo, sizeof(*m_pUserInfo));
		PushData(_PVAR(m_nStrPasswdLen), &l_strPasswd, sizeof(*m_nStrPasswdLen));
		PushData(_PVAR(m_szStrPasswd), (void*)strPasswd.operator LPCTSTR(), l_strPasswd);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_llTargetUserKey), sizeof(*m_llTargetUserKey));
		TakeData(_PVAR(m_szTargetNickName), 20);
		TakeData(_PVAR(m_pSmallRoomInfo), sizeof(*m_pSmallRoomInfo));		
		TakeData(_PVAR(m_pUserInfo), sizeof(*m_pUserInfo));
		TakeData(_PVAR(m_nStrPasswdLen), sizeof(*m_nStrPasswdLen));
		TakeData(_PVAR(m_szStrPasswd), *m_nStrPasswdLen);		
	}
};

/////////// ��û �޼����� �ź���
class CSV_REFUSE_INVITE : public CCommMsg
{
public:

	int *TarUNum;		// ��� ���� ��ȣ
	char *TarID;		// ��� ���̵�
	char *MyID;			// ���� ���̵�
	int *l_StrMsg;		// ���Ҹ��� ����
	char *StrMsg;		// ���Ҹ�
	void Set(int targetunum, char* targetid, char *myid, char *strmsg) 
	{
		char strTarId[16]={0,};
		if(strlen(targetid) < 16) strcpy(strTarId, targetid);
		char strMyId[16]={0,};
		if(strlen(myid) < 16) strcpy(strMyId, myid);
		CString szMsg;
		szMsg.Format("%s", strmsg);
		int l_strmsg = szMsg.GetLength();
	
		SetHeader(SV_REFUSE_INVITE);
	
		PushData(_PVAR(TarUNum), &targetunum, sizeof(*TarUNum));
		PushData(_PVAR(TarID), strTarId, 16);
		PushData(_PVAR(MyID), strMyId, 16);
		PushData(_PVAR(l_StrMsg), &l_strmsg, sizeof(*l_StrMsg));
		PushData(_PVAR(StrMsg), (void*)szMsg.operator LPCTSTR(), l_strmsg);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TarUNum), sizeof(*TarUNum));
		TakeData(_PVAR(TarID), 16);
		TakeData(_PVAR(MyID), 16);
		TakeData(_PVAR(l_StrMsg), sizeof(*l_StrMsg));
		TakeData(_PVAR(StrMsg), *l_StrMsg);		
	}
};

/////////// �߹� ��ǥ ��û
class CSV_ASK_BANISHVOTE : public CCommMsg
{
public:

	BANISHVOTEINFO *pBV;
	void Set(BANISHVOTEINFO *pbv) 
	{
		SetHeader(SV_ASK_BANISHVOTE);
		PushData(_PVAR(pBV), pbv, sizeof(BANISHVOTEINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pBV), sizeof(BANISHVOTEINFO));
	}
};

/////////// ���� �߹� ��ǥ ���
class CSV_MYBANISHVOTE : public CCommMsg
{
public:

	BOOL *bAgree;
	void Set(BOOL bagree) 
	{
		SetHeader(SV_MYBANISHVOTE);
		PushData(_PVAR(bAgree), &bagree, sizeof(*bAgree));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(bAgree), sizeof(*bAgree));
	}
};


/////////// ���� �˸� �޼���
class CSV_SERVERMSG : public CCommMsg
{
public:
	enum MSG_KIND
	{
		MSG_TEXT=0,
		MSG_BOX=1,
		MSG_BOTH=2,
		MSG_ALLIN=9,
		MSG_GAME_END=57,
		MSG_PLAYTIMELIMIT= 101, //10�ð� ����
		MSG_BOX_WAIT_FALSE=102,
	};


	int *Kind;				// ����(0:�⺻ ��� 1:�޼��� �ڽ� ���)
	short *l_StrChat;		// ä�� ��Ʈ���� ����
	char *StrChat;			// ä�� ��Ʈ��
	BOOL *bColorAssigned;	// �ؽ�Ʈ �÷��� �����Ǿ��°�?
	COLORREF *Color;		// �ؽ�Ʈ �÷�

	void Set(int kind, const char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0)) 
	{
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();
	
		SetHeader(SV_SERVERMSG);
	
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(bColorAssigned), &bcolorassigned, sizeof(*bColorAssigned));
		PushData(_PVAR(Color), &color, sizeof(*Color));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(bColorAssigned), sizeof(*bColorAssigned));
		TakeData(_PVAR(Color), sizeof(*Color));
	}
};

/////////// �Ӹ� ��� ���� ����
class CSV_NOWHISPER : public CCommMsg
{
public:

	BOOL *bNoWhisper;
	void Set(BOOL bnowhisper) 
	{
		SetHeader(SV_NOWHISPER);
		PushData(_PVAR(bNoWhisper), &bnowhisper, sizeof(*bNoWhisper));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(bNoWhisper), sizeof(*bNoWhisper));
	}
};

/////////// ���ǿ� ä�� ����Ÿ
class CSV_CHATDATA : public CCommMsg
{
public:

	char *ID;			// ���̵� ��Ʈ��
	char* m_szNickName;	// �г���
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��

	void Set(const char* id, const char* szNickName, const char *chat) 
	{
		char strId[16]={0,};
		char strNickName[20]={0,};
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength() + 1;
	
		SetHeader(SV_CHATDATA);
	
		if(strlen(id) < 16) strcpy(strId, id);
		if (strlen(szNickName) < 20) strcpy(strNickName, szNickName);
	
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(m_szNickName), strNickName, 20);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(m_szNickName), 20);
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
	}
};

/////////// �ӼӸ�
class CSV_WHISPERCHAT : public CCommMsg
{
public:
	char*		m_szID;				// ������ ��� ���̵� ��Ʈ��
	char*		m_szNickName;		// ������ ��� �г���
	UINT64*	m_llUserKey;			// ������ ��� ����Ű
	char*		m_szReceiverID;				// �޴� ��� ���̵� ��Ʈ��
	char*		m_szReceiverNickName;	// �޴� ��� �г���
	UINT64*	m_llReceiverUserKey;		// �޴� ��� ����Ű
	short*		m_sStrChatLen;	// ä�� ��Ʈ���� ����
	char*		m_StrChat;		// ä�� ��Ʈ��
	void Set(const char *szID, const char* szNickName, UINT64 llUserKey, const char* szReceiverID, const char* szReceiverNickName, UINT64 llReceiverUserKey, const char* szChat) 
	{
		char strID[16]={0,};
		char strNickName[20]={0,};
		char strReceiverID[16]={0,};
		char strReceiverNickName[20] = {0,};
		CString strChat;
		strChat.Format("%s", szChat);
		short l_strchat = (short) strChat.GetLength() + 1;
	
		SetHeader(SV_WHISPERCHAT);
	
		if(strlen(szID) < 16) strcpy(strID, szID);
		if(strlen(szNickName) < 20 ) strcpy(strNickName, szNickName);
		if(strlen(szReceiverID) < 16 ) strcpy(strReceiverID, szReceiverID);
		if(strlen(szReceiverNickName) < 20 ) strcpy(strReceiverNickName, szReceiverNickName);
	
		PushData(_PVAR(m_szID), strID, 16);
		PushData(_PVAR(m_szNickName), strNickName, 20);
		PushData(_PVAR(m_llUserKey), &llUserKey, sizeof(*m_llUserKey));
		PushData(_PVAR(m_szReceiverID), strReceiverID, 16);
		PushData(_PVAR(m_szReceiverNickName), strReceiverNickName, 20);
		PushData(_PVAR(m_llReceiverUserKey), &llReceiverUserKey, sizeof(*m_llReceiverUserKey));
		PushData(_PVAR(m_sStrChatLen), &l_strchat, sizeof(*m_sStrChatLen));
		PushData(_PVAR(m_StrChat), (void*)strChat.operator LPCTSTR(), *m_sStrChatLen);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_szID), 16);
		TakeData(_PVAR(m_szNickName) , 20 );
		TakeData(_PVAR(m_llUserKey), sizeof(*m_llUserKey));
		TakeData(_PVAR(m_szReceiverID) , 16 );
		TakeData(_PVAR(m_szReceiverNickName) , 20 );
		TakeData(_PVAR(m_llReceiverUserKey), sizeof(*m_llReceiverUserKey));
		TakeData(_PVAR(m_sStrChatLen), sizeof(*m_sStrChatLen));
		TakeData(_PVAR(m_StrChat), *m_sStrChatLen);		   
	}
};

////////// �����߹� ��û	[���� ������ �۾�]
class CSV_ASK_FORCEBANISH : public CCommMsg
{
public:

	char *ID;
	void Set(const char* id) 
	{
		char strId[16]={0,};
		if(strlen(id) < 16) strcpy(strId, id);
		SetHeader(SV_ASK_FORCEBANISH);
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
	}
};

// �����߹� ī��Ʈ Ŭ���̾�Ʈ�� �˷��ֱ�
class CSV_NOTIFY_BANISHCOUNT : public CCommMsg
{
public:

	enum BANISH
	{
		Banish_Vote=0,
		Banish_Force,
	};
	char *BanishCount;
	char *Banish_Type;
	void Set(char nBanishCount , int type = Banish_Vote) 
	{	
		SetHeader(SV_NOTIFY_BANISHCOUNT);
		PushData(_PVAR(BanishCount), &nBanishCount, sizeof(*BanishCount));
		PushData(_PVAR(Banish_Type), &type, sizeof(*Banish_Type));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(BanishCount), sizeof(*BanishCount));
		TakeData(_PVAR(Banish_Type), sizeof(*Banish_Type));
	}
};

////////// ���� ������ ����Ʈ�� ��û
// [���Ӿ�����â ������] 2006.04.28
class CSV_ASK_GAMEITEMLIST : public CCommMsg
{
public:

	int *ItemCode; // 0: ����Ʈ�� ��û, 0�� �ƴϸ� ������ ���
	int *Itemno;
	void Set( int nItemCode = 0, int nItemno = 0 ) 
	{
		SetHeader(SV_ASK_GAMEITEMLIST);
		PushData(_PVAR(ItemCode), &nItemCode, sizeof(int) );
		PushData(_PVAR(Itemno), &nItemno, sizeof(int) );
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ItemCode), sizeof(int) );
		TakeData(_PVAR(Itemno), sizeof(int) );
	}
};


/////////// ���� ������ ����Ʈ�� �����ش�
class CSV_GAMEITEMLIST : public CCommMsg
{
public:

	GAMEITEM_INFO* pIN;
	GAMEITEM *ArrayGI;
	char *StrMsg1;
	char *StrMsg2;

	void Set(GAMEITEM_INFO* pin, GAMEITEM* ppGI, LPCTSTR strmsg1, LPCTSTR strmsg2) 
	{
		SetHeader(SV_GAMEITEMLIST);
		PushData(_PVAR(pIN), pin, sizeof(GAMEITEM_INFO));
		PushData(_PVAR(ArrayGI), ppGI, sizeof(GAMEITEM) * pin->TotNum);
		PushData(_PVAR(StrMsg1), (char*)strmsg1, pin->StrMsgLen1);
		PushData(_PVAR(StrMsg2), (char*)strmsg2, pin->StrMsgLen2);
		//Encode();
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pIN), sizeof(GAMEITEM_INFO));
		TakeData(_PVAR(ArrayGI), sizeof(GAMEITEM) * pIN->TotNum);		
		TakeData(_PVAR(StrMsg1), pIN->StrMsgLen1);		
		TakeData(_PVAR(StrMsg2), pIN->StrMsgLen2);		
	}
};

/////////// ���� �������� ���(��û, ��� ���)
class CSV_USEGAMEITEM : public CCommMsg
{
public:

	USEGAMEITEMINFO *pUG;

	void Set(USEGAMEITEMINFO *pug) 
	{
		SetHeader(SV_USEGAMEITEM);
		PushData(_PVAR(pUG), pug, sizeof(USEGAMEITEMINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(pUG), sizeof(*pUG));		
	}
};

/////////// ���� ������ ������ ���
class CSV_BUYGAMEITEM : public CCommMsg
{
public:

	BUYGAMEITEMINFO *pBI;
	char *StrMsg;

	void Set(BUYGAMEITEMINFO *pbi, LPCTSTR strmsg) 
	{
		SetHeader(SV_BUYGAMEITEM);
		PushData(_PVAR(pBI), pbi, sizeof(BUYGAMEITEMINFO));
		PushData(_PVAR(StrMsg), (void*)strmsg, pbi->StrMsgLen);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pBI), sizeof(BUYGAMEITEMINFO));
		TakeData(_PVAR(StrMsg), pBI->StrMsgLen);
	}
};


/////////////////////////////////////////////////////////////////////////////////////////
// [ ���� ������ ]

class CCL_ITEMRESULT : public CCommMsg
{
public:

	int *RetCode;
	void Set( int recv_code ) 
	{
		SetHeader( CL_ITEMRESULT);
		PushData(_PVAR(RetCode), &recv_code, sizeof(*RetCode) );
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RetCode), sizeof(*RetCode));	
	
	}
};

class CCL_ITEMLIST : public CCommMsg
{
public :

	int*	  Itemcount;
	GAMEITEM* Item;
	int*	  Use;
	void Set( GAMEITEM* recv_item, int itemcount, int recv_use ) 
	{
		SetHeader( CL_ITEMLIST);
		PushData(_PVAR(Itemcount), &itemcount, sizeof(*Itemcount));
		PushData(_PVAR(Item), recv_item, sizeof(GAMEITEM)*itemcount);
		PushData(_PVAR(Use), &recv_use, sizeof(*Use));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Itemcount), sizeof(*Itemcount) );
		TakeData(_PVAR(Item), sizeof(GAMEITEM) * (*Itemcount) );
		TakeData(_PVAR(Use), sizeof(*Use) );
	
	}
};
class CCL_ASK_ITEMLOGIN: public CCommMsg
{
public:

	int*	  Itemcount;
	GAMEITEM* Item;
	void Set( GAMEITEM* recv_item, int itemcount ) 
	{
		SetHeader( CL_ASK_ITEMLOGIN);
		PushData(_PVAR(Itemcount), &itemcount, sizeof(*Itemcount));
		PushData(_PVAR(Item), recv_item, sizeof(GAMEITEM)*itemcount);
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Itemcount), sizeof(*Itemcount) );
		TakeData(_PVAR(Item), sizeof(GAMEITEM) * (*Itemcount) );
	}
};

class CCL_ACCEPT_ITEMLOGIN: public CCommMsg
{
public:

	int*	  Itemcount;
	GAMEITEM* Item;
	void Set( GAMEITEM* recv_item, int itemcount ) 
	{
		SetHeader( CL_ACCEPT_ITEMLOGIN);
		PushData(_PVAR(Itemcount), &itemcount, sizeof(*Itemcount));
		PushData(_PVAR(Item), recv_item, sizeof(GAMEITEM)*itemcount);
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Itemcount), sizeof(*Itemcount) );
		TakeData(_PVAR(Item), sizeof(GAMEITEM) * (*Itemcount) );
	
	}
};



class CCL_ITEMLOGIN : public CCommMsg
{
public :

	int* Index;

	void Set( int index ) 
	{
		SetHeader( CL_ITEMLOGIN);
		PushData(_PVAR(Index), &index, sizeof(*Index));
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Index),sizeof(*Index));
	}
};



/////////// �ٷ� ���� �Ұ�(�����)
class CSV_REFUSE_QUICKSTART : public CCommMsg
{
public:
	enum
	{
		ERR_NOTSATISFIED = 0,	// ã�� ���̾���.
		ERR_ASKOTHERSERVER,	// �ٸ� ������ ���� ������� �ٷ������� ��û�ؾ� ��
		ERR_CREATENEWROOM,	// ���ο� ���� ������.
		ERR_DAILYLOSSLIMIT,
		ERR_PLAYTIMEOVER,
	};

	int *Code;
	int* m_nGameIndex;
	int* m_nChanNo;
	bool*	m_bUseFindRoomOption;		// �ٷΰ��� ��ɼ� ��뿩��, ����: true, �⺻�ٷΰ���: false
	ROOMOPTION* m_stFindRoomOption;
	void Set(int code, int nGameIndex, int nChanNo, bool bUseFindRoomOption, ROOMOPTION& stFindRoomOption)
	{
		SetHeader(SV_REFUSE_QUICKSTART);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex));
		PushData(_PVAR(m_nChanNo), &nChanNo, sizeof(*m_nChanNo));
		PushData(_PVAR(m_bUseFindRoomOption), &bUseFindRoomOption, sizeof(*m_bUseFindRoomOption));
		PushData(_PVAR(m_stFindRoomOption), &stFindRoomOption, sizeof(*m_stFindRoomOption));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));	 
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex));
		TakeData(_PVAR(m_nChanNo), sizeof(*m_nChanNo));
		TakeData(_PVAR(m_bUseFindRoomOption), sizeof(*m_bUseFindRoomOption));
		TakeData(_PVAR(m_stFindRoomOption), sizeof(*m_stFindRoomOption));
	}
};

// Ŭ�󿡰� �ٷΰ��� ����
class CSV_ORDER_QUICKSTART_IN_ROOM : public CCommMsg
{
public:
	INT64 *llRoomKeyPrev;
	ROOMOPTION *roomOption;		// �ٷΰ���� ����� �� �ɼ�
	void Set(INT64 rkey, ROOMOPTION &ro)
	{
		SetHeader(SV_ORDER_QUICKSTART_IN_ROOM);
		PushData(_PVAR(llRoomKeyPrev), &rkey, sizeof(*llRoomKeyPrev));
		PushData(_PVAR(roomOption), &ro, sizeof(*roomOption));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(llRoomKeyPrev), sizeof(*llRoomKeyPrev));
		TakeData(_PVAR(roomOption), sizeof(*roomOption));
	}
};

class CSV_REFUSE_QUICKSTART_IN_ROOM : public CCommMsg
{
public:
	enum
	{
		ERR_ASKOTHERSERVER,	// �ٸ� ������ ���� ������� �ٷ������� ��û�ؾ� ��
		ERR_CREATENEWROOM,	// ���ο� ���� ������.
		ERR_DAILYLOSSLIMIT,
		ERR_PLAYTIMEOVER,
	};

	int *Code;
	INT64 *llRoomKeyPrev;
	int *nChanNo;
	ROOMOPTION *roomOption;
	void Set(int code, INT64 rkey_prev, int chan, ROOMOPTION &ro)
	{
		SetHeader(SV_REFUSE_QUICKSTART_IN_ROOM);
		PushData(_PVAR(Code), &code, sizeof(*Code));
		PushData(_PVAR(llRoomKeyPrev), &rkey_prev, sizeof(*llRoomKeyPrev));
		PushData(_PVAR(nChanNo), &chan, sizeof(*nChanNo));
		PushData(_PVAR(roomOption), &ro, sizeof(*roomOption));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Code), sizeof(*Code));	 
		TakeData(_PVAR(llRoomKeyPrev), sizeof(*llRoomKeyPrev));
		TakeData(_PVAR(nChanNo), sizeof(*nChanNo));
		TakeData(_PVAR(roomOption), sizeof(*roomOption));
	}
};

/////////Ŭ���̾�Ʈ ���� ��Ŷ //////////////////////////////////////////////////////
class CNM_ADMIN_SETTINGMAN : public CCommMsg
{
public:

	enum TYPE_SETTING
	{
		TYPE_SETTING_LOAD,				//Ŭ���̾�Ʈ -> ���� ���� ���� ��û 
		TYPE_SETTING_ASK_SAVE,			//Ŭ���̾�Ʈ -> ���� ���� ��û  (���ÿ�û�� ���޸� ��)
		TYPE_SETTING_BROADCAST,			//������ ���� -> ������ ���ÿ�û (���� ������ �̷������ ��û)
		TYPE_SETTING_NOTIFY_LOAD,			// ���� -> Ŭ���̾�Ʈ ���õǾ��ִ� ���� �˷��� 
		TYPE_SETTING_NOTIFY_SAVE,			// ���� -> Ŭ���̾�Ʈ ���õ�  ���� �˷��� 
	};
	
#define NM_ADMIN_SETTINGMAN_LENBUF (50)
	char *Type;		//0:���ð��������� 1:���ð�������; 
	char *cSection;
	char *cKey;
	char *cValue;

	void Set(TYPE_SETTING SetType, const char *lpSection, const char *lpKey=NULL, const char *lpValue=NULL) 
	{			
		char temp[NM_ADMIN_SETTINGMAN_LENBUF]={0,};
		char cSet= (char) SetType;
		SetHeader(NM_ADMIN_SETTINGMAN);
		
		
		PushData(_PVAR(Type), &cSet, sizeof(*Type));
		
		if (lpSection)
			strncpy(temp, lpSection, NM_ADMIN_SETTINGMAN_LENBUF-1);
		PushData(_PVAR(cSection), temp, NM_ADMIN_SETTINGMAN_LENBUF);
		
		ZeroMemory(temp, sizeof(temp));
		if (lpKey)
			strncpy(temp, lpKey, NM_ADMIN_SETTINGMAN_LENBUF-1);
		PushData(_PVAR(cKey), temp, NM_ADMIN_SETTINGMAN_LENBUF);

		ZeroMemory(temp, sizeof(temp));
		if (lpValue)			
			strncpy(temp, lpValue, NM_ADMIN_SETTINGMAN_LENBUF-1);
		PushData(_PVAR(cValue), temp, NM_ADMIN_SETTINGMAN_LENBUF);
	}
	void Get(char* lpdata, int size) 
	{
		
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Type), sizeof(*Type));
		TakeData(_PVAR(cSection), NM_ADMIN_SETTINGMAN_LENBUF);
		TakeData(_PVAR(cKey), NM_ADMIN_SETTINGMAN_LENBUF);
		TakeData(_PVAR(cValue), NM_ADMIN_SETTINGMAN_LENBUF);
	}
};




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// ���� ������ �۾�

// ���� ������ ��� ä�� ���� ��û
class CCL_ASK_JUMPITEM: public CCommMsg
{
public:

	int*	  Itemcount;
	GAMEITEM* Item;


	void Set( GAMEITEM* recv_item, int itemcount) 
	{
		SetHeader( CL_ASK_JUMPITEM);
		PushData(_PVAR(Itemcount), &itemcount,sizeof(*Itemcount) );
		PushData(_PVAR(Item),recv_item, sizeof(GAMEITEM) * itemcount );
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Itemcount), sizeof(*Itemcount) );
		TakeData(_PVAR(Item), sizeof(GAMEITEM) * (*Itemcount) );
	
	}
};

// ���� ������ ä�� ���� ���
class CCL_ACCEPT_JUMPITEM: public CCommMsg
{
public:

	int*	  Itemcount;
	GAMEITEM* Item;
	ENTERCHANINFO *EC;

	void Set( GAMEITEM* recv_item, int itemcount, ENTERCHANINFO *pEC ) 
	{
		SetHeader( CL_ACCEPT_JUMPITEM);
		PushData(_PVAR(Itemcount),&itemcount, sizeof(*Itemcount) );
		PushData(_PVAR(Item),recv_item, sizeof(GAMEITEM) * itemcount);
		PushData(_PVAR(EC), pEC,sizeof(*EC));
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Itemcount), sizeof(*Itemcount) );
		TakeData(_PVAR(Item), sizeof(GAMEITEM) * (*Itemcount) );
		TakeData(_PVAR(EC), sizeof(ENTERCHANINFO));
	
	}
};

// ���� �������� ������ �α� �Ѵ�
class CCL_JUMPITEM_LOGIN : public CCommMsg
{
public :

	int* Index;

	void Set( int index ) 
	{
		SetHeader( CL_JUMPITEM_LOGIN);
		PushData(_PVAR(Index),&index, sizeof(*Index));
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Index), sizeof(*Index) );
	
	}
};

//  DB �� ��� ���� ������Ʈ �� ����� �˷��ش�.
class CCL_ACCEPT_JUMPITEM_CH: public CCommMsg
{
public:

	int*	  Itemcount;
	GAMEITEM* Item;
	void Set( GAMEITEM* recv_item, int itemcount ) 
	{
		SetHeader( CL_ACCEPT_JUMPITEM_CH);
		PushData(_PVAR(Itemcount),&itemcount, sizeof(*Itemcount));
		PushData(_PVAR(Item), recv_item, sizeof(GAMEITEM)*itemcount );
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Itemcount), sizeof(*Itemcount) );
		TakeData(_PVAR(Item), sizeof(GAMEITEM) * (*Itemcount) );
	
	}
};

// ### [���� �߰� �۾�] ### ��û
class CSV_ASK_CHANGESNDFX : public CCommMsg
{
public:

	int *UNum;
	int *ServPNum;
	int *RoomNum;
	char *ID;
	int *SndFX;
	void Set(int unum, int spnum, int rnum, char *id, int sndfx) 
	{
		char strID[16];
		strncpy(strID, id, 15);
		SetHeader(SV_ASK_CHANGESNDFX);
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ServPNum), &spnum, sizeof(*ServPNum));
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ID), strID, 16);
		PushData(_PVAR(SndFX), &sndfx, sizeof(*SndFX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(SndFX), sizeof(*SndFX));
	}
};


// ### [���� �߰� �۾�] ### ��û ���
class CSV_ACCEPT_CHANGESNDFX : public CCommMsg
{
public:

	int *SndFx;
	void Set(int sndfx) 
	{
		SetHeader(SV_ACCEPT_CHANGESNDFX);
		PushData(_PVAR(SndFx), &sndfx, sizeof(*SndFx));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SndFx), sizeof(*SndFx));
	}
};


// ���� �����ϱ�
class CNM_ADMIN_OUTUSER : public CCommMsg
{
public:

	ADMINMESSAGE *pAMESG;
	void Set(ADMINMESSAGE *padmsg) 
	{		
		SetHeader(NM_ADMIN_OUTUSER);	
		PushData(_PVAR(pAMESG), padmsg, sizeof(*pAMESG));		
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pAMESG), sizeof(*pAMESG));		
	}
};

// �������ϱ�
class CNM_ADMIN_DELROOM : public CCommMsg
{
public:
	
	ADMINMESSAGE *pAMESG;
	short *l_StrMsg;	// �޼��� ����
	char *StrMsg;		// �޼��� 
	void Set(ADMINMESSAGE *pmsg,  char *szMsg)
	{
		short lenmsg = (short)strlen(szMsg)+1;		

		SetHeader(NM_ADMIN_DELROOM);
		PushData(_PVAR(pAMESG), pmsg, sizeof(ADMINMESSAGE));
		PushData(_PVAR(l_StrMsg), &lenmsg, sizeof(*l_StrMsg));
		PushData(_PVAR(StrMsg), szMsg, *l_StrMsg);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pAMESG), sizeof(ADMINMESSAGE));
		TakeData(_PVAR(l_StrMsg),sizeof(*l_StrMsg));
		TakeData(_PVAR(StrMsg),*l_StrMsg);		
	}
};

/////////// ������ �ӼӸ�
class CNM_ADMIN_WHISPERCHAT : public CCommMsg
{
public:

	char *ID;			// ������ ��� ���̵� ��Ʈ��
	int  *DestUNum;		// �޴� ��� ����� ��ȣ
	char *DestNickName;		// �޴� ��� ���̵�
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
	void Set( const char *id, int destunum, const char *destnick,  char *chat)
	{
		char strId[16]={0,};		
		if(strlen(id) < 16) strcpy(strId, id);
		short l_strchat = (short)strlen(chat);	

		char strDestNickName[20]={0,};
		if(strlen(destnick) < 20) strcpy(strDestNickName, destnick);

		SetHeader(NM_ADMIN_WHISPERCHAT);
		PushData(_PVAR(ID),strId, 16);
		PushData(_PVAR(DestUNum),&destunum, sizeof(*DestUNum));
		PushData(_PVAR(DestNickName),strDestNickName, 20);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(*l_StrChat));
		PushData(_PVAR(StrChat), chat, *l_StrChat);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID),16);
		TakeData(_PVAR(DestUNum),sizeof(*DestUNum));
		TakeData(_PVAR(DestNickName),20);
		TakeData(_PVAR(l_StrChat),sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat),*l_StrChat);
	}

};

// ���� �����ϱ�
class CNM_ADMIN_CHANGEROOM : public CCommMsg
{
public:

	ADMINMESSAGE *pAMESG;
	STYLEROOMINFO *STYLE;
	void Set(ADMINMESSAGE *padmsg, STYLEROOMINFO *pmsg) 
	{		
		SetHeader(NM_ADMIN_CHANGEROOM);	
		PushData(_PVAR(pAMESG), padmsg, sizeof(*pAMESG));
		PushData(_PVAR(STYLE), pmsg, sizeof(*STYLE));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pAMESG), sizeof(*pAMESG));
		TakeData(_PVAR(STYLE), sizeof(*STYLE));
	}
};

/////////// �� ���� ���� ��û �˸�
class CSV_CHANGESTYLEROOMINFO : public CCommMsg
{
public:

	CHANGESTYLEROOMINFO *SRI;
	void Set(CHANGESTYLEROOMINFO* pSRI) 
	{
		SetHeader(SV_CHANGESTYLEROOMINFO);
		PushData(_PVAR(SRI), pSRI, sizeof(CHANGESTYLEROOMINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SRI), sizeof(CHANGESTYLEROOMINFO) );
	}
};

// ����� ����ֱ�
class CNM_SPECIALMESSAGE : public CCommMsg
{
public:

	ADMINMESSAGE *pAMESG;	
	void Set(ADMINMESSAGE *pmsg) 
	{		
		SetHeader(NM_SPECIALMESSAGE);	
		PushData(_PVAR(pAMESG), pmsg, sizeof(*pAMESG));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pAMESG), sizeof(*pAMESG));	
	}
};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////
/////////////////// ### [ ������� ] ###  ///////////////////

/////////// ������ ��û
class CSV_ASK_PARTICIPATION : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	int *SndFX;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	void Set(int rnum, int unum, int servpnum, char *id, int sndfx) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ASK_PARTICIPATION);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(SndFX), &sndfx, sizeof(*SndFX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(SndFX), sizeof(*SndFX));
	}
};


//////////// ������ ����
class CSV_ACCEPT_PARTICIPATION : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;
	USERINFO *UI;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	void Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, USERINFO *ui, IPARRINDEX *pIPA) // �߰�
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ACCEPT_PARTICIPATION);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(NowPNum), &nowpnum, sizeof(*NowPNum));
		PushData(_PVAR(NewPNum), &newpnum, sizeof(*NewPNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // �߰�
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(UI), ui, sizeof(USERINFO));
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(NowPNum), sizeof(*NowPNum));
		TakeData(_PVAR(NewPNum), sizeof(*NewPNum));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(UI), sizeof(USERINFO));
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};

//////////// ������ ����
class CSV_ENTER_PARTICIPATION : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;
	USERINFO *UI;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	void Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, USERINFO *ui, IPARRINDEX *pIPA) // �߰�
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ENTER_PARTICIPATION);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(NowPNum), &nowpnum, sizeof(*NowPNum));
		PushData(_PVAR(NewPNum), &newpnum, sizeof(*NewPNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // �߰�
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(UI), ui, sizeof(USERINFO));
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(NowPNum), sizeof(*NowPNum));
		TakeData(_PVAR(NewPNum), sizeof(*NewPNum));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(UI), sizeof(USERINFO));
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};

/////////// ������ ��û
class CSV_ASK_EMERAGE : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	void Set(int rnum, int unum, int servpnum, char *id) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ASK_EMERAGE);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(ID), 16);
	}
};


//////////// ���� ����
class CSV_ACCEPT_EMERAGE : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	void Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA) // �߰�
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ACCEPT_EMERAGE);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(NowPNum), &nowpnum, sizeof(*NowPNum));
		PushData(_PVAR(NewPNum), &newpnum, sizeof(*NewPNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(NowPNum), sizeof(*NowPNum));
		TakeData(_PVAR(NewPNum), sizeof(*NewPNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};

//////////// ���� �������� �ٸ��������� �˸���
class CSV_ENTER_EMERAGE : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	void Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA) // �߰�
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ENTER_EMERAGE);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(NowPNum), &nowpnum, sizeof(*NowPNum));
		PushData(_PVAR(NewPNum), &newpnum, sizeof(*NewPNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(NowPNum), sizeof(*NowPNum));
		TakeData(_PVAR(NewPNum), sizeof(*NewPNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};


/////////// ������ ���� ����
class CSV_RESERVATIONINFO : public CCommMsg
{
public:

	int *TotNum;
	RESERVATIONINFO *ReservedInfo;
	void Set(int totnum, RESERVATIONINFO* reservation) 
	{
		SetHeader(SV_RESERVATIONINFO);
		PushData(_PVAR(TotNum), &totnum, sizeof(*TotNum));
		PushData(_PVAR(ReservedInfo), reservation, sizeof(RESERVATIONINFO)*totnum);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotNum), sizeof(*TotNum));
		TakeData(_PVAR(ReservedInfo), sizeof(*ReservedInfo)*(*TotNum));
		
	}
};

/////////// �������� �˸� �޼���
class CSV_OBSERVER_MESSAGE : public CCommMsg
{
public:

	int *Kind;			// ����(0:�⺻ ��� 1:�޼��� �ڽ� ���)
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��

	void Set(int kind, char *chat) 
	{
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();
	
		SetHeader(SV_OBSERVER_MESSAGE);
	
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
	}
};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/////////// ���� ���� �޼���
class CSV_SERVERNOTIC : public CCommMsg
{
public:

	int *Kind;				// ����(0:�⺻ ��� 1:�޼��� �ڽ� ���)
	short *l_StrChat;		// ä�� ��Ʈ���� ����
	char *StrChat;			// ä�� ��Ʈ��
	BOOL *bColorAssigned;	// �ؽ�Ʈ �÷��� �����Ǿ��°�?
	COLORREF *Color;		// �ؽ�Ʈ �÷�

	void Set(int kind, char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0)) 
	{
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();
	
		SetHeader(SV_SERVERNOTIC);
	
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(bColorAssigned), &bcolorassigned, sizeof(*bColorAssigned));
		PushData(_PVAR(Color), &color, sizeof(*Color));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(bColorAssigned), sizeof(*bColorAssigned));
		TakeData(_PVAR(Color), sizeof(*Color));
	}
};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


/////////// �� ���� ���� ��û
class CCL_ASK_CHSTYLEROOMINFO : public CCommMsg
{
public:

	STYLEROOMINFO *SRI;
	void Set(STYLEROOMINFO* pSRI) 
	{
		SetHeader(CL_ASK_CHSTYLEROOMINFO);
		PushData(_PVAR(SRI),pSRI, sizeof(*SRI));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SRI),sizeof(*SRI));
	}
};

// [���ǵ���] 2004.09.01
class CSV_SPEEDHACK_DEFENSE : public CCommMsg
{
public:

	UINT *Time;
	void Set(int time) 
	{
		SetHeader(SV_SPEEDHACK_DEFENSE);
		PushData(_PVAR(Time), &time, sizeof(*Time));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Time), sizeof(*Time));
	}
};


// [DB��� Process] 2004.11.29
class CSV_ASK_GAME_CONTINUE : public CCommMsg
{
public:

	char *ID;
	int  *ViewDlg;
	void Set(char *id, int viewDlg) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ASK_GAME_CONTINUE);
		PushData(_PVAR(ViewDlg), &viewDlg, sizeof(*ViewDlg));
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ViewDlg), sizeof(*ViewDlg));
		TakeData(_PVAR(ID), 16);
	}
};

// [DB��� Process] 2004.11.29
//G_TFT Ŭ���� ��ü 
class CSV_RES_GAME_CONTINUE : public CCommMsg
{
public:
	INT64* llMoney;
	INT64* llRealPMoney;
	char *ID;

	// ��޺���[���κ��谳��]
	INT64* GradeInsuMoney;
	int* GradeInsuKind;
	
	INT64* MoneyRecord;//G_TFT

	// MaxRoom. 2011.01.27
	INT64* m_llInMoney;
	INT64* m_llSafeMoney;
	
	void Set(char *id, INT64 money, INT64 realPMoney, INT64 gradeInsuMoney, int gradeInsuKind, INT64 moneyrecord, INT64 llInMoney, INT64 llSafeMoney) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_RES_GAME_CONTINUE);
		PushData(_PVAR(llMoney), &money, sizeof(*llMoney));
		PushData(_PVAR(llRealPMoney), &realPMoney, sizeof(*llRealPMoney));
		PushData(_PVAR(ID), strid, 16);
		// ��޺���[���κ��谳��]
		PushData(_PVAR(GradeInsuMoney), &gradeInsuMoney, sizeof(*GradeInsuMoney));
		PushData(_PVAR(GradeInsuKind), &gradeInsuKind, sizeof(*GradeInsuKind));
		PushData(_PVAR(MoneyRecord), &moneyrecord, sizeof(*MoneyRecord));//G_TFT

		PushData(_PVAR(m_llInMoney), &llInMoney, sizeof(*m_llInMoney));
		PushData(_PVAR(m_llSafeMoney), &llSafeMoney, sizeof(*m_llSafeMoney));

	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(llMoney), sizeof(*llMoney));
		TakeData(_PVAR(llRealPMoney), sizeof(*llRealPMoney));
		TakeData(_PVAR(ID), 16);
		// ��޺���[���κ��谳��]
		TakeData(_PVAR(GradeInsuMoney), sizeof(*GradeInsuMoney));
		TakeData(_PVAR(GradeInsuKind), sizeof(*GradeInsuKind));
		TakeData(_PVAR(MoneyRecord), sizeof(*MoneyRecord));//G_TFT

		TakeData(_PVAR(m_llInMoney), sizeof(*m_llInMoney));
		TakeData(_PVAR(m_llSafeMoney), sizeof(*m_llSafeMoney));
	}
};

/////////////////////////////////////���� ������ //////////////////////// 

// Ư���� �������� �� �� �ִٰ� �˷���
class CSV_ALLOW_BUYITEM : public CCommMsg
{
public:

	char  *ID;
	short *ItemCode;
	short *GameCode;
	SYSTEMTIME *RecvTime;
	SYSTEMTIME *ExpireTime;
	short *ViewItemCode;

	void Set(char *GetID, short r_ItemCode, short r_GameCode, SYSTEMTIME r_RecvTime, SYSTEMTIME r_ExpireTime, short r_ViewItemCode )
	{
		SetHeader( SV_ALLOW_BUYITEM );
		PushData( _PVAR(ID), GetID, 16);
		PushData( _PVAR(RecvTime), &r_RecvTime, sizeof(*RecvTime) );
		PushData( _PVAR(ExpireTime), &r_ExpireTime, sizeof(*ExpireTime) );
		PushData( _PVAR(ItemCode), &r_ItemCode, sizeof(*ItemCode) );
		PushData( _PVAR(GameCode), &r_GameCode, sizeof(*GameCode) );
		PushData( _PVAR(ViewItemCode), &r_ViewItemCode, sizeof(*ViewItemCode) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(ID), 16);
		TakeData( _PVAR(RecvTime), sizeof(*RecvTime) );
		TakeData( _PVAR(ExpireTime), sizeof(*ExpireTime) );
		TakeData( _PVAR(ItemCode), sizeof(*ItemCode) );
		TakeData( _PVAR(GameCode), sizeof(*GameCode) );
		TakeData( _PVAR(ViewItemCode), sizeof(*ViewItemCode) );
	}
};

// Ư���� ������ url �Ķ���� �� ��û
class CSV_ASK_BUYITEM : public CCommMsg
{
public:
	
	short *ItemCode;
	short *GameCode;
	SYSTEMTIME *RecvTime;

	void Set( short r_ItemCode, short r_GameCode, SYSTEMTIME r_RecvTime )
	{
		SetHeader( SV_ASK_BUYITEM );
		PushData( _PVAR(RecvTime), &r_RecvTime, sizeof(*RecvTime) );
		PushData( _PVAR(GameCode), &r_GameCode, sizeof(*GameCode) );
		PushData( _PVAR(ItemCode), &r_ItemCode, sizeof(*ItemCode) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(RecvTime), sizeof(*RecvTime) );
		TakeData( _PVAR(GameCode), sizeof(*GameCode) );
		TakeData( _PVAR(ItemCode), sizeof(*ItemCode) );
	}
};


// ��û�� �������� �� �� �ִ� �Ķ���� �ּ�
class CSV_BUYITEMPARAM : public CCommMsg
{
public:
	
	short *l_StrParam;	// ��Ʈ���� ����
	char *StrParam;		// �Ķ���� ��Ʈ��
	short *GameCode;
	short *ItemCode;
	SYSTEMTIME *ItemTime;
	short	*BuySpecialItemCnt;	//[�����]2007.10.17

	void Set( SYSTEMTIME r_ItemTime , short r_ItemCode , short r_GameCode , char* param ,short buyitemcnt)
	{
		CString szParam = _T("");
		szParam.Format("%s", param);
		short paramlen = szParam.GetLength();

		SetHeader( SV_BUYITEMPARAM );
		PushData( _PVAR(ItemTime), &r_ItemTime, sizeof(*ItemTime) );
		PushData( _PVAR(ItemCode), &r_ItemCode, sizeof(*ItemCode) );
		PushData( _PVAR(GameCode), &r_GameCode, sizeof(*GameCode) );
		PushData(_PVAR(l_StrParam), &paramlen, sizeof(*l_StrParam));
		PushData(_PVAR(StrParam), param, paramlen);
		PushData(_PVAR(BuySpecialItemCnt), &buyitemcnt, sizeof(*BuySpecialItemCnt));
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(ItemTime), sizeof(*ItemTime) );
		TakeData( _PVAR(ItemCode), sizeof(*ItemCode) );
		TakeData( _PVAR(GameCode), sizeof(*GameCode) );
		TakeData( _PVAR(l_StrParam), sizeof(*l_StrParam) );
		TakeData( _PVAR(StrParam), *l_StrParam );
		TakeData( _PVAR(BuySpecialItemCnt), sizeof(*BuySpecialItemCnt) );
	}
};


// ������ ��û ������ �����.
class CSV_BUYITEM_RESULT : public CCommMsg
{
public:

	enum 
	{
		ERR_REWARDITEM_NONEXIST = 0,	// �ش� �������� �������� ����.
		ERR_REWARDITEM_EXPIRED,			// ������ ������ �̴�.
	};
	
	short *GameCode;
	short *ItemCode;
	int   *ErrCode;

	void Set( short r_GameCode , short r_ItemCode , int r_ErrCode )
	{
		SetHeader( SV_BUYITEM_RESULT );
		PushData( _PVAR(GameCode), &r_GameCode, sizeof(*GameCode) );
		PushData( _PVAR(ItemCode), &r_ItemCode, sizeof(*ItemCode) );
		PushData( _PVAR(ErrCode), &r_ErrCode, sizeof(*ErrCode) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(GameCode), sizeof(*GameCode) );
		TakeData( _PVAR(ItemCode), sizeof(*ItemCode) );
		TakeData( _PVAR(ErrCode), sizeof(*ErrCode) );
	}
};


// ������ ���� ���� üũ
class CSV_ASK_CHECK_REWARD : public CCommMsg
{
public:

	int			*ItemCode;
	SYSTEMTIME	*RecvTime;
	
	void Set( int r_ItemCode, SYSTEMTIME	r_RecvTime )
	{
		SetHeader( SV_ASK_CHECK_REWARD );
		PushData( _PVAR(RecvTime), &r_RecvTime, sizeof(*RecvTime) );
		PushData( _PVAR(ItemCode), &r_ItemCode, sizeof(*ItemCode) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(RecvTime), sizeof(*RecvTime) );
		TakeData( _PVAR(ItemCode), sizeof(*ItemCode) );
	}
};


// ���� ���� üũ ���
class CSV_REWARD_CHECK_RESULT : public CCommMsg
{
public:

	INT64 *Money;
	int	  *PurchaseResult;
	int	  *ItemCode;
	SYSTEMTIME *RecvTime;

	void Set( int  r_ItemCode, SYSTEMTIME r_RecvTime, int r_PurchaseResult , INT64 r_Money )
	{
		SetHeader( SV_REWARD_CHECK_RESULT );
		PushData( _PVAR(ItemCode), &r_ItemCode, sizeof(*ItemCode) );
		PushData( _PVAR(RecvTime), &r_RecvTime, sizeof(*RecvTime) );
		PushData( _PVAR(PurchaseResult), &r_PurchaseResult, sizeof(*PurchaseResult) );
		PushData( _PVAR(Money), &r_Money, sizeof(*Money) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(ItemCode), sizeof(*ItemCode) );
		TakeData( _PVAR(RecvTime), sizeof(*RecvTime) );
		TakeData( _PVAR(PurchaseResult), sizeof(*PurchaseResult) );
		TakeData( _PVAR(Money), sizeof(*Money) );
	}
};



// reward item db 08.02.27
// DB����� ���������� ����Ʈ ������
class CSV_REWARDITEM_LIST : public CCommMsg
{
public:
	
	char *count;
	REWARD_GAMEITEM_INFO *pRewardItem;

	void Set( int totnum, REWARD_GAMEITEM_INFO *pRewardItemList  )
	{
		SetHeader( SV_REWARDITEM_LIST );
		PushData( _PVAR(count), &totnum, sizeof(*count) );
		PushData( _PVAR(pRewardItem), pRewardItemList, sizeof(REWARD_GAMEITEM_INFO)*totnum);		
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(count), sizeof(*count) );
		TakeData( _PVAR(pRewardItem), sizeof(*pRewardItem)*(*count) );	
	}
};


// ���κ��� ���� ��û
class CSV_AIN_INSURANCE_JOIN_REQUEST : public CCommMsg
{
public:	
	AIN_INSURANCE_JOIN_REQUEST *AIN_DATA;

	void Set(AIN_INSURANCE_JOIN_REQUEST *pAindata) 
	{
		SetHeader(SV_AIN_INSURANCE_JOIN_REQUEST);
		PushData(_PVAR(AIN_DATA), pAindata, sizeof(AIN_INSURANCE_JOIN_REQUEST));	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;	
		TakeData(_PVAR(AIN_DATA), sizeof(AIN_INSURANCE_JOIN_REQUEST));
	}
};

// ���κ��� ���� ��û
class CSV_AIN_INSURANCE_SUPPLY_REQUEST : public CCommMsg
{
public:	
	AIN_INSURANCE_SUPPLY_REQUEST *AIN_DATA;

	void Set(AIN_INSURANCE_SUPPLY_REQUEST *pAindata) 
	{
		SetHeader(SV_AIN_INSURANCE_SUPPLY_REQUEST);
		PushData(_PVAR(AIN_DATA), pAindata, sizeof(AIN_INSURANCE_SUPPLY_REQUEST));	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;	
		TakeData(_PVAR(AIN_DATA), sizeof(AIN_INSURANCE_SUPPLY_REQUEST));
	}
};

// ���κ��� ���� ��û ��� �޼���
class CSV_AIN_INSURANCE_JOIN_RESULT : public CCommMsg
{
public:	
	AIN_INSURANCE_JOIN_RESULT *AIN_DATA;

	void Set(AIN_INSURANCE_JOIN_RESULT *pAindata) 
	{
		SetHeader(SV_AIN_INSURANCE_JOIN_RESULT);
		PushData(_PVAR(AIN_DATA), pAindata, sizeof(AIN_INSURANCE_JOIN_RESULT));	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;	
		TakeData(_PVAR(AIN_DATA), sizeof(AIN_INSURANCE_JOIN_RESULT));
	}
};

// ���κ��� ���� ��� �޼���
class CSV_AIN_INSURANCE_SUPPLY_RESULT : public CCommMsg
{
public:	
	AIN_INSURANCE_SUPPLY_RESULT *AIN_DATA;

	void Set(AIN_INSURANCE_SUPPLY_RESULT *pAindata) 
	{
		SetHeader(SV_AIN_INSURANCE_SUPPLY_RESULT);
		PushData(_PVAR(AIN_DATA), pAindata, sizeof(AIN_INSURANCE_SUPPLY_RESULT));	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;	
		TakeData(_PVAR(AIN_DATA), sizeof(AIN_INSURANCE_SUPPLY_RESULT));
	}
};


//[���ƹ�Ÿ] 2006.07.24
// �����Ͱ� �� �����鿡�� ���ƹ�Ÿ �Ⱦ� ����� �����Ѵ�.	
class CMS_SEND_SUPPLY_LUKCAVATAINFO : public CCommMsg
{
public:
	LUCKAVATA_DATA *pLUCKAVTAINFO;

	void Set(LUCKAVATA_DATA *pdata) 
	{		
		SetHeader(MS_SEND_SUPPLY_LUKCAVATAINFO);			
		PushData(_PVAR(pLUCKAVTAINFO), pdata, sizeof(*pLUCKAVTAINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(pLUCKAVTAINFO), sizeof(*pLUCKAVTAINFO));
	}
};	


// ���Ӽ����� �ȾҴٸ� �����Ϳ��� �����ش� � �ȾҴٰ�
class CMS_RESULT_LUCKAVATAIFO : public CCommMsg
{
public:
	
	int *nSaleCnt;		//�Ǹŵ� ����
	void Set(int salecnt) 
	{		
		SetHeader(MS_RESULT_LUCKAVATAIFO);
		PushData(_PVAR(nSaleCnt), &salecnt, sizeof(*nSaleCnt));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nSaleCnt), sizeof(*nSaleCnt));
	}
};

//[���ƹ�Ÿ] 2006.07.24
// �������� ���ƹ�Ÿ�� ���� ���� ���
class CSV_RESULT_BUYLUCKAVATAITEM : public CCommMsg
{
public:	
	char *TargetID;
	int	 *Unum;
	void Set(char *id,int unum) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_RESULT_BUYLUCKAVATAITEM);
		PushData(_PVAR(TargetID), strid, 16);
		PushData(_PVAR(Unum), &unum, sizeof(*Unum));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TargetID), 16);
		TakeData(_PVAR(Unum), sizeof(*Unum));
	}
};

// [������� �۾�] [2009-02-10]
class CCL_ASK_RECOVERY_CHARGE : public CCommMsg
{
public:

	enum USEKIND
	{
		HALF = 1,		// �ι��� �ޱ�
		ALLIN = 2,		// �ѹ��� �ޱ�
	};
	
	// �ѹ��� �ޱ�, �ι��� �ޱ�
	int *UseKind;

	void Set( int r_UseKind )
	{
		SetHeader( CL_ASK_RECOVERY_CHARGE );
		PushData( _PVAR(UseKind), &r_UseKind, sizeof(*UseKind) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(UseKind), sizeof(*UseKind) );
	}
};

class CCL_RECOVERY_CHARGE_INFO : public CCommMsg
{
public:
	

	INT64 *RecoveryAmount;
	DATE *Time;
	int *RemainUseCnt;
	int *Result;

	enum CHARGE_RESULT
	{
		FAILED_OVERMONEY = -4,			// ������ �����Ӵ� �߻� �Ҽ� �ִ�.
		PROHIBITED_TERM = -3,			// �Ŵ� 1�� 00~ 17�ÿ��� ������� ���Ѵ�.
		RUN_OUT_OF_USETIME = -2,
		DBWORK_BUSY = -1,
		MY_CHARGE_INFO = 0,
		SUCCESS = 1,				// ������ ������ ���
		RESERVED_SUCCESS = 2,	// ���޿����� �Ѱ��
		MAX_CHARGE_RESULT,
	};

	void Set( int r_Result , int r_RemainUseCnt , DATE r_Time , INT64 r_RecoveryAmount )
	{
		SetHeader( CL_RECOVERY_CHARGE_INFO );
		PushData( _PVAR(Result), &r_Result, sizeof(*Result) );
		PushData( _PVAR(RemainUseCnt), &r_RemainUseCnt, sizeof(*RemainUseCnt) );
		PushData( _PVAR(Time), &r_Time, sizeof(*Time) );
		PushData( _PVAR(RecoveryAmount), &r_RecoveryAmount, sizeof(*RecoveryAmount) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(Result), sizeof(*Result) );
		TakeData( _PVAR(RemainUseCnt), sizeof(*RemainUseCnt) );
		TakeData( _PVAR(Time), sizeof(*Time) );
		TakeData( _PVAR(RecoveryAmount), sizeof(*RecoveryAmount) );
	}
};




//���ȸ�� �ɼǻ�ǰ
class CCL_REPLY_KISACHARGE : public CCommMsg
{
public:
	char *bCharged;
	
	void Set( char _bCharged )
	{
		SetHeader( CL_REPLY_KISACHARGE );	
		PushData( _PVAR(bCharged), &_bCharged, sizeof(*bCharged) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(bCharged), sizeof(*bCharged) );
	}
};

//���ȸ�� �ɼǻ�ǰ //����â ����ֱ� 
class CSV_NOTIFY_ALLIN : public CCommMsg
{
public:
	enum ALLIN_DIALOG_KIND
	{
		ALLIN_DIALOG_KIND_FREE_CHARGE=0,
		ALLIN_DIALOG_KIND_KISA_CHARGE, //���ȸ�� â ���� 
		ALLIN_DIALOG_KIND_ALLIN_INSUARANCE, //���κ��� â ����
		ALLIN_DIALOG_KIND_GOLDENCHIP, //���Ĩ����Ʈ (�Ӵ� ��ȯ)
		ALLIN_DIALOG_KIND_GOLDENCHIP_FAIL,//���Ĩ����Ʈ (���Ĩ DB ���� )
	};

	int *nAllinDlg;		//����� ���� ����â ����
	
	void Set( ALLIN_DIALOG_KIND AllinDlgKind )
	{
		int temp=AllinDlgKind;
		SetHeader( SV_NOTIFY_ALLIN );	
		PushData( _PVAR(nAllinDlg), &temp, sizeof(*nAllinDlg) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(nAllinDlg), sizeof(*nAllinDlg) );
	}
};

/////////// �ٷ� ���� �� ������ ��û
enum
{
	ECALL_BTNTYPE_UNKNOWN = 0,
	ECALL_BTNTYPE_LOBY_QUICK,	 // �κ� �ٷν��۹�ư
	ECALL_BTNTYPE_DETAIL_SETUP, // �󼼺��� ����
	ECALL_BTNTYPE_QUICK_IN_ROOM, // �ٸ��� �ٷΰ���
};
class CSV_ASK_QUICKSTART : public CCommMsg
{
public:
	
	int *UNum;
	char *ID;
	int *SndFxKind;					// ### [���� �߰� �۾�] ###
	int *RoomType;					// 0 - ������, 1 - �Ϲݹ� 
	bool*	m_bUseFindRoomOption;	// �ٷΰ��� ��ɼ� ��뿩��, ����: true, �⺻�ٷΰ���: false
	ROOMOPTION* m_stFindRoomOption; //
	int *nCallBtnType;				// �ٷν��� ����( �κ� �ٷν��� ��ư = ECALL_BTNTYPE_LOBY_QUICK, �󼼺��� ���� = ECALL_BTNTYPE_DETAIL_SETUP )
	bool* bForceLocalServer;		// �ٷν��۽� �켱���� ���� ã���� ���ѷ��� ����
	INT64 *llRoomKeyExclude;

	void Set(int unum, char *id, int sndKind, int type, bool bUseFindRoomOption, ROOMOPTION& stFindRoomOption, int nBtnType, bool bLocalServer, INT64 rkeyExclude  ) 
	{
		char strId[16]={0,};
		if(strlen(id) < 16) strcpy(strId, id);
		SetHeader(SV_ASK_QUICKSTART);
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind));
		PushData(_PVAR(RoomType), &type, sizeof(*RoomType));
		PushData(_PVAR(m_bUseFindRoomOption), &bUseFindRoomOption, sizeof(*m_bUseFindRoomOption));
		PushData(_PVAR(m_stFindRoomOption), &stFindRoomOption, sizeof(*m_stFindRoomOption));
		PushData(_PVAR(nCallBtnType), &nBtnType, sizeof(*nCallBtnType));
		PushData(_PVAR(bForceLocalServer), &bLocalServer, sizeof(*bForceLocalServer));
		PushData(_PVAR(llRoomKeyExclude), &rkeyExclude, sizeof(*llRoomKeyExclude));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));
		TakeData(_PVAR(RoomType), sizeof(*RoomType));
		TakeData(_PVAR(m_bUseFindRoomOption), sizeof(*m_bUseFindRoomOption));
		TakeData(_PVAR(m_stFindRoomOption), sizeof(*m_stFindRoomOption));
		TakeData(_PVAR(nCallBtnType), sizeof(*nCallBtnType));
		TakeData(_PVAR(bForceLocalServer), sizeof(*bForceLocalServer));
		TakeData(_PVAR(llRoomKeyExclude), sizeof(*llRoomKeyExclude));
	}
};

// �ٸ��� �ٷΰ��� ��û
class CCL_ASK_QUICKSTART_IN_ROOM : public CCommMsg
{
public:

	int *SndFxKind;
	INT64 *llRoomKeyPrev;
	ROOMOPTION *roomOption;

	void Set(int sndKind, INT64 rkey_prev, ROOMOPTION& ro) 
	{
		SetHeader(CL_ASK_QUICKSTART_IN_ROOM);
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind));
		PushData(_PVAR(llRoomKeyPrev), &rkey_prev, sizeof(*llRoomKeyPrev));
		PushData(_PVAR(roomOption), &ro, sizeof(*roomOption));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));
		TakeData(_PVAR(llRoomKeyPrev), sizeof(*llRoomKeyPrev));
		TakeData(_PVAR(roomOption), sizeof(*roomOption));
	}
};

class CSV_ASK_QUICKSTART2 : public CCommMsg
{
public:

	enum
	{
		LOBBY_TYPE,			// �κ� �ٷν��۹�ư
		DETAIL_SETUP_TYPE,	// �󼼺��� ����
		IN_ROOM_TYPE,		// �ٸ��� �ٷΰ���
	};

	int *SndFxKind;
	int *nType;					// �ٷν��� ����( �κ� �ٷν��� ��ư = ECALL_BTNTYPE_LOBY_QUICK, �󼼺��� ���� = ECALL_BTNTYPE_DETAIL_SETUP )
	int *RoomType;				// 0 - ������, 1 - �Ϲݹ� 
	bool *bUseRoomOption;		// ��ɼ� ��뿩��, ����: true, �⺻�ٷΰ���: false
	bool *bMustSameRoomOption;	// ��ɼǰ� ������ �� �ƴϸ� �� ����
	bool *bFindLocalServer;		// ���� ���� ���������� ã���� ���� (ERR_ASKOTHERSERVER ���� ���)
	ROOMOPTION *roomOption;

	void Set(int sndKind, int type, int room, bool use_ro, bool must_same, bool find_local, const ROOMOPTION& ro ) 
	{
		SetHeader(SV_ASK_QUICKSTART2);
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind));
		PushData(_PVAR(nType), &type, sizeof(*nType));
		PushData(_PVAR(RoomType), &room, sizeof(*RoomType));
		PushData(_PVAR(bUseRoomOption), &use_ro, sizeof(*bUseRoomOption));
		PushData(_PVAR(bMustSameRoomOption), &must_same, sizeof(*bMustSameRoomOption));
		PushData(_PVAR(bFindLocalServer), &find_local, sizeof(*bFindLocalServer));
		PushData(_PVAR(roomOption), &ro, sizeof(*roomOption));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));
		TakeData(_PVAR(nType), sizeof(*nType));
		TakeData(_PVAR(RoomType), sizeof(*RoomType));
		TakeData(_PVAR(bUseRoomOption), sizeof(*bUseRoomOption));
		TakeData(_PVAR(bMustSameRoomOption), sizeof(*bMustSameRoomOption));
		TakeData(_PVAR(bFindLocalServer), sizeof(*bFindLocalServer));
		TakeData(_PVAR(roomOption), sizeof(*roomOption));
	}
};

//������� �ٷ�����
class CCL_ASK_PREMQUICKJOIN : public CCommMsg
{
public:	
	char*	m_idxGame;			// ������ �������� 
	bool*	m_bUseSearch;		// �˻���� ��뿩��
	int*	m_nSndFxKind; // ### [���� �߰� �۾�] ###
	SEARCHCONDITION* m_stSearchCondition;
	
	void Set( IDX_GAME idxGame_param, bool bUseSearch, int nSndFxKind, SEARCHCONDITION stSearchCondition)
	{		
		char cGameParam=(char)idxGame_param;
		SetHeader( CL_ASK_PREMQUICKJOIN );
		PushData( _PVAR(m_idxGame), &cGameParam, sizeof(*m_idxGame));
		PushData( _PVAR(m_bUseSearch), &bUseSearch, sizeof(*m_bUseSearch) );
		PushData( _PVAR(m_nSndFxKind), &nSndFxKind, sizeof(*m_nSndFxKind) );
		PushData( _PVAR(m_stSearchCondition), &stSearchCondition, sizeof(*m_stSearchCondition) );
	}
	void Get( char* lpdata, int size ) 
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_idxGame), sizeof(*m_idxGame));
		TakeData( _PVAR(m_bUseSearch), sizeof(*m_bUseSearch) );
		TakeData( _PVAR(m_nSndFxKind), sizeof(*m_nSndFxKind) );
		TakeData( _PVAR(m_stSearchCondition), sizeof( *m_stSearchCondition) );
	}
};

//������� �ٷ�����
class CSV_REFUSE_PREMQUICKJOIN : public CCommMsg
{
public: //������ ����
	enum
	{
		ERR_NOTSATISFIED,	//ã�� ���̾���.
		ERR_ENTERROOMERROR, //��������� 
		ERR_NOQJITEM,	//�������� ���ų� ����� 
		ERR_ASKOTHERSERVER, // �ٸ� ������ ���� ������� �ٷ������� ��û�ؾ� ��		
		ERR_PLAYTIMEOVER,
		ERR_DAILYLOSSLIMIT,
	};
	char*	m_cErrCode;
	int*	m_nGameIndex;
	int*	m_nChanNo;
	bool*	m_bUseSearch;		// �˻���� ��뿩��
	int*	m_nSndFxKind; // ### [���� �߰� �۾�] ###
	SEARCHCONDITION* m_stSearchCondition;
	
	void Set(char cErrCode, bool bUseSearch, int nSndFxKind, SEARCHCONDITION& stSearchCondition, int nGameIndex = -1, int nChanNo = -1)
	{
		SetHeader( SV_REFUSE_PREMQUICKJOIN );	
		PushData( _PVAR(m_cErrCode), &cErrCode, sizeof(*m_cErrCode) );
		PushData( _PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex) );
		PushData( _PVAR(m_nChanNo), &nChanNo, sizeof(*m_nChanNo) );
		PushData( _PVAR(m_bUseSearch), &bUseSearch, sizeof(*m_bUseSearch) );
		PushData( _PVAR(m_nSndFxKind), &nSndFxKind, sizeof(*m_nSndFxKind) );
		PushData( _PVAR(m_stSearchCondition), &stSearchCondition, sizeof(*m_stSearchCondition) );
	}
	void Get( char* lpdata, int size ) 
	{
		if( !GetHeader( lpdata, size ) ) return;	
		TakeData( _PVAR(m_cErrCode), sizeof(*m_cErrCode) );
		TakeData( _PVAR(m_nGameIndex), sizeof(*m_nGameIndex) );
		TakeData( _PVAR(m_nChanNo), sizeof(*m_nChanNo) );
		TakeData( _PVAR(m_bUseSearch), sizeof(*m_bUseSearch) );
		TakeData( _PVAR(m_nSndFxKind), sizeof(*m_nSndFxKind) );
		TakeData( _PVAR(m_stSearchCondition), sizeof(*m_stSearchCondition) );
	}
};

/////////// ��ü �� �ɼ� ������ �����ش� (ä�κ���� ����)
class CSV_ROOMOPTIONINFO : public CCommMsg
{
public:

	char *CreateOptionNum;
	ROOM_CREATEOPTION *pCreateOptionList;		// �������� ��ɼ�����

	void Set( char _CreateOptionNum, std::vector<ROOM_CREATEOPTION>& rvecCreateOptionList)
	{
		SetHeader(SV_ROOMOPTIONINFO);
		
		PushData(_PVAR(CreateOptionNum), &_CreateOptionNum, sizeof(*CreateOptionNum));
		for (size_t i = 0 ; i < rvecCreateOptionList.size() ; ++i)
		{
			PushData(_PVAR(pCreateOptionList), &rvecCreateOptionList[i], sizeof( ROOM_CREATEOPTION));
		}
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CreateOptionNum), sizeof(*CreateOptionNum));
		TakeData(_PVAR(pCreateOptionList), sizeof(ROOM_CREATEOPTION)*(*CreateOptionNum));		
	}
};


//DAT�α� (������ �ƾ�巹�� �뺸)
class CCL_NOTIFY_MACADDRESS : public CCommMsg
{
public:		
	char *MACADDRESS;	//�ƾ�巹��
	
	void Set( const char *pMacAddress)
	{	
		
		char MacBuf[20]={0,};
		strncpy(MacBuf, pMacAddress, 12);
		SetHeader( CL_NOTIFY_MACADDRESS );		
		PushData( _PVAR(MACADDRESS), MacBuf, 13);		
	}
	void Get( char* lpdata, int size ) 
	{
		if( !GetHeader( lpdata, size ) ) return;		
		TakeData( _PVAR(MACADDRESS), 13 );		
	}

};

//G_TFT
class CSV_CHANGE_MONEY : public CCommMsg
{
public:	

	char	*ID;
	char *UMCode;		//���ոӴ��ڵ� 6byte;
	INT64* ChangeMoney;
	INT64* PMoney;
	int *SmallData_1, *SmallData_2;	//���ոӴ��ڵ庰 ����� ���� int�� ������
	INT64 *BigData_1, *BigData_2;	//���ոӴ��ڵ庰 ����� ���� INT64�� ������		


	void Set(const char *p_id, const char *p_umcode, INT64 p_ChangeMoney, INT64 p_PMoney, int p_SmallData_1=0, int p_SmallData_2=0, INT64 p_BigData_1=0, INT64 p_BigData_2=0 ) 
	{
		char strid[16]={0,};
		if(p_id != NULL) strncpy(strid, p_id, 15);
		char strumcode[SIZE_UMCODE+1]={0,};
		if(p_id != NULL) strncpy(strumcode, p_umcode, SIZE_UMCODE);
		SetHeader(SV_CHANGE_MONEY);
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(UMCode), strumcode, SIZE_UMCODE);
		PushData(_PVAR(ChangeMoney), &p_ChangeMoney, sizeof(*ChangeMoney) );
		PushData(_PVAR(PMoney), &p_PMoney, sizeof(*PMoney) );
		PushData(_PVAR(SmallData_1), &p_SmallData_1, sizeof(*SmallData_1) );
		PushData(_PVAR(SmallData_2), &p_SmallData_2, sizeof(*SmallData_2) );
		PushData(_PVAR(BigData_1), &p_BigData_1, sizeof(*BigData_1) );
		PushData(_PVAR(BigData_2), &p_BigData_2, sizeof(*BigData_2) );


	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(UMCode), SIZE_UMCODE);
		TakeData(_PVAR(ChangeMoney), sizeof(*ChangeMoney) );
		TakeData(_PVAR(PMoney), sizeof(*PMoney) );
		TakeData(_PVAR(SmallData_1), sizeof(*SmallData_1) );
		TakeData(_PVAR(SmallData_2), sizeof(*SmallData_2) );
		TakeData(_PVAR(BigData_1), sizeof(*BigData_1) );
		TakeData(_PVAR(BigData_2), sizeof(*BigData_2) );		
	}
};


class CSV_NOTIFY_CHANGEOVERMONEY : public CCommMsg
{
public:	
	char* szID;

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	int* m_ChipKind; //������ Ĩ ������ �߰��Ѵ�.
	//////////////////////

	INT64* llOverMoney;						// �ʰ���
	INT64* llRevisionMoney;					// �����Ӵ�
	SYSTEMTIME* stOverMoneyEndDate;			// �ʰ��� ���� ����ð�
	SYSTEMTIME* stRevisionMoneyEndDate;		// �����Ӵ� ���� ����ð�
	
	void Set(const char* in_szID, int ChipKind, INT64 in_llOverMoney, const SYSTEMTIME* in_stOverMoneyEndDate, INT64 in_llRevisionMoney, const SYSTEMTIME* in_stRevisionMoneyEndDate) 
	{
		SetHeader(SV_NOTIFY_CHANGEOVERMONEY);
		
		char szIDBuf[ID_LEN] = {0, };
		if(in_szID != NULL)
		{
			::strncpy(szIDBuf, in_szID, ID_LEN-1);
		}

		PushData(_PVAR(szID), szIDBuf, ID_LEN);
		PushData(_PVAR(m_ChipKind), &ChipKind, sizeof(ChipKind));
		PushData(_PVAR(llOverMoney), &in_llOverMoney, sizeof(in_llOverMoney));
		PushData(_PVAR(stOverMoneyEndDate), in_stOverMoneyEndDate, sizeof(*in_stOverMoneyEndDate) );
		PushData(_PVAR(llRevisionMoney), &in_llRevisionMoney, sizeof(in_llRevisionMoney));
		PushData(_PVAR(stRevisionMoneyEndDate), in_stRevisionMoneyEndDate, sizeof(*in_stRevisionMoneyEndDate) );
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(szID), ID_LEN);
		TakeData(_PVAR(m_ChipKind), sizeof(*m_ChipKind));
		TakeData(_PVAR(llOverMoney), sizeof(*llOverMoney));
		TakeData(_PVAR(stOverMoneyEndDate), sizeof(*stOverMoneyEndDate));
		TakeData(_PVAR(llRevisionMoney), sizeof(*llRevisionMoney));
		TakeData(_PVAR(stRevisionMoneyEndDate), sizeof(*stRevisionMoneyEndDate));
	}
};

// ȸ���� ���� ���� �˸�
class CSV_NOTIFY_CHANGEPREMIUM : public CCommMsg
{
public:	
	char* szID;
	CODE_NEW_FAMILY* ePremiumCode;
	SYSTEMTIME* stEndDate;

	void Set(const char* in_szID, CODE_NEW_FAMILY in_ePremiumCode, SYSTEMTIME* in_stEndDate) 
	{
		SetHeader(SV_NOTIFY_CHANGEPREMIUM);

		char szIDBuf[ID_LEN] = {0, };
		::strncpy(szIDBuf, in_szID, ID_LEN-1);

		PushData(_PVAR(szID), szIDBuf, ID_LEN);
		PushData(_PVAR(ePremiumCode), &in_ePremiumCode, sizeof(in_ePremiumCode));
		PushData(_PVAR(stEndDate), in_stEndDate, sizeof(*in_stEndDate));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(szID), ID_LEN);
		TakeData(_PVAR(ePremiumCode), sizeof(*ePremiumCode));
		TakeData(_PVAR(stEndDate), sizeof(*stEndDate));
	}
};


/////////// ���Ӱ��� ���� ��û
class CCL_ASK_MOVEGAME : public CCommMsg
{
public:
	int* m_nGameIndex;
	int* m_nChanNo;

	void Set(int nGameIndex, int nChanNo = -1) 
	{
		SetHeader(CL_ASK_MOVEGAME);
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex));
		PushData(_PVAR(m_nChanNo), &nChanNo, sizeof(*m_nChanNo));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex));
		TakeData(_PVAR(m_nChanNo), sizeof(*m_nChanNo));
	}
};

class CCL_REQ_LOCKROOM : public CCommMsg
{
public:		
	char *LockState;

	void Set( char *pLockState )
	{
		SetHeader( CL_REQ_LOCKROOM );		
		PushData( _PVAR(LockState), pLockState, sizeof(char));
	}
	void Get( char* lpdata, int size ) 
	{
		if( !GetHeader( lpdata, size ) ) return;		
		TakeData( _PVAR(LockState), sizeof(char) );		
	}
};

class CCL_REQUEST_PLAYTIMEINFO : public CCommMsg
{
public:
	void Set()
	{
		SetHeader(CL_REQUEST_PLAYTIMEINFO) ;
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_RESPONSE_PLAYTIMEINFO : public CCommMsg
{
public:
	int *nPlayTimeSec; 

	void Set( int playtimesec )
	{
		SetHeader(SV_RESPONSE_PLAYTIMEINFO);
		PushData(_PVAR(nPlayTimeSec), &playtimesec, sizeof(*nPlayTimeSec));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nPlayTimeSec), sizeof(*nPlayTimeSec)) ;
	}
};

// ��ʸ�Ʈ���� �÷���Ÿ�� ���� 10������ Ư���� �˾��� ����ֱ� ���ؼ�
class CSV_NOTICE_TOURNAMENT_PLAYTIME : public CCommMsg
{
public:

	void Set()
	{
		SetHeader(SV_NOTICE_TOURNAMENT_PLAYTIME);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
	}
};


// UPP time ���� �޽��� 
// remainTimeSec : �ѵ��� �������ִ� ������ ���(1���) ���� �ð�  
// playTimeSec : ������� �÷����� �ð� ( ��� ���� ) -- �ǰ��ӽð�
// nRound : ���� ������ �Ǽ� 
// nRoundLimit : ���� �Ǽ� �ѵ� .( 1��޸� ����)

class CSV_UPPTIMEINFO : public CCommMsg
{
public:
	int *m_level ;  
	int *m_remainTimeSec ;
	int *m_playTimeSec;
	int *m_nRound;
	int *m_nRoundLimit ; 

	void Set( int level , int remainTimeSec ,int playTimeSec, int nRound, int nRoundLimit )
	{
		SetHeader( SV_UPPTIMEINFO) ;
		PushData(_PVAR(m_level), &level, sizeof(*m_level)) ;
		PushData(_PVAR(m_remainTimeSec), &remainTimeSec, sizeof(*m_remainTimeSec)) ;
		PushData(_PVAR(m_playTimeSec), &playTimeSec, sizeof(*m_playTimeSec)) ;
		PushData(_PVAR(m_nRound), &nRound, sizeof(*m_nRound)) ;
		PushData(_PVAR(m_nRoundLimit) , &nRoundLimit , sizeof(*m_nRoundLimit));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_level),sizeof(*m_level)) ;
		TakeData(_PVAR(m_remainTimeSec),sizeof(*m_remainTimeSec)) ;
		TakeData(_PVAR(m_playTimeSec),sizeof(*m_playTimeSec)) ;
		TakeData(_PVAR(m_nRound),sizeof(*m_nRound)) ;
		TakeData(_PVAR(m_nRoundLimit),sizeof(*m_nRoundLimit)) ;
	}
};

// Ŭ���̾�Ʈ���� event �� �˷��� 
// cmd : 0 , ����â , 30��( 3����� �ڱⰡ ������ �ð��� ������) �������� �˷��� 
//     : 1 , ����â ,( 3����� ��쿡�� ���� ���� ������ ������ ) 1 �ð� ������ ������ ���������ؾ� �� 
//     : 2 , UPP ������ �ɷ��� �α����� �ź� �Ǿ��� 
class CSV_UPPEVENT :public CCommMsg
{
public:
	enum UPPEVENT_TYPE 
	{
		WARNING , 
		EXIT ,			 // �˸��̿� ���� EXIT 
		DENYLOGIN,
		EXITFORCE,       // ���� exit ( 1����� �Ǽ� ���� �̳� �ð� ���� ) 
	};
	int *m_cmd ;
	void Set( int cmd)
	{
		SetHeader( SV_UPPEVENT ) ;
		PushData( _PVAR( m_cmd) , &cmd , sizeof(*m_cmd)) ;
	}
	void Get( char *lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_cmd),sizeof(*m_cmd)) ;
	}
};

////////////////////////////2017.04.08 �ű�UPP �۾�/////////////////////////////
/////2017�� ���ο� UPP �������� �߰��� ��Ŷ�̴�.
class CSV_NEW_UPPINFO : public CCommMsg
{
public:
	IDX_GAME *m_idxGame; //�ش� ���ӹ�ȣ
	int *m_Level; //�ش� ������ �̹��� UPP ����
	int *m_PlayCount; //�����Ǽ�
	int *m_PlayTime; //�����÷��̽ð�
	int *m_BillingMoney; //������ �����ݾ�
	int *m_State; //UPP ��Ƽ���� ���°�, 0.������(��ó��), 1.�����Ϸ�(ó���Ϸ�) ///1, 2 �ܰ�� Ŭ���̾�Ʈ�� ���۸� �ϸ� 1�� ����Ǹ�, 3�ܰ�� �������簡 �Ϸ�Ǿ�� 1�� �����

	/////////////////////////////UPP4�ܰ�, ������� ���� �߰� 2017.05.19/////////////////////////////////////////////
	//�ݸ������� DI�������� UPP�� �ɾ�����(m_Level == 5) �� ��쿡�� ��ȿ��
	NEW_UPP_REASON *m_NM_UPP_Reason;
	char *m_StartDate;
	char *m_EndDate;
	char *m_UserCN;//UPP 3�ܰ�� �ڰ����� ���������� ����� �ϴµ�, �� �������� �Ķ���ͷ� CN�� �Ѱܾ� �Ѵ�.
	/////////////////////////////////////////

	void Set(IDX_GAME idxGame, int Level, int PlayCount, int PlayTime, int BillingMoney, int State, NEW_UPP_REASON NM_UPP_Reason, char *StartDate, char *EndDate, char *UserCN)
	{
		SetHeader(SV_NEW_UPPINFO);
		PushData( _PVAR( m_idxGame) , &idxGame , sizeof(*m_idxGame)) ;
		PushData( _PVAR( m_Level) , &Level , sizeof(*m_Level)) ;
		PushData( _PVAR( m_PlayCount) , &PlayCount , sizeof(*m_PlayCount)) ;
		PushData( _PVAR( m_PlayTime) , &PlayTime , sizeof(*m_PlayTime)) ;
		PushData( _PVAR( m_BillingMoney) , &BillingMoney , sizeof(*m_BillingMoney)) ;
		PushData( _PVAR( m_State) , &State , sizeof(*m_State)) ;
		PushData( _PVAR( m_NM_UPP_Reason) , &NM_UPP_Reason , sizeof(*m_NM_UPP_Reason)) ;


		char strStartDate[20]={0,};
		if (StartDate != NULL) strncpy(strStartDate, StartDate, 20);
		PushData(_PVAR(m_StartDate), strStartDate, 20);

		char strEndDate[20]={0,};
		if (EndDate != NULL) strncpy(strEndDate, EndDate, 20);
		PushData(_PVAR(m_EndDate), strEndDate, 20);

		char strUserCN[16]={0,};
		if (UserCN != NULL) strncpy(strUserCN, UserCN, 16);
		PushData(_PVAR(m_UserCN), strUserCN, 16);
	}

	void Get( char *lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_idxGame),sizeof(*m_idxGame)) ;
		TakeData(_PVAR(m_Level),sizeof(*m_Level)) ;
		TakeData(_PVAR(m_PlayCount),sizeof(*m_PlayCount)) ;
		TakeData(_PVAR(m_PlayTime),sizeof(*m_PlayTime)) ;
		TakeData(_PVAR(m_BillingMoney),sizeof(*m_BillingMoney)) ;
		TakeData(_PVAR(m_State),sizeof(*m_State)) ;	
		TakeData(_PVAR(m_NM_UPP_Reason),sizeof(*m_NM_UPP_Reason)) ;	

		TakeData(_PVAR(m_StartDate),20);	
		TakeData(_PVAR(m_EndDate),20);	
		TakeData(_PVAR(m_UserCN),16);	
	}
};


// Ŭ���̾�Ʈ���� ���� ������� UPP ���� ������ �˷��ش�. 
class CSV_UPPINFO : public CCommMsg
{
public:
	int *m_level ; // UPP ���� 
	int *m_agree ; // UPP ��å�� agree �ߴ��� ���� 
	int *m_warning; // ��� â ���� ( �ʴ��� ) 
	int *m_exit   ; // ���� ���� ( �ʴ��� )
	int *m_play   ; // �������� ������ ���� �÷��� �ð� 
	int *m_round  ; // �������� ������ ���� �Ǽ� 
	int *m_policy ; // �ð� �˸��� �˸�â �ٿ�� ��å, 0 :������,1:�ѹ�,2:�׻� 
	int *m_usage  ; // 0 : ���� �α��ν� ���󰡴°�, 1 : moving �� ���󰡴°� , 2 : �ð��˸������� 

	void Set( int level, int agree, int warning, int exit,int play, int round,int policy,int usage)
	{
		SetHeader( SV_UPPINFO) ;
		PushData( _PVAR( m_level) , &level , sizeof(*m_level)) ;
		PushData( _PVAR( m_agree), &agree , sizeof(*m_agree)) ;
		PushData( _PVAR( m_warning), &warning , sizeof(*m_warning)) ;
		PushData( _PVAR( m_exit), &exit , sizeof( *m_exit)) ;
		PushData( _PVAR( m_play), &play , sizeof( *m_play)) ;
		PushData( _PVAR( m_round), &round , sizeof( *m_round)) ;
		PushData( _PVAR( m_policy), &policy , sizeof( *m_policy)) ;
		PushData( _PVAR( m_usage), &usage , sizeof( *m_usage)) ;
	}

	void Get( char *lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_level),sizeof(*m_level)) ;
		TakeData(_PVAR(m_agree),sizeof(*m_agree)) ;
		TakeData(_PVAR(m_warning),sizeof(*m_warning)) ;
		TakeData(_PVAR(m_exit),sizeof(*m_exit)) ;
		TakeData(_PVAR(m_play),sizeof(*m_play)) ;
		TakeData(_PVAR(m_round),sizeof(*m_round)) ;
		TakeData(_PVAR(m_policy),sizeof(*m_policy)) ;
		TakeData(_PVAR(m_usage),sizeof(*m_usage)) ;
	}

};

// Ŭ�� �������� �����ִ� upp ���� ���� �޽��� 
// cmd   1 :  UPP ��å�� �����ߴٴ� ���� �˷��� 
//       2 :  ���� ������ UPP �ð� �˸��� ������ �˷��޶�� ��û 
//       3 :  ������ �ð� �˸��� ������ ������ �������� �˷��ش�. 
//         ( param_1 �� ����ð� �ֱ� , param_2 �� ���� ���� �ð��� �ִ´�. �ʴ��� )
//		   ( param_3 ���� ���â �ٿ�� ��å, 0:������, 1:�ѹ�, 2:�׻� )
// param_1 , param_2, param_3 �� ���� ����� ���� �Ķ���͵� 
class CCL_ASK_UPPMSG : public CCommMsg
{
public:
	enum CmdType {
		AGREE,
		ASKINFO, 
		SETALARM, 
	};
	int *m_cmd ;
	int *m_param_1 ;
	int *m_param_2 ;
	int *m_param_3 ;
	void Set( int cmd, int param_1 , int param_2 , int param_3 )
	{
		SetHeader( CL_ASK_UPPMSG ) ;
		PushData( _PVAR( m_cmd) , &cmd , sizeof(*m_cmd)) ;
		PushData( _PVAR( m_param_1) , &param_1 , sizeof(*m_param_1)) ;
		PushData( _PVAR( m_param_2) , &param_2 , sizeof(*m_param_2)) ;
		PushData( _PVAR( m_param_3) , &param_3 , sizeof(*m_param_3)) ;
	}
	void Get( char *lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_cmd),sizeof(*m_cmd)) ;
		TakeData(_PVAR(m_param_1),sizeof(*m_param_1)) ;
		TakeData(_PVAR(m_param_2),sizeof(*m_param_2)) ;
		TakeData(_PVAR(m_param_3),sizeof(*m_param_3)) ;
	}

};

class CSV_EMERGENCYCHECK : public CCommMsg
{
public:
	int*	m_nGameIndex;
	bool*	m_bEmergencyCheck;		// �ӽ�������: true,  �������: false
	bool*	m_bAdminUser;				// �ӽ������߿��� ���尡�ɿ���
	void Set(int nGameIndex, bool bEmergencyCheck, bool bAdminUser)
	{
		SetHeader(SV_EMERGENCYCHECK);
		PushData(_PVAR(m_nGameIndex), &nGameIndex, sizeof(*m_nGameIndex) );
		PushData(_PVAR(m_bEmergencyCheck), &bEmergencyCheck, sizeof(*m_bEmergencyCheck) );
		PushData(_PVAR(m_bAdminUser), &bAdminUser, sizeof(*m_bAdminUser) );
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_nGameIndex), sizeof(*m_nGameIndex) );
		TakeData(_PVAR(m_bEmergencyCheck), sizeof(*m_bEmergencyCheck) );
		TakeData(_PVAR(m_bAdminUser), sizeof(*m_bAdminUser) );
	}
};


#if defined(_BETAVER)
class CCL_REQUEST_BETA_ALLIN_MONEY : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(CL_REQUEST_BETA_ALLIN_MONEY);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
	}
};
#endif // defined(_BETAVER)


//����׿� �Ӵϼ��� 
class CCL_DEBUG_SETMONEY : public CCommMsg
{
public:
	enum MONEYFLAG{
	           ADD_MONEY=1,
			   SET_MONEY,
			   SET_FAMILY,
			   SET_PCROOM,
			   SET_GOLD,////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	};


	int*   kind;
	INT64* money;
	char	*ID;

	void Set(char *id, int Kind, INT64 Money ) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(CL_DEBUG_SETMONEY);
		PushData(_PVAR(kind), &Kind, sizeof(*kind) );
		PushData(_PVAR(money), &Money, sizeof(*money) );
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(kind), sizeof(*kind) );
		TakeData(_PVAR(money), sizeof(*money) );
		TakeData(_PVAR(ID), 16);
	}
};


class CSV_ASK_RETENTION_REWARD : public CCommMsg
{
public:

	int *UNum;

	void Set(int uNum) 
	{

		SetHeader(CL_RETENTION_REWARD);

		PushData(_PVAR(UNum), &uNum, sizeof(*UNum));


	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(UNum), sizeof(*UNum));		
	}
};


//#if defined (__ROUND_MODE__)	// ���� ����

// ������ ����
class CCL_ASK_ROUND_MODE_RESERVE : public CCommMsg
{
public:

	int *nRet;
	int *nRoundModeType;

	
	void Set(int ret, int nroundmodetype) 
	{	
		SetHeader(CL_ROUND_MODE_RESERVE);		
		PushData(_PVAR(nRet), &ret, sizeof(*nRet));
		PushData(_PVAR(nRoundModeType), &nroundmodetype, sizeof(*nRoundModeType));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nRet), sizeof(*nRet));		
		TakeData(_PVAR(nRoundModeType), sizeof(*nRoundModeType));		
	}
};

class CSV_ASK_ROUND_MODE_RESERVE : public CCommMsg
{
public:

	int *nRet;
	int *nRoundModeType;


	void Set(int ret, int nroundmodetype) 
	{	
		SetHeader(SV_ROUND_MODE_RESERVE);		
		PushData(_PVAR(nRet), &ret, sizeof(*nRet));
		PushData(_PVAR(nRoundModeType), &nroundmodetype, sizeof(*nRoundModeType));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nRet), sizeof(*nRet));		
		TakeData(_PVAR(nRoundModeType), sizeof(*nRoundModeType));		
	}
};

// ������ ���
class CCL_ASK_ROUND_MODE_CANCEL : public CCommMsg
{
public:

	int *nRet;
	int *nRoundModeType;
	void Set( int ret, int nroundmodetype) 
	{
		
		SetHeader(CL_ROUND_MODE_CANCEL);

		PushData(_PVAR(nRet), &ret, sizeof(*nRet));
		PushData(_PVAR(nRoundModeType), &nroundmodetype, sizeof(*nRoundModeType));
		

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nRet), sizeof(*nRet));		
		TakeData(_PVAR(nRoundModeType), sizeof(*nRoundModeType));
	}
};

// ������ ���
class CSV_ASK_ROUND_MODE_CANCEL : public CCommMsg
{
public:

	int *nRet;
	int *nRoundModeType;
	void Set( int ret, int nroundmodetype) 
	{

		SetHeader(SV_ROUND_MODE_CANCEL);

		PushData(_PVAR(nRet), &ret, sizeof(*nRet));
		PushData(_PVAR(nRoundModeType), &nroundmodetype, sizeof(*nRoundModeType));


	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nRet), sizeof(*nRet));		
		TakeData(_PVAR(nRoundModeType), sizeof(*nRoundModeType));
	}
};

// ������ �� ����
class CSV_ROUND_MODE_SEND_SELECT_ENTER_ROOM : public CCommMsg
{
public:

	int *RoomNum;
	UINT64*	llRoomKey;			// IPS ��Ű
	int *UNum;
	bool *bAutoEnterRoom;		// true �� ���� ����ǰ� Ŭ�󿡼� ó��
	
	void Set(int rnum, UINT64 llnRoomKey, int unum , bool AutoEnterRoom) // �߰�
	{
		char strid[16]={0,};
		char strpass[5]={0,};
		
		SetHeader(SV_ROUND_MODE_SEND_SELECT_ENTER_ROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(llRoomKey), &llnRoomKey, sizeof(*llRoomKey));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(bAutoEnterRoom), &AutoEnterRoom, sizeof(*bAutoEnterRoom));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(llRoomKey), sizeof(*llRoomKey));
		TakeData(_PVAR(UNum), sizeof(*UNum));		
		TakeData(_PVAR(bAutoEnterRoom), sizeof(*bAutoEnterRoom));		
	}
};


// ������ �� ����
class CCL_ASK_ROUND_MODE_EXIT_ROOM : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	char *Reason;
	void Set(int rnum, int unum, char *id, char Reason_param) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(CL_ROUND_MODE_EXIT_ROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(Reason), &Reason_param, sizeof(*Reason));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Reason), sizeof(*Reason));
	}
};

class CCL_ASK_ROUND_MODE_ENTER_ROOM : public CCommMsg
{
public:

	int *RoomNum;
	UINT64*	llRoomKey;			// IPS ��Ű
	int *UNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;	
	int  *nEnter;	// 0: ��� , 1: ����

	void Set(int rnum, UINT64 llnRoomKey, int unum, int sndKind, char *id , int Enter) // �߰�
	{
		char strid[16]={0,};
		char strpass[5]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		
		SetHeader(CL_ROUND_MODE_SEND_SELECT_ENTER_ROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(llRoomKey), &llnRoomKey, sizeof(*llRoomKey));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // �߰�
		PushData(_PVAR(ID), strid, 16);		
		PushData(_PVAR(nEnter), &Enter, sizeof(*nEnter));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(llRoomKey), sizeof(*llRoomKey));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
		TakeData(_PVAR(ID), 16);		
		TakeData(_PVAR(nEnter), sizeof(*nEnter));
	}
};
// ���� ������ 3�� ���
class CSV_ROUND_MODE_GAME_START_WAIT_TIME : public CCommMsg
{
public:

	int *nWaitTimeCount;	// ��� �ð��� �ִ´�.
	INT64 *nTotalWinnerMoney;	// �� ���

	void Set( int ntimeCount , INT64 TotalWinnerMoney)
	{	
		SetHeader(SV_ROUND_MODE_GAME_START_WAIT_TIME);		
		
		PushData(_PVAR(nWaitTimeCount), &ntimeCount, sizeof(*nWaitTimeCount));		
		PushData(_PVAR(nTotalWinnerMoney), &TotalWinnerMoney, sizeof(*nTotalWinnerMoney));		
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		
		TakeData(_PVAR(nWaitTimeCount), sizeof(*nWaitTimeCount));		
		TakeData(_PVAR(nTotalWinnerMoney), sizeof(*nTotalWinnerMoney));	
	}
};

// ���� ��� ���� ���(������ ���� �� ȹ��ݾ� ǥ��) 
class CSV_ROUND_MODE_GAME_OVER : public CCommMsg
{	
public:

	ROUND_MODE_GAMEOVERRESULT *GOR;

	void Set(ROUND_MODE_GAMEOVERRESULT *pGOR) 
	{
		SetHeader(SV_ROUND_MODE_GAME_OVER);
		PushData(_PVAR(GOR), pGOR, sizeof(ROUND_MODE_GAMEOVERRESULT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(GOR), sizeof(ROUND_MODE_GAMEOVERRESULT) );
	}

};


// ���� ��� ���� ���� �Ǽ�
class CSV_ROUND_MODE_ROUND_COUNT : public CCommMsg
{	// ���� ���۽ÿ� �����ش�.
public:

	INT64 *nTotalRoundMoney;		// �� ���
	BYTE *nCount;				// ���� Ƚ��

	void Set(INT64 TotalRoundMoney , BYTE Count) 
	{
		SetHeader(SV_ROUND_MODE_ROUND_COUNT);
		PushData(_PVAR(nTotalRoundMoney), &TotalRoundMoney, sizeof(*nTotalRoundMoney));
		PushData(_PVAR(nCount), &Count, sizeof(*nCount));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nTotalRoundMoney), sizeof(nTotalRoundMoney) );
		TakeData(_PVAR(nCount), sizeof(nCount) );
	}

};

// ���� ��忡�� ����ϴ� Ĩ ����
class CSV_ROUND_MODE_BCAST_CHIPS : public CCommMsg
{	// ���� ��� ���� �����ش�.
public:
	ROUND_MODE_CHIPS *Chips;		// ������ Ĩ ������Ȳ

	void Set(ROUND_MODE_CHIPS *chips) 
	{
		SetHeader(SV_ROUND_MODE_BCAST_CHIPS);
		PushData(_PVAR(Chips), chips, sizeof(*Chips));
	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;			
		TakeData(_PVAR(Chips), sizeof(Chips) );
	}

};


class CSV_ROUND_MODE_FOCUS_TIME : public CCommMsg
{	// ���� ��� ���� �����ش�.
public:
	int *nStartTime;		//0-24
	int *nEndTime;		//0-24
	bool *bIsStart;		// 1: ����

	void Set(int StartTiem , int EndTime , bool IsStart) 
	{
		SetHeader(SV_ROUND_MODE_FOCUS_TIME);
		PushData(_PVAR(nStartTime), &StartTiem, sizeof(*nStartTime));
		PushData(_PVAR(nEndTime), &EndTime, sizeof(*nEndTime));
		PushData(_PVAR(bIsStart), &IsStart, sizeof(*bIsStart));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nStartTime), sizeof(nStartTime) );
		TakeData(_PVAR(nEndTime), sizeof(nEndTime) );
		TakeData(_PVAR(bIsStart), sizeof(bIsStart) );
	}
};

/////////////////////////////�ս��ѵ� �ʰ� ��� �߰� 2017.05.15/////////////////////////////////////////////
class CSV_DAILYLOSSLIMIT_USER_SETTING_INFO : public CCommMsg
{
	//�α���, ä���̵��ø��� ���� ���� ��Ȳ�� Ŭ���̾�Ʈ�� ������ �ش�.
public:
	IDX_GAME *m_idxGame; //�ش� ���ӹ�ȣ
	bool *m_EnableOver; //�ս��ѵ��� �����ؼ� ������ �� �� �ִ� ����
	bool *m_EnableSelect; //�ս��ѵ��� �����ϰڴٰ� ������ �� �ִ��� ����

	void Set(IDX_GAME idxGame, bool EnableOver, bool EnableSelect)
	{
		SetHeader(SV_DAILYLOSSLIMIT_USER_SETTING_INFO);
		PushData(_PVAR(m_idxGame), &idxGame, sizeof(*m_idxGame));
		PushData(_PVAR(m_EnableOver), &EnableOver, sizeof(*m_EnableOver));
		PushData(_PVAR(m_EnableSelect), &EnableSelect, sizeof(*m_EnableSelect));
	}

	void Get(char *lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_idxGame), sizeof(*m_idxGame));
		TakeData(_PVAR(m_EnableOver), sizeof(*m_EnableOver));
		TakeData(_PVAR(m_EnableSelect), sizeof(*m_EnableSelect));
	}
};

class CCL_SET_DAILYLOSSLIMIT_INFO : public CCommMsg
{
	//������ ���� �ս��ѵ��� �����ؼ� ������ �ҷ� ���ҷ� �����Ѵ�.
public:
	IDX_GAME *m_idxGame; //�ش� ���ӹ�ȣ
	bool *m_WishOverGame; //�����ؼ� ������ �����ϴ°� ���ϴ��� ����... ������ ������ FALSE, ���ϸ� TRUE
	
	void Set(IDX_GAME idxGame, bool WishOverGame) 
	{
		SetHeader(CL_SET_DAILYLOSSLIMIT_INFO);
		PushData(_PVAR(m_idxGame), &idxGame, sizeof(*m_idxGame));
		PushData(_PVAR(m_WishOverGame), &WishOverGame, sizeof(*m_WishOverGame));
	}

	void Get(char *lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_idxGame), sizeof(*m_idxGame));
		TakeData(_PVAR(m_WishOverGame), sizeof(*m_WishOverGame));
	}
};

class CSV_REWARD_DAILYLOSS_OVER : public CCommMsg
{
	//24�ð� ���ѿ� �ɸ����� ä�� �̵��̳� �α��ν� ���󳻿��� ������ ������ �ִ� ��Ŷ�̴�.
	//���ѿ� �ɷȴ��� �α��� �� ä���̵��� �����ϹǷ�, ������� ���õ� �������� ������0�ð� �Ѿ�� ���� ����
public:
	INT64 *m_RewardMoney; //���� ���� �Ӵ�
	SYSTEMTIME* m_BasicDate; //������ ���޵Ǵ� ���ؽð�.. �ش� �ð� ���Ŀ� �α��� Ȥ�� ä���̵��� �ؾ� ���� ������ �ȴٴ� �ǹ�

	void Set(INT64 RewardMoney, SYSTEMTIME BasicDate)
	{
		SetHeader(SV_REWARD_DAILYLOSS_OVER);
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
		PushData(_PVAR(m_BasicDate), &BasicDate, sizeof(*m_BasicDate));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
		TakeData(_PVAR(m_BasicDate), sizeof(*m_BasicDate));
	}
};

/////////////////////////////////////////////////////////////////////////////


////////////////////////////ä�ι뷱��, ����õ�� 2017.08.10/////////////////////////////////////
class CSV_REWARD_ALLIN_ANGEL : public CCommMsg
{
	//����õ�� ������� �˷��ִ� ��Ŷ�̴�.

public:
	INT64 *m_RewardMoney; //���� ���� �Ӵ�
	int *m_DailyEnableCount; //�Ϸ� ���� ������ ��Ƚ��
	int *m_RemainEnableCount; //���� ���� ���� ���� �ִ� ���� ���� Ƚ��

	void Set(INT64 RewardMoney, int EnableCount, int RemainCount)
	{
		SetHeader(SV_REWARD_ALLIN_ANGEL);
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
		PushData(_PVAR(m_DailyEnableCount), &EnableCount, sizeof(*m_DailyEnableCount));
		PushData(_PVAR(m_RemainEnableCount), &RemainCount, sizeof(*m_RemainEnableCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
		TakeData(_PVAR(m_DailyEnableCount), sizeof(*m_DailyEnableCount));
		TakeData(_PVAR(m_RemainEnableCount), sizeof(*m_RemainEnableCount));
	}
};
////////////////////////////////////////////////////


////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////
class CSV_PLAYINFO_SUPERMOON : public CCommMsg
{
	//���� ���۹� �̺�Ʈ ���� ������ �����ش�.

public:
	int *m_CurrentLevel; //���� ���� ����, ���� �� ���� -1 �̶�� ������ ���̻� ������ �̼��� ���ٴ� �ǹ��̴�.
	int *m_GoalCount; //��ǥ ī��Ʈ(�ش� ���� �Ǽ��� �Ǹ� ���� ����)
	int *m_CurrentCount; //������ �����Ǽ�

	void Set(int CurrentLevel, int GoalCount, int CurrentCount)
	{
		SetHeader(SV_PLAYINFO_SUPERMOON);
		PushData(_PVAR(m_CurrentLevel), &CurrentLevel, sizeof(*m_CurrentLevel));
		PushData(_PVAR(m_GoalCount), &GoalCount, sizeof(*m_GoalCount));
		PushData(_PVAR(m_CurrentCount), &CurrentCount, sizeof(*m_CurrentCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_CurrentLevel), sizeof(*m_CurrentLevel));
		TakeData(_PVAR(m_GoalCount), sizeof(*m_GoalCount));
		TakeData(_PVAR(m_CurrentCount), sizeof(*m_CurrentCount));
	}
};

class CSV_REWARD_SUPERMOON : public CCommMsg
{
	//���� ���۹� �̺�Ʈ ��÷ ������ �����ش�.
public:
	int *m_CurrentLevel; //���� ���� ����
	INT64 *m_MaxRewardMoney; //�ִ�� ���� ���� �� �ִ� �Ӵ�(���� ������ ������ �̷�� ����.)

	void Set(int CurrentLevel, int MaxRewardMoney)
	{
		SetHeader(SV_REWARD_SUPERMOON);
		PushData(_PVAR(m_CurrentLevel), &CurrentLevel, sizeof(*m_CurrentLevel));
		PushData(_PVAR(m_MaxRewardMoney), &MaxRewardMoney, sizeof(*m_MaxRewardMoney));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_CurrentLevel), sizeof(*m_CurrentLevel));
		TakeData(_PVAR(m_MaxRewardMoney), sizeof(*m_MaxRewardMoney));
	}
};

class CSV_REWARD_TOTALPOKER_START : public CCommMsg
{
	//��Ŀ�� ��ü ���� �̺�Ʈ�� ���������� �˷��ش�.
	//���� �ش� �̺�Ʈ�� ������ �� ��Ŷ�� �߼۵��� �ʴ´�.
	//Ư���� �� ������ ����.
public:
	void Set()
	{
		SetHeader(SV_REWARD_TOTALPOKER_START);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
	}
};

class CSV_REWARD_TOTALPOKER_SUPERSCORE : public CCommMsg
{
	//���������� �¸��Ͽ����� ������ �����ϴ� ��Ŷ
	//��ȿ� �ִ� ��ü �������� ��Ƽ�� �ؾ� �ϱ� ������, ���̵�� ������ȣ�� �Բ� �־���.
public:
	char *m_UserID; //��÷�� ���̵�
	int *m_UNum; //������ȣ
	int *m_UserGrade; //��÷���� �йи� ���
	int *m_ScoreCode; //��÷ �����ڵ�
	int *m_ScoreSubCode; //��÷ ���� �����ڵ�.. �ο쿡�� ��÷�� �� �����ڵ�� 0, ���⿡ ž��ȣ�� ����.
	int *m_TotalDailyRewardCount; //�Ϸ翡 ������ �ִ� ���� Ƚ��
	int *m_RemainDailyRewardCount; //���� ���� �ִ� ���� Ƚ��(���� ���� ��÷�� ����)
	INT64 *m_RewardMoney; //��÷��

	void Set(char *UserID, int UNum, int UserGrade, int ScoreCode, int ScoreSubCode, int TotalDailyRewardCount, int RemainDailyRewardCount, INT64 RewardMoney)
	{
		char _strUserID[ID_LEN]={0,};
		strncpy(_strUserID, UserID, ID_LEN-1);

		SetHeader(SV_REWARD_TOTALPOKER_SUPERSCORE);

		PushData(_PVAR(m_UserID), _strUserID, ID_LEN);
		PushData(_PVAR(m_UNum), &UNum, sizeof(*m_UNum));
		PushData(_PVAR(m_UserGrade), &UserGrade, sizeof(*m_UserGrade));
		PushData(_PVAR(m_ScoreCode), &ScoreCode, sizeof(*m_ScoreCode));
		PushData(_PVAR(m_ScoreSubCode), &ScoreSubCode, sizeof(*m_ScoreSubCode));
		PushData(_PVAR(m_TotalDailyRewardCount), &TotalDailyRewardCount, sizeof(*m_TotalDailyRewardCount));
		PushData(_PVAR(m_RemainDailyRewardCount), &RemainDailyRewardCount, sizeof(*m_RemainDailyRewardCount));
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_UserID), ID_LEN);
		TakeData(_PVAR(m_UNum), sizeof(*m_UNum));
		TakeData(_PVAR(m_UserGrade), sizeof(*m_UserGrade));
		TakeData(_PVAR(m_ScoreCode), sizeof(*m_ScoreCode));
		TakeData(_PVAR(m_ScoreSubCode), sizeof(*m_ScoreSubCode));
		TakeData(_PVAR(m_TotalDailyRewardCount), sizeof(*m_TotalDailyRewardCount));
		TakeData(_PVAR(m_RemainDailyRewardCount), sizeof(*m_RemainDailyRewardCount));
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
	}
};
////////////////////////////////////////////////////


////////////////////////////�����ε����, Ÿ�پ˸� 2017.10.12/////////////////////////////////////
class CSV_NOTICE_BIGWIN : public CCommMsg
{
	//x���̻� ȹ���� ���������� ���ǿ� �ִ� �����鿡�� �˷��ִ� ��Ŷ�̴�.
public:
	char *m_UserID; //��÷�� ���̵�
	INT64 *m_WinMoney; //��÷��
	int *m_ShowTime; //�޼��� ���� �ð�

	void Set(char *UserID, INT64 WinMoney, int ShowTime)
	{
		char _strUserID[ID_LEN]={0,};
		strncpy(_strUserID, UserID, ID_LEN-1);

		SetHeader(SV_NOTICE_BIGWIN);

		PushData(_PVAR(m_UserID), _strUserID, ID_LEN);
		PushData(_PVAR(m_WinMoney), &WinMoney, sizeof(*m_WinMoney));
		PushData(_PVAR(m_ShowTime), &ShowTime, sizeof(*m_ShowTime));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_UserID), ID_LEN);
		TakeData(_PVAR(m_WinMoney), sizeof(*m_WinMoney));
		TakeData(_PVAR(m_ShowTime), sizeof(*m_ShowTime));
	}
};

class CCL_SEND_CONG_MSG : public CCommMsg
{
	//Ư�� �������� ���� �޼����� �����Ѵ�.
	//Ŭ���̾�Ʈ�� ������ ������ ��Ŷ�̴�.
public:
	char *m_Receive_ID;			//�޴»�� ��� ID
	short *l_StrChat;	// �޼��� ��Ʈ���� ����
	char *StrChat;		// �޼��� ��Ʈ��

	void Set(const char* Receive_ID, const char *chat) 
	{
		char strReceiveID[ID_LEN]={0,};

		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength() + 1;
	
		SetHeader(CL_SEND_CONG_MSG);
	
		if(strlen(Receive_ID) < ID_LEN) strcpy(strReceiveID, Receive_ID);
	
		PushData(_PVAR(m_Receive_ID), strReceiveID, ID_LEN);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Receive_ID), ID_LEN);
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
	}
};

class CSV_RECEIVE_CONG_MSG : public CCommMsg
{
	//���� �޼��� ���������� �޾Ҵٰ� �˷��ش�.
public:
	char *m_Send_ID;	//������� ��� ID
	short *l_StrChat;	// �޼��� ��Ʈ���� ����
	char *StrChat;		// �޼��� ��Ʈ��
	int *m_ShowTime; //�޼��� ���� �ð�

	void Set(const char* Send_ID, const char *chat, int ShowTime) 
	{
		char strSendID[ID_LEN]={0,};

		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength() + 1;
	
		SetHeader(SV_RECEIVE_CONG_MSG);
	
		if(strlen(Send_ID) < ID_LEN) strcpy(strSendID, Send_ID);
	
		PushData(_PVAR(m_Send_ID), strSendID, ID_LEN);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(m_ShowTime), &ShowTime, sizeof(*m_ShowTime));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Send_ID), ID_LEN);
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(m_ShowTime), sizeof(*m_ShowTime));
	}
};
////////////////////////////////////////////////////////////


////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
class CSV_ETCCHIP_OPEN_REWARD : public CCommMsg
{
	//��Ÿ��ȭ ���� ������� �ش� ��ȭ�� �����ߴٰ� �˷��ش�.
public:
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_RewardMoney; //���� ���� �Ӵ�

	void Set(const char* ChipType, INT64 RewardMoney) 
	{
		char strChipType[3]={0,};
	
		SetHeader(SV_ETCCHIP_OPEN_REWARD);

		strcpy(strChipType, ChipType);

		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
	}
};

class CSV_REWARD_ALLIN_ANGEL_GOLD : public CCommMsg
{
	//���ä�� ����õ�� ������� �˷��ִ� ��Ŷ�̴�.

public:
	INT64 *m_RewardMoney; //���� ���� �Ӵ�
	int *m_DailyEnableCount; //�Ϸ� ���� ������ ��Ƚ��
	int *m_RemainEnableCount; //���� ���� ���� ���� �ִ� ���� ���� Ƚ��

	void Set(INT64 RewardMoney, int EnableCount, int RemainCount)
	{
		SetHeader(SV_REWARD_ALLIN_ANGEL_GOLD);
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
		PushData(_PVAR(m_DailyEnableCount), &EnableCount, sizeof(*m_DailyEnableCount));
		PushData(_PVAR(m_RemainEnableCount), &RemainCount, sizeof(*m_RemainEnableCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
		TakeData(_PVAR(m_DailyEnableCount), sizeof(*m_DailyEnableCount));
		TakeData(_PVAR(m_RemainEnableCount), sizeof(*m_RemainEnableCount));
	}
};

class CSV_ETCCHIP_EVENT_INFO : public CCommMsg
{
	//��Ÿ ��ȭ ���� �̺�Ʈ ������ �˷��ش�.
	//�Ʒ� float �����͵��� % ������ �ƴԿ� ��������.
public:
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	float *m_BasicRate; //�̺�Ʈ�� �ƴϾ����� �Ϲ� ä�ο��� �⺻���� �����Ǵ� ������..
	float *m_BasicRate_GoldCh; //�̺�Ʈ�� �ƴϾ����� ���ä�ο��� �⺻���� �����Ǵ� ������..
	float *m_MemberGradeRate; //�� ȸ����޺� �߰��Ǵ� ������
	float *m_EventRate; //�̺�Ʈ�� �߰��Ǿ� �����Ǵ� ������
	float *m_RealRate; //�� �⺻ ������ �̺�Ʈ ������ ���� ���� �����Ǵ� ������..
	float *m_RealRate_GoldCh; //�� �⺻ ������ �̺�Ʈ ������ ���� ���� �����Ǵ� ������..(���ä�ο��� ���ӽ�)
	bool *m_ChipUpdated; //�� ��Ŷ�� �ޱ� ������ ���� ��� ������ �̷�� ������ ����.. 
	                     //false�̸� �α���, ä���̵��� �ܼ� �̺�Ʈ �����̸�, true�̸� ������ ���� ������ ���� ���� �������� �˷��ִ� �����̴�.
	char *m_EventStartDate; //�̺�Ʈ �����Ͻ�
	char *m_EventEndDate; //�̺�Ʈ �����Ͻ�
	char *m_EventEveryStartTime; //�̺�Ʈ �Ⱓ�� �̺�Ʈ ���� �ð�
	char *m_EventEveryEndTime; //�̺�Ʈ �Ⱓ�� �̺�Ʈ ���� �ð�

	void Set(const char* ChipType, float BasicRate, float BasicRate_GoldCh, float MemberGradeRate, float EventRate, float RealRate, float RealRate_GoldCh,
		bool ChipUpdated, const char* EventStartDate, const char* EventEndDate, const char* EventEveryStartTime, const char* EventEveryEndTime) 
	{
		char strChipType[3]={0,};
		char strEventStartDate[20]={0,};
		char strEventEndDate[20]={0,};
		char strEventEveryStartTime[6]={0,};
		char strEventEveryEndTime[6]={0,};
	
		SetHeader(SV_ETCCHIP_EVENT_INFO);

		strcpy(strChipType, ChipType);
		strcpy(strEventStartDate, EventStartDate);
		strcpy(strEventEndDate, EventEndDate);
		strcpy(strEventEveryStartTime, EventEveryStartTime);
		strcpy(strEventEveryEndTime, EventEveryEndTime);

		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_BasicRate), &BasicRate, sizeof(*m_BasicRate));
		PushData(_PVAR(m_BasicRate_GoldCh), &BasicRate_GoldCh, sizeof(*m_BasicRate_GoldCh));
		PushData(_PVAR(m_MemberGradeRate), &MemberGradeRate, sizeof(*m_MemberGradeRate));
		PushData(_PVAR(m_EventRate), &EventRate, sizeof(*m_EventRate));
		PushData(_PVAR(m_RealRate), &RealRate, sizeof(*m_RealRate));
		PushData(_PVAR(m_RealRate_GoldCh), &RealRate_GoldCh, sizeof(*m_RealRate_GoldCh));
		PushData(_PVAR(m_ChipUpdated), &ChipUpdated, sizeof(*m_ChipUpdated));
		PushData(_PVAR(m_EventStartDate), strEventStartDate, 20);
		PushData(_PVAR(m_EventEndDate), strEventEndDate, 20);
		PushData(_PVAR(m_EventEveryStartTime), strEventEveryStartTime, 6);
		PushData(_PVAR(m_EventEveryEndTime), strEventEveryEndTime, 6);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_BasicRate), sizeof(*m_BasicRate));
		TakeData(_PVAR(m_BasicRate_GoldCh), sizeof(*m_BasicRate_GoldCh));
		TakeData(_PVAR(m_MemberGradeRate), sizeof(*m_MemberGradeRate));
		TakeData(_PVAR(m_EventRate), sizeof(*m_EventRate));
		TakeData(_PVAR(m_RealRate), sizeof(*m_RealRate));
		TakeData(_PVAR(m_RealRate_GoldCh), sizeof(*m_RealRate_GoldCh));
		TakeData(_PVAR(m_ChipUpdated), sizeof(*m_ChipUpdated));
		TakeData(_PVAR(m_EventStartDate), 20);
		TakeData(_PVAR(m_EventEndDate), 20);
		TakeData(_PVAR(m_EventEveryStartTime), 6);
		TakeData(_PVAR(m_EventEveryEndTime), 6);
	}
};

class CSV_ETCCHIP_RAKEBACK_INFO : public CCommMsg
{
	//��ŸĨ�� Rake Back�Ǿ� ����� ������ �����ش�.
	//Rake Back�� �������� �Ϻθ� ������ ���·� ������ �ִ°� ����.
	//�� ������ ���� ȹ���� �Ӵ��� �Ϻθ� ������� ���� �Ǵµ�, ���� �Ϻθ� �ٽ� �����޴°�~!!
public:
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_ChangeChip_Int; //����� �ݾ��� �����κ�
	int *m_ChangeChip_Prime; //����� �ݾ��� �Ҽ��κ�
	float *m_BasicRate; //�⺻ Rake Back ����(%������ �ƴϴ�~).. �� ���� 0.002 ������ ���µ�, �̰�� 0.2% �� �����Ǿ��ٰ� ���� ��
	float *m_BonusRate; //�߰� ���ʽ� ����(�� ���� 2.0f �̸� �ι� ���� �̺�Ʈ��� �̾߱�..)
	//�� �ΰ��� Rate�� ���� ���� �������� ������ ����� �� ����
	//���� m_BasicRate = 0.003, m_BonusRate = 2.0 �̶��, ������ 0.003 * 2.0 = 0.006(0.6%)�� �����޾Ҵٰ� ���� ��

	void Set(const char* ChipType, INT64 ChangeChip_Int, int ChangeChip_Prime, float BasicRate, float BonusRate) 
	{
		char strChipType[3]={0,};
	
		SetHeader(SV_ETCCHIP_RAKEBACK_INFO);

		strcpy(strChipType, ChipType);
	
		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_ChangeChip_Int), &ChangeChip_Int, sizeof(*m_ChangeChip_Int));
		PushData(_PVAR(m_ChangeChip_Prime), &ChangeChip_Prime, sizeof(*m_ChangeChip_Prime));
		PushData(_PVAR(m_BasicRate), &BasicRate, sizeof(*m_BasicRate));
		PushData(_PVAR(m_BonusRate), &BonusRate, sizeof(*m_BonusRate));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_ChangeChip_Int), sizeof(*m_ChangeChip_Int));
		TakeData(_PVAR(m_ChangeChip_Prime), sizeof(*m_ChangeChip_Prime));
		TakeData(_PVAR(m_BasicRate), sizeof(*m_BasicRate));
		TakeData(_PVAR(m_BonusRate), sizeof(*m_BonusRate));
	}
};

class CSV_ETCCHIP_CHANGE_INFO : public CCommMsg
{
	//��ŸĨ ����� �ݾ� �����̴�.

public:
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_ChangeChip; //ȭ�鿡 �����ִ� ���� �κи� ���͸��� ��ȭ�ݾ��̴�.
	INT64 *m_FullChangeChip_Int; //���� �Ҽ������� ����� �ݾ��� �����κ��̴�.
	int *m_FullChangeChip_Prime; //���� �Ҽ������� ����� �ݾ��� �Ҽ��κ��̴�.
	//���� ��ȭ�� �ݾ� �ؽ�Ʈ�� ���濹 => Format("%I64d.%d"), m_FullChangeChip_Int, m_FullChangeChip_Prime) 

	void Set(const char* ChipType, INT64 ChangeChip, INT64 FullChangeChip_Int, int FullChangeChip_Prime) 
	{
		char strChipType[3]={0,};
	
		SetHeader(SV_ETCCHIP_CHANGE_INFO);

		strcpy(strChipType, ChipType);
	
		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_ChangeChip), &ChangeChip, sizeof(*m_ChangeChip));
		PushData(_PVAR(m_FullChangeChip_Int), &FullChangeChip_Int, sizeof(*m_FullChangeChip_Int));
		PushData(_PVAR(m_FullChangeChip_Prime), &FullChangeChip_Prime, sizeof(*m_FullChangeChip_Prime));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_ChangeChip), sizeof(*m_ChangeChip));
		TakeData(_PVAR(m_FullChangeChip_Int), sizeof(*m_FullChangeChip_Int));
		TakeData(_PVAR(m_FullChangeChip_Prime), sizeof(*m_FullChangeChip_Prime));
	}
};


class CSV_ETCCHIP_INFO : public CCommMsg
{
	//���ӸӴ� �ܿ� ������ ��Ÿ Ĩ ������ �����ش�.
	//���� �α��ν�, ä���̵���, Ĩ ������ �ش� ��Ŷ�� Ŭ���̾�Ʈ�� �����Ѵ�.
	//���⿡ ���ӸӴ� ��� ��ġ�� �߰�..2017.11.22
public:
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_RealChip; //ȭ�鿡 �����ִ� ���� �����ݾ��̴�.
	int *m_RemainChip; //�Ҽ������� ���Ǿ� �������� ����Ǿ� �ִ� �ݾ��̴�. UI�����δ� �ش� �ݾ��� ������� ��ġ��, Ȥ�ó� �ؼ� �־���
	INT64 *m_ChipValue; //�ش� ��ȭ 1��, ���ӸӴ� ��ġ�̴�.. �� ���� 10���̸� 1��ȭ�� ���ӸӴ� 10���̶�� �ǹ�

	void Set(const char* ChipType, INT64 RealChip, int RemainChip, INT64 ChipValue) 
	{
		char strChipType[3]={0,};
	
		SetHeader(SV_ETCCHIP_INFO);

		strcpy(strChipType, ChipType);
	
		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_RealChip), &RealChip, sizeof(*m_RealChip));
		PushData(_PVAR(m_RemainChip), &RemainChip, sizeof(*m_RemainChip));
		PushData(_PVAR(m_ChipValue), &ChipValue, sizeof(*m_ChipValue));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_RealChip), sizeof(*m_RealChip));
		TakeData(_PVAR(m_RemainChip), sizeof(*m_RemainChip));
		TakeData(_PVAR(m_ChipValue), sizeof(*m_ChipValue));
	}
};

class CSV_ETCCHIP_PLAYER_INFO : public CCommMsg
{
	//���ӸӴ� �ܿ� ��� ������ ��Ÿ Ĩ ������ �����ش�.
	//��ȿ��� Ư�� ������ ��Ÿ Ĩ ������ �ٸ� �����鿡�� �����ٶ� ���
public:
	int *m_pnum; //����� �¼���ȣ(�÷��̾� ��ȣ)
	char *m_UserID; //�ش� ������ ID
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_RealChip; //ȭ�鿡 �����ִ� ���� �����ݾ��̴�.
	int *m_RemainChip; //�Ҽ������� ���Ǿ� �������� ����Ǿ� �ִ� �ݾ��̴�. UI�����δ� �ش� �ݾ��� ������� ��ġ��, Ȥ�ó� �ؼ� �־���

	void Set(int pnum, const char* UserID, const char* ChipType, INT64 RealChip, int RemainChip) 
	{
		char strChipType[3]={0,};
		char strUserID[ID_LEN]={0,};
	
		SetHeader(SV_ETCCHIP_PLAYER_INFO);

		strcpy(strChipType, ChipType);

		if(strlen(UserID) < ID_LEN) strcpy(strUserID, UserID);
	
		PushData(_PVAR(m_pnum), &pnum, sizeof(*m_pnum));
		PushData(_PVAR(m_UserID), strUserID, ID_LEN);
		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_RealChip), &RealChip, sizeof(*m_RealChip));
		PushData(_PVAR(m_RemainChip), &RemainChip, sizeof(*m_RemainChip));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_pnum), sizeof(*m_pnum));
		TakeData(_PVAR(m_UserID), ID_LEN);
		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_RealChip), sizeof(*m_RealChip));
		TakeData(_PVAR(m_RemainChip), sizeof(*m_RemainChip));
	}
};

class CCL_CHANGE_ETCCHIP : public CCommMsg
{
	//��Ÿ ��ȭ�� ���ӸӴϷ� �����û�Ѵ�.
public:
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_ChangeChip; //ȭ�鿡 �����ִ� ���� �����ݾ��̴�.

	void Set(const char* ChipType, INT64 ChangeChip) 
	{
		char strChipType[3]={0,};
	
		SetHeader(CL_CHANGE_ETCCHIP);

		strcpy(strChipType, ChipType);
	
		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_ChangeChip), &ChangeChip, sizeof(*m_ChangeChip));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_ChangeChip), sizeof(*m_ChangeChip));
	}
};

class CSV_CHANGE_ETCCHIP_RESULT : public CCommMsg
{
	//��Ÿ ��ȭ�� ���ӸӴϷ� �����ϴ°Ϳ� ���� ����ڵ��̴�.
public:
	int *m_Result; //����ڵ�..0.����, 1.�����ӴϺ���, 2.���ӸӴϺ����ѵ����ɸ�, 99.��Ÿ����
	char *m_ChipType; //ĨŸ��.. ó�� �߰��� ���� GD�� ����.
	INT64 *m_ChangeChip; //����(��û)�� ��Ÿ ��ȭ�ݾ�
	INT64 *m_CurChip; //����Ϸ��� Ȯ���� ��ȭ�ݾ�
	INT64 *m_CurGameMoney; //����Ϸ��� Ȯ���� ���ӸӴ�

	void Set(int Result, const char* ChipType, INT64 ChangeChip, INT64 CurChip, INT64 CurGameMoney) 
	{
		char strChipType[3]={0,};
	
		SetHeader(SV_CHANGE_ETCCHIP_RESULT);

		strcpy(strChipType, ChipType);
	
		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_ChipType), strChipType, 3);
		PushData(_PVAR(m_ChangeChip), &ChangeChip, sizeof(*m_ChangeChip));
		PushData(_PVAR(m_CurChip), &CurChip, sizeof(*m_CurChip));
		PushData(_PVAR(m_CurGameMoney), &CurGameMoney, sizeof(*m_CurGameMoney));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_ChipType), 3);
		TakeData(_PVAR(m_ChangeChip), sizeof(*m_ChangeChip));
		TakeData(_PVAR(m_CurChip), sizeof(*m_CurChip));
		TakeData(_PVAR(m_CurGameMoney), sizeof(*m_CurGameMoney));
	}
};
////////////////////////////////////////////////////////////

////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
class CSV_CLUB_RESPONSE_ALL_LIST : public CCommMsg
{
	//����� ��� Ŭ�� ������ ������ �ش�.
public:
	int *m_Count;
	CLUB_INFO *m_ArrayClubInfo;

	void Set(std::vector<CLUB_INFO>& rArrayClubInfo)
	{
		SetHeader(SV_CLUB_RESPONSE_ALL_LIST);

		int count = rArrayClubInfo.size();

		PushData( _PVAR(m_Count), &count, sizeof(*m_Count));

		size_t size = rArrayClubInfo.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData( _PVAR(m_ArrayClubInfo), &rArrayClubInfo[i], sizeof(CLUB_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayClubInfo), sizeof(CLUB_INFO) * (*m_Count));
	}
};

class CCL_CLUB_REQUEST_ALL_LIST : public CCommMsg
{
	//����� Ŭ�� ��ü ����Ʈ�� ��û�Ѵ�.
public:
	int *m_MaxCount; //�ʿ信 ���� �ִ�ġ�� �־��ָ� ��. -1�ϰ�� ������(�ִ°� �� ������)

	void Set(int MaxCount)
	{
		SetHeader(CL_CLUB_REQUEST_ALL_LIST);

		PushData(_PVAR(m_MaxCount), &MaxCount, sizeof(*m_MaxCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_MaxCount), sizeof(*m_MaxCount));
	}
};

class CSV_CLUB_MEMBER_FIRE_NOTIFY : public CCommMsg
{
	//BOSS�� Ŭ������ ������״ٴ� �˶��� ���ο��� �ش�.
public:
	int *m_ReasonCode; //Ż������.. enum CLUB_FIRE_REASON �����Ͱ� ��. CLUB_FIRE_REASON_BOSS_KICK, CLUB_FIRE_REASON_CLOSE
	char *m_ClubName; //Ŭ���̸�

	void Set(int ReasonCode, const char *ClubName)
	{
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};

		SetHeader(SV_CLUB_MEMBER_FIRE_NOTIFY);

		strcpy(strClubName, ClubName);

		PushData(_PVAR(m_ReasonCode), &ReasonCode, sizeof(*m_ReasonCode));
		PushData(_PVAR(m_ClubName), strClubName, CHAR_BUFFER_CLUB_NAME);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ReasonCode), sizeof(*m_ReasonCode));
		TakeData(_PVAR(m_ClubName), CHAR_BUFFER_CLUB_NAME);
	}
};

class CSV_CLUB_MEMBER_FIRE_RESULT : public CCommMsg
{
	//Ư���� Ŭ�� ���� ���� ����̴�.
public:
	int *m_Result; //0.����, -1000.���ο���, -1001.Ŭ����������, -1002.�������� �ʴ� ȸ��, -1003.���� Ż�� �ѵ� �ʰ�(1��1ȸ)
	char *m_FireID;

	void Set(int Result, const char *FireID)
	{
		SetHeader(SV_CLUB_MEMBER_FIRE_RESULT);

		char strFireID[NICK_LEN]={0,};
		if(strlen(strFireID) < NICK_LEN) strcpy(strFireID, FireID);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_FireID), strFireID, NICK_LEN);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_FireID), NICK_LEN);
	}
};

class CCL_CLUB_MEMBER_FIRE : public CCommMsg
{
	//������ �ڽ��� Ŭ������ Ư������ ���� Ż���ų�� ������.
public:
	char *m_FireID;
	char *m_FireCN;

	void Set(const char *FireID, const char *FireCN)
	{
		SetHeader(CL_CLUB_MEMBER_FIRE);

		char strFireID[NICK_LEN]={0,};
		char strFireCN[CN_LEN]={0,};

		if(strlen(FireID) < NICK_LEN) strcpy(strFireID, FireID);
		if(strlen(FireCN) < CN_LEN) strcpy(strFireCN, FireCN);

		PushData(_PVAR(m_FireID), strFireID, NICK_LEN);
		PushData(_PVAR(m_FireCN), strFireCN, CN_LEN);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_FireID), NICK_LEN);
		TakeData(_PVAR(m_FireCN), CN_LEN);
	}
};

class CSV_CLUB_WITHDRAW_RESULT : public CCommMsg
{
	//Ŭ�� Ż�� ���� ����� �����ش�.
public:
	int *m_Result; //0.����, -1000.���ο���, -1001.Ŭ����������, -1002.�簡���� Ŭ���� ���� Ż��Ұ�
	char *m_ClubName; //Ŭ���̸�

	void Set(int Result, const char *ClubName)
	{
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};

		SetHeader(SV_CLUB_WITHDRAW_RESULT);

		strcpy(strClubName, ClubName);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_ClubName), strClubName, CHAR_BUFFER_CLUB_NAME);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_ClubName), CHAR_BUFFER_CLUB_NAME);
	}
};

class CCL_CLUB_WITHDRAW : public CCommMsg
{
	//������ ���� ������ Ŭ������ Ż���Ѵٴ� ��û�̴�.
	//������ �� ��Ŷ�� �������, �ڽ��� ������ Ŭ���� ���� ��Ų��.
	//���� �ϳ��� Ŭ������ ������ �� �����Ƿ�, �߰��� ���� ������ ����.
public:
	void Set()
	{
		SetHeader(CL_CLUB_WITHDRAW);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
	}
};

class CSV_CLUB_RESPONSE_MYSELF_INVITED_LIST : public CCommMsg
{
	//���� ������ ��û�� Ŭ�� ���� ����Ʈ�� �����ش�.
public:
	int *m_TodayMax; //���� ������ �ִ� ���Կ�û Ƚ��
	int *m_TodayCount; //���� ���Կ�û�� Ƚ��
	int *m_Count; //����Ʈī��Ʈ(���� ���Կ�û�� Ƚ���� �ƴ�)
	CLUB_INVITED_INFO *m_ArrayInvitedInfo;

	void Set(int TodayMax, int TodayCount, std::vector<CLUB_INVITED_INFO>& rArrayInvitedInfo)
	{
		SetHeader(SV_CLUB_RESPONSE_MYSELF_INVITED_LIST);

		PushData( _PVAR(m_TodayMax), &TodayMax, sizeof(*m_TodayMax));
		PushData( _PVAR(m_TodayCount), &TodayCount, sizeof(*m_TodayCount));

		int count = rArrayInvitedInfo.size();

		PushData( _PVAR(m_Count), &count, sizeof(*m_Count));

		size_t size = rArrayInvitedInfo.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData( _PVAR(m_ArrayInvitedInfo), &rArrayInvitedInfo[i], sizeof(CLUB_INVITED_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_TodayMax), sizeof(*m_TodayMax));
		TakeData(_PVAR(m_TodayCount), sizeof(*m_TodayCount));
		TakeData(_PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayInvitedInfo), sizeof(CLUB_INVITED_INFO) * (*m_Count));
	}
};

class CCL_CLUB_REQUEST_MYSELF_INVITED_LIST : public CCommMsg
{
	//���� ������ ��û�� Ŭ�� ���� ����Ʈ�� ��û�Ѵ�.
public:
	int *m_MaxCount; //�ʿ信 ���� �ʴ��� ����Ʈ�� �ִ�ġ�� �־��ָ� ��. -1�ϰ�� ������(�ִ°� �� ������)

	void Set(int MaxCount)
	{
		SetHeader(CL_CLUB_REQUEST_MYSELF_INVITED_LIST);

		PushData(_PVAR(m_MaxCount), &MaxCount, sizeof(*m_MaxCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_MaxCount), sizeof(*m_MaxCount));
	}
};


class CSV_CLUB_RESPONSE_INVITED_LIST : public CCommMsg
{
	//�ش� ������ ���� �ʴ븮��Ʈ�� �����ش�.
public:
	int *m_Count;
	CLUB_INVITED_INFO *m_ArrayInvitedInfo;

	void Set(std::vector<CLUB_INVITED_INFO>& rArrayInvitedInfo)
	{
		SetHeader(SV_CLUB_RESPONSE_INVITED_LIST);

		int count = rArrayInvitedInfo.size();

		PushData( _PVAR(m_Count), &count, sizeof(*m_Count));

		size_t size = rArrayInvitedInfo.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData( _PVAR(m_ArrayInvitedInfo), &rArrayInvitedInfo[i], sizeof(CLUB_INVITED_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayInvitedInfo), sizeof(CLUB_INVITED_INFO) * (*m_Count));
	}
};

class CCL_CLUB_REQUEST_INVITED_LIST : public CCommMsg
{
	//���� ���� �ʴ��� ����Ʈ�� ��û�Ѵ�.
public:
	int *m_MaxCount; //�ʿ信 ���� �ʴ��� ����Ʈ�� �ִ�ġ�� �־��ָ� ��. -1�ϰ�� ������(�ִ°� �� ������)

	void Set(int MaxCount)
	{
		SetHeader(CL_CLUB_REQUEST_INVITED_LIST);

		PushData(_PVAR(m_MaxCount), &MaxCount, sizeof(*m_MaxCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_MaxCount), sizeof(*m_MaxCount));
	}
};

class CSV_CLUB_RESPONSE_MYSELF_JOIN_LIST : public CCommMsg
{
	//�������� �ʴ��� ����Ʈ ����(������)
public:
	int *m_TodayMax; //���� ������ �ִ� �ʴ� Ƚ��
	int *m_TodayCount; //���� ���� Ƚ��
	int *m_Count; //����Ʈ ī��Ʈ(���� ���� Ƚ���� �ƴ�)
	CLUB_JOIN_INFO *m_ArrayJoinInfo;

	void Set(int TodayMax, int TodayCount, std::vector<CLUB_JOIN_INFO>& rArrayJoinInfo)
	{
		SetHeader(SV_CLUB_RESPONSE_MYSELF_JOIN_LIST);

		PushData(_PVAR(m_TodayMax), &TodayMax, sizeof(*m_TodayMax));
		PushData(_PVAR(m_TodayCount), &TodayCount, sizeof(*m_TodayCount));

		int count = rArrayJoinInfo.size();

		PushData( _PVAR(m_Count), &count, sizeof(*m_Count));

		size_t size = rArrayJoinInfo.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData( _PVAR(m_ArrayJoinInfo), &rArrayJoinInfo[i], sizeof(CLUB_JOIN_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_TodayMax), sizeof(*m_TodayMax));
		TakeData(_PVAR(m_TodayCount), sizeof(*m_TodayCount));
		TakeData(_PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayJoinInfo), sizeof(CLUB_JOIN_INFO) * (*m_Count));
	}
};

class CCL_CLUB_REQUEST_MYSELF_JOIN_LIST : public CCommMsg
{
	//���� ���� �ʴ��� ����Ʈ ��û(������)
public:
	int *m_MaxCount; //�ʿ信 ���� �ʴ��� ����Ʈ�� �ִ�ġ�� �־��ָ� ��. -1�ϰ�� ������(�ִ°� �� ������)

	void Set(int MaxCount)
	{
		SetHeader(CL_CLUB_REQUEST_MYSELF_JOIN_LIST);

		PushData(_PVAR(m_MaxCount), &MaxCount, sizeof(*m_MaxCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_MaxCount), sizeof(*m_MaxCount));
	}
};


class CSV_CLUB_RESPONSE_JOIN_LIST : public CCommMsg
{
	//�ش� ������ ���� ���� ��û ����Ʈ�� �����ش�.
public:
	int *m_Count;
	CLUB_JOIN_INFO *m_ArrayJoinInfo;

	void Set(std::vector<CLUB_JOIN_INFO>& rArrayJoinInfo)
	{
		SetHeader(SV_CLUB_RESPONSE_JOIN_LIST);

		int count = rArrayJoinInfo.size();

		PushData( _PVAR(m_Count), &count, sizeof(*m_Count));

		size_t size = rArrayJoinInfo.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData( _PVAR(m_ArrayJoinInfo), &rArrayJoinInfo[i], sizeof(CLUB_JOIN_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayJoinInfo), sizeof(CLUB_JOIN_INFO) * (*m_Count));
	}
};

class CCL_CLUB_REQUEST_JOIN_LIST : public CCommMsg
{
	//���� ���� ���Կ�û ����Ʈ
public:
	int *m_MaxCount; //�ʿ信 ���� �ʴ��� ����Ʈ�� �ִ�ġ�� �־��ָ� ��. -1�ϰ�� ������(�ִ°� �� ������)

	void Set(int MaxCount)
	{
		SetHeader(CL_CLUB_REQUEST_JOIN_LIST);

		PushData(_PVAR(m_MaxCount), &MaxCount, sizeof(*m_MaxCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_MaxCount), sizeof(*m_MaxCount));
	}
};

class CSV_CLUB_ACCEPT_INVITE_RESULT : public CCommMsg
{
	//Ŭ�� ���� ������ ���� ����� �����ش�.(������ ������ �ο��ʰ� ���� ������ ������ �ȵɼ��� �����Ƿ�)
public:
	int *m_Result; //0.����, -1000.���ο���, -1001.�������� �ʴ� �ʴ�, -1002.Ŭ����������, -1003.���������ο� �ʰ�
	int *m_ActionCode; //���� Ȥ�� �ź�.. enum CLUB_ACTION_CODE ���� ������ ��.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY
	///Ŭ�����Կ� �����ؾ� �Ʒ� �����͵��� ��ȿ�ϴ�.
	int *m_Club_ID; //Ŭ��ID... �츮�� �˰� �ִ� ������ ID�� �ƴ϶�, ������ ����ũ ���̴�.
	char *m_ClubName; //Ŭ���̸�
	
	void Set(int Result, int ActionCode, int Club_ID, const char *ClubName)
	{
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};

		SetHeader(SV_CLUB_ACCEPT_INVITE_RESULT);

		strcpy(strClubName, ClubName);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_ActionCode), &ActionCode, sizeof(*m_ActionCode));
		PushData(_PVAR(m_Club_ID), &Club_ID, sizeof(*m_Club_ID));
		PushData(_PVAR(m_ClubName), strClubName, CHAR_BUFFER_CLUB_NAME);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_ActionCode), sizeof(*m_ActionCode));
		TakeData(_PVAR(m_Club_ID), sizeof(*m_Club_ID));
		TakeData(_PVAR(m_ClubName), CHAR_BUFFER_CLUB_NAME);
	}
};

class CCL_CLUB_ACCEPT_INVITE : public CCommMsg
{
	//Ŭ�� �ʴ븦 �����Ѵ�.
public:
	char *m_SendID; //�ʴ����� ���� ����ID
	INT64 *m_InvteSeq; //�ʴ� Sequence ��ȣ
	int *m_ClubID; //Ŭ�� ���� ��ȣ
	int *m_ActionCode; //���� Ȥ�� �ź�.. enum CLUB_ACTION_CODE ���� ������ ��.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY

	void Set(const char *SendID, INT64 InviteSeq, int ClubID, int ActionCode)
	{
		char strSendID[NICK_LEN]={0,};

		SetHeader(CL_CLUB_ACCEPT_INVITE);

		if(strlen(strSendID) < NICK_LEN) strcpy(strSendID, SendID);

		PushData(_PVAR(m_SendID), strSendID, NICK_LEN);
		PushData(_PVAR(m_InvteSeq), &InviteSeq, sizeof(*m_InvteSeq));
		PushData(_PVAR(m_ClubID), &ClubID, sizeof(*m_ClubID));
		PushData(_PVAR(m_ActionCode), &ActionCode, sizeof(*m_ActionCode));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_SendID), NICK_LEN);
		TakeData(_PVAR(m_InvteSeq), sizeof(*m_InvteSeq));
		TakeData(_PVAR(m_ClubID), sizeof(*m_ClubID));
		TakeData(_PVAR(m_ActionCode), sizeof(*m_ActionCode));
	}
};

class CSV_CLUB_INVITE_RESULT : public CCommMsg
{
	//Ŭ�� �ʴ뿡 ���� ����̴�.
public:
	int *m_Result; //0.����, -1001.Ŭ����������, -1002.���������ο��ʰ�, -1003.�̹��ʴ�������, 
					//-1004.�ʴ��ڰ� �ٸ�Ŭ�� ������, -1005.�ʴ��ڰ� �ٸ� Ŭ���� ������, -1006.�������� �ʴ� ������, -1007.���� Ŭ������ Ż���� ����
	char *m_InviteID; //�ʴ��� ���� ID

	void Set(int Result, const char *InviteID)
	{
		char strInviteID[NICK_LEN]={0,};

		SetHeader(SV_CLUB_INVITE_RESULT);

		if(strlen(strInviteID) < NICK_LEN) strcpy(strInviteID, InviteID);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_InviteID), strInviteID, NICK_LEN);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_InviteID), NICK_LEN);
	}
};

class CCL_CLUB_INVITE : public CCommMsg
{
	//Ŭ�� ���� �ʴ����� ������.
	//�� ��Ŷ�� ������ ������ Ŭ���� ���� ���¿��߸� �Ѵ�.
public:
	char *m_InviteID; //�ʴ��� ���� ID

	void Set(const char *InviteID)
	{
		char strInviteID[NICK_LEN]={0,};

		SetHeader(CL_CLUB_INVITE);

		if(strlen(strInviteID) < NICK_LEN) strcpy(strInviteID, InviteID);

		PushData(_PVAR(m_InviteID), strInviteID, NICK_LEN);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_InviteID), NICK_LEN);
	}
};

class CSV_CLUB_ACCEPT_JOIN_RESULT : public CCommMsg
{
	//Ŭ�� ���Կ�û ������ ���� ����̴�.
public:
	int *m_Result; //0.����, -1000.���ο���,-1001.Ŭ����������, -1002.���������ο��ʰ�, -1003.�����ڰ� �ٸ�Ŭ�� ������, -1004.�����ڰ� �ٸ� Ŭ���� ������
	int *m_ActionCode; //���� Ȥ�� �ź�.. enum CLUB_ACTION_CODE ���� ������ ��.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY
	char *m_JoinID; //���� ��û�� ���� ����ID

	void Set(int Result, const char *JoinID, int ActionCode)
	{
		char strJoinID[NICK_LEN]={0,};

		SetHeader(SV_CLUB_ACCEPT_JOIN_RESULT);

		if(strlen(strJoinID) < NICK_LEN) strcpy(strJoinID, JoinID);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_ActionCode), &ActionCode, sizeof(*m_ActionCode));
		PushData(_PVAR(m_JoinID), strJoinID, NICK_LEN);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_ActionCode), sizeof(*m_ActionCode));
		TakeData(_PVAR(m_JoinID), NICK_LEN);
	}
};

class CCL_CLUB_ACCEPT_JOIN : public CCommMsg
{
	//Ŭ�� ���Կ�û�� �����Ѵ�.
	//���� ȸ���� �� ��Ŷ�� ���� �� ����
public:
	char *m_JoinID; //���� ��û�� ���� ����ID
	char *m_ClubName; //������ Ŭ�� �̸�
	INT64 *m_JoinSeq; //���Կ�û Sequence ��ȣ
	int *m_ActionCode; //���� Ȥ�� �ź�.. enum CLUB_ACTION_CODE ���� ������ ��.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY

	void Set(const char *JoinID, const char *ClubName, INT64 JoinSeq, int ActionCode)
	{
		char strJoinID[NICK_LEN]={0,};
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};

		SetHeader(CL_CLUB_ACCEPT_JOIN);

		if(strlen(strJoinID) < NICK_LEN) strcpy(strJoinID, JoinID);
		strcpy(strClubName, ClubName);

		PushData(_PVAR(m_JoinID), strJoinID, NICK_LEN);
		PushData(_PVAR(m_ClubName), strClubName, CHAR_BUFFER_CLUB_NAME);
		PushData(_PVAR(m_JoinSeq), &JoinSeq, sizeof(*m_JoinSeq));
		PushData(_PVAR(m_ActionCode), &ActionCode, sizeof(*m_ActionCode));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_JoinID), NICK_LEN);
		TakeData(_PVAR(m_ClubName), CHAR_BUFFER_CLUB_NAME);
		TakeData(_PVAR(m_JoinSeq), sizeof(*m_JoinSeq));
		TakeData(_PVAR(m_ActionCode), sizeof(*m_ActionCode));
	}
};

class CSV_CLUB_JOIN_RESULT : public CCommMsg
{
	//Ŭ���� ���� ��û�� ���� ����̴�.
public:
	int *m_Result; //0.����, -1000.�̹̰��Ե� Ŭ���� ����, -1001.��û�� ������ �ʰ��� -1002.�̹� �ش�Ŭ���� ���Խ�û�� ����
	int *m_Club_ID; //Ŭ��ID

	void Set(int Result, int Club_ID)
	{
		SetHeader(SV_CLUB_JOIN_RESULT);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_Club_ID), &Club_ID, sizeof(*m_Club_ID));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_Club_ID), sizeof(*m_Club_ID));
	}
};

class CCL_CLUB_JOIN : public CCommMsg
{
	//Ŭ���� ���� ��û���� ������.
	//�� ��Ŷ�� ������ ������ �ڿ��λ���(Ŭ������ �ƴϰ� ������ �ƴ�) ���� �Ѵ�.
public:
	int *m_Club_ID; //Ŭ��ID

	void Set(int Club_ID)
	{
		SetHeader(CL_CLUB_JOIN);

		PushData(_PVAR(m_Club_ID), &Club_ID, sizeof(*m_Club_ID));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Club_ID), sizeof(*m_Club_ID));
	}
};

class CSV_CLUB_CREATE_RESULT : public CCommMsg
{
	//Ŭ�� ������ ���� ���
public:
	int *m_Result; //0.����, -1000.���ο���, -1001.�̴̹ٸ�Ŭ���� ������, -1002.���ϻ����Ұ�
	///Ŭ�������� �����ؾ� �Ʒ� �����͵��� ��ȿ�ϴ�.
	char *m_Club_Name; //Ŭ���̸�

	void Set(int Result, const char* Club_Name)
	{
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};

		SetHeader(SV_CLUB_CREATE_RESULT);

		strcpy(strClubName, Club_Name);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_Club_Name), strClubName, CHAR_BUFFER_CLUB_NAME);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_Club_Name), CHAR_BUFFER_CLUB_NAME);
	}
};

class CCL_CLUB_CREATE : public CCommMsg
{
	//Ŭ�� ������ ��û�Ѵ�.
	//Ŭ�� �̸��� ������ �г������� �ڵ� ��ġ �ǹǷ�, SET�� �����Ͱ� �ƹ��͵� ����
public:
	void Set()
	{
		SetHeader(CL_CLUB_CREATE);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
	}
};

class CSV_CLUB_RESPONSE_DETAIL_INFO : public CCommMsg
{
	//���� ���� Ŭ���� �� ������ �����ش�.
	//Ŭ�� ��û�Ҷ� ���� DB�� �ֽ� �����͸� �ε��Ͽ� �����ش�.
public:
	char *m_Club_Name; //Ŭ���̸�
	int *m_Club_BossLevel; //Ŭ��BOSS�� VIP �����̴�.
	int *m_Club_MemberGrade; //Ŭ���� ��� ����̴�.enum CLUB_MEMBER_GRADE �� �����Ǵ� ��..CLUB_MEMBER_GRADE_BOSS,CLUB_MEMBER_GRADE_NORMAL,CLUB_MEMBER_GRADE_NONE
	int *m_Club_MemberMax; //Ŭ���� ������ �� �ִ� �ִ� �ο�
	int *m_Club_MemberCount; //Ŭ���� ������ ���� �ο�
	INT64 *m_Club_Boss_GetGold_Today; //���� Ŭ�����鿡 ���� Boss���� PayBack�� ��� �Ѿ��̴�.
	INT64 *m_Club_Boss_GetGold_Yesterday; //���� Ŭ�����鿡 ���� Boss���� PayBack�� ��� �Ѿ��̴�.

	int *m_Club_Next_MemberMax; //���� ��޿��� ������ �ִ� Ŭ�� �ִ� �ο�
	float *m_Boss_PayBack_Rate; //������ ���̹� ����(%����)
	float *m_Member_AddGet_Rate; //����� �Ǹ鼭 �ްԵǴ� �߰� ���� ����(%)

	int *m_Count; //Ŭ���� ����Ʈ�� ���� ī��Ʈ.. �Ϲ����� ��� �� m_Club_MemberCount ���� ��ġ�ϰ�����, �� ���� ���� ������ ī��Ʈ�� ���� ���̹Ƿ�,
	             //�Ʒ� Array�� �Ľ��Ҷ�, �� ���� �����ϸ� ��Ȯ�ҵ�.
	CLUB_MEMBER_INFO *m_ArrayClubMemberInfo; //������ Ŭ���� ����Ʈ�̴�.

	void Set(const char *Club_Name, int Club_BossLevel, int Club_MemberGrade, int Club_MemberMax, int Club_MemberCount, INT64 Club_Boss_GetGold_Today, 
		INT64 Club_Boss_GetGold_Yesterday, int Club_Next_MemberMax, float Boss_PayBack_Rate, float Member_AddGet_Rate, std::vector<CLUB_MEMBER_INFO>& rArrayMemberInfo)
	{
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};

		SetHeader(SV_CLUB_RESPONSE_DETAIL_INFO);

		strcpy(strClubName, Club_Name);

		PushData(_PVAR(m_Club_Name), strClubName, CHAR_BUFFER_CLUB_NAME);
		PushData(_PVAR(m_Club_BossLevel), &Club_BossLevel, sizeof(*m_Club_BossLevel));
		PushData(_PVAR(m_Club_MemberGrade), &Club_MemberGrade, sizeof(*m_Club_MemberGrade));
		PushData(_PVAR(m_Club_MemberMax), &Club_MemberMax, sizeof(*m_Club_MemberMax));
		PushData(_PVAR(m_Club_MemberCount), &Club_MemberCount, sizeof(*m_Club_MemberCount));
		PushData(_PVAR(m_Club_Boss_GetGold_Today), &Club_Boss_GetGold_Today, sizeof(*m_Club_Boss_GetGold_Today));
		PushData(_PVAR(m_Club_Boss_GetGold_Yesterday), &Club_Boss_GetGold_Yesterday, sizeof(*m_Club_Boss_GetGold_Yesterday));

		PushData(_PVAR(m_Club_Next_MemberMax), &Club_Next_MemberMax, sizeof(*m_Club_Next_MemberMax));
		PushData(_PVAR(m_Boss_PayBack_Rate), &Boss_PayBack_Rate, sizeof(*m_Boss_PayBack_Rate));
		PushData(_PVAR(m_Member_AddGet_Rate), &Member_AddGet_Rate, sizeof(*m_Member_AddGet_Rate));

		int count = rArrayMemberInfo.size();

		PushData(_PVAR(m_Count), &count, sizeof(*m_Count));

		size_t size = rArrayMemberInfo.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData( _PVAR(m_ArrayClubMemberInfo), &rArrayMemberInfo[i], sizeof(CLUB_MEMBER_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Club_Name), CHAR_BUFFER_CLUB_NAME);
		TakeData(_PVAR(m_Club_BossLevel), sizeof(*m_Club_BossLevel));
		TakeData(_PVAR(m_Club_MemberGrade), sizeof(*m_Club_MemberGrade));
		TakeData(_PVAR(m_Club_MemberMax), sizeof(*m_Club_MemberMax));
		TakeData(_PVAR(m_Club_MemberCount), sizeof(*m_Club_MemberCount));
		TakeData(_PVAR(m_Club_Boss_GetGold_Today), sizeof(*m_Club_Boss_GetGold_Today));
		TakeData(_PVAR(m_Club_Boss_GetGold_Yesterday), sizeof(*m_Club_Boss_GetGold_Yesterday));

		TakeData(_PVAR(m_Club_Next_MemberMax), sizeof(*m_Club_Next_MemberMax));
		TakeData(_PVAR(m_Boss_PayBack_Rate), sizeof(*m_Boss_PayBack_Rate));
		TakeData(_PVAR(m_Member_AddGet_Rate), sizeof(*m_Member_AddGet_Rate));

		TakeData(_PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayClubMemberInfo), sizeof(CLUB_MEMBER_INFO) * (*m_Count));
	}
};

class CCL_CLUB_REQUEST_DETAIL_INFO : public CCommMsg
{
	//���� ���� Ŭ���� �� ������ ��û�Ѵ�.
	//��û�� ������ ������ ����
public:
	void Set()
	{
		SetHeader(CL_CLUB_REQUEST_DETAIL_INFO);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;
	}
};

class CSV_CLUB_MYINFO : public CCommMsg
{
	//�� Ŭ�� ���� �����̴�.
public:
	int *m_InfoType; //�ش� ������ �� ������ Ÿ���ڵ��̴�.enum CLUB_MYINFO_TYPE �� �����Ǵ� ��
	int *m_Club_ID; //Ŭ��ID... �츮�� �˰� �ִ� ������ ID�� �ƴ϶�, ������ ����ũ ���̴�.
	char *m_Club_Name; //Ŭ���̸�
	char *m_Club_BossID; //Ŭ���� BOSS ID�̴�.
	int *m_Club_MemberGrade; //Ŭ���� ��� ����̴�.enum CLUB_MEMBER_GRADE �� �����Ǵ� ��..CLUB_MEMBER_GRADE_BOSS,CLUB_MEMBER_GRADE_NORMAL,CLUB_MEMBER_GRADE_NONE
	bool *m_Club_Enable_Create; //���� �� ���°� Ŭ�� ���� �������� ���θ� �˷���.. VIP����� ������ �̹� ������ ��쿡�� false�� ���õǾ� ��
	float *m_Member_AddGet_Rate; //����� �Ǹ鼭 �ްԵǴ� �߰� ���� ����(%)
	float *m_Boss_PayBack_Rate; //������ ���̹� ����(%����) - ������ �Ǹ� �޴� ��������
	
	void Set(int InfoType, int Club_ID, const char* Club_Name, const char* Club_BossID, int Club_MemberGrade, bool Club_Enable_Create, float Member_AddGet_Rate, float Boss_PayBack_Rate)
	{
		char strClubName[CHAR_BUFFER_CLUB_NAME]={0,};
		char strClubBossID[CHAR_BUFFER_CLUB_BOSS_ID]={0,};

		SetHeader(SV_CLUB_MYINFO);

		strcpy(strClubName, Club_Name);
		strcpy(strClubBossID, Club_BossID);

		PushData(_PVAR(m_InfoType), &InfoType, sizeof(*m_InfoType));
		PushData(_PVAR(m_Club_ID), &Club_ID, sizeof(*m_Club_ID));
		PushData(_PVAR(m_Club_Name), strClubName, CHAR_BUFFER_CLUB_NAME);
		PushData(_PVAR(m_Club_BossID), strClubBossID, CHAR_BUFFER_CLUB_BOSS_ID);
		PushData(_PVAR(m_Club_MemberGrade), &Club_MemberGrade, sizeof(*m_Club_MemberGrade));
		PushData(_PVAR(m_Club_Enable_Create), &Club_Enable_Create, sizeof(*m_Club_Enable_Create));
		PushData(_PVAR(m_Member_AddGet_Rate), &Member_AddGet_Rate, sizeof(*m_Member_AddGet_Rate));
		PushData(_PVAR(m_Boss_PayBack_Rate), &Boss_PayBack_Rate, sizeof(*m_Boss_PayBack_Rate));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_InfoType), sizeof(*m_InfoType));
		TakeData(_PVAR(m_Club_ID), sizeof(*m_Club_ID));
		TakeData(_PVAR(m_Club_Name), CHAR_BUFFER_CLUB_NAME);
		TakeData(_PVAR(m_Club_BossID), CHAR_BUFFER_CLUB_BOSS_ID);
		TakeData(_PVAR(m_Club_MemberGrade), sizeof(*m_Club_MemberGrade));
		TakeData(_PVAR(m_Club_Enable_Create), sizeof(*m_Club_Enable_Create));
		TakeData(_PVAR(m_Member_AddGet_Rate), sizeof(*m_Member_AddGet_Rate));
		TakeData(_PVAR(m_Boss_PayBack_Rate), sizeof(*m_Boss_PayBack_Rate));
	}
};

class CSV_TOTAL_REQUEST_FAIL_RESULT : public CCommMsg
{
	//���� ������ ���� Request�Ͽ� Response�� �޾ƾ� �ϴ°��
	//�������� ��Ȳ������ Response�� ��������, �ƴҰ�� ���� ����� �ʿ��Ҷ��� �ִ�.
	//���� �̹� Ŭ�������� ���� ��û��, Ŭ�������̰ų� ����� �ƴҰ�� Response�� �Ȱ��� �Ǵµ�,
	//Ư���� ���� ��Ŷ�� �߼۵��� ������, Ŭ�� ���忡���� �����ϰ� �����ϱⰡ ��ƴ�.
public:
	int *m_Result_Msg_ID; //�޼��� ID�̴�. �� ��Ŷ ����� ���� ����. ��) SV_CLUB_MYINFO �̷���
	int *m_CommonErrCode; //���� �ڵ��̴�. enum COMMON_ERR_CODE�� ���ǵ� ���� ��

	void Set(int Result_Msg_ID, int CommonErrCode)
	{
		SetHeader(SV_TOTAL_REQUEST_FAIL_RESULT);

		PushData(_PVAR(m_Result_Msg_ID), &Result_Msg_ID, sizeof(*m_Result_Msg_ID));
		PushData(_PVAR(m_CommonErrCode), &CommonErrCode, sizeof(*m_CommonErrCode));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result_Msg_ID), sizeof(*m_Result_Msg_ID));
		TakeData(_PVAR(m_CommonErrCode), sizeof(*m_CommonErrCode));
	}
};
////////////////////////////////////////////////////////

////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
class CSV_PROMOTION_COMMON_REWARD_ALL_NOTICE : public CCommMsg
{
	//������ ���������� �޾Ҵٴ� ������ ��� �������� ��Ƽ�ϴ� �뵵�̴�.
public:
	char *m_NickName;
	char *m_GameName;
	char *m_ChannelName;
	char *m_RewardName;

	void Set(const char *NickName, const char *GameName, const char *ChannelName, const char *RewardName)
	{
		SetHeader(SV_PROMOTION_COMMON_REWARD_ALL_NOTICE);

		char strNickName[NICK_LEN]={0,};
		char strGameName[CHAR_BUFFER_GAME_NAME]={0,};
		char strChannelName[CHAR_BUFFER_CHANNEL_NAME]={0,};
		char strRewardName[CHAR_BUFFER_REWARD_NAME]={0,};

		strcpy(strNickName, NickName);
		strcpy(strGameName, GameName);
		strcpy(strChannelName, ChannelName);
		strcpy(strRewardName, RewardName);

		PushData(_PVAR(m_NickName), strNickName, NICK_LEN);
		PushData(_PVAR(m_GameName), strGameName, CHAR_BUFFER_GAME_NAME);
		PushData(_PVAR(m_ChannelName), strChannelName, CHAR_BUFFER_CHANNEL_NAME);
		PushData(_PVAR(m_RewardName), strRewardName, CHAR_BUFFER_REWARD_NAME);
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_NickName), NICK_LEN);
		TakeData(_PVAR(m_GameName), CHAR_BUFFER_GAME_NAME);
		TakeData(_PVAR(m_ChannelName), CHAR_BUFFER_CHANNEL_NAME);
		TakeData(_PVAR(m_RewardName), CHAR_BUFFER_REWARD_NAME);
	}
};

class CSV_PROMOTION_COMMON_ATTENDANCE_GET_REWARD : public CCommMsg
{
	//�⼮�����̴�~!!
public:
	int *m_Result; //0.��������, 1.�ش������� DI�� ���� �ٸ� ������ �̹� ������ ������
	int *m_Attendance_ID; //�⼮�� ID
	int *m_Attendance_Type; //�⼮����, enum PROMOTION_ATTENDANCE_TYPE �� �ִ� ���̸� ATTENDANCE_NORMAL�� �Ϲݴ���, ATTENDANCE_CONTINUE�� �����⼮
	int *m_Attendance_Count; //�⼮����
	char *m_RewardType;
	//GM.���ӸӴ�, GD���, CP.����, DC.���α�, MBM.������̴�, MBS.����� �ǹ�, MBG.��������, MBD.��������̾�, GLP.Ȳ�ݺ��ָӴ�
	INT64 *m_RewardAmount; //����ݾ�.. ���� ����� �׷��Ÿ� ���ڶ�� ���� ��

	void Set(int Result, int Attendance_ID, int Attendance_Type, int Attendance_Count, const char *RewardType, INT64 RewardAmount)
	{
		SetHeader(SV_PROMOTION_COMMON_ATTENDANCE_GET_REWARD);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_Attendance_ID), &Attendance_ID, sizeof(*m_Attendance_ID));
		PushData(_PVAR(m_Attendance_Type), &Attendance_Type, sizeof(*m_Attendance_Type));
		PushData(_PVAR(m_Attendance_Count), &Attendance_Count, sizeof(*m_Attendance_Count));

		char strRewardType[CHAR_BUFFER_PROMOTION_COMMON_ATTENDANCE_REWARDTYPE]={0,};
		strcpy(strRewardType, RewardType);

		PushData(_PVAR(m_RewardType), strRewardType, CHAR_BUFFER_PROMOTION_COMMON_ATTENDANCE_REWARDTYPE);

		PushData(_PVAR(m_RewardAmount), &RewardAmount, sizeof(*m_RewardAmount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_Attendance_ID), sizeof(*m_Attendance_ID));
		TakeData(_PVAR(m_Attendance_Type), sizeof(*m_Attendance_Type));
		TakeData(_PVAR(m_Attendance_Count), sizeof(*m_Attendance_Count));
		TakeData(_PVAR(m_RewardType), CHAR_BUFFER_PROMOTION_COMMON_ATTENDANCE_REWARDTYPE);
		TakeData(_PVAR(m_RewardAmount), sizeof(*m_RewardAmount));
	}
};

class CSV_PROMOTION_COMMON_ATTENDANCE_USERINFO : public CCommMsg
{
	//�Ϲ������� �⼮�� �̿��Ͽ� �����ϴ� ���θ�ǿ� ���� �����̴�.
	//Ȥ�� �⼮�ΰ� ������ �����Ҽ��� �־�, �⼮�� ID�� �������´�.
public:
	int *m_Attendance_ID; //�⼮�� ID
	int *m_NormalCount; //�Ϲ� ����Ƚ��
	int *m_ContiCount; //���� ����Ƚ��
	bool *m_NewDayFlag; //�� ���� true�̸� �̹� ������ ������ ó���̶�� �ǹ�(���� �����ϴ� Ÿ�̹��̶� �ǹ��̴�.)

	void Set(int Attendance_ID, int NormalCount, int ContiCount, bool NewDayFlag)
	{
		SetHeader(SV_PROMOTION_COMMON_ATTENDANCE_USERINFO);

		PushData(_PVAR(m_Attendance_ID), &Attendance_ID, sizeof(*m_Attendance_ID));
		PushData(_PVAR(m_NormalCount), &NormalCount, sizeof(*m_NormalCount));
		PushData(_PVAR(m_ContiCount), &ContiCount, sizeof(*m_ContiCount));
		PushData(_PVAR(m_NewDayFlag), &NewDayFlag, sizeof(*m_NewDayFlag));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Attendance_ID), sizeof(*m_Attendance_ID));
		TakeData(_PVAR(m_NormalCount), sizeof(*m_NormalCount));
		TakeData(_PVAR(m_ContiCount), sizeof(*m_ContiCount));
		TakeData(_PVAR(m_NewDayFlag), sizeof(*m_NewDayFlag));
	}
};

class CSV_PROMOTION_COMMON_GAMECOUNT_RUNNING_INFO : public CCommMsg
{
	//�Ϲ������� �����Ǽ��� �̿��Ͽ� �����ϴ� ���θ�ǿ� ���� ���࿩�� �����̴�.
	//��κ��� ���θ���� �� ��Ŷ�� ������ �ʿ䰡 ������(�Ⱓ�� �����Ǿ� �ִٸ� �翬�� �������̴ϱ�)
	//2018 ġŲ �̺�Ʈ ó�� �Ϸ翡�� �ð��� ���� ���÷� �����ߴ� �����ϴ� ��� �ʿ��� �����̴�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ���� ��
	bool *m_RunningFlag;

	void Set(int PM_Code, bool RunningFlag)
	{
		SetHeader(SV_PROMOTION_COMMON_GAMECOUNT_RUNNING_INFO);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
		PushData(_PVAR(m_RunningFlag), &RunningFlag, sizeof(*m_RunningFlag));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_RunningFlag), sizeof(*m_RunningFlag));
	}
};

class CSV_PROMOTION_COMMON_GAMECOUNT_USERINFO : public CCommMsg
{
	//�Ϲ������� �����Ǽ��� �̿��Ͽ� �����ϴ� ���θ�ǿ� ���� �����̴�.
	//�� ���θ���� ���θ���ڵ�� �����Ѵ�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ���� ��
	int *m_CurrentGameCount; //���� �������� �Ǽ�. ������ ���޵Ǹ� 0���� �ʱ�ȭ �ȴ�.
	int *m_TodayRewardCount; //���� ���޹��� ����Ƚ��(�ʱ�ȭ �ð��� �Ǹ� 0���� �ʱ�ȭ �ȴ�.)
	int *m_NeedGameCount; //���� ������ �ޱ� ���� �ʿ��� ����Ƚ��
	int *m_TodayRewardLimtCount; //���� ���޹��� �� �ִ� ����Ƚ��
	int *m_TicketCount; //����ǵ� ����ϱ� ������ �����Ҽ� �ִ� ��ȭ�� ����
	int *m_TicketLimitCount; //����ǵ� ������ �� �ִ� �ִ�ġ

	void Set(int PM_Code, int CurrentGameCount, int TodayRewardCount, int NeedGameCount, int TodayRewardLimtCount, int TicketCount, int TicketLimitCount)
	{
		SetHeader(SV_PROMOTION_COMMON_GAMECOUNT_USERINFO);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
		PushData(_PVAR(m_CurrentGameCount), &CurrentGameCount, sizeof(*m_CurrentGameCount));
		PushData(_PVAR(m_TodayRewardCount), &TodayRewardCount, sizeof(*m_TodayRewardCount));
		PushData(_PVAR(m_NeedGameCount), &NeedGameCount, sizeof(*m_NeedGameCount));
		PushData(_PVAR(m_TodayRewardLimtCount), &TodayRewardLimtCount, sizeof(*m_TodayRewardLimtCount));
		PushData(_PVAR(m_TicketCount), &TicketCount, sizeof(*m_TicketCount));
		PushData(_PVAR(m_TicketLimitCount), &TicketLimitCount, sizeof(*m_TicketLimitCount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_CurrentGameCount), sizeof(*m_CurrentGameCount));
		TakeData(_PVAR(m_TodayRewardCount), sizeof(*m_TodayRewardCount));
		TakeData(_PVAR(m_NeedGameCount), sizeof(*m_NeedGameCount));
		TakeData(_PVAR(m_TodayRewardLimtCount), sizeof(*m_TodayRewardLimtCount));
		TakeData(_PVAR(m_TicketCount), sizeof(*m_TicketCount));
		TakeData(_PVAR(m_TicketLimitCount), sizeof(*m_TicketLimitCount));
	}
};

class CSV_PROMOTION_COMMON_GAMECOUNT_GET_REWARD : public CCommMsg
{
	//�Ϲ������� �����Ǽ��� �̿��Ͽ� �����ϴ� ���θ�ǿ� ���� ����߻� �����̴�.
	//�� ���θ���� ���θ���ڵ�� �����Ѵ�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ���� ��
	int *m_TodayRewardCount; //���� ���޹��� ����Ƚ��(�ʱ�ȭ �ð��� �Ǹ� 0���� �ʱ�ȭ �ȴ�.)
	int *m_TodayRewardLimtCount; //���� ���޹��� �� �ִ� ����Ƚ��
	char *m_RewardType; //����Ÿ�� = GM.���ӸӴ�, GD���, CP.����, DC.���α�
	INT64 *m_RewardAmount; //����ݾ�(���� ������ �Ѱ� ���޵Ǿ��ٸ� ���⿡ 1�� ��)
	char *m_RewardAddInfo; //���� ���� �߰� ����, ���� �������� ���޵Ǿ��ٸ� ������ȣ�� ��.
	SYSTEMTIME *m_ExpiredTime; //���� ���� �� �ִ� ������. ���ӸӴϳ� ���� ��� ���� ��ǰ�̶�� �ǹ� ����

	void Set(int PM_Code, int TodayRewardCount, int TodayRewardLimtCount, const char *RewardType, INT64 RewardAmount, const char *RewardAddInfo, SYSTEMTIME &ExpiredTime)
	{
		SetHeader(SV_PROMOTION_COMMON_GAMECOUNT_GET_REWARD);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
		PushData(_PVAR(m_TodayRewardCount), &TodayRewardCount, sizeof(*m_TodayRewardCount));
		PushData(_PVAR(m_TodayRewardLimtCount), &TodayRewardLimtCount, sizeof(*m_TodayRewardLimtCount));

		char strRewardType[CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_REWARDTYPE]={0,};
		strcpy(strRewardType, RewardType);
		PushData(_PVAR(m_RewardType), strRewardType, CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_REWARDTYPE);
		
		PushData(_PVAR(m_RewardAmount), &RewardAmount, sizeof(*m_RewardAmount));
		
		char strRewardAddInfo[CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_ADDINFO]={0,};
		strcpy(strRewardAddInfo, RewardAddInfo);
		PushData(_PVAR(m_RewardAddInfo), strRewardAddInfo, CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_ADDINFO);
		
		PushData(_PVAR(m_ExpiredTime), &ExpiredTime, sizeof(*m_ExpiredTime));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_TodayRewardCount), sizeof(*m_TodayRewardCount));
		TakeData(_PVAR(m_TodayRewardLimtCount), sizeof(*m_TodayRewardLimtCount));
		TakeData(_PVAR(m_RewardType), CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_REWARDTYPE);
		TakeData(_PVAR(m_RewardAmount), sizeof(*m_RewardAmount));
		TakeData(_PVAR(m_RewardAddInfo), CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_ADDINFO);
		TakeData(_PVAR(m_ExpiredTime), sizeof(*m_ExpiredTime));
	}
};

class CCL_PROMOTION_COMMON_REQUEST_TICKET_GIFTLIST : public CCommMsg
{
	//��������� ������ �� �ִ� ��ǰ ����Ʈ�� ��û�Ѵ�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ���� ���� ��

	void Set(int PM_Code)
	{
		SetHeader(CL_PROMOTION_COMMON_REQUEST_TICKET_GIFTLIST);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
	}
};

class CSV_PROMOTION_COMMON_RESPONSE_TICKET_GIFTLIST : public CCommMsg
{
	//��������� ������ �� �ִ� ��ǰ ����Ʈ�� ������ �ش�.
	//�α��� �� ä���̵��� �ڵ����� �����ָ�, CCL_PROMOTION_COMMON_REQUEST_TICKET_GIFTLIST ��Ŷ���� ��û�ÿ��� ��������
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ��
	int *m_Count; //���䰹��
	TICKET_GIFT_INFO *m_ArrayTicketGiftList; //��ϵ� ��ǰ ����

	void Set(int PM_Code, std::vector<TICKET_GIFT_INFO>& rArrayGiftList)
	{
		SetHeader(SV_PROMOTION_COMMON_RESPONSE_TICKET_GIFTLIST);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));

		int Count = rArrayGiftList.size();
		PushData(_PVAR(m_Count), &Count, sizeof(*m_Count));

		size_t size = rArrayGiftList.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData(_PVAR(m_ArrayTicketGiftList), &rArrayGiftList[i], sizeof(TICKET_GIFT_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_Count), sizeof(*m_Count));

		TakeData(_PVAR(m_ArrayTicketGiftList), sizeof(TICKET_GIFT_INFO) * (*m_Count));
	}
};

class CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION_LIST : public CCommMsg
{
	//Ƽ������ ������ ��Ȳ�� ��û�Ѵ�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ��

	void Set(int PM_Code)
	{
		SetHeader(CL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION_LIST);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
	}
};

class CSV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST : public CCommMsg
{
	//�ش� ������ Ƽ�� ���� ��Ȳ�� �����ش�.
	//Ŭ�� ��û�� DB�� �ִ� �����͸� �״�� �����ִ� ������ ����
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ��
	int *m_MyTicketCount; //������ �ִ� �����(Ƽ��) ����
	int *m_Count; //���䰹��
	TICKET_SUBSCRIPTION_INFO *m_ArraySubscriptionList; //��ϵ� ���� ����

	void Set(int PM_Code, int MyTicketCount, std::vector<TICKET_SUBSCRIPTION_INFO>& rArraySubscriptionList)
	{
		SetHeader(SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
		PushData(_PVAR(m_MyTicketCount), &MyTicketCount, sizeof(*m_MyTicketCount));

		int Count = rArraySubscriptionList.size();
		PushData(_PVAR(m_Count), &Count, sizeof(*m_Count));

		size_t size = rArraySubscriptionList.size();
		for (size_t i = 0; i < size; ++i)
		{
			PushData(_PVAR(m_ArraySubscriptionList), &rArraySubscriptionList[i], sizeof(TICKET_SUBSCRIPTION_INFO));
		}
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_MyTicketCount), sizeof(*m_MyTicketCount));
		TakeData(_PVAR(m_Count), sizeof(*m_Count));

		TakeData(_PVAR(m_ArraySubscriptionList), sizeof(TICKET_SUBSCRIPTION_INFO) * (*m_Count));
	}
};

class CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION : public CCommMsg
{
	//������� ����Ͽ� ��ǰ�� �����Ѵ�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ��
	int *m_GiftIndex; //��ǰ ������ȣ

	void Set(int PM_Code, int GiftIndex)
	{
		SetHeader(CL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
		PushData(_PVAR(m_GiftIndex), &GiftIndex, sizeof(*m_GiftIndex));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_GiftIndex), sizeof(*m_GiftIndex));
	}
};

class CSV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION : public CCommMsg
{
	//���� ����� ������ �ش�.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE�� ���ǵ� ��
	int *m_GiftIndex; //��ǰ ������ȣ
	int *m_ResultCode; //0�̸� ���� ����
	int *m_SB_Count; //�ش� ��ǰ�� ��� �����ϰ� �Ǿ������� �˷���(���𼺰���)

	void Set(int PM_Code, int GiftIndex, int ResultCode, int SB_Count)
	{
		SetHeader(SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION);

		PushData(_PVAR(m_PM_Code), &PM_Code, sizeof(*m_PM_Code));
		PushData(_PVAR(m_GiftIndex), &GiftIndex, sizeof(*m_GiftIndex));
		PushData(_PVAR(m_ResultCode), &ResultCode, sizeof(*m_ResultCode));
		PushData(_PVAR(m_SB_Count), &SB_Count, sizeof(*m_SB_Count));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_PM_Code), sizeof(*m_PM_Code));
		TakeData(_PVAR(m_GiftIndex), sizeof(*m_GiftIndex));
		TakeData(_PVAR(m_ResultCode), sizeof(*m_ResultCode));
		TakeData(_PVAR(m_SB_Count), sizeof(*m_SB_Count));
	}
};
////////////////////////////////////////////////////////

////////////////////////////������� ������� 2018.02.20/////////////////////////////////////
class CSV_GOLD_TEMPMONEY_INFO : public CCommMsg
{
	//����� �ӽøӴ�(����) �����̴�.
public:
	INT64 *m_CurTempGold; //���� ������ �ӽ� ������ ��差�̴�. �� �ʵ�� ���� �����Ӵϸ� ����ȭ ��Ű�� ��
	INT64 *m_ThisAddTempGold; //�̹��� ��ȭ�Ǵ� ��差�̴�. 
	int *m_ThisAddTempGold_Remain; //�̹��� ��ȭ�Ǵ� ����� �Ҽ��� ���� �����̴�.
	//�Ѱ��� ����� �� �ΰ��� ������ + �Ǿ��ٴ� UI�� �����ϸ� �� (��) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);

	INT64 *m_MaxTempGold; //�ִ�� �������� �� �ִ� �ݾ��̴�. �� ���� m_CurTempGold �� ���Ͽ� �ִ�ġ ���޵��� UI ������ ���ָ� �ȴ�.

	void Set(INT64 CurTempGold, INT64 ThisAddTempGold, int ThisAddTempGold_Remain, INT64 MaxTempGold) 
	{
		SetHeader(SV_GOLD_TEMPMONEY_INFO);
	
		PushData(_PVAR(m_CurTempGold), &CurTempGold, sizeof(*m_CurTempGold));
		PushData(_PVAR(m_ThisAddTempGold), &ThisAddTempGold, sizeof(*m_ThisAddTempGold));
		PushData(_PVAR(m_ThisAddTempGold_Remain), &ThisAddTempGold_Remain, sizeof(*m_ThisAddTempGold_Remain));
		PushData(_PVAR(m_MaxTempGold), &MaxTempGold, sizeof(*m_MaxTempGold));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_CurTempGold), sizeof(*m_CurTempGold));
		TakeData(_PVAR(m_ThisAddTempGold), sizeof(*m_ThisAddTempGold));
		TakeData(_PVAR(m_ThisAddTempGold_Remain), sizeof(*m_ThisAddTempGold_Remain));
		TakeData(_PVAR(m_MaxTempGold), sizeof(*m_MaxTempGold));
	}
};

class CSV_GOLD_TEMPMONEY_REAL_UPDATE : public CCommMsg
{
	//�ӽð�尡 �������� �ݿ��Ǿ����� �˷��ش�.
public:
	INT64 *m_TempGold; //������Ʈ�Ǵ� �ӽ�(������) ��� �ݾ��̴�.
	INT64 *m_RealGold; //�ջ�Ǿ� ���� �����ϰԵ� ��� �ݾ��̴�.(���� �����Ӵ� ���� �Ѿ�)

	void Set(INT64 TempGold, INT64 RealGold) 
	{
		SetHeader(SV_GOLD_TEMPMONEY_REAL_UPDATE);

		PushData(_PVAR(m_TempGold), &TempGold, sizeof(*m_TempGold));
		PushData(_PVAR(m_RealGold), &RealGold, sizeof(*m_RealGold));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_TempGold), sizeof(*m_TempGold));
		TakeData(_PVAR(m_RealGold), sizeof(*m_RealGold));
	}
};
///////////////////////////////////////////////////////////////


////////////////////////////���� �������, ��������� 2018.02.27/////////////////////////////////////
class CSV_GOLD_HELP_REWARD_INFO : public CCommMsg
{
	//��� ������ ���θ�� �����̴�.(���������� ���θ� �˷���)
public:
	bool *m_Playing; //���� ���������� ����
	char *m_StartDay;
	char *m_EndDay;

	void Set(bool Playing, const char *StartDay, const char *EndDay) 
	{
		SetHeader(SV_GOLD_HELP_REWARD_INFO);

		PushData(_PVAR(m_Playing), &Playing, sizeof(*m_Playing));

		char strStartDay[CHAR_BUFFER_DATE_LEN]={0,};
		char strEndDay[CHAR_BUFFER_DATE_LEN]={0,};

		strcpy(strStartDay, StartDay);
		strcpy(strEndDay, EndDay);

		PushData(_PVAR(m_StartDay), strStartDay, CHAR_BUFFER_DATE_LEN);
		PushData(_PVAR(m_EndDay), strEndDay, CHAR_BUFFER_DATE_LEN);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Playing), sizeof(*m_Playing));
		TakeData(_PVAR(m_StartDay), CHAR_BUFFER_DATE_LEN);
		TakeData(_PVAR(m_EndDay), CHAR_BUFFER_DATE_LEN);
	}
};

class CSV_GOLD_HELP_REWARD : public CCommMsg
{
	//��� ���� ���� ��÷ ��Ŷ�̴�.
public:
	int *m_Grade; //����.. �� ���� -1�̸� ���̴�...
	INT64 *m_RewardGold; //������̴�.

	void Set(int Grade, INT64 RewardGold) 
	{
		SetHeader(SV_GOLD_HELP_REWARD);

		PushData(_PVAR(m_Grade), &Grade, sizeof(*m_Grade));
		PushData(_PVAR(m_RewardGold), &RewardGold, sizeof(*m_RewardGold));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Grade), sizeof(*m_Grade));
		TakeData(_PVAR(m_RewardGold), sizeof(*m_RewardGold));
	}
};
///////////////////////////////////////


////////////////////////////Ȧ¦�̴ϰ��� �̺�Ʈ 2018.03.12/////////////////////////////////////
class CSV_PM_ODDEVEN_START_INFO : public CCommMsg
{
	//���� Ȧ¦ ������ ������ �� �ִ����� �˷��ش�.
	//�� ��Ŷ�� true�� ������ ���������� �� �� �ִٰ� �Ǵ��Ͽ�, ������ ���� ���� ��û ��Ŷ�� ������ �ȴ�.
public:
	bool *m_StartEnable;
	bool *m_NeedCaptcha; //ĸ�������� �ʿ����� �����̴�.

	void Set(bool StartEnable, bool NeedCaptcha) 
	{
		SetHeader(SV_PM_ODDEVEN_START_INFO);

		PushData(_PVAR(m_StartEnable), &StartEnable, sizeof(*m_StartEnable));
		PushData(_PVAR(m_NeedCaptcha), &NeedCaptcha, sizeof(*m_NeedCaptcha));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_StartEnable), sizeof(*m_StartEnable));
		TakeData(_PVAR(m_NeedCaptcha), sizeof(*m_NeedCaptcha));
	}
};

class CCL_PM_ODDEVEN_REQUEST_START : public CCommMsg
{
	//���ӽ����� ��û�Ѵ�.
public:
	////////////////////////////����⼮�� CAPTCHA 2018.07.31/////////////////////////////////////
	char *m_CaptchaKey; //ĸ�� ���������� ���� ����Ű�� �־� �ش�.
	//////////////////////////////

	void Set(const char *CaptchaKey) 
	{
		SetHeader(CL_PM_ODDEVEN_REQUEST_START);

		char strCaptchaKey[CHAR_BUFFER_CAPTCHA_KEY_LEN]={0,};
		strcpy(strCaptchaKey, CaptchaKey);

		PushData(_PVAR(m_CaptchaKey), strCaptchaKey, CHAR_BUFFER_CAPTCHA_KEY_LEN);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_CaptchaKey), CHAR_BUFFER_CAPTCHA_KEY_LEN);
	}
};

class CSV_PM_ODDEVEN_RESPONSE_START : public CCommMsg
{
	//���ӽ��� ��û�� ���� ����̴�.
public:
	int *m_ErrCode; //0.�������, 1.�����̹�������, 2.��Ÿ����, 10.ĸ���ڵ����ġ enum PROMOTION_ODDEVEN_START_ERRCODE ����

	void Set(int ErrCode) 
	{
		SetHeader(SV_PM_ODDEVEN_RESPONSE_START);

		PushData(_PVAR(m_ErrCode), &ErrCode, sizeof(*m_ErrCode));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ErrCode), sizeof(*m_ErrCode));
	}
};


class CSV_PM_ODDEVEN_START_ROUND : public CCommMsg
{
	//�� ���尡 ����Ǿ����� �˷��ش�.
	//�� ��Ŷ�� ������, ������ ������ �޾� Ȧ Ȥ�� ¦�� ����� ������ �����ָ� �ȴ�.
public:
	int *m_Round; //�ܰ� 1 ~ 8���� ����
	INT64 *m_CurrentReward; //���ݱ��� Ȯ���� ����ݾ�(���ý���, Ȥ�� �������⸦ �ϸ� �ް� �Ǵ� �����)
	INT64 *m_WinReward; //�̹� ���忡�� ���ü����� �ްԵǴ� ����ݾ��̴�.
	int *m_BetTime; //�������ѽð��̴�(��), ���� -1�� ���� ������ ���ٴ� �ǹ�

	void Set(int Round, INT64 CurrentReward, INT64 WinReward, int BetTime) 
	{
		SetHeader(SV_PM_ODDEVEN_START_ROUND);

		PushData(_PVAR(m_Round), &Round, sizeof(*m_Round));
		PushData(_PVAR(m_CurrentReward), &CurrentReward, sizeof(*m_CurrentReward));
		PushData(_PVAR(m_WinReward), &WinReward, sizeof(*m_WinReward));
		PushData(_PVAR(m_BetTime), &BetTime, sizeof(*m_BetTime));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Round), sizeof(*m_Round));
		TakeData(_PVAR(m_CurrentReward), sizeof(*m_CurrentReward));
		TakeData(_PVAR(m_WinReward), sizeof(*m_WinReward));
		TakeData(_PVAR(m_BetTime), sizeof(*m_BetTime));
	}
};

class CCL_PM_ODDEVEN_USER_BET : public CCommMsg
{
	//Ȧ�̳�, ¦�̳� ������ �����̴�.
public:
	int *m_Round; //�ܰ� 1 ~ 8���� ����
	int *m_BetSide; //���û��̵� 0.¦��, 1.Ȧ�� enum PROMOTION_ODDEVEN_BET_SIDE ����

	void Set(int Round, int BetSide) 
	{
		SetHeader(CL_PM_ODDEVEN_USER_BET);

		PushData(_PVAR(m_Round), &Round, sizeof(*m_Round));
		PushData(_PVAR(m_BetSide), &BetSide, sizeof(*m_BetSide));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Round), sizeof(*m_Round));
		TakeData(_PVAR(m_BetSide), sizeof(*m_BetSide));
	}
};

class CSV_PM_ODDEVEN_USER_BET_RESULT : public CCommMsg
{
	//�������ÿ� ���� ����̴�.
	//������ �̹� ���� ���ÿ� ���� �����.. �������� Ȯ�� ��Ŷ�� ������ �и�~!!
public:
	bool *m_BetSuccess; //��������
	int *m_Round; //������ ����
	int *m_CardNum; //�̹��� ����� �ο��� ī���ȣ�̴�.(������Ŀ ī�� �ε����� ������)
	INT64 *m_RoundReward; //������ ���忡 ���ü����� ȹ���ϰԵǴ� �����
	
	void Set(bool BetSuccess, int Round, int CardNum, INT64 RoundReward)
	{
		SetHeader(SV_PM_ODDEVEN_USER_BET_RESULT);

		PushData(_PVAR(m_BetSuccess), &BetSuccess, sizeof(*m_BetSuccess));
		PushData(_PVAR(m_Round), &Round, sizeof(*m_Round));
		PushData(_PVAR(m_CardNum), &CardNum, sizeof(*m_CardNum));
		PushData(_PVAR(m_RoundReward), &RoundReward, sizeof(*m_RoundReward));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_BetSuccess), sizeof(*m_BetSuccess));
		TakeData(_PVAR(m_Round), sizeof(*m_Round));
		TakeData(_PVAR(m_CardNum), sizeof(*m_CardNum));
		TakeData(_PVAR(m_RoundReward), sizeof(*m_RoundReward));
	}
};

class CSV_PM_ODDEVEN_FIX_REWARD : public CCommMsg
{
	//���������� ���� ������ �Ϸ� �Ǿ���~!!
public:
	int *m_LastRound; //���������� ������ ����
	INT64 *m_RewardMoney; //�������� ���޵� �ݾ�

	void Set(int LastRound, INT64 RewardMoney)
	{
		SetHeader(SV_PM_ODDEVEN_FIX_REWARD);

		PushData(_PVAR(m_LastRound), &LastRound, sizeof(*m_LastRound));
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_LastRound), sizeof(*m_LastRound));
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
	}
};

class CCL_PM_ODDEVEN_GAME_GIVEUP : public CCommMsg
{
	//���߿� ���� ���⸦ ��û�Ѵ�.
public:

	void Set()
	{
		SetHeader(CL_PM_ODDEVEN_GAME_GIVEUP);
	}

	void Get(char* lpdata, int size) 
	{
		if (!GetHeader(lpdata, size)) return;
	}
};

class CSV_PM_ODDEVEN_WINNER_NOTI : public CCommMsg
{
	//��÷�ڸ� ��ü �������� ��Ƽ�� �ش�~!
public:
	char *m_UserID; //���̵� Ȥ�� �г���
	int *m_WinRound; //��÷ ����
	INT64 *m_RewardMoney; //��÷��
	int *m_RoundWinner; //�ش� ���� ��÷�� ��

	void Set(const char *UserID, int WinRound, INT64 RewardMoney, int RoundWinner)
	{
		SetHeader(SV_PM_ODDEVEN_WINNER_NOTI);

		char strUserID[NICK_LEN] = {0,}; //�г��� ���۰� ID���� ũ�Ƿ� �г��� �������� ����ش�.
		ZeroMemory(strUserID, NICK_LEN);
		
		strcpy(strUserID, UserID);

		PushData(_PVAR(m_UserID), strUserID, NICK_LEN);
		PushData(_PVAR(m_WinRound), &WinRound, sizeof(*m_WinRound));
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
		PushData(_PVAR(m_RoundWinner), &RoundWinner, sizeof(*m_RoundWinner));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_UserID), NICK_LEN);
		TakeData(_PVAR(m_WinRound), sizeof(*m_WinRound));
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
		TakeData(_PVAR(m_RoundWinner), sizeof(*m_RoundWinner));
	}
};


///////////////////////////////////////

//////////////////////////// �� �̴ϰ��� �̺�Ʈ 2019.07.15/////////////////////////////////////
class CSV_PM_WHEEL_START_INFO : public CCommMsg
{
	//���� �� ������ ������ �� �ִ����� �˷��ش�.
	//�� ��Ŷ�� true�� ������ ���������� �� �� �ִٰ� �Ǵ��Ͽ�, ������ ���� ���� ��û ��Ŷ�� ������ �ȴ�.
public:
	bool *m_StartEnable;
	bool *m_NeedCaptcha; //ĸ�������� �ʿ����� �����̴�.
	INT64 m_vecRewardTable[9];

	void Set(bool StartEnable, bool NeedCaptcha, INT64* vecRewardTable) 
	{
		SetHeader(SV_PM_WHEEL_START_INFO);

		memcpy(m_vecRewardTable, vecRewardTable, sizeof(INT64) * 9);

		PushData(_PVAR(m_StartEnable), &StartEnable, sizeof(*m_StartEnable));
		PushData(_PVAR(m_NeedCaptcha), &NeedCaptcha, sizeof(*m_NeedCaptcha) * 9);
		PushData(_PVAR(m_vecRewardTable), &vecRewardTable, sizeof(*m_vecRewardTable));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_StartEnable), sizeof(*m_StartEnable));
		TakeData(_PVAR(m_NeedCaptcha), sizeof(*m_NeedCaptcha));
		TakeData(_PVAR(m_vecRewardTable), sizeof(*m_vecRewardTable));
	}
};

class CCL_PM_WHEEL_REQUEST_START : public CCommMsg
{
	//���ӽ����� ��û�Ѵ�.
public:
	////////////////////////////����⼮�� CAPTCHA 2018.07.31/////////////////////////////////////
	char *m_CaptchaKey; //ĸ�� ���������� ���� ����Ű�� �־� �ش�.
	//////////////////////////////

	void Set(const char *CaptchaKey) 
	{
		SetHeader(CL_PM_WHEEL_REQUEST_START);

		char strCaptchaKey[CHAR_BUFFER_CAPTCHA_KEY_LEN]={0,};
		strcpy(strCaptchaKey, CaptchaKey);

		PushData(_PVAR(m_CaptchaKey), strCaptchaKey, CHAR_BUFFER_CAPTCHA_KEY_LEN);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_CaptchaKey), CHAR_BUFFER_CAPTCHA_KEY_LEN);
	}
};

class CSV_PM_WHEEL_RESPONSE_START : public CCommMsg
{
	//���ӽ��� ��û�� ���� ����̴�.
public:
	int *m_ErrCode; //0.�������, 1.�����̹�������, 2.��Ÿ���� enum PROMOTION_WHEEL_START_ERRCODE ����
	char *m_StartDate;
	char *m_EndDate; 

	void Set(int ErrCode, const char* szStartDate, const char* szEndDate) 
	{
		SetHeader(SV_PM_WHEEL_RESPONSE_START);

		PushData(_PVAR(m_ErrCode), &ErrCode, sizeof(*m_ErrCode));
		char strStartDate[20]={0,};
		strcpy(strStartDate, szStartDate);
		char strEndDate[20]={0,};
		strcpy(strEndDate, szEndDate);
		PushData(_PVAR(m_StartDate), strStartDate, 20);
		PushData(_PVAR(m_EndDate), strEndDate, 20);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ErrCode), sizeof(*m_ErrCode));
		TakeData(_PVAR(m_StartDate), 20);
		TakeData(_PVAR(m_EndDate), 20);
	}
};


class CCL_PM_WHEEL_USER_BET : public CCommMsg
{
	//Ȧ�̳�, ¦�̳� ������ �����̴�.
public:

	void Set(int Round, int BetSide) 
	{
		SetHeader(CL_PM_WHEEL_USER_BET);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_PM_WHEEL_USER_BET_RESULT : public CCommMsg
{
	//�������ÿ� ���� ����̴�.
	//������ �̹� ���� ���ÿ� ���� �����..
public:
	INT64 *m_RoundReward; //������ ȹ���ϰԵǴ� �����
	int *m_RewardIndex;
	
	void Set(INT64 RoundReward, int RewardIndex)
	{
		SetHeader(SV_PM_WHEEL_USER_BET_RESULT);

		PushData(_PVAR(m_RoundReward), &RoundReward, sizeof(*m_RoundReward));
		PushData(_PVAR(m_RewardIndex), &RewardIndex, sizeof(*m_RewardIndex));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_RoundReward), sizeof(*m_RoundReward));
		TakeData(_PVAR(m_RewardIndex), sizeof(*m_RewardIndex));
	}
};

class CSV_PM_WHEEL_WINNER_NOTI : public CCommMsg
{
	//��÷�ڸ� ��ü �������� ��Ƽ�� �ش�~!
public:
	char *m_UserID; //���̵� Ȥ�� �г���
	INT64 *m_RewardMoney; //��÷��
	int *m_RoundWinner; //�ش� ���� ��÷�� ��

	void Set(const char *UserID, int WinRound, INT64 RewardMoney, int RoundWinner)
	{
		SetHeader(SV_PM_WHEEL_WINNER_NOTI);

		char strUserID[NICK_LEN] = {0,}; //�г��� ���۰� ID���� ũ�Ƿ� �г��� �������� ����ش�.
		ZeroMemory(strUserID, NICK_LEN);
		
		strcpy(strUserID, UserID);

		PushData(_PVAR(m_UserID), strUserID, NICK_LEN);
		PushData(_PVAR(m_RewardMoney), &RewardMoney, sizeof(*m_RewardMoney));
		PushData(_PVAR(m_RoundWinner), &RoundWinner, sizeof(*m_RoundWinner));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_UserID), NICK_LEN);
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
		TakeData(_PVAR(m_RoundWinner), sizeof(*m_RoundWinner));
	}
};


///////////////////////////////////////

//////////////////////////// 2019 �Ѱ��� �̺�Ʈ 2019.09.05/////////////////////////////////////
class CSV_PM_2019HAVEST_INFO : public CCommMsg
{
	// 2019 �߼� ���� ����ڿ��Ը� �ش� ��Ŷ�� ���� �Ѵ�.
public:

	int *m_RewardDay;		// ���� ����
	INT64 *m_RewardMoney;	// �����

	void Set(int rewardDay, INT64 rewardMoney) 
	{
		SetHeader(SV_PM_2019HAVEST_INFO);
		PushData(_PVAR(m_RewardDay), &rewardDay, sizeof(*m_RewardDay));
		PushData(_PVAR(m_RewardMoney), &rewardMoney, sizeof(*m_RewardMoney));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_RewardDay), sizeof(*m_RewardDay));
		TakeData(_PVAR(m_RewardMoney), sizeof(*m_RewardMoney));
	}
};

class CCL_PM_2019HAVEST_REQUEST_REWARD : public CCommMsg
{
	// 2019 �߼� ���� ��û�Ѵ�.
public:

	void Set() 
	{
		SetHeader(CL_PM_2019HAVEST_REQUEST_REWARD);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_2019HAVEST_REQUEST_RESULT : public CCommMsg
{
	// 2019 �߼� ���� �̺�Ʈ �ݾ�
public:
	INT64 *m_Reward; //������ ȹ���ϰԵǴ� �����
	
	void Set(INT64 Reward)
	{
		SetHeader(SV_PM_2019HAVEST_REQUEST_RESULT);

		PushData(_PVAR(m_Reward), &Reward, sizeof(*m_Reward));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Reward), sizeof(*m_Reward));
	}
};
////////////////////////////�̴ϰ��� ���� 2018.06.04/////////////////////////////////////
// ���� ���� Ƚ�� ��û (�� �ִ� 20ȸ)
class CCL_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT : public CCommMsg
{
public:
	void Set()
	{
		SetHeader(CL_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

// ���� ���� Ƚ�� ����
class CSV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT : public CCommMsg
{
public:
	int* m_nRemainCount;	// ���� ���� Ƚ��
	INT64* m_llUsingGold;	// ����ϰ� �� ���. ����� ���� 400. 

	void Set(int nRemainCount, INT64 llUsingGold)
	{
		SetHeader(SV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT);

		PushData(_PVAR(m_nRemainCount), &nRemainCount, sizeof(*m_nRemainCount));
		PushData(_PVAR(m_llUsingGold), &llUsingGold, sizeof(*m_llUsingGold));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_nRemainCount), sizeof(*m_nRemainCount));
		TakeData(_PVAR(m_llUsingGold), sizeof(*m_llUsingGold));
	}
};

// ���� �÷��� ��û
class CCL_PM_BIGWHEEL_PLAY_GAME : public CCommMsg
{
public:
	INT64* m_llUsingGold;	// ����ϰ� �� ���. ����� ���� 400. 

	void Set(INT64 llUsingGold)
	{
		SetHeader(CL_PM_BIGWHEEL_PLAY_GAME);

		PushData(_PVAR(m_llUsingGold), &llUsingGold, sizeof(*m_llUsingGold));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_llUsingGold), sizeof(*m_llUsingGold));
	}
};

// ���� �÷��� ����/���
class CSV_PM_BIGWHEEL_PLAY_GAME : public CCommMsg
{
public:
	int* m_nErrorCode;			// 0: ����, 1: ������� ����: 2: ���� Ƚ�� ����, 3 �̻� DB ����..
	int* m_nRemainCount;		// ������ ���� Ƚ��
	INT64* m_llUsedGold;		// ����� ��� (����� ���� 400)
	INT64* m_llAddedMoney;		// �߰��� ���ӸӴ� (400���� ȯ���� ���ӸӴ�)

	void Set(int nErrorCode, int nRemainCount, INT64 llUsedGold, INT64 llAddedMoney)
	{
		SetHeader(SV_PM_BIGWHEEL_PLAY_GAME);

		PushData(_PVAR(m_nErrorCode), &nErrorCode, sizeof(*m_nErrorCode));
		PushData(_PVAR(m_nRemainCount), &nRemainCount, sizeof(*m_nRemainCount));
		PushData(_PVAR(m_llUsedGold), &llUsedGold, sizeof(*m_llUsedGold));
		PushData(_PVAR(m_llAddedMoney), &llAddedMoney, sizeof(*m_llAddedMoney));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_nErrorCode), sizeof(*m_nErrorCode));
		TakeData(_PVAR(m_nRemainCount), sizeof(*m_nRemainCount));
		TakeData(_PVAR(m_llUsedGold), sizeof(*m_llUsedGold));
		TakeData(_PVAR(m_llAddedMoney), sizeof(*m_llAddedMoney));
	}
};
///////////////////////////////////////

////////////////////////////2018��06�� ���� 2018.06.01/////////////////////////////////////
class CSV_ETCCHIP_INSURANCE_USER_CHANGE_INFO : public CCommMsg
{
	//�ش� ������ ��Ÿ��ȭ ����� ���� ������ �����ش�.(����, ���޵�)
public:
	char *m_ChipType; //ĨŸ��.. 
	int *m_ChangeType; //1.����, 2.����
	INT64 *m_ChangeChip; //�����ݾ�.. �׻� ����ӿ� ��������~!!

	INT64 *m_CurrentChip; //�� ó�� ���� �����ݾ�
	int *m_FixedLevel; //�� ó������ Ȯ���� ������
	char *m_FixedName; //�� ó������ Ȯ���� �����̸�
	INT64 *m_FixedSaveChip; //�� ó������ ���� �ִ� ������

	void Set(const char* ChipType, int ChangeType, INT64 ChangeChip, INT64 CurrentChip, int FixedLevel, const char *FixedName, INT64 FixedSaveChip)
	{
		SetHeader(SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO);

		char strChipType[CHAR_BUFFER_ETC_CHIP_CODE]={0,};
		strcpy(strChipType, ChipType);

		char strInsuranceName[CHAR_BUFFER_GOLD_INSURANCE_NAME]={0,};
		strcpy(strInsuranceName, FixedName);

		PushData(_PVAR(m_ChipType), strChipType, CHAR_BUFFER_ETC_CHIP_CODE);
		PushData(_PVAR(m_ChangeType), &ChangeType, sizeof(*m_ChangeType));
		PushData(_PVAR(m_ChangeChip), &ChangeChip, sizeof(*m_ChangeChip));

		PushData(_PVAR(m_CurrentChip), &CurrentChip, sizeof(*m_CurrentChip));
		PushData(_PVAR(m_FixedLevel), &FixedLevel, sizeof(*m_FixedLevel));
		PushData(_PVAR(m_FixedName), strInsuranceName, CHAR_BUFFER_GOLD_INSURANCE_NAME);
		PushData(_PVAR(m_FixedSaveChip), &FixedSaveChip, sizeof(*m_FixedSaveChip));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), CHAR_BUFFER_ETC_CHIP_CODE);
		TakeData(_PVAR(m_ChangeType), sizeof(*m_ChangeType));
		TakeData(_PVAR(m_ChangeChip), sizeof(*m_ChangeChip));

		TakeData(_PVAR(m_CurrentChip), sizeof(*m_CurrentChip));
		TakeData(_PVAR(m_FixedLevel), sizeof(*m_FixedLevel));
		TakeData(_PVAR(m_FixedName), CHAR_BUFFER_GOLD_INSURANCE_NAME);
		TakeData(_PVAR(m_FixedSaveChip), sizeof(*m_FixedSaveChip));
	}
};

class CSV_ETCCHIP_INSURANCE_USER_INFO : public CCommMsg
{
	//�ش� ������ ��Ÿ��ȭ ����� ���� ������ �����ش�.
public:
	char *m_ChipType; //ĨŸ��.. 
	int *m_Level; //0�̸� �̰��� ����
	char *m_Name; //�����
	INT64 *m_SaveMoney; //���� ������ �ݾ�

	void Set(const char* ChipType, int Level, const char *Name, INT64 SaveMoney)
	{
		SetHeader(SV_ETCCHIP_INSURANCE_USER_INFO);

		char strChipType[CHAR_BUFFER_ETC_CHIP_CODE]={0,};
		strcpy(strChipType, ChipType);

		char strInsuranceName[CHAR_BUFFER_GOLD_INSURANCE_NAME]={0,};
		strcpy(strInsuranceName, Name);

		PushData(_PVAR(m_ChipType), strChipType, CHAR_BUFFER_ETC_CHIP_CODE);
		PushData(_PVAR(m_Level), &Level, sizeof(*m_Level));
		PushData(_PVAR(m_Name), strInsuranceName, CHAR_BUFFER_GOLD_INSURANCE_NAME);
		PushData(_PVAR(m_SaveMoney), &SaveMoney, sizeof(*m_SaveMoney));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_ChipType), CHAR_BUFFER_ETC_CHIP_CODE);
		TakeData(_PVAR(m_Level), sizeof(*m_Level));
		TakeData(_PVAR(m_Name), CHAR_BUFFER_GOLD_INSURANCE_NAME);
		TakeData(_PVAR(m_SaveMoney), sizeof(*m_SaveMoney));
	}
};

class CCL_GOLDBANK_WITHDRAW : public CCommMsg
{
	//�����¿� ������ �ݾ��� ���� ��û�Ѵ�.
public:
	INT64 *m_Amount; //���� �ݾ�

	void Set(INT64 Amount)
	{
		SetHeader(CL_GOLDBANK_WITHDRAW);

		PushData(_PVAR(m_Amount), &Amount, sizeof(*m_Amount));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Amount), sizeof(*m_Amount));
	}
};

class CSV_GOLDBANK_WITHDRAW_RESULT : public CCommMsg
{
	//������ ���� ��û�� ���� ����̴�.
public:
	int *m_Result; //����ڵ� 0.����, 1.�����ܾ׺���, 2.��庸���ѵ����ɸ�, 99.��Ÿ����
	INT64 *m_WithDrawAmount; //����� �ݾ�(���� ��û�� �ݾ�)
	INT64 *m_RemainGoldBank; //������ �����¿� ���� �ִ� �ݾ�
	INT64 *m_CurGold; //����ݾ� �ջ��� �����ϰԵ� ���

	void Set(int Result, INT64 WithDrawAmount, INT64 RemainGoldBank, INT64 CurGold)
	{
		SetHeader(SV_GOLDBANK_WITHDRAW_RESULT);

		PushData(_PVAR(m_Result), &Result, sizeof(*m_Result));
		PushData(_PVAR(m_WithDrawAmount), &WithDrawAmount, sizeof(*m_WithDrawAmount));
		PushData(_PVAR(m_RemainGoldBank), &RemainGoldBank, sizeof(*m_RemainGoldBank));
		PushData(_PVAR(m_CurGold), &CurGold, sizeof(*m_CurGold));
	}

	void Get(char *lpdata, int size)
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_Result), sizeof(*m_Result));
		TakeData(_PVAR(m_WithDrawAmount), sizeof(*m_WithDrawAmount));
		TakeData(_PVAR(m_RemainGoldBank), sizeof(*m_RemainGoldBank));
		TakeData(_PVAR(m_CurGold), sizeof(*m_CurGold));
	}
};

// ī�� ���� ����� ��� �Ǵ� ���� ��Ų��.
class CCL_ASK_CARDOPEN : public CCommMsg
{

public:

	bool *m_bCardOpen; // ī�� ���� ����

	void Set(bool bCardOpen)
	{
		SetHeader(CL_ASK_CARDOPEN);

		PushData(_PVAR(m_bCardOpen), &bCardOpen, sizeof(*m_bCardOpen));
	}

	void Get(char* lpdata, int size) 
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_bCardOpen), sizeof(*m_bCardOpen));
	}
};

// ī�� ���� ����� ��� �Ǵ� ���� ��û�� ���� ���
class CSV_ASK_CARDOPEN_RESULT : public CCommMsg
{

public:

	bool *m_bCardOpen; // ī�� ���� ����

	void Set(bool bCardOpen)
	{
		SetHeader(SV_ASK_CARDOPEN_RESULT);

		PushData(_PVAR(m_bCardOpen), &bCardOpen, sizeof(*m_bCardOpen));
	}

	void Get(char* lpdata, int size) 
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_bCardOpen), sizeof(*m_bCardOpen));
	}
};
//////////////////////////////////////////////////

//#endif

// �Ű��ϱ� ��� �޽���
class CSV_REPORT_RIGUSER_RESULT : public CCommMsg
{
public:
	int* m_nErrorCode;			// ���� ����: 0 - ����, �� ���� �� ����
	int* m_nRemainCount;		// ���� �Ű� Ƚ��

	void Set(int nErrorCode, int nRemainCount) 
	{
		SetHeader(SV_REPORT_RIGUSER_RESULT);

		PushData(_PVAR(m_nErrorCode), &nErrorCode, sizeof(*m_nErrorCode));
		PushData(_PVAR(m_nRemainCount), &nRemainCount, sizeof(*m_nRemainCount));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_nErrorCode), sizeof(*m_nErrorCode));
		TakeData(_PVAR(m_nRemainCount), sizeof(*m_nRemainCount));
	}
};

//////////////////////////////////////////////////////////////////////////
// ī�� �ý���. 2018.12.20

// ī��/���� ȸ�� ����Ʈ (���� ����)
class CCL_CAFE_MEMBERLIST : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;

	void Set(CafeSystem::CafeKind::Type cafeKind, int cafeID) 
	{
		SetHeader(CL_CAFE_MEMBERLIST);

		PushData(_PVAR(cafeKind_), &cafeKind, sizeof(*cafeKind_));
		PushData(_PVAR(cafeID_), &cafeID, sizeof(*cafeID_));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafeKind_), sizeof(*cafeKind_));
		TakeData(_PVAR(cafeID_), sizeof(*cafeID_));
	}
};

class CSV_CAFE_MEMBERLIST : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;
	int* memberInfoCount_;
	CafeSystem::MemberInfo* memberInfoList_;

	void Set(CafeSystem::CafeKind::Type cafeKind, int cafeID, const std::vector<CafeSystem::MemberInfo>& cafeMemberInfoList)
	{
		SetHeader(SV_CAFE_MEMBERLIST);

		PushData(_PVAR(cafeKind_), &cafeKind, sizeof(*cafeKind_));
		PushData(_PVAR(cafeID_), &cafeID, sizeof(*cafeID_));

		int listCount = cafeMemberInfoList.size();
		PushData(_PVAR(memberInfoCount_), &listCount, sizeof(*memberInfoCount_));

		for (int i = 0; i < listCount; ++i)
		{
			PushData(_PVAR(memberInfoList_), &cafeMemberInfoList[i], sizeof(CafeSystem::MemberInfo));
		}
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafeKind_), sizeof(*cafeKind_));
		TakeData(_PVAR(cafeID_), sizeof(*cafeID_));
		TakeData(_PVAR(memberInfoCount_), sizeof(*memberInfoCount_));
		TakeData(_PVAR(memberInfoList_), sizeof(CafeSystem::MemberInfo) * (*memberInfoCount_));
	}
};

// ī��/���� ���� ��û
class CCL_CAFE_ASK_JOIN : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;

	void Set(CafeSystem::CafeKind::Type cafeKind, int cafeID) 
	{
		SetHeader(CL_CAFE_ASK_JOIN);

		PushData(_PVAR(cafeKind_), &cafeKind, sizeof(*cafeKind_));
		PushData(_PVAR(cafeID_), &cafeID, sizeof(*cafeID_));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafeKind_), sizeof(*cafeKind_));
		TakeData(_PVAR(cafeID_), sizeof(*cafeID_));
	}
};

class CSV_CAFE_ASK_JOIN : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;
	CafeSystem::JoinResult::Type* joinResult_;

	void Set(CafeSystem::CafeKind::Type cafeKind, int cafeID, CafeSystem::JoinResult::Type joinResult) 
	{
		SetHeader(SV_CAFE_ASK_JOIN);

		PushData(_PVAR(cafeKind_), &cafeKind, sizeof(*cafeKind_));
		PushData(_PVAR(cafeID_), &cafeID, sizeof(*cafeID_));
		PushData(_PVAR(joinResult_), &joinResult, sizeof(*joinResult_));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafeKind_), sizeof(*cafeKind_));
		TakeData(_PVAR(cafeID_), sizeof(*cafeID_));
		TakeData(_PVAR(joinResult_), sizeof(*joinResult_));
	}
};

// ī��/���� �ʴ�
class CCL_CAFE_INVITE : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;
	char* inviteUserID_;		// �ʴ��� ���� ID

	void Set(CafeSystem::CafeKind::Type cafeKind, int cafeID, char* inviteUserID) 
	{
		SetHeader(CL_CAFE_INVITE);

		char userID[ID_LEN] = {0, };
		if (::strlen(userID) < ID_LEN)
		{
			::strcpy(userID, inviteUserID);
		}

		PushData(_PVAR(cafeKind_), &cafeKind, sizeof(*cafeKind_));
		PushData(_PVAR(cafeID_), &cafeID, sizeof(*cafeID_));
		PushData(_PVAR(inviteUserID_), userID, ID_LEN);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafeKind_), sizeof(*cafeKind_));
		TakeData(_PVAR(cafeID_), sizeof(*cafeID_));
		TakeData(_PVAR(inviteUserID_), ID_LEN);
	}
};

class CSV_CAFE_INVITE : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;
	char* inviteUserID_;		// �ʴ��� ���� ID
	CafeSystem::InviteResult::Type* inviteResult_;

	void Set(CafeSystem::CafeKind::Type cafeKind, int cafeID, char* inviteUserID, CafeSystem::InviteResult::Type inviteResult)
	{
		SetHeader(SV_CAFE_INVITE);

		char userID[ID_LEN] = {0, };
		if (::strlen(userID) < ID_LEN)
		{
			::strcpy(userID, inviteUserID);
		}

		PushData(_PVAR(cafeKind_), &cafeKind, sizeof(*cafeKind_));
		PushData(_PVAR(cafeID_), &cafeID, sizeof(*cafeID_));
		PushData(_PVAR(inviteUserID_), userID, ID_LEN);
		PushData(_PVAR(inviteResult_), &inviteResult, sizeof(*inviteResult_));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafeKind_), sizeof(*cafeKind_));
		TakeData(_PVAR(cafeID_), sizeof(*cafeID_));
		TakeData(_PVAR(inviteUserID_), ID_LEN);
		TakeData(_PVAR(inviteResult_), sizeof(*inviteResult_));
	}
};

// ���� �Բ� �ϱ�(���󰡱�) => ���� ��ƾ.
// Ŭ���̾�Ʈ���� CSV_ASK_ROOMINFO ���� -> ROOMINFO ������ �������� CSV_ASK_ENTERROOM �� �� ����.
// ROOMINFO, SMALLROOMINFO�� ī�� ����, ī�� ID �߰�

// �̰��� ������ ����Ʈ ���� ����. ���� ����Ʈ -> ���� �Ⱓ���� ī�信 �����ϸ� ���� ����Ʈ�� �ش�.
class CSV_CAFE_RESERVED_POINT : public CCommMsg
{
public:
	INT64* cafePoint_;

	void Set(INT64 cafePoint)
	{
		SetHeader(SV_CAFE_RESERVED_POINT);

		PushData(_PVAR(cafePoint_), &cafePoint, sizeof(*cafePoint_));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(cafePoint_), sizeof(*cafePoint_));
	}
};

class CSV_CAFE_NOTICE : public CCommMsg
{
public:
	CafeSystem::Notice::Type* noticeType_;

	void Set(CafeSystem::Notice::Type noticeType)
	{
		SetHeader(SV_CAFE_NOTICE);

		PushData(_PVAR(noticeType_), &noticeType, sizeof(*noticeType_));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(noticeType_), sizeof(*noticeType_));
	}
};

class CSV_REPORT_BANISH_MORNINGBET : public CCommMsg
{
public:
	char *m_cBanishCount;
	SYSTEMTIME *m_tmCurrent; 
	SYSTEMTIME *m_tmBanishEnd; 

	void Set(char cBanCount, SYSTEMTIME tmCurrent, SYSTEMTIME tmBanEnd )
	{
		SetHeader(SV_REPORT_BANISH_MORNINGBET);

		PushData(_PVAR(m_cBanishCount), &cBanCount, sizeof(*m_cBanishCount));
		PushData(_PVAR(m_tmCurrent), &tmCurrent, sizeof(*m_tmCurrent));
		PushData(_PVAR(m_tmBanishEnd), &tmBanEnd, sizeof(*m_tmBanishEnd));
	}

	void Get(char* lpdata, int size) 
	{
		if (!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_cBanishCount), sizeof(*m_cBanishCount));
		TakeData(_PVAR(m_tmCurrent), sizeof(*m_tmCurrent));
		TakeData(_PVAR(m_tmBanishEnd), sizeof(*m_tmBanishEnd));
	}
};

//////////////////////////////////////////////////////////////////////////
#endif	// __COMMMSGDEF_H__ 