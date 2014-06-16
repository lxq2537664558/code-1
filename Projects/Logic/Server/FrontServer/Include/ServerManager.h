/******************************************************************************/
#ifndef _SERVERMANAGER_H_
#define _SERVERMANAGER_H_
/******************************************************************************/

#include "FrontServerPreqs.h"
#include "DataSetManager.h"
#include "CommonGameDefine.h"
#include "ServerStateNetPacket.h"

/******************************************************************************/

class SServerSetting;

namespace MG
{
    /******************************************************************************/

	#define MAX_ACTIVE_MAP_SERVER_COUNT 10
	#define NET_ID_ERROR		-1		//�����NetId


    /******************************************************************************/
    //Server����
    /******************************************************************************/

    class ServerConnectInfo 
    {
    public:

        NetIdType volatile mNetId;

        ServerConnectInfo();
        virtual ~ServerConnectInfo();

        virtual void    onConnected(I32 id);
        virtual void    onDisConnected();
        virtual Bool    isConnected();
        virtual void    disConnected();

		NetIdType getNetId() const;
    };

    /******************************************************************************/
    //MapServerInfo����
    /******************************************************************************/

    class MapServerConnectInfo : public ServerConnectInfo
	{
    public:

		UInt			mClientNum;
		U32				nServerId;          //��������� ����Ψһ
		U64				mServerUniqueFlag;
		Bool			mIsValid;

		I32				mMapServerNum;		//mapserver ����
		I32				mMapServerIndex;    //��ǰmapserver����mapserver�б��е�����

		MapServerConnectInfo(){}
		virtual ~MapServerConnectInfo(){}

        virtual void    onConnected(I32 id);
        virtual void    onDisConnected();

	};

    /******************************************************************************/
    //������������������֮���ӵ�LoginServer��MapServer����ز���
    /******************************************************************************/
    class ServerManager
    {
    public:

        SINGLETON_INSTANCE(ServerManager);
        ServerManager();
        virtual ~ServerManager();

    public:

        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

		//��ʼ���ͷ���ʼ��
        Bool			    initialize(SServerSetting* setting);
        void			    uninitialize();
        //��ӡ �������������
        void				printServer();

        //-----------------------------------------------------------------------------------
        // Connect Base
        //-----------------------------------------------------------------------------------

		//����ײ��¼�
        void			    onConnected(I32 id, GameNetType type,U32 serverId,U64 serverUniqueFlag, NetAddress*address);
        void			    onDisconnect(I32 id);
        Bool			    processPacket(I32 id, NetEventRecv* packet);

        void                onConnectLoginServer(I32 id);
        void                onConnectMapServer(I32 id, U32 serverId, U64 serverUniqueFlag);
        void                onConnectCenterServer(I32 id);
        void                onConnectChatServer(I32 id);
        void                onConnectSysLogServer(I32 id);

        void                onDisconnectLoginServer(I32 id);
        void                onDisconnectMapServer(I32 id);
        void                onDisconnectCenterServer(I32 id);
        void                onDisconnectChatServer(I32 id);
        void                onDisconnectSysLogServer(I32 id);


        //-----------------------------------------------------------------------------------
        // Send Packet
        //-----------------------------------------------------------------------------------

        //����Ϣ�� CenterServer
        void                sendNetPacketToCenterServer(GameNetSendDataBufferPtr& dataPtr,I32 dataSize);
        //����Ϣ�� SysLogServer
        void                sendNetPacketToSysLogServer(GameNetSendDataBufferPtr& dataPtr,I32 dataSize);
        //����Ϣ�� ChatServer
        void                sendNetPacketToChatServer(GameNetSendDataBufferPtr& dataPtr,I32 dataSize);
        //����Ϣ�� LoginServer
        void                sendNetPacketToLoginServer(GameNetSendDataBufferPtr& dataPtr,I32 dataSize);

        // �㲥���ݰ���ActiveMapServer
        void                boardcastNetPacketToAcitveMapServer(GameNetSendDataBufferPtr& dataPtr,I32 dataSize);
        // �������ݰ���ActiveMapServer �����������
        void				sendNetPacketToActiveMapServerByRegionId(GameNetSendDataBufferPtr& dataPtr,I32 dataSize,REGION_OBJ_TYPE regionType, RegionIdType regionId);
        // �������ݰ���ActiveMapServer ����NetID����
        void				sendNetPacketToActiveMapServerByNetId(GameNetSendDataBufferPtr& dataPtr,I32 dataSize,NetIdType mapServerId);


