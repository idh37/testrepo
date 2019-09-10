#include "StdAfx.h"
#include "CommMessage.h"
#include "CompressUtil.h"

namespace NMBASE
{
	namespace SOCK
	{

#ifdef _DEBUG
		int CCommMessage::m_nCompressSizeLimit		= (int)0;
		int CCommMessage::m_nCompressReservedType	= COMPRESSTYPE_LZO_RESERVED;
#else
		int CCommMessage::m_nCompressSizeLimit		= (int)200;
		int CCommMessage::m_nCompressReservedType	= COMPRESSTYPE_NONE;
#endif

		void CCommMessage::SetCompressReservedType(int type)
		{
			CCommMessage::m_nCompressReservedType = type;
		}

		void CCommMessage::SetCompressSizeLimit(int size)
		{
			if(MAX_POOLSIZE >= size)
			{
				CCommMessage::m_nCompressSizeLimit = size;
			}
		}

		////////////////////////////////////////////////////////////////////////////
		//                              �޼��� �⺻ Ŭ����
		////////////////////////////////////////////////////////////////////////////
		// �޽��� ���� ���� Ŭ���� 
		static CMsgBuffer	s_cMsgBuffer;


		// ��ȣȭ/��ȣȭ�� ���̺�
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



		////////////////////////////////////////////////////////////////////////////
		//                              ���� Ŭ����
		////////////////////////////////////////////////////////////////////////////


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
			m_nSize = 0;
		}

		void CBufferUnit::Destroy()
		{
			if(m_pBuf) 
			{
				delete [] m_pBuf;
				m_pBuf = NULL;
			}

			Clear();
		}

		void CBufferUnit::Init(int size)
		{
			m_pBuf = new char[size];
			m_nSize = size;
		}

		char* CBufferUnit::Prepare(int size)
		{
			if(!m_pBuf) 
			{
				// �ּ� ���� ������� �䱸 �Ǵ� ������ �߿� ū ������� ���� �Ҵ�
				Init(max(SOCK_MINIMUM_MSGBUFFERSIZE, size));
				return m_pBuf;
			}

			// ���� ����� �۴ٸ� �ι��� ������ �̸� Ȯ���Ѵ�(���� �Ҵ��� ���� ����)
			if(m_nSize < size)
			{
				Resize(size * 2);
			}

			return m_pBuf;
		}

		void CBufferUnit::Resize(int size)
		{
			// ��������� ����� �ӽ� ����
			int nOriginalSize = m_nSize;

			char* pTemp = new char[m_nSize]; 
			memcpy(pTemp, m_pBuf, sizeof(char) * m_nSize);

			if(m_pBuf)
			{
				delete [] m_pBuf;
				m_pBuf = NULL;
			}

			m_pBuf = new char[size];
			m_nSize = size;

			if(pTemp) 
			{
				//memcpy(m_pBuf, pTemp, min(nOriginalSize, size));
				memcpy(m_pBuf, pTemp, sizeof(char) * nOriginalSize);

				delete [] pTemp;
				pTemp = NULL;
			} 
		}


		////////////////////////////////////////////////////////////////////////////
		//                              �޽��� ���� Ŭ����
		////////////////////////////////////////////////////////////////////////////


		CMsgBuffer::CMsgBuffer()
		{
			m_nLastIndex = -1;
		}

		CMsgBuffer::~CMsgBuffer()
		{
			for(int i=0; i<SOCK_MAX_MSGBUFFERNUM; ++i) 
			{
				m_cBuf[i].Destroy();
			}
		}

		CBufferUnit* CMsgBuffer::GetFreeBuf()
		{
			for(int i=0; i<SOCK_MAX_MSGBUFFERNUM; ++i) 
			{
				++m_nLastIndex;
				if(m_nLastIndex >= SOCK_MAX_MSGBUFFERNUM)
				{
					m_nLastIndex = 0;
				}

				if(!m_cBuf[m_nLastIndex].GetUse()) 
				{
					m_cBuf[m_nLastIndex].SetUse();
					return &m_cBuf[m_nLastIndex];
				}
			}

			ASSERT(0);
			return NULL;
		}



