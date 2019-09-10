/***********************************************************************************************
 �� Copyright 2011. CJ E&M, Inc

 IMPLEMENTATION FILE : NMContegoCtrl.h

 DATE			: 2011. 06. 15
 CURRENT DATE	: 2012. 01. 31
 AUTHOR			: ������

 DESCRIPTION	: 
					- export funtion�� ordinal number�� ���

					- Ű����/���콺 �ܺ� �Է� ����
					- Ű����/���콺 SW �Է� ����
					- ��� ���μ��� �޸� ��ȣ
					- ��� ���μ��� ���μ��� ����
					- ȭ��ĸ�� ��ȣ

					
HISTORY			:
	- 2012. 01. 31  : NMCTG_ISWINDOWS_SUPPORT_VER�� �����ϰ� NMCTG_INITIALIZE �߰�
					  NMCTG_ISWINDOWS_SUPPORT_VER ����� NMCTG_INITIALIZE �Լ��� ����



************************************************************************************************/

#ifndef _INCONTEGO_CTRL_H_
#define _INCONTEGO_CTRL_H_

#include <windows.h>


// �����ڵ� combination
// 
// 0x00(kernel error code) + 00(user error code) + 0000(windows last error code)

// �ڵ� PREFIX
//	
//	0xA0000000 - ������ �ܺ����� ����� �����ڵ�(NMContegoExtErrorCodes.h)
//  0xD0000000 - ��ȣ�ϰ� �ִ� ����� ����Ϸ��� �� �����Ǵ� �ڵ�
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
 * Keyboad, Mouse���� ������ �Է��� �����Ǹ� ȣ��Ǵ� CallBack
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
 * ȭ��ĸ�� ����, SW Ű����/���콺 �Է� ������ On �Ͽ����� �ܺ� ���μ������� ���ٽõ� �� �뺸 ���� CallBack
 * pImageName�� NULL check�� �ʿ�
 *
 *  NMCTG_PROTECT_AUTOKEY, NMCTG_PROTECT_AUTOMOUSE�� ȣ�� �Ͽ����� pImageName���� NULL�� �����ڵ尡 �뺸��
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
 * ��� �ʱ�ȭ
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
 * Protect Auto Keyboard - �ܺ� ���콺 �Է� ����
 *
 *  Parameters
 *		pfNotifyCallBack [in]	: ���������� �̺�Ʈ �߻��� �뺸 ���� �ݹ� �Լ�
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
 * Release Protect Auto Keyboard - �ܺ�Ű �Է� ���� ����
 */
typedef VOID (_stdcall *NMCTG_RELEASE_AUTOKEY)( 
	);


/*
 * Protect Auto Mouse - �ܺ� Ű���� �Է� ����
 *
 *  Parameters
 *		pfNotifyCallBack [in]	: ���������� �̺�Ʈ �߻��� �뺸 ���� �ݹ� �Լ�
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
 * Release Protect Auto Keyboard - �ܺ�Ű �Է� ���� ����
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
 *	- NMCTG_PROTECT_SCREENCAPTURE�� ����
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
 *			bHide is TRUE	->Process name to hide(��ҹ��� ����)
 *			bHide is FALSE	-> NULL
 *
 *		pWindowTitle[in]
 *			bHide is TRUE	-> Window title string to hide(��ҹ��� ����)
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
 *	- NMCTG_PROTECT_SCREENCAPTURE�� ����
 */
typedef LONG (_stdcall *NMCTG_PROTECT_INPUT)(
	BOOL bProtect 
	);


/*
 * Get detect code 
 *
 *  Parameters
 *		pfNotifyCallBack [in]	: ȭ��ĸ�� ����, SW Ű����/���콺 �Է� ������ On �Ͽ����� �ܺ� ���μ������� ���ٽõ� �� �뺸 ���� �ݹ� �Լ�
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
