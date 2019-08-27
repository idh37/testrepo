#pragma once

class CPromotionManager;

class UPG_GAMEBASE_EXPORT CPromotionBase
{
protected:
	bool				m_bActive;
	bool				m_bDraw;
	bool				m_bShowTab;
	bool				m_bComplete;
	CPromotionManager	*m_pManager;
	int					m_nType;
	CPoint				m_ptEndBenner;
public:
	CPromotionBase(CPromotionManager *pManager, int nType);
	virtual ~CPromotionBase();

	virtual const bool &IsActive(void){return m_bActive;}
	virtual void SetActive(const bool &bActive){m_bActive = bActive;}

	virtual const bool IsShowTab(void);
	virtual void SetShowTab(bool show);

	virtual const bool &IsDraw(void){return m_bDraw;}
	virtual void SetDraw(const bool &bDraw){m_bDraw = bDraw;}

	virtual void SetComplete(const bool &bComplete){m_bComplete = bComplete;}
	virtual const bool &IsComplete(void){return m_bComplete;}

	virtual const int &GetType(void){return m_nType;}

	virtual BOOL OnLButtonDownBottom(int &x , int &y);
	virtual BOOL OnLButtonUpBottom(int &x , int &y);
	virtual BOOL OnMouseMoveBottom(int &x , int &y);

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	virtual BOOL OnLButtonDownTop(int &x , int &y);
	virtual BOOL OnLButtonUpTop(int &x , int &y);
	virtual BOOL OnMouseMoveTop(int &x , int &y);

	virtual BOOL OnEventTabLButtonDown(int &x , int &y);
	virtual BOOL OnEventTabLButtonUp(int &x , int &y);
	virtual BOOL OnEventTabMouseMove(int &x , int &y);

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);

	virtual void OnTimer();
	
	virtual void OnStartGame();
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();
	virtual void OnEnterRoom(){}
	virtual void CurPromotionEventTab(bool bShow){}

	virtual void OnEndGame();	//게임이 끝나고 리셋 될때.
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);

	virtual TYPE_ENDPOPUP GetEndPopupType(void);
	virtual bool InitEndPopup(CEndPopup *pEndPopup, CPoint ptPos);
	virtual bool OnPaintEndPopup(CPaintDC *pDC);
	virtual bool OnClickEndPopup(UINT nID);
	virtual void OnDestroyEndPopup(void){}

	virtual int GetPriorities(void);
};

class CEffectBase
{
public:
	CEffectBase(){
		m_bShowEffect=FALSE;
		m_bEffectMulti=FALSE;
		m_bIsLoop = FALSE ;
		m_dwEffectStartTime=0;
		m_dwEffectTotalPlayTime=0;
		m_pSprEffect=NULL;
		m_nPosX = 0;
		m_nPosY = 0;
		m_dwEffectLoopStartTime=0;
		m_dwEffectLoopPlayTime=0;
	}
	virtual ~CEffectBase(){}
public:
	DWORD m_dwEffectStartTime;	
	DWORD m_dwEffectTotalPlayTime;	
	DWORD m_dwEffectLoopStartTime;
	DWORD m_dwEffectLoopPlayTime;
protected:
	BOOL m_bShowEffect, m_bEffectMulti, m_bIsLoop;
	NMBASE::GRAPHICGDI::xSprite	*m_pSprEffect;					//이펙트 spr
	int m_nEffectNumber;
	int m_nPosX;
	int m_nPosY;
public:
	NMBASE::GRAPHICGDI::xSprite	*GetSprEffect(){ return m_pSprEffect; }					//이펙트 spr
	void SetSprite( NMBASE::GRAPHICGDI::xSprite *pSprEffect){
		m_pSprEffect = pSprEffect;
	}
	void SetPos(int nPosX, int nPosY){
		m_nPosX = nPosX;
		m_nPosY = nPosY;
	}
	void SetLoopPlayTime(DWORD dwLoopPlayTime){
		m_dwEffectLoopPlayTime=dwLoopPlayTime;
		m_bIsLoop = (0<m_dwEffectLoopPlayTime);
	}
	//실제로 이펙트를 초기화한다.
	void ShowEffect(int nEffectNumber, BOOL bEffectMulti = FALSE, BOOL bEffectLoop = FALSE )
	{
		if(NULL==m_pSprEffect)
			return;

		m_bShowEffect = TRUE;
		m_bEffectMulti = bEffectMulti;
		m_dwEffectLoopStartTime = m_dwEffectStartTime=timeGetTime();
		m_nEffectNumber = nEffectNumber;
		m_bIsLoop = bEffectLoop;
		
		if(m_bEffectMulti == TRUE)
		{
			m_dwEffectTotalPlayTime = m_pSprEffect->GetMScenePlayTime(m_nEffectNumber);
		}
		else if(m_bEffectMulti == FALSE)
		{
			m_dwEffectTotalPlayTime = m_pSprEffect->GetScenePlayTime(m_nEffectNumber);
		}
	}
	void ChangeScean(int nTrack, int nSceneNo)
	{
		if(NULL==m_pSprEffect) return;
		NMBASE::GRAPHICGDI::MSceneData* ppMS = m_pSprEffect->multiscene.ppMScene[m_nEffectNumber];
		if(NULL==ppMS) return;
		if(ppMS->TotTrackNum<=nTrack) return;
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = (NMBASE::GRAPHICGDI::TRACK_SCENE*)ppMS->ppTrack[nTrack]->GetDataScene();
		if(NULL==pScnData) return;
		pScnData->SceneNo=nSceneNo;
	}
	void ChangeCell(int nScene, int nFrame, int nCell, int nSpr){
		if(NULL==m_pSprEffect) return;
		NMBASE::GRAPHICGDI::CellData* pCellData=m_pSprEffect->scene.GetCellPt(nScene, nFrame, nCell);
		if(NULL!=pCellData){
			pCellData->SprNo=nSpr;
		}
	}
	void ResetEffect(){
		m_bShowEffect=FALSE;
		m_dwEffectStartTime=timeGetTime();
		m_dwEffectTotalPlayTime = 0;
		m_nEffectNumber = 0;
	}
	//시간에 따른 프레임의 이미지를 보여준다.
	bool Draw(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
	{
		if(m_pSprEffect==NULL || m_bShowEffect==FALSE){
			return false;
		}
		DWORD dwNow = timeGetTime();
		DWORD dwElapsed = dwNow - m_dwEffectStartTime;
		if((TRUE==m_bIsLoop)&&(0 < m_dwEffectLoopPlayTime)){			
			if(m_dwEffectLoopPlayTime < (dwNow - m_dwEffectLoopStartTime)){
				ResetEffect();
				return true;
			}
		}
		if (m_dwEffectTotalPlayTime < dwElapsed){
			if(m_bIsLoop == TRUE){
				//dwElapsed = dwElapsed - m_dwEffectTotalPlayTime ; 
				m_dwEffectStartTime=dwNow;
				dwElapsed = 0; 
			}
			else{
				ResetEffect();
				return true;
			}
		}
		if(m_bEffectMulti == TRUE ){
			pPage->DrawMultiScene(m_nPosX, m_nPosY, m_pSprEffect, m_nEffectNumber, dwElapsed, FALSE);
		}		
		else if(m_bEffectMulti == FALSE ){
			pPage->DrawScene(m_nPosX, m_nPosY, m_pSprEffect, m_nEffectNumber, dwElapsed, FALSE);
		}
		return false;
	}
	BOOL IsShow(){
		return m_bShowEffect;
	}
};
