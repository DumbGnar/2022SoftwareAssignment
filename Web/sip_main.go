package main

/*
正确的
#cgo CFLAGS: -I ./sip/c/
#cgo LDFLAGS: -L${SRCDIR}/libtest.so
#include "./c/c.h"
int addCmd_cgo(int t, char* str);
*/

/*
#cgo CFLAGS: -I ./sip/c/
#cgo LDFLAGS: ./c/libtest.so
#include "./c/c.h"
int addCmd_cgo(int t, char* str);
*/
import "C"
import (
	"fmt"
	"unsafe"
)

var (
	sipChannel        chan SipCommand
	sipCommandChannel chan SipCommand
)

/*
@apiNote	初始化(Do Nothing)
*/
func init() {

}

/*
@apiNote	启动函数
*/
func SipStart() {
	/*从数据中心获取自己需要的配置*/
	config := GetConfigInstance()

	/*初始化通道*/
	sipChannel = make(chan SipCommand, config.SIP_CHANNEL_MAX)
	sipCommandChannel = make(chan SipCommand, config.SIP_COMMAND_CHANNEL_MAX)

	/*启动两个关键协程*/
	go CgoCriticalGoroutine()
	go ReceiverCriticalGoroutine()

	return
}

/*
@apiNote	给C++用的回调函数
*/
//export addCmd
func addCmd(t int, cmd *C.char) int {
	/*生成指令结构体*/
	sipcommand := SipCommand{
		Type:    t,
		OUT:     false,
		SubType: 0,
		Info:    []byte(C.GoString(cmd)),
	}
	sipChannel <- sipcommand
	return 0
}

/*
@apiNote 关键协程，只有本协程可以访问cgo，其他携程通过向本协程通讯来使用cgo
*/
func CgoCriticalGoroutine() {
	/*初始化协议栈*/
	C.CSIPUA_Init((C.goFunc)(unsafe.Pointer(C.addCmd_cgo)))
	/*退出时结束协议栈*/
	defer C.CSIPUA_End()
	/*开启监听*/
	C.CGb28181_StackListenPort()
	for {
		cmd := <-sipCommandChannel
		/*处理协议栈发来的信息*/
		/*根据收到的Type来决定给那个模块的发送函数，由发送函数调用cgo*/
		switch cmd.Type {
		case SIP_REGISTER:
			dispatchRegister(cmd)
		case SIP_INVITE:

		case SIP_MESSAGE:

		case SIP_SUBSCRIBE:

		case SIP_NOTIFY:

		case SIP_ACK:

		case SIP_BYE:

		case SIP_INFO:

		default:
			/*给出警示信息*/
			fmt.Println("Warning: Unknown Type" + fmt.Sprintf("%d", cmd.Type))
		}
	}
}

/*
@apiNote 关键协程，负责处理来自协议栈回调函数返回的信息
*/
func ReceiverCriticalGoroutine() {
	for {
		message := <-sipChannel
		/*处理协议栈发来的信息*/
		/*由于不需要调用cgo，因此可以异步处理*/
		switch message.Type {
		case SIP_REGISTER:
			go handleRegister(message)
		case SIP_INVITE:

		case SIP_MESSAGE:

		case SIP_SUBSCRIBE:

		case SIP_NOTIFY:

		case SIP_ACK:

		case SIP_BYE:

		case SIP_INFO:

		default:
			/*给出警示信息*/
			fmt.Println("Warning: Unknown Type" + fmt.Sprintf("%d", message.Type))
		}
	}
}
