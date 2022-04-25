package main

import (
	"fmt"

	"github.com/gin-gonic/gin"
)

var router *gin.Engine

func HttpStart() {
	router = gin.Default()
	config := GetConfigInstance()
	router.Run(fmt.Sprintf(":%d", config.GinPort))
}
