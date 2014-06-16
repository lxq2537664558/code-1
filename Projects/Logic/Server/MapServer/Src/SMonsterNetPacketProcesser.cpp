/******************************************************************************/

#include "stdafx.h"
#include "SMonsterNetPacketProcesser.h"
#include "NetTranspondPacketProcesser.h"
#include "MapServerMain.h"
#include "SWorldManager.h"
#include "MGameTimeSystem.h"
#include "SSceneGridManager.h"


/******************************************************************************/

namespace MG
{
	//--------------------------------------------------------------------------
	SMonsterNetPacketProcesser::SMonsterNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	SMonsterNetPacketProcesser::~SMonsterNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	Bool SMonsterNetPacketProcesser::processFrontServerPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_MONSTER )
		{
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{

			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	Bool SMonsterNetPacketProcesser::processMapServerPacket(I32 id, NetEventRecv* packet)
	{

		return false;
	}

	//--------------------------------------------------------------------------
	void SMonsterNetPacketProcesser::broadcastMonsterMove(SMonster* monster, NetVec2 startMovePos, NetVec2 endMovePos)
	{
#if 0
		if(monster)
		{
			NetTranspondSendDataBufferPtr dataPtr ;
            NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

			GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
			if(packet)
			{
				packet->type = PT_MONSTER_S2C_MOVE_POS;
				packet->channel = GNPC_MONSTER;

				PT_MONSTER_S2C_MOVE_POS_DATA* sendData = (PT_MONSTER_S2C_MOVE_POS_DATA*)packet->data;
				if(sendData)
				{
					sendData->monsterId = monster->getID();
					sendData->startMovePos  = startMovePos.getData();
					sendData->endMovePos  = endMovePos.getData();
					sendData->startMoveTime = MGameTimeSystem::getInstance().getCurrActualTime();

					SRegionObjectBase* regionObjectBase = monster->getCurrentRegionObject();
					if(!regionObjectBase)
					{
						return;
					}

					NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, GameNetPacketData_INFO::headSize + PT_MONSTER_S2C_MOVE_POS_DATA_INFO::dataSize, 
						monster->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, regionObjectBase, 0);
				}
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void SMonsterNetPacketProcesser::broadcastInstantMonsterMove(SMonster* monster, NetVec2 moveToPos)
	{
#if 0
		if(monster)
		{
			NetTranspondSendDataBufferPtr dataPtr ;
            NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

			GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
			if(packet)
			{
				packet->type = PT_MONSTER_S2C_INSTANT_MOVE_POS;
				packet->channel = GNPC_MONSTER;

				PT_MONSTER_S2C_INSTANT_MOVE_POS_DATA* sendData = (PT_MONSTER_S2C_INSTANT_MOVE_POS_DATA*)packet->data;
				if(sendData)
				{
					sendData->monsterId = monster->getID();
					sendData->moveToPos  = moveToPos.getData();

					SRegionObjectBase* regionObjectBase = monster->getCurrentRegionObject();
					if(regionObjectBase)
					{
						NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, GameNetPacketData_INFO::headSize + PT_MONSTER_S2C_INSTANT_MOVE_POS_DATA_INFO::dataSize, 
							monster->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, regionObjectBase, 0);
					}

				}
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void  SMonsterNetPacketProcesser::broadcastMonsterStopMove(SMonster* monster)
	{
#if 0
		if(monster)
		{
			NetTranspondSendDataBufferPtr dataPtr ;
            NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );
			GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
			if(packet)
			{
				packet->type = PT_MONSTER_S2C_STOP_MOVE;
				packet->channel = GNPC_MONSTER;

				PT_MONSTER_S2C_STOP_MOVE_DATA* sendData = (PT_MONSTER_S2C_STOP_MOVE_DATA*)packet->data;
				if(sendData)
				{
					sendData->monsterId = monster->getID();
					Vec3 pos = monster->getPos();
					sendData->pos = ((NetVec2)pos).getData();

					SRegionObjectBase* regionObjectBase = monster->getCurrentRegionObject();
					if(regionObjectBase)
					{
						NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, GameNetPacketData_INFO::headSize + PT_MONSTER_S2C_STOP_MOVE_DATA_INFO::dataSize, 
							monster->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, regionObjectBase, 0);
					}
				}
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void  SMonsterNetPacketProcesser::broadcastMonsterEnterFight(SMonster* monster)
	{
#if 0
		if(monster)
		{
			NetTranspondSendDataBufferPtr dataPtr ;
            NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

			GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
			if(packet)
			{
				packet->type = PT_MONSTER_S2C_ENTER_FIGHT;
				packet->channel = GNPC_MONSTER;

				PT_MONSTER_S2C_ENTER_FIGHT_DATA* sendData = (PT_MONSTER_S2C_ENTER_FIGHT_DATA*)packet->data;
				if(sendData)
				{
					sendData->monsterId = monster->getID();
					sendData->moveSpeed = monster->getMoveSpeed();

					SRegionObjectBase* regionObjectBase = monster->getCurrentRegionObject();
					if(regionObjectBase)
					{
						NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, GameNetPacketData_INFO::headSize + PT_MONSTER_S2C_ENTER_FIGHT_DATA_INFO::dataSize, 
							monster->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, regionObjectBase, 0);
					}
				}
			}
		}
#endif
	}

	//--------------------------------------------------------------------------
	void  SMonsterNetPacketProcesser::broadcastMonsterEndFight(SMonster* monster)
	{
#if 0
		if(monster)
		{
			NetTranspondSendDataBufferPtr dataPtr;
            NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer( dataPtr, MG_GAME_NET_PACKET_BIG_MAX_SIZE );

			GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
			if(packet)
			{
				packet->type = PT_MONSTER_S2C_END_FIGHT;
				packet->channel = GNPC_MONSTER;

				PT_MONSTER_S2C_END_FIGHT_DATA* sendData = (PT_MONSTER_S2C_END_FIGHT_DATA*)packet->data;
				if(sendData)
				{
					sendData->monsterId = monster->getID();
					sendData->moveSpeed = monster->getMoveSpeed();

					SRegionObjectBase* regionObjectBase = monster->getCurrentRegionObject();
					if(regionObjectBase)
					{
						NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, GameNetPacketData_INFO::headSize + PT_MONSTER_S2C_END_FIGHT_DATA_INFO::dataSize, 
							monster->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, regionObjectBase, 0);
					}
				}
			}
		}
#endif
	}


	//--------------------------------------------------------------------------
	void SMonsterNetPacketProcesser::sendMonsterBeKilledByPlayerCharacter( I32 id, MonsterIdType monsterID, PlayerCharacterIdType playerCharacterId,PlayerIdType playerID, NetIdType clientIdInFrontServer )
	{
#if 0
         GameNetSendDataBufferPtr dataPtr;
         MapServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

         GameNetPacketData* pack         = (GameNetPacketData*)dataPtr->getLogicData();
		 if (pack)
		 {
			pack->type = PT_MONSTER_MTF_BEKILLED;
			pack->channel = GNPC_MONSTER;
			PT_MONSTER_M2F_BEKILLED* sendData = (PT_MONSTER_M2F_BEKILLED*)pack->data;
			sendData->monsterID = monsterID;
			sendData->playerCharacterId = playerCharacterId;
			sendData->playerID = playerID;
			sendData->clientIdInFrontServer = clientIdInFrontServer;

			MapServerMain::getInstance().getServerLauncher()->sendServer(dataPtr, 
				GameNetPacketData_INFO::headSize + PT_MONSTER_M2F_BEKILLED_DATA_INFO::dataSize,id);
			//dataPtr->send(GameNetPacketData_INFO::headSize + PT_MONSTER_M2F_BEKILLED_DATA_INFO::dataSize,id);
            
		 }
#endif
	}



	//--------------------------------------------------------------------------

}	//namespace MG