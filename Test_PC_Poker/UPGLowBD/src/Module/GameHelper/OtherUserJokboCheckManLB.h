#pragma once

class COtherUserJokboCheckManLB : public COtherUserJokboCheckMan
{
public:
	COtherUserJokboCheckManLB();
	virtual ~COtherUserJokboCheckManLB();

	virtual void Init(NMBASE::GRAPHICGDI::xSprite *pArrowSpr);
	virtual void Clear(void);
	virtual void Draw(CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void CheckJokbo(void);
};