//#include "StdAfx2.h"

/*
#include <Windows.h>

#include "PCommonDialog.h"
#include "PBodySetup.h"
#include "PBXMLSetup.h"
*/
/*



void CPBParentDialog::OnTabChange(int last,int current)
{
	
		//mTestViControl.SetActiveTab(current);
		return;
		CPSharedBase* lastDlg = (CPSharedBase*)getDialog(last);
		if (lastDlg)
		{

			switch(last)
			{
				case BTAB_XML:
					{
						CPBXMLSetup*xmlDlg=(CPBXMLSetup*)CPSharedBase::getInstance()->getDialog(BTAB_XML);
						if (xmlDlg)
						{
							//xmlDlg->ShowWindow(SW_HIDE);
							//xmlDlg->UpdateWindow();
						}
					}break;

				case BTAB_COMMON:
					{
						CPBodyCfg*cfg=(CPBodyCfg*)CPSharedBase::getInstance()->getDialog(BTAB_COMMON);
						if (cfg)
						{
							//cfg->ShowWindow(SW_HIDE);
							//cfg->UpdateWindow();
						}
					}break;

			}
		}

		CPSharedBase* selectedDlg = (CPSharedBase*)getDialog(current);
		if (selectedDlg)
		{

			switch(current)
			{
				case BTAB_XML:
				{
					CPBXMLSetup*xmlDlg=(CPBXMLSetup*)CPSharedBase::getInstance()->getDialog(BTAB_XML);
					if (xmlDlg)
					{
						//xmlDlg->ShowWindow(SW_SHOW);
						//xmlDlg->UpdateWindow();
						//xmlDlg->OnSelect(last);

					}
				}break;

				case BTAB_COMMON:
				{
					CPBodyCfg*cfg=(CPBodyCfg*)CPSharedBase::getInstance()->getDialog(BTAB_COMMON);
					if (cfg)
					{
//						cfg->ShowWindow(SW_NORMAL);
						//cfg->UpdateWindow();
						cfg->OnSelect(last);
					}
				}break;
			}

			
		}
}
*/