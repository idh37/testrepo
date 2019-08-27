#ifndef __COMMMSGDEF_H__ 
#define __COMMMSGDEF_H__ 

#include "CommMsg.h"
#include "CommMsgIdHeader.h"
#include "PromotionMsgHeader.h"

#pragma warning (disable:4996)
////////////////////////////////////////////////////////////////////////////
//                 마스터 서버용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

/////////// 버젼을 체크함
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

/////////// 게임 서버 로그인 요청 정보
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

/////////// 그룹점검시 입장가능한 어드민IP정보 전달(게임서버 접속요청시점)
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

/////////// 게임 서버 로그인을 허용함
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

/////////// 게임서버 로그인을 거부함(거부 이유 코드를 보냄)
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

/////////// 게임 서버 상태 변경 정보
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

/////////// 마스터 서버 상태 정보
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

/////////// 게임서버 제어 명령
class CMS_CONTROLCMD : public CCommMsg
{
public:

	int *Code;			// 커맨드 코드
	VSTRING vstrCmd;	// 커맨드 스트링

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

/////////// 상태 모니터링 로그(from 게임서버)
class CMS_STATUSLOG : public CCommMsg
{
public:

	int *Kind;			// 종류
	VSTRING vstrLog;	// 로그 스트링

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

/////////// 사용자 정보 리스트(최초 로그인시 업데이트용)
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

/////////// 채널 업데이트 정보
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

/////////// 사용자 채널 변경 정보
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

/////////// 사용자 강제 종료 명령
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

/////////// 사용자 접속 체크 명령
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

/////////// 사용자가 다른 게임으로 이동 요청
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

	////////////////////////////골드로딩 및 동기화로직 변경 2018.04.18/////////////////////////////////////	
	ETC_MOVE_INFO *m_EtcMoveInfo; //위 데이터 외에 게임이동시 서버간 옮겨야 할 데이터들을 정의한다.
	//위 구조체가 변경되면 마스터서버에서도 반드시 변경해줘야 함~!!

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

		////////////////////////////골드로딩 및 동기화로직 변경 2018.04.18/////////////////////////////////////	
		PushData(_PVAR(m_EtcMoveInfo), &EtcMoveInfo, sizeof(*m_EtcMoveInfo));
		//////////////////////////////////////////////////

		// 훌라 2인룸 매치메이킹 고도화
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

		////////////////////////////골드로딩 및 동기화로직 변경 2018.04.18/////////////////////////////////////	
		TakeData(_PVAR(m_EtcMoveInfo), sizeof(*m_EtcMoveInfo));
		///////////////////////////////////////////////////////

		// 훌라 2인룸 매치메이킹 고도화
		TakeData( _PVAR(m_Count), sizeof(*m_Count));
		TakeData(_PVAR(m_ArrayRoomKey), sizeof(UINT64) * (*m_Count));
	}
};

/////////// 접속할 다른 게임 정보 응답
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

/////////// 사용자가 다른 채널로 이동 요청
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

	////////////////////////////골드로딩 및 동기화로직 변경 2018.04.18/////////////////////////////////////	
	ETC_MOVE_INFO *m_EtcMoveInfo; //위 데이터 외에 게임이동시 서버간 옮겨야 할 데이터들을 정의한다.
	//위 구조체가 변경되면 마스터서버에서도 반드시 변경해줘야 함~!!

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

		////////////////////////////골드로딩 및 동기화로직 변경 2018.04.18/////////////////////////////////////	
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

		////////////////////////////골드로딩 및 동기화로직 변경 2018.04.18/////////////////////////////////////	
		TakeData(_PVAR(m_EtcMoveInfo), sizeof(*m_EtcMoveInfo));
		///////////////////////////////////////////////////////
	}
};

/////////// 사용자가 다른 채널로 이동 요청 결과
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

/////////// 사용자 찾기 요청
class CMS_ASK_FINDUSER : public CCommMsg
{
public:

	int  *ClientUNum;	// 의뢰인 UNum
	char *ClientID;		// 의뢰인 ID
	char *TargetNickName;		// 찾을 대상 NickName
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

/////////// 아이디 찾기 결과
class CMS_RESULT_FINDUSER : public CCommMsg
{
public:

	int  *ClientUNum;	// 의뢰인 UNum
	char *ClientID;		// 의뢰인 ID
	RESULTFINDID *RF;	// 찾기 결과
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

/////////// 접속할 게임서버 정보 요청(From 클라이언트)
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

/////////// 게임서버 정보 요청 거부(To 클라이언트)
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

/////////// 접속할 게임서버 정보(To 클라이언트)
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
// 토너먼트 오픈 공지
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
//                      서버용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

/////////// 버젼을 체크함
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

/////////// 서버 정보	[게임 아이템 작업]
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



// [SSO 작업] - 추가
/////////// 로그인 요청 정보(ID, Password, Game Version)
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

/////////// 로그인을 거부함(거부 이유 코드를 보냄)	[SSO 작업] - 교체
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


/////////// 로그인을 허용함(사용자 정보를 보냄)
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



/////////// 로그아웃을 요청
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

/////////// 로그아웃을 허용
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

// 기존게임(하이로우, 섯다, 훌라 및 마스터, 에이젼트, 로그서버)와의 호환성을 위해서 남겨둠. 향후 삭제해야함.
/////////// 허트비트
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

/////////// 허트비트고스트 체크용
class CSV_HEARTBEATGHOSTCHECK : public CCommMsg
{
public:
	UINT64*	m_llTick;			// 서버에서 보낸 시간

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

/////////// 다른 서버로 접속할 채널 정보
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

/////////// 다른서버에서 채널 이동으로 접속 요청
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

/////////// 다른 서버로 접속할 채널 정보
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

/////////// 다른서버에서 게임 이동으로 접속 요청
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

/////////// 게임 입장을 허용
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


/////////// 채널 입장을 요청
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

/////////// 채널 입장을 허용
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

/////////// 아이디 찾기 요청

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

/////////// 아이디 찾기 결과
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

	BOOL *bAnnounce;	// 신고 알림 여부
	int *Type;			//신고종류	//## 112신구 추가 ##//
	int *RNum;			// 방번호(0이면 대기실)
	int *UNum;			// 신고인 유저번호
	char *ID;			// 신고인 아이디
	char *TargetID;			// 신고대상 아이디
	char *TargetNickName;		// 신고대상 닉네임
	char *SubmitReason;	// 신고사유
	int *l_StrWatchID;	// 증인 아이디 스트링의 길이
	char *WatchID;		// 증인 아이디 스트링
	int *l_StrChat;		// 신고될 채팅 스트링의 길이
	char *StrChat;		// 신고될 채팅 스트링
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
		PushData(_PVAR(Type), &ntype, sizeof(*Type));	//추가 
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

/////////// 전체 채널 목록을 요구한다
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

/////////// 전체 채널 목록을 보내준다
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

/////////// 사용자 목록을 요구한다
class CSV_ASK_ALLUSERINFO : public CCommMsg
{
public:
	int* m_nGameIndex;			// -1: 전체, 0:7포커, 1:로우바둑이, 2:뉴포커, 3:맞포커, 4:하이로우
	MONEYRANGE* m_stMoneyRange;	// 사용자 목록을 요청할 머니구간

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

/////////// 사용자 목록을 보내준다
class CSV_ALLUSERINFO : public CCommMsg
{
public:
	int* m_nGameIndex;			// -1: 전체, 0:7포커, 1:로우바둑이, 2:뉴포커, 3:맞포커, 4:하이로우
	MONEYRANGE* m_stMoneyRange;	// 사용자 목록의  머니구간
	int *TotNum;
	SMALLUSERINFO *ArraySUI;

	// 500 단위로 사용자 목록을 끊어서 보내주기
	int* m_nTotalSendCnt;		// 총 보낼 패킷개수
	int* m_nCurSendCnt;		// 현재 보내고 있는 패킷인덱스	
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

//////// 초대할 유저정보를 요청한다.
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

/////////// 특정 사용자의 상세 정보를 요구한다
class CSV_ASK_DETAILUSERINFO : public CCommMsg
{
public:

	int *UNum;
	// 닉네임 작업
	//char *ID;
	char *NickName;
	// 여기까지
	void Set(int unum, char* nickname) 
	{
		char strNickName[20]={0,};
		// 닉네임 작업
		if(strlen(nickname) < 20) strcpy(strNickName, nickname);
		// 여기까지

		SetHeader(SV_ASK_DETAILUSERINFO);
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));

		// 닉네임 작업
		PushData(_PVAR(NickName), strNickName, 20);
		// 여기까지

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UNum), sizeof(*UNum));

		// 닉네임 작업
		TakeData(_PVAR(NickName), 20);
		// 여기까지

	}
};

/////////// 특정 사용자 정보을 요구한다
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

/////////// 특정 사용자 정보를 보내줌
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

/////////// 새로운 사용자가 접속했음을 알린다
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

/////////// 사용자가 접속을 종료했음을 알린다
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

/////////// 다른 사용자의 접속 정보가 변했음
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

/////////// 방 목록을 요구한다
class CSV_ASK_ALLROOMINFO : public CCommMsg
{
public:
	int* m_nGameIndex;				// 사용자 목록을 요청할 게임코드

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

/////////// 방 목록을 보내준다
class CSV_ALLROOMINFO : public CCommMsg
{
public:
	int* m_nGameIndex;			// 사용자 목록을 요청할 게임코드
	int *TotNum;
	SMALLROOMINFO *ArrayRI;

