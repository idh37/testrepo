#pragma once

#include "./Base/ObjectBase.h"
#include <map>
#include <set>
#include "../Utility/FrameSkip.h"

#define GKS_CONTROL	0x00000001
#define GKS_ALT		0x00000002
#define GKS_SHIFT	0x00000004

#define UM_OBJECTSELECT		(WM_USER+30000)
#define UM_OBJECTMOVE		(WM_USER+30001)
#define UM_OBJECTMOVEEND	(WM_USER+30002)
#define UM_MAKEOBJECTRECT	(WM_USER+30003)
#define UM_UPDATEMOVEPOS	(WM_USER+30004)

#define GMKS_LBUTTON	0x00000001	//LButton이 눌렸을때
#define GMKS_RBUTTON	0x00000002	//LButton이 눌렸을때
#define GMKS_MBUTTON	0x00000004	//LButton이 눌렸을때

class TiXmlElement;

namespace LSY
{
class CObjectMan;
class CObjectBase;
class IObject;
class CImage;

class UPG_LSYLIB_EXPORT CObjectProject
{
protected:
	int			m_nID;
	bool		m_bActive;
	std::string	m_strName;

	CRITICAL_SECTION	m_cLockLogic;
	CRITICAL_SECTION	m_cLockMouse;
	CRITICAL_SECTION	m_cLockObject;

	CObjectBase			*m_pMainObject;

	CObjectBase			*m_pPopupObject;
	CObjectBase			*m_pPopupParent;

	CSize				m_szSize;
	CObjectMan			*m_pParent;

	typedef std::map<IObject *, DWORD>	LISTLOGIC;
	typedef LISTLOGIC::iterator			LISTLOGICITER;

	typedef std::map<int, CObjectBase *>	MAPIDTOOBJECT;
	typedef MAPIDTOOBJECT::iterator			MAPIDTOOBJECTITER;

	typedef std::map<std::string, int>		MAPNAMETOID;
	typedef MAPNAMETOID::iterator			MAPNAMETOIDITER;

	LISTLOGIC			m_listLogicObject;
	LISTOBJECT			m_listMouseMessageObject;

	LISTOBJECT			m_listRemove;

	MAPIDTOOBJECT		m_mapObject;
	MAPNAMETOID			m_mapNameToID;

	LISTLOGICITER		m_itNextLogic;
	bool				m_bLogicProcess;
	LISTOBJECT			m_listLoadOverlapObject;

	bool				m_bFirst;
	std::string			m_strFileName;

	std::string			m_strBackFilePath;
	int					m_nBackIndex;

	CDelegate			m_cCreateObjectFucntor;
	CDelegate			m_cCreatedObjectFucntor;
	MAPIDTOOBJECTITER	m_itFind;
public:
	CObjectProject();
	virtual ~CObjectProject();

	void Create(int nWidth, int nHeight, CObjectMan *pParent);
	bool SaveXml(std::string strFileName);
	bool LoadXml(std::string strFileName = "");	//strFileName이 ""이면 m_strFileName에 있는 Path를 이용해서 Load한다.
	CObjectBase *LoadObject(const int &nVersion, TiXmlElement *pXmlObject);
	bool LoadSceneXml(TiXmlElement *pElementScene, CObjectMan *pParent);

	bool SetCallCreateObject(IFunctor* pFunctor);
	bool SetCallCreatedObject(IFunctor* pFunctor);
	CObjectBase *CreateObject(const std::string &strType, const std::string &strName, const int &nID);

	const std::string &GetFileName(void){return m_strFileName;}
	void SetFileName(const std::string &strFileName){m_strFileName = strFileName;}

	bool SetID(const int &nID);
	const int &GetID(void){return m_nID;}

	void SetPopup(CObjectBase *pPopup, CObjectBase *pFocusObject);
	CObjectBase *GetPopup(void){return m_pPopupObject;}

	bool SetName(const std::string &strName, bool bForce);
	const std::string &GetName(void){return m_strName;}

	void SetActive(const bool &bActive);
	const bool &IsActive(void){return m_bActive;}

	const LONG &GetWidth(void){return m_szSize.cx;}
	const LONG &GetHeight(void){return m_szSize.cy;}
	const CSize &GetSize(void){return m_szSize;}
	void SetSize(CSize szSize);

	CObjectBase *GetMainObject(void){return m_pMainObject;}
	CObjectMan *GetObjectMan(void){return m_pParent;}

