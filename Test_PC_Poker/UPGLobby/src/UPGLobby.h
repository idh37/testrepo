// UPGLobby.h : UPGLobby DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CUPGLobbyApp
// �� Ŭ������ ������ ������ UPGLobby.cpp�� �����Ͻʽÿ�.
//

class CUPGLobbyApp : public CWinApp
{
public:
	CUPGLobbyApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
