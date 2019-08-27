// NScene.h: interface for the CNScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSCENE_H__FDE11DD9_B803_42C1_8F91_FEB478B866DB__INCLUDED_)
#define AFX_NSCENE_H__FDE11DD9_B803_42C1_8F91_FEB478B866DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		typedef struct {
			int		bScenePlay;				//! 씬 플레이 되는가
			DWORD	nSceneTime;				//! 그 씬의  종료 타임
			DWORD	nStartSceneTime;		//! 그 씬의 시작 플레이 타임
			int		nLoop;					//! 씬 루프 횟수

			BOOL	bLoop;					//! 계속 반복인지

			DWORD	nCurPlayTime;			//! 현재 플레이 되는 타임(씬에 전달되는 인자)
			DWORD	nDeleyTime;				//딜레이
		} SCENE_STATUS;

		class NMSKINGDI_CLASS CNScene 
		{
		public:
			CNScene();
			CNScene(int x, int y, NMBASE::GRAPHICGDI::CPage* pPage=NULL, NMBASE::GRAPHICGDI::xSprite* pSpr=NULL, int nSceneNum=0);
			virtual ~CNScene();

		public:
			NMBASE::GRAPHICGDI::CPage				*m_pPage;
			int					m_Xp;
			int					m_Yp;
			int					m_nSceneNum;
			NMBASE::GRAPHICGDI::xSprite				*m_pSpr;
			SCENE_STATUS		m_stSceneState;	

		public:	
			void StartSceneLoop();

			void StopScene();

			void SetPostion(int x, int y );

			void StartScene(int nLoop=1 , DWORD deleytime = 0);	
			void ChangeSceneNum( int index = -1 );

			void SetSpriteFile(NMBASE::GRAPHICGDI::xSprite* pSpr);

			void OnTimer();
			BOOL DrawScene(CDC *pDC);
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_NSCENE_H__FDE11DD9_B803_42C1_8F91_FEB478B866DB__INCLUDED_)
