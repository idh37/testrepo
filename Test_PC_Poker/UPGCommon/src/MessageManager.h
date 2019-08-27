#pragma once

#include "Delegate.h"
#include <map>
#include <string>
#include <vector>

class UPG_COMMON_EXPORT CMessageManager
{
protected:
	CRITICAL_SECTION	m_cLock;

public:
	bool AddMsgH		(std::string stCommand, IFunctor* pFunctor);
	bool AppendMsgH		(std::string stCommand, IFunctor* pFunctor);
	bool OverrideMsgH	(std::string stCommand, IFunctor* pFunctor);
	LRESULT	Call		(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT	CallDefault	(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);
	
	bool RemoveMsgH		   (std::string stCommand);
	bool RemoveAppendMsgH  (std::string stCommand, IFunctor* pFunctor);	// Appand한 메시지 중 선택한 삭제한다.
	bool RemoveOverrideMsgH(std::string stCommand, IFunctor* pFunctor);	// Override메시지를 삭제한다.

public:
	SFINFOS	Info(std::string stCommand);
	void	Clear();
	void    DumpAllHandlers(bool bSort);

public:
	static	CMessageManager& Get();

private:
	CDelegate*	FindDelegate( std::string stCommand);

private:
	typedef	std::map< std::string, CDelegate>	MAP;
	typedef	MAP::iterator						IT;
	MAP	m_map;

	// singletone
public:
	//static CMessageManager& GetInstance() 
	//{
	//	if(destroyed) 
	//	{
	//		new(pInst) CMessageManager;
	//		atexit(killPhoenix);	// 종료될때 호출되는 예약 함수
	//		destroyed = false;
	//	} 
	//	else if(pInst == 0) 
	//	{
	//		create();
	//	}

	//	return *pInst;
	//}
private:
	CMessageManager();
	CMessageManager(const CMessageManager & other);
	~CMessageManager();

	/*static void create() 
	{
		static CMessageManager inst;
		pInst = &inst;
	}

	static void killPhoenix()
	{
		pInst->~CMessageManager();
	}*/

public:

	//static bool destroyed;
	//static CMessageManager* pInst;
};


UPG_COMMON_EXPORT CMessageManager& MM();