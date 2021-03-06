/******************************************************************************/
#ifndef _IEDITOROPERATION_H_
#define _IEDITOROPERATION_H_
/******************************************************************************/

#include "MGEngineInterfacePreqs.h"

/******************************************************************************/

namespace MG
{

    /******************************************************************************/
    //游戏编辑操作类
    /******************************************************************************/
    class IEditorOperation
    {
    public:
        //响应一次操作
        virtual void    execute() = NULL;
        //回退一次操作
        virtual void    rollback() = NULL;
    };


    /******************************************************************************/
    //游戏编辑操作类管理
    /******************************************************************************/
    class IEditorOperationManager
    {
    public:
        //响应一次操作
        virtual IEditorOperation*    createOneOperation() = NULL;
    };

}

/******************************************************************************/
#endif
