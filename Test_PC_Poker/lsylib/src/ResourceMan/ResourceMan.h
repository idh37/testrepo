#pragma once

#include <string>
#include <map>

class NMBASE::GRAPHICGDI::xSprite;

namespace LSY
{
class UPG_LSYLIB_EXPORT CResourceMan
{
protected:
	class CResourceUnit
	{
	protected:
		int				m_nReferenceCount;
		NMBASE::GRAPHICGDI::xSprite			*m_pSprite;
	public:
		CResourceUnit():m_nReferenceCount(1),m_pSprite(NULL){}
		virtual ~CResourceUnit(){}

		friend class CResourceMan;
	};

	class CFontUnit
	{
	protected:
		std::string		m_strFontName;
		int				m_nSize;
		bool			m_bBold;
		int				m_nReferenceCount;
		CFont			*m_pFont;
	public:
		CFontUnit():m_nReferenceCount(1),m_pFont(NULL){}
		virtual ~CFontUnit(){}

		friend class CResourceMan;
	};

	CRITICAL_SECTION	m_cLock;
	std::map<std::string, CResourceUnit>	m_listResource;
	std::list<CFontUnit>					m_listFont;
public:
	CResourceMan(void);
	virtual ~CResourceMan();

	NMBASE::GRAPHICGDI::xSprite *GetSprite(LPCTSTR lpPath);
	bool RemoveSprite(NMBASE::GRAPHICGDI::xSprite *pSprite);

	CFont *GetFont(LPCTSTR lpFontName, int nSize, bool bBold);
	bool RemoveFont(CFont *pFont);

	NMBASE::GRAPHICGDI::xSprite *Clone(NMBASE::GRAPHICGDI::xSprite *pSprite);
	CFont *Clone(CFont *Font);

	static CResourceMan &GetInstance(void);
};

CResourceMan &RM();
} //namespace LSY