		////////////////////////////////////////////////////////////////////////////
		//                              �޽��� �⺻ Ŭ����
		////////////////////////////////////////////////////////////////////////////

		CCommMessage::CCommMessage()
		{
			ClearMember();
		}

		CCommMessage::~CCommMessage()
		{
			Destroy();
		}

		void CCommMessage::ClearMember()
		{
			m_pData=NULL;
			m_nTotalSize = 0;

			m_pBufUnit = NULL;
			m_nNowOffset = 0;
		}

		void CCommMessage::Destroy()
		{
			if(m_pBufUnit)
				m_pBufUnit->SetFree();

			ClearMember();
		}

		int CCommMessage::GetTotalSize()
		{
			if(m_pData==NULL)
				return 0;
			return m_nTotalSize;
		}

		// �Ʒ� 1 ���� 3���� �� �� ���� �ڵ��̴�. 
		// ���߿� ����غ��� �մϴ�.
		void CCommMessage::BuildPattern(char *pdata, UINT packcnt, int size)
		{
			int i;
			UINT nTemp = (packcnt>>4)&4;
			switch(nTemp) 
			{
			case 0:
				{
				} 
				break;

			case 1:
				{
					char temp;
					int szi = (size >> 1);
					if(szi < 2) 
						return;
					for(i = 0; i < szi;i++)
					{
						temp = pdata[i];
						pdata[i] = pdata[szi+i];
						pdata[szi+i] = temp;
					}
				} 
				break;

			case 2:
				{
					int szi;
					WORD tempWD;
					WORD *pWdata = (WORD *)pdata;
					szi = (size >> 1) -1;
					if(szi < 2) 
						return;

					for(i = 0; i < szi; i+=2) 
					{
						tempWD = pWdata[i];
						pWdata[i] = pWdata[i+1];
						pWdata[i+1] = tempWD;
					}
				} 
				break;

			case 3:
				{
					char temp;
					int szi = size-1;
					if(szi < 2)
						return;

					for(i = 0; i < szi;i++) 
					{
						temp = pdata[i];
						pdata[i] = pdata[i+1];
						pdata[i+1] = temp;
					}
				} 
				break;
			}
		}

		void CCommMessage::EncodeAll(char *pdata)
		{
			char *pUserData = (char *)(pdata + sizeof(SPACKETHEADER));
			SPACKETHEADER *phead = (SPACKETHEADER*)pdata;

			if(phead->Encode==2)
				return;

			phead->PackCRC = g_GHashTable[phead->PackCnt & 255] & 0x7F;

			for(UINT i=0; i<(UINT)phead->MsgLen; i++)
			{
				phead->PackCRC += (pUserData[i]&0x5F);
				pUserData[i] += g_GHashTable[(phead->PackCnt + 51) & 255];	
				pUserData[i] ^= g_GHashTable[(phead->PackCnt + 27) & 255];
			}
			phead->PackCRC += g_GHashTable[(31+phead->PackCnt) & 255] & 0x7F;
			phead->PackCRC += phead->Signal;
			phead->PackCRC = (phead->PackCRC & 0x0FFFFFFF);

			if(phead->PackCRC<0x0FFFF)
			{
				phead->PackCRC += ((g_GHashTable[phead->PackCnt & 255] & 0xF) << 24) +
					((g_GHashTable[(phead->PackCnt+1) & 255] & 0xF) << 20) +
					((g_GHashTable[(phead->PackCnt+3) & 255] & 0xF) << 16);
				phead->PackCRC = (phead->PackCRC << 4) + 0xF;
			}
			else if(phead->PackCRC<0x0FFFFF) 
			{
				phead->PackCRC += ((g_GHashTable[phead->PackCnt & 255] & 0xF) << 24) +
					((g_GHashTable[(phead->PackCnt+1) & 255] & 0xF) << 20);
				phead->PackCRC = (phead->PackCRC << 4) + (g_GHashTable[(phead->PackCnt+5) & 255] & 0x7);

			}
			else 
			{
				phead->PackCRC = (phead->PackCRC << 4) + 0x0A;
			}
			phead->Signal += g_GHashTable[phead->PackCnt & 255];

			phead->Encode = 2;
			BuildPattern(pUserData, phead->PackCRC, phead->MsgLen);
		}

