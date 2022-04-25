package main

/*统一http请求方法编号*/
const (
	GET = iota
	POST
)

/*运行过程中的服务管理*/
const (
	StatusOffline = iota //离线或者未建立的状态
	StatusOnline         //在线或者已经建立的状态
	StatusHangup         //挂起状态
	StatusAbort          //中断状态
	StatusExiting        //退出状态
	StatusRegist         //正在注册状态
)
