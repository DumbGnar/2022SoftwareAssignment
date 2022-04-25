package main

import (
	"sync"
)

func main() {

	/*读取配置文件*/
	GetConfigInstance().ReadConfig("./")

	/*进行各个模块的初始化*/
	var wg sync.WaitGroup = sync.WaitGroup{}
	wg.Add(1)
	go func() {
		SipStart()
		defer wg.Done()
	}()
	wg.Add(1)
	go func() {
		SDKStart()
		defer wg.Done()
	}()
	wg.Add(1)
	go func() {
		ErrorStart()
		defer wg.Done()
	}()
	wg.Add(1)
	go func() {
		LoggerStart()
		defer wg.Done()
	}()
	wg.Wait() //等待初始化完成

	/*启动http服务*/
	Start()
}

func Start() {
	/*启动数据层*/
	// DataStart()
	/*启动逻辑层*/
	LogicStart()
	/*启动Gin服务*/
	HttpStart()
	for {

	}
}
