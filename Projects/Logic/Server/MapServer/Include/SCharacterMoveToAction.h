/******************************************************************************/
#ifndef _SCHARACTERMOVETOACTION_H_
#define _SCHARACTERMOVETOACTION_H_
/******************************************************************************/

#include "MapServerPreqs.h"
#include "LinedState.h"

/******************************************************************************/

namespace MG
{


	/******************************************************************************/
	//��Ϸ�����ƶ���Ϊ
	/******************************************************************************/
	class SCharacterMoveToAction : public LinedStateObject
	{
	public:
		SCharacterMoveToAction( SCharacter* character, Vec3 startMovePos, Vec3 moveToPos );
		virtual ~SCharacterMoveToAction();

	public:

		//״̬����
		//static Str STATE_NAME = "";
#define STATE_NAME "dfefefe"

		///�����¼�
		virtual void        update( Flt delta );
		///�Ƿ������
		virtual Bool        isFinished();
		///��ȥ״̬�¼�
		virtual void        onEnter();
		///�뿪״̬�¼�
		virtual void        onLeave();

		////////////////////////////////////////////////////////////////////////////////////

		void				setMoveSpeed( Flt speed );

		Vec3				getMoveToPos();

	private:

		SCharacter*			mCharacter;

		Vec3				mStartMovePos;
		Vec3				mMoveToPos;
		Vec3				mMoveToDir;

		Flt					mCurrMoveTime;
		
		Flt					mMoveSpeed;

		Flt					mMoveTimeLength;

		Flt					mMoveDis;

	};
}

/******************************************************************************/

#endif //