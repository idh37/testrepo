#pragma once

#include "../../Functor/MessageManager.h"
#include "../../Define.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT IObject
{
protected:
	CMessageManager		m_cMessage;		//���콺, Ű������� �̺�Ʈ�� �߻������� ȣ������ Founctor ������
	DWORD				m_dwLogicStartTime;	//������ ���۵� �ð��� ������ �ִ� ����
public:
	IObject();
	virtual ~IObject();

	virtual void Logic(const DWORD &dwTime) = 0;

	//�޽��� �ڵ鷯
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