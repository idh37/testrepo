#pragma once
#include "SpriteList.h"

enum eNUM_EFFECT{
	eNUM_EFFECT_NULL = -1,
	eNUM_EFFECT_THANK = 0,
	eNUM_EFFECT_THANK_BAK,
	eNUM_EFFECT_STOP,
	eNUM_EFFECT_STOP_BAK,		
	eNUM_EFFECT_JOKER_BAK,
	eNUM_EFFECT_SEVEN_BAK,
	eNUM_EFFECT_REGIST_BAK,
	eNUM_EFFECT_JOKER_SEVEN_BAK,
	eNUM_EFFECT_SEVEN_REIGIST_BAK,
	eNUM_EFFECT_REIGIST_JOKER_BAK,
	eNUM_EFFECT_JOKER_SEVEN_REIGIST_BAK,
	eNUM_EFFECT_ALLIN,
	eNUM_EFFECT_MAX
};

class CEffectDraw
{
	//이펙트는 기본적으로 멀티씬
public:
	CEffectDraw(void);
	~CEffectDraw(void);

	void SetAnimation( eNUM_EFFECT eType, int nPnum, NMBASE::GRAPHICGDI::CPage *pPage, CPoint pt );
	void PlayAnimation();
	void StopAnimation();
	void Draw(CDC *pDC);
	BOOL IsAnimationEnd();

protected:
	NMBASE::GRAPHICGDI::CPage				*m_pPage;
	NMBASE::GRAPHICGDI::xSprite				*m_pSpr;
	CPoint									m_pt;
	eNUM_EFFECT								m_eEffectType;
	int										m_nPnum;	
	BOOL									m_bPlay;
	int										m_nSceneNum;
	DWORD									m_dwStartTime;
	DWORD									m_dwEndTime;
	DWORD									m_dwTime;
	BOOL									m_bPlaySceneSound;
};