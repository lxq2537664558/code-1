//*****************************************************************************************************
#ifndef _MONSTERENTITYDATA_H_
#define _MONSTERENTITYDATA_H_
//*****************************************************************************************************
#include "CharacterEntityData.h"
//*****************************************************************************************************
namespace MG
{
	//*****************************************************************************************************
	//ArmyData
	struct MonsterEntityData : public CharacterEntityData
	{ 	


		//------------------------------------------------------------------------
		MonsterEntityData()
		{

		}

		//------------------------------------------------------------------------
		MonsterEntityData& operator=(const MonsterEntityData& A)
		{
			if (this == &A)
			{
				return *this;
			}

			CharacterEntityData::operator =(A);

			return *this;
		}  

	};
}
//*****************************************************************************************************
#endif
//*****************************************************************************************************