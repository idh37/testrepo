/**    
@file    CommMsg.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   공통 메시지
*/

#pragma once

#include "SockDefine.h"

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	Buffer Unit(버퍼 최소단위)
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
			char* Prepare(int size);	// 버퍼내의 데이터를 파괴시키지 않고 버퍼 사이즈를 늘린다

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
		@brief	메시지 버퍼
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
		@brief	통신 메세지 기본 클래스 
		*/
		class NMSOCK_CLASS CCommMessage
		{
		public:
			/**      
			@brief	가변 길이 스트링
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

			///< 메모리 버퍼의 데이터로부터 헤더 메시지를 분석하여 필요한 정보를 알려줌
			///< (lpdata는 최소한 헤더가 생성되는데 필요한 기본사이즈 이상 되어야한다)
			static BOOL CheckInfo(char *lpdata, int *psignal, int *pmsglen);


		protected:
			///< 헤더를 만들어주고 필요한 사이즈만큼 메모리를 할당한다
			BOOL SetHeader(int signal);

			///< 받은 데이터를 이용하여 헤더를 세팅한다
			///< 헤더를 포함한 데이터의 첫번째 포인터와 데이터의 전체 사이즈를 받는다
			BOOL GetHeader(char* lpdata, int size);

			void PushData(void** var, const void* src, int size);
			void TakeData(void** var, int size);
			void PushVSTR(SVSTRING& vstr, char* str);
			void TakeVSTR(SVSTRING& vstr);


		protected:
			int				m_nTotalSize;				///< 헤더를 포함한 데이터의 총 사이즈
			char*			m_pData;					///< 데이터 포인터(헤더 포함)
			SPACKETHEADER*	m_pHead;					///< 패킷 헤더 포인터

			CBufferUnit*	m_pBufUnit;					///< 메시지 생성용 버퍼 클래스 포인터
			int				m_nNowOffset;				///< 메모리 복사중인 최종 오프셋 위치

			static int		m_nCompressReservedType;
			static int		m_nCompressSizeLimit;
		};

	} //namespace SOCK

} //namespace NMBASE

