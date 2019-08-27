#pragma once

#include "../../Functor/MessageManager.h"
#include "../../Utility/Rect.h"
#include "../../Utility/Point.h"
#include <list>
#include "../../Define.h"
#include "IObject.h"

class NMBASE::GRAPHICGDI::CPage;
class TiXmlNode;

namespace LSY
{
class CObjectBase;
class CObjectProject;
class CWindows;
class IObject;
class CMover;
class CObjectRect;

typedef std::list<CObjectBase *>		LISTOBJECT;
typedef LISTOBJECT::iterator			LISTOBJECTITER;

class UPG_LSYLIB_EXPORT CObjectLocalData
{
public:
	CPoint		m_ptLocalPos;
	float		m_fLocalRotation;
	float		m_fLocalScaleX;
	float		m_fLocalScaleY;
	float		m_fLocalAlpha;
};

class UPG_LSYLIB_EXPORT CObjectBase : public IObject
{
protected:
	CPoint						m_ptPos;
	float						m_fRotation;
	float						m_fScaleX;
	float						m_fScaleY;
	CRect						m_rtRect;
	CRect						m_rtBound;

	CPoint						m_ptLocalPos;
	float						m_fLocalRotation;
	float						m_fLocalScaleX;
	float						m_fLocalScaleY;
	CRect						m_rtLocalRect;

	CPoint						m_ptMove;

	bool						m_bMessage;		//마우스, 키보드등의 이벤트가 발생할지의 여부

	bool						m_bLocalChild;	//외부에서 만들어지지 않고 Object에서 자체적으로 만들어 사용하는 Child인지의 여부

	LISTOBJECT					m_listLocalChild;	//외부에서 만들어지지 않고 Object에서 자체적으로 만들어 사용하는 Child 리스트
	LISTOBJECT					m_listChild;

	LISTOBJECTITER				m_itFind;
	bool						m_bLocalFind;

	CObjectBase					*m_pParent;
	CObjectRect					*m_pObjectRect;
	CObjectProject				*m_pProject;

	bool						m_bShow;
	bool						m_bShowRef;

	bool						m_bEnable;
	bool						m_bEnableRef;

	bool						m_bChange;		//위치, 배율, 회전이 변경 되었는지의 여부, 자신이 변경되지 않아도 부모가 변경되면 셋팅된다.
	bool						m_bChangeAlpha; //Alpha값이 변경 되었는지의 여부, 자신이 변경되지 않아도 부모가 변경되면 셋팅된다.

	bool						m_bFocus;		//현재 키보드 이벤트를 받는지 여부를 알려줌
	bool						m_bMouseOver;	//현재 마우스가 위에 있는지 여부

	ENUM_OBJECTEFFECTTYPE		m_nEffectType;
	bool						m_bXFlip;
	bool						m_bXFlipRef;
	bool						m_bYFlip;
	bool						m_bYFlipRef;
	int							m_nColor;

	bool						m_bFixed;
	bool						m_bDynamic;

	float						m_fAlpha;
	float						m_fLocalAlpha;
	int							m_nAlpha;

	CPoint						m_ptPivot;
	std::string					m_strName;
	int							m_nID;

	CRect						*m_pClipLocal;
	CRect						*m_pClipRect;

	bool						m_bLocalClipUseParentPos;	//로컬 클립핑 영역을 자신의 좌표가 아닌 부모의 좌표를 기준으로 설정하도록 한다.

	//드로우 관련 임시 변수
	DWORD						m_dwDrawOption;
	CPoint						m_ptDraw;
	float						m_fDrawRotaion;
	ENUM_OBJECTDRAWTYPE			m_nDrawType;
	bool						m_bClipDraw;		//클리핑 영역에 의해서 그려지는지의 여부(클리핑 영역이 0로인지 검사)를 지정
	bool						m_bDraw;			//클리핑 또는 화면과의 Bound에 의해서 그려지는지의 여부