	// 500 단위로 사용자 목록을 끊어서 보내주기
	int* m_nTotalSendCnt;		// 총 보낼 패킷개수
	int* m_nCurSendCnt;		// 현재 보내고 있는 패킷인덱스	

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

/////////// 방 정보를 요구
class CSV_ASK_ROOMINFO : public CCommMsg
{
public:
	enum RequestKind
	{
		DisplayRoomInfo = 0,	// 룸정보 표시용도
		EnterRoomInfo = 1,		// 룸입장 요청용도
		SearchRoomInfo = 2,		// 룸검색 요청용도
		EnterCafeRoomInfo = 3,		// 카페/연합방 입장 요청용도
	};

	int *RoomNum;
	char*	m_cKind;		// 방정보 요청사유(클라이언트에서 자체적으로 사용)
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

/////////// 방 정보
class CSV_ROOMINFO : public CCommMsg
{
public:

	int *ErrCode;
	char* m_cKind;		// 방정보 요청사유(클라이언트에서 자체적으로 사용)
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

/////////// 캐릭터 바꾸기를 요청
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

/////////// 캐릭터 바꾸기를 요청 허락
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

/////////// 방 개설 요구
class CSV_ASK_CREATEROOM : public CCommMsg
{
public:

	ROOMINFO *RI;
	int *SndFxKind; // ### [사운드 추가 작업] ###
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

/////////// 방 개설 거부
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

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		ERR_CEATEROOM_GIVEUP_MAXUSER_OVER = 101, //기권방 최대 인원을 오버했음
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

/////////// 방 개설 허용
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

/////////// 방 입장을 요청
class CSV_ASK_ENTERROOM : public CCommMsg
{
public:

	int *RoomNum;
	UINT64*	m_llRoomKey;			// IPS 룸키
	int *UNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;
	char *Pass;
	char *Observer;
	int *m_ChipKind;////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
	
	void Set(int rnum, UINT64 llRoomKey, int unum, int sndKind, char *id, char *pass=NULL, char observer = 0, int ChipKind = 0) // 추가
	{
		char strid[16]={0,};
		char strpass[5]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		if(pass != NULL) strncpy(strpass, pass, 4);
		SetHeader(SV_ASK_ENTERROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(m_llRoomKey), &llRoomKey, sizeof(*m_llRoomKey));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // 추가
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(Pass), strpass, 5);
		PushData(_PVAR(Observer), &observer, sizeof(char));
		PushData(_PVAR(m_ChipKind), &ChipKind, sizeof(*m_ChipKind));////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(m_llRoomKey), sizeof(*m_llRoomKey));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Pass), 5);
		TakeData(_PVAR(Observer), sizeof(char));
		TakeData(_PVAR(m_ChipKind), sizeof(*m_ChipKind));////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
	}
};

/////////// 입장을 허가함
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



/////////// 입장 요청을 거부함
class CSV_REFUSE_ENTERROOM : public CCommMsg
{
public:
	int *Code;				// -10 = 서버이동후 룸입장
	INT64 *LimitMoney;

	int* m_nGroupCode;	// 옮길 그룹코드
	int* m_nChanCode;		// 옮길 채널코드

	////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
	int *m_ChipKind; //enum ROOM_CHIPKIND 에 정의된 값이 들어감
	/////////////////////////////////////////

	void Set(int code, INT64 i64LimitMoney, int nGroupCode, int nChanCode, int ChipKind)  // 0:인원초과 1:비밀번호가 틀림
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

/////////// 새로운 방이 개설되었음
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

/////////// 방이 사라졌음
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

/////////// 방 정보가 변했음
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

/////////// 방에 사용자가 입장하였음
class CSV_USERENTERROOM : public CCommMsg
{
public:

	int *RoomNum;
	USERINFO *UI;
	INT64 *llMaxLossMoney;	// 최대 손실 가능 머니 
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	int *SndFxKind; // ### [사운드 추가 작업] ###
	void Set(int rnum, USERINFO *pUI, INT64 max_loss, IPARRINDEX *pIPA, int sndKind) 
	{
		SetHeader(SV_USERENTERROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UI), pUI, sizeof(USERINFO));
		PushData(_PVAR(llMaxLossMoney), &max_loss, sizeof(*llMaxLossMoney));
		PushData(_PVAR(IPA), pIPA, sizeof(IPARRINDEX));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // 추가

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UI), sizeof(USERINFO));
		TakeData(_PVAR(llMaxLossMoney), sizeof(*llMaxLossMoney));
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
	}
};

/////////// 사용자가 방에서 나갔음
class CSV_USEROUTROOM : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
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

/////////// 사용자가 방에서 나가기를 요청
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

/////////// 사용자가 방에서 나가기를 허가
class CSV_ACCEPT_OUTROOM : public CCommMsg
{
public:
	char *Reason;		//방퇴장이유 (enum REASON_OUTROOM 참조)

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

/////////// 나의 사용자 정보가 변경됨
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

/////////// 비정상적인 퇴장(방장과의 재접속에 실패했을 경우)
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

////////// 대기실 보기 초대창 오픈 상태 알림
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

/////////// 초청 메세지를 보냄
class CSV_ASK_INVITE : public CCommMsg
{
public:

	UINT64*	m_llTargetUserKey;			// 대상 유저 키
	char*		m_szTargetNickName;		// 대상 아이디
	SMALLROOMINFO*	m_pSmallRoomInfo;
	USERINFO*	m_pUserInfo;		// 나의 사용자 정보
	int*			m_nStrPasswdLen;		// 방패스워드 길이
	char*			m_szStrPasswd;			// 방패스워드
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

/////////// 초청 메세지를 거부함
class CSV_REFUSE_INVITE : public CCommMsg
{
public:

	int *TarUNum;		// 대상 유저 번호
	char *TarID;		// 대상 아이디
	char *MyID;			// 나의 아이디
	int *l_StrMsg;		// 전할말의 길이
	char *StrMsg;		// 전할말
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

/////////// 추방 투표 요청
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

/////////// 나의 추방 투표 결과
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


/////////// 서버 알림 메세지
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
		MSG_PLAYTIMELIMIT= 101, //10시간 제한
		MSG_BOX_WAIT_FALSE=102,
	};


	int *Kind;				// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;		// 채팅 스트링의 길이
	char *StrChat;			// 채팅 스트링
	BOOL *bColorAssigned;	// 텍스트 컬러가 지정되었는가?
	COLORREF *Color;		// 텍스트 컬러

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

/////////// 귓말 허용 상태 변경
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

/////////// 대기실용 채팅 데이타
class CSV_CHATDATA : public CCommMsg
{
public:

	char *ID;			// 아이디 스트링
	char* m_szNickName;	// 닉네임
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링

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

/////////// 귓속말
class CSV_WHISPERCHAT : public CCommMsg
{
public:
	char*		m_szID;				// 보내는 사람 아이디 스트링
	char*		m_szNickName;		// 보내는 사람 닉네임
	UINT64*	m_llUserKey;			// 보내는 사람 유저키
	char*		m_szReceiverID;				// 받는 사람 아이디 스트링
	char*		m_szReceiverNickName;	// 받는 사람 닉네임
	UINT64*	m_llReceiverUserKey;		// 받는 사람 유저키
	short*		m_sStrChatLen;	// 채팅 스트링의 길이
	char*		m_StrChat;		// 채팅 스트링
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

////////// 강제추방 요청	[게임 아이템 작업]
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

// 강제추방 카운트 클라이언트에 알려주기
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

////////// 게임 아이템 리스트를 요청
// [게임아이템창 리뉴얼] 2006.04.28
class CSV_ASK_GAMEITEMLIST : public CCommMsg
{
public:

	int *ItemCode; // 0: 리스트만 요청, 0이 아니면 아이템 사용
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


/////////// 게임 아이템 리스트를 보내준다
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

/////////// 게임 아이템을 사용(요청, 결과 모두)
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

/////////// 게임 아이템 구입을 물어봄
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
// [ 게임 아이템 ]

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



/////////// 바로 입장 불가(방생성)
class CSV_REFUSE_QUICKSTART : public CCommMsg
{
public:
	enum
	{
		ERR_NOTSATISFIED = 0,	// 찾는 방이없다.
		ERR_ASKOTHERSERVER,	// 다른 서버로 가서 내맘대로 바로입장을 요청해야 함
		ERR_CREATENEWROOM,	// 새로운 방을 만들어라.
		ERR_DAILYLOSSLIMIT,
		ERR_PLAYTIMEOVER,
	};

	int *Code;
	int* m_nGameIndex;
	int* m_nChanNo;
	bool*	m_bUseFindRoomOption;		// 바로가기 룸옵션 사용여부, 사용시: true, 기본바로가기: false
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

// 클라에게 바로가기 명령
class CSV_ORDER_QUICKSTART_IN_ROOM : public CCommMsg
{
public:
	INT64 *llRoomKeyPrev;
	ROOMOPTION *roomOption;		// 바로가기시 사용할 방 옵션
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
		ERR_ASKOTHERSERVER,	// 다른 서버로 가서 내맘대로 바로입장을 요청해야 함
		ERR_CREATENEWROOM,	// 새로운 방을 만들어라.
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

/////////클라이언트 전송 패킷 //////////////////////////////////////////////////////
class CNM_ADMIN_SETTINGMAN : public CCommMsg
{
public:

