/******************************************************************************/
#ifndef _GAMENETPACKET_H_
#define _GAMENETPACKET_H_
/******************************************************************************/

#include "NetCommonGameStruct.h"
#include "NetVec2.h"

/******************************************************************************/

//��������������ͨ�������    //1k   
#define MG_GAME_NET_PACKET_NORMAL_MAX_SIZE      MG_NET_DEFAULT_SEND_BUFFER_SIZE
//�����������ݴ��Ͱ������    //10k   
#define MG_GAME_NET_PACKET_BIG_MAX_SIZE         (MG_NET_DEFAULT_SEND_BUFFER_SIZE*10)

// Ĭ�Ϸ������ݻ����С
#define MG_NET_DEFAULT_SEND_BUFFER_SIZE	        1024
// Ĭ�Ͻ������ݻ���
#define MG_NET_DEFAULT_RECV_BUFFER_SIZE	        1024*20


/******************************************************************************/
namespace MG
{
	/******************************************************************************/
	//���������
	/******************************************************************************/
	enum GameNetType
	{
		GNT_UNKOWN = 0,
		GNT_CLIENT = 1,
		GNT_LOGIN_SERVER = 2,
		GNT_FRONT_SERVER = 3,
		GNT_CENTER_SERVER = 4,
		GNT_MAP_SERVER = 5,
        GNT_SYSLOG_SERVER = 6,
		GNT_CHAT_SERVER = 7,
        GNT_MALL_SERVER = 8,
		GNT_DB_SERVER = 9,
		GNT_GM_SERVER = 10,
		GNT_GM_SERVERMANAGER = 11,
	};

    /******************************************************************************/
    //��Ϸ��Ƶ��
    /******************************************************************************/
    enum GameNetPacketChannel
    {
		GNPC_UNKOWN,
        GNPC_TEST,                  //�������
        
        //
        GNPC_LOGIN,             	//�������
		GNPC_SERVER_STATE,			//������״̬
        GNPC_PLAYER,            	//������  
        
        GNPC_PLAYER_CHARACTER,  	//��ҽ�ɫ���
		GNPC_MONSTER,
		GNPC_TROOP,					//�������
        GNPC_ACTOR,             	//Actor���
        GNPC_NPC,
        GNPC_ITEM,
        GNPC_BUILDING,
        GNPC_ARMY,
        
        GNPC_DISTRICT,
        GNPC_WORLD,
        GNPC_MAP,
        GNPC_PLACE,
        GNPC_REGION,

		GNPC_CAMPAIGN,				//ս�����
		GNPC_BATTLE_SCORE,			//ս��ͳ��
		GNPC_PVE,					//PVE���
        
        GNPC_QUEST,     			//�������
        GNPC_CHAT,      			//�������
        GNPC_GUILD,   				//�������
        GNPC_TRADE,     			//�������
        GNPC_SHOP,                  //�̵����

		GNPC_SKILL,					//�뼼����� 
        GNPC_SYSLOG,                //����־ϵͳ���
        GNPC_ATTRIBUTE,             //������ϵͳ���
        GNPC_CD,                    //��CD��أ���ȴʱ�䣩
		GNPC_INSTANCE,				// �������
		GNPC_BUFF,					//��״̬��� 
		GNPC_GENERAL_RECRUIT,		//�佫��ļ

		GNPC_PACK,
        
        GNPC_NORMAL_MAX = 100,
        
        //
		GNPC_NET_TRANSPOND = 112,
		GNPC_TIME,					//ʱ��ϵͳ���
		GNPC_DB,					//���ݿ����
        GNPC_LINE_UP,				// �Ŷ�

		GNPC_GM_ACCOUNT = 150,		//GM��¼���
    };
    
    /******************************************************************************/
    //��Ϸ����������
    /******************************************************************************/
    enum GameNetPacketProcessMode
    {
        GNPPM_NORMAL  = 1,
        GNPPM_UNIQUE  = 2,
        GNPPM_DISCARDABLE = 4,
    };

#pragma pack (push)
#pragma pack (1)

    /******************************************************************************/
    //��Ϸ��ͷ
    /******************************************************************************/
    struct GameNetPacketHeader
    {
        Byte channel;
    };

    /******************************************************************************/
    //��Ϸ������
    /******************************************************************************/
    struct GameNetPacketData : public GameNetPacketHeader
    {
        Byte    type;
        Char8   data[1];
    };
    struct GameNetPacketData_INFO
    {
        static const Int headSize = sizeof(GameNetPacketData) - sizeof(Char8);
    };

    /******************************************************************************/
    // ���ݰ�������Ϣ����
    /******************************************************************************/
    
