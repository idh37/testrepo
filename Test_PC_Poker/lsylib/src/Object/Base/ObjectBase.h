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

	bool						m_bMessage;		//���콺, Ű������� �̺�Ʈ�� �߻������� ����

	bool						m_bLocalChild;	//�ܺο��� ��������� �ʰ� Object���� ��ü������ ����� ����ϴ� Child������ ����

	LISTOBJECT					m_listLocalChild;	//�ܺο��� ��������� �ʰ� Object���� ��ü������ ����� ����ϴ� Child ����Ʈ
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

	bool						m_bChange;		//��ġ, ����, ȸ���� ���� �Ǿ������� ����, �ڽ��� ������� �ʾƵ� �θ� ����Ǹ� ���õȴ�.
	bool						m_bChangeAlpha; //Alpha���� ���� �Ǿ������� ����, �ڽ��� ������� �ʾƵ� �θ� ����Ǹ� ���õȴ�.

	bool						m_bFocus;		//���� Ű���� �̺�Ʈ�� �޴��� ���θ� �˷���
	bool						m_bMouseOver;	//���� ���콺�� ���� �ִ��� ����

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

	bool						m_bLocalClipUseParentPos;	//���� Ŭ���� ������ �ڽ��� ��ǥ�� �ƴ� �θ��� ��ǥ�� �������� �����ϵ��� �Ѵ�.

	//��ο� ���� �ӽ� ����
	DWORD						m_dwDrawOption;
	CPoint						m_ptDraw;
	float						m_fDrawRotaion;
	ENUM_OBJECTDRAWTYPE			m_nDrawType;
	bool						m_bClipDraw;		//Ŭ���� ������ ���ؼ� �׷��������� ����(Ŭ���� ������ 0������ �˻�)�� ����
	bool						m_bDraw;			//Ŭ���� �Ǵ� ȭ����� Bound�� ���ؼ� �׷��������� ����

	bool						m_bMouseMove;		//���콺�� �������� ���θ� �����Ѵ�. m_bMessage�� true, m_bEnable�� true�϶��� �����Ѵ�.
	CPoint						m_ptDragGab;
	bool						m_bDrag;
	bool						m_bUseLogic;
	bool						m_bUseMouseWheel;

	bool						m_bIgnoreParentClip;	//�θ��� Ŭ������ ���������� ����
	bool						m_bIgnoreParentPos;		//�θ��� ��ġ/����/ȸ��/���������� ���������� ����

	//Clone�Ͽ� ������ Object���� ���� Object�� ��Ÿ����. 
	//������ �׻� Clone�� ����Ǵ� ���� �ƴ϶� Clone�� Object�� Event Message�� ���� Object�� ��ϵ� Functor�� ���ؼ� �����Ҷ� ����ϰ� �ȴ�.
	//Clone�� �ɼ����� ����
	CObjectBase					*m_pOriginalObject;
	CObjectBase					*m_pObserverObject;		//����� ����Ǿ� ���� ��� Object�� �޽����� �����ϴ� ���� Object
	CObjectBase					*m_pInterceptSelect;	//Object�� Select�ɶ� ����ç Object�� ��Ÿ����. �� Object�� Select�Ǹ� ���⿡ ��ϵ� Object�� Select�� ���� æ��.

	//ȸ������ ���� �ڻ��ΰ��� �̸� ����� �д�.
	float						m_fCos;
	float						m_fSin;

	CMover						*m_pMover;
	std::map<int, DWORD_PTR>	m_listUserData;
	bool						m_bExpand;
	bool						m_bTracking;
	bool						m_bRemove;

	//�̺�Ʈ �׸���� ����ϴ� �ӽ� ������
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

	//���� �Լ�
	virtual void Logic(const DWORD &dwTime);
	virtual void SetUseLogic(const bool &bUseLogic);
	virtual const bool &IsUseLogic(void);

	//�׸��� ���� �Լ���
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMy(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawLocalChild(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawChild(NMBASE::GRAPHICGDI::CPage *pPage);

	//���� ���� �Լ���
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

	virtual bool IsParent(CObjectBase *pParent);		//�θ����� �˻��ϴ� �Լ�... �θ��� �θ�..���� �˻���

	virtual void SetID(const int &nID){m_nID = nID;}
	virtual const int &GetID(void){return m_nID;}

	//��ġ ���� �Լ���
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

	//���� ���� �Լ���
	virtual void SetLocalScaleX(const float &fScaleX);
	virtual void SetLocalScaleY(const float &fScaleY);
	virtual void SetLocalScale(const float &fScale);
	virtual const float &GetLocalScaleX(void);
	virtual const float &GetLocalScaleY(void);

	virtual void SetScaleX(const float &fScaleX);
	virtual void SetScaleY(const float &fScaleY);
	virtual const float &GetScaleX(void);
	virtual const float &GetScaleY(void);

	//ȸ�� ���� �Լ���
	virtual void SetLocalRotation(const float &fRotation);
	virtual const float &GetLocalRotation(void);

	virtual void SetRotation(const float &fRotation);
	virtual const float &GetRotation(void);

	//������ ���� �Լ���
	virtual void SetPivot(const CPoint &ptPos);
	virtual void SetPivot(const float &fX, const float &fY);
	virtual const CPoint &GetPivot(void);

	virtual void SetLocalRect(const CRect &rtRect);
	virtual CRect &GetLocalRect(void);
	virtual CRect &GetRect(void);
	virtual CRect &GetOldDrawRect(void){return m_rtOldDraw;}

	//Alpha ���� �Լ���, ���� 0~1 ���� ���̴�
	virtual void SetLocalAlpha(const float &fAlpha);
	virtual const float &GetLocalAlpha(void);
	virtual const float &GetAlpha(void);

	//���� ���� �Լ���
	virtual void SetXFlip(const bool &bXFlip);
	virtual void SetXFlipRef(const bool &bXFlipRef);
	virtual const bool &IsXFlip(void);

	virtual void SetYFlip(const bool &bYFlip);
	virtual void SetYFlipRef(const bool &bYFlipRef);
	virtual const bool &IsYFlip(void);

	//���콺 ���� ���� �Լ���
	virtual void SetFixed(const bool &bFixed){m_bFixed = bFixed;}
	virtual const bool &IsFixed(void){return m_bFixed;}

	//���� �������� �Լ���
	virtual void SetDynamic(const bool &bDynamic){m_bDynamic = bDynamic;}
	virtual const bool &IsDynamic(void){return m_bDynamic;}

	//������ �Է� ���� �Լ���
	virtual void SetMessage(const bool &bMessage);
	virtual const bool &IsMessage(void);

	//Ŭ���� ���� �Լ���
	virtual void SetLocalClipRect(const CRect &rtClip);
	virtual CRect *GetLocalClipRect(void){return m_pClipLocal;}
	virtual void SetGlobalClipRect(const CRect &rtClip);
	virtual CRect *GetClipRect(void);

	virtual void SetLocalClipUseParentPos(const bool &bLocalClipUseParentPos);
	virtual const bool &IsLocalClipUseParentPos(void){return m_bLocalClipUseParentPos;}

	//���콺�� �巡�� �̵���ų�� �ִ����� ����
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

	//bIn�� ��� ��Ȱ��ȭ�� Object ������
	//�޽����� �޴� Object�� ��� �� ������ �޽����� �������� �ʱ� ���ؼ� ����ϴ� �ӽ� ����
	virtual bool IsIn(IN CPoint &ptPos, OUT bool &bIn);

	//���ϵ��� ���ϵ���� �˻��ؼ� �ش� Object�� �ִ��� �˾��ִ� �Լ�
	//�ӵ��� ������ ���� �Լ� �̹Ƿ� �ݵ�� List or Image���� Clone�� ListItem or MiddleImage����
	//������ �ִ� Object�� �˻��� ���� ����Ͻñ� �ٶ��ϴ�.
	virtual CObjectBase *GetFirstChildFromID(const int &nID);
	virtual CObjectBase *GetNextChildFromID(const int &nID);

	virtual CObjectBase *GetFirstChildFromName(const std::string &strName);
	virtual CObjectBase *GetNextChildFromName(const std::string &strName);
public:
	//Child ���� �Լ���
	//pBeforeObject ���� pObject�� �ִ´�. pBeforeObject == NULL �̸� ���� �ڿ� �ִ´�.
	virtual void AddChild(CObjectBase *pObject, CObjectBase *pDest = NULL, bool bBefore = true);
	virtual void AddLocalChild(CObjectBase *pObject, CObjectBase *pDest = NULL, bool bBefore = true);
	virtual void SubChild(CObjectBase *pObject);

	inline const float &GetSin(void){return m_fSin;}
	inline const float &GetCos(void){return m_fCos;}

	const LISTOBJECT &GetChildList(void){return m_listChild;}
	const LISTOBJECT &GetLocalChildList(void){return m_listLocalChild;}

	virtual void SetRemove(void);		//Draw ���� ���������� �����ϴ� �Լ�
public:
	//Object ���� ���� �Լ���
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
	//���콺 �̺�Ʈ ó��
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

	//Xml�� �ε��Ͽ� Object ������ �߻��ϴ� �Լ���
	virtual void OnCreated(void){}						//Object�� �����Ǿ�����
	virtual void OnCreateChildStart(void){}				//Child Object�� ��������� �����ϴ� ������ ȣ��Ǵ� �Լ�
	virtual void OnCreateChild(CObjectBase *pObject){}	//Child Object�� ��������� AddChild�� �� ���� ȣ��Ǵ� �Լ�
	virtual void OnCreateChildEnd(void){}				//Child Object�� ��� ������� ���� ȣ��Ǵ� �Լ�
	virtual void OnAttachChild(CObjectBase *pObject){}	//Child Object�� Attach ������ ȣ��Ǵ� �Լ�

	//���ϰ��� true�̸� Ʈ��ŷ�� �����Ѵ�. �⺻���� true�̴�
	virtual bool OnClickOutside(CObjectBase *pFocus);

	//Ű���� �̺�Ʈ ó��
	virtual CObjectBase *OnFocusIn(CObjectBase *pObject); //��Ŀ���� �Ҵ� Object�� �޴´�.
	virtual void OnFocusOut(CObjectBase *pObject); //��Ŀ�� ��� Object�� �޴´�.
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