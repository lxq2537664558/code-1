/******************************************************************************/
#include "stdafx.h"
#include "PrintDialog.h"
#include "../resource.h"

#include "MyListView.h"
#include "SWorldManager.h"

#include "SWorldManager.h"
#include "PrintWindowsCfg.h"
#include "SPlayer.h"
/******************************************************************************/

namespace MG
{
	static BOOL CALLBACK PlayerDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK PlayerCharacterDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	
	//-----------------------------------------------------------------------------
	PrintDialog::PrintDialog()
		:mPlayerDlg(NULL)
		,mPlayerCharacterDlg(NULL)
	{
	}

	//-----------------------------------------------------------------------------
	PrintDialog::~PrintDialog()
	{
	}


	//-----------------------------------------------------------------------------
	BOOL CALLBACK PlayerDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_INITDIALOG:
			{
				break;
			}
		case WM_KEYDOWN:
			{
				break;
			}
		case WM_COMMAND:
			{
				break;
			}

		case WM_CLOSE:
			{
				PostQuitMessage(0);
				break;
			}
		}
		return FALSE;	
	}
	//-----------------------------------------------------------------------------
	BOOL CALLBACK PlayerCharacterDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_INITDIALOG:
			{
				break;
			}
		case WM_KEYDOWN:
			{
				break;
			}
		case WM_COMMAND:
			{
				break;
			}

		case WM_CLOSE:
			{
				PostQuitMessage(0);
				break;
			}
		}
		return FALSE;	
	}

	//-----------------------------------------------------------------------------
	void PrintDialog::createMainDialog()
	{
#ifdef _DEBUG
		HINSTANCE hInst = GetModuleHandle("FrontServer_d.dll");
#else
		HINSTANCE hInst = GetModuleHandle("FrontServer.dll");
#endif


		// Player 的打印信息
		if(PrintWindowsCfg::getInstance().getPrintPlayer() == 1)
		{
			mPlayerDlg = ::CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_PLAYER), GetDesktopWindow(), PlayerDlgProc);
			ShowWindow(mPlayerDlg, SW_SHOW);
			std::string	strCaption = "FrontServer【Player】";
			strCaption += "[";
			strCaption += __DATE__;
			strCaption += " ";
			strCaption += __TIME__;
			strCaption += "]";
			SetWindowText(mPlayerDlg, strCaption.c_str());

			HMENU hMenu = GetSystemMenu(mPlayerDlg, FALSE);
			EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

			//设置事件
			mWindowProcPlayer = (WinProc)GetWindowLong((HWND)mPlayerDlg,GWL_WNDPROC);
			SetWindowLong((HWND)mPlayerDlg, GWL_WNDPROC, (LONG)(&windowPlayerMsg));
		}

		// PlayerCharacter 的打印信息
		if(PrintWindowsCfg::getInstance().getPrintPlayerCharacter() == 1)
		{
			mPlayerCharacterDlg = ::CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_PLAYERCHARACTER), GetDesktopWindow(), PlayerCharacterDlgProc);
			ShowWindow(mPlayerCharacterDlg, SW_SHOW);
			std::string	strCaption = "FrontServer【Character】";
			strCaption += "[";
			strCaption += __DATE__;
			strCaption += " ";
			strCaption += __TIME__;
			strCaption += "]";
			SetWindowText(mPlayerCharacterDlg, strCaption.c_str());

			HMENU hMenu = GetSystemMenu(mPlayerCharacterDlg, FALSE);
			EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

			//设置事件
			mWindowProcPlayerCharacter = (WinProc)GetWindowLong((HWND)mPlayerCharacterDlg,GWL_WNDPROC);
			SetWindowLong((HWND)mPlayerCharacterDlg, GWL_WNDPROC, (LONG)(&windowPlayerCharacterMsg));
		}

	}

	void PrintDialog::clearPlayerListView()
	{
		//static MyListView listView_Player;
		mListView_Player.GetListViewHandle(mPlayerDlg, IDC_LIST_PLAYER);
		mListView_Player.DeleteAllItem();
		mListView_Player.DeleteAllColum();


		mListView_Player.InsertColumn(0, 	"accountId",			70);
		mListView_Player.InsertColumn(1, 	"mClientNetId",			70);
		mListView_Player.InsertColumn(2, 	"mCampaignId",			90);
		mListView_Player.InsertColumn(3, 	"mMainClan",			110);
		mListView_Player.InsertColumn(4, 	"mainGeneralId",		100);
		mListView_Player.InsertColumn(5, 	"mMapServerNetId",		85);
		mListView_Player.InsertColumn(6, 	"RegionType",			80);
		mListView_Player.InsertColumn(7, 	"RegionId",				80);
		mListView_Player.InsertColumn(8, 	"InstanceUid",			80);
	}

	void PrintDialog::clearGeneralListView()
	{
		mListView_PlayerCharacter.GetListViewHandle(mPlayerCharacterDlg, IDC_LIST_PLAYERCHARACTER);
		mListView_PlayerCharacter.DeleteAllItem();
		mListView_PlayerCharacter.DeleteAllColum();


		mListView_PlayerCharacter.InsertColumn(0, 	"generalId",			70);
		mListView_PlayerCharacter.InsertColumn(1, 	"mParentClanId",		85);
		
		mListView_PlayerCharacter.InsertColumn(2, 	"mMapServerNetId",		100);
		mListView_PlayerCharacter.InsertColumn(3, 	"RegionType",			150);
		mListView_PlayerCharacter.InsertColumn(4, 	"RegionId",				150);
		mListView_PlayerCharacter.InsertColumn(5, 	"InstanceUId",			150);
	}

	//-----------------------------------------------------------------------------
	void PrintDialog::printPlayerList()
	{
		clearPlayerListView();
		clearGeneralListView();

		//Int index = 0;
		//std::map<NetIdType, SPlayerPtr>* playerList = SWorldManager::getInstance().getGameObjectManager()->getPlayerList();
		//for (std::map<NetIdType, SPlayerPtr>::iterator iter = playerList->begin(); 
		//	iter != playerList->end(); ++iter)
		//{
		//	printSiglePlayerInfo(index, iter->second);
		//	++index;
		//}
	}

	//-----------------------------------------------------------------------------
	void PrintDialog::printSiglePlayerInfo(Int index, SPlayerPtr player)
	{
#if 0
		mListView_Player.InsertItem(index);


		
		mListView_Player.SetItemTextU64(index, 0, player->getAccountID());
		mListView_Player.SetItemTextU64(index, 1, player->getClientNetID());
		mListView_Player.SetItemTextU64(index, 2, player->getCampaignId());
		mListView_Player.SetItemTextU64(index, 3, player->getMainClan()->getMainGenrealID());
		mListView_Player.SetItemTextU64(index, 4, player->getMainClan()->getMainPlayerCharacter()->getID());

		//MapServerNetID
		{
			char tempStr[32] = "";
			Sprintf_s(tempStr, 32, "[%d]/[%d]", player->getMapServerNetID(), player->getMapServerNetID());

			mListView_Player.SetItemTextStr(index, 5, tempStr);
		}
		mListView_Player.SetItemTextU64(index, 6, player->getCurrentRegionType());
		mListView_Player.SetItemTextU64(index, 7, player->getCurrentRegionId());
		mListView_Player.SetItemTextU64(index, 8, player->getCurrentInstanceUid());


		Int generalIndex = 0;
		std::map<PlayerCharacterIdType, SPlayerCharacter*>* playerCharacterList = player->getMainClan()->getPlayerCharacterList();
		std::map<PlayerCharacterIdType, SPlayerCharacter*>::iterator iter = playerCharacterList->begin();
		for ( ; iter != playerCharacterList->end(); ++iter)
		{
			printSigleGeneralInfo(generalIndex, iter->second);
			++generalIndex;
		}
#endif

	}

	void PrintDialog::printSigleGeneralInfo(Int index, SPlayerCharacter* general)
	{
#if 0
		mListView_PlayerCharacter.InsertItem(index);

		mListView_PlayerCharacter.SetItemTextU64(index, 0, general->getID());
		mListView_PlayerCharacter.SetItemTextU64(index, 1, general->getParentClan()->getClanID());

		//MapServerNetID
		{
			char tempStr[50] = "";
			Sprintf_s(tempStr, 50, "cur[%d]/last[%d]", general->getMapServerNetId(), general->getLastMapServerNetId());

			mListView_PlayerCharacter.SetItemTextStr(index, 2, tempStr);
		}

		//RegionType
		{
			char tempStr[50] = "";
			Sprintf_s(tempStr, 50, "cur[%d]/last[%d]", general->getRegionType(), general->getLastRegionType());

			mListView_PlayerCharacter.SetItemTextStr(index, 3, tempStr);
		}

		//RegionId
		{
			char tempStr[50] = "";
			Sprintf_s(tempStr, 50, "cur[%d]/last[%d]", general->getRegionId(), general->getLastRegionId());

			mListView_PlayerCharacter.SetItemTextStr(index, 4, tempStr);
		}

		//InstanceUId
		{
			char tempStr[50] = "";
			Sprintf_s(tempStr, 50, "cur[%d]/last[%d]", general->getInstanceUId(), general->getLastInstanceUId());

			mListView_PlayerCharacter.SetItemTextStr(index, 5, tempStr);
		}
#endif
	}

	LRESULT CALLBACK PrintDialog::windowPlayerMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
