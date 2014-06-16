
/******************************************************************************/

#include "stdafx.h"

#include "RegionObjectManagerBase.h"

#include "ArtMapResCsv.h"
#include "MapListCsv.h"
#include "AreaListCsv.h"
#include "PlaceListCsv.h"
#include "MapDistributionCsv.h"
#include "InstanceTiroListCsv.h"
#include "InstanceListCsv.h"

#include "RelationWorldMapAreaCsv.h"
#include "RelationWorldMapPlaceCsv.h"


/******************************************************************************/

namespace MG
{
	//--------------------------------------------------------------------------------------
	Bool RegionObjectManagerBase::loadCsvAndCreateRegion()
	{
		//1. ��csv��
		loadCsv();

		//2. �����߼�����
		createAllMapDistribution();
		createAllPlace();
		//createAllTiroInstance();
		//createAllPVEInstance();
		//createAllHomeInstance();

		return true;
	}

	void RegionObjectManagerBase::createAllMapDistribution()
	{
		std::map<MapDistributionIdType, MapDistributionCsvInfo*>* MapDistributionList = MapDistributionCsv::getInstance().getList();

		DYNAMIC_ASSERT(false == MapDistributionList->empty());

		std::map<MapDistributionIdType, MapDistributionCsvInfo*>::iterator iter = MapDistributionList->begin();

		for ( ; iter != MapDistributionList->end(); ++iter)
		{
			MapDistributionCsvInfo* mapDistributionInfo = iter->second;
			DYNAMIC_ASSERT(NULL != mapDistributionInfo);

			//����Ƿ����ظ���
			//DYNAMIC_ASSERT(false == hasMapDistributionObj(mapDistributionInfo->mapDistributionId));

			if (mapDistributionInfo->mapDistributionId <= 0)
			{
				DYNAMIC_ASSERT(0);
				continue;
			}

			if (MDOT_Open == mapDistributionInfo->openType)
			{
				createMapDistributionObj(mapDistributionInfo);
			}

		}
	}

	//--------------------------------------------------------------------------------------
	void RegionObjectManagerBase::createAllPlace()
	{
		std::map<PlaceIdType, PlaceListCsvInfo*>* placeCsvList = PlaceListCsv::getInstance().getList();

		DYNAMIC_ASSERT(false == placeCsvList->empty());

		std::map<PlaceIdType, PlaceListCsvInfo*>::iterator iter = placeCsvList->begin();

		for ( ; iter != placeCsvList->end(); ++iter)
		{
			PlaceListCsvInfo* placeInfo = iter->second;
			DYNAMIC_ASSERT(NULL != placeInfo);

			if (PlaceType_Not_Create == placeInfo->placeType)
			{
				//����Ҫ������Ӧ�ڴ�
				continue;
			}

			DYNAMIC_ASSERT(PlaceType_Create == placeInfo->placeType);

			//����Ƿ����ظ���
			//DYNAMIC_ASSERT(false == hasPlaceObj(placeInfo->placeId));

			if (placeInfo->mapListId <= 0)
			{
				DYNAMIC_ASSERT(0);
				continue;
			}

			if (POT_Open == placeInfo->placeOpen)
			{
				createPlaceObj(placeInfo);
			}
			
		}
	}

	void RegionObjectManagerBase::createAllTiroInstance()
	{
		std::map<InstanceIdType, InstanceListCsvInfo*>*	instanceList = InstanceListCsv::getInstance().getList();
		DYNAMIC_ASSERT(false == instanceList->empty());

		std::map<InstanceIdType, InstanceListCsvInfo*>::iterator iter = instanceList->begin();

		for ( ; iter != instanceList->end(); ++iter)
		{
			InstanceListCsvInfo* instanceInfo = iter->second;
			DYNAMIC_ASSERT(NULL != instanceInfo);

			

			if (instanceInfo->instanceListId <= 0)
			{
				DYNAMIC_ASSERT(0);
				continue;
			}

			if (InstanceType_Tiro == instanceInfo->instanceType)
			{
				//����Ƿ����ظ���
				DYNAMIC_ASSERT(false == hasTiroInstanceObj(instanceInfo->instanceId));

				createTiroInstanceObj(instanceInfo);
			}
		}
	}

	void RegionObjectManagerBase::createAllPVEInstance()
	{
		std::map<InstanceIdType, InstanceListCsvInfo*>*	instanceList = InstanceListCsv::getInstance().getList();
		DYNAMIC_ASSERT(false == instanceList->empty());

		std::map<InstanceIdType, InstanceListCsvInfo*>::iterator iter = instanceList->begin();

		for ( ; iter != instanceList->end(); ++iter)
		{
			InstanceListCsvInfo* instanceInfo = iter->second;
			DYNAMIC_ASSERT(NULL != instanceInfo);

			if (instanceInfo->instanceListId <= 0)
			{
				DYNAMIC_ASSERT(0);
				continue;
			}

			if (InstanceType_PVE == instanceInfo->instanceType)
			{
				//����Ƿ����ظ���
				DYNAMIC_ASSERT(false == hasPveInstanceObj(instanceInfo->instanceId));

				createPveInstanceObj(instanceInfo);
			}
		}
	}

	void RegionObjectManagerBase::createAllHomeInstance()
	{
		std::map<InstanceIdType, InstanceListCsvInfo*>*	instanceList = InstanceListCsv::getInstance().getList();
		DYNAMIC_ASSERT(false == instanceList->empty());

		std::map<InstanceIdType, InstanceListCsvInfo*>::iterator iter = instanceList->begin();

		for ( ; iter != instanceList->end(); ++iter)
		{
			InstanceListCsvInfo* instanceInfo = iter->second;
			DYNAMIC_ASSERT(NULL != instanceInfo);

			if (instanceInfo->instanceListId <= 0)
			{
				DYNAMIC_ASSERT(0);
				continue;
			}

			if (InstanceType_HomeLand == instanceInfo->instanceType)
			{
				//����Ƿ����ظ���
				DYNAMIC_ASSERT(false == hasHomeInstanceObj(instanceInfo->instanceId));

				createHomeInstanceObj(instanceInfo);
			}
		}
	}

}