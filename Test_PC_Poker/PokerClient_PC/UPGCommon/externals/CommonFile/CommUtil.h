// CommUtil.h: interface for the CCommUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUTIL_H__BD1567BA_AE5F_440D_9829_5F38C90358D2__INCLUDED_)
#define AFX_COMMUTIL_H__BD1567BA_AE5F_440D_9829_5F38C90358D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_MSGBUFFERNUM		20		// �޽��� ���� ����
#define MINIMUM_MSGBUFFERSIZE	36000	// �޽��� ������ �⺻ ������


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
	char* Prepare(int size);	// ���۳��� �����͸� �ı���Ű�� �ʰ� ���� ����� �ø���

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
