// Global....
#include "StdAfx.h"
#include "Global.h"

extern HINSTANCE		g_hInstBaseDLL=NULL;

BOOL			g_bLoadSprSkinKind = -1;

BOOL			g_bChipDrawKind = FALSE;
BOOL			g_bGetJokerCard;		// ��Ŀ ī�带 �޾Ҵ�

BOOL			g_nBetDeBugMode = 0;	// �����ڿ� ����� �÷��� 

char			g_strMyVoteList[2][16];

//int g_GetURLChannelNo()
//{
//	//�� URL ���� �����  ä�ι�ȣ ���ϱ�	
//	switch (g_NowGroupNo)
//	{		
//		case GROUP_GRADE_AMATEUR:
//		case GROUP_GRADE_PRO:
//			return 1;		
//		case GROUP_GRADE_PLATINUM_1:
//			return 2;
//		case GROUP_GRADE_FRIENDS:
//		case GROUP_GRADE_PLATINUM_2:
//		case GROUP_GRADE_PLATINUM_3:
//			return 3;
//	}
//
//	return 1;
//}

// ���� ��Ʈ��
char *StrArea[17]={
	"��������", "����",     "�λ�",     "����",     "�뱸",
		"����",     "��õ",     "���",     "��⵵",   "������",
		"��û�ϵ�", "��û����", "���ϵ�", "��󳲵�", "����ϵ�",
		"���󳲵�", "���ֵ�"};	
	
// ���� ��Ʈ��
char* StrJob[21]={
"��������", "�ʵ��л�", "�߰��", "����(��)��", "ȸ���",
	"������",   "������",   "�ֺ�",   "����",       "�Ƿ���",
	"������",   "������",   "�����", "������",     "�ڿ���",
	"�󸲾�",   "����",   "�����", "�����",     "����",
	"��Ÿ"};

// ���� ��Ʈ���� ����
char* GetStrArea(int num)
{
	if(num<0 || num>=17) return NULL;
	return StrArea[num];
}

// ���� ��Ʈ���� ����
char* GetStrJob(int num)
{
	if(num<0 || num>=21) return NULL;
	return StrJob[num];
}