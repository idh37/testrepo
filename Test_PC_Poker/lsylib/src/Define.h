#pragma once

class TiXmlNode;
class TiXmlElement;

namespace LSY
{
#define DEF_REDRAW_TIMER	10000

#ifndef _ID_OBJECT_ROOT
#define _ID_OBJECT_ROOT		(int)0xFFFFFFFE
#define _ID_OBJECT_NOTUSE	(int)0xFFFFFFFF
#endif

#ifndef _ID_PROJECT_ROOT
#define _ID_PROJECT_ROOT		(int)0xFFFFFFFD
#define _ID_PROJECT_OBJECTRECT	(int)0xFFFFFFFE
#define _ID_PROJECT_NOTUSE		(int)0xFFFFFFFF
#endif

#ifndef LSY_UM_EVENT_DRAW
#define LSY_UM_EVENT_DRAW	(WM_USER+30000)
#endif

enum ENUM_OBJECTEFFECTTYPE
{
	EOET_UNKNOWN = -1,
	EOET_NORMAL,
	EOET_HALFALPHA,
	EOET_STEPALPHA,
	EOET_GLOW,
	EOET_GLAYSCALE,
	EOET_MONO,
	EOET_ANTI,
	EOET_LIGHT,
	EOET_DARKEN,
	EOET_ALPHA,
	EOET_MAX,
};

enum ENUM_OBJECTTYPE
{
	EOT_UNKNOWN = 0,	//�˼� ���� ��ü
	EOT_OBJECTRECT,		//��ü���� ǥ�� ��ü
	EOT_CREATERECT,		//��ü���� ǥ�� ��ü
	EOT_WINDOWS,		//������
	EOT_GROUP,			//�׷�
	EOT_IMAGE,			//�̹���
	EOT_DUMYIMAGE,		//�ӽ� �̹���(�ؽ�Ʈ��� ����� ����)
	EOT_BUTTON,			//��ư
	EOT_CHECKBUTTON,	//üũ��ư
	EOT_CHECKBOX,		//üũ�ڽ�
	EOT_RADIO,			//������ư
	EOT_TEXT,			//�ؽ�Ʈ
	EOT_TAB,			//����Ʈ��
	EOT_SLIDEBAR,		//�����̵��
	EOT_PROGRESSIVEBAR,	//���α׷��ú��
	EOT_SCROLLBAR,		//��ũ�ѹ�
	EOT_EDIT,			//����Ʈ
	EOT_EDITCURSOR,		//����ƮĿ��
	EOT_RICHTEXT,		//��ġ�ؽ�Ʈ(�Է¾��� ��ġ ����Ʈ, ��ũ���� Attach �ϸ� �Է��� ó���Ѵ�.)
	EOT_TEXTLIST,		//�ؽ�Ʈ����Ʈ
	EOT_COMBO,			//�޺��ڽ�
	EOT_LISTITEM,		//����Ʈ ������
	EOT_LIST,			//����Ʈ
	EOT_SPRITE,			//��������Ʈ
	EOT_SPRITESCENE,	//��������Ʈ ��
	EOT_WEB,				//��
	EOT_AVATAR,				//�ƹ�Ÿ					//=== Object�߰�
	EOT_SPRITEBUTTON,		//��������Ʈ ��ư
	EOT_SPRITECHECKBUTTON,	//��������Ʈ üũ��ư
	EOT_IMAGENUMBER,		//�̹�������
	EOT_SPRITEWINDOWS,		//��������Ʈ ������
	EOT_TEXTNUMBER,			//�ؽ�Ʈ����
	EOT_TEXTBUTTON,			//�ؽ�Ʈ��ư
};

enum ENUM_OBJECTDRAWTYPE
{
	EODT_NORMAL = 0,	//�Ϲ� �׸���
	EODT_SCALE,			//Ȯ��
	EODT_ROTATION,		//ȸ��
	EODT_SCALEROTATION,	//Ȯ��&ȸ��
};

enum ENUM_TEXTDRAW_TYPE
{
	ETDT_UNKNOWN = -1,
	ETDT_NORMAL,
	ETDT_SHADOW,
	ETDT_OUTLINE,
};

//�������� ����ϴ� ������
enum OBJECT_USERDATA_TYPE
{
	OUDT_TREEITEM = 0,
	OUDT_MESSAGE,
	OUDT_ENABLE,
	OUDT_USEMOUSEWHEEL,
	OUDT_USELOGIC,
};

//��ư ����
enum ENUM_BUTTON_STATUS
{
	EBS_NORMAL = 0,
	EBS_OVER,
	EBS_DOWN,
	EBS_DISABLE,
	EBS_SNORMAL,
	EBS_SOVER,
	EBS_SDOWN,
	EBS_SDISABLE,
	EBS_MAX,
};

//��ư�� ���� ����
enum ENUM_BUTTON_STATUS_TYPE
{
	EBST_FREESTATUS = 0,	//�����Ӱ� ��ư �ε��� ����
	EBST_ONESTATUS,			//1���� ��ư
	EBST_TWOSTATUS,			//2���� ��ư
	EBST_THREESTATUS,		//3���� ��ư
	EBST_FOURSTATUS,		//4���� ��ư
};

enum ENUM_IMAGENUMBER_DRAWTYPE
{
	EIND_UNKNOWN = -1,
	EIND_NUMBER,				//��� ���ڸ� �� ǥ���Ѵ�.
	EIND_COMMA_MONEY,			//���� ǥ���ϴ� ������� ���� ������ �Ѱ��� �޸��� ����Ѵ�. ù��° ������ �����Ҽ� ����(��, $ ��)
	EIND_MONEY,					//���� ǥ���Ѵ� ������� ���� �װ��� �ѳ��� ������ ����Ѵ�. ù��° ����(��, $ ��) ���� �����Ѵ�.
	EIND_NEONSIGN_MONEY,		//������(��ġ ����), ���� ǥ���Ѵ� ������� ���� �װ��� �ѳ��� ������ ����Ѵ�. ù��° ����(��, $ ��) ���� �����Ѵ�. ���������� ù��° 0�� ������ ��ġ������ ����д�.
};

class CObjectProject;
class CObjectBase;

class UPG_LSYLIB_EXPORT FONTINFO
{
protected:
	std::string			m_strName;
	int					m_nSize;
	bool				m_bBold;
	CFont				*m_pFont;
public:
	COLORREF			m_clrColor;
	COLORREF			m_clrOutline;
	ENUM_TEXTDRAW_TYPE	m_nDrawType;
	int					m_nSpaceBetweenLetters;
	UINT				m_uFormat;
public:
	FONTINFO();
	FONTINFO(const FONTINFO &font);
	virtual ~FONTINFO();

