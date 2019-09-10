// AnimationQue.cpp: implementation of the CAnimationQue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnimationQue.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimationQue::CAnimationQue()
{

	m_dwNowtime = 0;
	m_dwPrevtime = timeGetTime();
	Reset();
//	InitGraphic();
}

CAnimationQue::~CAnimationQue()
{

}

//��������Ʈ�б�
void CAnimationQue::LoadImage()
{

	
	//ī��ִϸ��̼�(ī��Ŀ���°�)
	AddLoadImageList(&m_AniSpr[ANIMATION_CARDGROWTH ], ".\\sutda\\data\\AniData\\CARDGROWTH.spr", true);

	//ī��ִϸ��̼�(ī������ ȿ����)
	AddLoadImageList(&m_AniSpr[ANIMATION_CARDGROWTH_EFT], ".\\sutda\\data\\AniData\\CARDGROWTH_EFT.spr", true);

	//��������� ���� ����Ʈ
	AddLoadImageList(&m_AniSpr[ANIMATION_RESULT_EFT], ".\\sutda\\data\\AniData\\Result_Eft.spr", true);
	
	//�˾�â
	AddLoadImageList(&m_AniSpr[ANIMATION_POP2], ".\\sutda\\data\\pop\\Pop_2.spr", true);
	
	AddLoadImageList(&m_AniSpr[ANIMATION_REGAMENOTICE], ".\\sutda\\data\\pop\\RematchGameNotice.spr", true);
	
	//�ڵ��ɱ�
	AddLoadImageList(&m_AniSpr[ANIMATION_HIDDEN], ".\\sutda\\data\\Anidata\\Hidden.spr", true);
	
	//��� ���� �ִϸ��̼�
	AddLoadImageList(&m_AniSpr[ANIMATION_WINLOSS], ".\\sutda\\data\\Anidata\\WinLose.spr", true);

	//������ ������ ȿ��
	AddLoadImageList(&m_AniSpr[ANIMATION_ETCEFFECT], ".\\sutda\\data\\Anidata\\EtcEffect.spr", true);

	//����ǥ��
	AddLoadImageList(&m_AniSpr[ANIMATION_ALLIN], ".\\sutda\\data\\Anidata\\Allin.spr", true);
	

}


void CAnimationQue::Reset( BOOL r_alldata)
{
	int i;

	if( r_alldata )
	{
		memset( &m_Lowdata, 0, sizeof(m_Lowdata) ); 
		memset( &m_Middata, 0, sizeof(m_Middata) ); 
		memset( &m_Highdata, 0, sizeof(m_Highdata) ); 
	}
	else
	{
		for(  i = 0; i < ANIMATION_MAX; i++ )
		{
			memset( &m_Lowdata[i], 0, sizeof(m_Lowdata[i]) ); 
		}

		for( i = 0; i < ANIMATION_MAX; i++ )
		{
			//���������༭�� �ȵǴ� �ִϴ� ���� ���´�.
// 			if( m_Middata[i].kind == ANIMATION_SMOKE || m_Middata[i].kind == ANIMATION_AUTOBET )
// 			{
// 				continue;
// 			}			
			
			memset( &m_Middata[i], 0, sizeof(m_Middata[i]) ); 
		}

		for( i = 0; i < ANIMATION_MAX; i++ )
		{
			/*
			//���������༭�� �ȵǴ� �ִϴ� ���� ���´�.
			if(m_Highdata[i].kind == ANIMATION_AUTOBET)
			{
				continue;
			}
			*/
			memset( &m_Highdata[i], 0, sizeof(m_Highdata[i]) ); 
		}
	}
}