	bool						m_bMouseMove;		//마우스로 움직일지 여부를 지정한다. m_bMessage가 true, m_bEnable가 true일때만 동작한다.
	CPoint						m_ptDragGab;
	bool						m_bDrag;
	bool						m_bUseLogic;
	bool						m_bUseMouseWheel;

	bool						m_bIgnoreParentClip;	//부모의 클리핑을 무시할지의 여부
	bool						m_bIgnoreParentPos;		//부모의 위치/배율/회전/기준점등을 무시할지의 여부

	//Clone하여 생성된 Object에서 원본 Object를 나타낸다. 
	//하지만 항상 Clone시 저장되는 것이 아니라 Clone된 Object의 Event Message를 원본 Object에 등록된 Functor를 통해서 전달할때 사용하게 된다.
	//Clone시 옵션으로 선택
	CObjectBase					*m_pOriginalObject;
	CObjectBase					*m_pObserverObject;		//복사시 복사되어 지는 모든 Object의 메시지를 감시하는 감시 Object
	CObjectBase					*m_pInterceptSelect;	//Object가 Select될때 가로챌 Object를 나타낸다. 이 Object가 Select되면 여기에 등록된 Object가 Select를 가로 챈다.

	//회전값에 따른 코사인값을 미리 계산해 둔다.
	float						m_fCos;
	float						m_fSin;

	CMover						*m_pMover;
	std::map<int, DWORD_PTR>	m_listUserData;
	bool						m_bExpand;
	bool						m_bTracking;
	bool						m_bRemove;

	//이벤트 그리기시 사용하는 임시 변수들
	bool						m_bEventDraw;
	CRect						m_rtOldDraw;
	bool						m_bTempEventDraw;
	bool						m_bSendEventDraw;
protected:
	void		SetParent(CObjectBase *pParent);
	void		SetLocalChild(bool bLocal){m_bLocalChild = bLocal;}

	virtual void SetChildXFlipRef(void);
	virtual void SetChildYFlipRef(void);

	virtual void SetChildShowRef(void);
	virtual void OnSetParent(CObjectBase *pParent, bool bSet);
	virtual void SetChildEnableRef(void);

	virtual const bool &IsClipDraw(void);

	virtual bool CheckChange(void);
	virtual bool CheckChangeAlpha(void);
protected:
	virtual void SetObjectProject(CObjectProject *pProject);
public:
	virtual CObjectProject *GetObjectProject(void){return m_pProject;}
public:
	CObjectBase();
	virtual ~CObjectBase();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_UNKNOWN;}

	//로직 함수
	virtual void Logic(const DWORD &dwTime);
	virtual void SetUseLogic(const bool &bUseLogic);
	virtual const bool &IsUseLogic(void);

