
/******************************************************************************/

#ifndef _S_ATKMAPOBJECT_H_
#define _S_ATKMAPOBJECT_H_

/******************************************************************************/

#include "MapServerPreqs.h"
#include "SMapObject.h"
#include "SSceneObjectFactory.h"
#include "SPlaceSceneObject.h"

/******************************************************************************/

namespace MG
{
    /******************************************************************************/
    //  主城地图管理器
    /******************************************************************************/
    class SPlaceMapObject : public SMapObject
    {
    public:
        SPlaceMapObject(PlaceListCsvInfo* placeListCsvInfo, DistrictIdType districtId);
        virtual ~SPlaceMapObject();

        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

        //virtual Bool                initialize();
        //virtual Bool                unInitialize();
        //virtual void                clear();
        //virtual void                update(Flt delta);

        //-----------------------------------------------------------------------------------
        // Base Data
        //-----------------------------------------------------------------------------------
		PlaceListCsvInfo*			getPlaceListCsvInfo();

		virtual	DistrictIdType		getDistrictId();		//绑定的政区ID
		virtual DistrictRankType	getDistrictRankType();		//政区的类型
		virtual RegionIdType		getRegionId();

		PlaceObjectData*			getPlaceData();
		DistrictListCsvInfo*		getDistrictListCsvInfo();

		
        //-----------------------------------------------------------------------------------
        // Scene
        //-----------------------------------------------------------------------------------

        //副本列表
		void		                createScene(SPlaceSceneObjectPtr& ptr, InstanceUIdType instanceUid);
		Bool		                getScene(SPlaceSceneObjectPtr& ptr, InstanceUIdType instanceUid);
		void		                destroyScene(InstanceUIdType instanceUid);
		void		                destroyAllScene();

		//主城
		void		                createMainScene(SPlaceSceneObjectPtr& ptr);
		Bool		                getMainScene(SPlaceSceneObjectPtr& ptr);
		void		                destroyMainScene();
    
	private:

		PlaceListCsvInfo*		mPlaceListCsvInfo;

		// 【攻城副本地图】工厂
		SPlaceSceneObjectFactory  mPlaceSceneObjectFactory;

		//【攻城副本地图】列表
		std::map<InstanceUIdType,SPlaceSceneObjectPtr> mSAtkSceneObjecList;

		//主城
		SPlaceSceneObjectPtr  mMainAtkSceneObject;

		RWLocker    mSAtkSceneObjectListCs;


	private:
		SPlaceMapObject();
    };


}

#endif	