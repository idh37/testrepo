#pragma once

#include "ObjectBase.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CImage :public CObjectBase
{
protected:
	NMBASE::GRAPHICGDI::xSprite		*m_pSprite;
	int			m_nIndex;			//�̹��� �ε���
	int			m_nTotalNum;

	//������ �̹���(Attach�� �̹������� ������ �̹����� �����Ͽ� �ϳ��� �̹����� ����ϴ� ���� ���Ѵ�)
	//�������϶� ���� �̹����� ����� Middle�̹����� �߰��� �̹�����, End�� �ϴ��� �̹����� ��Ÿ����.
	//�������϶� ���� �̹����� ������ Middle�̹����� �߰��� �̹�����, End�� ������ �̹����� ��Ÿ����.
	//MiddleCount�� �߰��̹����� ��� �ݺ������� ��Ÿ����.
	CImage		*m_pMiddleImage;
	CImage		*m_pEndImage;
	int			m_nMiddleCount;		//�ذ� �̹��� ����
	bool		m_bHorizontality;	//������ ������ ����, �������϶�
	LISTOBJECT	m_listMiddleImage;
public:
	CImage(void);
	virtual ~CImage();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_IMAGE;}

	bool Create(LPCTSTR lpPath);
	bool Create(NMBASE::GRAPHICGDI::xSprite *pSprite);

	virtual void DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMy(NMBASE::GRAPHICGDI::CPage *pPage);

	//ȭ�鿡 ǥ���� �̹��� �ε��� ����
	void SetIndex(int nIndex, bool bForce = false);
	//���� ǥ�õǴ� �ε����� �����´�.
	const int &GetIndex(void){return m_nIndex;};
	//��ü �̹��� ������ �����´�.
	const int &GetTotalNum(void);
	const CRect GetImageRect(int nIndex);

	void AttachMiddle(CImage *pImage);
	const CImage *GetMiddle(void){return m_pMiddleImage;}
	void DettachMiddle(void);

	void AttachEnd(CImage *pImage);
	const CImage *GetEnd(void){return m_pEndImage;}
	void DettachEnd(void);

	void SetMiddleCount(const int &nMiddleCount);
	const int &GetMiddleCount(void){return m_nMiddleCount;}

	void SetHorizontality(const bool &bHorizontality);
	const bool &IsHorizontality(void){return m_bHorizontality;}

	std::string GetFileName(void);
	bool IsPossibilityIndex(const int &nIndex);

	CImage *GetMiddleImage(int nIndex);
	int GetMiddleImageToIndex(CImage *pImage);
	NMBASE::GRAPHICGDI::xSprite	*GetSprite(void){return m_pSprite;}

	virtual void CheckEventDraw(void);
public:
	//Object ���� ���� �Լ���
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

protected:
	void MakeMiddleEndImage(void);
	void ReleaseMiddleEndImage(void);
};
} //namespace LSY