	virtual bool AddLogicObject(IObject *pObject);
	virtual bool SubLogicObject(IObject *pObject);
	virtual void CallLogic(const DWORD &dwTime);
	virtual void CallDraw(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual const DWORD &GetTickCount(void);

	virtual void OnRemoveObject(CObjectBase *pObject);
	virtual void OnRemoveChildObject(CObjectBase *pObject);

	virtual bool AddMouseMessageObject(CObjectBase *pObject);
	virtual bool ChangeMouseMessageObject(CObjectBase *pObject);
	virtual bool SubMouseMessageObject(CObjectBase *pObject);

	virtual CObjectBase *IsInObject(CPoint &ptPos, const bool &bClick, OUT CObjectBase **ppDisableInObject = NULL);

	virtual bool IsExistID(const int &nID);
	virtual bool IsExistName(const std::string &strName);
	virtual bool IsExistObject(CObjectBase *pObject);

	virtual int GetLoadOverlapObjectCount(int &nID);
	virtual void SetLoadOverlapObjectCompulsionID(int nID);

	virtual CObjectBase *GetObject(const int &nID);
	virtual CObjectBase *GetObject(const std::string &strName);

	virtual bool AddObject(CObjectBase *pObject);
	
	virtual bool SubObject(CObjectBase *pObject);
	virtual bool ChangeObjectID(const int &nOldID, const int &nNewID);
	virtual bool ChangeObjectName(const std::string &strOldName, const std::string &strNewName);

	virtual int MakeID(void);
	virtual std::string MakeName(std::string strName);

	virtual void AddRemove(CObjectBase *pRemove);

	virtual CObjectBase *GetFirstObject(const ENUM_OBJECTTYPE &nType);
	virtual CObjectBase *GetNextObject(const ENUM_OBJECTTYPE &nType);
protected:
	virtual bool AddMouseMessageObjectTemp(CObjectBase *pParent, CObjectBase *pAddObject, LISTOBJECTITER &it, LISTOBJECTITER &end_it, OUT bool &bResult);
	virtual void GetMouseMessageObject(CObjectBase *pObject, LISTOBJECT &listTemp);	//ChangeMouseMessageObject에서 마우스 메시지를 제거하고 새로 추가할 Object를 구한다. pObject의 하위 Object중에 서도 마우스 Object을 처리하는 것들을 알아오는 함수
	friend CObjectMan;

public:
	virtual LSY::CObjectBase* AddSprObj(const std::string& strPath_,UINT nID_, UINT nPlayIndex_, bool bAfterHide_ = false, CPoint cpPos = CPointZero );
	LSY::CImage* AddImgObj(const std::string& strPath_,UINT nID_, UINT nPlayIndex_, CPoint cpPos = CPointZero);

};

typedef std::map<int, CObjectProject *>	MAPPROJECT;
typedef std::map<std::string, int>		MAPPROJECTNAMETOID;
typedef MAPPROJECT						MAPLAYER;

class UPG_LSYLIB_EXPORT CObjectMan
{
protected:
	MAPLAYER							m_mapLayer;

	CWnd								*m_pWnd;		//그려질 윈도우
	CWnd								*m_pMainWnd;	//Main윈도우로 실제로 Caption을 이용해서 이동하는 윈도우

	NMBASE::GRAPHICGDI::CPage			*m_pBackPage;
	NMBASE::GRAPHICGDI::CPage			*m_pPage;

	LISTOBJECT							m_listEventObject;

	DWORD								m_dwTickCount;
	CSize								m_szSize;

	CObjectBase							*m_pOverObject;
	CObjectBase							*m_pSelectObject;
	CObjectBase							*m_pFocusObject;
	CObjectBase							*m_pWheelFocusObject;

	CRITICAL_SECTION					m_cLock;
	CRITICAL_SECTION					m_cHotkeyLock;
	CRITICAL_SECTION					m_cTimerLock;
	MAPPROJECT							m_mapProject;
	MAPPROJECTNAMETOID					m_mapProjectNameToID;

	LONG_PTR							m_pOldProc;
	bool								m_bFirst;
	CPoint								m_ptMousePos;
	bool								m_bUseCustomCursor;

	HCURSOR								m_hCursorNormal;
	HCURSOR								m_hCursorOver;
	HCURSOR								m_hCursorDown;
	HCURSOR								m_hCursorEdit;

	CMessageManager						m_cMessageManager;
	bool								m_bMouseObserver;
	bool								m_bDrag;
	CPoint								m_ptDragGab;
	std::string							m_strFileName;

	FrameSkip							m_FrameSkip;
	bool								m_bTracking;

	DWORD								m_dwMouseStatus;
	DWORD								m_dwFirstClickMouseKey;
	int									m_nCaptionHeight;

	CDC									*m_pMemDC;
	CDC									*m_pBackDC;
	float								m_fMultiple;
	bool								m_bEventDraw;
	bool								m_bSendEventDraw;	//이벤트 그리기로 그리기 예약이 되어있는지의 여부를 나타냄

	bool								m_bMouseCapture;
	bool								m_bFirstPaint;

	bool								m_bFirstDrawPage;		//metaljack ADD;