	enum TYPE_SETTING
	{
		TYPE_SETTING_LOAD,				//클라이언트 -> 서버 세팅 내용 요청 
		TYPE_SETTING_ASK_SAVE,			//클라이언트 -> 서버 세팅 요청  (세팅요청을 전달만 함)
		TYPE_SETTING_BROADCAST,			//마스터 서버 -> 서버들 세팅요청 (실제 세팅이 이루어지는 요청)
		TYPE_SETTING_NOTIFY_LOAD,			// 서버 -> 클라이언트 세팅되어있는 내용 알려줌 
		TYPE_SETTING_NOTIFY_SAVE,			// 서버 -> 클라이언트 세팅된  내용 알려줌 
	};
	
#define NM_ADMIN_SETTINGMAN_LENBUF (50)
	char *Type;		//0:세팅값가져오기 1:세팅값보내기; 
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
// 점프 아이템 작업

// 점프 아이템 사용 채널 입장 요청
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

// 점프 아이템 채널 입장 허용
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

// 점프 아이템을 가지고 로긴 한다
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

//  DB 에 사용 유무 업데이트 후 결과를 알려준다.
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

// ### [사운드 추가 작업] ### 요청
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


// ### [사운드 추가 작업] ### 요청 허락
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


// 유저 종료하기
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

// 방제거하기
class CNM_ADMIN_DELROOM : public CCommMsg
{
public:
	
	ADMINMESSAGE *pAMESG;
	short *l_StrMsg;	// 메세지 길이
	char *StrMsg;		// 메세지 
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

/////////// 관리자 귓속말
class CNM_ADMIN_WHISPERCHAT : public CCommMsg
{
public:

	char *ID;			// 보내는 사람 아이디 스트링
	int  *DestUNum;		// 받는 사람 사용자 번호
	char *DestNickName;		// 받는 사람 아이디
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
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

// 방제 변경하기
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

/////////// 방 정보 변경 요청 알림
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

// 어뷰져 놀려주기
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
/////////////////// ### [ 관전기능 ] ###  ///////////////////

/////////// 겜참여 요청
class CSV_ASK_PARTICIPATION : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	int *SndFX;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
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


//////////// 겜참여 가능
class CSV_ACCEPT_PARTICIPATION : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;
	USERINFO *UI;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	void Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, USERINFO *ui, IPARRINDEX *pIPA) // 추가
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ACCEPT_PARTICIPATION);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(NowPNum), &nowpnum, sizeof(*NowPNum));
		PushData(_PVAR(NewPNum), &newpnum, sizeof(*NewPNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // 추가
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
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(UI), sizeof(USERINFO));
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};

//////////// 겜참여 가능
class CSV_ENTER_PARTICIPATION : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;
	USERINFO *UI;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	void Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, USERINFO *ui, IPARRINDEX *pIPA) // 추가
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ENTER_PARTICIPATION);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(NowPNum), &nowpnum, sizeof(*NowPNum));
		PushData(_PVAR(NewPNum), &newpnum, sizeof(*NewPNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // 추가
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
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(UI), sizeof(USERINFO));
		TakeData(_PVAR(IPA), sizeof(IPARRINDEX));
	}
};

/////////// 겜참여 요청
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


//////////// 관전 가능
class CSV_ACCEPT_EMERAGE : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	void Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA) // 추가
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

//////////// 관전 같은방의 다른유저에게 알린다
class CSV_ENTER_EMERAGE : public CCommMsg
{
public:

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	void Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA) // 추가
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


/////////// 예약자 정보 전송
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

/////////// 관전관련 알림 메세지
class CSV_OBSERVER_MESSAGE : public CCommMsg
{
public:

	int *Kind;			// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링

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

/////////// 서버 공지 메세지
class CSV_SERVERNOTIC : public CCommMsg
{
public:

	int *Kind;				// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;		// 채팅 스트링의 길이
	char *StrChat;			// 채팅 스트링
	BOOL *bColorAssigned;	// 텍스트 컬러가 지정되었는가?
	COLORREF *Color;		// 텍스트 컬러

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


/////////// 방 정보 변경 요청
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

// [스피드헥] 2004.09.01
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


// [DB기록 Process] 2004.11.29
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

// [DB기록 Process] 2004.11.29
//G_TFT 클래스 교체 
class CSV_RES_GAME_CONTINUE : public CCommMsg
{
public:
	INT64* llMoney;
	INT64* llRealPMoney;
	char *ID;

	// 등급보험[올인보험개편]
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
		// 등급보험[올인보험개편]
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
		// 등급보험[올인보험개편]
		TakeData(_PVAR(GradeInsuMoney), sizeof(*GradeInsuMoney));
		TakeData(_PVAR(GradeInsuKind), sizeof(*GradeInsuKind));
		TakeData(_PVAR(MoneyRecord), sizeof(*MoneyRecord));//G_TFT

		TakeData(_PVAR(m_llInMoney), sizeof(*m_llInMoney));
		TakeData(_PVAR(m_llSafeMoney), sizeof(*m_llSafeMoney));
	}
};

/////////////////////////////////////찬스 아이템 //////////////////////// 

// 특수한 아이템을 살 수 있다고 알려줌
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

// 특수한 아이템 url 파라미터 값 요청
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


// 요청한 아이템을 살 수 있는 파라미터 주소
class CSV_BUYITEMPARAM : public CCommMsg
{
public:
	
	short *l_StrParam;	// 스트링의 길이
	char *StrParam;		// 파라미터 스트링
	short *GameCode;
	short *ItemCode;
	SYSTEMTIME *ItemTime;
	short	*BuySpecialItemCnt;	//[스행운]2007.10.17

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


// 아이템 요청 에러시 결과값.
class CSV_BUYITEM_RESULT : public CCommMsg
{
public:

	enum 
	{
		ERR_REWARDITEM_NONEXIST = 0,	// 해당 아이템을 받은적이 없다.
		ERR_REWARDITEM_EXPIRED,			// 오래된 아이템 이다.
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


// 아이템 구매 여부 체크
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


// 구매 여부 체크 결과
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
// DB저장된 찬스아이템 리스트 보내줌
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


// 올인보험 가입 요청
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

// 올인보험 지급 요청
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

// 올인보험 가입 요청 결과 메세지
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

// 올인보험 지급 결과 메세지
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


//[행운아바타] 2006.07.24
// 마스터가 각 서버들에게 행운아바타 팔아 오라고 명령한다.	
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


// 게임서버가 팔았다면 마스터에게 보내준다 몇개 팔았다고
class CMS_RESULT_LUCKAVATAIFO : public CCommMsg
{
public:
	
	int *nSaleCnt;		//판매된 숫자
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

//[행운아바타] 2006.07.24
// 유저에게 행운아바타를 구매 할지 물어봄
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

// [재기충전 작업] [2009-02-10]
class CCL_ASK_RECOVERY_CHARGE : public CCommMsg
{
public:

	enum USEKIND
	{
		HALF = 1,		// 두번에 받기
		ALLIN = 2,		// 한번에 받기
	};
	
	// 한번에 받기, 두번에 받기
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
		FAILED_OVERMONEY = -4,			// 충전시 오버머니 발생 할수 있다.
		PROHIBITED_TERM = -3,			// 매달 1일 00~ 17시에는 사용하지 못한다.
		RUN_OUT_OF_USETIME = -2,
		DBWORK_BUSY = -1,
		MY_CHARGE_INFO = 0,
		SUCCESS = 1,				// 지급이 성공한 경우
		RESERVED_SUCCESS = 2,	// 지급예약을 한경우
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




//기사회생 옵션상품
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

//기사회생 옵션상품 //올인창 띄워주기 
class CSV_NOTIFY_ALLIN : public CCommMsg
{
public:
	enum ALLIN_DIALOG_KIND
	{
		ALLIN_DIALOG_KIND_FREE_CHARGE=0,
		ALLIN_DIALOG_KIND_KISA_CHARGE, //기사회생 창 띄우기 
		ALLIN_DIALOG_KIND_ALLIN_INSUARANCE, //올인보험 창 띄우기
		ALLIN_DIALOG_KIND_GOLDENCHIP, //골든칩퀘스트 (머니 교환)
		ALLIN_DIALOG_KIND_GOLDENCHIP_FAIL,//골든칩퀘스트 (골든칩 DB 실패 )
	};

	int *nAllinDlg;		//띄워줄 올인 충전창 종류
	
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

/////////// 바로 시작 방 입장을 요청
enum
{
	ECALL_BTNTYPE_UNKNOWN = 0,
	ECALL_BTNTYPE_LOBY_QUICK,	 // 로비 바로시작버튼
	ECALL_BTNTYPE_DETAIL_SETUP, // 상세보기 입장
	ECALL_BTNTYPE_QUICK_IN_ROOM, // 다른방 바로가기
};
class CSV_ASK_QUICKSTART : public CCommMsg
{
public:
	
	int *UNum;
	char *ID;
	int *SndFxKind;					// ### [사운드 추가 작업] ###
	int *RoomType;					// 0 - 자유방, 1 - 일반방 
	bool*	m_bUseFindRoomOption;	// 바로가기 룸옵션 사용여부, 사용시: true, 기본바로가기: false
	ROOMOPTION* m_stFindRoomOption; //
	int *nCallBtnType;				// 바로시작 구분( 로비 바로시작 버튼 = ECALL_BTNTYPE_LOBY_QUICK, 상세보기 입장 = ECALL_BTNTYPE_DETAIL_SETUP )
	bool* bForceLocalServer;		// 바로시작시 우선순위 조건 찾기중 무한루프 방지
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

// 다른방 바로가기 요청
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
		LOBBY_TYPE,			// 로비 바로시작버튼
		DETAIL_SETUP_TYPE,	// 상세보기 입장
		IN_ROOM_TYPE,		// 다른방 바로가기
	};

