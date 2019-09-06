#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include<string.h>
#include<pthread.h>



void* accept_msg(int socket_fd){
	char buff[1024]={};
	
	while(1){
		 ssize_t count= recv(socket_fd,buff,sizeof(buff),0);
		 if(count>0 && buff !=NULL){
			 printf("%s\n",buff);
		 }
	}

}


int main(int argc,char ** argv){
	
	char nick_name[100]={};
	char send_buff[1024]={};
	char data_buff[2048]={};
	pthread_t thread;

	printf("%s\n","请输入您的昵称");
	scanf("%s",nick_name);
	
	int socket_fd=socket(AF_INET,SOCK_STREAM,0);
	//构建服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port= htons(6666);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	//链接服务器
	int c_result=connect(socket_fd,(struct sockaddr *)&addr,sizeof(addr));
	if (c_result==-1){
		printf("%s\n","connect failed");
		return -1;
	}
	
	pthread_create(&thread,NULL,(void *)accept_msg,socket_fd);
	while(1){
		printf("%s\n","请输入...");
		scanf("%s\n",send_buff);
		strcat(data_buff,nick_name);
		strcat(data_buff," say: ");
		strcat(data_buff,send_buff);
		send(socket_fd,data_buff,strlen(data_buff),0);
		bzero(data_buff,2048);			
	}
	
	return 0;
}
