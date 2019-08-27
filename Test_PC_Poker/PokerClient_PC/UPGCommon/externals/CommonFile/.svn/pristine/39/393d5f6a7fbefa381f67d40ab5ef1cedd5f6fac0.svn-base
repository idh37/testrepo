#include "StdAfx.h"
#include "CommMsg.h"

#include "CompressUtil.h"

//압축 라이브러리 사용
#ifdef _DEBUG 
#pragma comment(lib, "Compress_Debug.lib")
#else
#pragma comment(lib, "Compress_Release.lib")
#endif 


#ifdef _DEBUG
int CCommMsg::CompressSizeLimit = (int)0;
int CCommMsg::CompressReservedType = COMPRESSTYPE_LZO_RESERVED;
#else
int CCommMsg::CompressSizeLimit = (int)200;
int CCommMsg::CompressReservedType = COMPRESSTYPE_NONE;
#endif

void CCommMsg::SetCompressReservedType(int type)
{
	CCommMsg::CompressReservedType = type;
}

void CCommMsg::SetCompressSizeLimit(int size)
{
	if(MAX_POOLSIZE >= size)
	{
		CCommMsg::CompressSizeLimit = size;
	}
}

////////////////////////////////////////////////////////////////////////////
//                              메세지 기본 클래스
////////////////////////////////////////////////////////////////////////////
// 메시지 버퍼 관리 클래스 
static CMsgBuffer	s_MsgBuf;

// 암호화/복호화용 테이블
static char g_GHashTable[256] =
{
	122, -64,-113,  44, 107, -36, -78, -93, -92,-106,-116,  54,
		-58, -29, -42,  11, 110,  75,  64,  81, -70,  92, -45, 119,
		36, -65,  53, 111, -98, 104,  22,  57,  91, -63, -35, -87,
		70,  65, -54, -14, -91, -68, -90,  89, 108, -96, -20, -61,
		39, -15,-128, 126, -12, -34,  68, -44,  79,  61, -86, -53,
		-37,  60,-102, 115, -66,  13, -16,  20, -97, -17,  25, 121,
		49,  50,  23,  24, -55, -30,  31,  -5,-120,  86, 127,-126,
		21, 123,  30,  38, -21,  12, -40,  95,  33,-112,  16, -24,
		-56,  84, -33,  69,   3, 109, 102, -80,  73,  96,  47,  40,
		-59,  66,  85,-100,  34, -82,  67,  99,-122, -39,  93,  59,
		-11,  -8, 124, 103,  46, -43, -71,-108,  56,  88,  -9,  58,
		76,-124, -74,  -1,  51, -81, -83, -94, -28, -50,   2,  71,
		-95, 100, -73,  17, -85,  37,  83, 105, -10, -46, -23,  18,
		-2,-107,  94, -57,-111,   4, -31,-118,  19, -51,  27, -62,
		112, -84,  64,   5,  42, -89,  14,  -4,  78, -18, -88, 113,
		-3,-121, 116,-110,   9,-119,  10, -19, -99,  80,  77, -38,
		-123,  90, -48,  97,-127, -77,  -6, -67, 114,  35,  48, -32,
		-76,  72, 101,-115, -75,  32, -79, -52,-117, 118, -49,  45,
		-104,  26,  63, -25, -60,  62, -13,-109,  15, -27, -47,  55,
		117,  28,  82,  87,   6, 106,   8, -72,-114, 125,   7,  52,
		29,  41,  -7,  43,-105,  98,-125, -26,-103, 120,  74,   1,
		-41, -22,-101, -69
};


CCommMsg::CCommMsg()
{
	ClearMember();
}

CCommMsg::~CCommMsg()
{
	Destroy();
}

void CCommMsg::ClearMember()
{
	pData=NULL;
	TotalSize = 0;

#if defined(_ONLY_SERVER)
#else		// defined(_ONLY_SERVER)
	pBufUnit = NULL;
#endif	// defined(_ONLY_SERVER)
	NowOffset = 0;
}