		BOOL CCommMessage::DecodeAll(char *pdata)
		{
			int signal =0;
			char *pUserData = (char *)(pdata + sizeof(SPACKETHEADER));
			SPACKETHEADER *phead = (SPACKETHEADER*)pdata;
			if( phead->Encode != 2 ) 
				return FALSE;

			BuildPattern(pUserData, phead->PackCRC, phead->MsgLen);

			phead->Signal -= g_GHashTable[phead->PackCnt & 255];
			UINT tempCRC = g_GHashTable[phead->PackCnt & 255] & 0x7F;
			for(UINT i=0; i<(UINT)phead->MsgLen; i++)
			{
				pUserData[i] ^= g_GHashTable[(phead->PackCnt + 27) & 255];
				pUserData[i] -= g_GHashTable[(phead->PackCnt + 51) & 255];
				tempCRC += (pUserData[i]&0x5F);
			}
			tempCRC+=g_GHashTable[(31+phead->PackCnt) & 255] & 0x7F;


			tempCRC = tempCRC & 0x0FFFFFFF;

			int flag = phead->PackCRC & 0x0F;
			phead->PackCRC = phead->PackCRC >> 4;

			if(flag==0x0F)
			{
				phead->PackCRC &= 0x0FFFF;
			}
			else if(flag==(g_GHashTable[(phead->PackCnt+5) & 255] & 0x7))
			{
				phead->PackCRC &= 0x0FFFFF;
			}
			else if(flag!=0x0A)
			{
				return FALSE;
			}

			signal = phead->PackCRC - tempCRC;

			if(signal != phead->Signal)
			{
				return FALSE;
			}

			phead->Encode  = 1;

			if(phead->PackCRC != tempCRC+signal)
			{
				return FALSE;
			}

			return TRUE;
		}

		BOOL CCommMessage::IsCompress(int length)
		{
			if( length > CCommMessage::m_nCompressSizeLimit &&  CCommMessage::m_nCompressReservedType != COMPRESSTYPE_NONE )
			{
				return TRUE;
			}

			return FALSE;
		}

		int	CCommMessage::CompressAll(BYTE* pInput, BYTE* pOutput)
		{	
			SPACKETHEADER * phead = (SPACKETHEADER *)pOutput;

			int CompressType = phead->Compress;
			if(CompressType == COMPRESSTYPE_NONE)
				return phead->MsgLen;		// Safe Code

			//����ȵ� ������
			BYTE* Input_ptr =  (BYTE*)(pInput + sizeof(SPACKETHEADER));
			DWORD Input_size = phead->MsgLen;
			DWORD comp_size = MAX_POOLSIZE-1;
			//����
			UtilCompress(CompressType, pOutput + sizeof(SPACKETHEADER) + sizeof(COMPRESS_HEADER), comp_size, Input_ptr, Input_size );

			// ���࿹�࿡�� ������·� �÷��� ����
			phead->Compress *= 10;	

			//����Ȱ�� ����� �߰��Ѵ�.
			COMPRESS_HEADER compressheader;
			compressheader.Size = Input_size;
			memcpy( pOutput + sizeof(SPACKETHEADER) , &compressheader , sizeof(COMPRESS_HEADER) );
			return comp_size + sizeof(COMPRESS_HEADER);
		}

