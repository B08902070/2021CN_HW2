#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<signal.h>

#define ERR_EXIT(a){ perror(a); exit(1); }
#define CMD_LEN 128
#define FILENAME_LEN 32
#define DATA_LEN 256
int sockfd=0;

void handler(int signo);

int main(int argc, char *argv[]){
	int client_id = atoi(argv[1]);
	int ip = atoi(strtok(argv[2], ":"));
	int port = atoi(strtok(NULL, ":"));

	struct sigaction sigact;
	struct sockaddr_in addr;


	char cmd[CMD_LEN];
	char filename[FILENAME_LEN];
	char recv_data[DATA_LEN];
	int data_len;
	

	// signal handle
	sigact.sa_handler = handler;
	for(int sig=1 ; sig<=SIGRTMAX ; sig++){
		sigaction(sig, &sigact, NULL);
	}

	// make the client folder
	make_folder(client_id);

	// Get socket file descriptor
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		ERR_EXIT("client socket failed\n");
	}

	// set server address
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = htons(port);

	// build connection with server
	if(connect(sockfd, (struct sockaddr *)addr, sizeof(addr) < 0)){
		ERR_EXIT("client connect error\n");
	}

	// start sending comment
	while(true){
		// client send command to server
		if(send(sockfd, cmd, strlen(cmd), 0) < 0){
			ERR_EXIT("client send cmd error\n");	
		}

		// determine type of received message according to cmd
		if( recv(sockfd, recv_data, strlen(recv_data), 0) < 0){
			ERR_EXIT("client receive data length error\n");
		}
		
		// check error
		if(check_error(recv_data) == 1){
			continue;
		}

		// "ls" command
		if(strncmp(cmd, "ls", 2) == 0){
			client_ls(sockfd, recv_data);
		}

		// "get" command
		else if(strncmp(cmd, "get", 3) == 0){	
			client_get(sockfd, recv_data, cmd);
		}

		// "put" command
		else if(strncmp(cmd, "put", 3) == 0){	
			client_put(sockfd, cmd);
		}

		// "play" command
	}

}


void handler(int signo){
	close(sockfd);
	exit(0);
}
