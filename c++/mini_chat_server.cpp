#include<iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<pthread.h>
#include<unistd.h>

using namespace std;

int user_fds[10]={};
int user_index;
string ip="127.0.0.1";
int port = 6666;

void * recv_msg(int cli_fd);

int main(){
	cout << "mini_chat client ...." << endl;
	pthread_t thread;
	struct sockaddr_in client_addr;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr=inet_addr(ip.c_str());	
	
	int socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 ==socket_fd){
		cout << "socket failed ..." <<endl;
		return -1;
	}	

	int bind_r = bind(socket_fd,(struct sockaddr *)&addr,sizeof(addr));
	
	if(-1 == bind_r){
		cout << "bind failed ..."<< endl;
		return -1;
	}

	int listen_r = listen(socket_fd,1000);
	
	if(-1 ==listen_r){
		cout << "listen failed ..."<<endl;
		return -1;
	}

	cout << "server is startup,waiting client connecting" << endl;

	while(true){
		socklen_t len=sizeof(client_addr);
		int cli_fd = accept(socket_fd,(struct sockadd*)&client_addr,&len);
		if(-1 == cli_fd){
			cout << "accept failed" << endl;
			return -1;
		}	
		
		user_fds[user_index]=cli_fd;
		++user_index;
		
		cout << "client_fd:" << cli_fd << " online"<< endl;
		 
		pthread_create(&thread,NULL,(void *)recv_msg,cli_fd);
		
	}	
	
	

	return 0;

}

void * recv_msg(int cli_fd){
	cout << "welcome cli_fd:" <<cli_fd<<"please chat ..." <<endl;
	char buff[1024]={};
	while(true){
		ssize_t count=recv(cli_fd,buff,1024,0);
		if(count>1){
			cout << "recv data from" << cli_fd<< ":"<< " buff" <<   endl;
			for(int i=0; i<10;i++){
				if(i>user_index){
					break;
				}

				if(user_fds[i]==cli_fd || 0 == user_fds[0]){
					continue;
				}
				send(user_fds[i],buff,sizeof(buff),0);
				cout << "seng message to" << user_fds[i] <<" msg:" <<buff <<endl;
			}
		}
			
	}

}








