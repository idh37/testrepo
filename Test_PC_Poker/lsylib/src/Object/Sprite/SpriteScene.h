#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CImage;
class UPG_LSYLIB_EXPORT CSpriteScene : public CObjectBase
{
protected:
	NMBASE::GRAPHICGDI::xSprite						*m_pSprite;
	NMBASE::GRAPHICGDI::SceneData					*m_pScene;
	int							m_nSceneIndex;

	typedef std::list<CImage *>	LISTLAYER;
	typedef LISTLAYER::iterator LISTLAYERITER;

	LISTLAYER					m_listLayer;
	int							m_nMaxLayer;
public:
	CSpriteScene(void);
	virtual ~CSpriteScene();

	virtual bool Create(NMBASE::GRAPHICGDI::xSprite *pSprite, int nSceneIndex);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_SPRITESCENE;}

	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual void Logic(const DWORD &dwTime, const BOOL &bLoop);
	virtual bool IsIn(IN CPoint &ptPos, OUT bool &bIn);
protected:
	virtual void RemoveAllLayer(void);
	virtual void MakeLayer(void);
	virtual BOOL GetDrawingCellInfo(NMBASE::GRAPHICGDI::FrameData* pFrame, NMBASE::GRAPHICGDI::FrameData* pNextFrame, int cell, float fdelta, CImage *pLayer);
};
} //namespace LSY