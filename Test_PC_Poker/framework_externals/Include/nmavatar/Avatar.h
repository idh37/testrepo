/**    
@file    Avatar.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   아바타 관리 
*/

#pragma once

#include "AvatarDefine.h"

namespace NMBASE
{
	namespace AVATAR
	{

		//////////////////////////////////////////////////////////////////////

		// 서비스 사이트에 따른 아바타 폴더를 변경하기 위하여 호출하는 함수(InitAvatarModule() 이전에 호출되어야 함)
		// 다음게임인 경우 "daum", 하나포스게임인 경우 "hanafos"를 넘김(넷마블인 경우는 호출할 필요가 없음)
		NMAVATAR_API void PresetAvatarModuleSetSiteName(LPCTSTR strSiteName);

		// 아바타 모듈 초기화 함수(모듈의 해제는 프로그램 종료시 자동으로 수행된다)
		NMAVATAR_API BOOL InitAvatarModule();

		// 테스트용 초기화 함수
		NMAVATAR_API BOOL InitAvatarModule_ForTest(LPCSTR strBaseDir);

		// 아바타 모듈 기본 폴더를 강제로 설정하는 함수
		NMAVATAR_API BOOL SetBaseAvatarDir(char *strFolder);

		// 아바타 모듈 기본 폴더를 얻는 함수
		NMAVATAR_API BOOL GetBaseAvatarDir(char *strBuffer, int buffersize);

		//////////////////////////////////////////////////////////////////////
		// 아바타 클래스

		class CAvatarResource;

		class NMAVATAR_CLASS CAvatar  
		{
		public:

			CAvatar();
			virtual ~CAvatar();

			// 아바타 초기화 및 생성
			BOOL Create();
			BOOL Create(short *pAvatarInfo);
			BOOL Create(int *pAvatarInfo);
			BOOL Create(int width, int height, short *pAvatarInfo);
			BOOL Create(int width, int height, int *pAvatarInfo);

			void ClearMember();
			void Destroy();											// 아바타 파괴

			BOOL SetAvatar(short *pAvatarInfo);						// 아바타 정보 설정
			BOOL SetAvatar(int *pAvatarInfo);						// 아바타 정보 설정
			BOOL SetForceAvatar(short *pAvatarInfo, BOOL bAutoPair=FALSE);	// 아바타 정보 설정(강제로 설정, AutoPair옵션이 TRUE인 경우 코트나 헤어와 같이 쌍으로 이루어지는 아이템만 자동 교정)
			BOOL SetForceAvatar(int *pAvatarInfo, BOOL bAutoPair=FALSE);	// 아바타 정보 설정(강제로 설정, AutoPair옵션이 TRUE인 경우 코트나 헤어와 같이 쌍으로 이루어지는 아이템만 자동 교정)
			BOOL SetSmallAvatar(short *pAvatarInfo);				// 작은 아바타 정보 설정
			BOOL SetSmallAvatar(int *pAvatarInfo);					// 작은 아바타 정보 설정
			BOOL SetBackgroundVisible(BOOL bShow);					// 배경인 0번 레이어의 표시 여부(Default: FALSE)
			BOOL SetSize(int width, int height);					// 아바타 사이즈 설정
			BOOL SetOffset(int ox, int oy);							// 아바타 오프셋 설정(디폴트 0,0)
			BOOL ResetAnimation();									// 아바타 에니메이션 리셋
			BOOL SetAnimationTime(WORD tick);						// 아바타 에니메이션 타임을 세팅
			BOOL SetBackBitmap(HBITMAP hBmp, int sx, int sy);		// 아바타 백그라운드 비트맵 이미지 설정(DC에 그리는 경우 배경 이미지를 설정하여 빠르게 그리는데 사용) 
			// 주의) 비트맵을 설정한 경우는 Draw()호출시 더블버퍼링이 필요없다(내부적으로 수행됨)
			BOOL IsAvatarDownloading();								// 아바타 이미지가 다운로드중인지 검사

			// 아바타를 DC에 그리기(백그라운드 비트맵을 설정하지 않는 경우는 Mask를 생성하기 때문에 속도가 느림)
			BOOL Draw(int xp, int yp, HDC hdcDest, BOOL bMirror);	
			// 아바타를 메모리에 그리기(디폴트는 16bit RGB555)
			BOOL Draw(int xp, int yp, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat = APF_RGB555);
			// 아바타를 DC에 그리기( 부드럽게 확대/축소가 적용됨 )
			BOOL Draw_SmoothZoom(int xp, int yp, HDC hdcDest, int tx, int ty, COLORREF crBackColor, BOOL bMirror);

			//////// static members
			static char* GetAvatarStatus();							// 아바타 DLL의 상태를 스트링으로 얻음

		protected:
			BOOL SetAvatarUnsignedShort(HAVATAR h, unsigned short *pLayerItem);
			BOOL SetAvatarInt(HAVATAR h, int *pLayerItem);
			BOOL raw_SetAvatar(HAVATAR h, int *pLayerItem);
			BOOL PrepareResource(CAvatarResource* pRes, int *item, BOOL bSmall);
			BOOL PrepareResource(HAVATAR h, int *item, BOOL bSmall);
			static char* GetAvatarStatusString();
			BOOL DrawAvatarToDC_SmoothZoom(int xp, int yp, HAVATAR h, HDC hdcDest, int tx, int ty, COLORREF crBackColor, BOOL bMirror);
			BOOL DrawAvatarToMemory(int xp, int yp, HAVATAR h, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat);
			BOOL DrawAvatarToDC(int xp, int yp, HAVATAR h, HDC hdcDest, BOOL bMirror);
			BOOL IsAvatarDownloading(HAVATAR h);
			BOOL SetAvatarBackBitmap(HAVATAR h, HBITMAP hBmp, int srcx, int srcy);
			BOOL SetAvatarAnimationTime(HAVATAR h, WORD tick);
			BOOL ResetAvatarAnimation(HAVATAR h);
			BOOL SetAvatarSize(HAVATAR h, int width, int height);
			BOOL SetAvatarOffset(HAVATAR h, int ox, int oy);
			BOOL SetBackgroundVisible(HAVATAR h, BOOL bShow);
			BOOL SetSmallAvatarEx(HAVATAR h, unsigned short *pLayerItem);
			BOOL SetSmallAvatar(HAVATAR h, short *pLayerItem);
			BOOL SetSmallAvatar(HAVATAR h, int *pLayerItem);
			BOOL SetForceAvatar(HAVATAR h, short *pLayerItem, BOOL bAutoPair);
			BOOL SetForceAvatar(HAVATAR h, int *pLayerItem, BOOL bAutoPair);
			BOOL DestroyAvatarResource(HAVATAR h);
			HAVATAR CreateAvatarResource();

		protected:

			HAVATAR hAvatar;

			BOOL m_bCheckMir;
			BOOL m_bLastDrawMir;

			int m_OrgAvatarInfo[MAX_LAYER_4HERE];
			int m_MirAvatarInfo[MAX_LAYER_4HERE];

			void PrepareMirror(BOOL bMirror);
		};

	}//namespace AVATAR
}//namespace NMBASE