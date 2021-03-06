
/******************************************************************************/
#include "stdafx.h"
#include "RegionNetPacketProcesser.h"
#include "ServerManager.h"
//#include "SWorldManager.h"
#include "FrontServerMain.h"
#include "SWorldManager.h"
#include "ItemPacketProcesser.h"
/******************************************************************************/

namespace MG
{
	//--------------------------------------------------------------------------
	RegionNetPacketProcesser::RegionNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	RegionNetPacketProcesser::~RegionNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	Bool RegionNetPacketProcesser::processClientPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_REGION )
		{   
			//FUNDETECTION(__MG_FUNC__);
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
			case PT_REGION_C2F_ADD_BUILDING:
				onRecvAddBattleBuildingByPlayer( id , (PT_REGION_C2F_ADD_BUILDING_DATA*)data->data);
				break;
			case PT_REGION_C2F_REMOVE_BUILDING:
				onRecvRemoveBattleBuildingByPlayer( id , (PT_REGION_C2F_REMOVE_BUILDING_DATA*)data->data);
				break;
			case PT_REGION_C2F_CREATE_TROOP_RET:
				onRecvCreateTroopRet( id , (PT_REGION_C2F_CREATE_TROOP_RET_DATA*)data->data);
				break;
			case PT_REGION_C2M_ADD_TROOP_TEST:
				onRecvAddTroopTest( id , (PT_REGION_C2M_ADD_TROOP_TEST_DATA*)data->data);
				break;
			case PT_REGION_C2F_PLAYER_CHARACTER_ALIVE:
				onRecvPlayerCharacterSetAlive( id , (PT_REGION_C2F_PLAYER_CHARACTER_ALIVE_DATA*)data->data);
				break;
			case PT_REGION_C2M_TEST:
				onRecvTest( id , (PT_REGION_C2M_TEST_DATA*)data->data);
				break;
			case PT_REGION_C2M_TEST1:
				onRecvTest1( id , (PT_REGION_C2M_TEST_DATA*)data->data);
				break;
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	Bool RegionNetPacketProcesser::processLoginServerPacket(I32 id, NetEventRecv* packet)
	{
		return false;
	}

	//--------------------------------------------------------------------------
	Bool RegionNetPacketProcesser::processMapServerPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_REGION )
		{   
			//FUNDETECTION(__MG_FUNC__);
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
			case PT_REGION_M2F_PLAYER_CHARACTER_ALIVE:
				{
					onRecvPlayerCharacterAliveFromMapServer(id , (PT_REGION_M2F_PLAYER_CHARACTER_ALIVE_DATA*)data->data);
					break;
				}
			default:
				{
					DYNAMIC_ASSERT(0);
					break;
				}
				
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvPlayerCharacterAliveFromMapServer(I32 id , PT_REGION_M2F_PLAYER_CHARACTER_ALIVE_DATA*data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getGameObjectManager()->getPlayerByAccountId(data->accountId);
		if (player.isNull())return;

		SClan* clan = player->getMainClan();
		if (!clan)
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		SPlayerCharacter* playerCharacter = clan->getPlayerCharacterFromList(data->playerCharacterId);
		DYNAMIC_ASSERT(NULL != playerCharacter);

		playerCharacter->setAlive();

		sendPlayerCharacterAliveToClient(player, playerCharacter);
#endif
	}

    //--------------------------------------------------------------------------
	void RegionNetPacketProcesser::sendPlayerCharacterAliveToClient(SPlayerPtr player, SPlayerCharacter* playerCharacter)
	{
		// 获得一个发送【客户端】缓存
		GameNetSendDataBufferPtr dataPtr ;
        FrontServerMain::getInstance().getServerLauncher()->getClientSendDataBuffer( dataPtr );

		// 转换成标准包格式
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// 设置标签
			packet->channel = GNPC_REGION;
			packet->type    = PT_REGION_F2C_PLAYER_CHARACTER_ALIVE;

			// 转换逻辑包
			PT_REGION_F2C_PLAYER_CHARACTER_ALIVE_DATA* sendData = (PT_REGION_F2C_PLAYER_CHARACTER_ALIVE_DATA*)(packet->data);
			if ( sendData )
			{
				// 填充逻辑包
				sendData->playerCharacterId	= playerCharacter->getID();

				// 发送数据
				/*dataPtr->send( GameNetPacketData_INFO::headSize + PT_REGION_F2C_PLAYER_CHARACTER_ALIVE_INFO::dataSize, 
					player->getClientNetID() );*/
				FrontServerMain::getInstance().getServerLauncher()->sendClient(dataPtr,GameNetPacketData_INFO::headSize + PT_REGION_F2C_PLAYER_CHARACTER_ALIVE_INFO::dataSize, 
					player->getClientNetID());
			}
		}
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvAddBattleBuildingByPlayer(I32 id , PT_REGION_C2F_ADD_BUILDING_DATA*data)
	{
#if 0
		SPlayer* player = SWorldManager::getInstance().getPlayerByClientNetId(id);
		
		if(NULL == player)
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		if (data->addBuildingNum <= 0)
		{
			DYNAMIC_ASSERT(0);
			return;
		}

        ///////////////////////////////////////////////////////////////////////////

        // 获得一个发送【服务端】缓存
        GameNetSendDataBufferPtr dataPtr = FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer();

        // 转换成标准包格式
        GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
        if ( packet )
        {
            // 设置标签
            packet->channel = GNPC_REGION;
            packet->type    = PT_REGION_F2M_ADD_BUILDING;

            // 转换逻辑包
            PT_REGION_F2M_ADD_BUILDING_DATA* sendData = (PT_REGION_F2M_ADD_BUILDING_DATA*)(packet->data);
            if ( sendData )
            {
                // 填充逻辑包
                sendData->accountId		                = player->getAccountID();
                sendData->characterId	                = data->characterId;
                sendData->regionId		                = data->regionId;
                sendData->regionType	                = data->regionType;

                sendData->addBuildingNum                = data->addBuildingNum;
                sendData->addBuildingInfo->templateId   = data->addBuildingInfo->templateId;
                sendData->addBuildingInfo->pos.x        = data->addBuildingInfo->pos.x;
                sendData->addBuildingInfo->pos.z        = data->addBuildingInfo->pos.z;

                // 发送数据
                ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
                    GameNetPacketData_INFO::headSize + PT_REGION_F2M_ADD_BUILDING_DATA_INFO::dataSize );
            }
        }
#endif
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvCreateTroopRet(I32 id , PT_REGION_C2F_CREATE_TROOP_RET_DATA*data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);

		if(player.isNull())
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		// 获得一个发送【服务端】缓存
		GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// 转换成标准包格式
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// 设置标签
			packet->channel = GNPC_REGION;
			packet->type    = PT_REGION_F2M_CREATE_TROOP_RET;

			// 转换逻辑包
			PT_REGION_F2M_CREATE_TROOP_RET_DATA* sendData = (PT_REGION_F2M_CREATE_TROOP_RET_DATA*)(packet->data);
			if ( sendData )
			{
				// 填充逻辑包
				sendData->accountId		= player->getAccountID();
				sendData->troopGroupId		= data->troopGroupId;
				/*sendData->pos.x			= data->pos.x;
				sendData->pos.z			= data->pos.z;*/
				//sendData->pos			= data->pos;
				sendData->regionId	 	= data->regionId;
				sendData->regionType 	= data->regionType;
				sendData->instanceUId 	= data->instanceId;
				sendData->troopNum 		= data->troopNum;
			
				for(UInt i = 0; i < data->troopNum; ++i)
				{
					/*sendData->soldierPos[i].x = data->soldierPos[i].x;
					sendData->soldierPos[i].z = data->soldierPos[i].z;*/
					sendData->troopPos[i] = data->troopPos[i];
				}

				// 发送数据
				ServerManager::getInstance().sendNetPacketToActiveMapServerByRegionId( dataPtr,
					GameNetPacketData_INFO::headSize + PT_REGION_F2M_CREATE_TROOP_RET_INFO::dataSize(sendData),
					(REGION_OBJ_TYPE)data->regionType, data->regionId);
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvAddTroopTest(I32 id , PT_REGION_C2M_ADD_TROOP_TEST_DATA* data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);

		if(player.isNull())
		{
			DYNAMIC_ASSERT(0);
			return;
		}


		// 获得一个发送【服务端】缓存
		GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// 转换成标准包格式
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// 设置标签
			packet->channel = GNPC_REGION;
			packet->type    = PT_REGION_C2M_ADD_TROOP_TEST;

			// 转换逻辑包
			PT_REGION_C2M_ADD_TROOP_TEST_DATA* sendData = (PT_REGION_C2M_ADD_TROOP_TEST_DATA*)(packet->data);
			if ( sendData )
			{
				// 填充逻辑包
				sendData->accountId		= data->accountId;

				// 发送数据
				ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
					GameNetPacketData_INFO::headSize + PT_REGION_C2M_ADD_TROOP_TEST_INFO::dataSize);
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvPlayerCharacterSetAlive(I32 id , PT_REGION_C2F_PLAYER_CHARACTER_ALIVE_DATA*data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);

		if(player.isNull())
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		SPlayerCharacter* playerCharacter = player->getPlayerCharacter(data->playerCharacterId);
		DYNAMIC_ASSERT(playerCharacter);
		
		if (playerCharacter->getHp() > 0)
		{
			return;
		}

		// 获得一个发送【服务端】缓存
		GameNetSendDataBufferPtr dataPtr ;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// 转换成标准包格式
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// 设置标签
			packet->channel = GNPC_REGION;
			packet->type    = PT_REGION_F2M_PLAYER_CHARACTER_ALIVE;

			// 转换逻辑包
			PT_REGION_F2M_PLAYER_CHARACTER_ALIVE_DATA* sendData = (PT_REGION_F2M_PLAYER_CHARACTER_ALIVE_DATA*)(packet->data);
			if ( sendData )
			{
				// 填充逻辑包
				sendData->accountId			= data->accountId;
				sendData->playerCharacterId	= data->playerCharacterId;
				sendData->aliveType			= data->aliveType;
				
				// 发送数据
				Int dataSize = GameNetPacketData_INFO::headSize + PT_REGION_F2M_PLAYER_CHARACTER_ALIVE_INFO::dataSize;
				ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,dataSize);
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvTest(I32 id , PT_REGION_C2M_TEST_DATA* data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);

		if(player.isNull())
		{
			DYNAMIC_ASSERT(0);
			return;
		}


		SClan* pSClan = player->getMainClan();
		DYNAMIC_ASSERT(pSClan);

		SPlayerCharacter* pSPlayerCharacter = pSClan->getMainPlayerCharacter();
		DYNAMIC_ASSERT(pSPlayerCharacter);
#endif
#if 0
		SItemManager* pItemManager = pSClan->getItemManager();
		DYNAMIC_ASSERT(pItemManager);

		pItemManager->handlAddExp(10000, CHAR_LVL_USETYPE_CHAR_LVL, pSPlayerCharacter);
		
		pItemManager->handlAddMoney(5, ITEM_MONEY_CLASS_MONEY_RMB_PRESENT);

		PlayerItem*  pItem = pItem = pItemManager->handlAddItem(7, 1);
		ItemPacketProcesser::getInstance().sendAddItemToClient(id, pItem);

		pItem = pItemManager->handlAddItem(8, 1);
		ItemPacketProcesser::getInstance().sendAddItemToClient(id, pItem);

		pItem = pItemManager->handlAddItem(11, 1);
		ItemPacketProcesser::getInstance().sendAddItemToClient(id, pItem);

		pItem = pItemManager->handlAddItem(13, 1);
		ItemPacketProcesser::getInstance().sendAddItemToClient(id, pItem);
		return;
#endif
		///////////////////////////////////////////////////////////////////////////

		// 获得一个发送【服务端】缓存
		GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// 转换成标准包格式
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// 设置标签
			packet->channel = GNPC_REGION;
			packet->type    = PT_REGION_C2M_TEST;

			// 转换逻辑包
			PT_REGION_C2M_TEST_DATA* sendData = (PT_REGION_C2M_TEST_DATA*)(packet->data);
			if ( sendData )
			{
				// 填充逻辑包
				sendData->accountId		= data->accountId;

				// 发送数据
				ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
					GameNetPacketData_INFO::headSize + PT_REGION_C2M_TEST_INFO::dataSize);
			}
		}
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvTest1(I32 id , PT_REGION_C2M_TEST_DATA*data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);

		if(player.isNull())
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		// 获得一个发送【服务端】缓存
		GameNetSendDataBufferPtr dataPtr ;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// 转换成标准包格式
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// 设置标签
			packet->channel = GNPC_REGION;
			packet->type    = PT_REGION_C2M_TEST1;

			// 转换逻辑包
			PT_REGION_C2M_TEST_DATA* sendData = (PT_REGION_C2M_TEST_DATA*)(packet->data);
			if ( sendData )
			{
				// 填充逻辑包
				sendData->accountId		= data->accountId;

				const Int dataSize = GameNetPacketData_INFO::headSize + PT_REGION_C2M_TEST_INFO::dataSize;
				// 发送数据
				ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
					dataSize);
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void RegionNetPacketProcesser::onRecvRemoveBattleBuildingByPlayer(I32 id , PT_REGION_C2F_REMOVE_BUILDING_DATA*data)
	{
#if 0
		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);

		if(player.isNull())
		{
			DYNAMIC_ASSERT(0);
			return;
		}

        // 获得一个发送【服务端】缓存
        GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

        // 转换成标准包格式
        GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
        if ( packet )
        {
            // 设置标签
            packet->channel = GNPC_REGION;
            packet->type    = PT_REGION_F2M_REMOVE_BUILDING;

            // 转换逻辑包
            PT_REGION_F2M_REMOVE_BUILDING_DATA* sendData = (PT_REGION_F2M_REMOVE_BUILDING_DATA*)(packet->data);
            if ( sendData )
            {
                // 填充逻辑包
                sendData->accountId  = player->getAccountID();
                sendData->buildingId = data->buildingId;

                // 发送数据
                ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
                    GameNetPacketData_INFO::headSize + PT_REGION_F2M_REMOVE_BUILDING_DATA_INFO::dataSize);
            }
        }
#endif
	}

}