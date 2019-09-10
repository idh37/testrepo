/**    
@file    StreamBuf.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   ��Ʈ�� ���� ���� Ŭ���� ���
*/

#pragma once

#include "SockDefine.h"

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	��Ʈ�� ���� Ŭ����
		*/
		class NMSOCK_CLASS CStreamBuf
		{
		public:
			CStreamBuf();
			~CStreamBuf();

		public:

			BOOL Init(int size);
			void Destroy();

			int GetBufSize() {return m_nBufSize;}
			int GetUsedSize() {return m_nUsedSize;}
			int GetFreeSize() {return (m_nBufSize - m_nUsedSize);}

			char* GetStartPointer() {return m_pBuf;}
			char* GetEndPointer() {return (m_pBuf+m_nUsedSize);}

			BOOL AddUsedSize(int size);						///< ���� ����� ������Ų��(�ܼ��� ī���͸� ������Ŵ)
			BOOL DelData(int size);							///< ������ ù�Ӹ����� ������ ����������� ������ �����ϰ�
			///<  �ٽ� ������ ó������ �� �� �ֵ��� �����Ѵ�.

			int AddData(void* m_pData, int size);			///< ���ۿ� �����͸� ����
			int AddData_Encode(void* m_pData, int size);	///< ���ۿ� �����͸� ����(��ȣȭ)
			int Data_Decode(int start, int size);			///< ���ۿ� �����͸� ����� ��ȣȭ �����͸� ���ڵ�
			int GetData(void *pTarget, int tarsize);		///< ���ۿ��� �����͸� �о������ ������ ����
			int PeekData(void *pTarget, int peeksize, int tarsize);	///< �����ʹ� �е� ������ �������� ����

		protected:
			char *m_pBuf;
			int m_nBufSize;
			int m_nUsedSize;
		};

	} //namespace SOCK

} // namespace NMBASE
