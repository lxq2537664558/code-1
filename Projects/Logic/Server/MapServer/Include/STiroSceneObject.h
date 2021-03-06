
/******************************************************************************/

#ifndef _STIROSCENEOBJECT_H_
#define _STIROSCENEOBJECT_H_

/******************************************************************************/

#include "MapServerPreqs.h"
#include "SSceneObject.h"

/******************************************************************************/

namespace MG
{
    /******************************************************************************/
    //  新手村场景管理器
    /******************************************************************************/
    class STiroSceneObject : public SSceneObject
    {
    public:

        STiroSceneObject();
        virtual ~STiroSceneObject();

        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

        virtual Bool                initialize();
        virtual Bool                unInitialize();
        virtual void                clear();
        virtual void                update(Flt delta);

		virtual GameType			getGameType() { return GameType_Rpg; }


    private:




    };


	/******************************************************************************/
	//服务端【新手村副本地图】智能指针
	/******************************************************************************/

	class STiroSceneObjectPtr : public SharedPtr<STiroSceneObject> 
	{

		friend class STiroSceneObjectFactory;

	public:

		STiroSceneObjectFactory* mFactory;

	public:

		virtual ~STiroSceneObjectPtr() { release(); }
		STiroSceneObjectPtr() : SharedPtr<STiroSceneObject>(),mFactory(NULL){}
		explicit STiroSceneObjectPtr(STiroSceneObject* rep) : SharedPtr<STiroSceneObject>(rep),mFactory(NULL){}
		STiroSceneObjectPtr(const STiroSceneObjectPtr& r) : SharedPtr<STiroSceneObject>(r),mFactory(r.mFactory) {} 

		STiroSceneObjectPtr& operator=(const STiroSceneObjectPtr& r) 
		{
			if (pRep == r.pRep)
				return *this;

			STiroSceneObjectPtr tmp(r);
			if ( isNull() == false )
			{
				tmp.mFactory = mFactory;
			}

			swap(tmp);

			mFactory = r.mFactory;

			return *this;
		}

	protected:

		virtual void    destroy(void){}

	};

}

#endif	