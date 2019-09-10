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
			int		bScenePlay;				//! �� �÷��� �Ǵ°�
			DWORD	nSceneTime;				//! �� ����  ���� Ÿ��
			DWORD	nStartSceneTime;		//! �� ���� ���� �÷��� Ÿ��
			int		nLoop;					//! �� ���� Ƚ��

			BOOL	bLoop;					//! ��� �ݺ�����

			DWORD	nCurPlayTime;			//! ���� �÷��� �Ǵ� Ÿ��(���� ���޵Ǵ� ����)
			DWORD	nDeleyTime;				//������
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
