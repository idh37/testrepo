#pragma once
#include "nmwidgetmanager.h"
#include "NMButton.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMTab : public CNMWidgetComposite
		{
			struct TABINFO		
			{
				int nViewWidgetID;
				INMWidget *pViewWidget;		
				CNMButton *pTabButton;
			};
		protected:
			int m_nControlID;
			int m_nStateWidgetID;
			vector<TABINFO> m_vec_TabInfo;	

			CNMWidgetState *m_pWidgetState;

		public:
			CNMTab(void);
			~CNMTab(void);


			virtual BOOL Initialize(int nControlID, int nStateWidgetID);
			virtual BOOL ChangeTab(int nViewWidgetID);
			virtual BOOL AddTab(int nViewWidget, INMWidget* pWidget, CNMButton *pButton);
			virtual int GetCurTab();
			virtual void SetRelationPosition( int nOffsetX, int nOffsetY );


			virtual BOOL CreateWidget( CWnd* pParent );		
			virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );

			virtual INMWidget* GetWidget( int nWidgetKey );

		};

	} //namespace SKINGDI

}//namespace NMBASE