	int *SndFxKind;
	int *nType;					// 바로시작 구분( 로비 바로시작 버튼 = ECALL_BTNTYPE_LOBY_QUICK, 상세보기 입장 = ECALL_BTNTYPE_DETAIL_SETUP )
	int *RoomType;				// 0 - 자유방, 1 - 일반방 
	bool *bUseRoomOption;		// 룸옵션 사용여부, 사용시: true, 기본바로가기: false
	bool *bMustSameRoomOption;	// 룸옵션과 동일한 방 아니면 방 생성
	bool *bFindLocalServer;		// 방을 로컬 서버에서만 찾을지 여부 (ERR_ASKOTHERSERVER 받은 경우)
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

//내맘대로 바로입장
class CCL_ASK_PREMQUICKJOIN : public CCommMsg
{
public:	
	char*	m_idxGame;			// 조인할 게임종류 
	bool*	m_bUseSearch;		// 검색기능 사용여부
	int*	m_nSndFxKind; // ### [사운드 추가 작업] ###
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

//내맘대로 바로입장
class CSV_REFUSE_PREMQUICKJOIN : public CCommMsg
{
public: //방입장 실패
	enum
	{
		ERR_NOTSATISFIED,	//찾는 방이없다.
		ERR_ENTERROOMERROR, //방입장실패 
		ERR_NOQJITEM,	//아이템이 없거나 만료됨 
		ERR_ASKOTHERSERVER, // 다른 서버로 가서 내맘대로 바로입장을 요청해야 함		
		ERR_PLAYTIMEOVER,
		ERR_DAILYLOSSLIMIT,
	};
	char*	m_cErrCode;
	int*	m_nGameIndex;
	int*	m_nChanNo;
	bool*	m_bUseSearch;		// 검색기능 사용여부
	int*	m_nSndFxKind; // ### [사운드 추가 작업] ###
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

/////////// 전체 방 옵션 정보를 보내준다 (채널변경시 전송)
class CSV_ROOMOPTIONINFO : public CCommMsg
{
public:

	char *CreateOptionNum;
	ROOM_CREATEOPTION *pCreateOptionList;		// 생성가능 룸옵션정보

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


//DAT로그 (서버에 맥어드레스 통보)
class CCL_NOTIFY_MACADDRESS : public CCommMsg
{
public:		
	char *MACADDRESS;	//맥어드레스
	
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
	char *UMCode;		//통합머니코드 6byte;
	INT64* ChangeMoney;
	INT64* PMoney;
	int *SmallData_1, *SmallData_2;	//통합머니코드별 사용자 정의 int형 데이터
	INT64 *BigData_1, *BigData_2;	//통합머니코드별 사용자 정의 INT64형 데이터		


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

	////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
	int* m_ChipKind; //오버된 칩 종류를 추가한다.
	//////////////////////

	INT64* llOverMoney;						// 초과금
	INT64* llRevisionMoney;					// 보정머니
	SYSTEMTIME* stOverMoneyEndDate;			// 초과금 지급 만료시간
	SYSTEMTIME* stRevisionMoneyEndDate;		// 보정머니 지급 만료시간
	
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

// 회원제 상태 변경 알림
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


/////////// 접속게임 변경 요청
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

// 토너먼트에선 플레이타임 오링 10분전에 특별한 팝업을 띄워주기 위해서
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


// UPP time 관련 메시지 
// remainTimeSec : 한도가 정해져있는 유저의 경우(1등급) 남은 시간  
// playTimeSec : 현재까지 플레이한 시간 ( 모든 유저 ) -- 실게임시간
// nRound : 게임 진행한 판수 
// nRoundLimit : 게임 판수 한도 .( 1등급만 적용)

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

// 클라이언트에게 event 를 알려줌 
// cmd : 0 , 워닝창 , 30분( 3등급은 자기가 설정한 시간이 있으면) 지났음을 알려줌 
//     : 1 , 종료창 ,( 3등급의 경우에는 종료 예약 설정이 있으면 ) 1 시간 지났기 때문에 강제종료해야 됨 
//     : 2 , UPP 제한이 걸려서 로그인이 거부 되었음 
class CSV_UPPEVENT :public CCommMsg
{
public:
	enum UPPEVENT_TYPE 
	{
		WARNING , 
		EXIT ,			 // 알리미에 의한 EXIT 
		DENYLOGIN,
		EXITFORCE,       // 강제 exit ( 1등급이 판수 소진 이나 시간 소진 ) 
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

////////////////////////////2017.04.08 신규UPP 작업/////////////////////////////
/////2017년 새로운 UPP 도입으로 추가된 패킷이다.
class CSV_NEW_UPPINFO : public CCommMsg
{
public:
	IDX_GAME *m_idxGame; //해당 게임번호
	int *m_Level; //해당 게임의 이번달 UPP 레벨
	int *m_PlayCount; //게임판수
	int *m_PlayTime; //게임플레이시간
	int *m_BillingMoney; //지난달 결제금액
	int *m_State; //UPP 노티관련 상태값, 0.미통지(미처리), 1.통지완료(처리완료) ///1, 2 단계는 클라이언트에 전송만 하면 1로 변경되며, 3단계는 설문조사가 완료되어야 1로 변경됨

	/////////////////////////////UPP4단계, 셀프등록 관련 추가 2017.05.19/////////////////////////////////////////////
	//넷마블에서 DI기준으로 UPP를 걸었을때(m_Level == 5) 인 경우에만 유효함
	NEW_UPP_REASON *m_NM_UPP_Reason;
	char *m_StartDate;
	char *m_EndDate;
	char *m_UserCN;//UPP 3단계는 자가진단 웹페이지를 띄워야 하는데, 이 페이지에 파라매터로 CN을 넘겨야 한다.
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


// 클라이언트에게 현재 사용자의 UPP 관련 정보를 알려준다. 
class CSV_UPPINFO : public CCommMsg
{
public:
	int *m_level ; // UPP 레벨 
	int *m_agree ; // UPP 정책에 agree 했는지 여부 
	int *m_warning; // 경고 창 간격 ( 초단위 ) 
	int *m_exit   ; // 종료 예약 ( 초단위 )
	int *m_play   ; // 세션중의 수행한 게임 플레이 시간 
	int *m_round  ; // 세션중의 수행한 게임 판수 
	int *m_policy ; // 시간 알리미 알림창 뛰우는 정책, 0 :사용안함,1:한번,2:항상 
	int *m_usage  ; // 0 : 최초 로그인시 날라가는거, 1 : moving 시 날라가는거 , 2 : 시간알리미응답 

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

// 클라가 서버에게 보내주는 upp 관련 정보 메시지 
// cmd   1 :  UPP 정책에 동의했다는 것을 알려줌 
//       2 :  현재 유저의 UPP 시간 알리미 설정을 알려달라고 요청 
//       3 :  유저가 시간 알리미 설정을 했음을 서버에게 알려준다. 
//         ( param_1 에 경고시간 주기 , param_2 에 예약 종료 시간을 넣는다. 초단위 )
//		   ( param_3 에는 경고창 뛰우는 정책, 0:사용안함, 1:한번, 2:항상 )
// param_1 , param_2, param_3 는 향후 사용을 위한 파라미터들 
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
	bool*	m_bEmergencyCheck;		// 임시점검중: true,  정상상태: false
	bool*	m_bAdminUser;				// 임시점검중에도 입장가능여부
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


//디버그용 머니세팅 
class CCL_DEBUG_SETMONEY : public CCommMsg
{
public:
	enum MONEYFLAG{
	           ADD_MONEY=1,
			   SET_MONEY,
			   SET_FAMILY,
			   SET_PCROOM,
			   SET_GOLD,////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
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


//#if defined (__ROUND_MODE__)	// 라운드 모드용

// 라운드모드 예약
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

// 라운드모드 취소
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

// 라운드모드 취소
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

// 라운드모드 방 입장
class CSV_ROUND_MODE_SEND_SELECT_ENTER_ROOM : public CCommMsg
{
public:

	int *RoomNum;
	UINT64*	llRoomKey;			// IPS 룸키
	int *UNum;
	bool *bAutoEnterRoom;		// true 면 강제 입장되게 클라에서 처리
	
	void Set(int rnum, UINT64 llnRoomKey, int unum , bool AutoEnterRoom) // 추가
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


// 라운드모드 방 퇴장
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
	UINT64*	llRoomKey;			// IPS 룸키
	int *UNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;	
	int  *nEnter;	// 0: 취소 , 1: 입장

	void Set(int rnum, UINT64 llnRoomKey, int unum, int sndKind, char *id , int Enter) // 추가
	{
		char strid[16]={0,};
		char strpass[5]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		
		SetHeader(CL_ROUND_MODE_SEND_SELECT_ENTER_ROOM);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(llRoomKey), &llnRoomKey, sizeof(*llRoomKey));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(SndFxKind), &sndKind, sizeof(*SndFxKind)); // 추가
		PushData(_PVAR(ID), strid, 16);		
		PushData(_PVAR(nEnter), &Enter, sizeof(*nEnter));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(llRoomKey), sizeof(*llRoomKey));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(SndFxKind), sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
		TakeData(_PVAR(ID), 16);		
		TakeData(_PVAR(nEnter), sizeof(*nEnter));
	}
};
// 게임 시작전 3초 대기
class CSV_ROUND_MODE_GAME_START_WAIT_TIME : public CCommMsg
{
public:

	int *nWaitTimeCount;	// 대기 시간을 넣는다.
	INT64 *nTotalWinnerMoney;	// 총 상금

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

// 라운드 모드 게임 결과(마지막 순위 및 획득금액 표시) 
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


// 라운드 모드 현재 게임 판수
class CSV_ROUND_MODE_ROUND_COUNT : public CCommMsg
{	// 게임 시작시에 보내준다.
public:

