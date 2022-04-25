package main

/*
#cgo CFLAGS: -I ./
#cgo LDFLAGS: ./libtest.so
#include "c.h"
int addCmd_cgo(int t, char* str);
*/
import "C"
import (
	"time"
	"unsafe"
)

var channel chan string

func main() {

	//所有的工作都交给专门的协程来做
	go func() {
		channel = make(chan string, 15)
		/*初始化协议栈*/
		C.CSIPUA_Init((C.goFunc)(unsafe.Pointer(C.addCmd_cgo)))
		/*退出时结束协议栈*/
		defer C.CSIPUA_End()

		/*开启监听*/
		C.CGb28181_StackListenPort()

		/*发起注册*/
		from := "sip:34020000001110000001@192.168.37.35:5060"
		proxy := "sip:34020000001110000001@192.168.37.35:5060"
		contact := "<sip:34020000001110000001@192.168.37.131:5060>"
		expires := 3600
		rid := C.CGb28181_Register_BuildInitialRegister(C.CString(from), C.CString(proxy), C.CString(contact), C.int(expires))
		<-channel

		/*默认发起第二次注册*/
		C.CGb28181_Register_BuildRegisterWithAutoAuth(rid, C.int(expires), C.CString("34020000001110000001"), C.CString("34020000001110000001"), C.CString("12345678"), C.CString("MD5"))
		<-channel

	}()

	time.Sleep(10 * time.Second)
}

//export addCmd
func addCmd(t int, cmd *C.char) int {
	channel <- C.GoString(cmd)
	return 0
}
