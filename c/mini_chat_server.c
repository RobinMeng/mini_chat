#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

# define BUFFSIZE 1024

int user_fds[10]={};
int user_index=0;
//转发消息
void * transfer_msg(int cli_fd);

int main(int argc,char* argv[]){
	printf("%s\n","mini_chat ...");
	pthread_t thread;
	char buff[BUFFSIZE];
	struct sockaddr_in client_addr;
	struct sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port= htons(6666);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	printf("%s\n","服务器设备已经启动成功,等待客户端链接");
	
	//1. 创建一个设备节点
	int socket_fd = socket(AF_INET,SOCK_STREAM,0);

	if(-1 == socket_fd){
		printf("%s\n","socket failed");
		return -1;
	}

	//2. 给设备绑定一个网络地址
    int bind_result=bind(socket_fd,(struct sockaddr *)&addr,sizeof(addr));
	if(-1 == bind_result){
		printf("%s\n","绑定失败..");
		return -1;
	}

	//3. 监听设备传过来的信息
	int listen_result=listen(socket_fd,100);
	if(listen_result==-1){
		printf("%s\n","listen failed");
		return -1;
	}
	while(1){
		//从socket 上接收信息
		socklen_t len=sizeof(client_addr);
		int cli_fd = accept(socket_fd, (struct sockaddr*)&client_addr,&len);	
		if(-1 == cli_fd){
			printf("%s\n","accept failed");
			return -1;
		}
		user_fds[user_index]=cli_fd;
		++user_index;
		printf("client_fd:%d online\n",cli_fd);
		pthread_create(&thread,NULL,(void *)transfer_msg,cli_fd);
		for(int i=0;i<10;i++){
			printf("current cli_fd is %d\n",user_fds[i]);
		}
	}

	return 0;
}
void * transfer_msg(int cli_fd){
	 char buff[BUFFSIZE];
         printf("welcome cli:%d  c-s is finished.. please chat\n",cli_fd);
	 while(1){
		bzero(buff,sizeof(buff));
	 	ssize_t count=recv(cli_fd,buff,BUFFSIZE,0);
		if(count>1){
			printf("recv data from cli_fd:%d msg:%s\n",cli_fd,buff);
			for(int i=0;i<10;i++){
				if(i>user_index){
					break;
				}
				if(user_fds[i]==cli_fd || 0 == user_fds[i]){
					continue;
				}
				
				send(user_fds[i], buff,sizeof(buff),0);
				printf("send message to %d msg:%s\n",user_fds[i],buff);
			}
		 }	
	}	
}