//�ߺ��� �ִϸ��̼��� �ִ��� �˻��Ѵ�.
BOOL CAnimationQue::CheckAniData( int kind )
{

	int i;
	//## ������̾ ���ϴ��� �˾ƺ���. ##//
	int layer_kind = GetLayerkind( kind );
	if( layer_kind == -1 ) return FALSE;

	//## ����ִ� �ִϸ��̼� ����ü�� ã�´�. ##//
	GAMEANI_DATA* tempdata = NULL;	
	for(  i = 0; i < ANIMATION_MAX; i++)
	{
		if( layer_kind == 0 )
		{
			//���� �ִϸ��̼��� �ִ��� ã�ƺ���.
			if( m_Lowdata[i].kind == kind) 
			{
				return FALSE;
			}
		}
		else if( layer_kind == 1 )
		{
			if( m_Middata[i].kind == kind) 
			{
				return FALSE;
			}
		}
		else if( layer_kind == 2 )
		{
			if( m_Highdata[i].kind == kind) 
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;	
}

//��� ���̾ ���ϴ� �ִϸ��̼����� �˾ƺ���.
int CAnimationQue::GetLayerkind( int kind)
{
	int layer_kind = -1;

	//�ϴܿ� �򸮴� �ִϸ��̼� 
	if( kind < ANIMATION_LOW)	
	{
		layer_kind = 0;
	}
	//�߰��� �򸮴� �ִϸ��̼� 
	else if( kind > ANIMATION_LOW && kind < ANIMATION_HIGH)
	{
		layer_kind = 1;
	}
	//�ֻ�ܿ� �򸮴� �ִϸ��̼� 
	else if( kind > ANIMATION_HIGH && kind < ANIMATION_MAX )
	{
		layer_kind = 2;
	}

	return layer_kind;
}

void CAnimationQue::Ani_Insert( int kind, POINT pos, int option, int option2, int scene, BOOL disappeareft, char* soundsnd, char* voicesnd, int delay )
{
//	if( m_Ani_index < 0 || m_Ani_index >= ANIMATION_MAX) return;

	//## ������̾ ���ϴ��� �˾ƺ���. ##//
	int layer_kind = GetLayerkind( kind );
	if( layer_kind == -1 ) return;

	//## ����ִ� �ִϸ��̼� ����ü�� ã�´�. ##//
	GAMEANI_DATA* tempdata = NULL;	
	for( int i = 0; i < ANIMATION_MAX; i++)
	{
		if( layer_kind == 0 )
		{
			if( m_Lowdata[i].kind == 0) 
			{
				tempdata = &m_Lowdata[i];
//				m_Ani_index = i;
				break;
			}
		}
		else if( layer_kind == 1 )
		{
			if( m_Middata[i].kind == 0) 
			{
				tempdata = &m_Middata[i];
//				m_Ani_index = i;
				break;
			}
		}
		else if( layer_kind == 2 )
		{
			if( m_Highdata[i].kind == 0) 
			{
				tempdata = &m_Highdata[i];
//				m_Ani_index = i;
				break;
			}
		}
	}
	
	//## ����ִ� ����Ÿ�� ã�� �������� ����~ ##//
	if( tempdata == NULL ) return;
/*
	if( m_Ani_delay < 0)
	{
#ifdef _DEBUG
		AfxMessageBox("CAnimationQue : m_Ani_delay == 0 ");
#endif
		m_Ani_delay = 0;
	}
*/

	//## �ִϸ��̼� ������ �Ѱ��ش� ##//
	tempdata->kind = kind;
	tempdata->pos = pos;
	tempdata->index = 0;
	tempdata->delay = 0;
	tempdata->updateunit = 0;
	tempdata->option = option;
	tempdata->option2 = option2;
	tempdata->scene = scene;
	tempdata->DisappearEft = disappeareft;
	tempdata->soundsnd = soundsnd;
	tempdata->voicesnd = voicesnd;
	tempdata->delay = delay;

	switch(kind)
	{
		//### ���ο� �ִ� ###//

	//ī�� Ŀ���� �޹��
	case ANIMATION_CARDGROWTH_EFT:
		{
			//tempdata->delay = 10;
		}break;
	//ī�尡 Ŀ������ ī�带 �����ش�.
	case ANIMATION_CARDGROWTH:
		{
		//	tempdata->pos.x = Play[option].MyCard[option2].Xp;
		//	tempdata->pos.y = Play[option].MyCard[option2].Yp;
			
		//	if( option2 == 1) tempdata->delay = 22;

			//ī�带 �Ⱥ��̰� ���ش�.
			//Play[option].MyCard[option2].SetShow( FALSE );
		}break;
		//////////////////////////////////////////////////////////////////////////
		


	default:
		{
			tempdata->updateunit = 0.08f;
		}break;
	}

//	m_Ani_delay += DEFAULT_DELAY_TERM;
//	m_Ani_index++;		//?????
//	m_Ani_index %= ANIMATION_MAX;
}

void CAnimationQue::SetAniData( int kind, POINT pos, int option, int option2, int scene, BOOL disappeareft, char* soundsnd, char* voicesnd, int delay )
{
	//## ������̾ ���ϴ��� �˾ƺ���. ##//
	int layer_kind = GetLayerkind( kind );
	if( layer_kind == -1 ) return;

	//## ����ִ� �ִϸ��̼� ����ü�� ã�´�. ##//
	GAMEANI_DATA* tempdata = NULL;	
	for( int i = 0; i < ANIMATION_MAX; i++)
	{
		if( layer_kind == 0 )
		{
			//���� �ִϸ��̼��� �ִ��� ã�ƺ���.
			if( m_Lowdata[i].kind == kind) 
			{
				tempdata = &m_Lowdata[i];
				break;
			}
		}
		else if( layer_kind == 1 )
		{
			if( m_Middata[i].kind == kind) 
			{
				tempdata = &m_Middata[i];
				break;
			}
		}
		else if( layer_kind == 2 )
		{
			if( m_Highdata[i].kind == kind) 
			{
				tempdata = &m_Highdata[i];
				break;
			}
		}
	}
	
	//## ����ִ� ����Ÿ�� ã�� �������� ����~ ##//
	if( tempdata == NULL ) return;


	//## �ִϸ��̼� ������ �Ѱ��ش� ##//
	tempdata->kind = kind;
	tempdata->pos = pos;
	tempdata->index = 0;
	tempdata->delay = 0;
	tempdata->updateunit = 0;
	tempdata->option = option;
	tempdata->option2 = option2;
	tempdata->scene = scene;
	tempdata->DisappearEft = disappeareft;
	tempdata->soundsnd = soundsnd;
	tempdata->voicesnd = voicesnd;
	tempdata->delay = delay;
}

//################## �ִϸ��̼ǵ���Ÿ�� ����� ######################//
void CAnimationQue::Clear( int datanum, int kind )
{
	if( datanum < 0 || datanum >= ANIMATION_MAX) return;

	int layer_kind = GetLayerkind( kind );

	switch( layer_kind )
	{
	case 0:
		{
			if( m_Lowdata[datanum].kind > 0 )
			{
				ZeroMemory( &m_Lowdata[datanum], sizeof(GAMEANI_DATA) );
			}
		}break;

	case 1:
		{
			if( m_Middata[datanum].kind > 0 )
			{
				ZeroMemory( &m_Middata[datanum], sizeof(GAMEANI_DATA) );
			}
		}break;

	case 2:
		{
			if( m_Highdata[datanum].kind > 0 )
			{
				ZeroMemory( &m_Highdata[datanum], sizeof(GAMEANI_DATA) );
			}
		}break;
	}

	/*
	if( m_Animationdata[i].kind > 0 )
	{
		ZeroMemory( &m_Animationdata[i], sizeof(GAMEANI_DATA) );
	}
	*/
}

//################## �ִϸ��̼��� ����� ######################//
void CAnimationQue::Stop( int ani_kind, int part )
{
	int layer_kind =  GetLayerkind( ani_kind );
	
	switch( layer_kind )
	{
	case 0:
		{
			Low_Stop( ani_kind, part);
		}break;
		
	case 1:
		{
			Mid_Stop( ani_kind, part);
		}break;

	case 2:
		{
			High_Stop( ani_kind, part);
		}break;
	}
}

void CAnimationQue::Low_Stop(int ani_kind, int part)
{
//	for( int i = 0; i < ANIMATION_MAX; i++)
//	{
		//�������� �ִϸ��̼����� �˻��Ѵ�.
//		if( m_Lowdata[i].kind != ani_kind ) continue;
	   
		//�ִϸ��̼��� �߰��Ǹ� �ּ������ �ڵ� �߰� 
// 		switch( m_Lowdata[i].kind )
// 		{
// 		case ANIMATION_JOKBOHELP:	//�����м���
// 			{
// 				Clear(i, m_Lowdata[i].kind);	
// 			}break;
// 
// 		case ANIMATION_BET:		//����ǥ��
// 			{
// 				//������������ üũ�Ѵ�.
// 				if( m_Lowdata[i].option == part )
// 				{
// 					Clear(i, m_Lowdata[i].kind);
// 				}
// 			}break;
// 
// 			
// 		}
//	}
}

void CAnimationQue::Mid_Stop(int ani_kind, int part)
{
	for( int i = 0; i < ANIMATION_MAX; i++)
	{
		//�������� �ִϸ��̼����� �˻��Ѵ�.
		if(m_Middata[i].kind != ani_kind ) continue;

		switch( m_Middata[i].kind )
		{		
			
			case ANIMATION_RESULT_EFT:			
			{			
				Clear(i, m_Middata[i].kind);				
			}break;
		}
	}
}

void CAnimationQue::High_Stop(int ani_kind, int part)
{
	for( int i = 0; i < ANIMATION_MAX; i++)
	{
		//�������� �ִϸ��̼����� �˻��Ѵ�.
		if( m_Highdata[i].kind != ani_kind ) continue;
	
		switch( m_Highdata[i].kind )
		{
			//case ANIMATION_HIDDEN:
			case ANIMATION_REGAMENOTICE:
			case ANIMATION_POP2:
			case ANIMATION_CARDGROWTH_EFT:
			case ANIMATION_WINLOSS:
			case ANIMATION_ETCEFFECT:			
			{							
				Clear(i, m_Highdata[i].kind);				
			}break;

			case ANIMATION_HIDDEN:
			{
				//Game.SetJjogiMsg(); //ī�带 ��ôٰ� ������ ����
				Clear(i, m_Highdata[i].kind);
			}break;

			case ANIMATION_ALLIN:
			{
				if(part == m_Highdata[i].option)
				{
					Clear(i, m_Highdata[i].kind);			
				}
			}break;
		}
	}
}
//#####################################################################

//############## �ִϸ��̼��� ������Ʈ�ϴ� �κ� ###############//
void CAnimationQue::Update()
{
	//[�������]
	m_dwNowtime = timeGetTime();
	DWORD delta = m_dwNowtime - m_dwPrevtime;
	m_dwPrevtime = m_dwNowtime;


	for( int i = 0; i < ANIMATION_MAX; i++ )
	{
		//����Ÿ�� ������� ��츸 ������Ʈ���ش�.
		if(m_Lowdata[i].kind != 0) Low_Update( i, delta );
		if(m_Middata[i].kind != 0) Mid_Update( i, delta );
		if(m_Highdata[i].kind != 0) High_Update( i, delta );
	}
}

void CAnimationQue::Low_Update( int datanum, DWORD delta )
{
	if( m_Lowdata[datanum].delay > 0 ) m_Lowdata[datanum].delay -= 1;
	if( m_Lowdata[datanum].delay != 0 ) return;

	int sprindex = m_Lowdata[datanum].kind;
	if( sprindex < 0 || sprindex >= ANIMATION_MAX ) return;

	int kind = m_Lowdata[datanum].kind;
//	POINT pt = m_Lowdata[datanum].pos;
	
	//���带 ������ �Ѵ�
	if( m_Lowdata[datanum].soundsnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayWav( m_Lowdata[datanum].soundsnd );
		m_Lowdata[datanum].soundsnd = NULL;
	}
	//������ ������ �Ѵ�.
	if( m_Lowdata[datanum].voicesnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayOggStream( m_Lowdata[datanum].voicesnd, FALSE );
		m_Lowdata[datanum].voicesnd = NULL;
	}

// 	switch( kind )
// 	{
// 	case ANIMATION_JOKBOHELP:	//������������
// 		{
// 			if( m_AniSpr[sprindex].IsSceneTimeOver( m_Lowdata[datanum].scene, m_Lowdata[datanum].nowtime ) )
// 			{
// 				Clear( datanum,  kind );
// 			}
// 			else
// 			{
// 				m_Lowdata[datanum].nowtime += delta;
// 			}
// 		}break;
// 		
// 		//������ �ִϸ��̼��� ������ �ִ´�
// 	case ANIMATION_BET:
// 		{
// 			m_Lowdata[datanum].nowtime += delta;
// 			if( m_Lowdata[datanum].option2 == 1)
// 			{
// 				//��Ƽ��
// 				if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Lowdata[datanum].scene, m_Lowdata[datanum].nowtime ) )
// 				{	
// 
// 					Clear( datanum,  kind );
// 				}
// 				else
// 				{
// 					m_Lowdata[datanum].nowtime += delta;
// 				}
// 			}
// 			else if(  m_Lowdata[datanum].option2 == -1)
// 			{			
// 				if( m_AniSpr[sprindex].IsSceneTimeOver( m_Lowdata[datanum].scene, m_Lowdata[datanum].nowtime ) )
// 				{
// 					Clear( datanum,  kind );
// 				}
// 				else
// 				{
// 					m_Lowdata[datanum].nowtime += delta;
// 				}
// 			}
// 		}break;
// 	}
}

void CAnimationQue::Mid_Update( int datanum, DWORD delta )
{
	if( m_Middata[datanum].delay > 0 ) m_Middata[datanum].delay -= 1;
	if( m_Middata[datanum].delay != 0 ) return;

	int sprindex = m_Middata[datanum].kind;
	if( sprindex < 0 || sprindex >= ANIMATION_MAX ) return;

	int kind = m_Middata[datanum].kind;
//	POINT pt = m_Middata[datanum].pos;

		
	//���带 ������ �Ѵ�
	if( m_Middata[datanum].soundsnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayWav( m_Middata[datanum].soundsnd );
		m_Middata[datanum].soundsnd = NULL;
		//Sound.PlayWav( (SND_ENUM)m_Middata[datanum].soundsnd );
		//m_Middata[datanum].soundsnd = -1;
	}

	//������ ������ �Ѵ�.
	if( m_Middata[datanum].voicesnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayOggStream( m_Middata[datanum].voicesnd, FALSE );
		m_Middata[datanum].voicesnd = NULL;
	}

	switch(kind)
	{
	case ANIMATION_RESULT_EFT:
		{
			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Middata[datanum].scene, m_Middata[datanum].nowtime ) )
			{				
				Clear( datanum,  kind );
			}
			else
			{
				m_Middata[datanum].nowtime += delta;

				if(m_Middata[datanum].nowtime >= 1000 && m_Middata[datanum].option != 0)
				{

					//���Ӱ���� ���� ������ ����ش�.
					//����а� �ƴѰ��� ���س��� ���带 ����ش�. 
					//���� ���̽�  ������ �ƴϰ� �������� ���� �����ϰ�� 
				
					//����ο����� ����а� �ƴϴ�.
					//if( !g_bWatch && strlen(Play[0].UI.ID) > 0 && Play[0].JoinState == 0/* || Play[0].LastState == GCMND_FOLD*/ )					
					if (g_ObserverMan.IsMyObserver()==FALSE)
					{					
						if ( strlen(g_PLAYER(0)->UI.ID) && g_PLAYER(0)->JoinState == EGAME_JOINOUT )
						{
							SutdaGameSound().OnActionVoice( AV_FAIL_HIDDEN, 0, 2); //���̽�
						}
						else
						{
							BOOL bMyWin = (BOOL) m_Middata[datanum].option2;    //���� �̰峪?
							BOOL bMyFold = (BOOL) g_PLAYER(0)->bFold;    //���� �����߳�?
							SutdaGameSound().OnResultVoice(bMyWin, bMyFold);
						}
					}
					
					m_Middata[datanum].option = 0;
				
				}	
				else if(m_Middata[datanum].nowtime >= 2800)
				{
					if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
					{
						MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
					}
					else
					{					
						MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
					}
				}
			}
		}break;

		//[�������]
		//### �ѹ��� ������ �ִϸ��̼� ###//
		case ANIMATION_CARDGROWTH:			//ī��Ŀ����ȿ�� 
			{
				if( m_AniSpr[sprindex].IsSceneTimeOver( m_Middata[datanum].scene, m_Middata[datanum].nowtime ) )
				{			
					//���� ������ ī�带 �ٽ� ���̰� ���ش�.
					if(	m_Middata[datanum].kind == ANIMATION_CARDGROWTH )
					{
						//Play[ m_Middata[datanum].option ].MyCard[ m_Middata[datanum].option2 ].SetShow( TRUE );
						g_PLAYER(m_Middata[datanum].option)->ShowCard(m_Middata[datanum].option2, true);
					}

					Clear( datanum,  kind );
				}
				else
				{
					//ī�带 �����ش�.					
					if( m_Middata[datanum].option2 >= 0 )
					{
						
						//Play[m_Middata[datanum].option].MyCard[m_Middata[datanum].option2].SetShow( FALSE );
						g_PLAYER(m_Middata[datanum].option)->ShowCard(m_Middata[datanum].option2, false);
					
					}
					
					m_Middata[datanum].nowtime += delta;
				
				}
			}break;	
		
	}
	
	//��������� �ƴѰ�� �̹��� ������ ���ؼ� �־��ش�.
	m_Middata[datanum].index += m_Middata[datanum].updateunit;
}

