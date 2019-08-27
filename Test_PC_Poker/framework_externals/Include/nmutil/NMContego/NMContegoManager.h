#pragma once

#include "NMContegoCtrl.h"
#include "VirtualizerSDK.h"
#include "UtilDefine.h"


/*

보안모듈 담당자 : 보안팀 이승희

1. 에러코드

define string                    code         설명
--------------------------------------------------------------------------------------------
NMCTG_EXT_ERROR_FILE_NOT_EXIST  0xA0000001    - 보안모듈 파일이 없음
NMCTG_EXT_ERROR_LOAD_FAIL       0xA0000002    - 모듈 로드 실패
NMCTG_EXT_ERROR_INVALID_EXPORT  0xA0000003    - 보안모듈의 EXPORT 함수 얻기 실패
NMCTG_EXT_ERROR_NOT_SURPPORT    0xA0000004    - 지원되지 않는 OS

NMCTG_ERROR_INVALID_PARAM       0xE0000001    - 유효하지 않은 파라메터 전달됨

NMCTG_ERROR_APIHOOK             0xE0000002    - API HOOK 실패
NMCTG_ERROR_APIUNHOOK           0xE0000003    - API UNHOOK 실패
NMCTG_ERROR_APIHOOKDLL_LOAD     0xE0000004    - API HOOK DLL 로드 실패
NMCTG_ERROR_NOT_SURPPORT        0xE0000005    - 지원되지 않는 OS에서 호출됨



2. 감지코드

define string                     code          설명
--------------------------------------------------------------------------------------------
NMCTG_FLTDRV_DETECT_AUTOKEY       0xD0000001     - 원격제어 프로그램으로부터의 키보드 이벤트 감지
NMCTG_FLTDRV_DETECT_AUTOMOUSE     0xD0000002     - 원격제어 프로그램으로부터의 마우스 이벤트 감지

NMCTG_APIHOOK_DETECT_BITBLT       0xD0000010     - 화면캡쳐 시도됨(BITBLT API 사용)
NMCTG_APIHOOK_DETECT_STRETCHBLT   0xD0000020     - 화면캡쳐 시도됨(STRETCHBLT API 사용)
NMCTG_APIHOOK_DETECT_SETCURSOR    0xD0000030     - 원격제어 프로그램으로부터 마우스 움직임 제어 시도됨

NMCTG_SDTHOOK_DETECT_SENDINPUT    0xD0010000     - 원격제어 프로그램으로부터 키보드/마우스 제어 시도됨



3. 테스트 케이스별 감지코드 발생

- 게임중 화면캡쳐 프로그램을 사용하여 캡쳐 기능을 실행하면 실패되면서
	0xD0000010 또는0xD0000020 의 감지코드가 발생해야함

- 원격제어 프로그램(vnc 등)을 사용하면 원격제어가 실패되고
	0xD0000010, 0xD0010000 의 감지코드가 발생하여야함

	(툴에따라 원격제어가 성공되면 감지코드 발생안함)

- 윈도우의 원격데스크탑으로 원격제어하여 게임프로그램내에서
	키보드, 마우스를 사용할 경우 0xD0000001 와 0xD0000002 의 감지코드 발생

- 모든 종류의 감지코드는 게임실행 중 코드별 1번만 발생
	(게임 종류 후 재로딩 시 감지코드가 리셋되어 다시 종류별 1회 씩만 발생)
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