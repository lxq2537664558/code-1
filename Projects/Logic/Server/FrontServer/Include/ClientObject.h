/******************************************************************************/
#ifndef _CLIENTOBJECT_H_
#define _CLIENTOBJECT_H_
/******************************************************************************/

#include "FrontServerPreqs.h"
#include "SPlayer.h"
#include "SharedPtr.h"
/******************************************************************************/
namespace MG
{

    /******************************************************************************/
    //ClientConnectInfo对象
    /******************************************************************************/

    class ClientConnectInfo 
    {
        friend class ClientManager;
        friend class ClientObjectFactory;
    public:

        ClientConnectInfo();
        virtual ~ClientConnectInfo();

        virtual void        onConnected(I32 id);
        virtual void        onDisConnected();
        virtual Bool        isConnected();
        virtual void        disConnected();

        virtual NetIdType   getNetID();

    protected:

        NetIdType volatile mNetId;

    };

    
    /******************************************************************************/
    // ClientObject
    /******************************************************************************/
    class ClientObject : public ClientConnectInfo
    {

        friend class ClientManager;
        friend class ClientObjectFactory;

    public:

        ClientObject();
        virtual ~ClientObject();

        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

        virtual Bool                    initialize();
        virtual Bool                    unInitialize();
        virtual void                    update(Flt delta);
        virtual void                    clear();

        //-----------------------------------------------------------------------------------
        // Account Login
        //-----------------------------------------------------------------------------------
        
        //绑定账号信息到客户端
        void	                        setAccountInfo( Char16* accName, Char16* accPsw );
        Str16&	                        getAccountNAme();

		Bool	                        isMatchAccount( Char16* accName, Char16* accPsw );

        // 设置账号验证通过
        void	                        setAccountActive( Bool isActive );
        Bool	                        isAccountActive();


        //-----------------------------------------------------------------------------------
        // Player
        //-----------------------------------------------------------------------------------
        
        void                            setPlayer( SPlayerPtr& ptr );
        void                            getPlayer( SPlayerPtr& ptr );


    protected:

        Str16				mAccName;
        Str16				mAccPsw;
        Bool                mAccountActive;
        U32					mIpAddress;

        SPlayerPtr          mPlayerPtr;

        I32					mConnectTime;

    };


    /******************************************************************************/
    // ClientObjectPtr
    /******************************************************************************/

    /******************************************************************************/
    //客户端智能指针
    /******************************************************************************/

    class ClientObjectPtr : public SharedPtr<ClientObject> 
    {
        friend class ClientObjectFactory;

    protected:

        ClientObjectFactory* mFactory;

    public:

        virtual ~ClientObjectPtr() { release(); }
        ClientObjectPtr() : SharedPtr<ClientObject>(),mFactory(NULL){}
        explicit ClientObjectPtr(ClientObject* rep) : SharedPtr<ClientObject>(rep),mFactory(NULL){}
        ClientObjectPtr(const ClientObjectPtr& r) : SharedPtr<ClientObject>(r),mFactory(r.mFactory) {} 

        ClientObjectPtr& operator=(const ClientObjectPtr& r) 
        {
            //if (pRep == r.pRep)
            //    return *this;

            //SPlayerPtr tmp(r);
            //if ( isNull() == false )
            //{
            //    tmp.mFactory = mFactory;
            //}

            //swap(tmp);

            mFactory = r.mFactory;

            return *this;
        }

    protected:

        virtual void    destroy(void);

    };



}

#endif