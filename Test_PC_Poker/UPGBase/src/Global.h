// �۷ι� ����, Ŭ����, �Լ�..

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <../src/GlobalBase.h>
//#include <UPGPlatform.h>
#include <../src/Player.h>

#include "UserStruct.h"

#include "ImageDef.h"
#include "clientdlg.h"

// ### [ ������� ] ###

extern HINSTANCE		g_hInstBaseDLL;

extern BOOL				g_bLoadSprSkinKind;		// �̹����� �ε�� ����.(�׷������� �������)

extern BOOL				g_bChipDrawKind;
extern BOOL				g_bGetJokerCard;	// ��Ŀ ī�带 �޾Ҵ�

extern BOOL				g_nBetDeBugMode;	// �����ڿ� ����� �÷��� 

extern char				g_strMyVoteList[2][16];

char* GetStrArea(int num);
char* GetStrJob(int num);


#endif