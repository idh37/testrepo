#pragma once

#include "ObjectBase.h"

class NMBASE::AVATAR::CAvatar;
namespace LSY
{
#define MAX_AVATAR_INFO 26

class CDumyImage;
class UPG_LSYLIB_EXPORT CAvatar :public CObjectBase
{
protected:
	NMBASE::AVATAR::CAvatar		*m_pAvatar;
	CDumyImage					*m_pImage;
	int							m_AvatarInfo[MAX_AVATAR_INFO];
	DWORD						m_dwStartTime;
	DWORD						m_dwCurTime;
public:
	CAvatar(void);
	virtual ~CAvatar();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_AVATAR;}

	virtual bool Create(int nWidth, int nHeight);
	virtual bool SetAvatar(short *pAvatarInfo);
	virtual bool SetAvatar(int *pAvatarInfo);

	virtual bool IsPossibilityReSize(void){return true;}

	virtual void SetLogicStartTime(const DWORD &dwStartTime);
	virtual void Logic(const DWORD &dwTime);
	virtual void SetObjectProject(CObjectProject *pProject);

	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
};
} //namespace LSY