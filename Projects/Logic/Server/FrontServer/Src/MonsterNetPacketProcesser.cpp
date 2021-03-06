/******************************************************************************/
#include "stdafx.h"
#include "MonsterNetPacketProcesser.h"
#include "FrontServerMain.h"
#include "MonsterNetPacket.h"
#include "SWorldManager.h"
#include "ServerManager.h"
#include "SkillObject.h"
#include "SWorldManager.h"
#include "SSkillOwnManager.h"
#include "SClanQuestManager.h"
/******************************************************************************/




namespace MG
{
	//--------------------------------------------------------------------------
	MonsterNetPacketProcesser::MonsterNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	MonsterNetPacketProcesser::~MonsterNetPacketProcesser()
	{

	}

	//--------------------------------------------------------------------------
	Bool MonsterNetPacketProcesser::processClientPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_MONSTER)
		{   
			//FUNDETECTION(__MG_FUNC__);
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	Bool MonsterNetPacketProcesser::processLoginServerPacket(I32 id, NetEventRecv* packet)
	{

		return false;
	}

	//--------------------------------------------------------------------------
	Bool MonsterNetPacketProcesser::processMapServerPacket(I32 id, NetEventRecv* packet)
	{
		if ( packet->getChannel() == GNPC_MONSTER )
		{   
			//FUNDETECTION(__MG_FUNC__);
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());
			switch (data->type)
			{
			case PT_MONSTER_MTF_BEKILLED:
				onRecvMonsterBeKilledByPlayerCharacterFromMapServer(id,(PT_MONSTER_M2F_BEKILLED*)data->data);
                break;
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	//void MonsterNetPacketProcesser::sendMonsterMoveToMapServer(I32 id, PT_MONSTER_C2F_MOVE_POS_DATA* data )
	//{
	//	// 获得一个发送【服务端】缓存
	//	GameNetSendDataBufferPtr dataPtr;
 //       FrontServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

	//	// 转换成标准包格式
	//	GameNetPacketData* packet = (GameNetPacketData*) dataPtr->getLogicData();
	//	if ( packet )
	//	{
	//		// 设置标签
	//		packet->channel = GNPC_MONSTER;
	//		packet->type    = PT_MONSTER_C2S_MOVE_POS;

	//		// 复制
	//		memcpy( packet->data, (Char8*)data, PT_MONSTER_C2S_MOVE_POS_DATA_INFO::dataSize );

	//		// 广播到地图服务器
	//		ServerManager::getInstance().boardcastNetPacketToAcitveMapServer( dataPtr,
	//			GameNetPacketData_INFO::headSize + PT_MONSTER_C2S_MOVE_POS_DATA_INFO::dataSize);
	//	}
	//}

    //--------------------------------------------------------------------------
	void MonsterNetPacketProcesser::onRecvMonsterBeKilledByPlayerCharacterFromMapServer( I32 id,PT_MONSTER_M2F_BEKILLED* data )
	{
        SPlayerPtr playerPtr;// = SWorldManager::getInstance().getPlayerByAccountId(data->playerID);
		if (playerPtr.isNull())return;
		SClanQuestManager* questManager;// = playerPtr->getQuestManager();
		DYNAMIC_ASSERT(questManager);
		questManager->notifyKillMonster(data->monsterIndex);

	}
	//--------------------------------------------------------------------------

}