	//그리기 관련 함수들
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMy(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawLocalChild(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawChild(NMBASE::GRAPHICGDI::CPage *pPage);

	//상태 관련 함수들
	inline	CObjectBase *GetParent(void){return m_pParent;}

	inline const bool &IsShow(void){return m_bShow;}
	inline const bool &IsShowRef(void){return m_bShowRef;}
	inline const bool &IsEnable(void){return m_bEnable;}
	inline const bool &IsEnableRef(void){return m_bEnableRef;}
	inline const bool &IsChange(void){return m_bChange;}
	inline const bool &IsChangeAlpha(void){return m_bChangeAlpha;}
	inline const bool &IsFocus(void){return m_bFocus;}
	inline const bool &IsMouseOver(void){return m_bMouseOver;}

	const bool IsChild(CObjectBase *pObject);
	const bool	&IsLocalChild(void){return m_bLocalChild;}

	virtual void SetShow(const bool &bShow);
	virtual void SetShowRef(const bool &bShowRef);

	virtual void SetEnable(const bool &bEnable);
	virtual void SetEnableRef(const bool &bEnableRef);

	virtual void SetChange(const bool &bChange);
	virtual void SetChangeAlpha(const bool &bChangeAlpha);

	virtual void SetUseMouseWheel(const bool &bUse){m_bUseMouseWheel = bUse;}
	virtual const bool &IsUseMouseWheel(void){return m_bUseMouseWheel;}

	virtual const float GetWidth(void);
	virtual const float GetHeight(void);

	virtual bool IsParent(CObjectBase *pParent);		//부모인지 검사하는 함수... 부모의 부모..까지 검사함

	virtual void SetID(const int &nID){m_nID = nID;}
	virtual const int &GetID(void){return m_nID;}

	//위치 관련 함수들
	virtual void SetLocalPos(const CPoint &ptPos);
	virtual void SetLocalPos(const float &fX, const float &fY);
	virtual const CPoint &GetLocalPos(void);

	virtual void SetPos(const CPoint &ptPos);
	virtual void SetPos(const float &fX, const float &fY);
	virtual const CPoint &GetPos(void);

	virtual void SetMove(const CPoint &ptMove);
	virtual void SetMove(const float &fX, const float &fY);
	virtual const CPoint &GetMove(void);

	virtual void SetInterceptSelect(CObjectBase *pObject){m_pInterceptSelect = pObject;}
	virtual CObjectBase *GetInterceptSelect(void){return m_pInterceptSelect;}

	//배율 관련 함수들
	virtual void SetLocalScaleX(const float &fScaleX);
	virtual void SetLocalScaleY(const float &fScaleY);
	virtual void SetLocalScale(const float &fScale);
	virtual const float &GetLocalScaleX(void);
	virtual const float &GetLocalScaleY(void);

	virtual void SetScaleX(const float &fScaleX);
	virtual void SetScaleY(const float &fScaleY);
	virtual const float &GetScaleX(void);
	virtual const float &GetScaleY(void);

	//회전 관련 함수들
	virtual void SetLocalRotation(const float &fRotation);
	virtual const float &GetLocalRotation(void);

	virtual void SetRotation(const float &fRotation);
	virtual const float &GetRotation(void);

	//기준점 관련 함수들
	virtual void SetPivot(const CPoint &ptPos);
	virtual void SetPivot(const float &fX, const float &fY);
	virtual const CPoint &GetPivot(void);

	virtual void SetLocalRect(const CRect &rtRect);
	virtual CRect &GetLocalRect(void);
	virtual CRect &GetRect(void);
	virtual CRect &GetOldDrawRect(void){return m_rtOldDraw;}

	//Alpha 관련 함수들, 값은 0~1 사이 값이다
	virtual void SetLocalAlpha(const float &fAlpha);
	virtual const float &GetLocalAlpha(void);
	virtual const float &GetAlpha(void);

	//반전 관련 함수들
	virtual void SetXFlip(const bool &bXFlip);
	virtual void SetXFlipRef(const bool &bXFlipRef);
	virtual const bool &IsXFlip(void);

	virtual void SetYFlip(const bool &bYFlip);
	virtual void SetYFlipRef(const bool &bYFlipRef);
	virtual const bool &IsYFlip(void);

	//마우스 고정 관련 함수들
	virtual void SetFixed(const bool &bFixed){m_bFixed = bFixed;}
	virtual const bool &IsFixed(void){return m_bFixed;}

	//동적 생성관련 함수들
	virtual void SetDynamic(const bool &bDynamic){m_bDynamic = bDynamic;}
	virtual const bool &IsDynamic(void){return m_bDynamic;}

	//메지시 입력 관련 함수들
	virtual void SetMessage(const bool &bMessage);
	virtual const bool &IsMessage(void);

	//클리핑 관련 함수들
	virtual void SetLocalClipRect(const CRect &rtClip);
	virtual CRect *GetLocalClipRect(void){return m_pClipLocal;}
	virtual void SetGlobalClipRect(const CRect &rtClip);
	virtual CRect *GetClipRect(void);

	virtual void SetLocalClipUseParentPos(const bool &bLocalClipUseParentPos);
	virtual const bool &IsLocalClipUseParentPos(void){return m_bLocalClipUseParentPos;}

	//마우스로 드래그 이동시킬수 있는지의 여부
	virtual void SetMouseMove(const bool &bMove){m_bMouseMove = bMove;}
	virtual const bool &IsMouseMove(void){return m_bMouseMove;}

	virtual CPoint GetGlobalToLocalPos(const CPoint &ptPos);
	virtual CPoint GetLocalToGlobalPos(const CPoint &ptPos);

	virtual CObjectLocalData GetLocalData(void);
	virtual void SetLocalData(CObjectLocalData &cData);

	virtual bool OnClosePopup(CObjectBase *pFocusObject);
	virtual CObjectBase *GetWindowsParent(void);

	virtual CObjectBase *GetWheelFocusObject(void);
	virtual CObjectBase *GetObserverObject(void){return m_pObserverObject;}

	virtual void SetEffect(ENUM_OBJECTEFFECTTYPE nEffect);
	virtual const ENUM_OBJECTEFFECTTYPE &GetEffect(void){return m_nEffectType;}

	virtual void SetColor(int nColor){m_nColor = nColor;}
	virtual int GetColor(void){return m_nColor;}

	virtual void SetMover(CMover *pMover);
	virtual CMover *GetMover(void){return m_pMover;}

	virtual void SetUserData(int nID, DWORD_PTR dwData);
	virtual DWORD_PTR GetUserData(int nID);

	virtual void SetExpand(bool bExpand){m_bExpand = bExpand;}
	virtual const bool &IsExpand(void){return m_bExpand;}

	virtual void SetName(std::string strName){m_strName = strName;}
	virtual const std::string &GetName(void){return m_strName;}

	virtual void SetObserverRect(CObjectRect *pObjectRect);
	virtual CObjectRect *GetObserverRect(void){return m_pObjectRect;}

	virtual bool IsPossibilityReSize(void){return false;}

	virtual void SetTracking(const bool &bTracking);
	virtual void RemoveTracking(void);
	virtual const bool &IsTracking(void){return m_bTracking;}

	virtual void SetIgnoreParentClip(bool bIgnoreClip);
	virtual const bool &IsIgnoreParentClip(void){return m_bIgnoreParentClip;}

	virtual void SetIgnoreParentPos(bool bIgnorePos);
	virtual const bool &IsIgnoreParentPos(void){return m_bIgnoreParentPos;}

	virtual void SetTempEventDraw(const bool &bEventDraw){m_bTempEventDraw = bEventDraw;}
	virtual const bool &IsTempEventDraw(void){return m_bTempEventDraw;}

	//bIn의 경우 비활성화된 Object 이지만
	//메시지를 받는 Object의 경우 그 하위로 메시지를 전달하지 않기 위해서 사용하는 임시 변수
	virtual bool IsIn(IN CPoint &ptPos, OUT bool &bIn);

	//차일드의 차일드까지 검사해서 해당 Object가 있는지 알아주는 함수
	//속도가 굉장히 느린 함수 이므로 반드시 List or Image에서 Clone된 ListItem or MiddleImage에서
	//하위에 있는 Object를 검색할 때만 사용하시기 바랍니다.
	virtual CObjectBase *GetFirstChildFromID(const int &nID);
	virtual CObjectBase *GetNextChildFromID(const int &nID);

	virtual CObjectBase *GetFirstChildFromName(const std::string &strName);
	virtual CObjectBase *GetNextChildFromName(const std::string &strName);
public:
	//Child 관련 함수들
	//pBeforeObject 전에 pObject를 넣는다. pBeforeObject == NULL 이면 가장 뒤에 넣는다.
	virtual void AddChild(CObjectBase *pObject, CObjectBase *pDest = NULL, bool bBefore = true);
	virtual void AddLocalChild(CObjectBase *pObject, CObjectBase *pDest = NULL, bool bBefore = true);
	virtual void SubChild(CObjectBase *pObject);

	inline const float &GetSin(void){return m_fSin;}
	inline const float &GetCos(void){return m_fCos;}

	const LISTOBJECT &GetChildList(void){return m_listChild;}
	const LISTOBJECT &GetLocalChildList(void){return m_listLocalChild;}

	virtual void SetRemove(void);		//Draw 전에 삭제할지를 지정하는 함수
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *Clone(const bool &bChildClone = false, const bool &bUseOriginalObjectFunctor = false, const bool &bCloneObserverChild = false, CObjectBase *pObserverObject = NULL, const bool &bFirst = true);
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetClipSize(int &nVersion);
	virtual size_t GetInheritanceClipSize(int &nVersion){return 0;}
	virtual size_t SaveClipSize(int &nVersion, BYTE *pDest);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest){return 0;}
	virtual size_t LoadClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID){return 0;}

