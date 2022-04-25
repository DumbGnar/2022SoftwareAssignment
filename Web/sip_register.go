package main

import (
	"encoding/json"
	"fmt"
)

/*
#cgo CFLAGS: -I ./sip/c/
#cgo LDFLAGS: ./c/libtest.so
#include "./c/c.h"
*/
import "C"

/*
@apiNote	负责处理来自Cgo的信息
*/
func handleRegister(message SipCommand) {
	if message.OUT {
		/*说明这个信息是本模块向外发送的，不应该使用这个函数*/
		fmt.Println("信息类型不应该为out")
		return
	}
	/*Type应该由调用者已经验证过是Register了，现在验证subType*/
	/*subType表示状态码信息*/
	switch message.SubType {
	case 200:
		handle200Register(message.Info)
	case 403:
		handle403Register(message.Info)
	case 401:
		handle401Register(message.Info)
	default:
		// do nothing
	}
}

/*
@apiNote	处理来自Cgo的200消息
*/
func handle200Register(info []byte) {
	/*将指令string转换成struct*/
	var message Register200OK
	err := json.Unmarshal([]byte(info), &message)
	if err != nil {
		// 日志打印
		fmt.Println("日志输出：无法解析200OK的结构体")
		return
	}

}

/*
@apiNote	处理来自Cgo的403消息
*/
func handle403Register(info []byte) {

}

/*
@apiNote	处理来自Cgo的401消息
*/
func handle401Register(info []byte) {

}

/*设置的SubType值和含义的枚举定义*/
const (
	SIP_REGISTER_INITIAL_REGISTER int = iota //SubType = 0 发起新的注册
)

/*
@apiNote	根据项目内部的信息，调用cgo进行eXosip交互
*/
func dispatchRegister(cmd SipCommand) {
	if !cmd.OUT {
		fmt.Println("日志输出：信息类型应该为OUT")
		return
	}
	switch cmd.SubType {
	case SIP_REGISTER_INITIAL_REGISTER:
		sip_register_initial_register(cmd)
	default:
		sip_register_missing_subtype(cmd)
	}
}

func sip_register_initial_register(cmd SipCommand) {
	var params RegisterInitialRegister
	err := json.Unmarshal(cmd.Info, &params)
	if err != nil {
		fmt.Println("日志输出：解析Params有问题")
	}
	fmt.Println("Heee;")
	// fmt.Printf("%v\n", params)
	/*调用Cgo*/
	C.CGb28181_Register_BuildInitialRegister(C.CString(params.From), C.CString(params.Proxy), C.CString(params.Contact), C.int(params.Expires))
}

func sip_register_missing_subtype(cmd SipCommand) {
	fmt.Println("日志输出：无法解析的SubType = ", cmd.SubType)
}