	std::list<CObjectMan *>				m_listChildObjectMan;
	//외부에서 클릭이 일어났는지를 조사하는 
	LISTOBJECT							m_listTrackingOutsideClick;
public:
	static std::map<HWND, CObjectMan *> m_sMapProc;
protected:
	static CObjectMan					*m_spMainObjectMan;
	
public:
	CObjectMan(CWnd *pWnd, CWnd *pMainWnd = NULL, bool bMessageHooking = true, bool bEventDraw = false);
	virtual ~CObjectMan();

protected:
	const bool IsMouseMessage(UINT &nMessage);
	const bool IsKeyboardMessage(UINT &nMessage);
	virtual void SetMouseOverObject(CObjectBase *pObject);

	virtual bool AddProject(CObjectProject *pProject);
	virtual bool RemoveProject(const int &nID);

	virtual void CloseAllPopup(CObjectProject *pIgnoreProject);

	virtual bool ChangeProjectID(const int &nOldID, const int &nNewID);
	virtual bool SetProjectName(const std::string &strOldName, const std::string &strNewName);

public:
	virtual const bool &IsEventDraw(void){return m_bEventDraw;}
	virtual void SetEventDraw(void);
	virtual const bool &IsSendEventDraw(void){return m_bSendEventDraw;}

	virtual void AllDraw(void);
public:
	virtual LRESULT WindowsProc(UINT nMessage, WPARAM wParam, LPARAM lParam);
	virtual bool Draw(CDC *pDC = NULL);
	virtual bool BitBlt(CDC *pDC = NULL);
	virtual void SetLayer(int nLayer, CObjectProject *pProject);
	virtual void OnRemoveChildObject(CObjectBase *pObject);

	virtual bool EventBackDraw(CDC *pDC = NULL);
	virtual bool EventDraw(CObjectBase *pObject = NULL, CDC *pDC = NULL);
	virtual bool EventBackToFrontBitBlt(CRect *pRect = NULL);
	virtual bool EventFrontBitBlt(CRect *pRect, CDC *pDC, CPoint *pPtSorce);
	virtual bool EventBitBlt(CRect *pRect = NULL, CDC *pDC = NULL);
	virtual void CallEventLogic(void);
	virtual void ClearEventDrawList(void);

	virtual CObjectProject *CreateProject(const int &nWidth, const int &nHeight);
	virtual bool SetProject(const int &nLayer, const int &nID);
	virtual bool SetProject(const int &nLayer, const std::string &strName);
	virtual CObjectProject *GetProjectFromLayer(const int &nLayer);
	virtual CObjectProject *GetProjectFromID(const int &nID);
	virtual CObjectProject *GetProjectFromName(const std::string &strName);
	virtual int GetProjectNameToID(const std::string &strName);
	virtual void ReSize(CSize szSize);

	virtual bool LoadXml(std::string strFileName);

	virtual const CPoint &GetCurMousePos(void){return m_ptMousePos;}

	virtual void SetUseCustomCursor(bool bUse);
	virtual const bool &IsUseCustomCursor(void){return m_bUseCustomCursor;}

	virtual void SetCursorNormal(HCURSOR hCursor);
	virtual void SetCursorOver(HCURSOR hCursor);
	virtual void SetCursorDown(HCURSOR hCursor);
	virtual void SetCursorEdit(HCURSOR hCursor);

	virtual CObjectBase *GetSelectObject(void){return m_pSelectObject;}
	virtual CObjectBase *GetFocusObject(void){return m_pFocusObject;}

	virtual const HCURSOR &GetCursorNormal(void){return m_hCursorNormal;}
	virtual const HCURSOR &GetCursorOver(void){return m_hCursorOver;}
	virtual const HCURSOR &GetCursorDown(void){return m_hCursorDown;}
	virtual const HCURSOR &GetCursorEdit(void){return m_hCursorEdit;}

	virtual void SetUseObserverMouseEvent(bool bUse);
	virtual void ClearObserverMouseEvent(void);

	virtual void SetCursorFunctor(IFunctor* pFunctor);
	virtual void SetObserverMouseDragBeginFuntor(IFunctor* pFunctor);
	virtual void SetObserverMouseDragingFuntor(IFunctor* pFunctor);
	virtual void SetObserverMouseDragEndFuntor(IFunctor* pFunctor);

	virtual void SetMouseLeaveFunctor(IFunctor* pFunctor);
	virtual void SetMouseOverFunctor(IFunctor* pFunctor);

	virtual void SetTracking(CObjectBase *pObject);
	virtual void RemoveTracking(CObjectBase *pObject);

	virtual bool OnEventObjectDraw(CObjectBase *pObject);

	virtual CObjectBase *GetCurrentOverObject(void){return m_pOverObject;}

