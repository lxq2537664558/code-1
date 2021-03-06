//***************************************************************************************
#include "stdafx.h"
#include "SBuffPacketProcesser.h"
#include "SWorldManager.h"
#include "SPlayerCharacter.h"
#include "SBuffManager.h"
#include "MapServerMain.h"
#include "NetTranspondNetPacket.h"
#include "SCharacter.h"
#include "BuffData.h"
#include "SSceneGridManager.h"
//***************************************************************************************
namespace MG
{
	//--------------------------------------------------------------------------
	MG::Bool SBuffPacketProcesser::processFrontServerPacket( I32 id, NetEventRecv* packet )
	{
		if ( packet->getChannel() == GNPC_BUFF )
		{
			GameNetPacketData* data = (GameNetPacketData*)(packet->getData());

			switch (data->type)
			{
			case PT_BUFF_F2M_PLAYERCHARACTER_BUFFDATA_LIST:
				recvBuffDataListFromFrontServer(id, (PT_BUFF_F2M_PLAYERCHARACTER_BUFFDATA_LIST_Data*)(data->data));
				break;
			}

			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------
	void SBuffPacketProcesser::sendBuffDataToFrontServer(I32 frontId, IdType accountId, PlayerCharacterIdType charId, BuffData* pBuffData )
	{
		GameNetSendDataBufferPtr dataPtr  ;
        MapServerMain::getInstance().getServerLauncher()->getServerSendDataBuffer( dataPtr );

		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr->getLogicData();
		if(packet)
		{
			packet->type = PT_BUFF_M2F_PLAYERCHARACTER_BUFFDATA;
			packet->channel = GNPC_BUFF;

			PT_BUFF_M2F_PLAYERCHARACTER_BUFFDATA_Data* sendData = (PT_BUFF_M2F_PLAYERCHARACTER_BUFFDATA_Data*)packet->data;
			if(sendData)
			{
				sendData->acountId = accountId;
				sendData->dbId = charId;
				sendData->buffdatalist.buffId = pBuffData->mBuffId;
				sendData->buffdatalist.curAddNum = pBuffData->mCurAddNum;
				sendData->buffdatalist.timeUse = pBuffData->mTimeUse;

				MapServerMain::getInstance().getServerLauncher()->sendServer(dataPtr,
					GameNetPacketData_INFO::headSize + PT_BUFF_M2F_PLAYERCHARACTER_BUFFDATA_INFO_Data::dataSize, frontId);
			}
		}
	}

	//--------------------------------------------------------------------------
	void SBuffPacketProcesser::syncPlayerCharacterBuffDataListToClient(SPlayerCharacter* pSPlayerCharacter, Bool isFilterSelf)
	{
		std::map<U32, BuffData> mapBuffData;
		pSPlayerCharacter->getSBuffManager()->getBuffData(mapBuffData);
		SRegionObjectBase* tempRegion;// = pSPlayerCharacter->getCurrentRegionObject();

		DYNAMIC_ASSERT ( tempRegion );

		//--------------------------------------------------------------------------

		// 获得一个发送【服务端】缓存
		NetTranspondSendDataBufferPtr dataPtr ;
		GameNetPacketData* packet = NULL;
		PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_LIST_Data* sendData = NULL;

		PlayerIdType playeridtype = 0;
		if ( isFilterSelf )
		{
			playeridtype = pSPlayerCharacter->getAccountId();
		}

		//封包头的大小，CDNetData的大小
		UInt headSize = GameNetPacketData_INFO::headSize + PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_LIST_INFO_Data::dataSize - netBuffInfoData::dataSize;
		UInt elemSize = netBuffInfoData::dataSize;
		//封包大小
		UInt bagSize = headSize;

		std::map<U32, BuffData>::iterator  front = mapBuffData.begin();
		std::map<U32, BuffData>::iterator  back = mapBuffData.end();
		for ( ;front != back; front++ )
		{
			//--------------------------------------------------------------------------

			if ( front->second.mTimeUse == false )
				continue;
			
			//--------------------------------------------------------------------------

			if ( dataPtr.mDataPtr.isNull() == false )
			{
				//检查是否需要发送
				if ( bagSize + elemSize > dataPtr.getTranspondDataMaxSize(100) )
				{
					// 发送数据
	//				NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, bagSize, 
	//					pSPlayerCharacter->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, tempRegion, playeridtype);
					dataPtr.mDataPtr.setNull();
				}
			}

			//--------------------------------------------------------------------------

			if ( dataPtr.mDataPtr.isNull() )
			{
				// 获得一个发送【服务端】缓存
				NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer(dataPtr,MG_GAME_NET_PACKET_BIG_MAX_SIZE);
				packet       = (GameNetPacketData*) dataPtr.getTranspondData();
				if ( !packet )
					return;

				packet->type = PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_LIST;
				packet->channel = GNPC_BUFF;

				sendData = (PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_LIST_Data*)packet->data;
				if(!sendData)
					return;

				//封包头
				sendData->dbId = pSPlayerCharacter->getID();
				sendData->ncount = 0;

				//封包头的大小，CDNetData的大小
				headSize = GameNetPacketData_INFO::headSize + PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_LIST_INFO_Data::dataSize - netBuffInfoData::dataSize;
				elemSize = netBuffInfoData::dataSize;

				//封包大小
				bagSize = headSize;
			}

			//--------------------------------------------------------------------------

			//填充
			sendData->buffdatalist[sendData->ncount].buffId =  front->second.mBuffId;
			sendData->buffdatalist[sendData->ncount].timeUse =  front->second.mTimeUse;
			sendData->buffdatalist[sendData->ncount].curAddNum =  front->second.mCurAddNum;

			//更新记录数据
			bagSize += elemSize;
			++sendData->ncount;
		}

		//--------------------------------------------------------------------------
		if ( dataPtr.mDataPtr.isNull() == false )
		{
			// 发送数据
	//		NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, bagSize, 
	//				pSPlayerCharacter->getSceneObjectInSceneGridsManager()->currSceneGrid, NULL, tempRegion, playeridtype);
			dataPtr.mDataPtr.setNull();
		}
	}

	//--------------------------------------------------------------------------
	void SBuffPacketProcesser::recvBuffDataListFromFrontServer( I32 id, PT_BUFF_F2M_PLAYERCHARACTER_BUFFDATA_LIST_Data* pData )
	{
		SPlayerCharacter* pSPlayerCharacter = NULL;//SWorldManager::getInstance().getPlayerCharacterFromMemory(pData->dbId);
		DYNAMIC_ASSERT(pSPlayerCharacter);
		SBuffManager* pSBuffManager = pSPlayerCharacter->getSBuffManager();
		DYNAMIC_ASSERT(pSBuffManager);

		//先清空所有的buff。
		pSBuffManager->clear();
		//在重新添加。
		netBuffData* pNetBuffData = NULL;
		for (Int i= 0; i<pData->ncount; i++)
		{
			pNetBuffData = &(pData->buffdatalist[i]);
			pSBuffManager->addOldBuff(pNetBuffData->buffId, pNetBuffData->timeUse, pNetBuffData->curAddNum);
		}

	}

	//--------------------------------------------------------------------------
	void SBuffPacketProcesser::syncPlayerCharacterBuffDataToClient( SPlayerCharacter* pSPlayerCharacter, UInt buffId, UInt addNum, UInt useTime )
	{
		std::map<U32, BuffData> mapBuffData;
		pSPlayerCharacter->getSBuffManager()->getBuffData(mapBuffData);
		SRegionObjectBase* tempRegion;// = pSPlayerCharacter->getCurrentRegionObject();

		DYNAMIC_ASSERT ( tempRegion );

		// 获得一个发送【服务端】缓存
		NetTranspondSendDataBufferPtr dataPtr;
        NetTranspondPacketProcesser::getInstance().getClientTranspondSendDataBuffer(dataPtr,MG_GAME_NET_PACKET_BIG_MAX_SIZE);

		GameNetPacketData* packet       = (GameNetPacketData*) dataPtr.getTranspondData();
		if ( packet )
		{
			packet->type = PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA;
			packet->channel = GNPC_BUFF;

			PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_Data* sendData = (PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_Data*)packet->data;
			if(sendData)
			{
				//封包头
				sendData->dbId = pSPlayerCharacter->getID();
				sendData->acountId = pSPlayerCharacter->getAccountId();
				sendData->buffdatalist.buffId = buffId;
				sendData->buffdatalist.curAddNum = addNum;
				sendData->buffdatalist.timeUse = useTime;

				// 发送数据
	//			NetTranspondPacketProcesser::getInstance().broadcastClientTranspondPackToFrontServer(dataPtr, 
	//				GameNetPacketData_INFO::headSize + PT_BUFF_M2C_PLAYERCHARACTER_BUFFDATA_INFO_Data::dataSize, 
	//				pSPlayerCharacter->getSceneObjectInSceneGridsManager()->currSceneGrid,
	//				NULL, tempRegion, 0
	//				);
			}
		}
	}
}