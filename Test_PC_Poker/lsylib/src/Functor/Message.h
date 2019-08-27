#pragma once

#include <string>
#include "../Utility/Point.h"
#include "../Utility/Rect.h"

namespace LSY
{
enum ENUM_MESSAGE
{
	EM_UNKNOWN		  = -1,
	//Object 관련 메시지
	EM_O_LBUTTONDOWN  = 0,		//마우스 왼쪽 버튼이 눌릴때 보내지는 메시지
	EM_O_LBUTTONUP,				//마우스 왼쪽 버튼이 올라올때 보내지는 메시지
	EM_O_LBUTTONDBCLICK,		//마우스 왼쪽 버튼이 더블 클릭되었을때 보내지는 메시지
	EM_O_RBUTTONDOWN,			//마우스 오른쪽 버튼이 눌릴때 보내지는 메시지
	EM_O_RBUTTONUP,				//마우스 오른쪽 버튼이 올라올때 보내지는 메시지
	EM_O_RBUTTONDBCLICK,		//마우스 오른쪽 버튼이 더블 클릭되었을때 보내지는 메시지
	EM_O_MBUTTONDOWN,			//마우스 중간 버튼이 눌릴때 보내지는 메시지
	EM_O_MBUTTONUP,				//마우스 중간 버튼이 올라올때 보내지는 메시지
	EM_O_MBUTTONDBCLICK,		//마우스 중간 버튼이 더블 클릭되었을때 보내지는 메시지
	EM_O_MOUSEMOVE,				//MouseOver 또는 클릭으로 Select된 Object에 Mouse가 움직일때 보내지는 메시지
	EM_O_MOUSEWHEEL,			//마우스 Wheel Focus를 가진 Object에 마우스의 휠이 돌아갔을 때 보내지는 메시지
	EM_O_MOUSEOVER,				//Object 밖에 있던 마우스커서가 Object로 들어왔을때 보내지는 메시지
	EM_O_MOUSELEAVE,			//Object 위에 있던 마우스커서가 Object를 벋어났을때 보내지는 메시지
	EM_O_MOUSELCLICK,			//마우스 왼쪽 버튼이 클릭되었을 때 보내지는 메시지
	EM_O_MOUSERCLICK,			//마우스 오른쪽 버튼이 클릭되었을 때 보내지는 메시지
	EM_O_MOUSEMCLICK,			//마우스 가운데 버튼이 클릭되었을 때 보내지는 메시지
	EM_O_FOCUSIN,				//Object가 Focus를 얻었을 때 보내지는 메시지
	EM_O_FOCUSOUT,				//Object가 Focus를 잃을때 보내지는 메시지
	EM_O_KEYDOWN,				//키보드가 눌릴때 보내지는 메시지
	EM_O_KEYUP,					//키보드가 눌린상태에서 키는 땔때 보내지는 메시지
	EM_O_TAPDOWN,				//키보드 입력중 Tab키가 입력되었을때 보내지는 메시지
	EM_O_RETURN,				//키보드 입력중 Enter키가 입력되었을때 보내지는 메시지
	EM_O_CHECKCHANGE,			//CheckButton, CheckBox, RadioButton의 체크 상태가 변경될때 보내지는 메시지
	EM_O_DRAG_BEGIN,			//Drag 가능한 Object의 Drag가 시잘될때 보내지는 메시지
	EM_O_DRAGING,				//Drag 가능한 Object의 Drag중일때 보내지는 메시지
	EM_O_DRAG_END,				//Drag 가능한 Object의 Drag가 끝날때 보내지는 메시지
	EM_O_CURVALUECHANGE,		//스크롤 바의 값이 변경될때 보내지는 메시지
	EM_O_CLICKUP,				//스크롤 바의 Up버튼이 클릭되었을때 보내지는 메시지
	EM_O_CLICKDOWN,				//스크롤 바의 Down버튼이 클릭되었을때 보내지는 메시지
	EM_O_CLICKPAGEUP,			//스크롤 바에서 BAR의 PageUp이 호출될때 보내지는 메시지
	EM_O_CLICKPAGEDOWN,			//스크롤 바에서 BAR의 PageDown이 호출될때 보내지는 메시지
	EM_O_SELCHANGE,				//List, TextList, Radio에서 Select된 아이템이 변경될 때 보내지는 메시지
	EM_O_KEYMESSAGEPROC,		//키보드 이벤트들이 오면 보내지는 메시지
	EM_HOTKEY,					//CObjectMan에서 사용하며, Hotkey의 메시지를 전달
	EM_O_LOCALCHILDNOTIFY,		//Local Child의 모든 Notify는 Parent를 통해서 이 메시지로 Functor에 등록된 곳으로 전달한다.
	EM_O_ANIEND,				//Sprite Animation이 종료 되었을때 보내지는 메시지
	EM_O_ANISTART,				//Sprite Animation이 시작  되었을때 보내지는 메시지(Delay후 실제로 시작될때 보내짐)
	EM_O_ANIREPEAT,				//Sprite Animation이 Repeat될때 마다 보내지는 메시지
	EM_O_ANISOUND,				//Sprite Animation의 사운드가 재생 되어야 할 시점에 보내지는 메시지
	EM_O_ANILOGIC,				//Sprite Animation의 Logic이 호출될때 마다 한번씩 호출되는 메시지
	EM_O_MOVESTART,				//Mover를 통한 이동이 시작 되었을 때 전달됨
	EM_O_MOVEEND,				//Mover를 통한 이동이 완료 되었을 때 전달됨
	EM_O_LBUTTONDOWNITEM,		//Rich Edit에서 lParam이 등록된 Item에 L버튼이 눌렸을때 호출되는 메시지
	EM_O_RBUTTONDOWNITEM,		//Rich Edit에서 lParam이 등록된 Item에 L버튼이 눌렸을때 호출되는 메시지
	EM_TIMER,					//등록된 Timer의 시간에 도달했을때 보내지는 메시지
	EM_SETCURSOR,				//커서를 셋팅할 타이밍에 보내지는 메시지
	EM_DRAG_BEGIN,				//마우스 이벤트의 드래그 시작을 Observer에 알려주는 메지시(CObjectMan에서 사용)
	EM_DRAGING,					//마우스 이벤트의 드래그 중일때 Observer에 알려주는 메지시(CObjectMan에서 사용)
	EM_DRAG_END,				//마우스 이벤트의 드래그 종료를 Observer에 알려주는 메지시(CObjectMan에서 사용)
	EM_OBJECT_CREATE,			//프로젝트 로딩시 Object생성전에 사용자 제작 Object가 있는지 확인하는 CallBack 함수
	EM_OBJECT_CREATED,			//프로젝트 로딩시 Object생성후에 Object가 생성 되었을때 호출해주는 CallBack함수
	EM_O_LISTITEM_CLICK,		//리스트의 리스트 아이템을 클릭 하였을 때 발생하는 메시지
	EM_O_LISTITEM_DBCLICK,		//리스트의 리스트 아이템을 더블 클릭하였을 때 발생하는 메시지
	EM_O_LISTITEM_RCLICK,		//리스트의 리스트 아이템에서 R버튼을 클릭하였을 때 발생하는 메시지
	EM_O_LISTITEM_RDBCLICK,		//리스트의 리스트 아이템에서 R버튼을 더블 클릭하였을 때 발생하는 메시지
	EM_O_LISTITEM_MOUSEOVER,	//리스트의 리스트 아이템 밖에 있던 마우스커서가 리스트 아이템 안으로 들어왔을때 보내지는 메시지	[12/14/2011 guesty]
	EM_O_LISTITEM_MOUSELEAVE,	//리스트의 리스트 아이템 위에 있던 마우스커서가 리스트 아이템을 벋어났을때 보내지는 메시지 [12/14/2011 guesty]
	EM_O_CLICKOUTSIDE,			//외부 클릭이 일어났을때 호출되는 메시지, Traking을 하고 있을때만 호출된다.
	EM_WINDOW_MOUSEOVER,		//마우스가 윈도우에 들어왔을 때 호출되는 메시지
	EM_WINDOW_MOUSELEAVE,		//마우스가 윈도우를 벋어났을때 호출되는 메시지
	EM_O_SETTXTLINECOUNT,		//텍스트의 라인의 값이 변경될때 호출되는 메시지
};

class UPG_LSYLIB_EXPORT CMessage
{
protected:
	int				m_nType;
public:
	CMessage(){}
	virtual ~CMessage(){}

