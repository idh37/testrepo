#pragma once

#include "ObjectBase.h"

namespace LSY
{
class CImage;

class UPG_LSYLIB_EXPORT CImageNumber :public CObjectBase
{
protected:
	CImage						*m_pImage;
	int							m_nStartIndex;			//이미지의 숫자의 시작 인덱스를 지정해준다.
	INT64						m_i64Number;
	int							m_nMaxPlace;			//표시할 최대 자리수를 나타낸다. EIND_NUMBER, EIND_COMMA_MONEY일때만 동작한다. 지정한 자리수보다 큰 값이면 모두 9로 표시한다. 0이면 모두 표시한다.
	int							m_nCutCount;			//몇단위까지 표시할지를 나타내는 변수, 0이면 전체를 표시한다. EIND_MONEY에서만 유효함
	int							m_nUnitStartIndex;		//EIND_MONEY, EIND_NEONSIGN_MONEY에서는 단위의 시작 인덱스, EIND_COMMA_MONEY에서는 콤마의 인덱스를 지정함
	int							m_nUnitIndexGap;		//단위가 증가할때 인덱스의 변화값, EIND_MONEY, EIND_NEONSIGN_MONEY에서만 유효함
	int							m_nFirstUnitIndex;		//가장 하위 단위의 인덱스
	int							m_nPrefixIndex;			//앞에 추가되는 프리픽스 이미지 인덱스
	int							m_nPlusIndex;			//Plus표시 이미지 인덱스
	int							m_nMinusIndex;			//Minus표시 이미지 인덱스, -1이면 0이상의 값만을 표시한다.
	bool						m_bShowZero;			//0을 표시할지의 여부
	bool						m_bShowZeroNumber;		//각단위에서 최초로 0이외의 숫자가 나오기 전까지의 상위 0을 보이게 할지의 여부를 지정한다. 예 : 90023일경우 9만0023으로 표시
	bool						m_bShowZeroPlus;		//0일때 플러스 표시를 할지 여부를 지정한다.
	bool						m_bRedraw;

	UINT						m_uFormat;				//정렬 방식
	ENUM_IMAGENUMBER_DRAWTYPE	m_nNumberType;			//이미지넘버 종류 지정

	std::list<CImage *>			m_listImage;
public:
	CImageNumber(void);
	virtual ~CImageNumber();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_IMAGENUMBER;}
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	bool Create(LPCTSTR lpPath, int nWidth, int nHeight);

	//전체 이미지 갯수를 가져온다.
	const int GetTotalNum(void);
	void SetRedraw(bool bRedraw = true){m_bRedraw = bRedraw;}
	const bool &IsRedraw(void){return m_bRedraw;}

	std::string GetFileName(void);
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual bool IsPossibilityReSize(void){return true;}

	virtual void SetNumber(const INT64 &i64Number);
	virtual const INT64 &GetNumber(void){return m_i64Number;}

	virtual void SetMaxPlace(const int &nMaxPlace);
	virtual const int &GetMaxPlace(void){return m_nMaxPlace;}

	virtual void SetCutCount(const int &nCutCount);
	virtual const int &GetCutCount(void){return m_nCutCount;}

	virtual void SetUnitStartIndex(const int &nUnitStartIndex);
	virtual const int &GetUnitStartIndex(void){return m_nUnitStartIndex;}

	virtual void SetUnitIndexGap(const int &nUnitIndexGap);
	virtual const int &GetUnitIndexGap(void){return m_nUnitIndexGap;}

	virtual void SetFirstUnitIndex(const int &nFirstUnitIndex);
	virtual const int &GetFirstUnitIndex(void){return m_nFirstUnitIndex;}

	virtual void SetPrefixIndex(const int &nPrefixIndex);
	virtual const int &GetPrefixIndex(void){return m_nPrefixIndex;}

	virtual void SetPlusIndex(const int &nPlusIndex);
	virtual const int &GetPlusIndex(void){return m_nPlusIndex;}

	virtual void SetMinusIndex(const int &nMinusIndex);
	virtual const int &GetMinusIndex(void){return m_nMinusIndex;}

	virtual void SetShowZero(const bool &bShowZero);
	virtual const bool &IsShowZero(void){return m_bShowZero;}

	virtual void SetShowZeroNumber(const bool &bShowZeroNumber);
	virtual const bool &IsShowZeroNumber(void){return m_bShowZeroNumber;}

	virtual void SetShowZeroPlus(const bool &bShowZeroPlus);
	virtual const bool &IsShowZeroPlus(void){return m_bShowZeroPlus;}

	virtual void SetStartIndex(const int &nStartIndex);
	virtual const int &GetStartIndex(void){return m_nStartIndex;}

	virtual void SetFormat(const UINT &uFormat);
	virtual const UINT &GetFormat(void){return m_uFormat;}

	virtual void SetNumberType(const ENUM_IMAGENUMBER_DRAWTYPE &nType);
	virtual const ENUM_IMAGENUMBER_DRAWTYPE &GetNumberType(void){return m_nNumberType;}

	CSize GetImageSize(void);
protected:
	void ClearImage(void);
	void CheckRedraw(void);

	void CheckNumberDraw(char *strNumber,int &nSize);
	void CheckCommaMoneyDraw(char *strNumber,int &nSize);
	void CheckMoneyDraw(char *strNumber,int &nSize);
	void CheckNeonsignMoneyDraw(char *strNumber,int &nSize);
};
} //namespace LSY