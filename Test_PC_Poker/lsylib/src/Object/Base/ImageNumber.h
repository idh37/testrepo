#pragma once

#include "ObjectBase.h"

namespace LSY
{
class CImage;

class UPG_LSYLIB_EXPORT CImageNumber :public CObjectBase
{
protected:
	CImage						*m_pImage;
	int							m_nStartIndex;			//�̹����� ������ ���� �ε����� �������ش�.
	INT64						m_i64Number;
	int							m_nMaxPlace;			//ǥ���� �ִ� �ڸ����� ��Ÿ����. EIND_NUMBER, EIND_COMMA_MONEY�϶��� �����Ѵ�. ������ �ڸ������� ū ���̸� ��� 9�� ǥ���Ѵ�. 0�̸� ��� ǥ���Ѵ�.
	int							m_nCutCount;			//��������� ǥ�������� ��Ÿ���� ����, 0�̸� ��ü�� ǥ���Ѵ�. EIND_MONEY������ ��ȿ��
	int							m_nUnitStartIndex;		//EIND_MONEY, EIND_NEONSIGN_MONEY������ ������ ���� �ε���, EIND_COMMA_MONEY������ �޸��� �ε����� ������
	int							m_nUnitIndexGap;		//������ �����Ҷ� �ε����� ��ȭ��, EIND_MONEY, EIND_NEONSIGN_MONEY������ ��ȿ��
	int							m_nFirstUnitIndex;		//���� ���� ������ �ε���
	int							m_nPrefixIndex;			//�տ� �߰��Ǵ� �����Ƚ� �̹��� �ε���
	int							m_nPlusIndex;			//Plusǥ�� �̹��� �ε���
	int							m_nMinusIndex;			//Minusǥ�� �̹��� �ε���, -1�̸� 0�̻��� ������ ǥ���Ѵ�.
	bool						m_bShowZero;			//0�� ǥ�������� ����
	bool						m_bShowZeroNumber;		//���������� ���ʷ� 0�̿��� ���ڰ� ������ �������� ���� 0�� ���̰� ������ ���θ� �����Ѵ�. �� : 90023�ϰ�� 9��0023���� ǥ��
	bool						m_bShowZeroPlus;		//0�϶� �÷��� ǥ�ø� ���� ���θ� �����Ѵ�.
	bool						m_bRedraw;

	UINT						m_uFormat;				//���� ���
	ENUM_IMAGENUMBER_DRAWTYPE	m_nNumberType;			//�̹����ѹ� ���� ����

	std::list<CImage *>			m_listImage;
public:
	CImageNumber(void);
	virtual ~CImageNumber();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_IMAGENUMBER;}
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	bool Create(LPCTSTR lpPath, int nWidth, int nHeight);

	//��ü �̹��� ������ �����´�.
	const int GetTotalNum(void);
	void SetRedraw(bool bRedraw = true){m_bRedraw = bRedraw;}
	const bool &IsRedraw(void){return m_bRedraw;}

	std::string GetFileName(void);
public:
	//Object ���� ���� �Լ���
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