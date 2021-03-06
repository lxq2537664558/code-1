/******************************************************************************/

#include "stdafx.h"
#include "SClan.h"
#include "SPlayer.h"
#include "LoginNetPacket.h"
//#include "STroopGroup.h"
#include "SItemManager.h"
#include "CharacterList.h"
#include "PlayerCharacterDB.h"
#include "FrontServerMain.h"
#include "ClanDataBase.h"
#include "SWorldManager.h"
#include "SSkillOwnManager.h"
//#include "HeartBeatManager.h"

/******************************************************************************/

namespace MG
{
    //--------------------------------------------------------------------------
    SClan::SClan()
    {
    }

    //--------------------------------------------------------------------------
    SClan::~SClan()
    {
	}

    //-----------------------------------------------------------------------------------
    // Main
    //-----------------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Bool SClan::initialize( ClanData& clanData )
    {
        mClanData = clanData;

        //TODO:
        mItemManager.setParent( this );
        mSBuffManager.setParent( this );
		mQuestManager.setClan( this );
		mCharacterIDBuilderManager.init(100);

		return true;
    }

    //--------------------------------------------------------------------------
    Bool SClan::unInitialize()
    {
		mCharacterIDBuilderManager.unInit();

		return true;
    }

    //--------------------------------------------------------------------------
    void SClan::update(Flt delta)
    {

    }

    //--------------------------------------------------------------------------
    void SClan::clear()
    {
        removeAllPlayerCharacter();
        mItemManager.clear();
        mSBuffManager.clear();
		mCharacterIDBuilderManager.clear();
    }

    //-----------------------------------------------------------------------------------
    // Debug
    //-----------------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void SClan::print()
    {
        //Clan::print();

        ////所有者
        //std::string strParent("空");
        //if (NULL != mParentPlayer)
        //{
        //    std::string strAccount;
        //    MGStrOp::toString(mParentPlayer->getAccountID(), strAccount);
        //    strParent = "AccountId = [" + strAccount + "]";
        //}

        ////遍历武将id
        //std::string strPlayerCharacterIds;
        //{
        //    std::map<IdType, SPlayerCharacter*>::iterator iter = mPlayerCharacterList.begin();
        //    for ( ; iter != mPlayerCharacterList.end(); iter++)
        //    {
        //        std::string id;
        //        MGStrOp::toString(iter->second->getID(), id);
        //        if (strPlayerCharacterIds.empty())
        //        {
        //            strPlayerCharacterIds = strPlayerCharacterIds + id;
        //        }
        //        else
        //        {
        //            strPlayerCharacterIds = strPlayerCharacterIds + ", " + id;
        //        }
        //    }

        //    if (strPlayerCharacterIds.empty())
        //    {
        //        strPlayerCharacterIds = "空";
        //    }
        //}
    }


    //------------------------------------------------------------------------------------
    //	PlayerCharactor
    //------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Bool SClan::addPlayerCharacter( SPlayerCharacterPtr& charPtr, PlayerCharacterData& charData )
    {
		//创建ID, 如果没有，就创建ID
		//把SPlayerCharacter计入IDBuilderManager，免得ID重复。
		charPtr.getPointer()->setID(charData.mId);
		charPtr.getPointer()->setParentClan(this);
		mCharacterIDBuilderManager.addIDObject((IDObject*)(charPtr.getPointer()));
		charData.mId = charPtr.getPointer()->getID();

        if ( hasPlayerCharacter( charData.mId ) )
        {
            DYNAMIC_EEXCEPT_LOG("already exist PlayerCharacter!");
            removePlayerCharacter( charData.mId );
        }

        charData.charClanId       = getClanID();

        mPlayerCharacterFactory.createSPlayerCharacterPtr( charPtr, charData );

        mPlayerCharacterMapCs.writeLock();
        {
            mPlayerCharacterMap[charData.mId] = charPtr;
        }
        mPlayerCharacterMapCs.writeUnLock();

		return true;
    }

    //--------------------------------------------------------------------------
    void SClan::removePlayerCharacter(PlayerCharacterIdType charId)
    {
        SPlayerCharacterPtr charPtr;

        if ( getPlayerCharacter( charPtr, charId ) )
        {
            //------------------------------------------------------------------------------------
			mCharacterIDBuilderManager.removeIDObject(charId);
            charPtr->clear();
            //------------------------------------------------------------------------------------

            mPlayerCharacterMapCs.writeLock();
            {
                std::map<PlayerCharacterIdType,SPlayerCharacterPtr>::iterator iter = mPlayerCharacterMap.find( charId );
                if ( iter != mPlayerCharacterMap.end() )
                {
                    mPlayerCharacterMap.erase( iter );
                }
            }
            mPlayerCharacterMapCs.writeUnLock();
        }
    }

    //--------------------------------------------------------------------------
    Bool SClan::getPlayerCharacter(SPlayerCharacterPtr& charPtr, PlayerCharacterIdType charId)
    {
        Bool result = false;

        mPlayerCharacterMapCs.readLock();
        {
            std::map<PlayerCharacterIdType,SPlayerCharacterPtr>::iterator iter = mPlayerCharacterMap.find( charId );
            if ( iter != mPlayerCharacterMap.end() )
            {
                charPtr = iter->second;
                result = true;
            }
        }
        mPlayerCharacterMapCs.readUnLock();

        return result;
    }

    //--------------------------------------------------------------------------
    Bool SClan::hasPlayerCharacter(PlayerCharacterIdType charId)
    {
        Bool result = false;

        mPlayerCharacterMapCs.readLock();
        {
            std::map<PlayerCharacterIdType,SPlayerCharacterPtr>::iterator iter = mPlayerCharacterMap.find( charId );
            if ( iter != mPlayerCharacterMap.end() )
            {
                result = true;
            }
        }
        mPlayerCharacterMapCs.readUnLock();

        return result;
    }

