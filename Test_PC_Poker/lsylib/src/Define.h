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
	EOT_UNKNOWN = 0,	//알수 없는 객체
	EOT_OBJECTRECT,		//객체영역 표시 객체
	EOT_CREATERECT,		//객체영역 표시 객체
	EOT_WINDOWS,		//윈도우
	EOT_GROUP,			//그룹
	EOT_IMAGE,			//이미지
	EOT_DUMYIMAGE,		//임시 이미지(텍스트등에서 사용할 예정)
	EOT_BUTTON,			//버튼
	EOT_CHECKBUTTON,	//체크버튼
	EOT_CHECKBOX,		//체크박스
	EOT_RADIO,			//라디오버튼
	EOT_TEXT,			//텍스트
	EOT_TAB,			//탭컨트롤
	EOT_SLIDEBAR,		//슬라이드바
	EOT_PROGRESSIVEBAR,	//프로그래시브바
	EOT_SCROLLBAR,		//스크롤바
	EOT_EDIT,			//에디트
	EOT_EDITCURSOR,		//에디트커서
	EOT_RICHTEXT,		//리치텍스트(입력없는 리치 에디트, 스크롤을 Attach 하면 입력을 처리한다.)
	EOT_TEXTLIST,		//텍스트리스트
	EOT_COMBO,			//콤보박스
	EOT_LISTITEM,		//리스트 아이템
	EOT_LIST,			//리스트
	EOT_SPRITE,			//스프라이트
	EOT_SPRITESCENE,	//스프라이트 씬
	EOT_WEB,				//웹
	EOT_AVATAR,				//아바타					//=== Object추가
	EOT_SPRITEBUTTON,		//스프라이트 버튼
	EOT_SPRITECHECKBUTTON,	//스프라이트 체크버튼
	EOT_IMAGENUMBER,		//이미지숫자
	EOT_SPRITEWINDOWS,		//스프라이트 윈도우
	EOT_TEXTNUMBER,			//텍스트숫자
	EOT_TEXTBUTTON,			//텍스트버튼
};

enum ENUM_OBJECTDRAWTYPE
{
	EODT_NORMAL = 0,	//일반 그리기
	EODT_SCALE,			//확대
	EODT_ROTATION,		//회전
	EODT_SCALEROTATION,	//확대&회전
};

enum ENUM_TEXTDRAW_TYPE
{
	ETDT_UNKNOWN = -1,
	ETDT_NORMAL,
	ETDT_SHADOW,
	ETDT_OUTLINE,
};

//툴에서만 사용하는 데이터
enum OBJECT_USERDATA_TYPE
{
	OUDT_TREEITEM = 0,
	OUDT_MESSAGE,
	OUDT_ENABLE,
	OUDT_USEMOUSEWHEEL,
	OUDT_USELOGIC,
};

//버튼 상태
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

//버튼의 상태 종류
enum ENUM_BUTTON_STATUS_TYPE
{
	EBST_FREESTATUS = 0,	//자유롭게 버튼 인덱스 지정
	EBST_ONESTATUS,			//1상태 버튼
	EBST_TWOSTATUS,			//2상태 버튼
	EBST_THREESTATUS,		//3상태 버튼
	EBST_FOURSTATUS,		//4상태 버튼
};

enum ENUM_IMAGENUMBER_DRAWTYPE
{
	EIND_UNKNOWN = -1,
	EIND_NUMBER,				//모든 숫자를 다 표시한다.
	EIND_COMMA_MONEY,			//돈을 표시하는 방식으로 숫자 세개당 한개씩 콤마를 사용한다. 첫번째 단위는 지정할수 있음(원, $ 등)
	EIND_MONEY,					//돈을 표시한느 방식으로 숫자 네개당 한나씩 단위를 사용한다. 첫번째 단위(원, $ 등) 따로 지정한다.
	EIND_NEONSIGN_MONEY,		//전광판(위치 고정), 돈을 표시한는 방식으로 숫자 네개당 한나씩 단위를 사용한다. 첫번째 단위(원, $ 등) 따로 지정한다. 각단위에서 첫번째 0이 나오는 위치까지를 비워둔다.
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

UPG_LSYLIB_EXPORT extern const float _PI;	// 파이값
UPG_LSYLIB_EXPORT extern const float _PI_D2;	// PI/2
UPG_LSYLIB_EXPORT extern const float _PI_D8;	// PI/8
UPG_LSYLIB_EXPORT extern const float _PI_M2;	// PI*2
UPG_LSYLIB_EXPORT extern const float _PI_D180;	// PI/180
UPG_LSYLIB_EXPORT extern const float _PI_180D;	// 180/PI
} //namespace LSY