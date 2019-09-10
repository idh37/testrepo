#pragma once

#include "ObjectBase.h"

class NMBASE::UTIL::CNMSSOWebBrowser;
namespace LSY
{
class CDumyImage;
class UPG_LSYLIB_EXPORT CWeb :public CObjectBase
{
protected:
	NMBASE::UTIL::CNMSSOWebBrowser		*m_pWeb;
	CDumyImage							*m_pRectImage; //���϶� ������ ǥ�����ִ� �̹���
	std::string							m_strURL;

	SWEBCALLBACK_ONBN2					*m_pBeforeNavigateCallback;
	SWEBCALLBACK_ONNC2					*m_pNavigateCompleteCallback;
	SWEBCALLBACK_ONQUIT					*m_pOnQuitCallback;
public:
	CWeb(void);
	virtual ~CWeb();

	BOOL SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback);
	BOOL SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback);
	BOOL SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_WEB;}

	virtual void Create(int nWidth, int nHeight);
	virtual bool IsPossibilityReSize(void){return true;}

	virtual bool CheckChange(void);
	virtual void SetURL(std::string strURL, bool bRefresh = false);
	virtual const std::string &GetURL(void){return m_strURL;}

	//Object ���� ���� �Լ���
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void SetShow(const bool &bShow);
	virtual void SetShowRef(const bool &bShowRef);

	virtual void SetLocalRect(const CRect &rtRect);

	virtual void SetFixed(const bool &bFixed);
	virtual void OnAddSubRootObject(bool bAdd);
	virtual void OnProjectActiveChange(bool bActive);

	// CNMSSOWebBrowser�ȿ� �ִ� �Լ��� CWeb���� ����ϱ� ���� �߰� [11/23/2011 guesty]
	virtual BOOL ExecJavascript(LPCSTR strScript);		
	virtual void ResetUserNavigatedCount();
	virtual int	GetUserNavigatedCount();
protected:
	virtual void CheckShowWebControl(CObjectProject *pProject);
};
} //namespace LSY