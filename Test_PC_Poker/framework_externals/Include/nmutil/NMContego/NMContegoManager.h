#pragma once

#include "NMContegoCtrl.h"
#include "VirtualizerSDK.h"
#include "UtilDefine.h"


/*

���ȸ�� ����� : ������ �̽���

1. �����ڵ�

define string                    code         ����
--------------------------------------------------------------------------------------------
NMCTG_EXT_ERROR_FILE_NOT_EXIST  0xA0000001    - ���ȸ�� ������ ����
NMCTG_EXT_ERROR_LOAD_FAIL       0xA0000002    - ��� �ε� ����
NMCTG_EXT_ERROR_INVALID_EXPORT  0xA0000003    - ���ȸ���� EXPORT �Լ� ��� ����
NMCTG_EXT_ERROR_NOT_SURPPORT    0xA0000004    - �������� �ʴ� OS

NMCTG_ERROR_INVALID_PARAM       0xE0000001    - ��ȿ���� ���� �Ķ���� ���޵�

NMCTG_ERROR_APIHOOK             0xE0000002    - API HOOK ����
NMCTG_ERROR_APIUNHOOK           0xE0000003    - API UNHOOK ����
NMCTG_ERROR_APIHOOKDLL_LOAD     0xE0000004    - API HOOK DLL �ε� ����
NMCTG_ERROR_NOT_SURPPORT        0xE0000005    - �������� �ʴ� OS���� ȣ���



2. �����ڵ�

define string                     code          ����
--------------------------------------------------------------------------------------------
NMCTG_FLTDRV_DETECT_AUTOKEY       0xD0000001     - �������� ���α׷����κ����� Ű���� �̺�Ʈ ����
NMCTG_FLTDRV_DETECT_AUTOMOUSE     0xD0000002     - �������� ���α׷����κ����� ���콺 �̺�Ʈ ����

NMCTG_APIHOOK_DETECT_BITBLT       0xD0000010     - ȭ��ĸ�� �õ���(BITBLT API ���)
NMCTG_APIHOOK_DETECT_STRETCHBLT   0xD0000020     - ȭ��ĸ�� �õ���(STRETCHBLT API ���)
NMCTG_APIHOOK_DETECT_SETCURSOR    0xD0000030     - �������� ���α׷����κ��� ���콺 ������ ���� �õ���

NMCTG_SDTHOOK_DETECT_SENDINPUT    0xD0010000     - �������� ���α׷����κ��� Ű����/���콺 ���� �õ���



3. �׽�Ʈ ���̽��� �����ڵ� �߻�

- ������ ȭ��ĸ�� ���α׷��� ����Ͽ� ĸ�� ����� �����ϸ� ���еǸ鼭
	0xD0000010 �Ǵ�0xD0000020 �� �����ڵ尡 �߻��ؾ���

- �������� ���α׷�(vnc ��)�� ����ϸ� ������� ���еǰ�
	0xD0000010, 0xD0010000 �� �����ڵ尡 �߻��Ͽ�����

	(�������� ������� �����Ǹ� �����ڵ� �߻�����)

- �������� ���ݵ���ũž���� ���������Ͽ� �������α׷�������
	Ű����, ���콺�� ����� ��� 0xD0000001 �� 0xD0000002 �� �����ڵ� �߻�

- ��� ������ �����ڵ�� ���ӽ��� �� �ڵ庰 1���� �߻�
	(���� ���� �� ��ε� �� �����ڵ尡 ���µǾ� �ٽ� ������ 1ȸ ���� �߻�)
/**/

#define NMCONTEGOMANAGER_NOUSER

class NMUTIL_CLASS CNMContegoManager
{
public:
	CNMContegoManager(void);
	virtual ~CNMContegoManager(void);

	static CNMContegoManager& GetInstance();

	bool NMCtgInitialize( int game_code, char* szProcess, char* szTitle );
		bool CheckRandomLoad( DWORD dwPercent );
		bool InitHostInfo();
		bool InitLibrary();
		bool BindLibraryFunction();

	bool CtgModuleInitialize();
	bool ProtectAutoKey();
	bool ProtectAutoMouse();
	bool ProtectInput();
	bool ProtectScreenCaptureGlobal();
	bool ProtectScreenCaptureLocal();
	bool UnProtectScreenCaptureLocal();
	bool ProtectMemory();
	bool HideProcess( TCHAR* pProcessName, TCHAR* pWindowTitle );
	
	static void CALLBACK NMCtgInputNotifyProc( NMCTG_INPUT_TYPE eInputType, ULONG lValue );
	static void CALLBACK NMCtgDetectNotifyProc( ULONG lDetectCode, LPCTSTR lpszImageName );
	void NMCtgRelease();

	static void ThrowError( DWORD dwErrCode, DWORD dwDetectCode = 0, LPCTSTR lpszImageName = NULL );

protected:
	CString									m_strIP;
	CString									m_strDomain;

	HMODULE									m_hNMCtrlModule;
	NMCTG_INITIALIZE						m_pfNMCtgInitialize;
	NMCTG_PROTECT_AUTOKEY					m_pfProtectAutoKey;
	NMCTG_RELEASE_AUTOKEY					m_pfReleaseAutoKey;
	NMCTG_PROTECT_AUTOMOUSE					m_pfProtectAutoMouse;
	NMCTG_RELEASE_AUTOMOUSE					m_pfReleaseAutoMouse;
	NMCTG_PROTECT_SCRCPT_GLOBAL				m_pfProtectScrCptGlobal;
	NMCTG_PROTECT_SCRCPT_LOCAL				m_pfProtectScrCptLocal;
	NMCTG_PROTECT_MEMORY					m_pfProtectMemory;
	NMCTG_HIDE_PROCESS						m_pfHideProcess;
	NMCTG_PROTECT_INPUT						m_pfProtectInput;
	NMCTG_SET_DETECT_CALLBACK				m_pfSetDetectCallBack;
	NMCTG_RELEASE							m_pfNMCtgRelease;
};



NMUTIL_API CNMContegoManager &gNMContegoMan();