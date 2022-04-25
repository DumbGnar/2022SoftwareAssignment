#ifndef _SIPUA_STACK_HPP_
#define _SIPUA_STACK_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <eXosip2/eXosip.h>
#include "SIPUA_Data.hpp"
#include "SIPUA_Register.hpp"

/*协议栈本体*/
extern eXosip_t *excontext;

/*回调函数*/
extern goFunc addSipCommandInGolang;


/**
 * @brief 读取配置文件(现在先默认)，初始化协议栈
 * 
 * @return 执行成功返回0，执行失败返回UAG错误码
*/
int SIPUA_Init(goFunc addSipCommand);


/**
 * @brief SIP模块的总出口，由Golang退出时调用
*/
void SIPUA_End();


/**
 * @brief 协议栈初始化工作，包括实例化协议栈，开始监听端口，默认使用IPv4
 * 
 * @param port int格式，SIP服务启动的端口号
 * @param transport int格式，0为UDP，1为TCP
 * 
 * @return 执行成功返回0，执行失败返回UAG错误码（待设计）
*/
int gb28181_StackInit(int32_t port, int32_t transport);


/**
 * @brief 协议栈退出工作，建议defer执行
 * 
 */
void gb28181_StackQuit();


/**
 * @brief UAS功能，循环监听端口号是否有SIP消息，适用于开启协程
 * 
 * @return 不会返回，除非程序直接崩溃
 */
int gb28181_StackListenPort();



#endif