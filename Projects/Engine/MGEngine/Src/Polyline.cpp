/******************************************************************************/
#include "stdafx.h"
#include "Polyline.h"
#include "MGEngineUtility.h"
/******************************************************************************/

namespace MG
{

    /*
    -----------------------------------------------------------------------------
    Class Polyline
    -----------------------------------------------------------------------------
    */

    //-----------------------------------------------------------------------
    Polyline::Polyline()
        :mAutoGeneratedNormalType(NORT_BISECTOR),
        mAutoGeneratedNormalEnable(false),
        mAutoGeneratedNormalScaleEnable(false),
        mAutoGeneratedNormalDirection(NORT_XZ_LEFTDIR)
    {
    }

    //-----------------------------------------------------------------------
    Polyline::~Polyline()
    {
        fireDestroy();
    }

    //-----------------------------------------------------------------------
    void Polyline::update( Real delta )
    {

    }

    //-----------------------------------------------------------------------
    void Polyline::addKeyPoint( Vec3 pos )
    {
        PolylinePoint point;
        point.position = MGEngineUtility::toVec3(pos);
        addKeyPoint( point );
    }

    //-----------------------------------------------------------------------
    void Polyline::setKeyPointPosition( Vec3 pos, Int index )
    {
        Ogre::Vector3 point = MGEngineUtility::toVec3(pos);

        setKeyPointPosition( point, index );
    }

    //-----------------------------------------------------------------------
    void Polyline::addKeyPoint( const PolylinePoint&  parameters )
    {
        KeyPointList::iterator i = mKeyPoints.begin();
        mKeyPoints.size();
        mKeyPoints.push_back(parameters);
        fireAdd(mKeyPoints.size()-1);
    }

    //-----------------------------------------------------------------------
    void Polyline::addKeyPoint( Real dis )
    {
        size_t lastIndex = getKeyPointCount() - 1;
        PolylinePoint addPoint;
        PolylinePoint& priorPoint =  getRefKeyPoint(lastIndex-1);
        PolylinePoint& currPoint  =  getRefKeyPoint(lastIndex);
        Vector3 addVec = currPoint.position - priorPoint.position;
        addVec.normalise();
        addVec = addVec * dis;
        addPoint.position = currPoint.position + addVec;

        addKeyPoint( addPoint );
    }

    //-----------------------------------------------------------------------
    const PolylinePoint& Polyline::getKeyPoint( size_t index )
    {
        return getRefKeyPoint(index);
    }

    //-----------------------------------------------------------------------
    size_t Polyline::getKeyPointCount()
    {
        return mKeyPoints.size();
    }

    //-----------------------------------------------------------------------
    size_t Polyline::getKeyPointMaxIndex()
    {
        size_t count;
        count = mKeyPoints.size();
        if ( count == 0 )
            return 0;
        else
            return count - 1;
    }

    //-----------------------------------------------------------------------
    std::list<PolylinePoint>& Polyline::getAllKeyPoint(void)
    {
        return mKeyPoints;
    }

    //-----------------------------------------------------------------------
    void Polyline::clearKeyPoints(void)
    {
        mKeyPoints.clear();
        fireClear();
    }

    //-----------------------------------------------------------------------
    void Polyline::rebuildKeyPoints(const std::vector<PolylinePoint>& points)
    {
        mKeyPoints.clear();

        for(std::vector<PolylinePoint>::const_iterator iter = points.begin(); iter != points.end(); ++iter)
        {
            mKeyPoints.push_back( *iter );
        }

        fireRebuild();
    }

