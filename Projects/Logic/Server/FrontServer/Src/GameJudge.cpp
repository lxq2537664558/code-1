//**********************************************************************************************************
#include "stdafx.h"
#include "GameJudge.h"
#include "CharacterJudgeTemplate.h"
#include "ClanJudgeTemplate.h"
#include "SClan.h"
#include "SCharacter.h"
#include "SClanQuestManager.h"
#include "SItemManager.h"
#include "CharacterList.h"
#include "SPlayer.h"
//**********************************************************************************************************
namespace MG
{
	Bool ClanJudge::checkClanLevel(SClan* pSClan, U32 level)
	{
		return true;
	}
	//--------------------------------------------------------------------------------------------
	Bool ClanJudge::checkClanQuest(SClan* pSClan, U32 questId)
	{
		if ( 0 == questId ) return true;

		if ( false == pSClan->getQuestManager()->isExistInFinishedQuests(questId) )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------
	Bool ClanJudge::checkClanItemAndNum(SClan* pSClan, U32 itemTemplateId, U32 num)
	{
		if ( 0 == itemTemplateId ) return true;

		U32 tempNum = pSClan->getItemManager()->findItemCountByTemplateId(itemTemplateId, ITEM_SITE_TYPE_CLAN);
		if ( tempNum < num )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------
	GAMEJUDGE_TYPE ClanJudge::checkClanJudge( SClan* pSClan, U32 clanJudgeId )
	{
		if ( 0 == clanJudgeId ) return GAMEJUDGE_TYPE_SUCCESS;

		const ClanJudgeInfo* pClanJudgeInfo = ClanJudgeTemplate::getInstance().getClanJudgeInfo(clanJudgeId);
		if (!pClanJudgeInfo)
		{   
            DYNAMIC_ASSERT_LOG(pClanJudgeInfo,"pClanJudgeInfo == NULL!");
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;
		}

		if ( false == checkClanLevel(pSClan, pClanJudgeInfo->Level) )
			return GAMEJUDGE_TYPE_CLAN_LEVEL_ERROR;

		if ( false == checkClanQuest(pSClan, pClanJudgeInfo->QuestId) )
			return GAMEJUDGE_TYPE_CLAN_COMPLETE_QUEST_ERROR;

		if ( false == checkClanItemAndNum(pSClan, pClanJudgeInfo->ItemTemplateId, pClanJudgeInfo->ItemNum) )
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;

		return GAMEJUDGE_TYPE_SUCCESS;
	}
	//--------------------------------------------------------------------------------------------
	Bool CharacterJudge::checkkCharacterType(SCharacter* pSCharacter, U32 characterType)
	{
		if ( 0 == characterType ) return true;

		if ( characterType & ( 1 << (pSCharacter->getCharacterTempInfo()->getCharacterType()-1) ) )
			return true;

		return false;
	}
	//--------------------------------------------------------------------------------------------
	Bool CharacterJudge::checkkCharacterLevel(SCharacter* pSCharacter, U32 characterLevel)
	{
		if ( pSCharacter->getLevel() < characterLevel )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------
	Bool CharacterJudge::checkkCharacterLineageLevel(SCharacter* pSCharacter, U32 characterLineageLevel)
	{
		if ( pSCharacter->getLineageNum() < characterLineageLevel )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------
	Bool CharacterJudge::checkkCharacterGenderType(SCharacter* pSCharacter, U32 characterGenderType)
	{
		if ( 0 == characterGenderType ) return true;

		if ( characterGenderType != pSCharacter->getCharacterTempInfo()->getCharacterGenderType() )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------
	//只有将领才会检查
	Bool CharacterJudge::checkkGenrealPowerfullType(SCharacter* pSCharacter, U32 genrealPowerfullType)
	{
		if ( GOT_PLAYERCHARACTER != pSCharacter->getType() ) return true;

		if ( 0 == genrealPowerfullType ) return true;

		if ( genrealPowerfullType & ( 1 << (pSCharacter->getCharacterTempInfo()->getGenrealPowerfulType() - 1) ) )
			return true;

		return false;
	}
	//--------------------------------------------------------------------------------------------
	//除了军队，其它都为特殊兵种。
	Bool CharacterJudge::checkkArmyType(SCharacter* pSCharacter, U32 armyType)
	{
		if ( 0 == armyType ) return true;

		if ( armyType & ( 1 << ( pSCharacter->getCharacterTempInfo()->getCharacterArmyType() -1 ) ) )
			return true;

		return false;
	}
	//--------------------------------------------------------------------------------------------
	GAMEJUDGE_TYPE CharacterJudge::checkCharacterJudge( SCharacter* pSCharacter, U32 characterJudgeId )
	{
		if ( 0 == characterJudgeId ) return GAMEJUDGE_TYPE_SUCCESS;

		const CharacterJudgeInfo* pCharacterJudgeInfo = CharacterJudgeTemplate::getInstance().getCharacterJudgeInfo(characterJudgeId);
		if (!pCharacterJudgeInfo)
		{
			DYNAMIC_ASSERT_LOG(pCharacterJudgeInfo,"error: pCharacterJudgeInfo == NULL!");
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;
		}
		if ( false == checkkCharacterType(pSCharacter, pCharacterJudgeInfo->CharacterType) ) 
			return GAMEJUDGE_TYPE_CHARACTER_TYPE_ERROR;

		if ( false == checkkCharacterLevel(pSCharacter, pCharacterJudgeInfo->Level) ) 
			return GAMEJUDGE_TYPE_CHARACTER_LEVEL_ERROR;

		if ( false == checkkCharacterLineageLevel(pSCharacter, pCharacterJudgeInfo->LineageLevel) ) 
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;

		if ( false == checkkCharacterGenderType(pSCharacter, pCharacterJudgeInfo->GenderType) ) 
			return GAMEJUDGE_TYPE_CHARACTER_GENDER_TYPE_ERROR;

		if ( false == checkkGenrealPowerfullType(pSCharacter, pCharacterJudgeInfo->PowerfulType) ) 
			return GAMEJUDGE_TYPE_CHARACTER_POWERFULL_TYPE_ERROR;

		if ( false == checkkArmyType(pSCharacter, pCharacterJudgeInfo->ArmyType) ) 
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;

		return GAMEJUDGE_TYPE_SUCCESS;
	}
	//--------------------------------------------------------------------------------------------
	GAMEJUDGE_TYPE GameJudge::checkGameJudge( SClan* pSClan, SCharacter* pSCharacter, U32 clanJudgeId, U32 characterJudgeId )
	{


		GAMEJUDGE_TYPE judegeType;
        if (pSCharacter)
        {
	       //角色判断：如果需要判断，并且判断为误，返回false
			judegeType =mCharacterJudge.checkCharacterJudge(pSCharacter, characterJudgeId);
			if (judegeType != GAMEJUDGE_TYPE_SUCCESS)
			{
				return judegeType;
			}
        }

		if (pSClan)
		{
			//氏族判断：如果需要判断，并且判断为误，返回false
			judegeType = mClanJudge.checkClanJudge(pSClan, clanJudgeId);	
			if (judegeType != GAMEJUDGE_TYPE_SUCCESS)
			{
				return judegeType;
			}
		}

		return GAMEJUDGE_TYPE_SUCCESS;
	}
	//--------------------------------------------------------------------------------------------
	GAMEJUDGE_TYPE GameJudge::checkClanJudge( SClan* pSClan, U32 clanJudgeId )
	{
		if (!pSClan)
		{
			DYNAMIC_ASSERT_LOG(pSClan,"error: pSClan == NULL!");
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;
		}
		
        return mClanJudge.checkClanJudge(pSClan, clanJudgeId);

		
	}
	//--------------------------------------------------------------------------------------------
	GAMEJUDGE_TYPE GameJudge::checkCharacterJudge( SCharacter* pSCharacter, U32 characterJudgeId )
	{
		if (!pSCharacter)
		{
			DYNAMIC_ASSERT_LOG(pSCharacter,"error: pSCharacter == NULL!");
			return GAMEJUDGE_TYPE_UNKNOWN_ERROR;
		}

		//角色判断：如果需要判断，并且判断为误，返回false
		return mCharacterJudge.checkCharacterJudge(pSCharacter, characterJudgeId);
		
	}
}   