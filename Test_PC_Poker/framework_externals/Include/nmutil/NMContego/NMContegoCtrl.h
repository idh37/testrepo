/***********************************************************************************************
 ⓒ Copyright 2011. CJ E&M, Inc

 IMPLEMENTATION FILE : NMContegoCtrl.h

 DATE			: 2011. 06. 15
 CURRENT DATE	: 2012. 01. 31
 AUTHOR			: 보안팀

 DESCRIPTION	: 
					- export funtion은 ordinal number로 사용

					- 키보드/마우스 외부 입력 감지
					- 키보드/마우스 SW 입력 차단
					- 사용 프로세스 메모리 보호
					- 사용 프로세스 프로세스 은폐
					- 화면캡쳐 보호

					
HISTORY			:
	- 2012. 01. 31  : NMCTG_ISWINDOWS_SUPPORT_VER을 삭제하고 NMCTG_INITIALIZE 추가
					  NMCTG_ISWINDOWS_SUPPORT_VER 기능을 NMCTG_INITIALIZE 함수에 포함



************************************************************************************************/

#ifndef _INCONTEGO_CTRL_H_
#define _INCONTEGO_CTRL_H_

#include <windows.h>


// 에러코드 combination
// 
// 0x00(kernel error code) + 00(user error code) + 0000(windows last error code)

// 코드 PREFIX
//	
//	0xA0000000 - 모듈사용시 외부적인 요소의 에러코드(NMContegoExtErrorCodes.h)
//  0xD0000000 - 보호하고 있는 기능을 사용하려할 때 감지되는 코드
//



typedef enum _tagInputType{
	NMCTG_KEYBOARD = 1,
	NMCTG_MOUSE
} NMCTG_INPUT_TYPE;


//
// CALLBACK FUNCTIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 


/* 
 * Keyboad, Mouse에서 비정상 입력이 감지되면 호출되는 CallBack
 *
 *  Parameters
 *		- eInputType[in]	:	input type(keyboard/mouse)
 *		- lDetectCode		:	detect internal code
 *		- lValue[in]		:	depend on eInputType values
 *
 *							NMCTG_KEYBOARD	- keyboard virtual key value
 *							NMCTG_MOUSE		- IC_LBUTTON_DOWN ~ IC_MBUTTON_UP
 *
 */
typedef VOID (CALLBACK *PF_INPUT_NOTIFY_CALLBACK)(
  NMCTG_INPUT_TYPE  eInputType,  
  ULONG				lValue
);




/* 
 * 화면캡쳐 방지, SW 키보드/마우스 입력 방지를 On 하였을때 외부 프로세스에서 접근시도 시 통보 받을 CallBack
 * pImageName는 NULL check가 필요
 *
 *  NMCTG_PROTECT_AUTOKEY, NMCTG_PROTECT_AUTOMOUSE를 호출 하였을때 pImageName값이 NULL인 감지코드가 통보됨
 *
 *  Parameters
 *		- lDetectCode[in]	:	internal detect code
 *		- pImageName		:	called image name 
 *								 + kernel hook		: process name
 *								 + user hook		: process full path
 *								 + filter driver	: NULL
 *								 
 */
typedef VOID (CALLBACK *PF_DETECT_NOTIFY_CALLBACK)(  
  ULONG		lDetectCode,
  LPCTSTR	lpszImageName		// must check NULL value
);





//
// EXPORT FUNCTIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 

/*
 * 모듈 초기화
 *	- Check Supported Windows Only 32bit Windows and WinXP or Later )
 *  - Check duplicated loading
 *  - initialize data
 *
 *	Return
 *		Success	: ERROR_SUCCESS
 *		Fail	: Error Code
 */
typedef LONG (_stdcall *NMCTG_INITIALIZE)( 	
	);


/*
 * Protect Auto Keyboard - 외부 마우스 입력 감지
 *
 *  Parameters
 *		pfNotifyCallBack [in]	: 비정상적인 이벤트 발생시 통보 받을 콜백 함수
 *		pdwError[out]			: Window Last Error Value  
 *
 *	Return
 *		Success	: ERROR_SUCCESS
 *		Fail	: Error Code
 */