	virtual bool SaveXml(TiXmlNode *pParentNode);
	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
public:
	//마우스 이벤트 처리
	virtual bool OnLButtonDown(const CPoint &ptPos);
	virtual bool OnLButtonUP(const CPoint &ptPos);
	virtual bool OnLButtonDbClick(const CPoint &ptPos);
	virtual bool OnRButtonDown(const CPoint &ptPos);
	virtual bool OnRButtonUp(const CPoint &ptPos);
	virtual bool OnRButtonDbClick(const CPoint &ptPos);
	virtual bool OnMButtonDown(const CPoint &ptPos);
	virtual bool OnMButtonUp(const CPoint &ptPos);
	virtual bool OnMButtonDbClick(const CPoint &ptPos);
	virtual bool OnMouseMove(const CPoint &ptPos);
	virtual bool OnMouseWheel(const int &nKey, const short &zDelta);

	virtual bool OnDragBegin(const CPoint &ptPos);
	virtual bool OnDraging(const CPoint &ptPos);
	virtual bool OnDragEnd(const CPoint &ptPos);

	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);
	virtual void OnMouseLClick(const CPoint &ptPos);
	virtual void OnMouseRClick(const CPoint &ptPos);
	virtual void OnMouseMClick(const CPoint &ptPos);

	virtual LRESULT Dispatch(CMessage *msg);
	virtual LRESULT OnLocalChildNotify(CMessage *msg);
	virtual LRESULT OnObserverNotify(CMessage *msg){return TRUE;}

	//Xml을 로딩하여 Object 생성시 발생하는 함수들
	virtual void OnCreated(void){}						//Object가 생성되었을때
	virtual void OnCreateChildStart(void){}				//Child Object가 만들어지기 시작하는 직전에 호출되는 함수
	virtual void OnCreateChild(CObjectBase *pObject){}	//Child Object가 만들어져서 AddChild가 된 직후 호출되는 함수
	virtual void OnCreateChildEnd(void){}				//Child Object가 모두 만들어진 직후 호출되는 함수
	virtual void OnAttachChild(CObjectBase *pObject){}	//Child Object가 Attach 됐을때 호출되는 함수

	//리턴값이 true이면 트래킹을 중지한다. 기본값을 true이다
	virtual bool OnClickOutside(CObjectBase *pFocus);

	//키보드 이벤트 처리
	virtual CObjectBase *OnFocusIn(CObjectBase *pObject); //포커스를 잃는 Object를 받는다.
	virtual void OnFocusOut(CObjectBase *pObject); //포커를 얻는 Object를 받는다.
	virtual void OnKeyDown(const int &nKey);
	virtual void OnKeyUp(const int &nKey);
	virtual void OnTabDown(void);
	virtual void OnReturn(void);
	virtual BOOL OnKeyProc(const int &nMessage, const WPARAM &wParam, const LPARAM &lParam);

	virtual void OnAddSubRootObject(bool bAdd);
	virtual void OnProjectActiveChange(bool bActive);

	virtual void CheckEventDraw(void);
	virtual void CheckEventObjectDraw(void);
	virtual const bool &IsSendEventDraw(void){return m_bSendEventDraw;}
	virtual void ClearSendEventDraw(void){m_bSendEventDraw = false;}
public:
	friend class CObjectProject;
	friend class CObjectMan;
	friend class CMover;
};
} //namespace LSY