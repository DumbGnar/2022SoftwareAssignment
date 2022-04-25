#ifndef _SIPUA_REGISTER_CPP_
#define _SIPUA_REGISTER_CPP_

#include "SIPUA_Register.hpp"


/**
 * @brief 根据osipEventPtr类型生成指令并方法名和其他参数部分 (已弃用！)
 * 
 * @param osipEventPtr 一个eXosip_Event事件
 * 
 * @return 方法名+其他参数部分，形式为string
*/
std::string gb28181_Register_GenerateCmd(eXosip_event_t *osipEventPtr)
{
    std::stringstream ret;

    /*类型名*/
    ret << "REGISTER;";

    return ret.str();
}


/**
 * @brief 构造JSON参数，回调发送给golang
 * 
 * @param osipEventPtr 一个eXosip_Event事件
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_GenerateJson(eXosip_event_t *osipEventPtr)
{
    // std::stringstream ret;
    // ret << "{";

    // if(isRequest)
    // {
    //     /*提取发送者的20位国标编码*/
    //     std::string username = osipEventPtr->request->from->url->username;
    //     ret << "\"Username\":" << "\"" << username << "\"";
    //     /*尝试提取其鉴权信息*/
    //     osip_authorization_t *authorization = NULL;
    //     osip_message_get_authorization(osipEventPtr->request, 0, &authorization);
    //     int hasAuth = (authorization == NULL)? 0: 1;
    //     ret << ", \"Authorization\":" << hasAuth;
    //     /*如果有鉴权信息*/
    //     if(authorization != NULL)
    //     {
    //         /*加密串提取*/
    //         std::string response = authorization->response;
    //         ret << ", \"Response\":" << "\"" << response << "\"";
    //         /*算法名提取*/
    //         std::string algorithm = authorization->algorithm;
    //         ret << ", \"Algorithm\":" << "\"" << algorithm << "\"";
    //         /*本次注册时间*/
    //         int expires;
    //         osip_header_t* expires_header = NULL;
    //         osip_message_header_get_byname(osipEventPtr->request, "Expires", 0, &expires_header);
    //         if(expires_header != NULL && expires_header->hvalue != NULL)
    //         {
    //             expires = atoi(expires_header->hvalue);
    //             ret << ", \"Expires\":" << expires;
    //         }
    //     }
    // }
    // else
    // {       
    //     /*提取回应者的20位国标编号*/
    //     std::string username = osipEventPtr->response->from->url->username;
    //     ret << "\"Username\":" << "\"" << username << "\"";

    //     /*如果回应类型为401*/
    //     if(osipEventPtr->response->status_code == 401)
    //     {
    //         /*提取认证字符串*/
    //         osip_www_authenticate_t *wwwAuth_header = NULL;
    //         osip_message_get_www_authenticate(osipEventPtr->response, 0, &wwwAuth_header);
    //         std::string nonce = wwwAuth_header->nonce;
    //         ret << ", \"Nonce\":" << "\"" << nonce << "\"";
    //     }
    //     /*如果回应类型为200*/
    //     else if(osipEventPtr->response->status_code == 200)
    //     {
    //         /*提取Date字符串*/
    //         osip_header_t *date_header = NULL;
    //         osip_message_header_get_byname(osipEventPtr->response, "Date", 0, &date_header);
    //         ret << ", \"Date\":" << "\"" << date_header->hvalue << "\"";
    //         /*提取UAS确认的本次注册时间*/
    //         int expires;
    //         osip_header_t* expires_header = NULL;
    //         osip_message_header_get_byname(osipEventPtr->request, "Expires", 0, &expires_header);
    //         if(expires_header != NULL && expires_header->hvalue != NULL)
    //         {
    //             expires = atoi(expires_header->hvalue);
    //             ret << ", \"Expires\":" << expires;
    //         }
    //     }

    // }

    // ret << "}";
    
    eXosip_event_free(osipEventPtr);
    //构造完成后加入到golang中
    return addSipCommandInGolang(0, "No");
}


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
int gb28181_Register_BuildInitialRegister(char* from, char* proxy, char* contact, int expires)
{
    
    osip_message_t *reg = NULL;
    int registerId = -1;
    
    eXosip_lock(excontext);
    /*构建REGISTER头域*/
    registerId = eXosip_register_build_initial_register(excontext, from, proxy, contact, expires, &reg);
    if(registerId < 0)
    {
        eXosip_unlock(excontext);
        return BASE_STATUS + 9;
    }

    /*发送REGISTER信息*/
    int ret = eXosip_register_send_register(excontext, registerId, reg);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        return BASE_STATUS + 10;
    }

    eXosip_unlock(excontext);
    return registerId;
}


/**
 * @brief 发送一个已经存在的Register，适用于已经有了Register ID的情况
 * 
 * @param rid   已经存在的Register ID
 * @param expires   此次注册维护有效时间，单位为秒
 * 
 * @return 成功返回0，不成功返回错误码   
*/
int gb28181_Register_BuildRegister(int rid, int expires)
{
    osip_message_t *reg = NULL;

    eXosip_lock(excontext);
    /*根据rid构建注册信息*/
    int ret = eXosip_register_build_register(excontext, rid, expires, &reg);
    
    /*发送注册报文*/
    ret = eXosip_register_send_register(excontext, rid, reg);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        return BASE_STATUS + 10;
    }

    eXosip_unlock(excontext);
    return 0;
}


