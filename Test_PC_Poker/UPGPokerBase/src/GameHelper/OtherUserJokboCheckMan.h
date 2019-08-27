#pragma once

class UPG_GAMEBASE_EXPORT COtherUserJokboCheckMan
{
protected:
	NMBASE::GRAPHICGDI::xSprite		*m_pArrowSpr;
public:
	COtherUserJokboCheckMan();
	virtual ~COtherUserJokboCheckMan();

	virtual void Init(NMBASE::GRAPHICGDI::xSprite *pArrowSpr);
	virtual void Clear(void) = 0;
	virtual void Draw(CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage) = 0;
	virtual void CheckJokbo(void) = 0;
};