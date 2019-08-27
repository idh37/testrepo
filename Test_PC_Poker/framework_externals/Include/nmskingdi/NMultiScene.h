// NMultiScene.h: interface for the CNMultiScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMULTISCENE_H__42D6D433_7BD4_40BB_843A_8D73788D491B__INCLUDED_)
#define AFX_NMULTISCENE_H__42D6D433_7BD4_40BB_843A_8D73788D491B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NScene.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMultiScene
		{
		public:
			CNMultiScene();
			CNMultiScene(int x, int y ,void *pParent = NULL, NMBASE::GRAPHICGDI::CPage* pPage=NULL, NMBASE::GRAPHICGDI::xSprite* pSpr=NULL, int nSceneNum=0);
			virtual ~CNMultiScene();

			BOOL				bShow;
			void				*m_pParent;
			NMBASE::GRAPHICGDI::CPage				*m_pPage;
			int					m_Xp;
			int					m_Yp;
			int					m_nSceneNum;
			NMBASE::GRAPHICGDI::xSprite				*m_pSpr;
			SCENE_STATUS		m_stSceneState;

			// 스크립트 쿼리
			NMBASE::GRAPHICGDI::QUERYMSCENE_SCRIPT scrquery;

		public:
			void StartMSceneLoop(int x, int y);
			void StartMSceneLoop();

			void ShowMScene(BOOL show){
				bShow = show;
			}
			void StopMScene(DWORD StopTime = 0);

			void SetPostion(int x, int y );

			void SetDrawIndex(int index );

			void StartMScene(int nLoop=1);
			void StartMScene(int x, int y, int nLoop=1);

			void SetSpriteFile(NMBASE::GRAPHICGDI::xSprite* pSpr);

			NMBASE::GRAPHICGDI::QUERYMSCENE_SCRIPT* OnTimer();
			BOOL DrawMScene(CDC *pDC);
		};

	} //namespace SKINGDI

}//namespace NMBASE


#endif // !defined(AFX_NMULTISCENE_H__42D6D433_7BD4_40BB_843A_8D73788D491B__INCLUDED_)
