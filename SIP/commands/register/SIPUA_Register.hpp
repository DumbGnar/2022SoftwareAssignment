#ifndef _SIPUA_REGISTER_HPP_
#define _SIPUA_REGISTER_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <eXosip2/eXosip.h>
#include <stdlib.h>
#include "../../eventqueue/SIPUA_EventQueue.hpp"
#include "../../SIPUA_Stack.hpp"

// /*协议栈本体*/
// extern eXosip_t *excontext;

// /*错误码基址*/
// extern int BASE_STATUS;


/**
 * @brief 根据osipEventPtr类型生成指令并方法名和其他参数部分
 * 
 * @param osipEventPtr 一个eXosip_Event事件
 * 
 * @return 方法名+其他参数部分，形式为string
*/
std::string gb28181_Register_GenerateCmd(eXosip_event_t *osipEventPtr);


/**
 * @brief 构造JSON参数，详见详细设计文档
 * 
 * @param osipEventPtr 一个eXosip_Event事件
 * 
 * @return 一个表示JSON的string
*/
int gb28181_Register_GenerateJson(eXosip_event_t *osipEventPtr);


/**
 * @brief 发送第一个Register请求，适用于还没有Register ID的情况
 * 
 * @param from      发送端的SIP地址
 * @param proxy     代理人的SIP地址
 * @param contact   发送端的SIP地址
 * @param expires   本次注册维护有效时间，单位为秒
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_BuildInitialRegister(char* from, char* proxy, char* contact, int expires);


/**
 * @brief 发送一个已经存在的Register，适用于已经有了Register ID的情况
 * 
 * @param rid   已经存在的Register ID
 * @param expires   此次注册维护有效时间，单位为秒
 * 
 * @return 成功返回0，不成功返回错误码   
*/
int gb28181_Register_BuildRegister(int rid, int expires);


/**
 * @brief 发送一个带着鉴权信息的Register，适用于401之后的挑战注册，其中整个鉴权信息Authorization:xxx中的xxx都由主服务提供
 * 
 * @param rid   已经存在的Register ID
 * @param expires   此次注册维护有效时间，单位为秒
 * @param authorization 整段鉴权信息(Digest username="..." ...)
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_BuildRegisterWithAuth(int rid, int expires, char* authorization);


/**
 * @brief 发送一个带着鉴权信息的Register，适用于401之后的挑战注册，但鉴权信息只需主服务提供一些其他参数，而且仅支持MD5加密算法（eXosip协议栈的局限性）
 * 
 * @param rid   已经存在的Register ID
 * @param expires   此次注册维护有效时间，单位为秒
 * @param username  表明身份的20位国标编号
 * @param userLoginId   进行注册的用户名，一般和20国标编号一致
 * @param userLoginPasswd   进行注册的密码
 * @param algorithmName 加密算法名称，这里只支持"MD5"
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_BuildRegisterWithAutoAuth(int rid, int expires, char* username, char* userLoginId, char* userLoginPasswd, char* algorithmName);


/**
 * @brief 向注册者回复200 OK，无论成功与否，完成后从事件队列中删除元素
 * 
 * @param index 这个eXosip_event在事件队列中的索引值
 * 
 * @return  成功返回0，不成功返回错误码
*/
int gb28181_Register_Answer200OK(int index);


/**
 * @brief 向注册者发送401 Unauthorized，无论成功与否，完成后从事件队列中删除元素
 * 
 * @param index 这个eXosip_event在事件队列中的索引值
 * @param realm 存疑？？？???是注册方的前8位?
 * @param nonce 服务器生成的随机字符串
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_Answer401Unauthorized(int index, char* realm, char* nonce);

#endif