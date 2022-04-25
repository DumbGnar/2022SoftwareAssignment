package main

import (
	"encoding/json"
	"fmt"
	"sync"
)

//=========================================================
/*会话Session基本属性*/
type SessionBase struct {
	Id         int    //会话ID
	Type       int    //会话类型
	ServerIp   string //服务器IP地址
	ServerPort int    //服务器端口号
	ConnIp     string //会话客户端(一般为网关服务)IP地址
	ConnPort   int    //会话客户端端口号
	Status     int    //会话状态
}

func (sess *SessionBase) Connect() {

}
func (sess *SessionBase) DisConnect() {

}
func (sess *SessionBase) ReConnect() {

}
func (sess *SessionBase) Heartbeat() {

}
func (sess *SessionBase) GetParent() *SessionBase {
	return sess
}

type SessionBean interface {
	Connect()
	DisConnect()
	ReConnect()
	Heartbeat()
	GetParent() *SessionBase
}

//=========================================================
/*服务器Server基本属性*/
type ServerBase struct {
	Id            int    //服务器标识号
	Ip            string //服务器IP地址
	Port          int    //服务器端口号
	SuccessCounts int    //成功的操作数
	FailureCounts int    //失败的操作数
	Status        int    //服务连接状态，后边改为枚举

	sessionMutex sync.RWMutex         //会话Map资源对应的锁
	sessionMap   map[int]*SessionBase //会话ID - 会话Bean
}

func (server *ServerBase) Connect() {
	fmt.Println("[Warning]  Trying to connect a base server: ", server.Id)
}
func (server *ServerBase) DisConnect() {

}
func (server *ServerBase) ReConnect() {

}
func (server *ServerBase) Heartbeat() {

}
func (server *ServerBase) GetParent() *ServerBase {
	return server
}

/*子类SipServer，用来表示上级域的SIP Server*/
type SipServerBase struct {
	ServerBase        //匿名方式实现类嵌套
	GB28181Id  string //服务器的国标编号
}

func (server *SipServerBase) Connect() {
	config := GetConfigInstance()
	fmt.Println("Connecting to ", server.Id)
	/*生成一条指令给SipCommandChannel*/
	params := RegisterInitialRegister{
		From:    fmt.Sprintf("%s@%s:%d", config.DomainGBId, server.Ip, server.Port),
		Proxy:   fmt.Sprintf("%s@%s:%d", config.DomainGBId, server.Ip, server.Port),
		Contact: fmt.Sprintf("<%s@%s:%d>", config.DomainGBId, config.GatewayIp, config.SipPort),
		Expires: config.DefaultExpires,
	}
	jsonStr, _ := json.Marshal(&params)
	command := SipCommand{
		Type:    SIP_REGISTER,
		OUT:     true,
		SubType: SIP_REGISTER_INITIAL_REGISTER,
		Info:    jsonStr,
	}
	command.SendToChannel()

	/*发送完指令后应该让逻辑层logic更改服务器状态为Registering*/
	ChangeServerStatus(server.Id, StatusRegist)
}
func (server *SipServerBase) DisConnect() {

}
func (server *SipServerBase) ReConnect() {

}
func (server *SipServerBase) Heartbeat() {

}
func (server *SipServerBase) GetParent() *ServerBase {
	return &server.ServerBase
}

/*子类HttpServer，用来表示和SDK的Gin Server*/
type HttpServerBase struct {
	ServerBase //匿名方式实现类嵌套
}

func (server *HttpServerBase) Connect() {
	fmt.Println("Connecting to ", server.Id)
}
func (server *HttpServerBase) DisConnect() {

}
func (server *HttpServerBase) ReConnect() {

}
func (server *HttpServerBase) Heartbeat() {

}
func (server *HttpServerBase) GetParent() *ServerBase {
	return &server.ServerBase
}

type ServerBean interface {
	Connect()
	DisConnect()
	ReConnect()
	Heartbeat()
	GetParent() *ServerBase
}
