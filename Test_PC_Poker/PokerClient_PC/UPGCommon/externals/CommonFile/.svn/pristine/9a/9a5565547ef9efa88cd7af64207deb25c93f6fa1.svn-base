#pragma once

// TMS Protocol
#include "./CommMsg.h"
#include "./CommMsgIDHeader.h"


// 서버에서는 Camel::Dar::TMS::State 등이 중복 선언되는걸 방지하기 위해 TMSCommon.h 를 사용하지 않는다.
#ifdef _ONLY_SERVER
//	#include "../../externals/TMS/GS2TS/TMSProtocol.h"
#else
	#include "TMSCommon.h"
#endif // _ONLY_SERVER


	// ////////////////////////
	// 토너먼트  시작 공지
	// ////////////////////////
namespace TMS
{
	struct _TMS_UserInfo
	{
		char strId[16];
		char strNickname[20];
		__int64 llChip;
		int ePos;
		int nGameCount;
		int nRank;


		virtual void Clear()
		{
			// Clear Member strId
			::memset(strId, 0, sizeof(char)*16);
			// Clear Member strNickname
			::memset(strNickname, 0, sizeof(char)*20);
			// Clear Member llChips
			::memset(&llChip, 0, sizeof(llChip));
			// Clear Member ePos
			::memset(&ePos, 0, sizeof(ePos));
			// Clear Member nGameCount
			::memset(&nGameCount, 0, sizeof(nGameCount));
			// Clear Member nRank
			::memset(&nRank, 0, sizeof(nRank));
		}

		bool Copy(const _TMS_UserInfo& rhs)
		{
			if (this == &rhs)
			{
				return false;
			}
			Clear();
			// copy Member strId
			sprintf_s(strId, 15, "%s", rhs.strId);
			// copy Member strNickname
			sprintf_s(strNickname, 20, "%s", rhs.strNickname);
			// copy Member llChips
			llChip = rhs.llChip;
			// copy Member ePos
			ePos = rhs.ePos;
			// copy Member nGameCount
			nGameCount = rhs.nGameCount;
			// copy Member nRank
			nRank = rhs.nRank;
			return true;
		}
	};

	struct _TMS_UserRat
	{

		char strId[16];
		__int64 llTotalPrizeChips;
		int nFinalWinCount;
		int nEntryWinCount;
		int nFinalEntryCount;
		int nEntryCount;
		bool bReserved;

		virtual void Clear()
		{
			// Clear Member strId
			::memset(&strId, 0, sizeof(char)*16);
			// Clear Member llTotalPrizeChips
			::memset(&llTotalPrizeChips, 0, sizeof(llTotalPrizeChips));
			// Clear Member nFinalWinCount
			::memset(&nFinalWinCount, 0, sizeof(nFinalWinCount));
			// Clear Member nEntryWinCount
			::memset(&nEntryWinCount, 0, sizeof(nEntryWinCount));
			// Clear Member nFinalEntryCount
			::memset(&nFinalEntryCount, 0, sizeof(nFinalEntryCount));
			// Clear Member nEntryCount
			::memset(&nEntryCount, 0, sizeof(nEntryCount));
			// Clear Member bReserved;
			bReserved = false;
		}

		bool Copy(const _TMS_UserRat& rhs)
		{
			if (this == &rhs)
			{
				return false;
			}
			Clear();
			// copy Member strId
			sprintf_s(strId, 15, "%s", rhs.strId);
			// copy Member llTotalPrizeChips
			llTotalPrizeChips = rhs.llTotalPrizeChips;
			// copy Member nFinalWinCount
			nFinalWinCount = rhs.nFinalWinCount;
			// copy Member nEntryWinCount
			nEntryWinCount = rhs.nEntryWinCount;
			// copy Member nFinalEntryCount
			nFinalEntryCount = rhs.nFinalEntryCount;
			// copy Member nEntryCount
			nEntryCount = rhs.nEntryCount;
			bReserved = rhs.bReserved;
			return true;
		}
	};

	struct _TMS_TournamentInfo
	{
		int nInning;
		int eState;
		int nEntryCount;
		int nAliveCount;
		__int64 tStartTime;
		__int64 tPlayTime;

		virtual void Clear()
		{
			// Clear Member nInning
			::memset(&nInning, 0, sizeof(nInning));
			// Clear Member eState
			::memset(&eState, 0, sizeof(eState));
			// Clear Member nEntryCount
			::memset(&nEntryCount, 0, sizeof(nEntryCount));
			// Clear Member nAliveCount
			::memset(&nAliveCount, 0, sizeof(nAliveCount));
			// Clear Member tStartTime
			::memset(&tStartTime, 0, sizeof(tStartTime));
			// Clear Member tPlayTime
			::memset(&tPlayTime, 0, sizeof(tPlayTime));
		}

