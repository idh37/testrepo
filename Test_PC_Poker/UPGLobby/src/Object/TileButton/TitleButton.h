#pragma once

class UPG_LOBBY_EXPORT CTitleButtonGroup : public LSY::CGroup
{
protected:
	LSY::CButton	*m_pBtnVIPCounsel;
	LSY::CButton	*m_pBtnPCRoom;
	LSY::CButton	*m_pBtnCapture;
	LSY::CButton	*m_pBtnSetup;
	LSY::CButton	*m_pBtnInfo;
	LSY::CButton	*m_pBtnGrade;
	LSY::CButton	*m_pBtnMinimize;
	LSY::CButton	*m_pBtnWindow;
	LSY::CButton	*m_pBtnMaxmize;
	LSY::CButton	*m_pBtnClose;
	LSY::CButton	*m_pBtnTimeNotifier;
public:
	CTitleButtonGroup();
	virtual ~CTitleButtonGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	LRESULT OnClickButton(LSY::CMessage *msg);

	void SetChangeDisplayMode(bool bFull);
	void SetPCRoomBtn(int nPCRoomType);

	void SetVIPCounselShow(bool bShow);
};