	FONTINFO *Clone(void);
	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlNode);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlFont, CObjectProject *pProject);

	void SetFont(const std::string &strName, const int &nSize, const bool &bBold);
	const std::string &GetName(void){return m_strName;}
	const int &GetSize(void){return m_nSize;}
	const bool &IsBold(void){return m_bBold;}
	const CFont *GetFont(void){return m_pFont;}

	const FONTINFO &operator=(const FONTINFO &font);
	friend bool operator==(const FONTINFO &z, const FONTINFO &w);
	friend bool operator!=(const FONTINFO &z, const FONTINFO &w);
};

typedef std::list<FONTINFO>			FONTINFOLIST;
typedef FONTINFOLIST::iterator		FONTINFOLISTITER;

UPG_LSYLIB_EXPORT extern bool operator==(const FONTINFOLIST &z, const FONTINFOLIST &w);
UPG_LSYLIB_EXPORT extern bool operator!=(const FONTINFOLIST &z, const FONTINFOLIST &w);

UPG_LSYLIB_EXPORT extern const float _cos(const float &fDegree);
UPG_LSYLIB_EXPORT extern const float _sin(const float &fDegree);

UPG_LSYLIB_EXPORT extern void GetPointerToString(IN BYTE *pSrc, IN size_t &nLen, OUT std::string &strOutput);

UPG_LSYLIB_EXPORT extern void GetFontDrawTypeToString(IN const ENUM_TEXTDRAW_TYPE &nType, OUT CString &strOutput);
UPG_LSYLIB_EXPORT extern ENUM_TEXTDRAW_TYPE GetFontStringToDrawType(std::string &strType);

UPG_LSYLIB_EXPORT extern void GetObjectTypeToString(IN const ENUM_OBJECTTYPE &nType, OUT std::string &strOutput);
UPG_LSYLIB_EXPORT extern ENUM_OBJECTTYPE GetStringToObjectType(const std::string &strType);

UPG_LSYLIB_EXPORT extern void GetEffectTypeToString(IN const ENUM_OBJECTEFFECTTYPE &nType, OUT std::string &strOutput);
UPG_LSYLIB_EXPORT extern ENUM_OBJECTEFFECTTYPE GetStringToEffectType(const std::string &strType);

UPG_LSYLIB_EXPORT extern std::string GetImageNumberDrawTypeToString(ENUM_IMAGENUMBER_DRAWTYPE &nType);
UPG_LSYLIB_EXPORT extern ENUM_IMAGENUMBER_DRAWTYPE GetStringToImageNumberDrawType(std::string strType);

UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, float &fValue);
UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, bool &bValue);
UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, int &nValue);
UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, unsigned int &nValue);
UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, std::string &strValue);
UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, DWORD &dwValue);
UPG_LSYLIB_EXPORT extern void GetAttribute(TiXmlElement *pNode, const std::string &strName, INT64 &i64Value);
UPG_LSYLIB_EXPORT extern void GetAttributeMime64(TiXmlElement *pNode, const std::string &strName, std::string &strValue);
UPG_LSYLIB_EXPORT extern void GetAttributeColor(TiXmlElement *pNode, const std::string &strName, COLORREF &clrValue);
UPG_LSYLIB_EXPORT extern void GetAttributeFormat(TiXmlElement *pNode, const std::string &strName, UINT &uFormat);

UPG_LSYLIB_EXPORT extern const float _PI;	// ���̰�
UPG_LSYLIB_EXPORT extern const float _PI_D2;	// PI/2
UPG_LSYLIB_EXPORT extern const float _PI_D8;	// PI/8
UPG_LSYLIB_EXPORT extern const float _PI_M2;	// PI*2
UPG_LSYLIB_EXPORT extern const float _PI_D180;	// PI/180
UPG_LSYLIB_EXPORT extern const float _PI_180D;	// 180/PI
} //namespace LSY