/******************************************************************************/
#ifndef _CAIENTITYFIGHTTRIGGER_H_
#define _CAIENTITYFIGHTTRIGGER_H_
/******************************************************************************/

#include "MapServerPreqs.h"
#include "LinedState.h"
#include "FightManager.h"
#include "AIFightTrigger.h"

/******************************************************************************/

namespace MG
{

	/******************************************************************************/
	/******************************************************************************/

	/////////////////////////////////////////////////////////////////////////////////////////////

	class SAIFightTrigger : public AIFightTrigger, public LinedStateObject
	{
	public:
		SAIFightTrigger( SCharacter* owner );
		virtual ~SAIFightTrigger();

		//状态名字
		static Str STATE_NAME;

		///更新事件
		virtual void        						update( Flt delta );
		///是否结束了
		virtual Bool        						isFinished();
		///进去状态事件
		virtual void        						onEnter();
		///离开状态事件
		virtual void        						onLeave();

		/////////////////////////////////////////////////////////////////////////////////////////////

		//void										addFightTriggerListener( FightTriggerListener* listener );

		//void										removeFightTriggerListener( FightTriggerListener* listener );

		//void										clearAllFightTriggerListener();

		/////////////////////////////////////////////////////////////////////////////////////////////

		void										notifyFightTrigger( FightTriggerEvent& fightTriggerEvent );

		/////////////////////////////////////////////////////////////////////////////////////////////

		void										setCurrTarget( GAMEOBJ_TYPE gameObjType, IdType gameObjId );

	protected:

		////////////////////////////////////////////////////////////////////////////////////////////

		void	getCharacterForContactCharType( FightAIInfo* fightAIInfo, GAMEOBJ_TYPE& gameObjType, IdType& gameObjId );

		void	getCharacterForEventCharType( FightAIInfo* fightAIInfo, std::vector<FightEventGameObj>& fightEventGameObjList );

		////////////////////////////////////////////////////////////////////////////////////////////

		FightAIInfo*								checkFightTrigger( const FightAIGroup* fightAIInfoGroup );

		Bool										checkContactType1( FightAIInfo* fightAIInfo );

		Bool										checkContactType2( FightAIInfo* fightAIInfo );

		Bool										checkContactType3( FightAIInfo* fightAIInfo );

		Bool										checkContactType4( FightAIInfo* fightAIInfo );

		Bool										checkContactType5( FightAIInfo* fightAIInfo );

		Bool										checkContactType6( FightAIInfo* fightAIInfo );

		Bool										checkContactType7( FightAIInfo* fightAIInfo );

		Bool										checkContactType8( FightAIInfo* fightAIInfo );

		void										setFightEvent( FightAIInfo* fightAIInfo );

		////////////////////////////////////////////////////////////////////////////////////////////

		void										addFightAITriggerCount( UInt fightAIId );

		UInt										getFightAITriggerCount( UInt fightAIId );

	protected:

		/*std::list<FightTriggerListener*>			mListenerList;*/

		SCharacter*									mOwner;

		GAMEOBJ_TYPE								mCurrTargetType;
		IdType										mCurrTargetId;
		const FightAIGroup*							mFightAIGroup;
		Flt											mPauseThinkTime;
	};
}

#endif