	const int &GetID(void){ return m_nType; }
};

#define MESSAGE_EVENT_ID(m, id) \
class UPG_LSYLIB_EXPORT m : public CMessage\
{\
public:\
	m(){m_nType = id;}\
}

class CObjectBase;
class UPG_LSYLIB_EXPORT CObjectMessage : public CMessage
{
protected:
	CObjectBase		*m_pObject;
public:
	CObjectMessage(CObjectBase *pObject):m_pObject(pObject){}
	virtual ~CObjectMessage(){}

	CObjectBase *GetObject(void){return m_pObject;}
};

#define MESSAGE_CREATE_MOUSE_EVENT_ID(m, id) \
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	CPoint			m_ptPos;\
public:\
	m(CObjectBase *pObject, const CPoint &ptPos):CObjectMessage(pObject),m_ptPos(ptPos){m_nType = id;}\
	const CPoint &GetPos(void){return m_ptPos;}\
}

#define MESSAGE_CREATE_EVENT_ID(m, id) \
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
public:\
	m(CObjectBase *pObject):CObjectMessage(pObject){m_nType = id;}\
}

#define MESSAGE_CREATE_BOOL_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	bool			m_bValue;\
public:\
	m(CObjectBase *pObject, const bool &bValue):CObjectMessage(pObject), m_bValue(bValue){m_nType = id;}\
	const bool		&GetValue(void){return m_bValue;}\
}

