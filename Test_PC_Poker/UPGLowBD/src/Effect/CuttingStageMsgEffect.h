#pragma once

#include "../AnimationDefine.h"	//UPG작업 추가

namespace effect
{

//컷팅을 알려주는 메세지 
class CCuttingStageMsgEffect : public CBaseObject // 바꿀 카드를 선택해주세요
{
public:	

	CCuttingStageMsgEffect();
	virtual ~CCuttingStageMsgEffect();

	int		nChangeType;	//1 아침, 2 점심 3 저녁
	NMBASE::SKINGDI::CNScene *m_pScene;
	DWORD	nDelayTime;	

	void Init(int changetype );
	virtual void DrawObject(CDC *pDC);
	virtual void Destroy();
	virtual int  ProcessObject();
};


}