		int	CCommMessage::DePressAll(BYTE** pSrc)
		{
			SPACKETHEADER *phead = (SPACKETHEADER*)*pSrc;
			// ������ �ȵ� ������
			if(phead->Compress < COMPRESSTYPE_RESERVED )
				return phead->MsgLen;		// Safe Code

			SPACKETHEADER pTemp;
			memcpy(&pTemp, phead, sizeof(SPACKETHEADER));

			BYTE * m_pData= (BYTE*)(*pSrc +  sizeof(SPACKETHEADER) + sizeof(COMPRESS_HEADER) );

			BYTE* original_ptr = (BYTE*)m_pData;
			DWORD original_size = phead->MsgLen - sizeof(COMPRESS_HEADER);
			DWORD decompressed_size = MAX_POOLSIZE-1;

			// �̸� ������� ���۸� ���� ������ ����Ѵ�.
			CBufferUnit* p_InBuffer = s_cMsgBuffer.GetFreeBuf();
			if(!p_InBuffer) 
				return NULL;

			COMPRESS_HEADER * pcompressheader = (COMPRESS_HEADER*)(*pSrc + sizeof(SPACKETHEADER)); 
			BYTE* pOutputData = (BYTE*)p_InBuffer->Prepare(pcompressheader->Size + sizeof(SPACKETHEADER));

			//���� ����
			int decompresstype = phead->Compress;

			UtilDecompress(decompresstype, pOutputData, decompressed_size, original_ptr, original_size);
			// �÷��� ���¸� ���� �ȵ� ���·� �����Ѵ�.
			pTemp.Compress = COMPRESSTYPE_NONE;
			pTemp.MsgLen = decompressed_size;

			if(decompressed_size > (MAX_POOLSIZE-sizeof(SPACKETHEADER )))
			{
				//�Ͼ�� �ȵȴ�
				p_InBuffer->SetFree();
				return phead->MsgLen;		// Safe Code
			}

			BYTE *pRet = *pSrc;
			memcpy(pRet, &pTemp, sizeof(SPACKETHEADER));
			memcpy(pRet+sizeof(SPACKETHEADER), pOutputData, decompressed_size);
			p_InBuffer->SetFree();

			return decompressed_size;
		}

