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

//스프라이트읽기
void CAnimationQue::LoadImage()
{

	
	//카드애니메이션(카드커지는거)
	AddLoadImageList(&m_AniSpr[ANIMATION_CARDGROWTH ], ".\\sutda\\data\\AniData\\CARDGROWTH.spr", true);

	//카드애니메이션(카드위에 효과들)
	AddLoadImageList(&m_AniSpr[ANIMATION_CARDGROWTH_EFT], ".\\sutda\\data\\AniData\\CARDGROWTH_EFT.spr", true);

	//족보결과에 따른 이펙트
	AddLoadImageList(&m_AniSpr[ANIMATION_RESULT_EFT], ".\\sutda\\data\\AniData\\Result_Eft.spr", true);
	
	//팝업창
	AddLoadImageList(&m_AniSpr[ANIMATION_POP2], ".\\sutda\\data\\pop\\Pop_2.spr", true);
	
	AddLoadImageList(&m_AniSpr[ANIMATION_REGAMENOTICE], ".\\sutda\\data\\pop\\RematchGameNotice.spr", true);
	
	//자동쪼기
	AddLoadImageList(&m_AniSpr[ANIMATION_HIDDEN], ".\\sutda\\data\\Anidata\\Hidden.spr", true);
	
	//결과 승패 애니메이션
	AddLoadImageList(&m_AniSpr[ANIMATION_WINLOSS], ".\\sutda\\data\\Anidata\\WinLose.spr", true);

	//땡잡이 암행어사 효과
	AddLoadImageList(&m_AniSpr[ANIMATION_ETCEFFECT], ".\\sutda\\data\\Anidata\\EtcEffect.spr", true);

	//올인표시
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
			//리셋을해줘서는 안되는 애니는 따로 놓는다.
// 			if( m_Middata[i].kind == ANIMATION_SMOKE || m_Middata[i].kind == ANIMATION_AUTOBET )
// 			{
// 				continue;
// 			}			
			
			memset( &m_Middata[i], 0, sizeof(m_Middata[i]) ); 
		}

		for( i = 0; i < ANIMATION_MAX; i++ )
		{
			/*
			//리셋을해줘서는 안되는 애니는 따로 놓는다.
			if(m_Highdata[i].kind == ANIMATION_AUTOBET)
			{
				continue;
			}
			*/
			memset( &m_Highdata[i], 0, sizeof(m_Highdata[i]) ); 
		}
	}
}

