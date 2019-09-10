
#pragma once

#include "CommMsg.h"

#include "BetStruct.h"

#define POKERLOGMAN_VER	777		// ���÷α� �������� ����


typedef enum _eUserType : unsigned char
{
	USER_TYPE_NORMAL = 0,
	USER_TYPE_SUPER,
	USER_TYPE_MAX

}TYPE_USER;


// Ÿ ���� ȣȯ �޼��� #########################################################

enum
{
	///////// ���÷αװ������� <-> �뱹�� ����
	LSV_CHECK_VERSION=4000,				// ���� üũ
	LSV_HEARTBEAT,
	LSV_ASK_LOGIN,						// �α��� ������ �α��� ��û
	LSV_ACCEPT_LOGIN,					// �α��� ���
	LSV_REFUSE_LOGIN,					// �α��� �ź�
	
	LSV_ADMINUSER_LIST_REQ,				// ������ ��� ��û
	LSV_ADMINUSER_LIST_RES,				// ������ ��� ����
	LSV_ADMINUSER_LIST_COMMIT_START,	// ������ ����� ���Ӱ� ���� ����
	LSV_ADMINUSER_LIST_COMMIT_DATA,		// ������ ������
	LSV_ADMINUSER_LIST_COMMIT_END,		// ������ ����� ���Ӱ� ���� ��


	LSV_CHEAT_MONEY_CONFIG,				// �α׳��� �Ӵ� ����
	LSV_CHEAT_PLAYCOUNT_CONFIG,			// �α׳��� �Ǽ� ����

	LSV_CHNENTER_NOFITY,				// �α��ε� ������� ������ �α׼����� ���� (Server -> LogServer)

	LSV_CHECK_CHEAT_USER,				// ��ϵǾ��ִ� �������� üũ		(Server	-> LogServer) 
	LSV_CHEAT_USER_INFO,				// ������ CHEAT �������� �˸�		(LogServer -> Server) 
	LSV_SHEAT_USER_TO_ADMIN,			// ���� Ŭ�����̾�Ʈ�� CHEAT �������� �˸�
	LSV_USER_PLAYCOUNT_OVER,			// ������ �����濡�� 2���̻��� ������..

	LCL_ADMIN_CHEAT_USER,				// �����ڰ� �ҷ� ���� ���� (���, ����)

	LSV_NOTIFY_CHEAT,					// ��ϵ� ������ ���� �ǽð� �˸�
										// �α��ν� �˸�
										// �����Ӵ�/�����Ӵ� �ʰ� �˸�, (Server -> LogServer)

	LSV_NOTIFY_CHEAT_TO_ADMIN,			// ���� ���� ��Ŷ������ LogServer ->Admin ���� �ణ Ʋ��

	LCL_SEARCH_CHEAT_INFO,				// Cheat���� �˻� (ID, IP) (Admin -> Server)
	LSV_SEARCH_RESULT_CHEAT_INFO,		// Cheat���� �˻� ��� ���� (ID, IP) (Server -> Admin)

	LCL_SEARCH_CHEAT_PLAY,				// ������ �÷��� ���� �˻� (Admin -> Server)
	LCL_SEARCH_CHEAT_PLAY_IN_ROOM,		// ������ �÷��� ���� �˻� ���ȣ �������� �˻� (Admin -> Server)
	LCL_SEARCH_RESULT_CHEAT_PLAY,		// ������ �ÿ��� �����˻� ��� ���� (Server -> Admin)

	LSV_SERVERMSG,						// �α׼��� �޼���


	LCL_NOTIFY_DOUBT_DATA,				// DOUBT DATA �뺸 : ����ȭ�� ��Ŷ
	LCL_NOTIFY_DOUBT_ORG_DATA,			// DOUBT DATA �뺸 : �������� ������
	LCL_SEARCH_DOUBT_LIST_REQ,			// DOUBT LIST �˻� ��û (�̰� ID, IP ������ �˻��ϴ°� ���ϴ� ��Ŷ)
	LCL_SEARCH_DOUBT_LIST_RES,			// DOUBT LIST �˻� ��� (�̰� ID, IP ������ �˻��ϴ°� ���ϴ� ��Ŷ)
	LCL_GET_DOUBT_LIST,					// DOUBT LIST �������� (�̰� ��ó�� ���� ���� �������⸦ ���ϴ� ��Ŷ)
	LCL_MODIFY_DOUBT_DATA,				// DOUBT ������ ����
	LCL_REMOVE_DOUBT_DATA,				// DOUBT ������ ����


};




