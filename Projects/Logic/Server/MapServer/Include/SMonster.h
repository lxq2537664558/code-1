

//////////////////////////////////////////////////////////////
//
// Copyright (C), 2011, MG Software Co., Ltd. 
//
//      FileName: SMonster.h
//        Author: yuanlinhu
//          Date: 2012-6-4
//          Time: 15:02
//   Description: 
//			
//
// modify History:
//      <author>    <time>        <descript>
//      yuanlinhu  2012-6-4      add
//////////////////////////////////////////////////////////////


//*************************************************************************************************

#ifndef _S_MONSTER_H_
#define _S_MONSTER_H_

//*************************************************************************************************

#include "MapServerPreqs.h"
#include "SCharacter.h"
#include "MonsterData.h"
#include "SPlayerIdentifyInfo.h"

//*************************************************************************************************
namespace MG
{

    /******************************************************************************/
    // SMonster 实例对象
    /******************************************************************************/
	class SMonster : public SCharacter
	{
	public:
		SMonster();
		virtual ~SMonster();


        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

		virtual Bool                            initialize( SMonsterXml& xmlObj ){return true;}
		virtual Bool                            unInitialize(){ return true;}
		virtual void                            clear(){}

		virtual	void							update(Flt delta){}

        //-----------------------------------------------------------------------------------
        // Debug
        //-----------------------------------------------------------------------------------

		virtual void				            print();

        //-----------------------------------------------------------------------------------
        // Base Info
        //-----------------------------------------------------------------------------------

		MonsterData*				            getMonsterData();

        //-----------------------------------------------------------------------------------
        // Player
        //-----------------------------------------------------------------------------------

		// 由子类的 SPlayerIdentifyInfo 实现
		virtual AccountIdType	                getAccountId(){return 0;}
		virtual NetIdType 		                getFrontServerId(){return 0;} 
		virtual NetIdType 		                getClientNetIdInFrontServer() {return 0;}
		virtual ClanIdType		                getClanId(){return 0;}

        //-----------------------------------------------------------------------------------
        // Alive
        //-----------------------------------------------------------------------------------

		virtual void                            dead(SCharacter* murderer);
		virtual void				            setAlive(AliveType aliveType);		//复活操作

        //-----------------------------------------------------------------------------------
        // World Op
        //-----------------------------------------------------------------------------------

		virtual void				            moveToPos( Vec3 startMovePos, Vec3 MoveToPos );

		virtual void				            instantMoveToPos( Vec3 moveToPos );

		virtual void				            stopMove();

        //-----------------------------------------------------------------------------------
        // Fight Op
        //-----------------------------------------------------------------------------------

		virtual const FightAIGroup*		        getFightAIGroup();

		virtual void					        addFightAITriggerCount( UInt fightAIId );

		virtual UInt					        getFightAITriggerCount( UInt fightAIId );

		virtual void					        clearFightAITriggerCount();

        //-----------------------------------------------------------------------------------
        // AI
        //-----------------------------------------------------------------------------------

		virtual SAIController*			        getAIController();

		virtual void					        calAttrOther();


	protected:

		const FightAIGroup*				mFightAIGroup;

		SAIController*					mAIController;

		std::map<UInt, UInt>			mFightAITriggerCountList;

	private:

		SPlayerIdentifyInfo   mPlayerIdentifyInfo;

	};


	/******************************************************************************/
	//SMonster智能指针
	/******************************************************************************/

	class SMonsterPtr : public SharedPtr<SMonster> 
	{
		friend class SMonsterFactory;

	protected:

		SMonsterFactory* mFactory;

	public:

		virtual ~SMonsterPtr() { release(); }
		SMonsterPtr() : SharedPtr<SMonster>(),mFactory(NULL){}
		explicit SMonsterPtr(SMonster* rep) : SharedPtr<SMonster>(rep),mFactory(NULL){}
		SMonsterPtr(const SMonsterPtr& r) : SharedPtr<SMonster>(r),mFactory(r.mFactory) {} 

		SMonsterPtr& operator=(const SMonsterPtr& r) 
		{
			if (pRep == r.pRep)
				return *this;

			SMonsterPtr tmp(r);
			if ( isNull() == false )
			{
				tmp.mFactory = mFactory;
			}

			swap(tmp);

			mFactory = r.mFactory;

			return *this;
		}

	protected:

		virtual void    destroy(void);

	};
}

#endif	//_S_PLAYER_CHARACTER_NEW_H_