    // ���������
    enum GameNetPacketLevel
    {
        GPL_NONE                = 0,     
        GPL_SYS                 = BIT(0),   // ϵͳ��
        GPL_LOGIC               = BIT(1),   // �߼���
    };

    // �������������
    enum GameNetPacketFlowType
    {
        GPFL_NONE= 0,     
        GPFL_C2S,   // Client to Server
        GPFL_S2C,   // Server to Server
        GPFL_S2S,   // Server to Server
		GPFL_S2S2C,	// Server to Server to Client
		GPFL_C2S2S,	// Client to Server to Server
    };
    
    // ����������滻����
    enum GameNetPacketReplaceLevel
    {
        GPRL_NONE   = 0,     
        GPRL_SELF,      // �滻�Լ�
        GPRL_ALL,       // �滻����
    };
    
    // ��������涪������
    enum GameNetPacketDiscardLevel
    {
        GPDL_NONE   = 0, // �����滻     
        GPDL_HIGH,       // �����������ɵ�ʱ���滻
        GPDL_NORMAL,     // ��������ѹ��ʱ���滻
        GPDL_LOW,        // ��ʱ�����滻
    };

    // ���������ȴ�����
    enum GameNetPacketBlockWaitLevel
    {
        GPWL_NONE   = 0, // ���ȴ�   
        GPWL_SLEF,       // �ȴ��Լ�����Ϣ��Ӧ
        GPWL_All,        // �ȴ����е���Ϣ��Ӧ
    };


    // ��Ϣ����
    class GameNetPacketDataDescribe
    {
    public:

        GameNetPacketDataDescribe()
            :mChannel(0)
            ,mType(0)
            ,mBaseDataSize(0)
            ,mLevel(GPL_NONE)
            ,mFlow(GPFL_NONE)
            ,mReplaceLevelInSendQueue(GPRL_NONE)
            ,mReplaceIndexInSendQueue(0)
            ,mReplaceLevelInRecvQueue(GPRL_NONE)
            ,mReplaceIndexInRecvQueue(0)
            ,mDiscardLevelInSendQueue(GPDL_NONE)
            ,mDiscardLevelInRecvQueue(GPDL_NONE)
            ,mBlockWaitLevelInSendQueue(GPWL_NONE)
            ,mBlockWaitMaxTimeInSendQueue(0)         
            ,mBlockWaitReplyChannelInSendQueue(0)
            ,mBlockWaitReplyTypeInSendQueue(0)
            ,mBlockWaitLevelInRecvQueue(GPWL_NONE)
            ,mBlockWaitMaxTimeInRecvQueue(0)         
            ,mBlockWaitReplyChannelInRecvQueue(0)
            ,mBlockWaitReplyTypeInRecvQueue(0)
        {
        }

        // �õ��ȼ�
        const GameNetPacketLevel&   getLevel(){return mLevel;}
        // �õ�Ƶ��
        const Byte&                 getChannel(){return mChannel;}
        // �õ�����
        const Byte&                 getType(){return mType;}
        // �õ��������ݽṹ��С
        const Int&                  getBaseDataSize(){return mBaseDataSize;}

        // ��֤���ݵ���Ч��(��С�����ݺϷ���)
        virtual Bool                checkDataValid( GameNetPacketData* data, I32 dataLen )
        {
            if ( data->channel != mChannel )
                return false;
            if ( data->type != mType )
                return false;
            if ( dataLen != mBaseDataSize )
                return false;
            return true;
        }
        
    protected:

        Byte                        mChannel;
        Byte                        mType;
        Int                         mBaseDataSize;

        GameNetPacketLevel          mLevel;
        GameNetPacketFlowType       mFlow;

        GameNetPacketReplaceLevel   mReplaceLevelInSendQueue;
        Int                         mReplaceIndexInSendQueue;
        GameNetPacketReplaceLevel   mReplaceLevelInRecvQueue;
        Int                         mReplaceIndexInRecvQueue;

        GameNetPacketDiscardLevel   mDiscardLevelInSendQueue;
        GameNetPacketDiscardLevel   mDiscardLevelInRecvQueue;

        GameNetPacketBlockWaitLevel mBlockWaitLevelInSendQueue;
        Int                         mBlockWaitMaxTimeInSendQueue;
        Byte                        mBlockWaitReplyChannelInSendQueue;            
        Byte                        mBlockWaitReplyTypeInSendQueue;

        GameNetPacketBlockWaitLevel mBlockWaitLevelInRecvQueue;  
        Int                         mBlockWaitMaxTimeInRecvQueue;  
        Byte                        mBlockWaitReplyChannelInRecvQueue;              
        Byte                        mBlockWaitReplyTypeInRecvQueue;  
    };
    
#pragma pack (pop)

}

/******************************************************************************/

#endif