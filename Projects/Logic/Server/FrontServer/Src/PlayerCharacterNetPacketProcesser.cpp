
/******************************************************************************/
#include "stdafx.h"
#include "PlayerCharacterNetPacketProcesser.h"
#include "FrontServerMain.h"
#include "PlayerCharacterNetPacket.h"
//#include "SWorldManager.h"
#include "ServerManager.h"
#include "SkillObject.h"
#include "SWorldManager.h"
#include "SSkillOwnManager.h"
#include "SClanQuestManager.h"
#include "PlaceObjectData.h"
/******************************************************************************/




namespace MG
{
	//--------------------------------------------------------------------------
	PlayerCharacterNetPacketProcesser::PlayerCharacterNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	PlayerCharacterNetPacketProcesser::~PlayerCharacterNetPacketProcesser()
	{

	}

    //--------------------------------------------------------------------------
    Bool PlayerCharacterNetPacketProcesser::processClientPacket(I32 id, NetEventRecv* packet)
    {
		if ( packet->getChannel() == GNPC_PLAYER_CHARACTER )
		{   
			//FUNDETECTION(__MG_FUNC__);
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
				case PT_PLAYERCHARACTER_C2F_JUMP_REGION:
					onRecvPlayerCharacterJumpRegionFromClient( id , (PT_PLAYERCHARACTER_C2F_JUMP_REGION_DATA*)data->data);
					break;
				case PT_PLAYERCHARACTER_C2F_JUMP_REGION_TIRO:
					onRecvPlayerCharacterJumpTiroRegionFromClient( id , (PT_PLAYERCHARACTER_C2F_JUMP_REGION_TIRO_DATA*)data->data);
					break;
				case PT_PLAYERCHARACTER_C2F_JUMP_OUT_REGION:
					onRecvPlayerCharacterJumpOutRegionFromClient( id , (PT_PLAYERCHARACTER_C2F_JUMP_OUT_REGION_DATA*)data->data);
					break; 
				case PT_PLAYERCHARACTER_C2F_JUMP_LAST_REGION:
					onRecvPlayerCharacterJumpLastRegionFromClient( id , (PT_PLAYERCHARACTER_C2F_JUMP_LAST_REGION_DATA*)data->data);
					break; 
				case PT_PLAYERCHARACTER_C2S_MOVE_POS:
					//onRecvPlayerCharacterMoveClient( id , (PT_PLAYERCHARACTER_C2S_MOVE_POS_DATA*)data->data);
					break;
				default:
					DYNAMIC_ASSERT(0);
					break;
			}

			return true;
		}
		return false;
    }

    //--------------------------------------------------------------------------
    Bool PlayerCharacterNetPacketProcesser::processLoginServerPacket(I32 id, NetEventRecv* packet)
    {

        return false;
    }

    //--------------------------------------------------------------------------
    Bool PlayerCharacterNetPacketProcesser::processMapServerPacket(I32 id, NetEventRecv* packet)
    {
		if ( packet->getChannel() == GNPC_PLAYER_CHARACTER )
		{   
			//FUNDETECTION(__MG_FUNC__);
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());
			switch (data->type)
			{
            /*case PT_PLAYERCHARACTER_M2F_ATTR:
                onRecvPlayerCharacterDamage(id, (PT_PLAYERCHARACTER_M2F_ATTR_DATA*)data->data);
                break;*/
			case PT_PLAYERCHARACTER_M2F_JUMP_REGION:
				onRecvPlayerCharacterJumpRegionFromMapServer(id, (PT_PLAYERCHARACTER_M2F_JUMP_REGION_DATA*)data->data);
				break;
			}

			return true;
		}
        return false;
    }
    //--------------------------------------------------------------------------
