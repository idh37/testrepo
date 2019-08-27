#pragma once

#include "Text.h"
#include <vector>
#include <list>
#include <string>

namespace LSY
{
class UPG_LSYLIB_EXPORT CTextNumber : public CText
{
protected:
	INT64						m_i64Number;
	int							m_nMaxPlace;			//표시할 최대 자리수를 나타낸다. EIND_NUMBER, EIND_COMMA_MONEY일때만 동작한다. 지정한 자리수보다 큰 값이면 모두 9로 표시한다. 0이면 모두 표시한다.
	int							m_nCutCount;			//몇단위까지 표시할지를 나타내는 변수, 0이면 전체를 표시한다. EIND_MONEY에서만 유효함
	std::vector<std::string>	m_listUnit;				//단위 표시 문자 리스트
	std::string					m_strFirstUnit;			//가장 하위 단위 문자
	std::string					m_strPrefix;			//앞에 추가되는 프리픽스 문자
	bool						m_bUsePlus;				//Plus표시 사용 유무
	bool						m_bUseMinus;			//Minus표시 사용 유무, false이면 0이상의 값만 표시
	bool						m_bShowZero;			//0을 표시할지의 여부
	bool						m_bShowZeroNumber;		//각단위에서 최초로 0이외의 숫자가 나오기 전까지의 상위 0을 보이게 할지의 여부를 지정한다. 예 : 90023일경우 9만0023으로 표시
	bool						m_bShowZeroPlus;		//0일때 플러스 표시를 할지 여부를 지정한다.

	ENUM_IMAGENUMBER_DRAWTYPE	m_nNumberType;			//이미지넘버 종류 지정
public:
	CTextNumber(void);
	virtual ~CTextNumber();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_TEXTNUMBER;}

	virtual const std::string GetUnit(void);
	virtual void SetUnit(const std::string &strUnit);

	virtual void SetMaxPlace(const int &nMaxPlace);
	virtual const int &GetMaxPlace(void){return m_nMaxPlace;}

	virtual void SetCutCount(const int &nCutCount);
	virtual const int GetCutCount(void){return m_nCutCount;}

	virtual void SetFirstUnit(const std::string &strFirstUnit);
	virtual const std::string &GetFirstUnit(void){return m_strFirstUnit;}

	virtual void SetPrefix(const std::string &strPrefix);
	virtual const std::string GetPrefix(void){return m_strPrefix;}

	virtual void SetUsePlus(const bool &bUse);
	virtual const bool &IsUsePlus(void){return m_bUsePlus;}

	virtual void SetUseMinus(const bool &bUse);
	virtual const bool &IsUseMinus(void){return m_bUseMinus;}

	virtual void SetShowZero(const bool &bShowZero);
	virtual const bool &IsShowZero(void){return m_bShowZero;}

	virtual void SetShowZeroNumber(const bool &bShowZeroNumber);
	virtual const bool &IsShowZeroNumber(void){return m_bShowZeroNumber;}

	virtual void SetShowZeroPlus(const bool &bShowZeroPlus);
	virtual const bool &IsShowZeroPlus(void){return m_bShowZeroPlus;}

	virtual void SetNumberType(const ENUM_IMAGENUMBER_DRAWTYPE &nType);
	virtual const ENUM_IMAGENUMBER_DRAWTYPE &GetNumberType(void){return m_nNumberType;}

	virtual void SetNumber(const INT64 &i64Number);
	virtual const INT64 &GetNumber(void){return m_i64Number;}

	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
protected:
	virtual int CheckRedraw(void);

	void CheckNumberDraw(std::string &strText, char *strNumber,int &nSize);
	void CheckCommaMoneyDraw(std::string &strText, char *strNumber,int &nSize);
	void CheckMoneyDraw(std::string &strText, char *strNumber,int &nSize);
	void CheckNeonsignMoneyDraw(std::string &strText, char *strNumber,int &nSize);
};
} //namespace LSY