//중복된 애니메이션이 있는지 검사한다.
BOOL CAnimationQue::CheckAniData( int kind )
{

	int i;
	//## 어느레이어에 속하는지 알아본다. ##//
	int layer_kind = GetLayerkind( kind );
	if( layer_kind == -1 ) return FALSE;

	//## 비어있는 애니메이션 구조체를 찾는다. ##//
	GAMEANI_DATA* tempdata = NULL;	
	for(  i = 0; i < ANIMATION_MAX; i++)
	{
		if( layer_kind == 0 )
		{
			//같은 애니메이션이 있는지 찾아본다.
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

//어느 레이어에 속하는 애니메이션인지 알아본다.
int CAnimationQue::GetLayerkind( int kind)
{
	int layer_kind = -1;

	//하단에 깔리는 애니메이션 
	if( kind < ANIMATION_LOW)	
	{
		layer_kind = 0;
	}
	//중간에 깔리는 애니메이션 
	else if( kind > ANIMATION_LOW && kind < ANIMATION_HIGH)
	{
		layer_kind = 1;
	}
	//최상단에 깔리는 애니메이션 
	else if( kind > ANIMATION_HIGH && kind < ANIMATION_MAX )
	{
		layer_kind = 2;
	}

	return layer_kind;
}

void CAnimationQue::Ani_Insert( int kind, POINT pos, int option, int option2, int scene, BOOL disappeareft, char* soundsnd, char* voicesnd, int delay )
{
//	if( m_Ani_index < 0 || m_Ani_index >= ANIMATION_MAX) return;

	//## 어느레이어에 속하는지 알아본다. ##//
	int layer_kind = GetLayerkind( kind );
	if( layer_kind == -1 ) return;

	//## 비어있는 애니메이션 구조체를 찾는다. ##//
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
	
	//## 비어있는 데이타를 찾지 못했으면 리턴~ ##//
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

	//## 애니메이션 정보를 넘겨준다 ##//
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
		//### 새로운 애니 ###//

	//카드 커질때 뒷배경
	case ANIMATION_CARDGROWTH_EFT:
		{
			//tempdata->delay = 10;
		}break;
	//카드가 커질때는 카드를 숨겨준다.
	case ANIMATION_CARDGROWTH:
		{
		//	tempdata->pos.x = Play[option].MyCard[option2].Xp;
		//	tempdata->pos.y = Play[option].MyCard[option2].Yp;
			
		//	if( option2 == 1) tempdata->delay = 22;

			//카드를 안보이게 해준다.
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
	//## 어느레이어에 속하는지 알아본다. ##//
	int layer_kind = GetLayerkind( kind );
	if( layer_kind == -1 ) return;

	//## 비어있는 애니메이션 구조체를 찾는다. ##//
	GAMEANI_DATA* tempdata = NULL;	
	for( int i = 0; i < ANIMATION_MAX; i++)
	{
		if( layer_kind == 0 )
		{
			//같은 애니메이션이 있는지 찾아본다.
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
	
	//## 비어있는 데이타를 찾지 못했으면 리턴~ ##//
	if( tempdata == NULL ) return;


	//## 애니메이션 정보를 넘겨준다 ##//
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

//################## 애니메이션데이타를 지운다 ######################//
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

//################## 애니메이션을 멈춘다 ######################//
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
		//지워야할 애니메이션인지 검사한다.
//		if( m_Lowdata[i].kind != ani_kind ) continue;
	   
		//애니메이션이 추가되면 주석지우고 코드 추가 
// 		switch( m_Lowdata[i].kind )
// 		{
// 		case ANIMATION_JOKBOHELP:	//높은패설명
// 			{
// 				Clear(i, m_Lowdata[i].kind);	
// 			}break;
// 
// 		case ANIMATION_BET:		//배팅표시
// 			{
// 				//같은유저인지 체크한다.
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
		//지워야할 애니메이션인지 검사한다.
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
		//지워야할 애니메이션인지 검사한다.
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
				//Game.SetJjogiMsg(); //카드를 펴봤다고 서버에 전송
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

//############## 애니메이션을 업데이트하는 부분 ###############//
void CAnimationQue::Update()
{
	//[레볼루션]
	m_dwNowtime = timeGetTime();
	DWORD delta = m_dwNowtime - m_dwPrevtime;
	m_dwPrevtime = m_dwNowtime;


	for( int i = 0; i < ANIMATION_MAX; i++ )
	{
		//데이타가 들어있을 경우만 업데이트해준다.
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
	
	//사운드를 나오게 한다
	if( m_Lowdata[datanum].soundsnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayWav( m_Lowdata[datanum].soundsnd );
		m_Lowdata[datanum].soundsnd = NULL;
	}
	//음성을 나오게 한다.
	if( m_Lowdata[datanum].voicesnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayOggStream( m_Lowdata[datanum].voicesnd, FALSE );
		m_Lowdata[datanum].voicesnd = NULL;
	}

// 	switch( kind )
// 	{
// 	case ANIMATION_JOKBOHELP:	//높은족보설명
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
// 		//갱신할 애니메이션의 종류를 넣는다
// 	case ANIMATION_BET:
// 		{
// 			m_Lowdata[datanum].nowtime += delta;
// 			if( m_Lowdata[datanum].option2 == 1)
// 			{
// 				//멀티씬
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

		
	//사운드를 나오게 한다
	if( m_Middata[datanum].soundsnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayWav( m_Middata[datanum].soundsnd );
		m_Middata[datanum].soundsnd = NULL;
		//Sound.PlayWav( (SND_ENUM)m_Middata[datanum].soundsnd );
		//m_Middata[datanum].soundsnd = -1;
	}

	//음성을 나오게 한다.
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

					//게임결과에 따른 음성을 들려준다.
					//드롭패가 아닌경우는 정해놓은 사운드를 들려준다. 
					//재대결 보이스  관전이 아니고 포기하지 않은 상태일경우 
				
					//여기로왔으면 드롭패가 아니다.
					//if( !g_bWatch && strlen(Play[0].UI.ID) > 0 && Play[0].JoinState == 0/* || Play[0].LastState == GCMND_FOLD*/ )					
					if (g_ObserverMan.IsMyObserver()==FALSE)
					{					
						if ( strlen(g_PLAYER(0)->UI.ID) && g_PLAYER(0)->JoinState == EGAME_JOINOUT )
						{
							SutdaGameSound().OnActionVoice( AV_FAIL_HIDDEN, 0, 2); //보이스
						}
						else
						{
							BOOL bMyWin = (BOOL) m_Middata[datanum].option2;    //내가 이겼나?
							BOOL bMyFold = (BOOL) g_PLAYER(0)->bFold;    //내가 다이했나?
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

		//[레볼루션]
		//### 한번만 나오는 애니메이션 ###//
		case ANIMATION_CARDGROWTH:			//카드커지는효과 
			{
				if( m_AniSpr[sprindex].IsSceneTimeOver( m_Middata[datanum].scene, m_Middata[datanum].nowtime ) )
				{			
					//씬이 끝나면 카드를 다시 보이게 해준다.
					if(	m_Middata[datanum].kind == ANIMATION_CARDGROWTH )
					{
						//Play[ m_Middata[datanum].option ].MyCard[ m_Middata[datanum].option2 ].SetShow( TRUE );
						g_PLAYER(m_Middata[datanum].option)->ShowCard(m_Middata[datanum].option2, true);
					}

					Clear( datanum,  kind );
				}
				else
				{
					//카드를 가려준다.					
					if( m_Middata[datanum].option2 >= 0 )
					{
						
						//Play[m_Middata[datanum].option].MyCard[m_Middata[datanum].option2].SetShow( FALSE );
						g_PLAYER(m_Middata[datanum].option)->ShowCard(m_Middata[datanum].option2, false);
					
					}
					
					m_Middata[datanum].nowtime += delta;
				
				}
			}break;	
		
	}
	
	//레볼루션이 아닌경우 이미지 갱신을 위해서 넣어준다.
	m_Middata[datanum].index += m_Middata[datanum].updateunit;
}

void CAnimationQue::High_Update( int datanum, DWORD delta )
{
	if( m_Highdata[datanum].delay > 0 ) m_Highdata[datanum].delay -= 1;
	if( m_Highdata[datanum].delay != 0 ) return;
	
	//애니메이션의 종류를 찾아온다.
	int sprindex = m_Highdata[datanum].kind;
	if( sprindex < 0 || sprindex >= ANIMATION_MAX ) return;

	int kind = m_Highdata[datanum].kind;
//	POINT pt = m_Highdata[datanum].pos;

	//사운드를 나오게 한다
	if( m_Highdata[datanum].soundsnd != NULL )
	{
		NMBASE::SOUND::g_Sound.PlayWav( m_Highdata[datanum].soundsnd );
		m_Highdata[datanum].soundsnd = NULL;
	}

		//음성을 나오게 한다.
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


	case ANIMATION_ALLIN:	//올인배팅효과	
		{
			//멀티씬
			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{	
				Clear( datanum,  kind );
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;
			}
		}break;

		//갱신할 애니메이션의 종류를 넣는다.
	case ANIMATION_ETCEFFECT:
	case ANIMATION_WINLOSS:
	case ANIMATION_CARDGROWTH_EFT:
		{
			if( kind == ANIMATION_WINLOSS && m_Highdata[datanum].scene == 2 )
			{
				int a = 0;
			}
			//멀티씬
			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{	
				//패가 찢어지는 효과 뒤에는 카드를 찢어진 카드로 바꿔준다.
				if( kind == ANIMATION_WINLOSS && m_Highdata[datanum].scene == 1 && m_Highdata[datanum].option != -1 )
				{
					//Play[m_Highdata[datanum].option].SetLoser(TRUE);//$$$
					g_PLAYER(m_Highdata[datanum].option)->SetCardTornOut(TRUE); //찢어진 카드 연출
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
			//씬
			if( m_Highdata[datanum].option != 1 && m_AniSpr[sprindex].IsSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{				
				Clear( datanum,  kind );
			}
			else
			{
				m_Highdata[datanum].nowtime += delta;
			}
		}break;

	//히든카드보기
	case ANIMATION_HIDDEN:
		{
			static BOOL bPlay = FALSE;

			if( m_AniSpr[sprindex].IsMSceneTimeOver( m_Highdata[datanum].scene, m_Highdata[datanum].nowtime ) )
			{				
				//Game.SetJjogiMsg(); //카드를 펴봤다고 서버에 전송//$$$$$$
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
// 					Game.JjogiEnd();//쪼기를 끝낸다.	//$$$$$$
					NMBASE::SOUND::g_Sound.PlayWav( SND01 );	//쪼기 효과음
					
					//땡이상 
					if( m_Highdata[datanum].option2 == 1 )
					{
						NMBASE::SOUND::g_Sound.PlayWav(SND02_1);		//FX(효과음)

						if(!g_ObserverMan.IsMyObserver())
							SutdaGameSound().OnActionVoice( AV_MAKE_HIDDEN, 0, 2);	//보이스
					}
					//갑오이하
					else if( m_Highdata[datanum].option2 == 2)
					{
						NMBASE::SOUND::g_Sound.PlayWav(SND02_2);		//FX(효과음)
						if(!g_ObserverMan.IsMyObserver())
							SutdaGameSound().OnActionVoice( AV_FAIL_HIDDEN, 0, 2); //보이스
					}
				}
			}
		}break;
	}
}
//############### 애니메이션을 업데이트하는 끝 ################//

//################ 애니메이션을 그려주는 부분 #################//
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
// 		case ANIMATION_JOKBOHELP:	//높은족보설명
// 			{
// 				page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Lowdata[i].scene, m_Lowdata[i].nowtime, FALSE);
// 			}break;
// 
// 		case ANIMATION_BET:
// 			{
// 				//올인배팅은 멀티씬으로 일반배팅은 씬으로 
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
		//### [레볼루션] ###//
		//### 한번만 플레이된다 ###//		
		case ANIMATION_CARDGROWTH:	//카드커지는효과 	
			{
				page->DrawScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Middata[i].scene, m_Middata[i].nowtime, FALSE);				
				
			}break;


			//멀티씬
		case ANIMATION_RESULT_EFT:
			{
				page->DrawMultiScene(pt.x, pt.y, &m_AniSpr[sprindex], m_Middata[i].scene, m_Middata[i].nowtime, FALSE);
			}break;


		//디폴드값일경우		
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


		//멀티씬
		case ANIMATION_ALLIN:	//올인배팅효과
		case ANIMATION_ETCEFFECT:
		case ANIMATION_WINLOSS:
		case ANIMATION_CARDGROWTH_EFT:
			{
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );
			}break;
		
		//씬
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

			//히든카드보기
		case ANIMATION_HIDDEN:
			{
				page->SetDrawingTrackScope(-1, 0);
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );
			
				page->SetDrawingTrackScope(1, 1);
				page->DrawMultiScene( pt.x, pt.y, &m_AniSpr[sprindex], m_Highdata[i].scene, m_Highdata[i].nowtime, FALSE );

				//본인의 카드로 덮어준다.
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
