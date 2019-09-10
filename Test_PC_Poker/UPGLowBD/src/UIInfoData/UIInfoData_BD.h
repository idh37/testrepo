#pragma once

namespace ui
{

	enum E_POSITION_BD
	{
		EPOS_UNKNOWN = -1,
		//Point
		EPOS_DEALING_TITEL,// ��� ī�� ����
		EPOS_CARD_POS_0,   // ī�� ��ǥ
		EPOS_CARD_POS_1,
		EPOS_CARD_POS_2,
		EPOS_CARD_POS_3,
		EPOS_CARD_POS_4,
		EPOS_CHANGE,      // �ٲٱ� ��ư ��ǥ
		EPOS_PASS,		  // �н� ��ư ��ǥ
		EPOS_CUTINFO,     // ��ħ, ����, ����
		EPOS_CUTSELECT,   // �ٲ� ī�带 �����ϼ���
		EPOS_RESULT_0,    // ��,������Ʈ ���� ��ǥ
		EPOS_RESULT_1,
		EPOS_RESULT_2,
		EPOS_RESULT_3,
		EPOS_RESULT_4,
		EPOS_CUTNUM_0,		 // Ŀ�ü��� 
		EPOS_CUTNUM_1,
		EPOS_CUTNUM_2,
		EPOS_CUTNUM_3,
		EPOS_CUTNUM_4,
		EPOS_CUTNUM_SCENE_0, // Ŀ�ü���  Scene Ani P, 1, 2, 3, 4
		EPOS_CUTNUM_SCENE_1,
		EPOS_CUTNUM_SCENE_2,
		EPOS_CUTNUM_SCENE_3,
		EPOS_CUTNUM_SCENE_4,
		EPOS_HIDDEN_OPENKEY,  // ���� ����Ű
		EPOS_START_BTN,		  // ���� ��ư
		EPOS_AUTOUP_BTN,     // �ڵ� �ø�
		EPOS_AUTOSORT_BTN,     // �ڵ� ����:2012.09.27 YS.Jeong 
		EPOS_DEALER_POS_0,
		EPOS_DEALER_POS_1,
		EPOS_DEALER_POS_2,
		EPOS_DEALER_POS_3,
		EPOS_DEALER_POS_4, 


		//Gab
		EGAB_CARD_POS_X,		// ī�嵦 ī�� ���� 
		EGAB_CARD_POS_OTHER_X,  // ī�嵦 ī�� ���� �ٸ�����
		EGAB_CUTINFO_X,			// ��ħ, ����, ���� 
		EGAB_CUTNUM_Y,			// �Ƴѹ� 
		EGAB_RESULT_X,			// ���Ӱ�� ���������� �����ǥ �Ӵ�
		EGAB_RESULT_Y,			// ���Ӱ�� ���������� �����ǥ �Ӵ�

		//Rect
		ERECT_WININFO,    // ��,�� ����Ʈ ���� ��¿���

		//Float
		EFLOAT_F4_SPEED,			// ó�� 4�� ī�� �̵��ӵ�
		EFLOAT_F4_TOTAL_CARD_DELAY, // ó�� 4�� ī�� �̵��� ���ð�
		EFLOAT_F4_CARD_DELAY,		// ó�� 4�� ī�� �̵����( ī��� ī�� )
		EFLOAT_F4_USER_DELAY,		// ó�� 4�� ī�� �̵����( ������ ���� )
		EFLOAT_CUT_SPEED,			// ���� ī�� �̵��ӵ�
		EFLOAT_CUT_TOTAL_DELAY,		// ���� ī�� �̵� �� ���ð�
		EFLOAT_CUT_CARD_DELAY,		// ���� ī�� �̵����( ī��� ī�� )
		EFLOAT_CUT_PASS_DELAY,		// Ŀ�� �н� �̵� �� ���ð�

		//RGB
		ERGB_BETBTNTEXT,				// ��ư�� ���� �ݾ� ǥ�� ���ڻ�
		ERGB_BETBTNTEXT_SHADOW,			// ��ư�� ���� �ݾ� ǥ�� ������ �׸��ڻ�

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
		std::map< E_POSITION_BD, CPoint >	m_mapPos;	// ��ǥ
		std::map< E_POSITION_BD, CRect >	m_mapRect;	// ũ��
		std::map< E_POSITION_BD, int >		m_mapGab;	// �����ǥ
		std::map< E_POSITION_BD, float >	m_mapFloat; // �̵��ӵ�, ���ð�
		std::map< E_POSITION_BD, COLORREF > m_mapRGB;   // ��Ʈ�÷�
	};
}
