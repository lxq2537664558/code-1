/******************************************************************************/
#ifndef _REGION_NET_PACKET_PROCESSER_H_
#define _REGION_NET_PACKET_PROCESSER_H_
/******************************************************************************/

#include "RegionNetPacket.h"
#include "FrontServerPreqs.h"
#include "SPlayer.h"
/******************************************************************************/

namespace MG
{

	/******************************************************************************/
	//区域相关网络消息处理
	/******************************************************************************/
	/******************************************************************************/
	class RegionNetPacketProcesser
	{
	public:
		SINGLETON_INSTANCE(RegionNetPacketProcesser);
		RegionNetPacketProcesser();
		~RegionNetPacketProcesser();

		Bool	processClientPacket(I32 id, NetEventRecv* packet);
		Bool	processLoginServerPacket(I32 id, NetEventRecv* packet);
		Bool	processMapServerPacket(I32 id, NetEventRecv* packet);

	public:

		//************************************************************


	public:
		//@{ Client


		void	onRecvAddBattleBuildingByPlayer(I32 id , PT_REGION_C2F_ADD_BUILDING_DATA*data);
		void	onRecvRemoveBattleBuildingByPlayer(I32 id , PT_REGION_C2F_REMOVE_BUILDING_DATA*data);

		void	onRecvCreateTroopRet(I32 id , PT_REGION_C2F_CREATE_TROOP_RET_DATA*data);

		void	onRecvAddTroopTest(I32 id , PT_REGION_C2M_ADD_TROOP_TEST_DATA*data);

		void	onRecvPlayerCharacterSetAlive(I32 id , PT_REGION_C2F_PLAYER_CHARACTER_ALIVE_DATA*data);

		void	onRecvTest(I32 id , PT_REGION_C2M_TEST_DATA*data);

		void	onRecvTest1(I32 id , PT_REGION_C2M_TEST_DATA*data);

		//@ }


		//@{ MapServer
	public:
		void	onRecvPlayerCharacterAliveFromMapServer(I32 id , PT_REGION_M2F_PLAYER_CHARACTER_ALIVE_DATA*data);

		void	sendPlayerCharacterAliveToClient(SPlayerPtr player, SPlayerCharacter* playerCharacter);
		//void	onRecvAddTroopFromMapServer(I32 id , PT_REGION_M2F_ADD_TROOP_DATA*data);

		//void	onRecvAddMonsterFromMapServer(I32 id , PT_REGION_M2F_ADD_MONSTER_DATA*data);

	protected:


		//@ }
	};

}


#endif	//_REGION_CHARACTER_NET_PACKET_PROCESSER_H_