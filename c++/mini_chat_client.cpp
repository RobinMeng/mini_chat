#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<pthread.h>
#include<string>
using namespace std;

const string ip="127.0.0.1";
const int port=6666;

void* recv_msg(int socket_fd){
	char buff[1024]={};
	while(true){
		ssize_t count=recv(socket_fd,buff,sizeof(buff),0);
		if(count>0 && NULL != buff){
			cout << buff <<  endl;
		}

	}

}


int main(){
	
	pthread_t thread;
	string nick_name;
	
	cout << "please input nickname ..." << endl;
	cin >> nick_name;	

	int socket_fd=socket(AF_INET,SOCK_STREAM,0);
	//构建服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr=inet_addr(ip.c_str());
	//连接服务器
	int c_result = connect(socket_fd,(struct sockaddr *)&addr,sizeof(addr));
	
	if(-1==c_result){
		cout << "connect failed" << endl;
		return -1;
	}
	
	pthread_create(&thread,NULL,(void*)recv_msg,socket_fd);

	while(true){
		cout << "please input ..." << endl;
		string send_data;
		cin >> send_data;
		string data= nick_name + "say : " + send_data;
		send(socket_fd,data.c_str(),data.length(),0);
	}
	return 0;
}
