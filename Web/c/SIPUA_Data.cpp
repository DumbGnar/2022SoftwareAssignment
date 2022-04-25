#ifndef _SIPUA_DATA_CPP_
#define _SIPUA_DATA_CPP_

#include "SIPUA_Data.hpp"


/**
 * 暂时写成全局data的形式，后续会改为全局变量
*/


/*********************************************************/
/**************************协议栈启动配置******************/
/*是SIP协议栈相关状态在总状态枚举量中的偏移，一般用+0表示成功，+x(x > 0)表示某个错误码*/
int BASE_STATUS = 0;
/*本地启动的服务信息*/
int sip_port = 5060;
int sip_transport = 0;  //0为UDP，1位TCP
/*回调函数格式定义*/
typedef int (*goFunc)(int, const char*);


/*********************************************************/
/**************************事件队列配置********************/
int MAX_EVENTS_SIZE = 128;



/**
 * @brief 获取配置文件中的信息，并配置参数
 * 
 * @param path string类型的配置文件路径
 * 
 * @return 0表示成功，否则返回错误码
*/
int readConfig(std::string path)
{
    // do nothing
    return 0;
}

#endif