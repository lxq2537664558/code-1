//**********************************************************************************************************
#ifndef _H_IROLLINGTEXT_
#define _H_IROLLINGTEXT_
//**********************************************************************************************************
#include "MGEngineInterfacePreqs.h"
#include "IWidget.h"
//**********************************************************************************************************
//竖型列表接口
namespace MG
{
    class IRollingText
    {
    public:
        virtual IWidget*    getWidget() = 0;

        virtual void        addMsg(Str16 msg, U32 rollingTime, Byte priority) = 0;

    public: //事件
        //当所有信息都滚动完毕。
        virtual void        setEventRollingTextNone(PVOID_CALLBACK pFunc) = 0;

    };
}

//**********************************************************************************************************
#endif
//**********************************************************************************************************