/******************************************************************************/
#ifndef _QUESTNETPACKETPROCESSER_H_
#define _QUESTNETPACKETPROCESSER_H_
/******************************************************************************/
#include "QuestNetPacket.h"
#include "ClanQuestObject.h"
#include "QuestList.h"
/******************************************************************************/

namespace MG
{
	class SPlayerPtr;
    /******************************************************************************/
    //任务相关网络消息处理
    /******************************************************************************/
    class QuestNetPacketProcesser
    {
    public:
        SINGLETON_INSTANCE(QuestNetPacketProcesser);
        QuestNetPacketProcesser();
        ~QuestNetPacketProcesser();

        Bool	processClientPacket(I32 id, NetEventRecv* packet);
        Bool	processLoginServerPacket(I32 id, NetEventRecv* packet);
        Bool	processMapServerPacket(I32 id, NetEventRecv* packet);

    public:

        //一些具体处理包方法
		void    onRecvQuestAcceptFromClient(I32 id , PT_QUEST_C2F_ACCEPT_DATA* data);
		void    onRecvQuestSubmitFromClient(I32 id , PT_QUEST_C2F_SUBMIT_DATA* data);
		void    onRecvQuestGiveupFromClient(I32 id , PT_QUEST_C2F_GIVEUP_DATA* data);
		void    onRecvTalkToNpc( I32 id , PT_QUEST_C2F_TALK_TO_NPC_DATA* data );


        //一些具体发送包方法
        void    sendAllQuestInfoToClient( I32 id, SPlayerPtr& playerPtr );

		void	sendQuestAcceptResult(I32 id, U32 questInfoId, PT_QUEST_F2C_ACCEPT_RESULT result);
		void	sendQuestSubmitResult(I32 id, U32 questInfoId, byte result);
		void	sendQuestGiveupResult(I32 id, U32 questInfoId, Bool result);
		void	sendQuestCompletedState(I32 id, U32 questInfoId,Byte completeState);
		void    sendDoingQuestInfoToClient(I32 id,std::map< U32 , ClanQuestObject*>* doingQuestInfo);
		void    sendFinishedQuestInfoToClient(I32 id,std::map<U32,FinishedClanQuestObject*>* finishedQuestInfo);
		void    sendQuestConditionVariable(I32 id,U32 questInfoId,Byte varPos,Int varValue);
		
    };

}


#endif