///////////////////////////// ���Ӽ����� ////////////////////////////

/////////// ����Ű �䱸 
class CLSV_CHECK_VERSION : public CCommMsg
{
public:
	int *pnVersion;

	void Set(int nVersion)
	{
		SetHeader(LSV_CHECK_VERSION);
		PushData(_PVAR(pnVersion), &nVersion, sizeof(*pnVersion));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pnVersion), sizeof(*pnVersion));
	}
};

// �α��� ������ �α��� ��û
class CLSV_ASK_LOGIN : public CCommMsg
{
public:
	ST_LOGSERVER_LOGIN_INFO*	pLoginInfo;

	void Set( ST_LOGSERVER_LOGIN_INFO* plogininfo )
	{
		SetHeader( LSV_ASK_LOGIN );
		PushData( _PVAR(pLoginInfo), plogininfo, sizeof(*pLoginInfo) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(pLoginInfo), sizeof(*pLoginInfo) );
	}
};

/////////// ��Ʈ��Ʈ
class CLSV_HEARTBEAT : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(LSV_HEARTBEAT);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};


// �α��� ���
class CLSV_ACCEPT_LOGIN : public CCommMsg
{
public:
	int *nType;					// �������� ��������
	BYTE* bSuperUser;	// ������������ �Ϲ� ��������
	VSTRING szUserName;

	void Set(int ntype, BYTE bSU, char* szName)
	{
		SetHeader(LSV_ACCEPT_LOGIN);
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
		PushData(_PVAR(bSuperUser), &bSU, sizeof(*bSuperUser));
		PushVSTR(szUserName, szName );
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
		TakeData(_PVAR(bSuperUser), sizeof(*bSuperUser));
		TakeVSTR( szUserName );
	}
};

// �α��� �ź� 
class CLSV_REFUSE_LOGIN : public CCommMsg
{
public:
	int *nType;			// �ź�����

	void Set(int ntype)
	{
		SetHeader( LSV_REFUSE_LOGIN );
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

// --------------------------------------------
// add by inthesky (2010/6/29-14:19:39) - begin

class CLSV_ADMINUSER_LIST_REQ : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(LSV_ADMINUSER_LIST_REQ);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CLSV_ADMINUSER_LIST_RES : public CCommMsg
{
public:
	VSTRING strIP;
	VSTRING strName;
	BYTE* bSuperUser;	// ������������ �Ϲ� ��������

	void Set(char* szIP, char* szName, BYTE bSU)
	{
		SetHeader(LSV_ADMINUSER_LIST_RES);
		PushVSTR(strIP, szIP );
		PushVSTR(strName, szName );
		PushData(_PVAR(bSuperUser), &bSU, sizeof(*bSuperUser));
		
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeVSTR( strIP );
		TakeVSTR( strName );
		TakeData(_PVAR(bSuperUser), sizeof(*bSuperUser));
	}
};

class CLSV_ADMINUSER_LIST_COMMIT_START : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(LSV_ADMINUSER_LIST_COMMIT_START);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CLSV_ADMINUSER_LIST_COMMIT_DATA : public CCommMsg
{
public:
	VSTRING strIP;
	VSTRING strName;
	BYTE* bSuperUser;	// ������������ �Ϲ� ��������
	INT* nUserIdx;

	void Set(char* szIP, char* szName, BYTE bSU, INT nIdx)
	{
		SetHeader(LSV_ADMINUSER_LIST_COMMIT_DATA);
		PushVSTR(strIP, szIP );
		PushVSTR(strName, szName );
		PushData(_PVAR(bSuperUser), &bSU, sizeof(*bSuperUser));
		PushData(_PVAR(nUserIdx), &nIdx, sizeof(*nUserIdx));

	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeVSTR( strIP );
		TakeVSTR( strName );
		TakeData(_PVAR(bSuperUser), sizeof(*bSuperUser));
		TakeData(_PVAR(nUserIdx), sizeof(*nUserIdx));
	}
};

class CLSV_ADMINUSER_LIST_COMMIT_END : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(LSV_ADMINUSER_LIST_COMMIT_END);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

// add by inthesky (2010/6/29-14:19:39) - end
// --------------------------------------------


// �α׳��� �Ӵ� ����
class CLSV_CHEAT_MONEY_CONFIG : public CCommMsg
{
public:
	ST_CHEAT_MONEY_CONFIG*		m_pConfig;

	void Set( ST_CHEAT_MONEY_CONFIG* config )
	{
		SetHeader( LSV_CHEAT_MONEY_CONFIG );
		PushData( _PVAR(m_pConfig), config, sizeof(*m_pConfig) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(m_pConfig), sizeof(*m_pConfig) );
	}
};


// �α׳��� �Ǽ� ����
class CLSV_CHEAT_PLAYCOUNT_CONFIG : public CCommMsg
{
public:
	ST_CHEAT_PLAYCOUNT_CONFIG*		m_pConfig;

	void Set( ST_CHEAT_PLAYCOUNT_CONFIG* config )
	{
		SetHeader( LSV_CHEAT_PLAYCOUNT_CONFIG );
		PushData( _PVAR(m_pConfig), config, sizeof(*m_pConfig) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(m_pConfig), sizeof(*m_pConfig) );
	}
};





// ��ϵǾ��ִ� �������� üũ		(Server -> LogServer) 
class CLSV_CHECK_CHEAT_USER : public CCommMsg
{
public:
	int*					m_nServerNumber;			// ���� ��ȣ

	ST_REQ_CHEAT_USERINFO*			m_pCheatUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( int nServer, ST_REQ_CHEAT_USERINFO *pInfo )
	{
		SetHeader( LSV_CHECK_CHEAT_USER );
		PushData( _PVAR(m_nServerNumber), &nServer, sizeof(*m_nServerNumber) );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_REQ_CHEAT_USERINFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_nServerNumber), sizeof(*m_nServerNumber) );
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_REQ_CHEAT_USERINFO) );
	}
};

