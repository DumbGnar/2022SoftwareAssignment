#ifndef _SIPUA_EVENTQUEUE_CPP_
#define _SIPUA_EVENTQUEUE_CPP_

#include "SIPUA_EventQueue.hpp"

/**
 * @brief 这个功能的初始化操作
 * 
 * @return 成功时返回0，不成功返回错误码
*/
int eventQueueInit()
{
    /*申请动态数据*/
    osipEventQueue = (eXosip_event_t**)malloc(sizeof(eXosip_event_t*) * MAX_EVENTS_SIZE);
    /*初始化事件队列*/
    memset(osipEventQueue, 0, sizeof(osipEventQueue));
    /*申请互斥锁*/
    pthread_mutex_init(&mutex, NULL);

    return 0;
}


/**
 * @brief 这个功能的退出操作
 * 
 * @return 成功时返回0，不成功返回错误码
*/
int eventQueueQuit()
{

    /*free每一个非空指针*/
    for(int i = 0; i < MAX_EVENTS_SIZE; i++)
    {
        if(osipEventQueue[i] != NULL)
        {
            eXosip_event_free(osipEventQueue[i]);
        }
    }

    /*free动态数组*/
    free(osipEventQueue);

    return 0;
}


/**
 * @brief 查询一个空闲的索引值；前提是要已经申请到锁
 * 
 * @return 返回索引值或者-1
*/
int getFreeIndex()
{
    int steps, i;
    for(steps = 1, i = nullIndex; steps <= MAX_EVENTS_SIZE; i = (i+1)%MAX_EVENTS_SIZE, steps++)
    {
        if(osipEventQueue[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}



/**
 * @brief 将一个事件存储下来
 * 
 * @param osipEventPtr 要存储的eXosip_event指针
 * 
 * @return 成功返回正数索引值，不成功返回-1；一旦返回-1对应的错误码只能是BASE_STATUS+6
*/
int addQueue(eXosip_event_t *osipEventPtr)
{
    pthread_mutex_lock(&mutex);

    int index = getFreeIndex();
    if(index == -1)
    {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    pthread_mutex_unlock(&mutex);

    return index;
}


/**
 * @brief 从事件队列中删除一个事件，成功时返回0，不成功返回错误码
 * 
 * @param index 事件队列索引值
 * 
 * @return 成功返回0，不成功返回错误码
*/
int deleteQueue(int index)
{
    pthread_mutex_lock(&mutex);

    if(osipEventQueue[index] == NULL)
    {
        pthread_mutex_unlock(&mutex);
        return BASE_STATUS + 7;
    }

    /*调用协议栈的方法进行free*/
    eXosip_event_free(osipEventQueue[index]);
    /*事件队列设置*/
    osipEventQueue[index] = NULL;
    /*优化部分：下一次空闲索引从这里开始*/
    nullIndex = index;

    pthread_mutex_unlock(&mutex);

    return 0;
}


/**
 * @brief 访问事件队列索引为index的元素
 * 
 * @param index 事件队列索引值
 * 
 * @return 对应元素值（类型为eXosip_event_t*）或者NULL（比如不合理的索引值）
*/
eXosip_event_t* getElementAt(int index)
{
    if(index < 0 || index >= MAX_EVENTS_SIZE)
    {
        return NULL;
    }
    else
    {
        /*这边为了保险起见，最好用上互斥锁*/
        /*对于"读"操作，感觉不用上锁*/
        return osipEventQueue[index];
    }
}

#endif