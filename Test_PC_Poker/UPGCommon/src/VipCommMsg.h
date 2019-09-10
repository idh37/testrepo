
#pragma once

//#include <UPGPlatform.h>

#include "VipStruct.h"

#define POKERVIP_VER	590		// ���÷α� �������� ����



// Ÿ ���� ȣȯ �޼��� #########################################################

enum
{
	///////// ���÷αװ������� <-> �뱹�� ����
	VIPSV_HEARTBEAT = 5000,

	VIPSV_CHECK_VERSION,				// ���� üũ
	VIPSV_ASK_LOGIN,					// �α��� ������ �α��� ��û
	VIPSV_ACCEPT_LOGIN,					// �α��� ���
	VIPSV_REFUSE_LOGIN,					// �α��� �ź�

	VIPSV_SERVICE_CFG,					// ��㼳�� ����

	VIPSV_CHATDATA,

	VIPSV_SERVERMSG,

	VIPSV_REQUEST_COUNSEL,				// c->s ��� ��û
	VIPSV_REFUSE_COUNSEL,				// s->c ��� �ź�

	VIPSV_WAITING_COUNSEL,				// waiting
	VIPSV_WAITING_CANCEL,				// ��ٸ��°� ���

	VIPSV_CONNECT_COUNSEL,				// ��ڿ� ����
	VIPSV_CONNECT_COUNSEL_FAIL,			// ������ ���� ���� 

	VIPSV_EXIT_COUNSEL,					// �������

	VIPSV_WAITING_LIST,

};


/////////// ��Ʈ��Ʈ
class CVIPSV_HEARTBEAT : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(VIPSV_HEARTBEAT);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};


/////////// ����Ű �䱸 
class CVIPSV_CHECK_VERSION : public CCommMsg
{
public:
	int *pnVersion;

	void Set(int nVersion)
	{
		SetHeader(VIPSV_CHECK_VERSION);
		PushData(_PVAR(pnVersion), &nVersion, sizeof(*pnVersion));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pnVersion), sizeof(*pnVersion));
	}
};

// �α��� ������ �α��� ��û
class CVIPSV_ASK_LOGIN : public CCommMsg
{
public:
	ST_VIPSERVER_LOGIN_INFO*	pLoginInfo;

	void Set( ST_VIPSERVER_LOGIN_INFO* plogininfo )
	{
		SetHeader( VIPSV_ASK_LOGIN );
		PushData( _PVAR(pLoginInfo), plogininfo, sizeof(*pLoginInfo) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(pLoginInfo), sizeof(*pLoginInfo) );
	}
};



// �α��� ���
class CVIPSV_ACCEPT_LOGIN : public CCommMsg
{
public:
	int *nType;			// �������� ��������

	void Set(int ntype)
	{
		SetHeader(VIPSV_ACCEPT_LOGIN);
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

// �α��� �ź� 
class CVIPSV_REFUSE_LOGIN : public CCommMsg
{
public:
	int *nType;			// �ź�����

	void Set(int ntype)
	{
		SetHeader( VIPSV_REFUSE_LOGIN );
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

class CVIPSV_SERVICE_CFG : public CCommMsg
{
public:
	ST_SERVICE_ON_OFF *stSerceInfo;

	void Set( ST_SERVICE_ON_OFF* pserviceinfo )
	{
		SetHeader( VIPSV_SERVICE_CFG );
		PushData( _PVAR(stSerceInfo), pserviceinfo, sizeof(*stSerceInfo) );
	}
	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(stSerceInfo), sizeof(*stSerceInfo) );
	}

};

class CVIPSV_CHATDATA : public CCommMsg
{
public:

	char *ID;			// ���̵� ��Ʈ��
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
	int	*nRoom;

	void Set(char* id, char *chat, int room) 
	{
		char strId[16]={0,};
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();
	
		SetHeader(VIPSV_CHATDATA);
	
		if(strlen(id) < 16) strcpy(strId, id);
	
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(nRoom), &room, sizeof(*nRoom));

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(nRoom), sizeof(*nRoom));
	}

};


class CVIPSV_SERVERMSG : public CCommMsg
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
	
		SetHeader(VIPSV_SERVERMSG);
	
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

class CVIPSV_REQUEST_COUNSEL : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(VIPSV_REQUEST_COUNSEL);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CVIPSV_REFUSE_COUNSEL : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(VIPSV_REFUSE_COUNSEL);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CVIPSV_WAITING_COUNSEL : public CCommMsg
{
public:
	int *nCount;			// �ź�����

	void Set( int ncount) 
	{
		SetHeader(VIPSV_WAITING_COUNSEL);
		PushData(_PVAR(nCount), &ncount, sizeof(*nCount));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nCount), sizeof(*nCount));
	}
};

class CVIPSV_WAITING_CANCEL : public CCommMsg
{
public:
	void Set() 
	{
		SetHeader(VIPSV_WAITING_CANCEL);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CVIPSV_CONNECT_COUNSEL : public CCommMsg
{
public:
	ST_WAITING_USERINFO		*pInfo;

	void Set(ST_WAITING_USERINFO* pinfo) 
	{
		SetHeader(VIPSV_CONNECT_COUNSEL);
		PushData(_PVAR(pInfo), pinfo, sizeof(ST_WAITING_USERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pInfo), sizeof(ST_WAITING_USERINFO));
	}
};

class CVIPSV_CONNECT_COUNSEL_FAIL : public CCommMsg
{
public:
	ST_WAITING_USERINFO		*pInfo;

	void Set(ST_WAITING_USERINFO* pinfo) 
	{
		SetHeader(VIPSV_CONNECT_COUNSEL_FAIL);
		PushData(_PVAR(pInfo), pinfo, sizeof(ST_WAITING_USERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pInfo), sizeof(ST_WAITING_USERINFO));
	}
};

class CVIPSV_EXIT_COUNSEL : public CCommMsg
{
public:
	ST_WAITING_USERINFO		*pInfo;
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
	int	*nType;

	void Set(ST_WAITING_USERINFO* pinfo, char *chat, int type ) 
	{
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength();

		SetHeader(VIPSV_EXIT_COUNSEL);
		PushData(_PVAR(pInfo), pinfo, sizeof(ST_WAITING_USERINFO));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		PushData(_PVAR(nType), &type, sizeof(*nType));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pInfo), sizeof(ST_WAITING_USERINFO));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

class CVIPSV_WAITING_LIST : public CCommMsg
{
public:
	
	int						*TotUser;
	ST_WAITING_USERINFO		*ArrayUI;

	void Set(int totuser, ST_WAITING_USERINFO* pArrayUI) 
	{
		SetHeader(VIPSV_WAITING_LIST);
		PushData(_PVAR(TotUser), &totuser, sizeof(*TotUser));
		PushData(_PVAR(ArrayUI), pArrayUI, sizeof(ST_WAITING_USERINFO) * totuser);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TotUser), sizeof(*TotUser));
		TakeData(_PVAR(ArrayUI), sizeof(ST_WAITING_USERINFO) * (*TotUser));
	}
};