/******************************************************************************/
#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_
/******************************************************************************/

#include "FrontServerPreqs.h"
#include "LoginNetPacket.h"
#include "ClientObjectFactory.h"
#include "ClientObject.h"
/******************************************************************************/
namespace MG
{

 //   /******************************************************************************/
 //   //ClientConnectInfo����
 //   /******************************************************************************/

 //   class ClientConnectInfo 
 //   {
 //   public:

 //       NetIdType   mNetId;

 //       ClientConnectInfo();
 //       virtual ~ClientConnectInfo();

 //       virtual void    onConnected(I32 id);
 //       virtual void    onDisConnected();
 //       virtual Bool    isConnected();
 //       virtual void    disConnected();
 //   };


	///******************************************************************************/
	////�ͻ��˶���
	///******************************************************************************/
 //   class ClientObject : public ClientConnectInfo
	//{
	//public:

	//	I32					connectTime;
	//	Bool volatile		isDestroy;
 //       
	//	Str16				accName;
	//	Str16				accPsw;
	//	U32					ipAddress;

 //       Player
	//	
	//	ClientObject(I32 id, U64 uin, U32 ip)
	//	{
	//		accName.reserve(MAX_ACCOUNT_LONGIN_NAME_LEN);
	//		accPsw.reserve(MAX_ACCOUNT_LONGIN_PSW_LEN);
	//		accName = L"";
	//		accPsw	= L"";
	//		netId	= id;
	//		ipAddress	= ip;
	//		revert(uin);
	//		
	//	}

	//	void revert(U64 uin)
	//	{
	//		isDestroy = false;
	//		addressUIN	= uin;
	//		connectTime = MGTimeOp::getCurrTick();
	//	}
	//	
	//	void destroy()
	//	{
	//		MG_LOG(out_info,"connect -> checkok time = %d\n",MGTimeOp::getCurrTick() - connectTime);
	//		isDestroy = true;
	//	}
	//};

    /******************************************************************************/
	// ʧЧ��   
	//�ͻ��˹���
	//# ���������δ����Ŀͻ��˶�����Ҫְ��Ϊ���ͻ��˵��볬ʱ��

	// �޸�Ϊ��
	//# ����������޸ģ���Ҫְ��������⽡׳�ԣ���ͻ��˶Ͽ������ӣ�splayer������
	//# ClientObject �����ϵĶ��� �Ѿ�����δ��¼�Ķ��� ���Բ�������ⳬʱ
    /******************************************************************************/
    class ClientManager
    {
    public:

        SINGLETON_INSTANCE(ClientManager);
        ClientManager();
        virtual ~ClientManager();

    public:

        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

        Bool                    initialize();
        void                    uninitialize();
		void	                update();

        //-----------------------------------------------------------------------------------
        // Connect Base
        //-----------------------------------------------------------------------------------

        void	                onConnected(I32 id,NetAddress*address,INetConnect* connect);
        void	                onDisconnect(I32 id,INetConnect* connect);
        Bool	                processPacket(I32 id, NetEventRecv* packet,INetConnect* connect);

        //-----------------------------------------------------------------------------------
        // ClientObject Manager
        //-----------------------------------------------------------------------------------

        //���Ӻ��Ƴ�ClientObject����
        void                    addClientObject(NetIdType netId, NetAddress*address,INetConnect* connect);
        void                    removeClientObject(NetIdType netId);
        void                    removeAllClientObject();

        Bool                    hasClientObject(NetIdType netId);
        ClientObject*                    getClientObject(/*ClientObject** ptr, */NetIdType netId);

       
        //-----------------------------------------------------------------------------------
        // TimeOut Check
        //-----------------------------------------------------------------------------------
        
		//���ͻ��˳�ʱ
		static void	            checkTimeOutThread(Ptr ptr); 

	private:

        ClientObjectFactory  mClientObjectFactory;
        //std::map<NetIdType,ClientObjectPtr> mClientObjectMap;
        RWLocker    mClientObjectMapCs;

		OneThreadHandle mCheckTimeOutThread;

		std::map<NetIdType,ClientObject*> mClientObjectMap;

    };
}
#endif