	INT64 *nTotalRoundMoney;		// 총 상금
	BYTE *nCount;				// 라운드 횟수

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

// 라운드 모드에서 사용하는 칩 정보
class CSV_ROUND_MODE_BCAST_CHIPS : public CCommMsg
{	// 게임 결과 때에 보내준다.
public:
	ROUND_MODE_CHIPS *Chips;		// 유저별 칩 보유상황

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
{	// 게임 결과 때에 보내준다.
public:
	int *nStartTime;		//0-24
	int *nEndTime;		//0-24
	bool *bIsStart;		// 1: 시작

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

/////////////////////////////손실한도 초과 기능 추가 2017.05.15/////////////////////////////////////////////
class CSV_DAILYLOSSLIMIT_USER_SETTING_INFO : public CCommMsg
{
	//로그인, 채널이동시마다 현재 셋팅 상황을 클라이언트에 전달해 준다.
public:
	IDX_GAME *m_idxGame; //해당 게임번호
	bool *m_EnableOver; //손실한도를 오버해서 게임을 할 수 있는 상태
	bool *m_EnableSelect; //손실한도를 오버하겠다고 선택할 수 있는지 여부

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
	//유저가 오늘 손실한도를 오버해서 게임을 할래 안할래 선택한다.
public:
	IDX_GAME *m_idxGame; //해당 게임번호
	bool *m_WishOverGame; //오버해서 게임을 진행하는걸 원하는지 여부... 원하지 않으면 FALSE, 원하면 TRUE
	
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
	//24시간 제한에 걸린이후 채널 이동이나 로그인시 보상내용이 있으면 보상해 주는 패킷이다.
	//제한에 걸렸더라도 로그인 및 채널이동이 가능하므로, 보상금이 셋팅된 시점에서 다음날0시가 넘어가야 지급 가능
public:
	INT64 *m_RewardMoney; //보상 지급 머니
	SYSTEMTIME* m_BasicDate; //보상이 지급되는 기준시간.. 해당 시간 이후에 로그인 혹은 채널이동을 해야 보상 지급이 된다는 의미

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


////////////////////////////채널밸런싱, 올인천사 2017.08.10/////////////////////////////////////
class CSV_REWARD_ALLIN_ANGEL : public CCommMsg
{
	//올인천사 보상금을 알려주는 패킷이다.

public:
	INT64 *m_RewardMoney; //보상 지급 머니
	int *m_DailyEnableCount; //하루 보상 가능한 총횟수
	int *m_RemainEnableCount; //지금 보상 빼고 남아 있는 보상 가능 횟수

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


////////////////////////////한가위, 유통량확대 2017.08.25/////////////////////////////////////
class CSV_PLAYINFO_SUPERMOON : public CCommMsg
{
	//현재 슈퍼문 이벤트 진행 정보를 보내준다.

public:
	int *m_CurrentLevel; //현재 진행 레벨, 만일 이 값이 -1 이라면 오늘은 더이상 진행할 미션이 없다는 의미이다.
	int *m_GoalCount; //목표 카운트(해당 게임 판수가 되면 레벨 성공)
	int *m_CurrentCount; //진행한 게임판수

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
	//현재 슈퍼문 이벤트 당첨 정보를 보내준다.
public:
	int *m_CurrentLevel; //현재 진행 레벨
	INT64 *m_MaxRewardMoney; //최대로 보상 받을 수 있는 머니(실제 보상은 웹에서 이루어 진다.)

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
	//포커류 전체 땡값 이벤트가 진행중임을 알려준다.
	//만일 해당 이벤트가 없으면 이 패킷은 발송되지 않는다.
	//특별히 상세 정보는 없다.
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
	//슈퍼족보로 승리하였을때 땡값을 지불하는 패킷
	//방안에 있는 전체 유저에게 노티를 해야 하기 때문에, 아이디와 유저번호를 함께 넣었음.
public:
	char *m_UserID; //당첨자 아이디
	int *m_UNum; //유저번호
	int *m_UserGrade; //당첨유저 패밀리 등급
	int *m_ScoreCode; //당첨 족보코드
	int *m_ScoreSubCode; //당첨 족보 서브코드.. 로우에서 당첨시 위 족보코드는 0, 여기에 탑번호가 들어간다.
	int *m_TotalDailyRewardCount; //하루에 받을수 있는 보상 횟수
	int *m_RemainDailyRewardCount; //오늘 남아 있는 보상 횟수(지금 받은 당첨금 빼고)
	INT64 *m_RewardMoney; //당첨금

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


////////////////////////////블라인드배팅, 타겟알림 2017.10.12/////////////////////////////////////
class CSV_NOTICE_BIGWIN : public CCommMsg
{
	//x조이상 획득한 유저정보를 대기실에 있는 유저들에게 알려주는 패킷이다.
public:
	char *m_UserID; //당첨자 아이디
	INT64 *m_WinMoney; //당첨금
	int *m_ShowTime; //메세지 노출 시간

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
	//특정 유저에게 축하 메세지를 전달한다.
	//클라이언트가 서버로 보내는 패킷이다.
public:
	char *m_Receive_ID;			//받는사람 사람 ID
	short *l_StrChat;	// 메세지 스트링의 길이
	char *StrChat;		// 메세지 스트링

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
	//축하 메세를 누군가에게 받았다고 알려준다.
public:
	char *m_Send_ID;	//보낸사람 사람 ID
	short *l_StrChat;	// 메세지 스트링의 길이
	char *StrChat;		// 메세지 스트링
	int *m_ShowTime; //메세지 노출 시간

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


////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
class CSV_ETCCHIP_OPEN_REWARD : public CCommMsg
{
	//기타재화 오픈 기념으로 해당 재화를 지급했다고 알려준다.
public:
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_RewardMoney; //보상 지급 머니

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
	//골드채널 올인천사 보상금을 알려주는 패킷이다.

public:
	INT64 *m_RewardMoney; //보상 지급 머니
	int *m_DailyEnableCount; //하루 보상 가능한 총횟수
	int *m_RemainEnableCount; //지금 보상 빼고 남아 있는 보상 가능 횟수

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
	//기타 재화 적립 이벤트 정보를 알려준다.
	//아래 float 데이터들은 % 단위가 아님에 주의하자.
public:
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	float *m_BasicRate; //이벤트가 아니었을때 일반 채널에서 기본으로 제공되는 적립율..
	float *m_BasicRate_GoldCh; //이벤트가 아니었을때 골드채널에서 기본으로 제공되는 적립율..
	float *m_MemberGradeRate; //각 회원등급별 추가되는 적립율
	float *m_EventRate; //이벤트로 추가되어 제공되는 적립율
	float *m_RealRate; //위 기본 적립과 이벤트 적립을 합해 실제 적립되는 적립율..
	float *m_RealRate_GoldCh; //위 기본 적립과 이벤트 적립을 합해 실제 적립되는 적립율..(골드채널에서 게임시)
	bool *m_ChipUpdated; //이 패킷을 받기 직전에 실제 골드 적립이 이루어 졌는지 여부.. 
	                     //false이면 로그인, 채널이동시 단순 이벤트 정보이며, true이면 게임을 통해 적립된 이후 실제 적립율을 알려주는 정보이다.
	char *m_EventStartDate; //이벤트 시작일시
	char *m_EventEndDate; //이벤트 종료일시
	char *m_EventEveryStartTime; //이벤트 기간중 이벤트 시작 시간
	char *m_EventEveryEndTime; //이벤트 기간중 이벤트 종료 시간

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
	//기타칩이 Rake Back되어 변경된 내역만 보내준다.
	//Rake Back은 수수료의 일부를 적립금 형태로 지급해 주는걸 말함.
	//즉 게임을 통해 획득한 머니의 일부를 수수료로 내게 되는데, 이중 일부를 다시 돌려받는것~!!
public:
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_ChangeChip_Int; //변경된 금액의 정수부분
	int *m_ChangeChip_Prime; //변경된 금액의 소수부분
	float *m_BasicRate; //기본 Rake Back 비율(%단위가 아니다~).. 이 값은 0.002 등으로 오는데, 이경우 0.2% 가 적립되었다고 보면 됨
	float *m_BonusRate; //추가 보너스 배율(이 값이 2.0f 이면 두배 적립 이벤트라는 이야기..)
	//위 두개의 Rate를 곱해 실제 적립받은 비율을 계산할 수 있음
	//가령 m_BasicRate = 0.003, m_BonusRate = 2.0 이라면, 실제로 0.003 * 2.0 = 0.006(0.6%)를 적립받았다고 보면 됨

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
	//기타칩 변경된 금액 정보이다.

public:
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_ChangeChip; //화면에 보여주는 정수 부분만 필터링된 변화금액이다.
	INT64 *m_FullChangeChip_Int; //실제 소수점까지 변경된 금액의 정수부분이다.
	int *m_FullChangeChip_Prime; //실제 소수점까지 변경된 금액의 소수부분이다.
	//실제 변화된 금액 텍스트로 변경예 => Format("%I64d.%d"), m_FullChangeChip_Int, m_FullChangeChip_Prime) 

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
	//게임머니 외에 본인의 기타 칩 정보를 보내준다.
	//최초 로그인시, 채널이동시, 칩 변동시 해당 패킷을 클라이언트에 전송한다.
	//여기에 게임머니 대비 가치를 추가..2017.11.22
public:
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_RealChip; //화면에 보여주는 실제 보유금액이다.
	int *m_RemainChip; //소수점으로 계산되어 낙전으로 저장되어 있는 금액이다. UI상으로는 해당 금액이 노출되지 않치만, 혹시나 해서 넣었음
	INT64 *m_ChipValue; //해당 재화 1당, 게임머니 가치이다.. 이 값이 10억이면 1재화당 게임머니 10억이라는 의미

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
	//게임머니 외에 방안 유저의 기타 칩 정보를 보내준다.
	//방안에서 특정 유저의 기타 칩 정보를 다른 유저들에게 보내줄때 사용
public:
	int *m_pnum; //방안의 좌석번호(플레이어 번호)
	char *m_UserID; //해당 유저의 ID
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_RealChip; //화면에 보여주는 실제 보유금액이다.
	int *m_RemainChip; //소수점으로 계산되어 낙전으로 저장되어 있는 금액이다. UI상으로는 해당 금액이 노출되지 않치만, 혹시나 해서 넣었음

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
	//기타 재화를 게임머니로 변경신청한다.
public:
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_ChangeChip; //화면에 보여주는 실제 보유금액이다.

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
	//기타 재화를 게임머니로 변경하는것에 대한 결과코드이다.
public:
	int *m_Result; //결과코드..0.성공, 1.보유머니부족, 2.게임머니보유한도에걸림, 99.기타오류
	char *m_ChipType; //칩타입.. 처음 추가된 골드는 GD로 간다.
	INT64 *m_ChangeChip; //변경(신청)된 기타 재화금액
	INT64 *m_CurChip; //변경완료후 확정된 재화금액
	INT64 *m_CurGameMoney; //변경완료후 확정된 게임머니

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

////////////////////////////클럽시스템 2017.12.21/////////////////////////////////////
class CSV_CLUB_RESPONSE_ALL_LIST : public CCommMsg
{
	//운영중인 모든 클럽 정보를 리턴해 준다.
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
	//운영중인 클럽 전체 리스트를 요청한다.
public:
	int *m_MaxCount; //필요에 따라 최대치를 넣어주면 됨. -1일경우 무제한(있는거 다 보내줌)

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
	//BOSS가 클럽에서 강퇴시켰다는 알람을 본인에게 준다.
public:
	int *m_ReasonCode; //탈퇴이유.. enum CLUB_FIRE_REASON 데이터가 들어감. CLUB_FIRE_REASON_BOSS_KICK, CLUB_FIRE_REASON_CLOSE
	char *m_ClubName; //클럽이름

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
	//특정인 클럽 강퇴에 대한 결과이다.
public:
	int *m_Result; //0.성공, -1000.내부에러, -1001.클럽정보오류, -1002.존재하지 않는 회원, -1003.금일 탈퇴 한도 초과(1일1회)
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
	//보스가 자신의 클럽원중 특정인을 강제 탈퇴시킬때 보낸다.
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
	//클럽 탈퇴에 대한 결과를 보내준다.
public:
	int *m_Result; //0.성공, -1000.내부에러, -1001.클럽정보오류, -1002.재가입한 클럽에 당일 탈퇴불가
	char *m_ClubName; //클럽이름

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
	//유저가 현재 가입한 클럽에서 탈퇴한다는 요청이다.
	//보스가 이 패킷을 보낼경우, 자신이 생성한 클럽을 폭파 시킨다.
	//오직 하나의 클럽에만 가입할 수 있으므로, 추가로 보낼 정보는 없다.
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
	//내가 스스로 요청한 클럽 가입 리스트를 보내준다.
public:
	int *m_TodayMax; //오늘 보낼수 있는 가입요청 횟수
	int *m_TodayCount; //오늘 가입요청한 횟수
	int *m_Count; //리스트카운트(오늘 가입요청한 횟수가 아님)
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
	//내가 스스로 요청한 클럽 가입 리스트를 요청한다.
public:
	int *m_MaxCount; //필요에 따라 초대장 리스트의 최대치를 넣어주면 됨. -1일경우 무제한(있는거 다 보내줌)

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
	//해당 유저가 받은 초대리스트를 보내준다.
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
	//내가 받은 초대장 리스트를 요청한다.
public:
	int *m_MaxCount; //필요에 따라 초대장 리스트의 최대치를 넣어주면 됨. -1일경우 무제한(있는거 다 보내줌)

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
	//내가보낸 초대장 리스트 응답(보스용)
public:
	int *m_TodayMax; //오늘 보낼수 있는 초대 횟수
	int *m_TodayCount; //오늘 보낸 횟수
	int *m_Count; //리스트 카운트(오늘 보낸 횟수가 아님)
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
	//내가 보낸 초대장 리스트 요청(보스용)
public:
	int *m_MaxCount; //필요에 따라 초대장 리스트의 최대치를 넣어주면 됨. -1일경우 무제한(있는거 다 보내줌)

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
	//해당 유저가 받은 가입 요청 리스트를 보내준다.
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
	//내가 받은 가입요청 리스트
public:
	int *m_MaxCount; //필요에 따라 초대장 리스트의 최대치를 넣어주면 됨. -1일경우 무제한(있는거 다 보내줌)

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
	//클럽 가입 수락에 대한 결과를 보내준다.(수락은 했지만 인원초과 등의 이유로 가입이 안될수도 있으므로)
public:
	int *m_Result; //0.성공, -1000.내부에러, -1001.존재하지 않는 초대, -1002.클럽정보오류, -1003.가입제한인원 초과
	int *m_ActionCode; //수락 혹은 거부.. enum CLUB_ACTION_CODE 값을 넣으면 됨.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY
	///클럽가입에 성공해야 아래 데이터들이 유효하다.
	int *m_Club_ID; //클럽ID... 우리가 알고 있는 문자형 ID가 아니라, 숫자형 유니크 값이다.
	char *m_ClubName; //클럽이름
	
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
	//클럽 초대를 수락한다.
public:
	char *m_SendID; //초대장을 보낸 유저ID
	INT64 *m_InvteSeq; //초대 Sequence 번호
	int *m_ClubID; //클럽 고유 번호
	int *m_ActionCode; //수락 혹은 거부.. enum CLUB_ACTION_CODE 값을 넣으면 됨.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY

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
	//클럽 초대에 대한 결과이다.
public:
	int *m_Result; //0.성공, -1001.클럽정보오류, -1002.가입제한인원초과, -1003.이미초대한유저, 
					//-1004.초대자가 다른클럽 보스임, -1005.초대자가 다른 클럽에 가입중, -1006.존재하지 않는 유저임, -1007.오늘 클럽에서 탈퇴한 유저
	char *m_InviteID; //초대한 유저 ID

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
	//클럽 가입 초대장을 보낸다.
	//이 패킷을 보내는 유저는 클럽의 보스 상태여야만 한다.
public:
	char *m_InviteID; //초대할 유저 ID

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
	//클럽 가입요청 수락에 대한 결과이다.
public:
	int *m_Result; //0.성공, -1000.내부에러,-1001.클럽정보오류, -1002.가입제한인원초과, -1003.가입자가 다른클럽 보스임, -1004.가입자가 다른 클럽에 가입중
	int *m_ActionCode; //수락 혹은 거부.. enum CLUB_ACTION_CODE 값을 넣으면 됨.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY
	char *m_JoinID; //가입 요청을 보낸 유저ID

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
	//클럽 가입요청을 수락한다.
	//보스 회원만 이 패킷을 보낼 수 있음
public:
	char *m_JoinID; //가입 요청을 보낸 유저ID
	char *m_ClubName; //가입할 클럽 이름
	INT64 *m_JoinSeq; //가입요청 Sequence 번호
	int *m_ActionCode; //수락 혹은 거부.. enum CLUB_ACTION_CODE 값을 넣으면 됨.CLUB_ACTION_ALLOW = 0 / CLUB_ACTION_DENY

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
	//클럽에 가입 요청에 대한 결과이다.
public:
	int *m_Result; //0.성공, -1000.이미가입된 클럽이 있음, -1001.요청수 제한을 초과함 -1002.이미 해당클럽에 가입신청을 했음
	int *m_Club_ID; //클럽ID

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
	//클럽에 가입 요청서를 보낸다.
	//이 패킷을 보내는 유저는 자연인상태(클럽원도 아니고 보스도 아닌) 여야 한다.
public:
	int *m_Club_ID; //클럽ID

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
	//클럽 개설에 대한 결과
public:
	int *m_Result; //0.성공, -1000.내부에러, -1001.이미다른클럽에 가입중, -1002.당일생성불가
	///클럽생성에 성공해야 아래 데이터들이 유효하다.
	char *m_Club_Name; //클럽이름

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
	//클럽 개설을 요청한다.
	//클럽 이름은 보스의 닉네임으로 자동 대치 되므로, SET할 데이터가 아무것도 없음
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
	//내가 속한 클럽의 상세 정보를 보내준다.
	//클라가 요청할때 마다 DB상 최신 데이터를 로딩하여 보내준다.
public:
	char *m_Club_Name; //클럽이름
	int *m_Club_BossLevel; //클럽BOSS의 VIP 레벨이다.
	int *m_Club_MemberGrade; //클럽내 멤버 등급이다.enum CLUB_MEMBER_GRADE 와 대응되는 값..CLUB_MEMBER_GRADE_BOSS,CLUB_MEMBER_GRADE_NORMAL,CLUB_MEMBER_GRADE_NONE
	int *m_Club_MemberMax; //클럽에 가입할 수 있는 최대 인원
	int *m_Club_MemberCount; //클럽에 가입한 현재 인원
	INT64 *m_Club_Boss_GetGold_Today; //오늘 클럽원들에 의해 Boss에게 PayBack한 골드 총액이다.
	INT64 *m_Club_Boss_GetGold_Yesterday; //어제 클럽원들에 의해 Boss에게 PayBack한 골드 총액이다.

	int *m_Club_Next_MemberMax; //다음 등급에서 가질수 있는 클럽 최대 인원
	float *m_Boss_PayBack_Rate; //보스의 패이백 비율(%단위)
	float *m_Member_AddGet_Rate; //멤버가 되면서 받게되는 추가 적립 비율(%)

	int *m_Count; //클럽원 리스트의 버퍼 카운트.. 일반적인 경우 위 m_Club_MemberCount 값과 일치하겠으나, 이 값은 실제 버퍼의 카운트를 넣은 값이므로,
	             //아래 Array를 파싱할때, 이 값을 참조하면 정확할듯.
	CLUB_MEMBER_INFO *m_ArrayClubMemberInfo; //가입한 클럽원 리스트이다.

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
	//내가 속한 클럽의 상세 정보를 요청한다.
	//요청시 셋팅할 정보는 없음
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
	//내 클럽 가입 정보이다.
public:
	int *m_InfoType; //해당 정보가 왜 가는지 타입코드이다.enum CLUB_MYINFO_TYPE 과 대응되는 값
	int *m_Club_ID; //클럽ID... 우리가 알고 있는 문자형 ID가 아니라, 숫자형 유니크 값이다.
	char *m_Club_Name; //클럽이름
	char *m_Club_BossID; //클럽의 BOSS ID이다.
	int *m_Club_MemberGrade; //클럽내 멤버 등급이다.enum CLUB_MEMBER_GRADE 와 대응되는 값..CLUB_MEMBER_GRADE_BOSS,CLUB_MEMBER_GRADE_NORMAL,CLUB_MEMBER_GRADE_NONE
	bool *m_Club_Enable_Create; //현재 내 상태가 클럽 개설 가능한지 여부를 알려줌.. VIP등급은 되지만 이미 보스일 경우에도 false로 셋팅되어 감
	float *m_Member_AddGet_Rate; //멤버가 되면서 받게되는 추가 적립 비율(%)
	float *m_Boss_PayBack_Rate; //보스의 패이백 비율(%단위) - 보스가 되면 받는 적립비율
	
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
	//무언가 서버로 부터 Request하여 Response를 받아야 하는경우
	//정상적인 상황에서야 Response가 가겠지만, 아닐경우 에러 결과가 필요할때가 있다.
	//가령 이번 클럽디테일 정보 요청시, 클럽보스이거나 멤버가 아닐경우 Response가 안가게 되는데,
	//특별히 에러 패킷도 발송되지 않으니, 클라 입장에서는 적절하게 대응하기가 어렵다.
public:
	int *m_Result_Msg_ID; //메세지 ID이다. 각 패킷 헤더로 들어가는 값임. 예) SV_CLUB_MYINFO 이런거
	int *m_CommonErrCode; //에러 코드이다. enum COMMON_ERR_CODE에 정의된 값이 들어감

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

////////////////////////////2018설날이벤트 2018.01.22/////////////////////////////////////
class CSV_PROMOTION_COMMON_REWARD_ALL_NOTICE : public CCommMsg
{
	//누군가 보상지급을 받았다는 정보를 모든 유저에게 노티하는 용도이다.
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
	//출석보상이다~!!
public:
	int *m_Result; //0.정상지급, 1.해당유저의 DI가 같은 다른 계정이 이미 수령한 보상임
	int *m_Attendance_ID; //출석부 ID
	int *m_Attendance_Type; //출석형태, enum PROMOTION_ATTENDANCE_TYPE 에 있는 값이며 ATTENDANCE_NORMAL는 일반누적, ATTENDANCE_CONTINUE는 연속출석
	int *m_Attendance_Count; //출석일자
	char *m_RewardType;
	//GM.게임머니, GD골드, CP.쿠폰, DC.할인권, MBM.멤버스미니, MBS.멤버스 실버, MBG.멤버스골드, MBD.멤버스다이아, GLP.황금복주머니
	INT64 *m_RewardAmount; //보상금액.. 만일 멤버스 그런거면 일자라고 보면 됨

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
	//일반적으로 출석을 이용하여 진행하는 프로모션에 대한 정보이다.
	//혹시 출석부가 여러개 존재할수도 있어, 출석부 ID로 구분짓는다.
public:
	int *m_Attendance_ID; //출석부 ID
	int *m_NormalCount; //일반 누적횟수
	int *m_ContiCount; //연속 누적횟수
	bool *m_NewDayFlag; //이 값이 true이면 이번 접속이 오늘은 처음이라는 의미(보상 지급하는 타이밍이란 의미이다.)

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
	//일반적으로 게임판수를 이용하여 진행하는 프로모션에 대한 진행여부 정보이다.
	//대부분의 프로모션은 이 패킷을 전달할 필요가 없으나(기간이 설정되어 있다면 당연히 진행중이니까)
	//2018 치킨 이벤트 처럼 하루에도 시간을 정해 수시로 시작했다 종료하는 경우 필요한 정보이다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값이 들어감
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
	//일반적으로 게임판수를 이용하여 진행하는 프로모션에 대한 정보이다.
	//각 프로모션은 프로모션코드로 구별한다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값이 들어감
	int *m_CurrentGameCount; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
	int *m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	int *m_NeedGameCount; //보상 지급을 받기 위해 필요한 게임횟수
	int *m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
	int *m_TicketCount; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
	int *m_TicketLimitCount; //응모권등 보유할 수 있는 최대치

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
	//일반적으로 게임판수를 이용하여 진행하는 프로모션에 대한 보상발생 정보이다.
	//각 프로모션은 프로모션코드로 구별한다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값이 들어감
	int *m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	int *m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
	char *m_RewardType; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
	INT64 *m_RewardAmount; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
	char *m_RewardAddInfo; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
	SYSTEMTIME *m_ExpiredTime; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음

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
	//응모권으로 응모할 수 있는 상품 리스트를 요청한다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값이 들어가면 됨

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
	//응모권으로 응모할 수 있는 상품 리스트를 전송해 준다.
	//로그인 및 채널이동시 자동으로 보내주며, CCL_PROMOTION_COMMON_REQUEST_TICKET_GIFTLIST 패킷으로 요청시에도 전송해줌
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값
	int *m_Count; //응답갯수
	TICKET_GIFT_INFO *m_ArrayTicketGiftList; //등록된 상품 정보

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
	//티켓으로 응모한 현황을 요청한다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값

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
	//해당 유저의 티켓 응모 현황을 보내준다.
	//클라 요청시 DB에 있는 데이터를 그대로 보내주는 구조로 가자
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값
	int *m_MyTicketCount; //가지고 있는 응모권(티켓) 갯수
	int *m_Count; //응답갯수
	TICKET_SUBSCRIPTION_INFO *m_ArraySubscriptionList; //등록된 응모 정보

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
	//응모권을 사용하여 상품에 응모한다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값
	int *m_GiftIndex; //상품 고유번호

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
	//응모 결과를 리턴해 준다.
public:
	int *m_PM_Code; //enum PROMOTION_COMMON_GAMECOUNT_PMCODE에 정의된 값
	int *m_GiftIndex; //상품 고유번호
	int *m_ResultCode; //0이면 응모 성공
	int *m_SB_Count; //해당 상품에 몇번 응모하게 되었는지를 알려줌(응모성공시)

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

////////////////////////////골드경기장 골드적립 2018.02.20/////////////////////////////////////
class CSV_GOLD_TEMPMONEY_INFO : public CCommMsg
{
	//골드의 임시머니(적립) 정보이다.
public:
	INT64 *m_CurTempGold; //현재 보유한 임시 적립된 골드량이다. 이 필드로 현재 적립머니를 동기화 시키면 됨
	INT64 *m_ThisAddTempGold; //이번에 변화되는 골드량이다. 
	int *m_ThisAddTempGold_Remain; //이번에 변화되는 골드의 소수점 이하 낙전이다.
	//한게임 종료시 위 두개의 값으로 + 되었다는 UI를 구성하면 됨 (예) Temp.Format("+%I64d.%d", m_ThisAddTempGold, m_ThisAddTempGold_Remain);

	INT64 *m_MaxTempGold; //최대로 적립받을 수 있는 금액이다. 이 값과 m_CurTempGold 를 비교하여 최대치 도달등의 UI 구성을 해주면 된다.

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
	//임시골드가 실제골드로 반영되었음을 알려준다.
public:
	INT64 *m_TempGold; //업데이트되는 임시(적립된) 골드 금액이다.
	INT64 *m_RealGold; //합산되어 실제 보유하게된 골드 금액이다.(이전 보유머니 포함 총액)

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


////////////////////////////섯다 골드경기장, 골드지원금 2018.02.27/////////////////////////////////////
class CSV_GOLD_HELP_REWARD_INFO : public CCommMsg
{
	//골드 지원금 프로모션 정보이다.(진행중인지 여부를 알려줌)
public:
	bool *m_Playing; //현재 진행중인지 여부
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
	//골드 보상 지급 당첨 패킷이다.
public:
	int *m_Grade; //순위.. 이 값이 -1이면 꽝이다...
	INT64 *m_RewardGold; //보상금이다.

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


////////////////////////////홀짝미니게임 이벤트 2018.03.12/////////////////////////////////////
class CSV_PM_ODDEVEN_START_INFO : public CCommMsg
{
	//금일 홀짝 게임을 진행할 수 있는지를 알려준다.
	//이 패킷을 true로 받으면 게임진행을 할 수 있다고 판단하여, 서버에 게임 시작 요청 패킷을 보내면 된다.
public:
	bool *m_StartEnable;
	bool *m_NeedCaptcha; //캡차인증이 필요한지 여부이다.

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
	//게임시작을 요청한다.
public:
	////////////////////////////골드출석부 CAPTCHA 2018.07.31/////////////////////////////////////
	char *m_CaptchaKey; //캡차 페이지에서 받은 인증키를 넣어 준다.
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
	//게임시작 요청에 대한 결과이다.
public:
	int *m_ErrCode; //0.정상시작, 1.오늘이미진행함, 2.기타오류, 10.캡차코드불일치 enum PROMOTION_ODDEVEN_START_ERRCODE 참조

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
	//각 라운드가 진행되었음을 알려준다.
	//이 패킷을 받으면, 유저의 선택을 받아 홀 혹은 짝의 결과를 서버로 보내주면 된다.
public:
	int *m_Round; //단계 1 ~ 8까지 있음
	INT64 *m_CurrentReward; //지금까지 확보된 보상금액(배팅실패, 혹은 게임포기를 하면 받게 되는 보상금)
	INT64 *m_WinReward; //이번 라운드에서 배팅성공시 받게되는 보상금액이다.
	int *m_BetTime; //배팅제한시간이다(초), 만일 -1로 가면 제한이 없다는 의미

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
	//홀이냐, 짝이냐 유저의 배팅이다.
public:
	int *m_Round; //단계 1 ~ 8까지 있음
	int *m_BetSide; //배팅사이드 0.짝수, 1.홀수 enum PROMOTION_ODDEVEN_BET_SIDE 참조

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
	//유저배팅에 대한 결과이다.
	//오로지 이번 라운드 배팅에 대한 결과임.. 보상지급 확정 패킷은 별도로 분리~!!
public:
	bool *m_BetSuccess; //성공여부
	int *m_Round; //도전한 라운드
	int *m_CardNum; //이번판 결과로 부여된 카드번호이다.(세븐포커 카드 인덱스와 동일함)
	INT64 *m_RoundReward; //도전한 라운드에 배팅성공시 획득하게되는 보상금
	
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
	//최종적으로 보상 지급이 완료 되었다~!!
public:
	int *m_LastRound; //최종적으로 도전한 라운드
	INT64 *m_RewardMoney; //보상으로 지급된 금액

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
	//도중에 배팅 포기를 요청한다.
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
	//당첨자를 전체 유저에게 노티해 준다~!
public:
	char *m_UserID; //아이디 혹은 닉네임
	int *m_WinRound; //당첨 라운드
	INT64 *m_RewardMoney; //당첨금
	int *m_RoundWinner; //해당 라운드 당첨자 수

	void Set(const char *UserID, int WinRound, INT64 RewardMoney, int RoundWinner)
	{
		SetHeader(SV_PM_ODDEVEN_WINNER_NOTI);

		char strUserID[NICK_LEN] = {0,}; //닉네임 버퍼가 ID보다 크므로 닉네임 기준으로 잡아준다.
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

//////////////////////////// 휠 미니게임 이벤트 2019.07.15/////////////////////////////////////
class CSV_PM_WHEEL_START_INFO : public CCommMsg
{
	//금일 휠 게임을 진행할 수 있는지를 알려준다.
	//이 패킷을 true로 받으면 게임진행을 할 수 있다고 판단하여, 서버에 게임 시작 요청 패킷을 보내면 된다.
public:
	bool *m_StartEnable;
	bool *m_NeedCaptcha; //캡차인증이 필요한지 여부이다.
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
	//게임시작을 요청한다.
public:
	////////////////////////////골드출석부 CAPTCHA 2018.07.31/////////////////////////////////////
	char *m_CaptchaKey; //캡차 페이지에서 받은 인증키를 넣어 준다.
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
	//게임시작 요청에 대한 결과이다.
public:
	int *m_ErrCode; //0.정상시작, 1.오늘이미진행함, 2.기타오류 enum PROMOTION_WHEEL_START_ERRCODE 참조
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
	//홀이냐, 짝이냐 유저의 배팅이다.
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
	//유저배팅에 대한 결과이다.
	//오로지 이번 라운드 배팅에 대한 결과임..
public:
	INT64 *m_RoundReward; //도전한 획득하게되는 보상금
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
	//당첨자를 전체 유저에게 노티해 준다~!
public:
	char *m_UserID; //아이디 혹은 닉네임
	INT64 *m_RewardMoney; //당첨금
	int *m_RoundWinner; //해당 라운드 당첨자 수

	void Set(const char *UserID, int WinRound, INT64 RewardMoney, int RoundWinner)
	{
		SetHeader(SV_PM_WHEEL_WINNER_NOTI);

		char strUserID[NICK_LEN] = {0,}; //닉네임 버퍼가 ID보다 크므로 닉네임 기준으로 잡아준다.
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

//////////////////////////// 2019 한가위 이벤트 2019.09.05/////////////////////////////////////
class CSV_PM_2019HAVEST_INFO : public CCommMsg
{
	// 2019 추석 보상 대상자에게만 해당 패킷을 전달 한다.
public:

	int *m_RewardDay;		// 접속 일차
	INT64 *m_RewardMoney;	// 보상금

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
	// 2019 추석 보상 요청한다.
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
	// 2019 추석 보상 이벤트 금액
public:
	INT64 *m_Reward; //도전한 획득하게되는 보상금
	
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
////////////////////////////미니게임 빅휠 2018.06.04/////////////////////////////////////
// 남은 게임 횟수 요청 (일 최대 20회)
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

// 남은 게임 횟수 응답
class CSV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT : public CCommMsg
{
public:
	int* m_nRemainCount;	// 남은 게임 횟수
	INT64* m_llUsingGold;	// 사용하게 될 골드. 현재는 고정 400. 

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

// 게임 플레이 요청
class CCL_PM_BIGWHEEL_PLAY_GAME : public CCommMsg
{
public:
	INT64* m_llUsingGold;	// 사용하게 될 골드. 현재는 고정 400. 

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

// 게임 플레이 응답/결과
class CSV_PM_BIGWHEEL_PLAY_GAME : public CCommMsg
{
public:
	int* m_nErrorCode;			// 0: 성공, 1: 보유골드 부족: 2: 남은 횟수 부족, 3 이상 DB 에러..
	int* m_nRemainCount;		// 게임후 남은 횟수
	INT64* m_llUsedGold;		// 사용한 골드 (현재는 고정 400)
	INT64* m_llAddedMoney;		// 추가된 게임머니 (400으로 환전된 게임머니)

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

////////////////////////////2018년06월 오픈 2018.06.01/////////////////////////////////////
class CSV_ETCCHIP_INSURANCE_USER_CHANGE_INFO : public CCommMsg
{
	//해당 유저의 기타재화 보험금 변동 내역을 보내준다.(가입, 지급등)
public:
	char *m_ChipType; //칩타입.. 
	int *m_ChangeType; //1.가입, 2.지급
	INT64 *m_ChangeChip; //변동금액.. 항상 양수임에 주의하자~!!

	INT64 *m_CurrentChip; //위 처리 이후 보유금액
	int *m_FixedLevel; //위 처리이후 확정된 보험등급
	char *m_FixedName; //위 처리이후 확정된 보험이름
	INT64 *m_FixedSaveChip; //위 처리이후 남아 있는 적립금

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
	//해당 유저의 기타재화 보험금 가입 정보를 보내준다.
public:
	char *m_ChipType; //칩타입.. 
	int *m_Level; //0이면 미가입 상태
	char *m_Name; //보험명
	INT64 *m_SaveMoney; //누적 적립된 금액

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
	//골드계좌에 적립된 금액을 인출 요청한다.
public:
	INT64 *m_Amount; //인출 금액

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
	//골드계좌 인출 요청에 대한 결과이다.
public:
	int *m_Result; //결과코드 0.성공, 1.계좌잔액부족, 2.골드보유한도에걸림, 99.기타오류
	INT64 *m_WithDrawAmount; //인출된 금액(최초 요청한 금액)
	INT64 *m_RemainGoldBank; //인출후 골드계좌에 남아 있는 금액
	INT64 *m_CurGold; //인출금액 합산후 보유하게된 골드

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

// 카드 오픈 기능을 사용 또는 정지 시킨다.
class CCL_ASK_CARDOPEN : public CCommMsg
{

public:

	bool *m_bCardOpen; // 카드 오픈 여부

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

// 카드 오픈 기능을 사용 또는 정지 요청에 대한 결과
class CSV_ASK_CARDOPEN_RESULT : public CCommMsg
{

public:

	bool *m_bCardOpen; // 카드 오픈 여부

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

// 신고하기 결과 메시지
class CSV_REPORT_RIGUSER_RESULT : public CCommMsg
{
public:
	int* m_nErrorCode;			// 성공 여부: 0 - 성공, 그 외의 값 실패
	int* m_nRemainCount;		// 남은 신고 횟수

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
// 카페 시스템. 2018.12.20

// 카페/연합 회원 리스트 (본인 기준)
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

// 카페/연합 가입 요청
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

// 카페/엽합 초대
class CCL_CAFE_INVITE : public CCommMsg
{
public:
	CafeSystem::CafeKind::Type* cafeKind_;
	int* cafeID_;
	char* inviteUserID_;		// 초대할 유저 ID

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
	char* inviteUserID_;		// 초대할 유저 ID
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

// 게임 함께 하기(따라가기) => 기존 루틴.
// 클라이언트에서 CSV_ASK_ROOMINFO 전송 -> ROOMINFO 정보를 바탕으로 CSV_ASK_ENTERROOM 로 방 진입.
// ROOMINFO, SMALLROOMINFO에 카페 종류, 카페 ID 추가

// 미가입 유저의 포인트 적립 정보. 예비 포인트 -> 일정 기간내에 카페에 가입하면 예비 포인트를 준다.
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