#ifndef _C_CPP_
#define _C_CPP_

#include "c.h"
/*对所有头文件进行一次引用*/

/*信令commands*/
/*注册register*/
#include "SIPUA_Register.hpp"

/*错误errors*/
/*事件队列eventqueue*/
/*日志logger*/
/*指令模型models*/
/*解析器parser*/

/*主路径*/
#include "SIPUA_Data.hpp"
#include "SIPUA_Stack.hpp"


/*SIPUA_Stack.hpp*/
int CSIPUA_Init(goFunc addSipCommand)
{
    return SIPUA_Init(addSipCommand);
}
void CSIPUA_End()
{
    return SIPUA_End();
}
int CGb28181_StackInit(int port, int transport)
{
    return gb28181_StackInit(port, transport);
}
void CGb28181_StackQuit()
{
    return gb28181_StackQuit();
}
int CGb28181_StackListenPort()
{
    return gb28181_StackListenPort();
}


/*SIPUA_Data.hpp*/
int CReadConfig(char* path)
{
    return readConfig(std::string(path));
}



/*commands.register.SIPUA_Register.hpp*/
const char* CGb28181_Register_GenerateCmd(eXosip_event_t *osipEventPtr)
{
    return gb28181_Register_GenerateCmd(osipEventPtr).c_str();
}
int CGb28181_Register_GenerateJson(eXosip_event_t *osipEventPtr)
{
    return gb28181_Register_GenerateJson(osipEventPtr);
}
int CGb28181_Register_BuildInitialRegister(char* from, char* proxy, char* contact, int expires)
{
    return gb28181_Register_BuildInitialRegister(from, proxy, contact, expires);
}
int CGb28181_Register_BuildRegister(int rid, int expires)
{
    return gb28181_Register_BuildRegister(rid, expires);
}
int CGb28181_Register_BuildRegisterWithAuth(int rid, int expires, char* authorization)
{
    return gb28181_Register_BuildRegisterWithAuth(rid, expires, authorization);
}
int CGb28181_Register_BuildRegisterWithAutoAuth(int rid, int expires, char* username, char* userLoginId, char* userLoginPasswd, char* algorithmName)
{
    return gb28181_Register_BuildRegisterWithAutoAuth(rid, expires, username, userLoginId, userLoginPasswd, algorithmName);
}
int CGb28181_Register_Answer200OK(int index)
{
    return gb28181_Register_Answer200OK(index);
}
int CGb28181_Register_Answer401Unauthorized(int index, char* realm, char* nonce)
{
    return gb28181_Register_Answer401Unauthorized(index, realm, nonce);
}


#endif