void CCommMsg::Destroy()
{
#if defined(_ONLY_SERVER)
#else		// defined(_ONLY_SERVER)
	if(pBufUnit)
		pBufUnit->SetFree();
#endif	// defined(_ONLY_SERVER)

	ClearMember();
}

int CCommMsg::GetTotalSize() const
{
	if(pData==NULL) return 0;
	return TotalSize;
}

void CCommMsg::BuildPattern(char *pdata, UINT packcnt, int size)
{
	int i;
	switch((packcnt>>4)&4) {
		case 0:
			{
			} break;

		case 1:
			{
				char temp;
				int szi = (size >> 1);
				if(szi < 2) return;
				for(i = 0; i < szi;i++) {
					temp = pdata[i];
					pdata[i] = pdata[szi+i];
					pdata[szi+i] = temp;
				}
			} break;

		case 2:
			{
				int szi;
				WORD tempWD;
				WORD *pWdata = (WORD *)pdata;
				szi = (size >> 1) -1;
				if(szi < 2) return;
				for(i = 0; i < szi; i+=2) {
					tempWD = pWdata[i];
					pWdata[i] = pWdata[i+1];
					pWdata[i+1] = tempWD;
				}
			} break;

		case 3:
			{
				char temp;
				int szi = size-1;
				if(szi < 2) return;
				for(i = 0; i < szi;i++) {
					temp = pdata[i];
					pdata[i] = pdata[i+1];
					pdata[i+1] = temp;
				}
			} break;
	}
}

void CCommMsg::EncodeAll(char *pdata)
{
	char *pUserData = (char *)(pdata + sizeof(PACKETHEADER));
	PACKETHEADER *phead = (PACKETHEADER*)pdata;

	if(phead->Encode==2) return;

	phead->PackCRC = g_GHashTable[phead->PackCnt & 255] & 0x7F;

	for(UINT i=0; i<phead->MsgLen; i++)
	{
		phead->PackCRC += (pUserData[i]&0x5F);
		pUserData[i] += g_GHashTable[(phead->PackCnt + 51) & 255];	
		pUserData[i] ^= g_GHashTable[(phead->PackCnt + 27) & 255];
	}
	phead->PackCRC += g_GHashTable[(31+phead->PackCnt) & 255] & 0x7F;
	phead->PackCRC += phead->Signal;
	phead->PackCRC = (phead->PackCRC & 0x0FFFFFFF);

	if(phead->PackCRC<0x0FFFF) {
		phead->PackCRC += ((g_GHashTable[phead->PackCnt & 255] & 0xF) << 24) +
					((g_GHashTable[(phead->PackCnt+1) & 255] & 0xF) << 20) +
				    ((g_GHashTable[(phead->PackCnt+3) & 255] & 0xF) << 16);
		phead->PackCRC = (phead->PackCRC << 4) + 0xF;
	}
	else
	if(phead->PackCRC<0x0FFFFF) {
		phead->PackCRC += ((g_GHashTable[phead->PackCnt & 255] & 0xF) << 24) +
					((g_GHashTable[(phead->PackCnt+1) & 255] & 0xF) << 20);
		phead->PackCRC = (phead->PackCRC << 4) + (g_GHashTable[(phead->PackCnt+5) & 255] & 0x7);

	}else {
		phead->PackCRC = (phead->PackCRC << 4) + 0x0A;
	}
	phead->Signal += g_GHashTable[phead->PackCnt & 255];

	phead->Encode = 2;
	BuildPattern(pUserData, phead->PackCRC, phead->MsgLen);
}

