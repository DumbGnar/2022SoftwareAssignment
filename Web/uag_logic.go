package main

import (
	"sync"
	"time"
)

/*读写锁和ServerBeans资源*/
var serverBeanLock sync.RWMutex
var serverBeanList map[int]ServerBean // 服务器编号 - 服务Bean

/*
@apiNote: 逻辑层入口
*/
func LogicStart() {
	/*根据配置项生成并初始化服务器队列*/
	config := GetConfigInstance()
	serverBeanLock.Lock()
	defer serverBeanLock.Unlock()
	serverBeanList = make(map[int]ServerBean)
	/*设置上级域Server*/
	serverBeanList[0] = &SipServerBase{
		ServerBase: ServerBase{
			Id:            0,
			Ip:            config.UpDomainIp,
			Port:          config.UpDomainPort,
			SuccessCounts: 0,
			FailureCounts: 0,
			Status:        StatusOffline,
			sessionMap:    make(map[int]*SessionBase),
		},
		GB28181Id: config.UpDomainGBId,
	}
	/*设置和SDK交互Server*/
	serverBeanList[1] = &HttpServerBase{
		ServerBase: ServerBase{
			Id:            1,
			Ip:            config.SDKIp,
			Port:          config.SDKPort,
			SuccessCounts: 0,
			FailureCounts: 0,
			Status:        StatusOffline,
			sessionMap:    make(map[int]*SessionBase),
		},
	}

	/*启动检查离线设备的协程*/
	go checkOnlineServer(time.Duration(config.CheckServerListCyctime))
	/*启动定期发送心跳的协程*/
	go heartbeatToServer(time.Duration(config.CheckServerListCyctime))
}

/*
@apiNote 每隔timebase时间检查一次ServerBeanlist，直接执行连接服务器方法
*/
func checkOnlineServer(timebase time.Duration) {
	for {
		serverBeanLock.RLock()
		for _, serverBean := range serverBeanList {
			temp := serverBean.GetParent()
			if temp.Status == StatusOffline {
				if temp.SuccessCounts == 0 {
					go serverBean.Connect()
				} else {
					go serverBean.ReConnect()
				}
			}
		}
		serverBeanLock.RUnlock()
		time.Sleep(timebase)
	}
}

/*
@apiNote: 每隔timebase时间检查一次ServerBeanlist，对于在线服务器直接执行心跳方法
*/
func heartbeatToServer(timebase time.Duration) {
	for {
		serverBeanLock.RLock()
		for _, serverBean := range serverBeanList {
			temp := serverBean.GetParent()
			if temp.Status == StatusOnline {
				temp.Heartbeat()
			}
		}
		serverBeanLock.RUnlock()
		time.Sleep(timebase)
	}
}

/*
@apiNote: 更改服务器状态
@param serverId 服务器编号
@param newStatus 新的状态
*/
func ChangeServerStatus(serverId int, newStatus int) {
	serverBeanLock.Lock()
	defer serverBeanLock.Unlock()
	serverBeanList[serverId].GetParent().Status = newStatus
}