	virtual void AddEventObject(CObjectBase *pObject);

	CWnd *GetWnd(void){return m_pWnd;}
	NMBASE::GRAPHICGDI::CPage *GetPage(void){return m_pPage;}

	virtual CObjectBase *IsInObject(CPoint &ptPos, const bool &bClick, OUT CObjectBase **ppDisableInObject = NULL);

	const LONG_PTR &GetOldWindowProc(void);

	void SetCaptionHeight(const int &nCaptionHeight){m_nCaptionHeight = nCaptionHeight;}
	const int &GetCaptionHeight(void){return m_nCaptionHeight;}

	void SetFocus(CObjectBase *pObject, bool bReleseWheel = true);
	CObjectBase *GetFocus(void);
	const CSize &GetSize(void){return m_szSize;}

	const DWORD &GetTickCount(void){return m_dwTickCount;}

	int			MakeProjectID(void);
	std::string	MakeProjectName(void);

	virtual void SetMultiple(const float &fMultiple);
	virtual const float &GetMultiple(void){return m_fMultiple;}

	LRESULT		CheckHotkey(UINT &nMessage, WPARAM &wParam, LPARAM &lParam);
	static int	GetScanCode(const WPARAM &wParam, const LPARAM &lParam);

	virtual void SetFirstDraw(const bool &bFirstDraw){m_bFirstPaint = bFirstDraw;}
	virtual const bool &IsFirstDraw(void){return m_bFirstPaint;}

	virtual void AddChildObjectMan(CObjectMan *pObjectMan);
	virtual void SubChildObjectMan(CObjectMan *pObjectMan);

	void SetMouseCapture(void);
	void ReleaseMouseCapture(void);

	static void SetMainObjectMan(CObjectMan *pObjectMan){m_spMainObjectMan = pObjectMan;}
	static CObjectMan *GetMainObjectMan(void){return m_spMainObjectMan;}

	void SetFPS(float fps);
	CDC *GetMemDC(void){return m_pMemDC;}
	
	bool GetFirstDrawPage() { return m_bFirstDrawPage; }

protected:
	typedef std::map<int, CDelegate>						MAP_SCANCODE_DELEGATE; //<스캔코드, 델리게이트>
	typedef std::map<unsigned short, MAP_SCANCODE_DELEGATE>	MAP_HOTKEY;			   //<(ctrl, alt)조합, 스캔델리게이트>
	MAP_HOTKEY									m_mapHotKey;

	LRESULT		CallHotKey(const unsigned short &sHotkey, const int &nScanCode, const bool &bPrevious);
	CDelegate*	FindDelegate(const unsigned short &sHotkey, const int &nScanCode);

	void CallTracking(CObjectBase *pObject);

	struct TIMER_INFO
	{
	DWORD		dwStart;
	DWORD		dwNext;
	DWORD		dwElapse;		
	int			nCurRepeat;		//현재 반복 횟수
	int			nRepeat;		//반복 횟수, 0이면 무한 반복
	CDelegate	cDelegate;
	};
	typedef std::map<int, TIMER_INFO>			MAP_TIMER;

	MAP_TIMER									m_mapTimer;
	bool										m_bCallTimer;
	std::list<int>								m_listDeleteTimer;
public:
	bool AddHotKeyMsgH				(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor);
	bool AppendHotKeyMsgH			(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor);
	bool OverrideHotKeyMsgH			(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor);

	bool RemoveHotKeyMsgH			(bool bControl,  bool bAlt, int nScanCode);
	bool RemoveAppendHotkeyMsgH		(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor);	// Appand한 메시지 중 선택한 삭제한다.
	bool RemoveOverrideHotKeyMsgH	(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor);	// Override메시지를 삭제한다.

	void ClearHotKey(void);

	void SetTimer(int nID, DWORD dwElapse, IFunctor* pFunctor, int nRepeat = 0);
	void KillTimer(int nID, bool bDel = false );
	bool IsTimer(int nID);

	void ClearTimer(void);
	void CheckMouseOverObject(void);
protected:
	void CheckTimer(void);
	void InitFirstTime(void);
public:
	friend class CObjectProject;
	friend class CLSYLibDialogBase;
};

UPG_LSYLIB_EXPORT const bool &IsTool(void);
UPG_LSYLIB_EXPORT void SetTool(const bool &bTool);
UPG_LSYLIB_EXPORT CObjectBase *CreateNewObject(ENUM_OBJECTTYPE nType);
UPG_LSYLIB_EXPORT BOOL IsExistFile(LPCTSTR szFileName);
UPG_LSYLIB_EXPORT BOOL IsExistFile(std::string strFileName);

UPG_LSYLIB_EXPORT void InitPlatform(CWnd *pWnd);
UPG_LSYLIB_EXPORT void ReleasePlatform(void);
} //namespace LSY