#pragma once

class UPG_LOBBY_EXPORT CRadGoldRoomListNormal : public LSY::CRadio
{
protected:
	CLobbyView *m_pLobbyView;

public:
	CRadGoldRoomListNormal(void);
	~CRadGoldRoomListNormal(void);
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);
	virtual bool OnMouseMove(const CPoint &ptPos); // ¾È¸ÔÈû ¤Ñ¤Ñ
};
