#pragma once

class CMessageBoxImgFull : public LSY::CImage
{
protected:
	LSY::CRadio		*m_pTab;
public:
	CMessageBoxImgFull();
	virtual ~CMessageBoxImgFull(){}

	void SetTab(LSY::CRadio *pTab){m_pTab = pTab;}

	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);

	virtual bool OnLButtonDown(const LSY::CPoint &ptPos);
	virtual bool OnLButtonUP(const LSY::CPoint &ptPos);
	virtual bool OnLButtonDbClick(const LSY::CPoint &ptPos);
	virtual void OnMouseLClick(const LSY::CPoint &ptPos);
};