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
	int							m_nMaxPlace;			//ǥ���� �ִ� �ڸ����� ��Ÿ����. EIND_NUMBER, EIND_COMMA_MONEY�϶��� �����Ѵ�. ������ �ڸ������� ū ���̸� ��� 9�� ǥ���Ѵ�. 0�̸� ��� ǥ���Ѵ�.
	int							m_nCutCount;			//��������� ǥ�������� ��Ÿ���� ����, 0�̸� ��ü�� ǥ���Ѵ�. EIND_MONEY������ ��ȿ��
	std::vector<std::string>	m_listUnit;				//���� ǥ�� ���� ����Ʈ
	std::string					m_strFirstUnit;			//���� ���� ���� ����
	std::string					m_strPrefix;			//�տ� �߰��Ǵ� �����Ƚ� ����
	bool						m_bUsePlus;				//Plusǥ�� ��� ����
	bool						m_bUseMinus;			//Minusǥ�� ��� ����, false�̸� 0�̻��� ���� ǥ��
	bool						m_bShowZero;			//0�� ǥ�������� ����
	bool						m_bShowZeroNumber;		//���������� ���ʷ� 0�̿��� ���ڰ� ������ �������� ���� 0�� ���̰� ������ ���θ� �����Ѵ�. �� : 90023�ϰ�� 9��0023���� ǥ��
	bool						m_bShowZeroPlus;		//0�϶� �÷��� ǥ�ø� ���� ���θ� �����Ѵ�.

	ENUM_IMAGENUMBER_DRAWTYPE	m_nNumberType;			//�̹����ѹ� ���� ����
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

	//Object ���� ���� �Լ���
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