#ifndef _DATA_HPP_
#define _DATA_HPP_

#include <iostream>

/**
 * 暂时写成全局data的形式，后续会改为全局变量
*/


/*********************************************************/
/**************************协议栈启动配置******************/
/*是SIP协议栈相关状态在总状态枚举量中的偏移，一般用+0表示成功，+x(x > 0)表示某个错误码*/
extern int BASE_STATUS;
/*本地启动的服务信息*/
extern int sip_port;
extern int sip_transport;  //0为UDP，1位TCP
/*回调函数格式定义*/
typedef int (*goFunc)(int, const char*);


/*********************************************************/
/**************************事件队列配置********************/
extern int MAX_EVENTS_SIZE;



/**
 * @brief 获取配置文件中的信息，并配置参数
 * 
 * @param path string类型的配置文件路径
 * 
 * @return 0表示成功，否则返回错误码
*/
int readConfig(std::string path);


#endif