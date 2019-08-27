#pragma once

#include "Functor.h"
#include "Delegate.h"
#include <map>
#include <string>
#include <vector>

namespace LSY
{
class UPG_LSYLIB_EXPORT CMessageManager
{
protected:
	CRITICAL_SECTION	m_cLock;
public:
	bool AddMsgH		(int nCommand, IFunctor* pFunctor);
	bool AppendMsgH		(int nCommand, IFunctor* pFunctor);
	bool OverrideMsgH	(int nCommand, IFunctor* pFunctor);
	LRESULT	Call		(CMessage *pMsg);
	LRESULT	CallDefault	(CMessage *pMsg);
	
	bool RemoveMsgH		   (int nCommand);
	bool RemoveAppendMsgH  (int nCommand, IFunctor* pFunctor);	// Appand한 메시지 중 선택한 삭제한다.
	bool RemoveOverrideMsgH(int nCommand, IFunctor* pFunctor);	// Override메시지를 삭제한다.

public:
	SFINFOS	Info(int nCommand);
	void	Clear();
	void    DumpAllHandlers(bool bSort);

private:
	CDelegate*	FindDelegate(int nCommand);

private:
	typedef	std::map<int, CDelegate>			MAP;
	typedef	MAP::iterator						IT;
	MAP	m_map;

public:
	CMessageManager();
	~CMessageManager();
};
} //namespace LSY