/**
 * @brief 发送一个带着鉴权信息的Register，适用于401之后的挑战注册，其中整个鉴权信息Authorization:xxx中的xxx都由主服务提供
 * 
 * @param rid   已经存在的Register ID
 * @param expires   此次注册维护有效时间，单位为秒
 * @param authorization 整段鉴权信息(Digest username="..." ...)
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_BuildRegisterWithAuth(int rid, int expires, char* authorization)
{
    osip_message_t *reg = NULL;

    eXosip_lock(excontext);
    /*根据rid构建注册信息*/
    int ret = eXosip_register_build_register(excontext, rid, expires, &reg);

    /*清除现在的鉴权信息*/
    eXosip_clear_authentication_info(excontext);
    /*添加上主服务生成的鉴权信息*/
    osip_message_set_header(reg, (char*)"Authorization", authorization);

    /*发送注册报文*/
    ret = eXosip_register_send_register(excontext, rid, reg);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        return BASE_STATUS + 10;
    }

    eXosip_unlock(excontext);
    return 0;
}


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
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_BuildRegisterWithAutoAuth(int rid, int expires, char* username, char* userLoginId, char* userLoginPasswd, char* algorithmName)
{
    osip_message_t *reg = NULL;

    eXosip_lock(excontext);
    
    /*清除现在的鉴权信息*/
    eXosip_clear_authentication_info(excontext);
    /*添加上生成的鉴权信息*/
    eXosip_add_authentication_info(excontext, username, userLoginId, userLoginPasswd, algorithmName, NULL);

    /*根据rid构建注册信息*/
    int ret = eXosip_register_build_register(excontext, rid, expires, &reg);

    /*发送注册报文*/
    ret = eXosip_register_send_register(excontext, rid, reg);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        return BASE_STATUS + 10;
    }

    eXosip_unlock(excontext);
    return 0;
}


/**
 * @brief 向注册者回复200 OK，无论成功与否，完成后从事件队列中删除元素
 * 
 * @param index 这个eXosip_event在事件队列中的索引值
 * 
 * @return  成功返回0，不成功返回错误码
*/
int gb28181_Register_Answer200OK(int index)
{
    osip_message_t *answer = NULL;
    /*获取事件队列中的eXosip_event*/
    eXosip_event_t *osipEventPtr = getElementAt(index);
    if(osipEventPtr == NULL)
    {
        return BASE_STATUS + 11;
    }

    eXosip_lock(excontext);
    /*开始构建应答报文*/
    int ret = eXosip_message_build_answer(excontext, osipEventPtr->tid, 200, &answer);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        deleteQueue(index);
        return BASE_STATUS + 12;
    }

    /*在answer中添加上校时信息，按照本地时间为准*/
    time_t timep;
    time(&timep);
    char tmp[64] = {0};
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    osip_message_set_header(answer, "Date", tmp);

    /*在answer中添加维护时间expires*/
    osip_header_t* header = NULL;
    osip_message_header_get_byname(osipEventPtr->request, "expires", 0, &header);
    if(header != NULL && header->hvalue != NULL)
    {
        osip_message_set_header(answer, "Expires", header->hvalue);
    }

    /*正式发送报文*/
    ret = eXosip_message_send_answer(excontext, osipEventPtr->tid, 200, answer);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        deleteQueue(index);
        return BASE_STATUS + 12;
    }

    eXosip_unlock(excontext);
    deleteQueue(index);
    return 0;
}


/**
 * @brief 向注册者发送401 Unauthorized，无论成功与否，完成后从事件队列中删除元素
 * 
 * @param index 这个eXosip_event在事件队列中的索引值
 * @param realm 存疑？？？???是注册方的前8位?
 * @param nonce 服务器生成的随机字符串
 * 
 * @return 成功返回0，不成功返回错误码
*/
int gb28181_Register_Answer401Unauthorized(int index, char* realm, char* nonce)
{
    osip_message_t *answer = NULL;
    /*获取事件队列中的eXosip_event*/
    eXosip_event_t *osipEventPtr = getElementAt(index);
    if(osipEventPtr == NULL)
    {
        return BASE_STATUS + 11;
    }

    eXosip_lock(excontext);
    /*开始构建应答报文*/
    int ret = eXosip_message_build_answer(excontext, osipEventPtr->tid, 401, &answer);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        deleteQueue(index);
        return BASE_STATUS + 12;
    }

    /*头部添加 WWW-Authenticate 字段*/
    std::stringstream stream;
    stream << "Digest realm=\"" << realm << "\",nonce=\"" << nonce << "\"";
    osip_message_set_header(answer, "WWW-Authenticate", stream.str().c_str());

    /*正式发送报文*/
    ret = eXosip_message_send_answer(excontext, osipEventPtr->tid, 200, answer);
    if(ret != OSIP_SUCCESS)
    {
        eXosip_unlock(excontext);
        deleteQueue(index);
        return BASE_STATUS + 12;
    }

    eXosip_unlock(excontext);
    deleteQueue(index);
    return 0;
}

#endif