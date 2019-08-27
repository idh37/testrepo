// ObjectDef.h: interface for the CBaseObject class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

// 오브젝트 종류
enum ENUM_ANIFILE
{
	/////////////////////////////
	//이벤트 관련 부분 
	ANF_EVENTNONE = 0,
	ANF_EVENTCOIN,			//골드칩 이벤트 코인
	ANF_EVENTSTAGE,			//골든칩 이벤트 시작 활성화
	ANF_EVENTLEVELUP,		//도신지수 이벤트 렙업
	ANF_STEP_EVENTLEVELUP,	//도신지수 단계별 렙업
	ANF_GETBULLET,			//도신지수 응모권 지급 
	ANF_EVENTSTAGETEXT,		// 골든이벤트 스테이지 알림 텍스트
	//////////////////////////////////////

	/////////////////////////////////////
	//베팅 커멘드 부분
	ANF_BET_CMD = 10,
	ANF_BET_ALLIN,
	ANF_BET_HALF,
	ANF_BET_CALL,
	ANF_BET_CHECK,
	ANF_BET_DADANG,
	ANF_BET_PING,
	ANF_BET_DIE,
	
	//규제안 : 맥스 이팩트 추가
	ANF_BET_MAX,
	ANF_BET_FULL,
	ANF_TOTAL,

	//ANF_BET_MAX,
	/////////////////////////////////////

	ANF_ALLINCHARGE_INFO = 100,	//올인천사 충전 뉴도신지수 2006.3.2
	ANF_SUPERAVATA,				//슈퍼 아바타 지급 효과
	ANF_SUMMERAVATA,			//[썸머 이벤트] 2006.07.12
	ANF_VSMODEGAMESTARTCOUNT,	//VS모드 게임 시작 효과
	ANF_HIDDEN,					//히든 효과
	ANF_PREMSUPERALLINCHARGE,	//프리미엄 슈퍼 충전

	ANF_SDLG_GRADE,			//이용등급 표시

	ANF_PCROOMEVENT,		//pc방 이벤트
	ANF_HANDO,		//보유한도.
	ANF_PLAYER_COMPLETEMADE_EFT,	// 메이드이펙트
	ANF_PLAYER_CHANCEITEM,			// 찬스아바타 획득 이펙트

	
};
