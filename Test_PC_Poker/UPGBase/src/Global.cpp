// Global....
#include "StdAfx.h"
#include "Global.h"

extern HINSTANCE		g_hInstBaseDLL=NULL;

BOOL			g_bLoadSprSkinKind = -1;

BOOL			g_bChipDrawKind = FALSE;
BOOL			g_bGetJokerCard;		// 조커 카드를 받았다

BOOL			g_nBetDeBugMode = 0;	// 개발자용 디버그 플래그 

char			g_strMyVoteList[2][16];

//int g_GetURLChannelNo()
//{
//	//웹 URL 에서 사용할  채널번호 구하기	
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

// 지역 스트링
char *StrArea[17]={
	"지역선택", "서울",     "부산",     "대전",     "대구",
		"광주",     "인천",     "울산",     "경기도",   "강원도",
		"충청북도", "충청남도", "경상북도", "경상남도", "전라북도",
		"전라남도", "제주도"};	
	
// 직업 스트링
char* StrJob[21]={
"직업선택", "초등학생", "중고생", "대학(원)생", "회사원",
	"공무원",   "교직자",   "주부",   "군인",       "의료인",
	"법조인",   "종교인",   "언론인", "예술인",     "자영업",
	"농림업",   "축산업",   "수산업", "광고업",     "무직",
	"기타"};

// 지역 스트링을 얻음
char* GetStrArea(int num)
{
	if(num<0 || num>=17) return NULL;
	return StrArea[num];
}

// 직업 스트링을 얻음
char* GetStrJob(int num)
{
	if(num<0 || num>=21) return NULL;
	return StrJob[num];
}