#if 0
		switch(message)
		{
		case WM_COMMAND:
			{
				int kk = 0;
				switch(wParam)
				{
				case IDOK:
					{
						/////////////////////////////////////////////////////////////////////////////////////////////////////
						// 遍历playerList
						SPlayerPtr player;
						I32 currPlayerIndex = 0;
						NetIdType clientId = -1;
						Bool isContinue = true;
						while(isContinue)
						{
							isContinue = SWorldManager::getInstance().getGameObjectManager()->traversalPlayerList(currPlayerIndex,clientId,player);
							if (!isContinue)
							{
								break;
							}
							SClan* clan = player->getMainClan();
							DYNAMIC_ASSERT(clan);
							SPlayerCharacter* playerCharacter = clan->getMainPlayerCharacter();
							DYNAMIC_ASSERT(clan);
							playerCharacter->getSAttributeSystem()->print();
						}

						if(PrintWindowsCfg::getInstance().getPrintPlayer() == 1)
						{
							PrintDialog::getInstance().printPlayerList();
						}
						/////////////////////////////////////////////////////////////////////////////////////////////////////

						/*SWorldManager* manager = &(SWorldManager::getInstance());
						std::map<NetIdType, SPlayerPtr>* playerList = manager->getGameObjectManager()->getPlayerList();
						for (std::map<NetIdType, SPlayerPtr>::iterator iter = playerList->begin(); iter != playerList->end(); ++iter)
						{
							SClan* clan = iter->second->getMainClan();
							SPlayerCharacter* playerCharacter = clan->getMainPlayerCharacter();
							playerCharacter->getSAttributeSystem()->print();
						}
						if(PrintWindowsCfg::getInstance().getPrintPlayer() == 1)
						{
							PrintDialog::getInstance().printPlayerList();
						}*/
					}
					break;
				}
			}
			break;
		default:
			{

			}
			break;;
		}
