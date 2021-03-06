/******************************************************************************/
#include "stdafx.h"
#include "SCampaignNetPacketProcesser.h"
#include "CampaignNetPacket.h"
#include "NetTranspondPacketProcesser.h"
#include "MapServerMain.h"
#include "SPlayerNetPacketProcesser.h"
#include "SWorldManager.h"
#include "CampaignManager.h"

#include "ServerManager.h"
#include "SRegionNetPacketProcesser.h"
#include "SCamp.h"
#include "BattleScoreSystem.h"

#include "SAtkInstance.h"
#include "InstanceAtkListCsv.h"

#include "SPlayerManager.h"

/******************************************************************************/

namespace MG
{
	//--------------------------------------------------------------------------
	SCampaignNetPacketProcesser::SCampaignNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	SCampaignNetPacketProcesser::~SCampaignNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	Bool SCampaignNetPacketProcesser::processFrontServerPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_CAMPAIGN )
		{
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
			case PT_CAMPAIGN_F2M_START:
				onRecvCampaignStartFromFrontServer( id , (PT_CAMPAIGN_F2M_START_DATA*)data->data);
				break;			
			case PT_CAMPAIGN_F2M_CREATE_CAMP_BY_CONTROLER:
				onRecvCampaignCreateCampByControler( id , (PT_CAMPAIGN_F2M_CREATE_CAMP_BY_CONTROLER_DATA*)data->data);
				break;
			case PT_CAMPAIGN_F2M_CHANGE_CAMP_CONTROL:
				onRecvCampaignChangeCampControl( id , (PT_CAMPAIGN_F2M_CHANGE_CAMP_CONTROL_DATA*)data->data);
				break;
			case PT_CAMPAIGN_F2M_CHANGE_CAMP_OWNER:
				onRecvCampaignChangeCampOwner( id , (PT_CAMPAIGN_F2M_CHANGE_CAMP_OWNER_DATA*)data->data);
				break;
			case PT_CAMPAIGN_F2M_TROOP_CONTROLLER_LIST:
				onRecvCampaignTroopControllerList( id , (PT_CAMPAIGN_F2M_TROOP_CONTROLLER_LIST_DATA*)data->data);
				break;
			case PT_CAMPAIGN_F2M_END:
				onRecvCampaignEnd( id , (PT_CAMPAIGN_F2M_END_DATA*)data->data);
				break;
			default:
				{
					DYNAMIC_ASSERT(0);
				}
				break;
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	Bool SCampaignNetPacketProcesser::processCenterServerPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_CAMPAIGN )
		{
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
			case PT_CAMPAIGN_T2M_INVITE_DEFENSE_REGION:
				onRecvCampaignInviteDefenseFromCenterServer( id , (PT_CAMPAIGN_T2M_INVITE_DEFENSE_REGION_DATA*)data->data);
				break;
			default:
				{
					DYNAMIC_ASSERT(0);
				}
				break;
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignChangeCampControl(I32 id, PT_CAMPAIGN_F2M_CHANGE_CAMP_CONTROL_DATA* data)
	{
		//SInstanceRegion* realRegion;// = SWorldManager::getInstance().getInstanceRegion(ROT_PLACE, data->targetRegionId, data->targetInstanceUId);
		//DYNAMIC_ASSERT(NULL != realRegion);
		
		//realRegion->changeCampControl(data->campId, data->targetControlerAccountId);	//for test
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignChangeCampOwner(I32 id, PT_CAMPAIGN_F2M_CHANGE_CAMP_OWNER_DATA* data)
	{
		//SInstanceRegion* realRegion;// = SWorldManager::getInstance().getInstanceRegion(ROT_PLACE, data->targetRegionId, data->targetInstanceUId);
		//if (NULL == realRegion)
		//{
		//	return;
		//}
		//DYNAMIC_ASSERT(NULL != realRegion);

		//realRegion->changeCampOwner(data->accountId);
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignTroopControllerList(I32 id, PT_CAMPAIGN_F2M_TROOP_CONTROLLER_LIST_DATA* data)
	{
#if 0
		SPlayer* myPlayer = NULL;//SWorldManager::getInstance().getPlayerFromMemory(data->myAccountId);
		DYNAMIC_ASSERT(NULL != myPlayer);

		SInstanceRegion* realRegion = SWorldManager::getInstance().getInstanceRegion(ROT_PLACE, data->targetRegionId, data->targetInstanceUId);
		if (NULL == realRegion)
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		SRegionObjectBase* regionBase;// = realRegion->getNewRegionObject();
		DYNAMIC_ASSERT(NULL != regionBase);

		SCamp* camp;// = regionBase->getCampFromMemory(data->campId);
		DYNAMIC_ASSERT(NULL != camp);

		AccountIdType curControlTroopAccountId = camp->getControlTroopGroupAccountId();
		DYNAMIC_ASSERT(curControlTroopAccountId > 0);
		data->troopCurControllerAccountId = curControlTroopAccountId;

		//准备 发送消息
		NetTranspondSendDataBufferPtr dataPtr  ;
        NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
		if ( NULL == packet )
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		packet->channel = GNPC_CAMPAIGN;
		packet->type    = PT_CAMPAIGN_M2C_TROOP_CONTROLLER_LIST;
		PT_CAMPAIGN_M2C_TROOP_CONTROLLER_LIST_DATA* sendData = (PT_CAMPAIGN_M2C_TROOP_CONTROLLER_LIST_DATA*)(packet->data);
		if(NULL == sendData)
		{
			DYNAMIC_ASSERT(0);
			return;
		}


		//开始填充
		sendData->campId						= data->campId;
		sendData->campOwnerAccountId			= data->campOwnerAccountId;
		sendData->troopCurControllerAccountId	= data->troopCurControllerAccountId;
		sendData->campInfo.troopNum				= camp->getAllTroopNumInCamp();
		sendData->campInfo.campHp				= camp->getHp();
		sendData->addMemberNum					= data->addMemberNum;
		for (Int i=0; i<sendData->addMemberNum; ++i)
		{
			sendData->addMemberInfo[i].accountId			= data->addMemberInfo[i].accountId;
			sendData->addMemberInfo[i].templateId			= data->addMemberInfo[i].templateId;
			sendData->addMemberInfo[i].playerCharacterId	= data->addMemberInfo[i].playerCharacterId;
			sendData->addMemberInfo[i].charArtIcon			= data->addMemberInfo[i].charArtIcon;
			WStrnCpyS(sendData->addMemberInfo[i].playerCharacterName, data->addMemberInfo[i].playerCharacterName, MAX_CHARACTER_NAME_LEN);
		}

		//开始填充 End
		NetTranspondPacketProcesser::getInstance().sendClientTranspondSendDataToFrontServer(dataPtr,
			GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_TROOP_CONTROLLER_LIST_DATA_INFO::dataSize(sendData),
			myPlayer->getFrontServerId(), myPlayer->getClientNetIdInFrontServer(), myPlayer->getAccountId());
#endif
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignEnd(I32 id, PT_CAMPAIGN_F2M_END_DATA* data)
	{
#if 0
		//统计战功， 广播给所有的客户端
		SInstanceRegion* realRegion = SWorldManager::getInstance().getInstanceRegion(ROT_PLACE, data->targetRegionId, data->targetInstanceUId);
		if (NULL == realRegion)
		{
			return;
		}

		SRegionObjectBase* regionBase;// = realRegion->getNewRegionObject();
		DYNAMIC_ASSERT(NULL != regionBase);

		std::map<PlayerCharacterIdType,	SCharacter*>* playerCharacterList;// = regionBase->getPlayerCharacterList();

		//开始广播
		std::map<RegionIdType, SBigRegionObject*>* bigRegionList = NULL;	//SWorldManager::getInstance().getBigPlaceRegionList();
		for (std::map<RegionIdType, SBigRegionObject*>::iterator iter = bigRegionList->begin(); iter != bigRegionList->end(); ++iter)
		{
			SBigRegionObject* bigRegionObj;// = iter->second;
			//if (NULL == bigRegionObj || bigRegionObj->getInstanceType() != InstanceType_Atk)
			//{
			//	continue;
			//}

			SInstanceRegion* mainInstanceRegion;// = bigRegionObj->getMainInstanceRegion();
			SAtkInstance* atkInstanceInfo;// = (SAtkInstance*)mainInstanceRegion->getNewInstanceBase();

			if (NULL != mainInstanceRegion)
			{
				//广播给主城里的所以玩家
				//broadcastCampaignEnd(playerCharacterList, mainInstanceRegion->getNewRegionObject(), data->campaignId, (FORCE)data->winnerForce, data->fromRegionId, atkInstanceInfo);
			}

			std::map<InstanceUIdType, SInstanceRegion*>* instanceRegionList;// = iter->second->getInstanceRegionList();
			
			for (std::map<InstanceUIdType, SInstanceRegion*>::iterator iterInstance = instanceRegionList->begin(); 
				iterInstance != instanceRegionList->end(); 
				++iterInstance)
			{
				SInstanceRegion* instanceRegion = iterInstance->second;
				if (NULL != instanceRegion)
				{
					//broadcastCampaignEnd(playerCharacterList, instanceRegion->getNewRegionObject(), data->campaignId, (FORCE)data->winnerForce, data->fromRegionId, atkInstanceInfo);
				}
			}
		}

		//if (NULL != regionBase)
		//{
		//	regionBase->clearData();
		//}
#endif
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::broadcastCampaignEnd(std::map<PlayerCharacterIdType, SCharacter*>* playerCharacterList, SRegionObjectBase* regionObject, CampaignIdType campaignId, FORCE winnerForce, RegionIdType fromRegionId, SAtkInstance* atkInstanceInfo)
	{
#if 0
		NetTranspondSendDataBufferPtr dataPtr ;
		GameNetPacketData* packet       = NULL;
		PT_CAMPAIGN_M2C_END_DATA* sendData = NULL;

		// 当前发送数据头大小
		I32 sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_END_DATA_INFO::headSize;
		// 当前发送数据大小
		I32 sendDataSize        = sendDataHeadSize;
		// 准备发送数据大小
		I32 prepareSendDataSize = 0;

		// 遍历角色队列
		std::map<PlayerCharacterIdType, SCharacter*>::iterator iter = playerCharacterList->begin();
		while( playerCharacterList->end() != iter )
		{
			//---------------------------------------------------------------------------

			if ( dataPtr.mDataPtr.isNull() == false )
			{
				// 递增一个角色数据大小到发送数据缓存大小
				prepareSendDataSize = sendDataSize + PT_CAMPAIGN_M2C_END_DATA_INFO::netAddBattleScoreInfoSize;

				// 如果大于缓存最大大小则发送
				if ( prepareSendDataSize >= dataPtr.getTranspondDataMaxSize(100) )
				{
					NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, 
						GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_END_DATA_INFO::dataSize(sendData), regionObject);

					dataPtr.mDataPtr.setNull();
				}
			}

			//---------------------------------------------------------------------------

			if ( dataPtr.mDataPtr.isNull() )
			{
				NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );
				packet       = (GameNetPacketData*) dataPtr.getTranspondData();
				if (NULL == packet)
				{
					DYNAMIC_ASSERT(0);
					return ;
				}

				packet->channel = GNPC_CAMPAIGN;
				packet->type    = PT_CAMPAIGN_M2C_END;
				sendData = (PT_CAMPAIGN_M2C_END_DATA*)(packet->data);

				sendData->addBattleScoreNum = 0;
				sendData->campaignId		= campaignId;
				sendData->winnerForce		= winnerForce;
				sendData->winnerItemGroupId	= atkInstanceInfo->getAtkListCsvInfo()->itemGroupId1;
				sendData->loserItemGroupId	= atkInstanceInfo->getAtkListCsvInfo()->itemGroupId2;

				sendData->forceInfo[0].force = FORCE_ATTACK;
				sendData->forceInfo[0].regionId = fromRegionId;
				sendData->forceInfo[0].troopNum = regionObject->getAllTroopNumInCampByForce(FORCE_ATTACK);
				sendData->forceInfo[1].force = FORCE_DEFENSE;
				sendData->forceInfo[1].regionId = fromRegionId;
				sendData->forceInfo[1].troopNum = regionObject->getAllTroopNumInCampByForce(FORCE_DEFENSE);


				// 当前发送数据头大小
				sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_END_DATA_INFO::headSize;
				// 当前发送数据大小
				sendDataSize        = sendDataHeadSize;
				// 准备发送数据大小
				prepareSendDataSize = sendDataHeadSize;
			}

			//---------------------------------------------------------------------------

			SPlayerCharacter* tempPlayerCharacter = (SPlayerCharacter*)iter->second;

			NetAddCampaignEnd* tempSend 	= &(sendData->addBattleScoreInfo[sendData->addBattleScoreNum]);

			//填充 Begin
			BattleScoresRankListCsvInfo* rankListInfo = BattleScoreSystem::getInstance().getBattleScoreRankListByBattleStatics(tempPlayerCharacter->getBattleStatics());
			wcscpy_s(tempSend->playerCharacterName, MAX_CHARACTER_NAME_LEN, tempPlayerCharacter->getName().c_str());
			tempSend->force					= tempPlayerCharacter->getForce();
			tempSend->killGeneralPoint		= tempPlayerCharacter->getBattleStatics()->getKillGeneralPoint();
			tempSend->killArmyPoint			= tempPlayerCharacter->getBattleStatics()->getKillArmyPoint();
			tempSend->destoryForcePoint		= tempPlayerCharacter->getBattleStatics()->getDestoryForcePoint();
			tempSend->constructBuild		= tempPlayerCharacter->getBattleStatics()->getConstructBuildPoint();
			tempSend->healGeneralPoint		= tempPlayerCharacter->getBattleStatics()->getHealGeneralPoint();
			tempSend->deathPoint			= tempPlayerCharacter->getBattleStatics()->getDeathPoint();
			tempSend->battleAchievement		= rankListInfo->rewardLevel;
			tempSend->RankList				= rankListInfo->classLevel;

			//测试用
			//static Int testIndex = 1;
			//tempSend->killGeneralPoint		= testIndex++;
			//tempSend->killArmyPoint			= testIndex++;
			//tempSend->destoryForcePoint		= testIndex++;
			//tempSend->constructBuild		= testIndex++;
			//tempSend->healGeneralPoint		= testIndex++;
			//tempSend->deathPoint			= testIndex++;
			//tempSend->battleAchievement		= testIndex++;
			//tempSend->RankList				= testIndex++;
			//填充 end

			sendDataSize += PT_CAMPAIGN_M2C_END_DATA_INFO::netAddBattleScoreInfoSize;

			++sendData->addBattleScoreNum;
			++iter;
		}

		//--------------------------------------------------------------------------
		if ( dataPtr.mDataPtr.isNull() == false )
		{
			NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, 
				GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_END_DATA_INFO::dataSize(sendData), regionObject);
			dataPtr.mDataPtr.setNull();
		}
#endif
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignCreateCampByControler(I32 id, PT_CAMPAIGN_F2M_CREATE_CAMP_BY_CONTROLER_DATA* data)
	{
#if 0
		SInstanceRegion* realRegion = SWorldManager::getInstance().getInstanceRegion(ROT_PLACE, data->targetRegionId, data->targetInstanceUId);

		DYNAMIC_ASSERT(NULL != realRegion);

		SAtkInstance* atkInstanceInfo;// = (SAtkInstance*)realRegion->getNewInstanceBase();

		SCampListCsv*  campListInfo = atkInstanceInfo->getCampListByForce((FORCE)data->campForce);

		for (Int index=0; index<data->campNum; ++index)
		{
			InstanceAtkStrengthListCsvInfo* strengthInfo = campListInfo->getAtkStrengthInfoByIndex(index);
			//realRegion->createCamp(data->campaignId, data->targetControlerAccountId, (FORCE)data->campForce, data->campIndexStart + index, strengthInfo);
		}
#endif
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::broadcastChangeCampControlToRegion(SCamp* myCamp, SRegionObjectBase* region)
	{
		NetTranspondSendDataBufferPtr dataPtr;
        NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
		if ( NULL == packet )
		{
			DYNAMIC_ASSERT(0);
			return;
		}

		packet->channel = GNPC_CAMPAIGN;
		packet->type    = PT_CAMPAIGN_M2C_CHANGE_CAMP_CONTROL;
		PT_CAMPAIGN_M2C_CHANGE_CAMP_CONTROL_DATA* sendData = (PT_CAMPAIGN_M2C_CHANGE_CAMP_CONTROL_DATA*)(packet->data);
		if(sendData)
		{
			sendData->campId					= myCamp->getCampId();
			sendData->targetControlerAccountId	= myCamp->getControlTroopGroupAccountId();

			//开始填充 End
			NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, 
				GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_CHANGE_CAMP_CONTROL_INFO::dataSize, region);
		}
		
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::broadcastChangeTroopControlToRegion(SCamp* myCamp, SRegionObjectBase* region)
	{
		std::map<AccountIdType, Player*>* playerList;// = region->getPlayerList();

		for (std::map<AccountIdType, Player*>::iterator iter = playerList->begin(); iter != playerList->end(); ++iter)
		{
			sendChangeTroopControlToPlayer((SPlayer*)iter->second, myCamp);
		}
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendChangeTroopControlToPlayer( SPlayer* player, SCamp* myCamp)
	{
#if 0
		NetTranspondSendDataBufferPtr dataPtr;
		GameNetPacketData* packet       = NULL;
		PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL_DATA* sendData = NULL;

		// 当前发送数据头大小
		I32 sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL_DATA_INFO::headSize;;
		// 当前发送数据大小
		I32 sendDataSize        = 0;
		// 准备发送数据大小
		I32 prepareSendDataSize = 0;

		//-------------------------------------------------------------------------- 
		std::map<TroopGroupIdType, STroopGroup*>* troopGroupList;// = myCamp->getCurControlTroopGroupList();
		std::map<TroopGroupIdType, STroopGroup*>::iterator iterGroup = troopGroupList->begin();
		for (; iterGroup != troopGroupList->end(); ++iterGroup)
		{
			//STroopGroup* myTroopGroup = iterGroup->second;
			std::map<TroopIdType, STroop*>* troopList;// = myTroopGroup->getTroopList();
			std::map<TroopIdType, STroop*>::iterator iterTroop = troopList->begin();
			for (; iterTroop != troopList->end(); ++iterTroop)
			{

                //--------------------------------------------------------------------------

				STroop* troopInfo;// = (STroop*)iterTroop->second;
				if (true == troopInfo->isDead())
				{
					continue;
				}

                //--------------------------------------------------------------------------

                if ( dataPtr.mDataPtr.isNull() == false )
                {
                    // 递增一个角色数据大小到发送数据缓存大小
                    prepareSendDataSize = sendDataSize + PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL_DATA_INFO::netAddMemberInfoSize;

                    // 如果大于缓存最大大小则发送
                    if ( prepareSendDataSize >= dataPtr.getTranspondDataMaxSize(100) )
                    {
                        NetTranspondPacketProcesser::getInstance().sendClientTranspondSendDataToFrontServer(
                            dataPtr, 
                            sendDataSize, 
                            player->getFrontServerId(), 
                            player->getClientNetIdInFrontServer(), player->getAccountId() );
                        dataPtr.mDataPtr.setNull();
                    }
                }

                //--------------------------------------------------------------------------

                if ( dataPtr.mDataPtr.isNull() )
                {
                    NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );
                    packet  = (GameNetPacketData*) dataPtr.getTranspondData();
                    if ( !packet )
                        return;

                    packet->channel = GNPC_CAMPAIGN;
                    packet->type    = PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL;

                    sendData = (PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL_DATA*)(packet->data);
                    sendData->addNum					= 0;
                    sendData->campId					= myCamp->getID();
                    sendData->targetControlerAccountId	= myCamp->getControlTroopGroupAccountId();

                    sendDataHeadSize    = GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL_DATA_INFO::headSize;
                    sendDataSize        = sendDataHeadSize;
                    prepareSendDataSize = sendDataHeadSize;
                }

                //--------------------------------------------------------------------------
				
				//开始填充
				NetTroopChangeControlInfo* tempSend = &(sendData->addMemberInfo[sendData->addNum]);
				tempSend->troopId = troopInfo->getID();

				sendDataSize += PT_CAMPAIGN_M2C_CHANGE_TROOP_CONTROL_DATA_INFO::netAddMemberInfoSize;
				++sendData->addNum;
			}
		}
		
		if ( dataPtr.mDataPtr.isNull() == false )
		{
			NetTranspondPacketProcesser::getInstance().sendClientTranspondSendDataToFrontServer(
                dataPtr, 
                sendDataSize, 
                player->getFrontServerId(), 
                player->getClientNetIdInFrontServer(), 
                player->getAccountId());
            dataPtr.mDataPtr.setNull();
		}
#endif
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendTroopControllerListToPlayer(PT_CAMPAIGN_F2M_TROOP_CONTROLLER_LIST_DATA* data, SPlayer* myPlayer)
	{
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendCampaignStartNewRegionToCenterServer(CampaignIdType campaignId, SInstanceRegion *newRegion)
	{
		//【广播给区域的人】
		GameNetSendDataBufferPtr dataPtr ;
        MapServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr->getLogicData();
		if(packet)
		{
			packet->channel = GNPC_CAMPAIGN;
			packet->type	= PT_CAMPAIGN_M2T_START_NEW_INSTANCE;

			PT_CAMPAIGN_M2T_START_NEW_INSTANCE_DATA* sendData = (PT_CAMPAIGN_M2T_START_NEW_INSTANCE_DATA*)(packet->data);
			sendData->campaignId	= campaignId;
			//sendData->targetRegionId= newRegion->getNewRegionObject()->getRegionId();
			//sendData->instanceUid	= newRegion->getInstanceUId();

			//广播给所有的FrontServer， FrontServer也要保存一份内存
			ServerManager::getInstance().sendNetPacketToCenterServer(dataPtr, 
				GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2T_START_NEW_INSTANCE_INFO::dataSize);	
		}
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendMainCampDestroyToCenterServer(const CampaignIdType campaignId, const FORCE myCampForce)
	{
		//【广播给区域的人】
		GameNetSendDataBufferPtr dataPtr ;
        MapServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr->getLogicData();
		if(NULL == packet)
		{
			return;
		}

		packet->channel = GNPC_CAMPAIGN;
		packet->type	= PT_CAMPAIGN_M2T_MAIN_CAMP_DESTROY;

		PT_CAMPAIGN_M2T_MAIN_CAMP_DESTROY_DATA* sendData = (PT_CAMPAIGN_M2T_MAIN_CAMP_DESTROY_DATA*)(packet->data);
		sendData->campaignId	= campaignId;
		sendData->campForce		= myCampForce;

		ServerManager::getInstance().sendNetPacketToCenterServer(dataPtr, 
			GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2T_MAIN_CAMP_DESTROY_INFO::dataSize);	
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignStartFromFrontServer(I32 id, PT_CAMPAIGN_F2M_START_DATA* data)
	{
		U64	accountId		= data->accountId;
		//U64	campaignId		= data->campaignId;
		//U64	starterClanId	= data->starterClanId;

		SPlayerPtr player;
		if (false == SPlayerManager::getInstance().getPlayer(player, data->accountId))
		{
			return;
		}

		DYNAMIC_ASSERT(false == player.isNull());
		if ( player.isNull() )
		{
			return;
		}

		std::map<RegionIdType, SBigRegionObject*>* regionObjInstanceList = NULL;	//SWorldManager::getInstance().getBigPlaceRegionList();
		DYNAMIC_ASSERT(false == regionObjInstanceList->empty());

		SInstanceRegion *newRegion;// = SWorldManager::getInstance().createInstanceRegion(ROT_PLACE, data->targetRegionId, data->accountId, JUMP_ATTACK);
		//CampaignManager::getInstance().startCampaign(campaignId, starterClanId);

		sendCampaignStartNewRegionToCenterServer(data->campaignId, newRegion);
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onRecvCampaignInviteDefenseFromCenterServer(I32 id, PT_CAMPAIGN_T2M_INVITE_DEFENSE_REGION_DATA* data)
	{
		RegionIdType		toRegionId		= data->toRegionId;
		InstanceIdType		toInstanceId	= data->toInstanceId;
		SBigRegionObject* regionObj;// = SWorldManager::getInstance().getBigRegion(ROT_PLACE, toRegionId);
		if (NULL == regionObj)
		{
			return;
		}
		DYNAMIC_ASSERT(NULL != regionObj);

		SInstanceRegion* mainRegionObj;// = regionObj->getMainInstanceRegion();
		DYNAMIC_ASSERT(NULL != mainRegionObj);

		SRegionObjectBase* mainRegion;// = mainRegionObj->getNewRegionObject();
		DYNAMIC_ASSERT(NULL != mainRegion);


		//【广播给区域的人】
		NetTranspondSendDataBufferPtr dataPtr;
        NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE);
		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
		if(packet)
		{
			packet->channel = GNPC_CAMPAIGN;
			packet->type	= PT_CAMPAIGN_M2C_INVITE_DEFENSE_REGION;

			PT_CAMPAIGN_M2C_INVITE_DEFENSE_REGION_DATA* sendData = (PT_CAMPAIGN_M2C_INVITE_DEFENSE_REGION_DATA*)(packet->data);
			sendData->toRegionId	= data->toRegionId;
			sendData->toInstanceId	= data->toInstanceId;
			sendData->campaignId	= data->campaignId;


			NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr,
				GameNetPacketData_INFO::headSize + PT_CAMPAIGN_M2C_INVITE_DEFENSE_REGION_INFO::dataSize,
										mainRegion);
		}
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::playerCharacterAndTroopJumpRegionBackWithPlayer(std::map<ClanIdType, ClanArmy*>* armyList)
	{
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendCampaignError(U64	accountId, CAMPAIGN_ERROR error)
	{
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendToPlayerInMapObject(NetTranspondSendDataBufferPtr& dataPtr, Int dataSize, SPlayer* player)
	{
		if (NULL == player)
		{
			return;
		}

		NetTranspondPacketProcesser::getInstance().sendClientTranspondSendDataToFrontServer(dataPtr, dataSize, 
			player->getFrontServerId(), player->getClientNetIdInFrontServer(), player->getAccountId());
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::sendToPlayerInMapObject(NetTranspondSendDataBufferPtr& dataPtr, Int dataSize, AccountIdType accountId)
	{
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::onCampaignLeave(const U64 accountId, const U64 regionId, const U64 campaignId, const U64 clanId)
	{
	}

	//--------------------------------------------------------------------------
	void SCampaignNetPacketProcesser::broadToMapObject(NetTranspondSendDataBufferPtr& dataPtr, Int dataSize, SMapObject* mapObject)
	{
	}

	//--------------------------------------------------------------------------
}