BOOL CCommMsg::DecodeAll(char *pdata)
{
	int signal =0;
	char *pUserData = (char *)(pdata + sizeof(PACKETHEADER));
	PACKETHEADER *phead = (PACKETHEADER*)pdata;
	if( phead->Encode != 2 ) return FALSE;
	BuildPattern(pUserData, phead->PackCRC, phead->MsgLen);

	phead->Signal -= g_GHashTable[phead->PackCnt & 255];
	UINT tempCRC = g_GHashTable[phead->PackCnt & 255] & 0x7F;
	for(UINT i=0; i<phead->MsgLen; i++) {
		pUserData[i] ^= g_GHashTable[(phead->PackCnt + 27) & 255];
		pUserData[i] -= g_GHashTable[(phead->PackCnt + 51) & 255];
		tempCRC += (pUserData[i]&0x5F);
	}
	tempCRC+=g_GHashTable[(31+phead->PackCnt) & 255] & 0x7F;


	tempCRC = tempCRC & 0x0FFFFFFF;

	int flag = phead->PackCRC & 0x0F;
	phead->PackCRC = phead->PackCRC >> 4;
	if(flag==0x0F) phead->PackCRC &= 0x0FFFF;
	else
	if(flag==(g_GHashTable[(phead->PackCnt+5) & 255] & 0x7)) phead->PackCRC &= 0x0FFFFF;
	else
	if(flag!=0x0A)
	{
		return FALSE;
	}
	signal = phead->PackCRC - tempCRC;

	if(signal != phead->Signal) {
		return FALSE;
	}
	phead->Encode  = 1;
	if(phead->PackCRC != tempCRC+signal) {
		return FALSE;
	}

	return TRUE;
}

BOOL CCommMsg::IsCompress(int length)
{
	if( length > CCommMsg::CompressSizeLimit &&  
		CCommMsg::CompressReservedType != COMPRESSTYPE_NONE )
	{
		return TRUE;
	}
	return FALSE;
}

int	CCommMsg::CompressAll(BYTE* pInput, BYTE* pOutput)
{	
	PACKETHEADER * phead = (PACKETHEADER *)pOutput;

	int CompressType = phead->Compress;
	if(CompressType == COMPRESSTYPE_NONE) return phead->MsgLen;		// Safe Code

	//압축안된 데이터
	BYTE* Input_ptr =  (BYTE*)(pInput + sizeof(PACKETHEADER));
	DWORD Input_size = phead->MsgLen;
	DWORD comp_size = MAX_POOLSIZE-1;
	//압축
	UtilCompress(CompressType, pOutput + sizeof(PACKETHEADER) + sizeof(COMPRESS_HEADER), comp_size, Input_ptr, Input_size );

	// 압축예약에서 압축상태로 플레그 변경
	phead->Compress *= 10;	

	//압축된경우 헤더를 추가한다.
	COMPRESS_HEADER compressheader;
	compressheader.Size = Input_size;
	memcpy( pOutput + sizeof(PACKETHEADER) , &compressheader , sizeof(COMPRESS_HEADER) );
	return comp_size + sizeof(COMPRESS_HEADER);
}

int	CCommMsg::DePressAll(BYTE** pSrc)
{
	PACKETHEADER *phead = (PACKETHEADER*)*pSrc;
	// 압축이 안된 데이터
	if(phead->Compress < COMPRESSTYPE_RESERVED ) return phead->MsgLen;		// Safe Code

	PACKETHEADER pTemp;
	memcpy(&pTemp, phead, sizeof(PACKETHEADER));

	BYTE * pData= (BYTE*)(*pSrc +  sizeof(PACKETHEADER) + sizeof(COMPRESS_HEADER) );

	BYTE* original_ptr = (BYTE*)pData;
	DWORD original_size = phead->MsgLen - sizeof(COMPRESS_HEADER);
	DWORD decompressed_size = MAX_POOLSIZE-1;

	// 미리 만들어진 버퍼를 압축 해제에 사용한다.
	CBufferUnit* p_InBuffer = s_MsgBuf.GetFreeBuf();
	if(!p_InBuffer) return NULL;

	COMPRESS_HEADER * pcompressheader = (COMPRESS_HEADER*)(*pSrc + sizeof(PACKETHEADER)); 
	BYTE* pOutputData = (BYTE*)p_InBuffer->Prepare(pcompressheader->Size + sizeof(PACKETHEADER));

	//압축 해제
	int decompresstype = phead->Compress;

	UtilDecompress(decompresstype, pOutputData, decompressed_size, original_ptr, original_size);
	// 플레그 상태를 압축 안된 상태로 변경한다.
	pTemp.Compress = COMPRESSTYPE_NONE;
	pTemp.MsgLen = decompressed_size;

	if(decompressed_size > (MAX_POOLSIZE-sizeof(PACKETHEADER )))
	{
		//일어나선 안된다
		p_InBuffer->SetFree();
		return phead->MsgLen;		// Safe Code
	}

	BYTE *pRet = *pSrc;
	memcpy(pRet, &pTemp, sizeof(PACKETHEADER));
	memcpy(pRet+sizeof(PACKETHEADER), pOutputData, decompressed_size);
	p_InBuffer->SetFree();

	return decompressed_size;
}

