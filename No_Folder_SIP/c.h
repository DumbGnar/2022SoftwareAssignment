#ifndef _C_H_
#define _C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <eXosip2/eXosip.h>

/**
 * 对所有文件的方法进行一层C包装
*/

/*回调函数格式定义*/
typedef int (*goFunc)(int, const char*);


/*SIPUA_Stack.hpp*/
int CSIPUA_Init(goFunc addSipCommand);
void CSIPUA_End();
int CGb28181_StackInit(int port, int transport);
void CGb28181_StackQuit();
int CGb28181_StackListenPort();

/*SIPUA_Data.hpp*/
int CReadConfig(char* path);


/*commands.register.SIPUA_Register.hpp*/
const char* CGb28181_Register_GenerateCmd(eXosip_event_t *osipEventPtr);
int CGb28181_Register_GenerateJson(eXosip_event_t *osipEventPtr);
int CGb28181_Register_BuildInitialRegister(char* from, char* proxy, char* contact, int expires);
int CGb28181_Register_BuildRegister(int rid, int expires);
int CGb28181_Register_BuildRegisterWithAuth(int rid, int expires, char* authorization);
int CGb28181_Register_BuildRegisterWithAutoAuth(int rid, int expires, char* username, char* userLoginId, char* userLoginPasswd, char* algorithmName);
int CGb28181_Register_Answer200OK(int index);
int CGb28181_Register_Answer401Unauthorized(int index, char* realm, char* nonce);

#ifdef __cplusplus
}
#endif

#endif