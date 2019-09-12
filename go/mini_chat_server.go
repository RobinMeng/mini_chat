package main

import (
	"log"
	"net"
)

var fd_ids = make([]interface{}, 0, 100)

func transer_message(curr_conn net.Conn) {
	for {
		data := make([]byte, 1024)
		curr_conn.Read(data)
		if len(string(data)) == 0 {
			continue
		}
		log.Println("receive data from user message:", string(data))

		for index := range fd_ids {
			var conn = fd_ids[index].(*net.TCPConn)
			if conn == curr_conn.(*net.TCPConn) {
				continue
			}
			conn.Write(data)
		}
	}
}

//初始化服务器
func init_server() {
	port := "127.0.0.1:6666"
	//监听
	listen, err := net.Listen("tcp", port)
	defer listen.Close()
	if err != nil {
		log.Fatal(err)
	}
	log.Println("mini_chat server is startup....")
	for {
		fd, err := listen.Accept()
		log.Println("welcome user:")
		if err != nil {
			log.Fatal(err)
		}
		fd_ids = append(fd_ids, fd)
		//并行接收
		go transer_message(fd)
	}
}

func main() {
	init_server()
}