void CAnimationQue::High_Update( int datanum, DWORD delta )
{
	if( m_Highdata[datanum].delay > 0 ) m_Highdata[datanum].delay -= 1;
	if( m_Highdata[datanum].delay != 0 ) return;
	
	//�ִϸ��̼��� ������ ã�ƿ´�.
	int sprindex = m_Highdata[datanum].kind;
	if( sprindex < 0 || sprindex >= ANIMATION_MAX ) return;

	int kind = m_Highdata[datanum].kind;
//	POINT pt = m_Highdata[datanum].pos;

	//���带 ������ �Ѵ�
	if( m_Highdata[datanum].soundsnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayWav( m_Highdata[datanum].soundsnd );
		m_Highdata[datanum].soundsnd = NULL;
	}

		//������ ������ �Ѵ�.
	if( m_Highdata[datanum].voicesnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayOggStream( m_Highdata[datanum].voicesnd, FALSE );
		m_Highdata[datanum].voicesnd = NULL;
	}

	switch( kind )
	{

	case ANIMATION_REGAMENOTICE:
		{						
			if( m_AniSpr[sprindex].IsSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{				
				Clear( datanum,  kind );
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;
			}
		}break;


	case ANIMATION_ALLIN:	//���ι���ȿ��	
		{
			//��Ƽ��
			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{	
				Clear( datanum,  kind );
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;
			}
		}break;

		//������ �ִϸ��̼��� ������ �ִ´�.
	case ANIMATION_ETCEFFECT:
	case ANIMATION_WINLOSS:
	case ANIMATION_CARDGROWTH_EFT:
		{
			if( kind == ANIMATION_WINLOSS && m_Highdata[datanum].scene == 2 )
			{
				int a = 0;
			}
			//��Ƽ��
			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{	
				//�а� �������� ȿ�� �ڿ��� ī�带 ������ ī��� �ٲ��ش�.
				if( kind == ANIMATION_WINLOSS && m_Highdata[datanum].scene == 1 && m_Highdata[datanum].option != -1 )
				{
					//Play[m_Highdata[datanum].option].SetLoser(TRUE);//$$$
					g_PLAYER(m_Highdata[datanum].option)->SetCardTornOut(TRUE); //������ ī�� ����
				}

				Clear( datanum,  kind );
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;

				if( m_Highdata[datanum].nowtime >= 500 &&  m_Highdata[datanum].option2 == 1 )
				{
					m_Highdata[datanum].option2 = -1;
					NMBASE::SOUND::g_Sound.PlayWav(SND43);
				}
			}
		}break;
		
	case ANIMATION_POP2:
		{
			//��
			if( m_Highdata[datanum].option != 1 && m_AniSpr[sprindex].IsSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{				
				Clear( datanum,  kind );
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;
			}
		}break;

	//����ī�庸��
	case ANIMATION_HIDDEN:
		{
			static BOOL bPlay = FALSE;

			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{				
				//Game.SetJjogiMsg(); //ī�带 ��ôٰ� ������ ����//$$$$$$
				g_GAMEPROCESS()->OnCompleteJJogiAnimation();
				Clear( datanum,  kind );
				
				bPlay = FALSE;
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;
				
				if( m_Highdata[datanum].nowtime >= 1500 && bPlay == FALSE)
				{
					bPlay = TRUE;
					
// 					Game.m_Jjogi.bClick = TRUE;	//$$$$$$
// 					Game.JjogiEnd();//�ɱ⸦ ������.	//$$$$$$
					NMBASE::SOUND::g_Sound.PlayWav( SND01 );	//�ɱ� ȿ����
					
					//���̻� 
					if( m_Highdata[datanum].option2 == 1 )
					{
						NMBASE::SOUND::g_Sound.PlayWav(SND02_1);		//FX(ȿ����)

						if(!g_ObserverMan.IsMyObserver())
							SutdaGameSound().OnActionVoice( AV_MAKE_HIDDEN, 0, 2);	//���̽�
					}
					//��������
					else if( m_Highdata[datanum].option2 == 2)
					{
						NMBASE::SOUND::g_Sound.PlayWav(SND02_2);		//FX(ȿ����)
						if(!g_ObserverMan.IsMyObserver())
							SutdaGameSound().OnActionVoice( AV_FAIL_HIDDEN, 0, 2); //���̽�
					}
				}
			}
		}break;
	}
}
//############### �ִϸ��̼��� ������Ʈ�ϴ� �� ################//

//################ �ִϸ��̼��� �׷��ִ� �κ� #################//
void CAnimationQue::Low_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC)
{
	if( page == NULL ) return;
	
	POINT pt;

	for( int i = 0; i < ANIMATION_MAX; i++)
	{	
		if( m_Lowdata[i].kind == 0 || m_Lowdata[i].delay > 0 ) continue;

		pt = m_Lowdata[i].pos;
		
		int sprindex = m_Lowdata[i].kind;
		int index = (int)m_Lowdata[i].index;
		int total = m_AniSpr[sprindex].GetTotalSpr();

// 		switch( m_Lowdata[i].kind )
// 		{
// 		case ANIMATION_JOKBOHELP:	//������������
// 			{
// 				page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Lowdata[i].scene, m_Lowdata[i].nowtime, FALSE);
// 			}break;
// 
// 		case ANIMATION_BET:
// 			{
// 				//���ι����� ��Ƽ������ �Ϲݹ����� ������ 
// 				if( m_Lowdata[i].option2 == 1)	
// 					page->DrawMultiScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Lowdata[i].scene, m_Lowdata[i].nowtime, TRUE);
// 				else if(m_Lowdata[i].option2 == -1)	
// 					page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Lowdata[i].scene, m_Lowdata[i].nowtime, TRUE);
// 			}break;
// 		}
	}
}

void CAnimationQue::Mid_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC )
{
	if( page == NULL ) return;
	
	POINT pt;
	
	for( int i = 0; i < ANIMATION_MAX; i++ )
	{
		if( m_Middata[i].kind == 0 || m_Middata[i].delay > 0 ) continue;
		pt = m_Middata[i].pos;
		
		int sprindex = m_Middata[i].kind;
		int index = (int)m_Middata[i].index;
		int total = m_AniSpr[sprindex].GetTotalSpr();
		
		switch(m_Middata[i].kind)
		{
		//### [�������] ###//
		//### �ѹ��� �÷��̵ȴ� ###//		
		case ANIMATION_CARDGROWTH:	//ī��Ŀ����ȿ�� 	
			{
				page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Middata[i].scene, m_Middata[i].nowtime, FALSE);				
				
			}break;


			//��Ƽ��
		case ANIMATION_RESULT_EFT:
			{
				page->DrawMultiScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Middata[i].scene, m_Middata[i].nowtime, FALSE);
			}break;


		//�����尪�ϰ��		
		default:
			{
				if( index > total)
				{
					Clear( i , m_Middata[i].kind );
					continue;
				}

				if( sprindex >= 0 && sprindex <= ANIMATION_MAX ) page->PutSprAuto( pt.x, pt.y, &m_AniSpr[sprindex], index);	
			}break;
		}

	}

}

