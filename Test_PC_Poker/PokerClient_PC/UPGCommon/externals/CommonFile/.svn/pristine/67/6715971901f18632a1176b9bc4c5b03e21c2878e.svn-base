// CommUtil.h: interface for the CCommUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUTIL_H__BD1567BA_AE5F_440D_9829_5F38C90358D2__INCLUDED_)
#define AFX_COMMUTIL_H__BD1567BA_AE5F_440D_9829_5F38C90358D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_MSGBUFFERNUM		20		// 메시지 버퍼 개수
#define MINIMUM_MSGBUFFERSIZE	36000	// 메시지 버퍼의 기본 사이즈


class CBufferUnit
{
public:

	BOOL	m_bUse;

	char*	m_pBuf;
	int		m_Size;

	CBufferUnit();
	~CBufferUnit();

	void Clear();
	void Destroy();

	void Init(int size);
	char* Prepare(int size);	// 버퍼내의 데이터를 파괴시키지 않고 버퍼 사이즈를 늘린다

	void Resize(int size);

	inline void SetUse() { m_bUse = TRUE; }
	inline void SetFree() { m_bUse = FALSE; }
};

class CMsgBuffer  
{
public:

	int			m_LastIdx;
	CBufferUnit m_Buf[MAX_MSGBUFFERNUM];

	CBufferUnit* GetFreeBuf();

	CMsgBuffer();
	virtual ~CMsgBuffer();

};

#endif // !defined(AFX_COMMUTIL_H__BD1567BA_AE5F_440D_9829_5F38C90358D2__INCLUDED_)
