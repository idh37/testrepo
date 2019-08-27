// AnimationQue.h: interface for the CAnimationQue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATIONQUE_H__CB560465_D8E7_4FFE_9857_FEF28633BB3A__INCLUDED_)
#define AFX_ANIMATIONQUE_H__CB560465_D8E7_4FFE_9857_FEF28633BB3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Graphic.h"

// const int MAX_ANIMATION_DATA = 20;	//각레이어당 표현할 수 있는 애니메이션숫자 
// const int MAX_ANIMATION_LIST = 30;	//애니메이션 종류

typedef struct
{
	POINT pos;
	int   kind;
	int	  delay;
	int	 option;
	int  option2;

	float index;
	float updateunit;

	//사라지는효과
	BOOL DisappearEft;	//사라지는 효과를 썻는가?
	int DisappearCnt;	//카운터
		
	char* soundsnd;	//[사운드인덱스]
	char* voicesnd;	//[음성인덱스]
	
	//[레볼루션용]
	DWORD nowtime;
	DWORD enddelay;
	int	  scene;

} GAMEANI_DATA;

enum GAMEANIMATION_LIST
{
	

	//### 하단에 깔리는 애니메이션 ###//	
	ANIMATION_LOW=0,
	
	//### 중단에 깔리는 애니메이션 ###//	
	ANIMATION_CARDGROWTH,	//카드 커지는 애니메이션 
	ANIMATION_RESULT_EFT,		//게임결과에 따른 이펙트	
	
	//### 상단에 깔리는 애니메이션 ###//
	ANIMATION_HIGH,	

	//레볼루션 방식의 애니메이션
	ANIMATION_CARDGROWTH_EFT,	//카드 커지는 애니메이션효과
	ANIMATION_POP2,				//팝업창2
	ANIMATION_REGAMENOTICE,		//재시합대결 안내문구
	ANIMATION_HIDDEN,			//자동쪼기
	ANIMATION_WINLOSS,			//승자패자효
	ANIMATION_ETCEFFECT,		//암행어사.떙잡이효과
	ANIMATION_ALLIN,			//올인표시	
	
	ANIMATION_MAX,	
};

class CAnimationQue  
{

	DWORD m_dwNowtime;
	DWORD m_dwPrevtime;
public:
	CAnimationQue();
	virtual ~CAnimationQue();

	void LoadImage();
	

	void Reset( BOOL r_alldata = TRUE );
	void Update();
	void Ani_Insert( int kind, POINT pos, int option = -1, int option2 = -1, int scene = -1, BOOL disappeareft = FALSE, char* soundsnd = NULL, char* voicesnd = NULL, int delay = 0 );
	//애니메이션의 속성을 바꿔준다. (ex 좌표, 옵션등등 )
	void SetAniData( int kind, POINT pos, int option = -1, int option2 = -1, int scene = -1, BOOL disappeareft = FALSE, char* soundsnd = NULL, char* voicesnd = NULL, int delay = 0 );

	BOOL CheckAniData( int kind );	//중복된 애니메이션이 있는지 검사한다.

	void Stop( int ani_kind, int part = -1 );	//클래스외의 다른곳에서 사용 
	
	//애니메이션수정
	void Low_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC );
	void Mid_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC );
	void High_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC );

	NMBASE::GRAPHICGDI::xSprite	m_AniSpr[ANIMATION_MAX];
	
private:

//	int m_Ani_index;	//몇번째 인덱스에 넣어줘야 되나 체크한다. 
//	int m_Ani_delay;	//동시에 애니메이션을 뜰경우 딜레이를 준다.

	void Clear( int datanum, int kind ); //클래스안에서 Update() 안에서 사용 

	//상중하중 어느 레이어에 속하는지 알아본다. 
	int GetLayerkind(int kind);

	//애니메이션수정
	GAMEANI_DATA m_Lowdata[ANIMATION_MAX];	
	GAMEANI_DATA m_Middata[ANIMATION_MAX];	
	GAMEANI_DATA m_Highdata[ANIMATION_MAX];	

	//애니메이션수정
	void Low_Stop( int ani_kind, int part );
	void Mid_Stop( int ani_kind, int part );
	void High_Stop( int ani_kind, int part );

	//애니메이션수정
	void Low_Update( int datanum, DWORD delta );
	void Mid_Update( int datanum, DWORD delta );
	void High_Update( int datanum, DWORD delta );

};

#endif // !defined(AFX_ANIMATIONQUE_H__CB560465_D8E7_4FFE_9857_FEF28633BB3A__INCLUDED_)