    //-----------------------------------------------------------------------
    void Polyline::insertKeyPoint( size_t index, Real lastDis )
    {
        checkKeyPointIndexValid(index);
        if ( index != ( getKeyPointCount() - 1 ) )
        {
            PolylinePoint insertPoint;
            PolylinePoint& currPoint =  getRefKeyPoint(index);
            PolylinePoint& nextPoint =  getRefKeyPoint(index+1);
            insertPoint.position = currPoint.position.midPoint(nextPoint.position);
            insertKeyPoint( insertPoint, index );
        }else
        {
            addKeyPoint( lastDis );
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::insertKeyPoint( const PolylinePoint&  parameters, size_t index )
    {
        checkKeyPointIndexValid(index);

        size_t n = 0;
        KeyPointList::iterator i = mKeyPoints.begin();
        for(; i != mKeyPoints.end(); ++i, ++n)
        {
            if (n == index)
            {
                ++i;
                break;
            }
        }

        //notice now
        //???
        if ( i == mKeyPoints.end() )
            mKeyPoints.push_back( parameters );
        else
            mKeyPoints.insert(i, parameters);

        fireAdd(index+1);
    }

    //-----------------------------------------------------------------------
    void Polyline::removeKeyPoint( size_t index )
    {
        checkKeyPointIndexValid(index);

        size_t n = 0;
        for(KeyPointList::iterator i = mKeyPoints.begin(); i != mKeyPoints.end(); ++i, ++n)
        {
            if (n == index)
            {
                mKeyPoints.erase(i);
                break;
            }
        }

        fireRemove(index);
    }

    //-----------------------------------------------------------------------
    void Polyline::setKeyPointPosition( const Vector3& point, size_t index )
    {
        checkKeyPointIndexValid(index);
        PolylinePoint& refpoint = getRefKeyPoint(index);
        refpoint.position = point;

        fireChanged(index);
    }

    //-----------------------------------------------------------------------
    void Polyline::setKeyPointNormal( const Vector3& normal, size_t index )
    {
        checkKeyPointIndexValid(index);
        PolylinePoint& refpoint = getRefKeyPoint(index);
        refpoint.normal = normal;

        fireChanged(index);
    }

    //-----------------------------------------------------------------------
    void Polyline::setKeyPointType( PolylinePoint::PolylinePointType type, size_t index, Bool isCover )
    {
        checkKeyPointIndexValid(index);
        PolylinePoint& refpoint = getRefKeyPoint(index);

        if (isCover)
            refpoint.type = type;
        else
            refpoint.type = static_cast<PolylinePoint::PolylinePointType>( refpoint.type | type );

        fireChanged(index);
    }

    //-----------------------------------------------------------------------
    Bool Polyline::checkKeyPointIndexValid(size_t index)
    {
        if ( (index >= getKeyPointCount()) || (index < 0) )
        {
            MG_LOG( out_error, "%s error, Polyline::checkKeyPointIndexValid\n", __MG_FUNC__ );
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------
    PolylinePoint& Polyline::getRefKeyPoint(size_t index)
    {
        checkKeyPointIndexValid(index);

        size_t n = 0;
        KeyPointList::iterator i = mKeyPoints.begin();
        for(; i != mKeyPoints.end(); ++i, ++n)
        {
            if (n == index)
                break;
        }
        return (*i);
    }

    //-----------------------------------------------------------------------
    void Polyline::setAutoGeneratedNormalDirection(AutoGeneratedNormalDirection type)
    {
        if (mAutoGeneratedNormalDirection != type)
        {
            mAutoGeneratedNormalDirection = type;
        }
    }

    //-----------------------------------------------------------------------
    Polyline::AutoGeneratedNormalDirection Polyline::getAutoGeneratedNormalDirection(void)
    {
        return mAutoGeneratedNormalDirection;
    }

    //-----------------------------------------------------------------------
    void Polyline::setAutoGeneratedNormalType(AutoGeneratedNormalType type)
    {
        if (mAutoGeneratedNormalType != type)
        {
            mAutoGeneratedNormalType = type;
        }
    }	

    //-----------------------------------------------------------------------
    void Polyline::setAutoGeneratedNormalEnable(Bool enable)
    {
        if (mAutoGeneratedNormalEnable != enable)
        {
            mAutoGeneratedNormalEnable = enable;
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::setAutoGeneratedNormalScaleEnable(Bool enable)
    {
        if (mAutoGeneratedNormalScaleEnable != enable)
        {
            mAutoGeneratedNormalScaleEnable = enable;
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::updateAllNormal()
    {
        buildAllNormal();
    }

    //-----------------------------------------------------------------------
    void Polyline::buildNormal( size_t index, Bool isEffectSideNormal )
    {
        checkKeyPointIndexValid( index );
        PolylinePoint& refpoint = getRefKeyPoint( index );
        if ( refpoint.type & PolylinePoint::KP_INDEPENDENT )
            return;

        if ( !mAutoGeneratedNormalEnable )
            return;

        switch ( mAutoGeneratedNormalType )
        {
        case NORT_BISECTOR:
            buildBisectNormal( index, isEffectSideNormal );
            break;
        case NORT_VERTICALNEXT:
            break;
        case NORT_VERTICALPRIOR:
            break;
        }

        if ( mAutoGeneratedNormalScaleEnable )
            buildNormalScale( index );
    }

    //-----------------------------------------------------------------------
    void Polyline::buildAllNormal()
    {
        for(size_t i = 0; i < mKeyPoints.size(); ++i)
        {
            if ( (i == 0) || (i ==  (mKeyPoints.size() - 1)) )
                buildNormal(i, true);
            else
                buildNormal(i, false);
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::buildNormalScale( size_t index )
    {
        //TODO: 需要修改单独计算bug

        checkKeyPointIndexValid(index);
        size_t keyPointCount = getKeyPointCount();
        if ( keyPointCount <= 1 )
            return;

        PolylinePoint& refpoint = getRefKeyPoint(index);

        size_t minindex = 0;
        size_t maxindex = keyPointCount - 1;
        size_t sideIndex ;

        if ( index == minindex )
            sideIndex = index + 1;
        else
            sideIndex = index - 1;

        PolylinePoint& sideRefpoint  = getRefKeyPoint( sideIndex );
        Vector3 line = refpoint.position - sideRefpoint.position;
        line.normalise();

        Vector3 verticalVector;
        Real n;
        verticalVector = MGEngineUtility::getNormaliseVerticalVector( line );
        n = MGEngineUtility::getVectorProjection( refpoint.normal, verticalVector );
        Real s = 0;
        if ( n != 0.0 )
            s =  abs ( 1 / n ); 
        refpoint.normalscale = Math::Clamp(s, (Real)1.0, (Real)5.0);

    }

    //-----------------------------------------------------------------------
    void Polyline::buildBisectNormal(size_t index, Bool isEffectSideNormal)
    {
        checkKeyPointIndexValid(index);
        PolylinePoint& refpoint = getRefKeyPoint(index);
        size_t keyPointCount = getKeyPointCount();
        size_t minindex = 0;
        size_t maxindex = keyPointCount - 1;

        if ( keyPointCount <= 1 ) 
            return;

        if ( (index == minindex) || (index == maxindex) ) 
        {
            size_t sideIndex = 0; 
            if ( index == minindex )
                sideIndex = index + 1;
            else
                sideIndex = index - 1;

            PolylinePoint& sideRefpoint  = getRefKeyPoint( sideIndex );
            Vector3 line = refpoint.position - sideRefpoint.position;
            Vector3 result;

            result = MGEngineUtility::getNormaliseVerticalVector( line );

            refpoint.normal = result;

            adjustNormalDirection( index );

            if ( isEffectSideNormal )
            {
                buildNormal( sideIndex, false );
            }
        }
        else
        {
            PolylinePoint& priorRefpoint = getRefKeyPoint(index-1);
            PolylinePoint& nextRefpoint  = getRefKeyPoint(index+1);

            Vector3 line1 = refpoint.position - priorRefpoint.position;
            Vector3 line2 = refpoint.position - nextRefpoint.position;
            line1.normalise();
            line2.normalise();

            refpoint.normal = MGEngineUtility::getNormaliseBisectVector( line1, line2 );

            adjustNormalDirection( index );

            if ( isEffectSideNormal )
            {
                buildNormal( index + 1, false );
                buildNormal( index - 1, false );
            }
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::adjustNormalDirection( size_t index )
    {
        checkKeyPointIndexValid(index);
        size_t keyPointCount = getKeyPointCount();
        if ( keyPointCount < 1 )
            return;

        PolylinePoint& refpoint = getRefKeyPoint(index);

        size_t minindex = 0;
        size_t maxindex = keyPointCount - 1;
        size_t sideIndex ;

        if (minindex < maxindex)
        {
            Vector3 line1;
            Vector3 line2;
            if ( index == minindex )
            {
                sideIndex = index + 1;
                PolylinePoint& sideRefpoint  = getRefKeyPoint( sideIndex );
                line1 = sideRefpoint.position - refpoint.position;
                line2 = refpoint.normal + line1;

            }else
            {
                sideIndex = index - 1;
                PolylinePoint& sideRefpoint  = getRefKeyPoint( sideIndex );
                line1 = refpoint.position - sideRefpoint.position;
                line2 = refpoint.normal + line1;
            }

            Vector3 cp = line1.crossProduct( line2 );
            Real result;
            switch (mAutoGeneratedNormalDirection)
            {
            case NORT_XZ_LEFTDIR: 
                result = cp.y;
                if ( result > 0 )
                    refpoint.normal = -refpoint.normal;
                break;
            case NORT_XZ_RIGHTDIR:
                result = cp.y;
                if ( result < 0 )
                    refpoint.normal = -refpoint.normal;
                break;
            case NORT_XY_LEFTDIR: 
                result = cp.z;
                if ( result > 0 )
                    refpoint.normal = -refpoint.normal;
                break;
            case NORT_XY_RIGHTDIR:
                result = cp.z;
                if ( result < 0 )
                    refpoint.normal = -refpoint.normal;
                break;
            case NORT_ZY_LEFTDIR: 
                result = cp.x;
                if ( result > 0 )
                    refpoint.normal = -refpoint.normal;
                break;
            case NORT_ZY_RIGHTDIR:
                result = cp.x;
                if ( result < 0 )
                    refpoint.normal = -refpoint.normal;
                break;
            }
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::addListener(PolylineListener* lisn)
    {
        mListeners.push_back(lisn);
    }

    //-----------------------------------------------------------------------
    void Polyline::removeListener(PolylineListener* lisn)
    {
        mListeners.remove(lisn);
    }

    //-----------------------------------------------------------------------
    std::vector<PolylineListener*> Polyline::getAllListeners(void)
    {
        std::vector<PolylineListener*> listeners;

        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            listeners.push_back((*i));
        }

        return listeners;
    }

    //-----------------------------------------------------------------------
    void Polyline::fireClear(void)
    {
        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            (*i)->onClear(this);
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::fireRebuild(void)
    {
        buildAllNormal();

        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            (*i)->onRebuild(this);
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::fireAdd(size_t index)
    {
        buildNormal(index, true);

        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            (*i)->onAdd(this, index);
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::fireRemove(size_t index)
    {
        buildNormal(index, true);

        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            (*i)->onRemove(this, index);
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::fireChanged(size_t index)
    {
        buildNormal(index, true);

        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            (*i)->onChanged(this, index);
        }
    }

    //-----------------------------------------------------------------------
    void Polyline::fireDestroy()
    {
        for(PolylineListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        {
            (*i)->onDestroy(this);
        }
    }
        
}