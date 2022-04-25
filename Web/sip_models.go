package main

import (
	"fmt"
)

/*表示方法类型*/
const (
	SIP_REGISTER = iota
	SIP_INVITE
	SIP_MESSAGE
	SIP_SUBSCRIBE
	SIP_NOTIFY
	SIP_ACK
	SIP_BYE
	SIP_INFO
	SIP_UNKNOWN
)

/*
@apiNote	获取对应的字符串描述
@param	enumType	表示SIP方法的枚举变量
*/
func GetSipNameByEnum(enumType int) (ret string) {
	switch enumType {
	case SIP_REGISTER:
		ret = "Register"
	case SIP_INVITE:
		ret = "Invite"
	case SIP_MESSAGE:
		ret = "Message"
	case SIP_SUBSCRIBE:
		ret = "Subscribe"
	case SIP_NOTIFY:
		ret = "Notify"
	case SIP_ACK:
		ret = "ACK"
	case SIP_BYE:
		ret = "Bye"
	case SIP_INFO:
		ret = "Info"
	default:
		ret = "Unknown for type = " + fmt.Sprintf("%d", enumType)
	}
	return
}

/*统一的通道格式*/
type SipCommand struct {
	Type    int
	OUT     bool
	SubType int
	Info    []byte
}

/*
@apiNote	指令加入sipCommandChannel
*/
func (message SipCommand) SendToChannel() {
	sipCommandChannel <- message
	return
}

/*
@apiNote	打印指令信息到终端上
*/
func (message *SipCommand) Print() {
	fmt.Println("==========================")
	fmt.Println("Command: \t" + GetSipNameByEnum(message.Type))
	fmt.Println("SubType: \t" + fmt.Sprintf("%d", message.SubType))
	if message.OUT {
		fmt.Println("Direction: \tFrom UAG to eXosip stack")
	} else {
		fmt.Println("Direction: \tFrom eXosip stack to UAG")
	}
	fmt.Println("Params: \t" + string(message.Info))
	fmt.Println("==========================")
}

/*Register 用到的struct*/
/*定义从cgo回调来的信息，是上级域返回401应答的抽象*/
type Register401Unauthorized struct {
	IsRequest bool
	Status    int
	Identity  string
	Nonce     string
	Algorithm string
}

/*定义从cgo回调来的信息，是上级域返回403应答的抽象*/
type Register403Forbidden struct {
	IsRequest bool
	Status    int
	Identity  string
}

/*定义从cgo回调来的信息，是上级域返回200应答的抽象*/
type Register200OK struct {
	IsRequest bool
	Status    int
	Identity  string
	Date      string
	Expires   string
}

type RegisterInitialRegister struct {
	From    string `json:"from"`
	Proxy   string `json:"proxy"`
	Contact string `json:"contact"`
	Expires int    `json:"expires"`
}
