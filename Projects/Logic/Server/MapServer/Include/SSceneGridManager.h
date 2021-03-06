/******************************************************************************/
#ifndef _SREGIONGRIDMANAGER_H_
#define _SREGIONGRIDMANAGER_H_
/******************************************************************************/

#include "MapServerPreqs.h"
/******************************************************************************/

/******************************************************************************/

namespace MG
{

	/******************************************************************************/
	// 用于描述一个场景对象和那些场景网格有关联
	/******************************************************************************/
	struct SSceneObjectInSceneGridsManager
	{

		SSceneGrid*					mLastSceneGrid;
		SSceneGrid*					mCurrSceneGrid;

		// 当前关注的网格列表
		std::map<UInt, SSceneGrid*>	mInteractionGridList;
        Critical                    mInteractionGridListCs;

		SSceneObjectInSceneGridsManager()
			:mLastSceneGrid(NULL)
			,mCurrSceneGrid(NULL)
		{
		}
        
		~SSceneObjectInSceneGridsManager()
		{
			mLastSceneGrid = NULL;
			mCurrSceneGrid = NULL;
			mInteractionGridList.clear();
		}
	};


	/******************************************************************************/
	// SSceneGridManager
	/******************************************************************************/
	class SSceneGridManager
	{
	public:
		SSceneGridManager();
		virtual ~SSceneGridManager();


        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

		virtual Bool                            initialize( SSceneObject* sceneObj );
        virtual Bool                            unInitialize();
        virtual void                            clear();

	    virtual	void							update(Flt delta){ return ;}

        //-----------------------------------------------------------------------------------
        // Base Info
        //-----------------------------------------------------------------------------------

		UInt									getTemplateId();

        //-----------------------------------------------------------------------------------
        // Grid
        //-----------------------------------------------------------------------------------

    public:

		// ？？？
		SSceneGrid*							    getSceneGrid( Vec3 pos );

    protected:

		//-----------------------------------------------------------------------------------
		// Load Grid Map
		//-----------------------------------------------------------------------------------

		void									loadGridMap( SSceneObject* sceneObj );

        // 得到一定范围内的网格列表
        void									getSceneGridsList( Vec3 pos, UInt width, std::map<UInt, SSceneGrid*>& gridsList );


        // 2个网格列表相减
        // resultGridsList = gridsList1 - gridsList2 ( 范围域是gridsList1 )
        void									subSceneGridsList( std::map<UInt, SSceneGrid*>& gridsList1, std::map<UInt, SSceneGrid*>& gridsList2, std::map<UInt, SSceneGrid*>& resultGridsList );


        // ？？？
        UInt									getSceneGridIndex( Vec3 pos );
        // ？？？
        VecU2									getSceneGridPos( Vec3 pos );
        // ？？？
        UInt									getSceneGridIndexForGridPos( VecU2 gridPos );

        // ？？？
        void									destroyAllGrid();

        //-----------------------------------------------------------------------------------
        // SCharacter
        //-----------------------------------------------------------------------------------

    public:

		// 增加或移动一个角色网格信息到网格列表中
		void									addOrUpdateObjectInSceneGrid( SCharacter* character, Bool isFilterSelf = true );
        
		void									addOrUpdateObjectInSceneGrid( dropItemData* dropItem );
        // 移除一个角色对象
		void									removeObjectInSceneGrid( SCharacter* character, Bool isFilterSelf = true );

		// 移除所有角色对象
		void									removeAllObjectInSceneGrid();

		
    protected:

		void									onEnterSceneGrid( SCharacter* character, SSceneGrid* currGrid, std::map<UInt, SSceneGrid*>& enterGridList, Bool isFilterSelf = true );
		void									onLeaveSceneGrid( SCharacter* character, std::map<UInt, SSceneGrid*>& leaveGridList, Bool isFilterSelf = true );

	protected:

		SRegionObjectBase*						mRegionObjectBase;
		SSceneGrid*							    mSceneGridList;
		U16										mGridWidthNum;
		U16										mGridHeightNum;
		U16										mGridWidth;
		U16										mGridHeight;
		Byte									mInteractionGridWidth;
		Byte									mInteractionGridHalfWidth;
		Byte									mNeedAddGridWidth;
		Byte									mNeedAddGridHalfWidth;
		UInt									mTemplateId;
		Vec3									mLeftBottomPos;
	};
}


#endif	