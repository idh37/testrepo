#pragma once

#include "../../Functor/MessageManager.h"
#include "../../Define.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT IObject
{
protected:
	CMessageManager		m_cMessage;		//마우스, 키보드등의 이벤트가 발생했을때 호출해줄 Founctor 관리자
	DWORD				m_dwLogicStartTime;	//로직이 시작된 시간을 가지고 있는 변수
public:
	IObject();
	virtual ~IObject();

	virtual void Logic(const DWORD &dwTime) = 0;

	//메시지 핸들러
	virtual bool AddHandler(int nCommond, IFunctor *pFunctor);
	virtual bool AppendHandler(int nCommond, IFunctor *pFunctor);
	virtual bool OverrideHandler(int nCommond, IFunctor *pFunctor);

	virtual bool RemoveHandler(int nCommond);
	virtual bool RemoveAppendHandler(int nCommond, IFunctor *pFunctor);
	virtual bool RemoveOverrideHandler(int nCommond, IFunctor *pFunctor);
public:
	virtual const DWORD &GetLogicStartTime(void){return m_dwLogicStartTime;}
protected:
	virtual void SetLogicStartTime(const DWORD &dwStartTime){m_dwLogicStartTime = dwStartTime;}
public:
	friend class CObjectProject;
};
} //namespace LSY