#define MESSAGE_CREATE_INT_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	int				m_nValue;\
public:\
	m(CObjectBase *pObject, const int &nValue):CObjectMessage(pObject), m_nValue(nValue){m_nType = id;}\
	const int		&GetValue(void){return m_nValue;}\
}

#define MESSAGE_CREATE_FLOAT_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	float			m_fValue;\
public:\
	m(CObjectBase *pObject, const float &fValue):CObjectMessage(pObject), m_fValue(fValue){m_nType = id;}\
	const float		&GetValue(void){return m_fValue;}\
}

#define MESSAGE_CREATE_DWORD_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	DWORD			m_dwValue;\
public:\
	m(CObjectBase *pObject, const DWORD &dwValue):CObjectMessage(pObject), m_dwValue(dwValue){m_nType = id;}\
	const DWORD		&GetValue(void){return m_dwValue;}\
}

#define MESSAGE_CREATE_OBJECT_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	CObjectBase		*m_pValue;\
public:\
	m(CObjectBase *pObject, CObjectBase *pValue):CObjectMessage(pObject), m_pValue(pValue){m_nType = id;}\
	CObjectBase		*GetValue(void){return m_pValue;}\
}

#define MESSAGE_CREATE_STRING_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	std::string		m_strValue;\
public:\
	m(CObjectBase *pObject, LPCTSTR strValue):CObjectMessage(pObject), m_strValue(strValue){m_nType = id;}\
	const std::string &GetValue(void){return m_strValue;}\
}

class UPG_LSYLIB_EXPORT CMO_MouseWheel : public CObjectMessage
{
protected:
	int			m_nKey;
	short		m_zDelta;
public:
	CMO_MouseWheel(CObjectBase *pObject, const int &nKey, const short &zDelta):CObjectMessage(pObject), m_nKey(nKey), m_zDelta(zDelta){m_nType = EM_O_MOUSEWHEEL;}
	const int	&GetKey(void){return m_nKey;}
	const short	&GetDelta(void){return m_zDelta;}
};

class UPG_LSYLIB_EXPORT CMO_KeyMessageProc : public CObjectMessage
{
protected:
	UINT		m_nMessage;
	WPARAM		m_wParam;
	LPARAM		m_lParam;
public:
	CMO_KeyMessageProc(CObjectBase *pObject, const UINT &nMessage, const WPARAM &wParam, const LPARAM &lParam)
		:CObjectMessage(pObject), m_nMessage(nMessage), m_wParam(wParam), m_lParam(lParam){m_nType = EM_O_KEYMESSAGEPROC;}
	const UINT		&GetMessage(void){return m_nMessage;}
	const WPARAM	&GetWParam(void){return m_wParam;}
	const LPARAM	&GetLParam(void){return m_lParam;}
};