void CCommMsg::CheckCompressSizeInfo(char *lpdata, int *outputsize)
{
	PACKETHEADER *pHeader = (PACKETHEADER*)lpdata;
	if(pHeader->Compress < COMPRESSTYPE_RESERVED ) return;

	COMPRESS_HEADER *pH = (COMPRESS_HEADER*)(lpdata + sizeof(PACKETHEADER));
	//헤더를 얻고 헤더에서 사이즈 정보 주소를 가져온다
	unsigned char* sizeheader = (unsigned char*)&pH->Size;
	int size = sizeof(pH->Size);

	unsigned char data[4];
	if ( ( ( pHeader->PackCRC >> 4 ) & 4 ) == 1) //< CYS 110126 >
	{
		//빌드패턴에 데이터 반씩 옮기는부분이있어서 추가
		int szi = (pHeader->MsgLen >> 1);
		if(szi < 2) 
		{
			for(int i = 0 ; i < size; i++)
			{
				data[i] = sizeheader[szi + i];	
			}
		}
	}
	else
	{
		for(int i = 0 ; i < size ; i++)
		{
			data[i] = sizeheader[i];
		}
	}

	for(int i = 0 ; i < size ; i++)
	{
		data[i] ^= g_GHashTable[(pHeader->PackCnt + 27) & 255];
		data[i] -= g_GHashTable[(pHeader->PackCnt + 51) & 255];
	}

	BuildPattern( (char*)data, pHeader->PackCRC, size );

	int Lowbyte = data[1] << 8 | data[0];
	int hibyte  = data[3] << 8 | data[2];

	*outputsize = (hibyte << 16 | Lowbyte) + PACKET_HEADERSIZE;
}

BOOL CCommMsg::CheckInfo(char *lpdata, int *psignal, int *pmsglen)
{
	PACKETHEADER *pH = (PACKETHEADER*)lpdata;
	if(*((short*)pH->MsgID) != *((short*)MSG_ID)) return FALSE;
	if(pH->Encode != 2) return FALSE;
	*psignal = (int)(pH->Signal-g_GHashTable[pH->PackCnt & 255]);
	*pmsglen = (int)pH->MsgLen;

	return TRUE;
}

BOOL CCommMsg::SetHeader(int signal)
{
	Destroy();

	// 메시지 사이즈 계산
	TotalSize = PACKET_HEADERSIZE;

#if defined(_ONLY_SERVER)
	// 가용 공간 확보(리턴 되는 버퍼 포인터는 매번 달라질 수 있음)
	pData = m_clBufUnit.Prepare(TotalSize);
#else		// defined(_ONLY_SERVER)
	// 정적 버퍼 할당(주의: No Thread safe)
	pBufUnit = s_MsgBuf.GetFreeBuf();
	if(!pBufUnit) return NULL;
	// 가용 공간 확보(리턴 되는 버퍼 포인터는 매번 달라질 수 있음)
	pData = pBufUnit->Prepare(TotalSize);
#endif	// defined(_ONLY_SERVER)
	PACKETHEADER* pHead = (PACKETHEADER*)pData;

	*((short*)pHead->MsgID) = *((short*)MSG_ID);
	pHead->Signal   = (short)signal;	// 메시지 시그널
	pHead->MsgLen   = 0;			// 처음에는 헤더만 생성되고 메시지 내용은 없다
	pHead->Encode   = 1;
	pHead->Compress = COMPRESSTYPE_NONE;	
	NowOffset = TotalSize;
	return TRUE;
}

