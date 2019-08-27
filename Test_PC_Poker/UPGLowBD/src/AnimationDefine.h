#pragma once


//UPG작업 BaseObject에있던 enum OBJECT_TYPE을 여기로 옮김
// 오브젝트 종류
enum OBJECT_TYPE
{
	/////////////////////////////	
	ANF_UNKNOWN = -1,	
	//////////////////////////////////////
	ANF_BET_CMD,				// 배팅 Animation
	ANF_BET_HALF,
	ANF_BET_CALL,
	ANF_BET_CHECK,
	ANF_BET_DADANG,
	ANF_BET_PING,
	ANF_BET_DIE,
	ANF_BET_ALLIN,
		
	ANF_BET_LIMIT_SMALL_BET,
	ANF_BET_LIMIT_BIG_BET,
	ANF_BET_LIMIT_RAISE,	

	_ANF_SEPERATOR1_,

	ANF_BG_STAGESTATE,			//아침 점심 저녁 		
	ANF_BG_CUTINGINFO,		    //커팅 라운드와 장수 알림
	ANF_CUTINFO_NUM,

	ANF_PLAYER_WINEFFECT,		//승리 효과
	ANF_PLAYER_ALLINMARK,		//올인 마크
	ANF_PLAYER_ALLINANGEL,		//올인천사 (무료충전)
	ANF_PLAYER_JEKICHARGE,		//재기충전

	ANF_PLAYER_BLIND,			//로우바둑이 블라인드

	ANF_HIGHVALUE,				//골프( 잭팟 ), 세컨드, 써드
	ANF_PLAYER_CHANCEITEM,		//찬스아이템	
	
	ANF_PLAYER_OBSERVER,		//관전자 이미지
	ANF_PLAYER_COMPLETEMADE_EFT,//메이드 완성 효과

	ANF_SDLG_GRADE,

	//규제안 : 맥스 이팩트 추가
	ANF_BET_MAX,
	ANF_BET_FULL,
	ANF_TOTAL
};