// ��ϵ� �����̰ų� �����ڰ� ��� �Ǵ� ���� ��û�� �ϸ� ���� (LogServer -> Server)
class CLSV_CHEAT_USER_INFO : public CCommMsg
{
public:
	ST_RES_CHEAT_USERINFO*			m_pCheatUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_RES_CHEAT_USERINFO *pInfo )
	{
		SetHeader( LSV_CHEAT_USER_INFO );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_RES_CHEAT_USERINFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_RES_CHEAT_USERINFO) );
	}

};

// ���� Ŭ�����̾�Ʈ�� CHEAT �������� �˸�
class CLSV_SHEAT_USER_TO_ADMIN : public CCommMsg
{
public:
	ST_CHEAT_USER_LOGIN_INFO*			m_pCheatUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_CHEAT_USER_LOGIN_INFO *pInfo )
	{
		SetHeader( LSV_SHEAT_USER_TO_ADMIN );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_CHEAT_USER_LOGIN_INFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_CHEAT_USER_LOGIN_INFO) );
	}

};


// ������ �����濡�� 2���̻��� ������..
class CLSV_USER_PLAYCOUNT_OVER : public CCommMsg
{
public:
	ST_PLAYCOUNT_OVER*			m_pPlayCountOver;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_PLAYCOUNT_OVER *pPlayOver)
	{
		SetHeader( LSV_USER_PLAYCOUNT_OVER );
		PushData( _PVAR( m_pPlayCountOver ), pPlayOver, sizeof( ST_PLAYCOUNT_OVER ) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR( m_pPlayCountOver ), sizeof( ST_PLAYCOUNT_OVER ) );
	}
};


// --------------------------------------------
// add by inthesky (2010/7/6-13:10:21) - begin
// comment : ä������� ���Ӽ����� AbuserMonitor Server�� �Ѱ��ִ� ��Ŷ. ����� ������ ����ִ�.
class CLSV_CHNENTER_NOTIFY : public CCommMsg
{
public:
	ST_CHNENTER_NOTIFY*			m_pChnEnterNotify;

	void Set( ST_CHNENTER_NOTIFY *pChnEnter)
	{
		SetHeader( LSV_CHNENTER_NOFITY );
		PushData( _PVAR( m_pChnEnterNotify ), pChnEnter, sizeof( ST_CHNENTER_NOTIFY ) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR( m_pChnEnterNotify ), sizeof( ST_CHNENTER_NOTIFY ) );
	}
};


