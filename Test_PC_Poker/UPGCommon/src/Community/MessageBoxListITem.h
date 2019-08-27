#pragma once

class CMessageBoxListItem : public LSY::CListItem
{
protected:
	LSY::CCheckBox		*m_pCheck;
	LSY::CText			*m_pTxtSender;
	LSY::CText			*m_pTxtTitle;
	LSY::CText			*m_pTxtDate;
public:
	CMessageBoxListItem();
	virtual ~CMessageBoxListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

	friend class CMessageBox;
};