#ifndef __COMMMSG_H__ 
#define __COMMMSG_H__ 

#include "UserStruct.h"
#include "CommUtil.h"

#pragma warning (disable:4996)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define MSG_ID_LEN	2		// 메세지 인식문자 길이
#define MSG_ID ("NP")		// 메세지 인식문자

#define _PVAR(x)	((void**)&(x))	// 포인터 변환용 매크로

// 패킷 헤더 구조체
typedef struct
{
	char MsgID[2];
	short	Signal;				// 메시지 시그널
	int	MsgLen;				// 메시지 길이 unsigned short -> UINT
	char	Encode;				// Encode 상태
	char	Compress;			// 압축 상태 1:압축없음, 2~5:압축 예약, 10:압축하면 더커지는상태, 20~50:압축된 데이터
	unsigned short PackCnt;			// 패킷 카운터 UINT -> unsigned short
	UINT PackCRC;
} PACKETHEADER;


#define PACKET_HEADERSIZE sizeof(PACKETHEADER)




// 통신 메세지 기본 클래스 #########################################################
class CCommMsg
{
public:

	// 가변 길이 스트링
	struct VSTRING
	{
		short* pbufsize;
		char*  pstr;
	};

public:

	int				TotalSize;		// 헤더를 포함한 데이터의 총 사이즈
	char*			pData;			// 데이터 포인터(헤더 포함)
	PACKETHEADER*	pHead;			// 패킷 헤더 포인터

	CCommMsg();						// 생성자
	~CCommMsg();

	void ClearMember();
	void Destroy();
	int GetTotalSize() const;

	static void BuildPattern(char *pdata,UINT packcnt,int size);
	static void EncodeAll(char *pdata);
	static BOOL DecodeAll(char *pdata);

	static int  CompressAll( BYTE* pData, BYTE* pbuf );
	static int	DePressAll( BYTE** pSrc );
	static void SetCompressSizeLimit( int size );
	static BOOL IsCompress( int length );
	static void SetCompressReservedType(int type );
	static void CheckCompressSizeInfo(char *lpdata, int *outputsize);
	static CString GetCompressType();

	// 메모리 버퍼의 데이터로부터 헤더 메시지를 분석하여 필요한 정보를 알려줌
	// (lpdata는 최소한 헤더가 생성되는데 필요한 기본사이즈 이상 되어야한다)
	static BOOL CheckInfo(char *lpdata, int *psignal, int *pmsglen);

	static int		CompressReservedType;
	static int		CompressSizeLimit;

	// 받은 데이터를 이용하여 헤더를 세팅한다
	// 헤더를 포함한 데이터의 첫번째 포인터와 데이터의 전체 사이즈를 받는다
	BOOL GetHeader(char* lpdata, int size);

protected:
#if defined(_ONLY_SERVER)
	CBufferUnit	m_clBufUnit;
#else		// defined(_ONLY_SERVER)
	CBufferUnit*	pBufUnit;		// 메시지 생성용 버퍼 클래스 포인터
#endif	// defined(_ONLY_SERVER)

	int				NowOffset;		// 메모리 복사중인 최종 오프셋 위치

	// 헤더를 만들어주고 필요한 사이즈만큼 메모리를 할당한다
	BOOL SetHeader(int signal);

	void PushData(void** var, const void* src, int size);
	void TakeData(void** var, int size);
	void PushVSTR(VSTRING& vstr, char* str);
	void TakeVSTR(VSTRING& vstr);
};

#endif // #define __COMMMSG_H__ 