package main

import (
	"sync"
	"time"
)

type ConfigInfo struct {
	/*网关服务使用的IP*/
	GatewayIp string
	/*Gin服务启动的端口号*/
	GinPort int
	/*封装起来的MYSDK服务*/
	SDKPort int
	SDKIp   string
	/*所要连接的上级域信息(只有一个上级域)*/
	UpDomainPort int
	UpDomainIp   string
	UpDomainGBId string
	/*自己的SIP服务启动相关信息*/
	DomainGBId              string
	DefaultExpires          int
	SipPort                 int
	SIP_CHANNEL_MAX         int
	SIP_COMMAND_CHANNEL_MAX int
	/*检查服务器队列的周期*/
	CheckServerListCyctime int64
}

/*
@apiNote: 从配置文件中读取配置
*/
func (_config *ConfigInfo) ReadConfig(path string) (err error) {
	/*读取配置文件并设置里面的项*/
	_config.GatewayIp = "192.168.37.131"
	_config.GinPort = 8080
	_config.SDKIp = "127.0.0.1"
	_config.SDKPort = 13131
	_config.UpDomainIp = "192.168.37.35"
	_config.UpDomainPort = 5060
	_config.UpDomainGBId = "34020000002000000001"
	_config.CheckServerListCyctime = int64(time.Second)
	_config.DomainGBId = "34020000001110000001"
	_config.SipPort = 5061
	_config.DefaultExpires = 3600
	_config.SIP_CHANNEL_MAX = 128
	_config.SIP_COMMAND_CHANNEL_MAX = 128

	//do nothing
	err = nil
	return
}

/*用于执行一次函数的控制者*/
var once sync.Once

/*配置项*/
var _config *ConfigInfo

/*获取配置struct的单例*/
func GetConfigInstance() *ConfigInfo {
	once.Do(func() {
		_config = new(ConfigInfo)
	})
	return _config
}