BOOL CCommMsg::GetHeader(char *lpdata, int size)
{
	Destroy();

	if(!lpdata) return FALSE;

	pData = lpdata;
	PACKETHEADER* pHead = (PACKETHEADER*)lpdata;

	NowOffset = PACKET_HEADERSIZE;

	// 암호화 되어있다면 디코드함
	if(pHead->Encode == 2)
	{
		if(!DecodeAll(pData)) 
			return FALSE;
		if(pHead->Compress > COMPRESSTYPE_RESERVED)
		{
			size = DePressAll((BYTE**)&pData) + PACKET_HEADERSIZE; 
		}
	}
	TotalSize = size;
	return TRUE;
}

void CCommMsg::PushData(void** var,const void* src, int size)
{
	if(size <= 0 || !src) {
		*var = NULL;
		return;
	}

	TotalSize += size;

#if defined(_ONLY_SERVER)
	// 가용 공간 확보(리턴 되는 버퍼 포인터는 매번 달라질 수 있음)
	pData = m_clBufUnit.Prepare(TotalSize);
#else		// defined(_ONLY_SERVER)
	// 가용 공간 확보(리턴 되는 버퍼 포인터는 매번 달라질 수 있음)
	pData = pBufUnit->Prepare(TotalSize);
#endif	// defined(_ONLY_SERVER)

	PACKETHEADER* pHead = (PACKETHEADER*)pData;

	void* ptr = pData + NowOffset;
	memcpy(ptr, src, size);

	NowOffset += size;
	pHead->MsgLen += size;

	*var = ptr;
}

CString CCommMsg::GetCompressType()
{
	CString str;
	switch( CCommMsg::CompressReservedType )
	{
		case COMPRESSTYPE_NONE:		 str = "압축없음 상태";break;
		case COMPRESSTYPE_LZW_RESERVED:  str = "LZW 압축 상태";break;
		case COMPRESSTYPE_LZO_RESERVED:  str = "LZO 압축 상태";break;
		case COMPRESSTYPE_QLZO_RESERVED: str = "QLZO압축 상태";break;
		case COMPRESSTYPE_ZIP_RESERVED:  str = "ZIP 압축 상태";break;
		default:  str = "알 수 없음";break;
	}
	return str;
}

// [채널 통합]
void CCommMsg::TakeData(void** var, int size)
{
	if(size <= 0) {
		*var = NULL;
		return;
	}

	void* ptr = pData + NowOffset;

	NowOffset += size;

	*var = ptr;
}

void CCommMsg::PushVSTR(VSTRING& vstr, char* str)
{
//	int slen = strlen(str);
	int slen = (str) ? strlen(str) : 0;

	if(slen > 0) {
		// 스트링은 항상 널문자로 종결
		int bufsize = slen + 1;
		PushData(_PVAR(vstr.pbufsize), &bufsize, sizeof(*vstr.pbufsize));
		PushData(_PVAR(vstr.pstr), str, bufsize);
	}
	else {
		// 스트링은 항상 널문자로 종결(최소 버퍼 사이즈 = 1)
		int bufsize = 1;
		PushData(_PVAR(vstr.pbufsize), &bufsize, sizeof(*vstr.pbufsize));
		PushData(_PVAR(vstr.pstr), "\0", bufsize);
	}
}

void CCommMsg::TakeVSTR(VSTRING& vstr)
{
	TakeData(_PVAR(vstr.pbufsize), sizeof(*vstr.pbufsize));
	TakeData(_PVAR(vstr.pstr), *vstr.pbufsize);
}