class UPG_LSYLIB_EXPORT CMO_HotKey : public CObjectMessage
{
protected:
	bool		m_bPrevious;	//반복해서 키보드가 눌렸는지 여부
	int			m_nScanCode;	//눌린키보드의 스캔코드
	bool		m_bControl;		//컨트롤키가 눌렸는지의 여부
	bool		m_bAlt;			//알트키가 눌렸는지의 여부
public:
	CMO_HotKey(CObjectBase *pObject, const int &nScanCode, const bool &bControl, const bool &bAlt, const bool bPrevious)
		:CObjectMessage(pObject), m_nScanCode(nScanCode), m_bControl(bControl), m_bAlt(bAlt), m_bPrevious(bPrevious){m_nType = EM_HOTKEY;}
	const int	&GetScanCode(void){return m_nScanCode;}
	const bool	&IsPushControl(void){return m_bControl;}
	const bool	&IsPushAlt(void){return m_bAlt;}
	const bool	&IsPrevious(void){return m_bPrevious;}
};

class UPG_LSYLIB_EXPORT CMO_LocalChildNotify : public CObjectMessage
{
protected:
	CObjectMessage	*m_pMsessage;		//Local Child에서 전달된 메시지로 펑터에 등록된곳으로 전달해줄 메시지
public:
	CMO_LocalChildNotify(CObjectBase *pObject, CObjectMessage *pMsessage)
		:CObjectMessage(pObject), m_pMsessage(pMsessage){m_nType = EM_O_LOCALCHILDNOTIFY;}
	CObjectMessage *GetMessage(void){return m_pMsessage;}
};

class UPG_LSYLIB_EXPORT CMO_AniSound : public CObjectMessage
{
protected:
	std::string		m_strPath;
	int				m_nVolume;
public:
	CMO_AniSound(CObjectBase *pObject, LPCTSTR lpPath, const int &nVolume)
		:CObjectMessage(pObject), m_strPath(lpPath), m_nVolume(nVolume){m_nType = EM_O_ANISOUND;}
	const std::string &GetPath(void){return m_strPath;}
	const int &GetVolume(void){return m_nVolume;}
};

class CMover;
class UPG_LSYLIB_EXPORT CMoverMessage : public CMessage
{
protected:
	CMover		*m_pMover;
public:
	CMoverMessage(CMover *pMover):m_pMover(pMover){}
	virtual ~CMoverMessage(){}

	CMover *GetMover(void){return m_pMover;}
};

#define MESSAGE_MOVER_EVENT_ID(m, id) \
class UPG_LSYLIB_EXPORT m : public CMoverMessage\
{\
public:\
	m(CMover *pMover):CMoverMessage(pMover){m_nType = id;}\
}

class UPG_LSYLIB_EXPORT CMO_Timer : public CMessage
{
protected:
	int				m_nID;
	int				m_nCurRepeat;
	int				m_nRepeat;
public:
	CMO_Timer(const int &nID, const int &nCurRepeat, const int &nRepeat):m_nID(nID),m_nCurRepeat(nCurRepeat),m_nRepeat(nRepeat){m_nType = EM_TIMER;}
	virtual ~CMO_Timer(){}

	const int &GetID(void){return m_nID;}
	const int &GetCurRepeat(void){return m_nCurRepeat;}
	const int &GetRepeat(void){return m_nRepeat;}
};

class CRichItem;

#define MESSAGE_RICHEDIT_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	CPoint				m_ptLocal;\
	CPoint				m_ptGlobal;\
	CRect				m_rtItem;\
	CRichItem			*m_pItem;\
public:\
	m(CObjectBase *pObject, CRichItem *pItem, const CPoint &ptLocal, const CPoint &ptGlobal, const CRect &rtItem)\
	:CObjectMessage(pObject),m_pItem(pItem),m_ptLocal(ptLocal),m_ptGlobal(ptGlobal),m_rtItem(rtItem){m_nType = id;}\
	CRichItem *GetItem(void){return m_pItem;}\
	const CPoint &GetLocalPos(void){return m_ptLocal;}\
	const CPoint &GetGlobalPos(void){return m_ptGlobal;}\
	const CRect &rtRect(void){return m_rtItem;}\
}

