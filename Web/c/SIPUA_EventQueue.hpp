#ifndef _SIPUA_EVENTQUEUE_HPP_
#define _SIPUA_EVENTQUEUE_HPP_

#include <eXosip2/eXosip.h>
#include <pthread.h>
#include <string.h>
#include "SIPUA_Stack.hpp"

/**
 * 存储未完成的eXosip事件
 * 规定服务占内存不能超过64M，最多能有200个未被free的事件
 * sizeof(eXosip_event_t) = 328
*/
extern int MAX_EVENTS_SIZE;
extern eXosip_event_t** osipEventQueue;
/*golang是多线程，因此这个事件队列是临界区*/
extern pthread_mutex_t mutex;

/*记录上一次被free的索引号，并且每次查询空闲索引都从这个值开始*/
extern int nullIndex;

// extern int BASE_STATUS;

/**
 * @brief 这个功能的初始化操作
 * 
 * @return 成功时返回0，不成功返回错误码
*/
int eventQueueInit();


/**
 * @brief 这个功能的退出操作
 * 
 * @return 成功时返回0，不成功返回错误码
*/
int eventQueueQuit();


/**
 * @brief 查询一个空闲的索引值；前提是要已经申请到锁
 * 
 * @return 返回索引值或者-1
*/
int getFreeIndex();


/**
 * @brief 将一个事件存储下来
 * 
 * @param osipEventPtr 要存储的eXosip_event指针
 * 
 * @return 成功返回0，不成功返回错误码
*/
int addQueue(eXosip_event_t *osipEventPtr);


/**
 * @brief 从事件队列中删除一个事件，成功时返回0，不成功返回错误码
 * 
 * @param index 事件队列索引值
 * 
 * @return 成功返回0，不成功返回错误码
*/
int deleteQueue(int index);


/**
 * @brief 访问事件队列索引为index的元素
 * 
 * @param index 事件队列索引值
 * 
 * @return 对应元素值（类型为eXosip_event_t*）或者NULL（比如不合理的索引值）
*/
eXosip_event_t* getElementAt(int index);

#endif