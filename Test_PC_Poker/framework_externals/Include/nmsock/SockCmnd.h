/**    
@file    SockCmnd.
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   ��Ʈ�� �޽����� Controller���� �����ش�.
*/
#pragma once

#include "SockDefine.h"
#include "SockMan.h"
#include "CommMessage.h"

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	���� Command Ŭ���� (CWnd ���)
		*/
		class NMSOCK_CLASS CSockCmnd : public CWnd
		{
		public:
			CSockCmnd();
			virtual ~CSockCmnd();

		public:
			BOOL Init(CWnd *parent, CSockMan *pSockMan);

		protected:

			CWnd *m_pParentWnd;			///< �޼����� ���� �θ�������
			CString m_strMainServerIP;	///< ���μ����� IP
			CSockMan *m_pSockMan;		///< ���� �Ŵ��� ������

		protected:	
			afx_msg LONG OnDataReceive(WPARAM, LONG);
			afx_msg LONG OnDataClose(WPARAM, LONG);
			afx_msg LONG OnDataConnect(WPARAM, LONG);
			afx_msg LONG OnDataConnectFailure(WPARAM, LONG);

			DECLARE_MESSAGE_MAP()
		};

	} //namespace SOCK

} // namespace NMBASE

