/**    
@file    CommMsg.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   ���� �޽���
*/

#pragma once

#include "SockDefine.h"

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	Buffer Unit(���� �ּҴ���)
		*/
		class NMSOCK_CLASS CBufferUnit
		{
		public:
			CBufferUnit();
			~CBufferUnit();

		public:
			void Clear();
			void Destroy();

			void Init(int size);
			char* Prepare(int size);	// ���۳��� �����͸� �ı���Ű�� �ʰ� ���� ����� �ø���

			void SetUse()  {m_bUse = TRUE;}
			BOOL GetUse()  {return m_bUse;}
			void SetFree() {m_bUse = FALSE;}

			char* GetBuffer(){return m_pBuf;}

		private:
			void Resize(int size);

		protected:
			BOOL	m_bUse;
			char*	m_pBuf;
			int		m_nSize;
		};

		/**      
		@brief	�޽��� ����
		*/
		class NMSOCK_CLASS CMsgBuffer  
		{
		public:
			CMsgBuffer();
			virtual ~CMsgBuffer();

		public:
			CBufferUnit* GetFreeBuf();

		protected:
			int			m_nLastIndex;
			CBufferUnit m_cBuf[SOCK_MAX_MSGBUFFERNUM];
		};



		/**      
		@brief	��� �޼��� �⺻ Ŭ���� 
		*/
		class NMSOCK_CLASS CCommMessage
		{
		public:
			/**      
			@brief	���� ���� ��Ʈ��
			*/
			typedef struct _tagSVSTRING
			{
				short* pbufsize;
				char*  pstr;
			}SVSTRING;

		public:
			CCommMessage();						
			~CCommMessage();

		public:
			void ClearMember();
			void Destroy();
			int  GetTotalSize();

			static void BuildPattern(char *pdata,UINT packcnt,int size);
			static void EncodeAll(char *pdata);
			static BOOL DecodeAll(char *pdata);

			static int  CompressAll( BYTE* m_pData, BYTE* pbuf );
			static int	DePressAll( BYTE** pSrc );
			static void SetCompressSizeLimit( int size );
			static BOOL IsCompress( int length );
			static void SetCompressReservedType(int type );
			static void CheckCompressSizeInfo(char *lpdata, int *outputsize);
			static LPCTSTR GetCompressType();

			///< �޸� ������ �����ͷκ��� ��� �޽����� �м��Ͽ� �ʿ��� ������ �˷���
			///< (lpdata�� �ּ��� ����� �����Ǵµ� �ʿ��� �⺻������ �̻� �Ǿ���Ѵ�)
			static BOOL CheckInfo(char *lpdata, int *psignal, int *pmsglen);


		protected:
			///< ����� ������ְ� �ʿ��� �����ŭ �޸𸮸� �Ҵ��Ѵ�
			BOOL SetHeader(int signal);

			///< ���� �����͸� �̿��Ͽ� ����� �����Ѵ�
			///< ����� ������ �������� ù��° �����Ϳ� �������� ��ü ����� �޴´�
			BOOL GetHeader(char* lpdata, int size);

			void PushData(void** var, const void* src, int size);
			void TakeData(void** var, int size);
			void PushVSTR(SVSTRING& vstr, char* str);
			void TakeVSTR(SVSTRING& vstr);


		protected:
			int				m_nTotalSize;				///< ����� ������ �������� �� ������
			char*			m_pData;					///< ������ ������(��� ����)
			SPACKETHEADER*	m_pHead;					///< ��Ŷ ��� ������

			CBufferUnit*	m_pBufUnit;					///< �޽��� ������ ���� Ŭ���� ������
			int				m_nNowOffset;				///< �޸� �������� ���� ������ ��ġ

			static int		m_nCompressReservedType;
			static int		m_nCompressSizeLimit;
		};

	} //namespace SOCK

} //namespace NMBASE

