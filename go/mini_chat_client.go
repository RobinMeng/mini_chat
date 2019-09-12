package main

import (
	"fmt"
	"log"
	"net"
)

//receive data message from other user
func receive(conn net.Conn) {
	for {
		data := make([]byte, 1024)
		conn.Read(data)
		if len(string(data))==0{
			continue
		}
		log.Println(string(data))
	}
}

//init client
func init_client() {
	log.Println("please input nickname")
	var nick_name string = ""
	fmt.Scan(&nick_name)
	conn, err := net.Dial("tcp", "127.0.0.1:6666")
	if err != nil {
		log.Panic(err)
	}
	log.Println("client is startup ....")
	go receive(conn)

	for {
		log.Println("please input some message")
		var data = ""
		fmt.Scan(&data)
		data = nick_name + ": " + data
		conn.Write([]byte(data))
	}
}

func main() {
	init_client()
}