#endif
		return (CallWindowProc((WinProc)mWindowProcPlayer, hwnd, message, wParam, lParam));
	}

	LRESULT CALLBACK PrintDialog::windowPlayerCharacterMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
#if 0
		switch(message)
		{
		case WM_COMMAND:
			{
				int kk = 0;
				switch(wParam)
				{
				case IDOK:
					{
						if(PrintWindowsCfg::getInstance().getPrintPlayerCharacter() == 1)
						{
							SPlayerPtr player;
							I32 currPlayerIndex = 0;
							NetIdType clientId = -1;
							Bool isContinue = true;
							while(isContinue)
							{
								isContinue = SWorldManager::getInstance().getGameObjectManager()->traversalPlayerList(currPlayerIndex,clientId,player);
								if (!isContinue)
								{
									break;
								}
								SClan* clan = player->getMainClan();
								DYNAMIC_ASSERT(clan);
								SPlayerCharacter* playerCharacter = clan->getMainPlayerCharacter();
								DYNAMIC_ASSERT(clan);
								playerCharacter->getSAttributeSystem()->print();
							}

							if(PrintWindowsCfg::getInstance().getPrintPlayer() == 1)
							{
								PrintDialog::getInstance().printPlayerList();
							}
						}
					}
					break;
				}
			}
			break;
		default:
			{

			}
			break;
		}
#endif
		return (CallWindowProc((WinProc)mWindowProcPlayerCharacter, hwnd, message, wParam, lParam));
	}

	//-----------------------------------------------------------------------------

}