class CLCL_NOTIFY_DOUBT_DATA : public CCommMsg
{
public:
	ST_DOUBT_NOTIFY_DATA*		m_pDoubtUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_DOUBT_NOTIFY_DATA *pInfo )
	{
		SetHeader( LCL_NOTIFY_DOUBT_DATA );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_NOTIFY_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_NOTIFY_DATA) );
	}
 
};

class CLCL_NOTIFY_DOUBT_ORG_DATA : public CCommMsg
{
public:
	ST_DOUBT_ORG_DATA*		m_pDoubtUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_DOUBT_ORG_DATA *pInfo )
	{
		SetHeader( LCL_NOTIFY_DOUBT_ORG_DATA );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_ORG_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_ORG_DATA) );
	}

};


class CLCL_MODIFY_DOUBT_DATA : public CCommMsg
{
public:
	ST_DOUBT_ORG_DATA*		m_pDoubtUserInfo;

	void Set( ST_DOUBT_ORG_DATA *pInfo )
	{
		SetHeader( LCL_MODIFY_DOUBT_DATA );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_ORG_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_ORG_DATA) );
	}
};

class CLCL_REMOVE_DOUBT_DATA : public CCommMsg
{
public:
	ST_DOUBT_REMOVE_DATA*		m_pDoubtRemoveInfo;			// CheatLopg_DoubtList ������ IDX ��ȣ
	
	void Set( ST_DOUBT_REMOVE_DATA *pInfo )
	{
		SetHeader( LCL_REMOVE_DOUBT_DATA );
		PushData( _PVAR(m_pDoubtRemoveInfo), pInfo, sizeof(ST_DOUBT_REMOVE_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtRemoveInfo), sizeof(ST_DOUBT_REMOVE_DATA) );
	}
};

class CLCL_GET_DOUBT_LIST : public CCommMsg
{
public:
	ST_GET_DOUBT_LIST*		m_pGetDoubtParam;			// �������� �Ķ����

	void Set( ST_GET_DOUBT_LIST *pInfo )
	{
		SetHeader( LCL_GET_DOUBT_LIST );
		PushData( _PVAR(m_pGetDoubtParam), pInfo, sizeof(ST_GET_DOUBT_LIST) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pGetDoubtParam), sizeof(ST_GET_DOUBT_LIST) );
	}
};

class CLCL_SEARCH_DOUBT_DATA_REQ : public CCommMsg
{
public:
	ST_SEARCH_DOUBT_DATA_PARAM* m_pInfo;

	void Set( ST_SEARCH_DOUBT_DATA_PARAM*	pInfo )
	{
		SetHeader( LCL_SEARCH_DOUBT_LIST_REQ );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_DOUBT_DATA_PARAM) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_DOUBT_DATA_PARAM) );
	}
};

class CLCL_SEARCH_DOUBT_DATA_RES : public CCommMsg
{
public:
	ST_DOUBT_ORG_DATA*		m_pDoubtUserInfo;			

	void Set( ST_DOUBT_ORG_DATA *pInfo )
	{
		SetHeader( LCL_SEARCH_DOUBT_LIST_RES );
		PushData( _PVAR(m_pDoubtUserInfo), pInfo, sizeof(ST_DOUBT_ORG_DATA) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pDoubtUserInfo), sizeof(ST_DOUBT_ORG_DATA) );
	}

};

// add by inthesky (2010/7/6-13:10:21) - end
// --------------------------------------------

// �����ڰ� �ҷ� ���� ��� / ����
class CLCL_ADMIN_CHEAT_USER : public CCommMsg
{
public:
	BYTE*							m_nResult;				// 0:���� / 1:���� / �α׼������� �����Ҷ��� ���
	ST_REQ_ADMIN_CHEAT_USER*		m_pCheatUserInfo;		// �ҷ����� ����, ���̵�, ������

	void Set( ST_REQ_ADMIN_CHEAT_USER *pInfo, BYTE nresult )
	{
		SetHeader( LCL_ADMIN_CHEAT_USER );
		PushData( _PVAR(m_nResult), &nresult, sizeof(*m_nResult) );
		PushData( _PVAR(m_pCheatUserInfo), pInfo, sizeof(ST_REQ_ADMIN_CHEAT_USER) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_nResult), sizeof(*m_nResult) );
		TakeData( _PVAR(m_pCheatUserInfo), sizeof(ST_REQ_ADMIN_CHEAT_USER) );
	}
};