		void CCommMessage::CheckCompressSizeInfo(char *lpdata, int *outputsize)
		{
			SPACKETHEADER *pHeader = (SPACKETHEADER*)lpdata;
			if(pHeader->Compress < COMPRESSTYPE_RESERVED )
				return;

			COMPRESS_HEADER *pH = (COMPRESS_HEADER*)(lpdata + sizeof(SPACKETHEADER));
			//����� ��� ������� ������ ���� �ּҸ� �����´�
			unsigned char* sizeheader = (unsigned char*)&pH->Size;
			int size = sizeof(pH->Size);

			unsigned char data[4];
			if(((pHeader->PackCRC>>4)&4) == 1)
			{
				//�������Ͽ� ������ �ݾ� �ű�ºκ����־ �߰�
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

			*outputsize = (hibyte << 16 | Lowbyte) + SOCK_PACKET_HEADERSIZE;
		}

		BOOL CCommMessage::CheckInfo(char *lpdata, int *psignal, int *pmsglen)
		{
			SPACKETHEADER *pH = (SPACKETHEADER*)lpdata;

			if(*((short*)pH->MsgID) != *((short*)MSG_ID)) 
				return FALSE;
			if(pH->Encode != 2) 
				return FALSE;

			*psignal = (int)(pH->Signal-g_GHashTable[pH->PackCnt & 255]);
			*pmsglen = (int)pH->MsgLen;

			return TRUE;
		}

		BOOL CCommMessage::SetHeader(int signal)
		{
			Destroy();

			// �޽��� ������ ���
			m_nTotalSize = SOCK_PACKET_HEADERSIZE;

			// ���� ���� �Ҵ�(����: No Thread safe)
			m_pBufUnit = s_cMsgBuffer.GetFreeBuf();
			if(!m_pBufUnit) 
				return NULL;

			// ���� ���� Ȯ��(���� �Ǵ� ���� �����ʹ� �Ź� �޶��� �� ����)
			m_pData = m_pBufUnit->Prepare(m_nTotalSize);
			SPACKETHEADER* m_pHead = (SPACKETHEADER*)m_pData;

			*((short*)m_pHead->MsgID) = *((short*)MSG_ID);
			m_pHead->Signal   = (short)signal;	// �޽��� �ñ׳�
			m_pHead->MsgLen   = 0;			// ó������ ����� �����ǰ� �޽��� ������ ����
			m_pHead->Encode   = 1;
			m_pHead->Compress = COMPRESSTYPE_NONE;	
			m_nNowOffset	  = m_nTotalSize;

			return TRUE;
		}

		BOOL CCommMessage::GetHeader(char *lpdata, int size)
		{
			Destroy();

			if(!lpdata)
				return FALSE;

			m_pData = lpdata;
			SPACKETHEADER* m_pHead = (SPACKETHEADER*)lpdata;

			m_nNowOffset = SOCK_PACKET_HEADERSIZE;

			// ��ȣȭ �Ǿ��ִٸ� ���ڵ���
			if(m_pHead->Encode == 2)
			{
				if(!DecodeAll(m_pData)) 
					return FALSE;
				if(m_pHead->Compress > COMPRESSTYPE_RESERVED)
				{
					size = DePressAll((BYTE**)&m_pData) + SOCK_PACKET_HEADERSIZE; 
				}
			}
			m_nTotalSize = size;
			return TRUE;
		}

		void CCommMessage::PushData(void** var,const void* src, int size)
		{
			if(size <= 0 || !src)
			{
				*var = NULL;
				return;
			}

			m_nTotalSize += size;

			// ���� ���� Ȯ��(���� �Ǵ� ���� �����ʹ� �Ź� �޶��� �� ����)
			m_pData = m_pBufUnit->Prepare(m_nTotalSize);

			SPACKETHEADER* m_pHead = (SPACKETHEADER*)m_pData;

			void* ptr = m_pData + m_nNowOffset;
			memcpy(ptr, src, size);

			m_nNowOffset += size;
			m_pHead->MsgLen += size;

			*var = ptr;
		}

		LPCTSTR CCommMessage::GetCompressType()
		{
			static CString str;
			switch( CCommMessage::m_nCompressReservedType )
			{
			case COMPRESSTYPE_NONE:			 str = "������� ����";break;
			case COMPRESSTYPE_LZW_RESERVED:  str = "LZW ���� ����";break;
			case COMPRESSTYPE_LZO_RESERVED:  str = "LZO ���� ����";break;
			case COMPRESSTYPE_QLZO_RESERVED: str = "QLZO���� ����";break;
			case COMPRESSTYPE_ZIP_RESERVED:  str = "ZIP ���� ����";break;
			default:						 str = "�� �� ����";break;
			}
			return (LPCTSTR)str;
		}

		// [ä�� ����]
		void CCommMessage::TakeData(void** var, int size)
		{
			if(size <= 0) 
			{
				*var = NULL;
				return;
			}

			void* ptr = m_pData + m_nNowOffset;

			m_nNowOffset += size;

			*var = ptr;
		}

		void CCommMessage::PushVSTR(SVSTRING& vstr, char* str)
		{
			//	int slen = strlen(str);
			int slen = (str) ? strlen(str) : 0;

			if(slen > 0) 
			{
				// ��Ʈ���� �׻� �ι��ڷ� ����
				int bufsize = slen + 1;
				PushData(_PVAR(vstr.pbufsize), &bufsize, sizeof(*vstr.pbufsize));
				PushData(_PVAR(vstr.pstr), str, bufsize);
			}
			else 
			{
				// ��Ʈ���� �׻� �ι��ڷ� ����(�ּ� ���� ������ = 1)
				int bufsize = 1;
				PushData(_PVAR(vstr.pbufsize), &bufsize, sizeof(*vstr.pbufsize));
				PushData(_PVAR(vstr.pstr), "\0", bufsize);
			}
		}

		void CCommMessage::TakeVSTR(SVSTRING& vstr)
		{
			TakeData(_PVAR(vstr.pbufsize), sizeof(*vstr.pbufsize));
			TakeData(_PVAR(vstr.pstr), *vstr.pbufsize);
		}

	}	//namespace SOCK

} //namespace NMBASE