    //--------------------------------------------------------------------------
    Bool SClan::hasPlayerCharacter()
    {
        Bool result = false;

        mPlayerCharacterMapCs.readLock();
        {
            if ( mPlayerCharacterMap.size() > 0 )
            {
                result = true;
            }
        }
        mPlayerCharacterMapCs.readUnLock();

        return result;
    }

    //--------------------------------------------------------------------------
    I32 SClan::getPlayerCharacterCount()
    {
        I32 count = 0;

        mPlayerCharacterMapCs.readLock();
        {
            count = mPlayerCharacterMap.size();
        }
        mPlayerCharacterMapCs.readUnLock();

        return count;
    }

    //--------------------------------------------------------------------------
    void SClan::removeAllPlayerCharacter()
    {
        mPlayerCharacterMapCs.writeLock();
        {
            std::map<PlayerCharacterIdType,SPlayerCharacterPtr>::iterator iter = mPlayerCharacterMap.begin();
            for ( ;iter != mPlayerCharacterMap.end(); iter++ )
            {
                iter->second->clear();
            }

            mPlayerCharacterMap.clear();
        }
        mPlayerCharacterMapCs.writeUnLock();
		
    }

    //--------------------------------------------------------------------------
    Bool SClan::getMainPlayerCharacter( SPlayerCharacterPtr& charPtr )
    {
        return getPlayerCharacter( charPtr, getData()->mainGenrealId );
    }

    //--------------------------------------------------------------------------
    void SClan::setMainPlayerCharacter( PlayerCharacterIdType charId )
    {
        SPlayerCharacterPtr charPtr;

        if ( getPlayerCharacter( charPtr, charId ) )
        {
            getData()->mainGenrealId = charId;
        }
		else
		{
			DYNAMIC_ASSERT_LOG(0, "主武将不存在");
		}
    }
        
    //--------------------------------------------------------------------------
    Bool SClan::checkMainGenreal()
    {
		return 0 != getData()->mainGenrealId;
    }
	//-----------------------------------------------------------------------------------
	MG::Bool SClan::getWiseGenreal( SPlayerCharacterPtr& charPtr )
	{
		return getPlayerCharacter(charPtr, getData()->wiseGenrealId);
	}
	//-----------------------------------------------------------------------------------
	void SClan::setWiseGenreal( PlayerCharacterIdType charId )
	{
		SPlayerCharacterPtr charPtr;

		if ( getPlayerCharacter( charPtr, charId ) )
		{
			getData()->wiseGenrealId = charId;
		}
		else
		{
			DYNAMIC_ASSERT_LOG(0, "军师不存在");
		}
	}
	//-----------------------------------------------------------------------------------
	MG::Bool SClan::checkWiseGenreal()
	{
		return 0 != getData()->wiseGenrealId;
	}
	//-----------------------------------------------------------------------------------
	MG::Bool SClan::getCommanderGenreal( SPlayerCharacterPtr& charPtr )
	{
		return getPlayerCharacter(charPtr, getData()->commanderGenrealId);
	}
	//-----------------------------------------------------------------------------------
	void SClan::setCommanderGenreal( PlayerCharacterIdType charId )
	{
		SPlayerCharacterPtr charPtr;

		if ( getPlayerCharacter( charPtr, charId ) )
		{
			getData()->commanderGenrealId = charId;
		}
		else
		{
			DYNAMIC_ASSERT_LOG(0, "统领不存在");
		}
	}
	//-----------------------------------------------------------------------------------
	MG::Bool SClan::checkCommanderGenreal()
	{
		return 0 != getData()->commanderGenrealId;
	}
	//-----------------------------------------------------------------------------------
	MG::Bool SClan::getCurGenreal( SPlayerCharacterPtr& charPtr )
	{
		return getPlayerCharacter(charPtr, getData()->curGenrealId);
	}
	//-----------------------------------------------------------------------------------
	void SClan::setCurGenreal( PlayerCharacterIdType charId )
	{
		SPlayerCharacterPtr charPtr;

		if ( getPlayerCharacter( charPtr, charId ) )
		{
			getData()->curGenrealId = charId;
		}
		else
		{
			DYNAMIC_ASSERT_LOG(0, "当前武将不存在");
		}
	}
	//-----------------------------------------------------------------------------------
	MG::Bool SClan::checkCurGenreal()
	{
		return 0 != getData()->curGenrealId;
	}
    //------------------------------------------------------------------------------------
    //	Item
    //------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	SItemManager* SClan::getItemManager()
	{
		return &mItemManager;
	}	

    //------------------------------------------------------------------------------------
    //	Buff
    //------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    SBuffManager* SClan::getSBuffManager()
    {
        return &mSBuffManager;
    }
	
	//-----------------------------------------------------------------------------------
	// Quest
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	SClanQuestManager* SClan::getQuestManager()
	{
		return &mQuestManager;
	}


	//-----------------------------------------------------------------------------------
	// GeneralRecruit
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	GeneralRecruitSystem* SClan::getGeneralRecruitSystem()
	{
		return &mGeneralRecruitSystem;
	}

	//-----------------------------------------------------------------------------
	NetIdType SClan::getClientNetId() 
	{ 
		return mParentPlayer->getClientNetID(); 
	}

	//-----------------------------------------------------------------------------
	void SClanPtr::destroy(void)
	{
		mFactory->destroySClanPtr(*this);
	}
}