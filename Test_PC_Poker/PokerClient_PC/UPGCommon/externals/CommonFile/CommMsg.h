#ifndef __COMMMSG_H__ 
#define __COMMMSG_H__ 

#include "UserStruct.h"
#include "CommUtil.h"

#pragma warning (disable:4996)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define MSG_ID_LEN	2		// �޼��� �νĹ��� ����
#define MSG_ID ("NP")		// �޼��� �νĹ���

#define _PVAR(x)	((void**)&(x))	// ������ ��ȯ�� ��ũ��

// ��Ŷ ��� ����ü
typedef struct
{
	char MsgID[2];
	short	Signal;				// �޽��� �ñ׳�
	int	MsgLen;				// �޽��� ���� unsigned short -> UINT
	char	Encode;				// Encode ����
	char	Compress;			// ���� ���� 1:�������, 2~5:���� ����, 10:�����ϸ� ��Ŀ���»���, 20~50:����� ������
	unsigned short PackCnt;			// ��Ŷ ī���� UINT -> unsigned short
	UINT PackCRC;
} PACKETHEADER;


#define PACKET_HEADERSIZE sizeof(PACKETHEADER)




// ��� �޼��� �⺻ Ŭ���� #########################################################
class CCommMsg
{
public:

	// ���� ���� ��Ʈ��
	struct VSTRING
	{
		short* pbufsize;
		char*  pstr;
	};

public:

	int				TotalSize;		// ����� ������ �������� �� ������
	char*			pData;			// ������ ������(��� ����)
	PACKETHEADER*	pHead;			// ��Ŷ ��� ������

	CCommMsg();						// ������
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

	// �޸� ������ �����ͷκ��� ��� �޽����� �м��Ͽ� �ʿ��� ������ �˷���
	// (lpdata�� �ּ��� ����� �����Ǵµ� �ʿ��� �⺻������ �̻� �Ǿ���Ѵ�)
	static BOOL CheckInfo(char *lpdata, int *psignal, int *pmsglen);

	static int		CompressReservedType;
	static int		CompressSizeLimit;

	// ���� �����͸� �̿��Ͽ� ����� �����Ѵ�
	// ����� ������ �������� ù��° �����Ϳ� �������� ��ü ����� �޴´�
	BOOL GetHeader(char* lpdata, int size);

protected:
#if defined(_ONLY_SERVER)
	CBufferUnit	m_clBufUnit;
#else		// defined(_ONLY_SERVER)
	CBufferUnit*	pBufUnit;		// �޽��� ������ ���� Ŭ���� ������
#endif	// defined(_ONLY_SERVER)

	int				NowOffset;		// �޸� �������� ���� ������ ��ġ

	// ����� ������ְ� �ʿ��� �����ŭ �޸𸮸� �Ҵ��Ѵ�
	BOOL SetHeader(int signal);

	void PushData(void** var, const void* src, int size);
	void TakeData(void** var, int size);
	void PushVSTR(VSTRING& vstr, char* str);
	void TakeVSTR(VSTRING& vstr);
};

#endif // #define __COMMMSG_H__ 