void CAnimationQue::High_Draw( NMBASE::GRAPHICGDI::CPage* page, CDC* pDC)
{
	if( page == NULL ) return;
	
	POINT pt;
	
	for( int i = 0; i < ANIMATION_MAX; i++)
	{		
		if( m_Highdata[i].kind == 0 || m_Highdata[i].delay > 0 ) continue;
	
		pt = m_Highdata[i].pos;
		
		int sprindex = m_Highdata[i].kind;
		int index = (int)m_Highdata[i].index;
		int total = m_AniSpr[sprindex].GetTotalSpr();

		switch( m_Highdata[i].kind )
		{


		//��Ƽ��
		case ANIMATION_ALLIN:	//���ι���ȿ��
		case ANIMATION_ETCEFFECT:
		case ANIMATION_WINLOSS:
		case ANIMATION_CARDGROWTH_EFT:
			{
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );
			}break;
		
		//��
		case ANIMATION_REGAMENOTICE:
			{
				page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE);
			}break;

		case ANIMATION_POP2:
			{
				if( m_Highdata[i].option != 1)
					page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE);
				else
					page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, TRUE);
			}break;

			//����ī�庸��
		case ANIMATION_HIDDEN:
			{
				page->SetDrawingTrackScope(-1, 0);
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );
			
				page->SetDrawingTrackScope(1, 1);
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );

				//������ ī��� �����ش�.
				NMBASE::GRAPHICGDI::DRAWINGCELLDESC desc;
				if(page->GetMSceneDrawingCellInfo(pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, 1, 0, m_Highdata[i].nowtime, FALSE, &desc))
				page->PutSprAuto( (int)desc.xp, (int)desc.yp, &m_AniSpr[ANIMATION_CARDGROWTH], m_Highdata[i].option);


				page->SetDrawingTrackScope(2, -1);
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );

				page->SetDrawingTrackScope(-1, -1);
			}break;
		}
	}
}