        //-----------------------------------------------------------------------------------
        // Connect MapServer Manager
        //-----------------------------------------------------------------------------------

        //���Ӻ��Ƴ�ConnectMapServer����
        Bool                hasConnectMapServer(NetIdType netId)
		{
			return true;
		}
        void                addConnectMapServer(NetIdType netId, ServerIdType serverId,ServerUniqueFlagIdType serverUniqueFlag)
		{

		}
		void                removeConnectMapServer(NetIdType netId){}
        void                removeAllConnectMapServer(){}

        //-----------------------------------------------------------------------------------
        // AcitveMapServer Manager
        //-----------------------------------------------------------------------------------

        //���Ӻ��Ƴ�ConnectMapServer����
        Bool                hasActiveMapServer(NetIdType netId);
        void                addActiveMapServer(NetIdType netId, ServerIdType serverId,ServerUniqueFlagIdType serverUniqueFlag);
        void                removeActiveMapServer(NetIdType netId);
        void                removeAllActiveMapServer();

        NetIdType			getActiveMapServerIdByUniqueFlag(const ServerUniqueFlagIdType serverUniqueFlag);
        // �õ�һ�������ActiveMapServer
        NetIdType			getRandActiveMapServerNetId();

        //-----------------------------------------------------------------------------------
        // AcitveMapServer Region Assign
        // AcitveMapServer ��mapid��netidӳ�����		
        //-----------------------------------------------------------------------------------

        // ��������������е�ActiveMapServer
        void				divideRegionForActiveMapServer(){}
        // �رղ������� ����ָ��ActiveMapServer
		void				freezePartRegionFromActiveMapServer(I32 netId){}


		void		        openReginObjectByDistrictId(const DistrictIdType districtId, const NetIdType mapServerNetId);
		void		        openReginObjectBySlaveDistrictInfo(DistrictListCsvInfo* childDistrictListCsvInfo, const NetIdType mapServerNetId);

		//������Region��MapServer�Ƿ񵱻�
		Bool				isMapServerCrash(const REGION_OBJ_TYPE reginType, const RegionIdType regionId);

        //-----------------------------------------------------------------------------------
        // Close Handle
        //-----------------------------------------------------------------------------------

		//MapServer�쳣�رյ�ʱ�����FrontServer�����ڸ�MapServer�������ڴ�
		void				closeAllClientsByMapServerId(I32 netId);

		//MapServer ���ߺ� ����Ӧ�ĵ�ͼ�رյ�
		void				setAllMapObjectByDisConnectMapServer(const I32 mapServerNetId);

		//����MapServer��ͬ����Player������PlayerCharacter������TroopGroup���б�
		void				syncAllListEndToMapServer(NetIdType mapServerId);


		void				handleSyncServerStateNew(PT_SERVER_STATE_S2S_SYNC_MAPSERVER_STATE_NEW_DATA* data);
		void				handleSyncServerStateNewStart(PT_SERVER_STATE_S2S_SYNC_MAPSERVER_STATE_NEW_START_DATA* data);

		//
		NetIdType			getLoginServerNetId();
		NetIdType			getCenterServerNetId();
		NetIdType			getChatServerNetId();
		NetIdType			getMapServer(NetIdType netId);
	private:    

        //
        SServerSetting*				mServerSetting;

        // LoginServer������
        ServerConnectInfo           mLoginServerConnect;
        // CenterServer������
        ServerConnectInfo           mCenterServerConnect;
        // SysLogServer������
        ServerConnectInfo           mSysLogServerConnect;
        // ChatServer������
        ServerConnectInfo           mChatServerConnect;
        
        /*
            # MapServer���С�
        */
        std::map<NetIdType, MapServerConnectInfo> mConnectMapServerList;
        RWLocker mConnectMapServerListCs;
        std::map<ServerUniqueFlagIdType, MapServerConnectInfo> mActiveMapServerList;
        RWLocker mActiveMapServerListCs;
		// ר�����ڷ�����Ϣ�����MapServer�� ���Ч��
		Int mMapServerNum;
    };
}
#endif