/******************************************************************************/
#ifndef _IMINIMAP_H_
#define _IMINIMAP_H_
/******************************************************************************/

#include "MGEngineInterfacePreqs.h"

/******************************************************************************/

namespace MG
{

    /******************************************************************************/
    //��Ϸ������ӿ�
    /******************************************************************************/
    class IMiniMapManager
    {
    public:

        // ����MiniMap
        virtual void	        genarateMiniMap(Vec2 center, Vec2 size, Str savePath, Vec2 mapSize) = 0;
    };

}

/******************************************************************************/
#endif