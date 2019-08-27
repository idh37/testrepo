// 글로벌 변수, 클래스, 함수..

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <../src/GlobalBase.h>
//#include <UPGPlatform.h>
#include <../src/Player.h>

#include "UserStruct.h"

#include "ImageDef.h"
#include "clientdlg.h"

// ### [ 관전기능 ] ###

extern HINSTANCE		g_hInstBaseDLL;

extern BOOL				g_bLoadSprSkinKind;		// 이미지로 로드된 상태.(그레이인지 블루인지)

extern BOOL				g_bChipDrawKind;
extern BOOL				g_bGetJokerCard;	// 조커 카드를 받았다

extern BOOL				g_nBetDeBugMode;	// 개발자용 디버그 플래그 

extern char				g_strMyVoteList[2][16];

char* GetStrArea(int num);
char* GetStrJob(int num);


#endif