class UPG_LSYLIB_EXPORT CMO_SetCursor : public CMessage
{
protected:
	CObjectBase		*m_pOverObject;
	CObjectBase		*m_pSelectObject;
public:
	CMO_SetCursor(CObjectBase *pOverObject, CObjectBase *pSelectObject):CMessage(), m_pOverObject(pOverObject), m_pSelectObject(pSelectObject){m_nType = EM_SETCURSOR;}
	virtual ~CMO_SetCursor(){}

	CObjectBase *GetOverObject(void){return m_pOverObject;}
	CObjectBase *GetSelectObject(void){return m_pSelectObject;}
};

#define MESSAGE_OBSERVER_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CMessage\
{\
protected:\
	CPoint				m_ptPos;\
	bool				m_bRet;\
public:\
	m(const CPoint &ptPos)\
	:CMessage(),m_ptPos(ptPos),m_bRet(false){m_nType = id;}\
	const CPoint &GetPos(void){return m_ptPos;}\
	const bool &IsRet(void){return m_bRet;}\
	void SetRet(const bool &bRet){m_bRet = bRet;}\
}

class UPG_LSYLIB_EXPORT CMO_CreateObject : public CMessage
{
protected:
	int				*m_pID;
	std::string		*m_pStrName;
public:
	CMO_CreateObject(int *pID, std::string *pStrName):CMessage(), m_pID(pID), m_pStrName(pStrName){m_nType = EM_OBJECT_CREATE;}
	virtual ~CMO_CreateObject(){}

	int *GetID(void){return m_pID;}
	std::string *GetName(void){return m_pStrName;}
};

class CListItemData;

#define MESSAGE_LISTITEM_EVENT_ID(m, id)\
class UPG_LSYLIB_EXPORT m : public CObjectMessage\
{\
protected:\
	CListItemData		*m_pItemData;\
public:\
	m(CObjectBase *pObject, CListItemData *pItemData)\
	:CObjectMessage(pObject),m_pItemData(pItemData){m_nType = id;}\
	CListItemData *GetItemData(void){return m_pItemData;}\
}

//=== 마우스 이벤트 관련 클래스==============================================
//Object LButton 관련 클래스
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_LButtonDown		, EM_O_LBUTTONDOWN		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_LButtonUp		, EM_O_LBUTTONUP		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_LButtonDbClick	, EM_O_LBUTTONDBCLICK	);
//Object RButton 관련 클래스
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_RButtonDown		, EM_O_RBUTTONDOWN		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_RButtonUp		, EM_O_RBUTTONUP		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_RButtonDbClick	, EM_O_RBUTTONDBCLICK	);
//Object MButton 관련 클래스
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MButtonDown		, EM_O_MBUTTONDOWN		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MButtonv			, EM_O_MBUTTONUP		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MButtonDbClick	, EM_O_MBUTTONDBCLICK	);
//Object MouseMove 관련 클래스
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MouseMove		, EM_O_MOUSEMOVE		);
//Object Click 관련 클래스
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MouseLClick		, EM_O_MOUSELCLICK		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MouseRClick		, EM_O_MOUSERCLICK		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_MouseMClick		, EM_O_MOUSEMCLICK		);
//Object Mouse Over/Leave 관련 클래스
MESSAGE_CREATE_EVENT_ID		 ( CMO_MouseOver		, EM_O_MOUSEOVER		);
MESSAGE_CREATE_EVENT_ID		 ( CMO_MouseLeave		, EM_O_MOUSELEAVE		);
//Object Keyboard 관련 클래스
MESSAGE_CREATE_OBJECT_EVENT_ID	( CMO_FocusIn		, EM_O_FOCUSIN			);
MESSAGE_CREATE_OBJECT_EVENT_ID	( CMO_FocusOut		, EM_O_FOCUSOUT			);
MESSAGE_CREATE_EVENT_ID		 ( CMO_TabDown			, EM_O_TAPDOWN			);
MESSAGE_CREATE_EVENT_ID		 ( CMO_Return			, EM_O_RETURN			);
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_KeyDown			, EM_O_KEYDOWN			);
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_KeyUp			, EM_O_KEYUP			);
//CheckButton 관련 체크 이벤트 관련 클래스
MESSAGE_CREATE_BOOL_EVENT_ID ( CMO_CheckChange		, EM_O_CHECKCHANGE		);
//드래그 관련 클래스들
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_DragBegin		, EM_O_DRAG_BEGIN		);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_Draging			, EM_O_DRAGING			);
MESSAGE_CREATE_MOUSE_EVENT_ID( CMO_DragEnd			, EM_O_DRAG_END			);
//ScallBar에서 현재 값이 변경될때 호출되는 클래스
MESSAGE_CREATE_FLOAT_EVENT_ID( CMO_CurValueChange	, EM_O_CURVALUECHANGE	);
//ScallBar업, 다운 버튼이 클릭 되었을 때 호출되는 클래스
MESSAGE_CREATE_EVENT_ID		 ( CMO_ClickUp			, EM_O_CLICKUP			);
MESSAGE_CREATE_EVENT_ID		 ( CMO_ClickDown		, EM_O_CLICKDOWN		);
MESSAGE_CREATE_EVENT_ID		 ( CMO_ClickPageUp		, EM_O_CLICKPAGEUP		);
MESSAGE_CREATE_EVENT_ID		 ( CMO_ClickPageDown	, EM_O_CLICKPAGEDOWN	);
//아이템 선택이 변경되었을때 호울퇴는 클래스
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_SelChange		, EM_O_SELCHANGE		);
//스프라이트 Animation 관련 클래스들
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_AniEnd			, EM_O_ANIEND			);
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_AniStart			, EM_O_ANISTART			);
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_AniRepeat		, EM_O_ANIREPEAT		);
MESSAGE_CREATE_DWORD_EVENT_ID( CMO_AniLogic			, EM_O_ANILOGIC			);
MESSAGE_MOVER_EVENT_ID		 ( CMO_MoveStart		, EM_O_MOVESTART		);
MESSAGE_MOVER_EVENT_ID		 ( CMO_MoveEnd			, EM_O_MOVEEND			);
//Rich Text관련 클래스들
MESSAGE_RICHEDIT_EVENT_ID	 ( CMO_RichItemLButtonDown, EM_O_LBUTTONDOWNITEM );
MESSAGE_RICHEDIT_EVENT_ID	 ( CMO_RichItemRButtonDown, EM_O_RBUTTONDOWNITEM );