//	void PlayerCharacterNetPacketProcesser::onRecvPlayerCharacterMoveClient(I32 id , PT_PLAYERCHARACTER_C2S_MOVE_POS_DATA* data)
//	{
//#if NEW_VERSION_LINHU
//		SPlayerPtr player = SWorldManager::getInstance().getGameObjectManager()->getPlayerByClientNetId(id);
//
//#else
//		SPlayerPtr player = SWorldManager::getInstance().getPlayerByClientNetId(id);
//#endif
//
//		//SPlayer* player = SWorldManager::getInstance().getPlayerByClientNetId(id);
//
//		if(!player.isNull())
//		{
//			SClan* clan = player->getMainClan();
//			SPlayerCharacter* playerCharacter = clan->getPlayerCharacterFromList(data->playercharacter_id);
//
//			if(playerCharacter)
//			{
//                // playerCharacter �޷���õ�ͼ��ţ�ֻ���Ȼ��regionType��regionId֮�󣬲ſ��Ի�õ�ͼ��š�
//                //��������������
//				//I32 id = ServerManager::getInstance().getMapServerNetID(playerCharacter->mMapId);
//				playerCharacter->getMapServerNetId();
//				playerCharacter->getRegionId();
//				playerCharacter->getRegionType();
//				playerCharacter->getInstanceUId();
//				
//				sendNetPacketToActiveMapServerByNetId
//				sendPlayerCharacterMoveToMapServer(id, data);
//			}
//			
//		}
//	}


    //--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterMoveToMapServer(I32 id, PT_PLAYERCHARACTER_C2S_MOVE_POS_DATA* data )
	{

        ///////////////////////////////////////////////////////////////////////////

        // ���һ�����͡�����ˡ�����
        GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

        // ת���ɱ�׼����ʽ
        GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
        if ( packet )
        {
            // ���ñ�ǩ
            packet->channel = GNPC_PLAYER_CHARACTER;
            packet->type    = PT_PLAYERCHARACTER_C2S_MOVE_POS;

            // ����
            memcpy( packet->data, (Char8*)data, PT_PLAYERCHARACTER_C2S_MOVE_POS_DATA_INFO::dataSize );

            // �㲥����ͼ������
			



            //ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
            //    GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_C2S_MOVE_POS_DATA_INFO::dataSize);
        }

        ///////////////////////////////////////////////////////////////////////////

	}
    //--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::onRecvPlayerCharacterJumpRegionFromClient( I32 id , PT_PLAYERCHARACTER_C2F_JUMP_REGION_DATA*data)
	{
		const PlayerCharacterIdType	playerCharacterId	= data->playerCharacterId;
		const RegionIdType			toRegionId			= data->toRegionId;
		const REGION_OBJ_TYPE		toRegionType		= (REGION_OBJ_TYPE)data->toRegionType;
		const InstanceUIdType		toInstanceUId		= data->toInstanceUId;
		

		SPlayerPtr playerPtr;// = SWorldManager::getInstance().getGameObjectManager()->getPlayerByClientNetId(id);
		if (playerPtr.isNull())
		{
			DYNAMIC_ASSERT(false);
			return;
		}
		SPlayerCharacter* playerCharacter;// =  playerPtr->getPlayerCharacter(data->playerCharacterId); 
		if (NULL == playerCharacter)
		{
			return;
		}

		//�޸Ľ�ɫ��ͼ����
		if(true == ServerManager::getInstance().isMapServerCrash((REGION_OBJ_TYPE)data->toRegionType, data->toRegionId))
		{
			//MapServer ����
			//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Fail_Crash, data->playerCharacterId, data->toRegionId, (REGION_OBJ_TYPE)data->toRegionType, data->toInstanceUId);
			return;
		}

		sendPlayerCharacterJumpToRegionNewVer(playerCharacter, toRegionType, toRegionId, toInstanceUId, data->pointId);
	}

	//--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::onRecvPlayerCharacterJumpTiroRegionFromClient( I32 id , PT_PLAYERCHARACTER_C2F_JUMP_REGION_TIRO_DATA*data)
	{
		const PlayerCharacterIdType	playerCharacterId	= data->playerCharacterId;
		const RegionIdType			toRegionId			= data->toRegionId;
		const REGION_OBJ_TYPE		toRegionType		= (REGION_OBJ_TYPE)data->toRegionType;
		const InstanceUIdType		toInstanceUId		= data->toInstanceUId;
		const NetIdType				mapServerNetId		= data->mapServerNetId;

		DYNAMIC_ASSERT(toRegionType == ROT_TIRO && mapServerNetId > 0);
		

		SPlayerPtr playerPtr;// = SWorldManager::getInstance().getGameObjectManager()->getPlayerByClientNetId(id);
		if (playerPtr.isNull())
		{
			DYNAMIC_ASSERT(false);
			return;
		}
		SPlayerCharacter* playerCharacter;// =  playerPtr->getPlayerCharacter(data->playerCharacterId); 
		if (NULL == playerCharacter)
		{
			return;
		}

		//�޸Ľ�ɫ��ͼ����
		if(true == ServerManager::getInstance().isMapServerCrash((REGION_OBJ_TYPE)data->toRegionType, data->toRegionId))
		{
			//MapServer ����
			//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Fail_Crash, data->playerCharacterId, data->toRegionId, (REGION_OBJ_TYPE)data->toRegionType, data->toInstanceUId);
			return;
		}

		sendPlayerCharacterJumpToRegionNewVer(playerCharacter, toRegionType, toRegionId, toInstanceUId, 0, mapServerNetId);
	}

	//--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::onRecvPlayerCharacterJumpOutRegionFromClient( I32 id , PT_PLAYERCHARACTER_C2F_JUMP_OUT_REGION_DATA*data)
	{
		//TODO:�Ƿ����jump
		//sendPlayerCharacterJumpRegionToClient( id, ChatarcterJumpRegionResult_Succeed);

		//�޸Ľ�ɫ��ͼ����
		//if(true == ServerManager::getInstance().isMapServerCrash(data->toRegionId))
		//{
		//	//MapServer ����
		//	//send error msg to client
		//	//sendJumpRegionErrorToClient(id, PlayerCharacter_Jump_server_crash);

		//	SPlayerCharacter* playerCharacter = SWorldManager::getInstance().getGameObjectManager()->getPlayerCharacter(data->playerCharacterId);
		//	if (NULL == playerCharacter)
		//	{
		//		return;
		//	}

		//	sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Fail_Crash, data->playerCharacterId, (REGION_OBJ_TYPE)data->toRegionType, data->toRegionId, data->toInstanceId);
		//	return;
		//}

		//SWorldManager::getInstance().printWorldObject();

		//��ʾ�϶��ܳɹ���ת�� ��������ֱ������clan��Region��Ϣ
		//clan->setattRegin();
		const PlayerCharacterIdType playerCharacterId = data->playerCharacterId;

		SPlayerCharacter* playerCharacter;// = SWorldManager::getInstance().getPlayerCharacter(data->accountId, playerCharacterId);
		DYNAMIC_ASSERT(NULL != playerCharacter);

		SRegionObjectBase* curRegion = playerCharacter->getCurrentRegionObject();
		DYNAMIC_ASSERT(NULL != curRegion);

		if (ROT_PLACE != curRegion->getRegionType())
		{
			return;
		}

		PlaceListCsvInfo* placeListCsvInfo = ((PlaceObjectData*)curRegion->getRegionData())->getPlaceListCsvInfo();
		DYNAMIC_ASSERT(NULL != placeListCsvInfo && placeListCsvInfo->mapDistrListId > 0);

		sendPlayerCharacterJumpToRegionNewVer(playerCharacter, ROT_MAP_DISTRIBUTION, placeListCsvInfo->mapDistrListId, 0, placeListCsvInfo->artPointResId);
		//broadcastPlayerCharacterJumpToRegion(data->playerCharacterId, ROT_MAP_DISTRIBUTION,  placeListCsvInfo->mapDistrListId, 0, placeListCsvInfo->artPointResId);
	}

	//--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::onRecvPlayerCharacterJumpLastRegionFromClient( I32 id , PT_PLAYERCHARACTER_C2F_JUMP_LAST_REGION_DATA*data)
	{
		//TODO:�Ƿ����jump
		SPlayerPtr playerPtr;// = SWorldManager::getInstance().getGameObjectManager()->getPlayerByClientNetId(id);
		if (playerPtr.isNull())
		{
			return;
		}

		SPlayerCharacter* playerCharacter;// = playerPtr->getPlayerCharacter(data->playerCharacterId);
		if (NULL == playerCharacter)
		{
			return;
		}

		const RegionIdType		lastRegionId	= playerCharacter->getLastRegionId();
		const REGION_OBJ_TYPE	lastRegionType	= playerCharacter->getLastRegionType();
		InstanceUIdType	lastInstanceUId			= playerCharacter->getLastInstanceUId();

		if (lastRegionType == ROT_UNKNOWN || lastRegionId <= 0)
		{
			//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Fail, data->playerCharacterId, lastRegionId, lastRegionType, lastInstanceUId);
			return;
		}

		//�޸Ľ�ɫ��ͼ����
		if(true == ServerManager::getInstance().isMapServerCrash(playerCharacter->getLastRegionType(), lastRegionId))
		{
			//MapServer ����
			//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Fail_Crash, data->playerCharacterId, lastRegionId, lastRegionType, lastInstanceUId);
			return;
		}


		if (playerCharacter->getRegionId()		== lastRegionId		&& 
			playerCharacter->getRegionType()	== lastRegionType	&& 
			playerCharacter->getInstanceUId()	== lastInstanceUId)
		{
			//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Fail, data->playerCharacterId, lastRegionId, lastRegionType, lastInstanceUId);
			return;
		}

		if (lastRegionType == ROT_PVE || lastRegionType == ROT_TIRO)
		{
			lastInstanceUId = 0;
		}

		sendPlayerCharacterJumpToRegionNewVer(playerCharacter, lastRegionType, lastRegionId, lastInstanceUId);
		//broadcastPlayerCharacterJumpToRegion(data->playerCharacterId, lastRegionType, lastRegionId, lastInstanceUId);
	}

	//--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::onRecvPlayerCharacterJumpRegionFromMapServer(I32 id, PT_PLAYERCHARACTER_M2F_JUMP_REGION_DATA* data)
	{
		SPlayerPtr playerPtr;// = SWorldManager::getInstance().getGameObjectManager()->getPlayerByClientNetId(data->clientNetIdInFront);
		if (playerPtr.isNull())
		{
			// ���ܿͻ����Ѿ��˳���Ͽ�����
			// �Ժ��ٿ���ԭ��
			//DYNAMIC_ASSERT(false);
			return;
		}

		const PlayerCharacterIdType	playerCharacterId	= data->playerCharacterId;
		const RegionIdType			regionId			= data->toRegionId;
		const REGION_OBJ_TYPE		regionType			= (REGION_OBJ_TYPE)data->toRegionType;
		const InstanceUIdType		instanceUId			= data->toInstanceId;

		SPlayerCharacter* playerCharacter;// =  playerPtr->getPlayerCharacter(playerCharacterId); 
		if (NULL == playerCharacter)
		{
			return;
		}

		if (ChatarcterJumpRegionResult_Succeed != data->jumpResult)
		{
			//��תʧ��
			//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), data->jumpResult, playerCharacterId, regionId, regionType, instanceUId);
			//send error

			return;
		}

		playerCharacter->setRegion(regionType, regionId, instanceUId);
		playerCharacter->setMapServerNetId(id);
		//sendPlayerCharacterJumpRegionToClient(playerCharacter->getBelongPlayer()->getClientNetID(), ChatarcterJumpRegionResult_Succeed, data->playerCharacterId, regionId, regionType, instanceUId);

		//SWorldManager::getInstance().printWorldObject();
		//�����ж�
        SClanQuestManager* questManager;// = playerPtr->getQuestManager();
		DYNAMIC_ASSERT(questManager);
		questManager->notifyLeaveMap();

		//��ʱ�����⡣����ʱ���ع���ʱ������
		//�쿪��
		//playerCharacter->sendDataInfoOnJoinGame();
	}

    //-------------------------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterJumpRegionByMapServerNetIdNewVer(SPlayerCharacter* playerCharacter, NetIdType mapServerNetId, REGION_OBJ_TYPE regionType, RegionIdType regionId, InstanceUIdType instanceUId, Int pointId)
	{
		//3. ��ʼ����ͼ
		// ���һ�����͡�����ˡ�����
		GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// ת���ɱ�׼����ʽ
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// ���ñ�ǩ
			packet->channel = GNPC_PLAYER_CHARACTER;
			packet->type    = PT_PLAYERCHARACTER_F2M_JUMP_REGION;

			// ת���߼���
			PT_PLAYERCHARACTER_F2M_JUMP_REGION_DATA* sendData = (PT_PLAYERCHARACTER_F2M_JUMP_REGION_DATA*)(packet->data);
			if ( sendData )
			{
				// ����߼���
				sendData->playerCharacterId = playerCharacter->getID();
				sendData->toRegionId        = regionId;
				sendData->toRegionType      = regionType;
				sendData->toInstanceId		= instanceUId;
				sendData->pointId			= pointId;

				const Int dataSize = GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2M_JUMP_REGION_INFO::dataSize;
				// ��������
				ServerManager::getInstance().sendNetPacketToActiveMapServerByNetId( dataPtr, 
					dataSize,mapServerNetId);
			}
		}
	}

    //-------------------------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterJumpToRegionNewVer(SPlayerCharacter* playerCharacter, REGION_OBJ_TYPE regionType, RegionIdType regionId, InstanceUIdType instanceUId, Int pointId, NetIdType mapServerNetId)
	{
		//1. ���ϴε�ͼ��ɾ���佫ָ��
		//sendPlayerCharacterJumpToEmptyRegionNewVer(playerCharacter->getBelongPlayer()->getAccountID(), playerCharacter->getID(), playerCharacter->getMapServerNetId());

		//2. ����regionType ȷ������ͼĿ�ĵ�
		if (mapServerNetId == 0)
		{
			mapServerNetId = SWorldManager::getInstance().getMapServerNetIdByRegion(regionType, regionId);
		}

		sendPlayerCharacterJumpRegionByMapServerNetIdNewVer(playerCharacter, mapServerNetId, regionType, regionId, instanceUId, pointId);
	}

    //-------------------------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterJumpToEmptyRegionNewVer(AccountIdType accountId, PlayerCharacterIdType playerCharacterId, NetIdType mapServerNetId)
	{
		// ���һ�����͡�����ˡ�����
		GameNetSendDataBufferPtr dataPtr ;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// ת���ɱ�׼����ʽ
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// ���ñ�ǩ
			packet->channel = GNPC_PLAYER_CHARACTER;
			packet->type    = PT_PLAYERCHARACTER_F2M_JUMP_EMPTY_REGION;

			// ת���߼���
			PT_PLAYERCHARACTER_F2M_JUMP_EMPTY_REGION_DATA* sendData = (PT_PLAYERCHARACTER_F2M_JUMP_EMPTY_REGION_DATA*)(packet->data);
			if ( sendData )
			{
				// ����߼���
				sendData->accountId			= accountId;
				sendData->playerCharacterId = playerCharacterId;

				// ��������
				ServerManager::getInstance().sendNetPacketToActiveMapServerByNetId( dataPtr, 
					GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2M_JUMP_EMPTY_REGION_INFO::dataSize, mapServerNetId);
			}
		}
	}

	//--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::broadcastPlayerCharacterJumpToCampaignRegion(PlayerCharacterIdType playerCharacterId, PT_CAMPAIGN_T2F_START_JUMP_REGION_DATA* data)
	{
		// ���һ�����͡�����ˡ�����
		GameNetSendDataBufferPtr dataPtr;
        FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		// ת���ɱ�׼����ʽ
		GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
		if ( packet )
		{
			// ���ñ�ǩ
			packet->channel = GNPC_PLAYER_CHARACTER;
			packet->type    = PT_PLAYERCHARACTER_F2M_JUMP_CAMPAIGN_REGION;

			// ת���߼���
			PT_PLAYERCHARACTER_F2M_JUMP_CAMPAIGN_REGION_DATA* sendData = (PT_PLAYERCHARACTER_F2M_JUMP_CAMPAIGN_REGION_DATA*)(packet->data);
			if ( sendData )
			{
				// ����߼���
				sendData->playerCharacterId	 		= playerCharacterId;
				sendData->toRegionId       	 		= data->toRegionId;
				sendData->toRegionType     	 		= data->toRegionType;
				sendData->toInstanceId				= data->toInstanceId;
				sendData->jumpType					= data->jumpType;
				sendData->jumpIndex					= data->jumpIndex;
				sendData->isBattleLeader			= data->isBattleLeader;
				sendData->prepareTimeSecond			= data->prepareTimeSecond;

				const Int dataSize = GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2M_JUMP_CAMPAIGN_REGION_INFO::dataSize;

				// ��������
				ServerManager::getInstance().sendNetPacketToActiveMapServerByRegionId( dataPtr, 
					dataSize, (REGION_OBJ_TYPE)data->toRegionType, data->toRegionId);
			}
		}
	}

    //--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterJumpRegionToClient( I32 netId, ChatarcterJumpRegionResult result, PlayerCharacterIdType playerCharacterId, RegionIdType regionId, REGION_OBJ_TYPE regionType, InstanceUIdType instanceUId)
	{
        // ���һ�����͡��ͻ��ˡ�����
        GameNetSendDataBufferPtr dataPtr ;
        FrontServerMain::getInstance().getServerLauncher()->getClientSendDataBuffer( dataPtr );

        // ת���ɱ�׼����ʽ
        GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
        if ( packet )
        {
            // ���ñ�ǩ
            packet->channel = GNPC_PLAYER_CHARACTER;
            packet->type    = PT_PLAYERCHARACTER_F2C_JUMP_REGION;

            // ת���߼���
            PT_PLAYERCHARACTER_F2C_JUMP_REGION_DATA* sendData = (PT_PLAYERCHARACTER_F2C_JUMP_REGION_DATA*)(packet->data);
            if ( sendData )
            {
                // ����߼���
		        sendData->jumpResult		= result;
				sendData->playerCharacterId = playerCharacterId;
				sendData->regionId			= regionId;
				sendData->regionType		= regionType;
				sendData->instanceUId		= instanceUId;

                // ��������
				FrontServerMain::getInstance().getServerLauncher()->sendClient(dataPtr,
					GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2C_JUMP_REGION_INFO::dataSize,
					netId );
            }
        }
	}
    
	//-------------------------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterrSkillInfoToMapServer(I32 mapserverNetid, IdType characterId, SSkillOwnManager* skillOwnManager )
	{
		// ���һ�����͡��ͻ��ˡ�����
		GameNetSendDataBufferPtr dataPtr;

		// ת���ɱ�׼����ʽ
		GameNetPacketData* packet       = NULL;

		// ת���߼���
		PT_PLAYERCHARACTER_F2M_SKILL_INFO_DATA* sendData = NULL;

		// ��ǰ��������ͷ��С
		I32 sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2M_SKILL_INFO_DATA_INFO::headSize;
		// ��ǰ�������ݴ�С
		I32 sendDataSize        = sendDataHeadSize;
		// ׼���������ݴ�С
		I32 prepareSendDataSize = sendDataHeadSize;


		// �������ܶ���
		IdType skillId = 0;
		for(UInt i = 0; i < skillOwnManager->getOwnSkillNum(); ++i)
		{

			// ��ü��ܶ���
			skillId = skillOwnManager->getOwnerSkillByIndex(i);
			if(skillId == 0)
			{
				continue;
			}

			//-------------------------------------------------------------------------------------------

			if ( dataPtr.isNull() == false )
			{
				// ����һ���������ݴ�С���������ݻ����С
				prepareSendDataSize = sendDataSize + PT_PLAYERCHARACTER_F2M_SKILL_INFO_DATA_INFO::elemetSize;

				// ������ڻ�������С����
				if ( prepareSendDataSize >= dataPtr->getLogicDataMaxSize() )
				{
					FrontServerMain::getInstance().getServerLauncher()->sendServer(dataPtr,sendDataSize, mapserverNetid);
					dataPtr.setNull();
				}
			}

			//-------------------------------------------------------------------------------------------

			if ( dataPtr.isNull() )
			{
				// ���һ�����͡��ͻ��ˡ�����
				FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

				// ת���ɱ�׼����ʽ
				packet       = (GameNetPacketData*) dataPtr->getLogicData();
				if ( !packet )
					return;

				// ���ñ�ǩ
				packet->channel = GNPC_PLAYER_CHARACTER;
				packet->type    = PT_PLAYERCHARACTER_F2M_SKILL_INFO;

				// ת���߼���
				sendData = (PT_PLAYERCHARACTER_F2M_SKILL_INFO_DATA*)(packet->data);
				sendData->playercharacterId = characterId;
				// ���
				sendData->skillNum = 0;

				// ��ǰ��������ͷ��С
				sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2M_SKILL_INFO_DATA_INFO::headSize;
				// ��ǰ�������ݴ�С
				sendDataSize        = sendDataHeadSize;
				// ׼���������ݴ�С
				prepareSendDataSize = sendDataHeadSize;
			}

			//-------------------------------------------------------------------------------------------

			// ���Ӽ��ܼ�¼
			SkillNetData* skillNetData = &(sendData->skillList[sendData->skillNum]);
			skillNetData->skillId      = skillId;

			// ��������
			sendDataSize += PT_PLAYERCHARACTER_F2M_SKILL_INFO_DATA_INFO::elemetSize;
			++sendData->skillNum;
		}

		//-------------------------------------------------------------------------------------------
		// �����ʣ������
		if ( dataPtr.isNull() == false )
		{
			FrontServerMain::getInstance().getServerLauncher()->sendServer(dataPtr,sendDataSize, mapserverNetid);
			dataPtr.setNull();
		}
		
	}

    //--------------------------------------------------------------------------
    void PlayerCharacterNetPacketProcesser::sendAllPlayerCharacterSkillInfoInClanToClient( I32 id, SClanPtr& clanPtr )
    {
		SPlayerCharacterPtr playerCharacter;
		clanPtr->getMainPlayerCharacter(playerCharacter);
		//playerCharacter.sendPlayerCharacterSkillInfoToClient();
  //      std::map<U64, SPlayerCharacter*>::iterator iter = mPlayerCharacterList.begin();
  //      for ( ;iter != mPlayerCharacterList.end();iter++ )
  //      {
  //          iter->second->sendPlayerCharacterSkillInfoToClient();
  //      }


    }

    //--------------------------------------------------------------------------
	void PlayerCharacterNetPacketProcesser::sendPlayerCharacterSkillInfoToClient(I32 id, IdType characterId, SSkillOwnManager* skillOwnManager)
	{		
        // ���һ�����͡��ͻ��ˡ�����
        GameNetSendDataBufferPtr dataPtr ;
        
        // ת���ɱ�׼����ʽ
        GameNetPacketData* packet       = NULL;

        // ת���߼���
        PT_PLAYERCHARACTER_F2C_SKILL_INFO_DATA* sendData = NULL;

        // ��ǰ��������ͷ��С
        I32 sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2C_SKILL_INFO_DATA_INFO::headSize;
        // ��ǰ�������ݴ�С
        I32 sendDataSize        = sendDataHeadSize;
        // ׼���������ݴ�С
        I32 prepareSendDataSize = sendDataHeadSize;

        // �������ܶ���
        IdType skillId = 0;
        for(UInt i = 0; i < skillOwnManager->getOwnSkillNum(); ++i)
        {

			// ��ü��ܶ���
			skillId = skillOwnManager->getOwnerSkillByIndex(i);
			if(skillId == 0)
			{
				continue;
			}


			//--------------------------------------------------------------------------

			if ( dataPtr.isNull() == false )
			{
				// ����һ���������ݴ�С���������ݻ����С
				prepareSendDataSize = sendDataSize + PT_PLAYERCHARACTER_F2C_SKILL_INFO_DATA_INFO::elemetSize;

				// ������ڻ�������С����
				if ( prepareSendDataSize >= dataPtr->getLogicDataMaxSize() )
				{
					//dataPtr->send( sendDataSize, id );
					FrontServerMain::getInstance().getServerLauncher()->sendClient(dataPtr,sendDataSize, id);
					
					dataPtr.setNull();
				}
			}

			//--------------------------------------------------------------------------

			if ( dataPtr.isNull() )
			{
				// ���һ�����͡��ͻ��ˡ�����
				FrontServerMain::getInstance().getServerLauncher()->getClientSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

				// ת���ɱ�׼����ʽ
				packet       = (GameNetPacketData*) dataPtr->getLogicData();
				if ( !packet )
					return;

				// ���ñ�ǩ
				packet->channel = GNPC_PLAYER_CHARACTER;
				packet->type    = PT_PLAYERCHARACTER_F2C_SKILL_INFO;

				// ת���߼���
				sendData = (PT_PLAYERCHARACTER_F2C_SKILL_INFO_DATA*)(packet->data);
				sendData->playercharacterId = characterId;
				// ���
				sendData->skillNum = 0;

				// ��ǰ��������ͷ��С
				sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_PLAYERCHARACTER_F2C_SKILL_INFO_DATA_INFO::headSize;
				// ��ǰ�������ݴ�С
				sendDataSize        = sendDataHeadSize;
				// ׼���������ݴ�С
				prepareSendDataSize = sendDataHeadSize;
			}

			//--------------------------------------------------------------------------
            
            // ���Ӽ��ܼ�¼
            SkillNetData* skillNetData = &(sendData->skillList[sendData->skillNum]);
            skillNetData->skillId      = skillId;

            // ��������
            sendDataSize += PT_PLAYERCHARACTER_F2C_SKILL_INFO_DATA_INFO::elemetSize;
            ++sendData->skillNum;
        }

		//--------------------------------------------------------------------------
        // �����ʣ������
        if ( dataPtr.isNull() == false )
        {
			FrontServerMain::getInstance().getServerLauncher()->sendClient(dataPtr,sendDataSize, id);
			dataPtr.setNull();
        }
	}

}