typedef LONG (_stdcall *NMCTG_PROTECT_AUTOKEY)( 
	IN	PF_INPUT_NOTIFY_CALLBACK	pfNotifyCallBack
	);


/*
 * Release Protect Auto Keyboard - 외부키 입력 감지 중지
 */
typedef VOID (_stdcall *NMCTG_RELEASE_AUTOKEY)( 
	);


/*
 * Protect Auto Mouse - 외부 키보드 입력 감지
 *
 *  Parameters
 *		pfNotifyCallBack [in]	: 비정상적인 이벤트 발생시 통보 받을 콜백 함수
 *		pdwError[out]			: Window Last Error Value  
 *
 *	Return
 *		Success	: ERROR_SUCCESS
 *		Fail	: Error Code
 */
typedef LONG (_stdcall *NMCTG_PROTECT_AUTOMOUSE)( 
	IN	PF_INPUT_NOTIFY_CALLBACK	pfNotifyCallBack
	);


/*
 * Release Protect Auto Keyboard - 외부키 입력 감지 중지
 */
 typedef VOID (_stdcall *NMCTG_RELEASE_AUTOMOUSE)(
	);


/*
 * Protect Screen Capture(with current process)
 *
 *  Parameters				Value		Meaning
 *		bProtect [in]	:	-----------------------
 *							TRUE		Protect On
 *							FALSE		Protect Off
 *
 *	Return
 *		Success	: ERROR_SUCCESS
 *		Fail	: Error Code
 */
typedef LONG (_stdcall *NMCTG_PROTECT_SCRCPT_GLOBAL)(
	BOOL bProtect 
	);



/*
 * Protect Screen Capture in current process only
 *
 *  Parameters				Value		Meaning
 *		bProtect [in]	:	-----------------------
 *							TRUE		Protect On
 *							FALSE		Protect Off
 *
 *	Return
 *		Success	: ERROR_SUCCESS
 *		Fail	: Error Code
 */
typedef LONG (_stdcall *NMCTG_PROTECT_SCRCPT_LOCAL)(
	BOOL bProtect 
	);


/*
 * Protect Process Memory
 *
 *	- NMCTG_PROTECT_SCREENCAPTURE와 동일
 */
typedef LONG (_stdcall *NMCTG_PROTECT_MEMORY)(
	BOOL bProtect 
	);

/*
 * Hide Process
 *	
 *  Parameters				Value		Meaning
 *							-----------------------
 *		bHide [in]	:		TRUE		Hide On
 *							FALSE		Hide Off
 *
 *		pProcessName[in]
 *			bHide is TRUE	->Process name to hide(대소문자 구분)
 *			bHide is FALSE	-> NULL
 *
 *		pWindowTitle[in]
 *			bHide is TRUE	-> Window title string to hide(대소문자 구분)
 *			bHide is FALSE	-> NULL
 *
 *	Return
 *		Success	: ERROR_SUCCESS
 *		Fail	: Error Code
 */
typedef LONG (_stdcall *NMCTG_HIDE_PROCESS)(
	BOOL bHide,
	const char* pProcessName, 
	const char* pWindowTitle
	);


/*
 * Protect Input Keyboard/Mouse by SDT Hooking
 *
 *	- NMCTG_PROTECT_SCREENCAPTURE와 동일
 */
typedef LONG (_stdcall *NMCTG_PROTECT_INPUT)(
	BOOL bProtect 
	);


/*
 * Get detect code 
 *
 *  Parameters
 *		pfNotifyCallBack [in]	: 화면캡쳐 방지, SW 키보드/마우스 입력 방지를 On 하였을때 외부 프로세스에서 접근시도 시 통보 받을 콜백 함수
 */
typedef VOID (_stdcall *NMCTG_SET_DETECT_CALLBACK)(
	IN	PF_DETECT_NOTIFY_CALLBACK	pfNotifyCallBack
	);

/*
 * Release all
 */
typedef VOID (_stdcall *NMCTG_RELEASE)(
	);

#endif // __DRVINSTALL_H__