		bool Copy(const _TMS_TournamentInfo& rhs)
		{
			if (this == &rhs)
			{
				return false;
			}
			Clear();
			// copy Member nInning
			nInning = rhs.nInning;
			// copy Member eState
			eState = rhs.eState;
			// copy Member nEntryCount
			nEntryCount = rhs.nEntryCount;
			// copy Member nAliveCount
			nAliveCount = rhs.nAliveCount;
			// copy Member tStartTime
			tStartTime = rhs.tStartTime;
			// copy Member tPlayTime
			tPlayTime = rhs.tPlayTime;
			return true;
		}
	};

	class ResultInfo
	{
	public:
		char Nickname[16];
		__int64 PrizeMoney;
	};

	class CGS2CL_StartTournament_Noti : public CCommMsg		// 토너먼트 시작 공지 (GS->CL)
	{
	public:
		Camel::Dar::TMS::State::Type* eState;				// 토너먼트 종류 (0:예선 시작, 1:본선 시작, 2:결선 시작)
		int* nEntryCount;
		void Set(const int EState, const int NEntryCount)
		{
			SetHeader(GS2CL_STARTTOURNAMENT_NOTI);
			PushData(_PVAR(eState), &EState, sizeof(*eState));
			PushData(_PVAR(nEntryCount), &NEntryCount, sizeof(*nEntryCount));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(eState), sizeof(*eState));
			TakeData(_PVAR(nEntryCount), sizeof(*nEntryCount));

		}
	};

	struct SetMoneyInfo
	{
		char strId[16];
		__int64 llPrizeMoney;
	};

	// ////////////////////////
	// 토너먼트 종료 공지
	// ////////////////////////
	class CGS2CL_EndTournament_Noti : public CCommMsg		// 토너먼트 종료 공지 (GS->CL)
	{
	public:
		Camel::Dar::TMS::State::Type* eState;			// 토너먼트 종류 (0:예선 종료, 1:본선종료, 2:결선종료)
		unsigned int* nEntryCount;			// 전체 참가자
		unsigned int* nAliveCount;			// 생존자
		int* TotUser;						// 토탈 유저
		_TMS_UserInfo *arrayUI;				// 유저 정보
		void Set(const int EState, const int NEntryCount, const int NAliveCount, std::vector<Camel::Dar::TMS::UserInfo*>& rArrayUI)
		{
			SetHeader(GS2CL_ENDTOURNAMENT_NOTI);
			PushData(_PVAR(eState), &EState, sizeof(*eState));
			PushData(_PVAR(nEntryCount), &NEntryCount, sizeof(*nEntryCount));
			PushData(_PVAR(nAliveCount), &NAliveCount, sizeof(*nAliveCount));
			int totuser = rArrayUI.size();
			PushData(_PVAR(TotUser), &totuser, sizeof(*TotUser));
			for ( size_t i = 0; i < totuser; ++i )
			{
				_TMS_UserInfo tempUI;
				if(!rArrayUI[i]->strId.empty()) strncpy(tempUI.strId, rArrayUI[i]->strId.c_str(), 15);
				if(!rArrayUI[i]->strNickname.empty()) strncpy(tempUI.strNickname, rArrayUI[i]->strNickname.c_str(), 19);
				tempUI.ePos = (int)rArrayUI[i]->ePos;
				tempUI.llChip = rArrayUI[i]->llChips;
				tempUI.nGameCount = rArrayUI[i]->nGameCount;
				tempUI.nRank = rArrayUI[i]->nRank;
				PushData(_PVAR(arrayUI), &tempUI, sizeof(*arrayUI));
			}
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(eState), sizeof(*eState));
			TakeData(_PVAR(nEntryCount), sizeof(*nEntryCount));
			TakeData(_PVAR(nAliveCount), sizeof(*nAliveCount));
			TakeData(_PVAR(TotUser), sizeof(*TotUser));
			TakeData(_PVAR(arrayUI), sizeof(*arrayUI) * (*TotUser));
		}
	};

	// ////////////////////////
	// 토너먼트 참가 신청
	// ////////////////////////
	class CCL2GS_RegisterUser_Req : public CCommMsg				// 토너먼트 참가 신청 (CL->GS)
	{
	public:
		char* strId;				// 참가 신청자 아이디
		void Set(const char *Id)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(CL2GS_REGISTERUSER_REQ);
			PushData(_PVAR(strId), _strid, 16);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);

		}
	};

	class CCL2GS_RegisterUser_Ans : public CCommMsg				// 토너먼트 참가 신청 결과 (GS->CL)
	{
	public:
		Camel::Dar::TMS::ErrorCode::Type* eResultCode;			// 참가 신청 결과
		_TMS_UserInfo *stUserInfo;
		void Set(const int EResultCode, const Camel::Dar::TMS::UserInfo *CLUserInfo)
		{
			SetHeader(CL2GS_REGISTERUSER_ANS);
			PushData(_PVAR(eResultCode), &EResultCode, sizeof(*eResultCode));
			_TMS_UserInfo tempUserInfo;
			if(CLUserInfo->strId.c_str() != NULL) strncpy(tempUserInfo.strId, CLUserInfo->strId.c_str(), 15);
			if(CLUserInfo->strNickname.c_str() != NULL) strncpy(tempUserInfo.strNickname, CLUserInfo->strNickname.c_str(), 19);
			tempUserInfo.llChip = CLUserInfo->llChips;
			tempUserInfo.ePos = CLUserInfo->ePos;
			tempUserInfo.nGameCount = CLUserInfo->nGameCount;
			tempUserInfo.nRank = CLUserInfo->nRank;
			PushData(_PVAR(stUserInfo), &tempUserInfo, sizeof(*stUserInfo));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(eResultCode), sizeof(*eResultCode));
			TakeData(_PVAR(stUserInfo), sizeof(*stUserInfo));

		}
	};

	class CGS2CL_RegisterUser_Noti : public CCommMsg			// 토너먼트 참가 공지 (GS->CL)
	{
	public:
		_TMS_UserInfo *stUserInfo;
		bool *bReserved;
		void Set(const Camel::Dar::TMS::UserInfo *CLUserInfo, const bool Reserved)
		{
			SetHeader(GS2CL_REGISTERUSER_NOTI);
			_TMS_UserInfo tempUserInfo;
			if(CLUserInfo->strId.c_str() != NULL) strncpy(tempUserInfo.strId, CLUserInfo->strId.c_str(), 15);
			if(CLUserInfo->strNickname.c_str() != NULL) strncpy(tempUserInfo.strNickname, CLUserInfo->strNickname.c_str(), 19);
			tempUserInfo.llChip = CLUserInfo->llChips;
			tempUserInfo.ePos = CLUserInfo->ePos;
			tempUserInfo.nGameCount = CLUserInfo->nGameCount;
			tempUserInfo.nRank = CLUserInfo->nRank;
			PushData(_PVAR(stUserInfo), &tempUserInfo, sizeof(*stUserInfo));
			PushData(_PVAR(bReserved), &Reserved, sizeof(*bReserved));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(stUserInfo), sizeof(*stUserInfo));
			TakeData(_PVAR(bReserved), sizeof(*bReserved));

		}
	};

	// ////////////////////////
	// 토너먼트 전적 요청
	// ////////////////////////
	class CCL2GS_GetUserRat_Req : public CCommMsg
	{
	public:
		char* strId;
		void Set(const char *Id)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(CL2GS_GETUSERRAT_REQ);
			PushData(_PVAR(strId), _strid, 16);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);

		}
	};

	class CCL2GS_GetUserRat_Ans : public CCommMsg
	{
	public:
		Camel::Dar::TMS::ErrorCode::Type* eResultCode;
		_TMS_UserRat *stUserRat;
		int* nMaxUserCnt;				// 전체 참여 가능자
		int* nRegUserCnt;					// 현재 참여자
		int* nReserveRegUserCnt;			// 사전 예약 참여자
		void Set(const int EResultCode, const Camel::Dar::TMS::UserRat *CLUserRat, const int NTotalUserCnt, const int NRegUserCnt, const int NReserveRegUserCnt)
		{
			SetHeader(CL2GS_GETUSERRAT_ANS);
			PushData(_PVAR(eResultCode), &EResultCode, sizeof(*eResultCode));
			_TMS_UserRat TempUsrRat;
			if(!CLUserRat->strId.empty()) strncpy(TempUsrRat.strId, CLUserRat->strId.c_str(), 15);
			TempUsrRat.llTotalPrizeChips = CLUserRat->llTotalPrizeChips;
			TempUsrRat.nEntryCount = CLUserRat->nEntryCount;
			TempUsrRat.nEntryWinCount = CLUserRat->nEntryWinCount;
			TempUsrRat.nFinalEntryCount = CLUserRat->nFinalEntryCount;
			TempUsrRat.nFinalWinCount = CLUserRat->nFinalWinCount;
			TempUsrRat.bReserved = CLUserRat->bReserved;
			PushData(_PVAR(stUserRat), &TempUsrRat, sizeof(*stUserRat));
 			PushData(_PVAR(nMaxUserCnt), &NTotalUserCnt, sizeof(*nMaxUserCnt));
 			PushData(_PVAR(nRegUserCnt), &NRegUserCnt, sizeof(*nRegUserCnt));
 			PushData(_PVAR(nReserveRegUserCnt), &NReserveRegUserCnt, sizeof(*nReserveRegUserCnt));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(eResultCode), sizeof(*eResultCode));
			TakeData(_PVAR(stUserRat), sizeof(*stUserRat));
			TakeData(_PVAR(nMaxUserCnt), sizeof(*nMaxUserCnt));
			TakeData(_PVAR(nRegUserCnt), sizeof(*nRegUserCnt));
			TakeData(_PVAR(nReserveRegUserCnt), sizeof(*nReserveRegUserCnt));
		}
	};

	// ////////////////////////
	// 토너먼트 참가 취소
	// ////////////////////////
	class CGS2CL_UnRegisterUser_Noti : public CCommMsg			// 토너먼트 참가 취소 공지 (GS->CL)
	{
	public:
		char* strId;				// 참가 취소 신청자 아이디			// 참가 신청자 아이디
		bool *bReserved;
		void Set(const char *Id, const bool Reserved)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(GS2CL_UNREGISTERUSER_NOTI);
			PushData(_PVAR(strId), _strid, 16);
			PushData(_PVAR(bReserved), &Reserved, sizeof(*bReserved));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);
			TakeData(_PVAR(bReserved), sizeof(*bReserved));

		}
	};

	// ////////////////////////
	// 방 생성
	// ////////////////////////	
	class CGS2CL_CreateRoom_Req : public CCommMsg				// 방 생성 요청 (GS->CL)
	{
	public:
		char* strId;				// 참가 취소 신청자 아이디			// 참가 신청자 아이디
		Camel::Dar::TMS::EnterRoomType::Type* eEnterRoomType;		// 토너먼트시작 / 리매칭 / 예선도중참가
		void Set(const char *Id, const Camel::Dar::TMS::EnterRoomType::Type eType)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(GS2CL_CREATEROOM_REQ);
			PushData(_PVAR(strId), _strid, 16);
			PushData(_PVAR(eEnterRoomType), &eType, sizeof(*eEnterRoomType));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);
			TakeData(_PVAR(eEnterRoomType), sizeof(*eEnterRoomType));
		}
	};

	// ////////////////////////
	// 방 입장
	// ////////////////////////
	class CGS2CL_JoinRoom_Req : public CCommMsg					// 방 입장 요청 (GS->CL)
	{
	public:
		char* strId;				// 방 입장할 Client ID
		int* nServerCode;			// 서버코드
		int* nRoomNum;				// 방 번호
		__int64* nRoomKey;				// IPS 방 번호
		Camel::Dar::TMS::EnterRoomType::Type* eEnterRoomType;		// 토너먼트시작 / 리매칭 / 예선도중참가
		void Set(const char *Id, const int NServerCode, const int NRoomNum, const __int64 NRoomKey, const Camel::Dar::TMS::EnterRoomType::Type eType)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(GS2CL_JOINROOM_REQ);
			PushData(_PVAR(strId), _strid, 16);
			PushData(_PVAR(nServerCode), &NServerCode, sizeof(*nServerCode));
			PushData(_PVAR(nRoomNum), &NRoomNum, sizeof(*nRoomNum));
			PushData(_PVAR(nRoomKey), &NRoomKey, sizeof(*nRoomKey));
			PushData(_PVAR(eEnterRoomType), &eType, sizeof(*eEnterRoomType));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);
			TakeData(_PVAR(nServerCode), sizeof(*nServerCode));
			TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
			TakeData(_PVAR(nRoomKey), sizeof(*nRoomKey));
			TakeData(_PVAR(eEnterRoomType), sizeof(*eEnterRoomType));
		}
	};

	// ////////////////////////
	// 방 퇴장
	// ////////////////////////
	class CGS2CL_LeaveRoom_Req : public CCommMsg					// 방 퇴장 요청 (GS->CL)
	{
	public:
		char* strId;				// 방 퇴장할 Client ID
		Camel::Dar::TMS::LeaveReason::Type eLeaveReason;			// 퇴장 이유
		void Set(const char *Id)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(GS2CL_LEAVEROOM_REQ);
			PushData(_PVAR(strId), _strid, 16);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);

		}
	};

	// ////////////////////////
	// 시드업 요청
	// ////////////////////////
	class CGS2CL_SeedUp_Noti : public CCommMsg					// 시드업 공지 (GS->CL)
	{
	public:
		INT64* llSeedMoney;				// 변경될 시드 머니
		void Set(const INT64 LLSeedMoney)
		{
			SetHeader(GS2CL_SEEDUP_NOTI);
			PushData(_PVAR(llSeedMoney), &LLSeedMoney, sizeof(*llSeedMoney));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(llSeedMoney), sizeof(*llSeedMoney));

		}
	};

	// ////////////////////////
	// 게임 결과 전달
	// ////////////////////////
	class CGS2CL_GameOver_Noti : public CCommMsg				// 게임 결과 공지 (GS->CL)
	{
	public:
		int* TotUser;
		_TMS_UserInfo *arrayUI;
		void Set(std::list<Camel::Dar::TMS::UserInfo*>& rArrayUI)
		{
			int totuser = rArrayUI.size();
			SetHeader(GS2CL_GAMEOVER_NOTI);
			PushData( _PVAR( TotUser ), &totuser, sizeof( *TotUser ));

			std::list<Camel::Dar::TMS::UserInfo*>::iterator it_UI = rArrayUI.begin();
			std::list<Camel::Dar::TMS::UserInfo*>::iterator it_UI_End = rArrayUI.end();
			for (; it_UI != it_UI_End; ++it_UI )
			{
				_TMS_UserInfo tempUI;
				Camel::Dar::TMS::UserInfo* tempUI2 = *it_UI;
				if(tempUI2->strId.c_str() != NULL) strncpy(tempUI.strId, tempUI2->strId.c_str(), 15);
				if(tempUI2->strNickname.c_str() != NULL) strncpy(tempUI.strNickname, tempUI2->strNickname.c_str(), 19);
				tempUI.ePos = tempUI2->ePos;
				tempUI.llChip = tempUI2->llChips;
				tempUI.nGameCount = tempUI2->nGameCount;
				tempUI.nRank = tempUI2->nRank;
				PushData(_PVAR(arrayUI), &tempUI, sizeof(*arrayUI));
			}
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(TotUser), sizeof(*TotUser));
			TakeData(_PVAR(arrayUI), sizeof(*arrayUI) * ( *TotUser));
		}
	};

	// ////////////////////////
	// 랭킹 요청
	// ////////////////////////
	// 토너먼트 유저 정보
	class CCL2GS_UserRanking_Req : public CCommMsg				// 유저 랭킹 요청 (CL->GS)
	{
	public:
		unsigned int* nStartRank;	// 시작 랭킹 : 시작 인덱스 0
		unsigned int* nRankCount;	// 갯수: 전체: 0
		void Set(const unsigned int NStartRank, const unsigned int NRankCount)
		{
			SetHeader(CL2GS_USERRANKING_REQ);
			PushData(_PVAR(nStartRank), &NStartRank, sizeof(*nStartRank));
			PushData(_PVAR(nRankCount), &NRankCount, sizeof(*nRankCount));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nStartRank), sizeof(*nStartRank));
			TakeData(_PVAR(nRankCount), sizeof(*nRankCount));

		}
	};

	class CCL2GS_UserRanking_Ans : public CCommMsg				// 유저 랭킹 (GS->CL)
	{
	public:
		int* nEntryCount;			// 토너먼트 참가자 수
		unsigned int* nStartRank;
		int* TotUser;
		_TMS_UserInfo *arrayUI;
		void Set(const int NEntryCount, const unsigned int NStartRank, std::vector<Camel::Dar::TMS::UserInfo*>& rArrayUI)
		{
			int totuser = rArrayUI.size();
			SetHeader(CL2GS_USERRANKING_ANS);
			PushData(_PVAR(nEntryCount), &NEntryCount, sizeof(*nEntryCount));
			PushData(_PVAR(nStartRank), &NStartRank, sizeof(*nStartRank));
			PushData( _PVAR( TotUser ), &totuser, sizeof( *TotUser ));
			size_t size = rArrayUI.size();
			for ( size_t i = 0; i < size; ++i )
			{
				_TMS_UserInfo tempUI;
				if(!rArrayUI[i]->strId.empty()) strncpy(tempUI.strId, rArrayUI[i]->strId.c_str(), 15);
				if(!rArrayUI[i]->strNickname.empty()) strncpy(tempUI.strNickname, rArrayUI[i]->strNickname.c_str(), 19);
				tempUI.ePos = (int)rArrayUI[i]->ePos;
				tempUI.llChip = rArrayUI[i]->llChips;
				tempUI.nGameCount = rArrayUI[i]->nGameCount;
				tempUI.nRank = rArrayUI[i]->nRank;
				PushData(_PVAR(arrayUI), &tempUI, sizeof(*arrayUI));
			}
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nEntryCount), sizeof(*nEntryCount));
			TakeData(_PVAR(nStartRank), sizeof(*nStartRank));
			TakeData(_PVAR(TotUser), sizeof(*TotUser));
			TakeData(_PVAR(arrayUI), sizeof(*arrayUI) * (*TotUser));

		}
	};

	// ////////////////////////
	// 토너먼트 정보 요청
	// ////////////////////////
	class CCL2GS_TournamentInfo_Req : public CCommMsg			// 토너먼트 정보 요청 (CL->GS)
	{
	public:
		Camel::Dar::TMS::State::Type* eReqState;			// 요청을 원하는 State
		void Set(const int EReqState)
		{
			SetHeader(CL2GS_TOURNAMENTINFO_REQ);
			PushData(_PVAR(eReqState), &EReqState, sizeof(*eReqState));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(eReqState), sizeof(*eReqState));

		}
	};

	class CCL2GS_TournamentInfo_Ans : public CCommMsg			// 토너먼트 정보 요청 결과 (GS->CL)
	{
	public:
		_TMS_TournamentInfo *stTournamentInfo;

		void Set(const Camel::Dar::TMS::TournamentInfo* CLTournamentInfo)
		{
			SetHeader(CL2GS_TOURNAMENTINFO_ANS);
			_TMS_TournamentInfo tempTournamentInfo;
			tempTournamentInfo.eState = CLTournamentInfo->eState;
			tempTournamentInfo.nAliveCount = CLTournamentInfo->nAliveCount;
			tempTournamentInfo.nEntryCount = CLTournamentInfo->nEntryCount;
			tempTournamentInfo.nInning = CLTournamentInfo->nInning;
			tempTournamentInfo.tPlayTime = CLTournamentInfo->tPlayTime;
			tempTournamentInfo.tStartTime = CLTournamentInfo->tStartTime;
			PushData(_PVAR(stTournamentInfo), &tempTournamentInfo, sizeof(*stTournamentInfo));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(stTournamentInfo), sizeof(*stTournamentInfo));
		}
	};

	class CCL2GS_TournamentResult_Req : public CCommMsg
	{
	public:
		char* strId;				// 토너먼트 정보 요청자 아이디 Client ID
		int* nIning;
		void Set(const char *Id, const int NIning)
		{
			char _strid[16]={0,};
			if(Id != NULL) strncpy(_strid, Id, 15);
			SetHeader(CL2GS_TOURNAMENTRESULT_REQ);
			PushData(_PVAR(strId), _strid, 16);
			PushData(_PVAR(nIning), &NIning, sizeof(*nIning));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(strId), 16);
			TakeData(_PVAR(nIning), sizeof(*nIning));
		}
	};

	class CCL2GS_TournamentResult_Ans : public CCommMsg
	{
	public:
		int* nIning;				// 회차 정보
		__int64* tStartTime;			// 시간 정보
		int* TotUser;				// 입상자 수
		ResultInfo *resultInfo;		// 입상자
		void Set(const int NIning, __int64 llTStartTime, vector<Camel::Dar::TMS::ResultInfo*>& vecRInfo)
		{
			int totuser = vecRInfo.size();
			SetHeader(CL2GS_TOURNAMENTRESULT_ANS);
			PushData(_PVAR(nIning), &NIning, sizeof(*nIning));
			PushData(_PVAR(tStartTime), &llTStartTime, sizeof(*tStartTime));
			PushData(_PVAR(TotUser), &totuser, sizeof(*TotUser));
			for(size_t i=0; i < totuser && i < 10; ++i)
			{
				ResultInfo rInfo;
				if(vecRInfo[i]->strNickname.c_str() != NULL) strncpy(rInfo.Nickname, vecRInfo[i]->strNickname.c_str(), 15);
				rInfo.PrizeMoney = vecRInfo[i]->llPrizeMoney;
				PushData(_PVAR(resultInfo), &rInfo, sizeof(ResultInfo));
			}
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nIning), sizeof(*nIning));
			TakeData(_PVAR(tStartTime), sizeof(*tStartTime));
			TakeData(_PVAR(TotUser), sizeof(*TotUser));
			TakeData(_PVAR(resultInfo), sizeof(ResultInfo) * (*TotUser));
		}
	};



	/////////// 대기실용 채팅 데이타
	class CCL2GS_ChatMsg_Req : public CCommMsg
	{
	public:

		char *ID;			// 아이디 스트링
		char* m_szNickName;	// 닉네임
		short *l_StrChat;	// 채팅 스트링의 길이
		char *StrChat;		// 채팅 스트링

		void Set(const char* id, const char* szNickName, const char *chat) 
		{
			char strId[16]={0,};
			char strNickName[20]={0,};
			CString szChat;
			szChat.Format("%s", chat);
			short l_strchat = (short) szChat.GetLength() + 1;

			SetHeader(CL2GS_CHATMSG_REQ);

			if(strlen(id) < 16) strcpy(strId, id);
			if (strlen(szNickName) < 20) strcpy(strNickName, szNickName);

			PushData(_PVAR(ID), strId, 16);
			PushData(_PVAR(m_szNickName), strNickName, 20);
			PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
			PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(ID), 16);
			TakeData(_PVAR(m_szNickName), 20);
			TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
			TakeData(_PVAR(StrChat), *l_StrChat);
		}
	};

	class CCL2GS_ChatMsg_Noti : public CCommMsg
	{
	public:

		char *ID;			// 아이디 스트링
		char* m_szNickName;	// 닉네임
		short *l_StrChat;	// 채팅 스트링의 길이
		char *StrChat;		// 채팅 스트링

		void Set(const char* id, const char* szNickName, const char *chat) 
		{
			char strId[16]={0,};
			char strNickName[20]={0,};
			CString szChat;
			szChat.Format("%s", chat);
			short l_strchat = (short) szChat.GetLength() + 1;

			SetHeader(CL2GS_CHATMSG_NOTI);

			if(strlen(id) < 16) strcpy(strId, id);
			if (strlen(szNickName) < 20) strcpy(strNickName, szNickName);

			PushData(_PVAR(ID), strId, 16);
			PushData(_PVAR(m_szNickName), strNickName, 20);
			PushData(_PVAR(l_StrChat), &l_strchat, sizeof(l_strchat));
			PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(ID), 16);
			TakeData(_PVAR(m_szNickName), 20);
			TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
			TakeData(_PVAR(StrChat), *l_StrChat);
		}
	};

	class CGS2CL_SetMoneyInfo_Noti : public CCommMsg
	{
	public:
		__int64 *llBaseChips;
		void Set(const __int64 LlBaseChips)
		{
			SetHeader(GS2CL_SETMONEYINFO_NOTI);
			PushData(_PVAR(llBaseChips), &LlBaseChips, sizeof(*llBaseChips));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(llBaseChips), sizeof(*llBaseChips));
		}
	};

	class CGS2CL_ShowMovie_Noti : public CCommMsg
	{
	public:
		void Set()
		{
			SetHeader(GS2CL_SHOWMOVIE_NOTI);
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
		}
	};

	class CGS2CL_LeaveRoom_Noti : public CCommMsg
	{
	public:
		_TMS_UserInfo *stUserInfo;
		void Set(const Camel::Dar::TMS::UserInfo *CLUserInfo)
		{
			SetHeader(GS2CL_LEAVEROOM_NOTI);
			_TMS_UserInfo tempUserInfo;
			if(CLUserInfo->strId.c_str() != NULL) strncpy(tempUserInfo.strId, CLUserInfo->strId.c_str(), 15);
			if(CLUserInfo->strNickname.c_str() != NULL) strncpy(tempUserInfo.strNickname, CLUserInfo->strNickname.c_str(), 19);
			tempUserInfo.llChip = CLUserInfo->llChips;
			tempUserInfo.ePos = CLUserInfo->ePos;
			tempUserInfo.nGameCount = CLUserInfo->nGameCount;
			tempUserInfo.nRank = CLUserInfo->nRank;
			PushData(_PVAR(stUserInfo), &tempUserInfo, sizeof(*stUserInfo));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(stUserInfo), sizeof(*stUserInfo));
		}
	};

	class CGS2CL_OpenTournament_Noti : public CCommMsg
	{
	public:
		int *nInning;
		__int64 *tStartingTime;
		void Set(const int NInning, const __int64 TStartingTime)
		{
			SetHeader(GS2CL_OPENTOURNAMENT_NOTI);
			PushData(_PVAR(nInning), &NInning, sizeof(*nInning));
			PushData(_PVAR(tStartingTime), &TStartingTime, sizeof(*tStartingTime));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nInning), sizeof(*nInning));
			TakeData(_PVAR(tStartingTime), sizeof(*tStartingTime));
		}
	};

	// 부전승 머니 지급
	class CGS2CL_UnearnedWin_Noti : public CCommMsg
	{
	public:
		__int64 *llPrizeMoney;
		void Set(const __int64 LlPrizeMoney)
		{
			SetHeader(GS2CL_UNEARNEDWIN_NOTI);
			PushData(_PVAR(llPrizeMoney), &LlPrizeMoney, sizeof(*llPrizeMoney));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(llPrizeMoney), sizeof(*llPrizeMoney));
		}
	};

	class CCL2GS_UnearnedWin_Req : public CCommMsg
	{
	public:
		void Set()
		{
			SetHeader(CL2GS_UNEARNEDWIN_REQ);
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
		}
	};

	class CCL2GS_UnearnedWin_Ans : public CCommMsg
	{
	public:
		int *nResultCode;
		__int64 *llPrizeMoney;
		void Set(const int NResultCode, const __int64 LlPrizeMoney)
		{
			SetHeader(CL2GS_UNEARNEDWIN_ANS);
			PushData(_PVAR(nResultCode), &NResultCode, sizeof(*nResultCode));
			PushData(_PVAR(llPrizeMoney), &LlPrizeMoney, sizeof(*llPrizeMoney));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
			TakeData(_PVAR(llPrizeMoney), sizeof(*llPrizeMoney));
		}
	};

	// 사전 예약자 머니 지급
	class CGS2CL_ReservedUserPrize_Noti : public CCommMsg
	{
	public:
		__int64 *llPrizeMoney;
		void Set(const __int64 LlPrizeMoney)
		{
			SetHeader(GS2CL_RESERVEDUSERPRIZE_NOTI);
			PushData(_PVAR(llPrizeMoney), &LlPrizeMoney, sizeof(*llPrizeMoney));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(llPrizeMoney), sizeof(*llPrizeMoney));
		}
	};

	class CCL2GS_ReservedUserPrize_Req : public CCommMsg
	{
	public:
		void Set()
		{
			SetHeader(CL2GS_RESERVEDUSERPRIZE_REQ);
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
		}
	};

	class CCL2GS_ReservedUserPrize_Ans : public CCommMsg
	{
	public:
		int *nResultCode;
		__int64 *llPrizeMoney;
		void Set(const int NResultCode, const __int64 LlPrizeMoney)
		{
			SetHeader(CL2GS_RESERVEDUSERPRIZE_ANS);
			PushData(_PVAR(nResultCode), &NResultCode, sizeof(*nResultCode));
			PushData(_PVAR(llPrizeMoney), &LlPrizeMoney, sizeof(*llPrizeMoney));
		}
		void Get(char* lpdata, int size)
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nResultCode), sizeof(*nResultCode));
			TakeData(_PVAR(llPrizeMoney), sizeof(*llPrizeMoney));
		}
	};

	class CGS2CL_ChangeTournamentState_Noti : public CCommMsg		// 토너먼트 상태변경 공지 (GS->CL)
	{
	public:
		Camel::Dar::TMS::State::Type* ePreviousState;
		Camel::Dar::TMS::State::Type* eCurrentState;

		void Set(const int EPrevState, const int ECurrState)
		{
			SetHeader(GS2CL_CHANGETOURNAMENTSTATE_NOTI);
			PushData(_PVAR(ePreviousState), &EPrevState, sizeof(*ePreviousState));
			PushData(_PVAR(eCurrentState), &ECurrState, sizeof(*eCurrentState));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(ePreviousState), sizeof(*ePreviousState));
			TakeData(_PVAR(eCurrentState), sizeof(*eCurrentState));

		}
	};

	class CGS2CL_EliminateTournament_Noti : public CCommMsg		// 토너먼트 탈락 노티 (GS->CL)
	{
	public:
		int* nEntryCount;	// 참가자수
		int* nRank;			// 랭킹
		int* nRound;		// 예선: 0, 본선: 1, 결선: 2

		void Set(const int EntryCount, const int Rank, const int Round)
		{
			SetHeader(GS2CL_ELIMINATETOURNAMENT_NOTI);
			PushData(_PVAR(nEntryCount), &EntryCount, sizeof(*nEntryCount));
			PushData(_PVAR(nRank), &Rank, sizeof(*nRank));
			PushData(_PVAR(nRound), &Round, sizeof(*nRound));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nEntryCount), sizeof(*nEntryCount));
			TakeData(_PVAR(nRank), sizeof(*nRank));
			TakeData(_PVAR(nRound), sizeof(*nRound));
		}
	};

	class CGS2CL_MatchWaiting_Noti : public CCommMsg		// 토너먼트 매치 대기 노티 (GS->CL)
	{
	public:
		void Set()
		{
			SetHeader(GS2CL_MATCHWAITING_NOTI);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
		}
	};

	class CGS2CL_FinalLastRoomMatch_Noti : public CCommMsg		// 결선 방이 하나 남았음을 노티
	{
	public:
		void Set()
		{
			SetHeader(GS2CL_FINALLASTROOMMATCH_NOTI);
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
		}
	};

	class CGS2CL_TournamentProgress_Noti : public CCommMsg		// 토너먼트 진행상황 노티(예선/본선 종료 5분전)
	{
	public:
		int* nRemainMinute;	// 남은시간(분단위)
		int* nRound;		// 예선: 0, 본선: 1, 결선: 2

		void Set(const int RemainMinute, const int Round)
		{
			SetHeader(GS2CL_TOURNAMENTPROGRESS_NOTI);
			PushData(_PVAR(nRemainMinute), &RemainMinute, sizeof(*nRemainMinute));
			PushData(_PVAR(nRound), &Round, sizeof(*nRound));
		}
		void Get(char* lpdata, int size) 
		{
			if(!GetHeader(lpdata, size)) return;
			TakeData(_PVAR(nRemainMinute), sizeof(*nRemainMinute));
			TakeData(_PVAR(nRound), sizeof(*nRound));
		}
	};

}	// namespace TMS