MESSAGE_OBSERVER_EVENT_ID	 ( CMO_Observer_DragBegin,	EM_DRAG_BEGIN		);
MESSAGE_OBSERVER_EVENT_ID	 ( CMO_Observer_Draging,	EM_DRAGING			);
MESSAGE_OBSERVER_EVENT_ID	 ( CMO_Observer_DragEnd,	EM_DRAG_END			);
//===========================================================================
MESSAGE_CREATE_EVENT_ID		 ( CMO_CreatedObject,		EM_OBJECT_CREATED	);
MESSAGE_LISTITEM_EVENT_ID	 ( CMO_ListItemClick,		EM_O_LISTITEM_CLICK	);
MESSAGE_LISTITEM_EVENT_ID	 ( CMO_ListItemDbClick,		EM_O_LISTITEM_DBCLICK);
MESSAGE_LISTITEM_EVENT_ID	 ( CMO_ListItemRClick,		EM_O_LISTITEM_RCLICK);
MESSAGE_LISTITEM_EVENT_ID	 ( CMO_ListItemRDbClick,	EM_O_LISTITEM_RDBCLICK);
MESSAGE_LISTITEM_EVENT_ID	 ( CMO_ListItemMouseOver,	EM_O_LISTITEM_MOUSEOVER);			// MouseOver 추가 [12/14/2011 guesty]
MESSAGE_LISTITEM_EVENT_ID	 ( CMO_ListItemMouseLeave,	EM_O_LISTITEM_MOUSELEAVE);			// MouseLeave 추가 [12/14/2011 guesty]
MESSAGE_CREATE_OBJECT_EVENT_ID ( CMO_ClickOutside,		EM_O_CLICKOUTSIDE);

MESSAGE_EVENT_ID			 ( CMO_WindowMouseOver,		EM_WINDOW_MOUSEOVER);
MESSAGE_EVENT_ID			 ( CMO_WindowMouseLeave,	EM_WINDOW_MOUSELEAVE);
MESSAGE_CREATE_INT_EVENT_ID	 ( CMO_SetTextLineCount,	EM_O_SETTXTLINECOUNT);
} //namespace LSY