class CLSV_NOTIFY_CHEAT : public CCommMsg
{
public:
	ST_CHEAT_NOTIFY*		m_pUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_CHEAT_NOTIFY *pInfo )
	{
		SetHeader( LSV_NOTIFY_CHEAT );
		PushData( _PVAR(m_pUserInfo), pInfo, sizeof(ST_CHEAT_NOTIFY) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pUserInfo), sizeof(ST_CHEAT_NOTIFY) );
	}

};

class CLSV_NOTIFY_CHEAT_TO_ADMIN : public CCommMsg
{
public:
	ST_CHEAT_NOTIFY_TO_ADMIN*		m_pUserInfo;			// �ҷ����� ����, ���̵�, ������

	void Set( ST_CHEAT_NOTIFY_TO_ADMIN *pInfo )
	{
		SetHeader( LSV_NOTIFY_CHEAT_TO_ADMIN );
		PushData( _PVAR(m_pUserInfo), pInfo, sizeof(ST_CHEAT_NOTIFY_TO_ADMIN) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pUserInfo), sizeof(ST_CHEAT_NOTIFY_TO_ADMIN) );
	}

};

	// Cheat���� �˻� (ID, IP) (Admin -> Server)
class CLCL_SEARCH_CHEAT_INFO : public CCommMsg
{
public:
	ST_SEARCH_CHEAT_INFO*	m_pInfo;

	void Set( ST_SEARCH_CHEAT_INFO*	pInfo )
	{
		SetHeader( LCL_SEARCH_CHEAT_INFO );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_CHEAT_INFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_CHEAT_INFO) );
	}
};

class CLSV_SEARCH_RESULT_CHEAT_INFO : public CCommMsg
{
public:
	ST_SERCH_RESULT_CHEAT_INFO*	m_pInfo;

	void Set( ST_SERCH_RESULT_CHEAT_INFO*	pInfo )
	{
		SetHeader( LSV_SEARCH_RESULT_CHEAT_INFO );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SERCH_RESULT_CHEAT_INFO) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SERCH_RESULT_CHEAT_INFO) );
	}
};


// ������ �÷��� ���� �˻� (Admin -> Server)
class CLCL_SEARCH_CHEAT_PLAY : public CCommMsg
{
public:
	ST_SEARCH_CHEAT_PLAY* m_pInfo;

	void Set( ST_SEARCH_CHEAT_PLAY*	pInfo )
	{
		SetHeader( LCL_SEARCH_CHEAT_PLAY );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_CHEAT_PLAY) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_CHEAT_PLAY) );
	}
};

class CLCL_SEARCH_CHEAT_PLAY_IN_ROOM : public CCommMsg
{
// public:
// 	ST_SEARCH_CHEAT_PLAY_IN_ROOM* m_pInfo;
// 
// 	void Set( ST_SEARCH_CHEAT_PLAY_IN_ROOM*	pInfo )
// 	{
// 		SetHeader( LCL_SEARCH_CHEAT_PLAY_IN_ROOM );
// 		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_CHEAT_PLAY_IN_ROOM) );
// 	}
// 	void Get( char* lpdata, int size )
// 	{
// 		if( !GetHeader( lpdata, size ) ) return;
// 		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_CHEAT_PLAY_IN_ROOM) );
// 	}
};

// ������ �ÿ��� �����˻� ��� ���� (Server -> Admin)
class CLCL_SEARCH_RESULT_CHEAT_PLAY : public CCommMsg
{
public:
	ST_SEARCH_RESULT_CHEAT_PLAY* m_pInfo;

	void Set( ST_SEARCH_RESULT_CHEAT_PLAY*	pInfo )
	{
		SetHeader( LCL_SEARCH_RESULT_CHEAT_PLAY );
		PushData( _PVAR(m_pInfo), pInfo, sizeof(ST_SEARCH_RESULT_CHEAT_PLAY) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(m_pInfo), sizeof(ST_SEARCH_RESULT_CHEAT_PLAY) );
	}
};

class CLSV_SERVERMSG : public CCommMsg
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
	
		SetHeader(LSV_SERVERMSG);
	
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