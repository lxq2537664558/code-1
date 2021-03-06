//***********************************************************************************************************
#include "stdafx.h"
#include "RobotTaskThread.h"
#include "ThreadTaskManager.h"
#include "CClientNew.h"
#include "MGRobot.h"
#include "ClientNetApplyManager.h"
#include "CCampaignManagerNew.h"
//***********************************************************************************************************
namespace MG
{
	//----------------------------------------------------------------------
	RobotTaskThread::RobotTaskThread()
	{
		mManager = NULL;
		mIndex = -1;
		mCurrClient = -1;
		
		mCanRun = false;
		mLastTick = MGTimeOp::getCurrTick();
		mCampaignMgr = MG_NEW CCampaignManager(mIndex);
#ifdef TEST_THREAD_USE
		//------------------------------
		mClientTest = NULL;
		mClientCount = 0;
		mCurrClientIndex = 0;
		//------------------------------
#else
		mClients = NULL;
#endif

	}
	//----------------------------------------------------------------------
	RobotTaskThread::~RobotTaskThread()
	{
		MG_SAFE_DELETE(mCampaignMgr);
	}
	//----------------------------------------------------------------------
	void RobotTaskThread::setManager(ThreadTaskManager* mgr,Int index)
	{
		mManager = mgr;
		mIndex = index;
		mCampaignMgr->setThreadIndex(index);

	}
	//----------------------------------------------------------------------
	void RobotTaskThread::listenEvent()
	{
#ifdef ONE_THREAD
		if (theApp.mIsThreadActive == 1)
		{
			ClientNetApplyManager::getInstance().update();
			ThreadTaskManager::getInstance().getThread(0)->OneThreadGameUpdate();
		}
		
#else
		U32 currTick = MGTimeOp::getCurrTick();
		Flt delta = (currTick - mLastTick) * 0.001f;
		mLastTick = currTick;
#ifdef TEST_THREAD_USE
		//----------------------------------------
		if (mCanRun)
		{
			if (mCurrClientIndex >= mClientCount)
			{
				mCurrClientIndex = 0;
			}
			mClientTest[mCurrClientIndex]->update(delta);
			mCurrClientIndex++;
		}
		//----------------------------------------
#else
		// 跟新客户端 
		if (mCanRun)
		{
			if (mClientIt == mClients->end())
			{
				mClientIt = mClients->begin();
			}

			(*mClientIt)->update(delta);
			mClientIt++;
		}
#endif
		// 更新战场信息
#endif

		
	}
	//----------------------------------------------------------------------
	void RobotTaskThread::prepare()
	{
#ifdef TEST_THREAD_USE
		//---------------------------------------------
		mClientTest = mManager->mThreadClients[mIndex];
		mClientCount = mManager->mClientCount[mIndex];
		if (mClientTest == NULL || mClientCount == 0)
		{
			mCanRun = false;
		}
		else
		{
			for (Int i = 0; i < mClientCount; i++)
			{
				mClientTest[i]->setCampaignManger(mCampaignMgr);
			}
			mCurrClientIndex = 0;
			mCanRun = true;
		}
		//---------------------------------------------
#else
		mClients = mManager->getThreadClientVec(mIndex);
		std::vector<CClient*>::iterator it;
		for (it =mClients->begin(); it != mClients->end(); it++)
		{
			(*it)->setCampaignManger(mCampaignMgr);
		}
		if (mClients && !mClients->empty())
		{
			mClientIt = mClients->begin();
			mCanRun = true;
		}
		else
		{
			mCanRun = false;
		}
#endif

		

	}
	//----------------------------------------------------------------------
	void RobotTaskThread::OneThreadGameUpdate()
	{
#ifdef ONE_THREAD
		U32 currTick = MGTimeOp::getCurrTick();
		Flt delta = (currTick - mLastTick) * 0.001f;
		mLastTick = currTick;
		if (mCanRun)
		{
			if (mClientIt == mClients->end())
			{
				mClientIt = mClients->begin();
			}
			(*mClientIt)->update(delta);
			mClientIt++;
		}
#else
#endif
	}

	//----------------------------------------------------------------------
	Int RobotTaskThread::preassignmentCampaginList( I32 campaignNum,I32 generalNum,I32 time )
	{
		return mCampaignMgr->preassignmentCampaignList(campaignNum,generalNum,time);
	}
	//----------------------------------------------------------------------
	CCampaignManager* RobotTaskThread::getCampaignMgr()
	{
		return mCampaignMgr;
	}
	//----------------------------------------------------------------------
}