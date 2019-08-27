#pragma once

#include "../AnimationDefine.h"	//UPG작업 추가

namespace effect
{

class CMadeEffect :public CBaseObject
{
public:

	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;
	DWORD nDelay;	

	CMadeEffect();
	virtual ~CMadeEffect();

	BOOL Init();

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);

protected:
	NMBASE::GRAPHICGDI::xSprite* m_pSpr;

};

}


