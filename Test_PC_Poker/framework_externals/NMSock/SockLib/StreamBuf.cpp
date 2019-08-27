#include "stdafx.h"
#include "StreamBuf.h"

namespace NMBASE
{
	namespace SOCK
	{
		CStreamBuf::CStreamBuf()
		{
			m_pBuf = NULL;
			m_nBufSize = m_nUsedSize = 0;
		}

		CStreamBuf::~CStreamBuf()
		{
			Destroy();
		}

		void CStreamBuf::Destroy()
		{
			if(m_pBuf) 
			{
				delete [] m_pBuf;
				m_pBuf = NULL;
			}

			m_nBufSize = m_nUsedSize = 0;
		}

		BOOL CStreamBuf::Init(int size)
		{
			if(size==0 || size<0) 
			{
				return FALSE;
			}

			if(m_pBuf != NULL)
			{
				delete [] m_pBuf;
				m_pBuf = NULL;
			}

			m_pBuf = new char[size];

			if(m_pBuf == NULL)
			{
				m_nBufSize = m_nUsedSize = 0;
				return FALSE;
			}
			m_nBufSize = size;
			m_nUsedSize = 0;
			return TRUE;
		}



		BOOL CStreamBuf::AddUsedSize(int size)
		{
			if((m_nUsedSize + size) > m_nBufSize) 
			{
				return FALSE;
			}
			else
			{
				m_nUsedSize += size;		
			}

			return TRUE;
		}

		BOOL CStreamBuf::DelData(int size)
		{
			if( m_nUsedSize < size || size < 0) 
			{
				return FALSE;
			}
			else
			{
				if(0 == size)
				{
					return TRUE;
				}

				//if(IsBadReadPtr(m_pBuf, 4)) return FALSE;

				memcpy(m_pBuf, m_pBuf+size, m_nUsedSize-size);
				m_nUsedSize -= size;		
			}

			return TRUE;
		}

		int CStreamBuf::AddData(void* m_pData, int size)
		{
			if(m_pData == NULL) 
				return 0;

			if(size <= 0 || size > (m_nBufSize - m_nUsedSize))
				return 0;

			memcpy((m_pBuf + m_nUsedSize), m_pData, size);
			m_nUsedSize += size;

			return size;
		}

		int CStreamBuf::AddData_Encode(void* m_pData, int size)
		{
			if(m_pData == NULL)
				return 0;

			if(size <= 0 || size > (m_nBufSize - m_nUsedSize))
				return 0;

			// 암호화 기록	
			for(int i = 0; i < size; ++i) 
			{
				m_pBuf[m_nUsedSize+i] = ((char*)m_pData)[i]^'J'^'H';
			}

			m_nUsedSize += size;

			return size;
		}

		int CStreamBuf::Data_Decode(int start, int size)
		{
			if(m_pBuf == NULL) 
				return 0;

			if(size<=0 || start<0 || start>=m_nBufSize) 
				return 0;

			if(start+size<=0 || start+size>m_nBufSize) 
				return 0;

			// 암호화 디코딩
			for(int i=start; i<start+size; i++)
			{
				m_pBuf[i] = m_pBuf[i]^'J'^'H';
			}

			return size;
		}

		int CStreamBuf::GetData(void *pTarget, int tarsize)
		{
			if(m_nUsedSize==0) 
				return 0;

			if(tarsize < m_nUsedSize)
				return 0;

			memcpy(pTarget, m_pBuf, m_nUsedSize);
			int rtn = m_nUsedSize;
			m_nUsedSize = 0;

			return rtn;
		}


		int CStreamBuf::PeekData(void *pTarget, int peeksize, int tarsize)
		{
			if(tarsize < m_nUsedSize) 
				return 0;

			if(m_nUsedSize==0 || m_nUsedSize<peeksize)
				return 0;

			memcpy(pTarget, m_pBuf, peeksize);

			return 0;
		}

	} //namespace SOCK

} // namespace NMBASE
