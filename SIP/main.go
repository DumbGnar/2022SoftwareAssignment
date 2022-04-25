package main

/*
#cgo CFLAGS: -I ./
#cgo LDFLAGS: ./libtest.so
#include "c.h"
int addCmd_cgo(int t, char* str);
*/
import "C"
import (
	"fmt"
	"unsafe"
)

var channel chan string

func main() {
	channel = make(chan string, 15)

	/*初始化协议栈*/
	C.CSIPUA_Init((C.goFunc)(unsafe.Pointer(addCmd_cgo)))
}

//export addCmd
func addCmd(t int, cmd *C.char) int {
	fmt.Println("Adding Cmd in Golang")
	channel <- C.GoString(cmd)
	fmt.Println("Adding Finished!")
	return 0
}
