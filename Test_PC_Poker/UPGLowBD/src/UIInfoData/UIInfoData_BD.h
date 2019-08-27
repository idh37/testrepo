#pragma once

namespace ui
{

	enum E_POSITION_BD
	{
		EPOS_UNKNOWN = -1,
		//Point
		EPOS_DEALING_TITEL,// 상단 카드 딜링
		EPOS_CARD_POS_0,   // 카드 좌표
		EPOS_CARD_POS_1,
		EPOS_CARD_POS_2,
		EPOS_CARD_POS_3,
		EPOS_CARD_POS_4,
		EPOS_CHANGE,      // 바꾸기 버튼 좌표
		EPOS_PASS,		  // 패스 버튼 좌표
		EPOS_CUTINFO,     // 아침, 점심, 저녁
		EPOS_CUTSELECT,   // 바꿀 카드를 선택하세요
		EPOS_RESULT_0,    // 승,패이펙트 족보 좌표
		EPOS_RESULT_1,
		EPOS_RESULT_2,
		EPOS_RESULT_3,
		EPOS_RESULT_4,
		EPOS_CUTNUM_0,		 // 커팅숫자 
		EPOS_CUTNUM_1,
		EPOS_CUTNUM_2,
		EPOS_CUTNUM_3,
		EPOS_CUTNUM_4,
		EPOS_CUTNUM_SCENE_0, // 커팅숫자  Scene Ani P, 1, 2, 3, 4
		EPOS_CUTNUM_SCENE_1,
		EPOS_CUTNUM_SCENE_2,
		EPOS_CUTNUM_SCENE_3,
		EPOS_CUTNUM_SCENE_4,
		EPOS_HIDDEN_OPENKEY,  // 히든 오픈키
		EPOS_START_BTN,		  // 시작 버튼
		EPOS_AUTOUP_BTN,     // 자동 올림
		EPOS_AUTOSORT_BTN,     // 자동 정렬:2012.09.27 YS.Jeong 
		EPOS_DEALER_POS_0,
		EPOS_DEALER_POS_1,
		EPOS_DEALER_POS_2,
		EPOS_DEALER_POS_3,
		EPOS_DEALER_POS_4, 


		//Gab
		EGAB_CARD_POS_X,		// 카드덱 카드 간격 
		EGAB_CARD_POS_OTHER_X,  // 카드덱 카드 간격 다른유저
		EGAB_CUTINFO_X,			// 아침, 점심, 저녁 
		EGAB_CUTNUM_Y,			// 컷넘버 
		EGAB_RESULT_X,			// 게임결과 족보영역의 상대좌표 머니
		EGAB_RESULT_Y,			// 게임결과 족보영역의 상대좌표 머니

		//Rect
		ERECT_WININFO,    // 승,패 이펙트 족보 출력영역

		//Float
		EFLOAT_F4_SPEED,			// 처음 4장 카드 이동속도
		EFLOAT_F4_TOTAL_CARD_DELAY, // 처음 4장 카드 이동전 대기시간
		EFLOAT_F4_CARD_DELAY,		// 처음 4장 카드 이동대기( 카드와 카드 )
		EFLOAT_F4_USER_DELAY,		// 처음 4장 카드 이동대기( 유저와 유저 )
		EFLOAT_CUT_SPEED,			// 컷팅 카드 이동속도
		EFLOAT_CUT_TOTAL_DELAY,		// 컷팅 카드 이동 전 대기시간
		EFLOAT_CUT_CARD_DELAY,		// 컷팅 카드 이동대기( 카드와 카드 )
		EFLOAT_CUT_PASS_DELAY,		// 커팅 패스 이동 전 대기시간

		//RGB
		ERGB_BETBTNTEXT,				// 버튼의 베팅 금액 표시 글자색
		ERGB_BETBTNTEXT_SHADOW,			// 버튼의 베팅 금액 표시 글자의 그림자색

		EPOS_ALL
	};

	class CUIInfoData_BD :	public CUIInfoData
	{
	public:
		CUIInfoData_BD(void);
		~CUIInfoData_BD(void);

	public:
		void Initialize();

		CPoint GetPosBD( E_POSITION_BD eUiTypeBD )
		{
			if ( !IsExistPos( eUiTypeBD ))
			{
				TPASSERT( 0 );
				return CPoint( 0, 0 );
			}

			return m_mapPos[ eUiTypeBD ];
		}

		CRect GetRectBD( E_POSITION_BD eUiTypeBD )
		{
			if ( !IsExistRect( eUiTypeBD ))
			{
				TPASSERT( 0 );
				return CRect( 0, 0, 10, 10  );
			}

			return m_mapRect[ eUiTypeBD ];
		}

		int GetGabBD( E_POSITION_BD eUiTypeBD )
		{
			if ( !IsExistGab( eUiTypeBD ))
			{
				TPASSERT( 0 );
				return 0;
			}

			return m_mapGab[ eUiTypeBD ];
		}

		float GetFloatBD( E_POSITION_BD eUiTypeBD )
		{
			if ( !IsExistFloat( eUiTypeBD ))
			{
				TPASSERT( 0 );
				return 0.f;
			}

			return m_mapFloat[ eUiTypeBD ];
		}

		COLORREF GetRGBBD( E_POSITION_BD eUiTypeBD )
		{
			if ( !IsExistRGB( eUiTypeBD ))
			{
				TPASSERT( 0 );
				return 0;
			}

			return m_mapRGB[ eUiTypeBD ];
		}

		//
		bool IsExistPos( E_POSITION_BD eUiTypeBD )
		{
			if ( m_mapPos.find( eUiTypeBD ) == m_mapPos.end() )
			{
				return 	false;
			}

			return true;
		}

		bool IsExistRect( E_POSITION_BD eUiTypeBD )
		{
			if ( m_mapRect.find( eUiTypeBD ) == m_mapRect.end() )
			{
				return 	false;
			}

			return true;
		}

		bool IsExistGab( E_POSITION_BD eUiTypeBD )
		{
			if ( m_mapGab.find( eUiTypeBD ) == m_mapGab.end() )
			{
				return 	false;
			}

			return true;
		}

		bool IsExistFloat( E_POSITION_BD eUiTypeBD )
		{
			if ( m_mapFloat.find( eUiTypeBD ) == m_mapFloat.end() )
			{
				return 	false;
			}

			return true;
		}

		bool IsExistRGB( E_POSITION_BD eUiTypeBD )
		{
			if ( m_mapRGB.find( eUiTypeBD ) == m_mapRGB.end() )
			{
				return 	false;
			}

			return true;
		}

		virtual void Clear();

	protected:
		std::map< E_POSITION_BD, CPoint >	m_mapPos;	// 좌표
		std::map< E_POSITION_BD, CRect >	m_mapRect;	// 크기
		std::map< E_POSITION_BD, int >		m_mapGab;	// 상대좌표
		std::map< E_POSITION_BD, float >	m_mapFloat; // 이동속도, 대기시간
		std::map< E_POSITION_BD, COLORREF > m_mapRGB;   // 폰트컬러
	};
}
