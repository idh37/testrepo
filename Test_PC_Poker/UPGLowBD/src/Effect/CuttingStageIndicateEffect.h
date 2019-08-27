#pragma once

#include "../AnimationDefine.h"	//UPG작업 추가

namespace effect
{

// 아침 , 점심 , 저녁 표시
class CCuttingStageIndicateEffect :public CBaseObject
{
public:
	CCuttingStageIndicateEffect();
	virtual ~CCuttingStageIndicateEffect();	

	void	Init();	
	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};


}


