#ifndef _SIPUA_STACK_CPP_
#define _SIPUA_STACK_CPP_

#include "SIPUA_Stack.hpp"


/*协议栈本体*/
eXosip_t *excontext;

/*回调函数*/
goFunc addSipCommandInGolang;


/*无限循环监听报文的线程以及对应pid*/
int sip_pid = -1;
pthread_t handle_thread;

/**
 * @brief 读取配置文件(现在先默认)，初始化协议栈
 * 
 * @param addSipCommand 一个参数列表为(int, const char*)，返回值为int的golang函数指针，用于添加SIP指令
 * 
 * @return 执行成功返回0，执行失败返回UAG错误码
*/
int SIPUA_Init(goFunc addSipCommand)
{
    /*读取配置文件，初始化全局参数*/
    readConfig(".");

    /*初始化事件队列*/
    eventQueueInit();

    /*初始化日志*/
    // 留白

    /*初始化错误模块*/
    // 留白

    /*初始化解析器*/
    // 留白

    /*初始化回调函数*/
    addSipCommandInGolang = addSipCommand;

    /*初始化协议栈*/
    return gb28181_StackInit(sip_port, sip_transport);
}


/**
 * @brief SIP模块的总出口，由Golang退出时调用
*/
void SIPUA_End()
{

    /*事件队列退出*/
    eventQueueQuit();

    /*日志退出*/
    // 留白

    /*解析器退出*/
    // 留白

    /*错误模块退出*/
    // 留白

    return gb28181_StackQuit();
}


/**
 * @brief 协议栈初始化工作，包括实例化协议栈，开始监听端口，默认使用IPv4
 * 
 * @param port int格式，SIP服务启动的端口号
 * @param transport int格式，0为UDP，1为TCP
 * 
 * @return 执行成功返回0，执行失败返回UAG错误码（待设计）
*/
int gb28181_StackInit(int32_t port, int32_t transport)
{
    int ret = 0;

    /*协议栈申请内存区*/
    excontext = eXosip_malloc();
    if(excontext == NULL)
    {
        /*不打印信息（现在只是测试），直接返回错误码*/
        return BASE_STATUS + 1;
    }

    /*协议栈初始化数据*/
    ret = eXosip_init(excontext);
    if(ret != OSIP_SUCCESS)
    {
        /*不打印信息（现在只是测试），直接返回错误码*/
        return BASE_STATUS + 2;
    }

    /*设置用户代理*/
    eXosip_set_user_agent(excontext, "eXosip/5.0.0");

    /*设置监听协议和端口号*/
    int protocol;
    switch (transport)
    {
    case 0:
        protocol = IPPROTO_UDP;
        break;
    case 1:
        protocol = IPPROTO_TCP;
        break;
    default:
        /*不打印信息（现在只是测试），直接返回错误码*/
        return BASE_STATUS + 3;
        break;
    }
    ret = eXosip_listen_addr(excontext, protocol, NULL, port, AF_INET, 0);
    if(ret != OSIP_SUCCESS)
    {
        /*不打印信息（现在只是测试），直接返回错误码*/
        return BASE_STATUS + 4;
    }


    /*后续改为日志*/
    printf("SIP Stack Initialized successfully!\n");
    return 0;
}


/**
 * @brief 协议栈退出工作，建议defer执行
 */
void gb28181_StackQuit()
{
    /*由于有另外一个线程在使用协议栈，因此应该停掉另一个线程，这个操作应该是互斥的*/
    eXosip_lock(excontext);
    pthread_cancel(handle_thread);
    eXosip_unlock(excontext);

    eXosip_quit(excontext);
}


/**
 * @brief 根据事件类型分配给不同的信令处理单元
 * 
 * @param osipEventPtr 指向事件的指针
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_DispatchEvent(eXosip_event_t* osipEventPtr)
{
    if(MSG_IS_REGISTER(osipEventPtr->request))
    {
        gb28181_Register_GenerateJson(osipEventPtr);
    }

    return 0;
}


/**
 * @brief 用于开启额外线程，DO NOT USE!
*/
void* eXosipListenEvent(void* pUser)
{

    eXosip_event_t *osipEventPtr = NULL;
    for(;;)
    {
        /*获取到来的事件*/
        osipEventPtr = eXosip_event_wait(excontext, 0, 500);
        if(osipEventPtr == NULL)
        {
            continue;
        }

        // eXosip_default_action(excontext, osipEventPtr);
        /*交给解析器来处理*/
        gb28181_DispatchEvent(osipEventPtr);

        /*事件free的工作交给解析器*/
        // eXosip_event_free(osipEventPtr);
        osipEventPtr = NULL;
    }
}


/**
 * @brief 开启一个循环监听事件的线程，每接收到一个事件，线程解析事件并回调通知Golang
*/
int gb28181_StackListenPort()
{
    /*开启一个线程，用于无限循环*/
    if((sip_pid = pthread_create(&handle_thread, NULL, eXosipListenEvent, NULL)) != 0)
    {
        // 返回错误码
        return BASE_STATUS + 998989;
    }

    return 0;
}


#endif