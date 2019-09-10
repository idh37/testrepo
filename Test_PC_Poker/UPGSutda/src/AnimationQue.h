// AnimationQue.h: interface for the CAnimationQue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATIONQUE_H__CB560465_D8E7_4FFE_9857_FEF28633BB3A__INCLUDED_)
#define AFX_ANIMATIONQUE_H__CB560465_D8E7_4FFE_9857_FEF28633BB3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Graphic.h"

// const int MAX_ANIMATION_DATA = 20;	//�����̾�� ǥ���� �� �ִ� �ִϸ��̼Ǽ��� 
// const int MAX_ANIMATION_LIST = 30;	//�ִϸ��̼� ����

typedef struct
{
	POINT pos;
	int   kind;
	int	  delay;
	int	 option;
	int  option2;

	float index;
	float updateunit;

	//�������ȿ��
	BOOL DisappearEft;	//������� ȿ���� ���°�?
	int DisappearCnt;	//ī����
		
	char* soundsnd;	//[�����ε���]
	char* voicesnd;	//[�����ε���]
	
	//[������ǿ�]
	DWORD nowtime;
	DWORD enddelay;
	int	  scene;

} GAMEANI_DATA;

enum GAMEANIMATION_LIST
{
	

	//### �ϴܿ� �򸮴� �ִϸ��̼� ###//	
	ANIMATION_LOW=0,
	
	//### �ߴܿ� �򸮴� �ִϸ��̼� ###//	
	ANIMATION_CARDGROWTH,	//ī�� Ŀ���� �ִϸ��̼� 
	ANIMATION_RESULT_EFT,		//���Ӱ���� ���� ����Ʈ	
	
	//### ��ܿ� �򸮴� �ִϸ��̼� ###//
	ANIMATION_HIGH,	

	//������� ����� �ִϸ��̼�
	ANIMATION_CARDGROWTH_EFT,	//ī�� Ŀ���� �ִϸ��̼�ȿ��
	ANIMATION_POP2,				//�˾�â2
	ANIMATION_REGAMENOTICE,		//����մ�� �ȳ�����
	ANIMATION_HIDDEN,			//�ڵ��ɱ�
	ANIMATION_WINLOSS,			//��������ȿ
	ANIMATION_ETCEFFECT,		//������.������ȿ��
	ANIMATION_ALLIN,			//����ǥ��	
	
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
	//�ִϸ��̼��� �Ӽ��� �ٲ��ش�. (ex ��ǥ, �ɼǵ�� )
	void SetAniData( int kind, POINT pos, int option = -1, int option2 = -1, int scene = -1, BOOL disappeareft = FALSE, char* soundsnd = NULL, char* voicesnd = NULL, int delay = 0 );

	BOOL CheckAniData( int kind );	//�ߺ��� �ִϸ��̼��� �ִ��� �˻��Ѵ�.

	void Stop( int ani_kind, int part = -1 );	//Ŭ�������� �ٸ������� ��� 
	
	//�ִϸ��̼Ǽ���
	void Low_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC );
	void Mid_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC );
	void High_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC );

	NMBASE::GRAPHICGDI::xSprite	m_AniSpr[ANIMATION_MAX];
	
private:

//	int m_Ani_index;	//���° �ε����� �־���� �ǳ� üũ�Ѵ�. 
//	int m_Ani_delay;	//���ÿ� �ִϸ��̼��� ���� �����̸� �ش�.

	void Clear( int datanum, int kind ); //Ŭ�����ȿ��� Update() �ȿ��� ��� 

	//�������� ��� ���̾ ���ϴ��� �˾ƺ���. 
	int GetLayerkind(int kind);

	//�ִϸ��̼Ǽ���
	GAMEANI_DATA m_Lowdata[ANIMATION_MAX];	
	GAMEANI_DATA m_Middata[ANIMATION_MAX];	
	GAMEANI_DATA m_Highdata[ANIMATION_MAX];	

	//�ִϸ��̼Ǽ���
	void Low_Stop( int ani_kind, int part );
	void Mid_Stop( int ani_kind, int part );
	void High_Stop( int ani_kind, int part );

	//�ִϸ��̼Ǽ���
	void Low_Update( int datanum, DWORD delta );
	void Mid_Update( int datanum, DWORD delta );
	void High_Update( int datanum, DWORD delta );

};

#endif // !defined(AFX_ANIMATIONQUE_H__CB560465_D8E7_4FFE_9857_FEF28633BB3A__INCLUDED_)
