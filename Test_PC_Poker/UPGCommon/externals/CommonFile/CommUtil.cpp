// CommUtil.cpp: implementation of the CCommUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CBufferUnit::CBufferUnit()
{
	Clear();
}

CBufferUnit::~CBufferUnit()
{
	Destroy();
}

void CBufferUnit::Clear()
{
	m_bUse = FALSE;

	m_pBuf = NULL;
	m_Size = 0;
}

void CBufferUnit::Destroy()
{
	if(m_pBuf) 
	{
#if defined(_ONLY_SERVER)
		Camel::Base::System::MPHeap::Free(NULL, m_pBuf);
#else 		// defined(_ONLY_SERVER)
		delete m_pBuf;
#endif	// defined(_ONLY_SERVER)
	}
	Clear();
}

void CBufferUnit::Init(int size)
{
#if defined(_ONLY_SERVER)
	m_pBuf = static_cast<char* >(Camel::Base::System::MPHeap::Alloc(NULL, size));
#else		// defined(_ONLY_SERVER)
	m_pBuf = new char[size];
#endif	// defined(_ONLY_SERVER)
	m_Size = size;
}

char* CBufferUnit::Prepare(int size)
{
	if(!m_pBuf) {
		// 최소 버퍼 사이즈와 요구 되는 사이즈 중에 큰 사이즈로 버퍼 할당
		Init(max(MINIMUM_MSGBUFFERSIZE, size));
		return m_pBuf;
	}

	// 버퍼 사이즈가 작다면 두배의 공간을 미리 확보한다(잦은 할당을 막기 위함)
	if(m_Size < size)
		Resize(size * 2);

	return m_pBuf;
}

void CBufferUnit::Resize(int size)
{
	char* ptmp = m_pBuf;
	int orgsize = m_Size;

#if defined(_ONLY_SERVER)
	m_pBuf = static_cast<char* >(Camel::Base::System::MPHeap::Alloc(NULL, size));
#else		// defined(_ONLY_SERVER)
	m_pBuf = new char[size];
#endif	// defined(_ONLY_SERVER)
	m_Size = size;

	if(ptmp) {
		memcpy(m_pBuf, ptmp, min(orgsize, size));
#if defined(_ONLY_SERVER)
		Camel::Base::System::MPHeap::Free(NULL, ptmp);
#else		// defined(_ONLY_SERVER)
		delete ptmp;
#endif	// defined(_ONLY_SERVER)
	}
}


//=======================================================================================


CMsgBuffer::CMsgBuffer()
{
	m_LastIdx = -1;
}

CMsgBuffer::~CMsgBuffer()
{
	for(int i=0; i<MAX_MSGBUFFERNUM; ++i) {
		m_Buf[i].Destroy();
	}
}

CBufferUnit* CMsgBuffer::GetFreeBuf()
{
	for(int i=0; i<MAX_MSGBUFFERNUM; ++i) {
		++m_LastIdx;
		if(m_LastIdx >= MAX_MSGBUFFERNUM)
			m_LastIdx = 0;
		if(!m_Buf[m_LastIdx].m_bUse) {
			m_Buf[m_LastIdx].SetUse();
			return &m_Buf[m_LastIdx];
		}
	}

	ASSERT(0);
	return NULL;
}
