#include"clientlib.h"
#include<sys/stat.h>
#ifndef FILENAME_LEN
#define FILENAME_LEN 32
#endif

#ifndef BUFSIZE
#define BUFSIZE 256
#endif

// create client folder
void make_folder(int client_id){
	char folder_name[32];
	sprintf(folder_name, "b08902070_%d_client_folder", client_id);
	mkdir(folder_name, 0777);
}


// return either 0 or 1(false or true) 
char check_error(char* recv_data){
	printf("%s\n", recv_data);
	if(strcmp(recv_data, "Command not found.\n") == 0){
		return 1;
	}
	if(strcmp(recv_data, "Command format error.\n") == 0){
		return 1;
	}
	if(strstr(recv_data, "doesn't exist.\n") == 0){
		return 1;
	}
	if(strstr(recv_data, "is not a mpg fil.\n") == 0){
		return 1;
	}
	return 0;
}

// "ls" command
void client_ls(int sockfd, char* recv_data){
	int data_len = atoi(recv_data);
	int recv_len;
	while(data_len > 0 && (recv_len = recv(sockfd, recv_data, strlen(recv_data), 0)) > 0){
		write(1, recv_data, recv_len);
		data_len -= recv_len;
	}
}


// "get" command
void client_get(int sockfd, char* recv_data, char* cmd){
	char filename[FILENAME_LEN];
	int data_len = atoi(recv_data);
	int recv_len;

	strtok(cmd, " ");
	while((filename = strtok(NULL, " ")) != NULL){
		if(data_len == 0){
			recv(sockfd, recv_data, strlen(recv_data), 0);
			data_len = atoi(recv_data);
		}
		
		FILE *fp;
		fp = fopen(filename, "ab+");
		while(data_len > 0){
			recv_len = recv(sockfd, recv_data, strlen(recv_data));
			fwrite(recv_data, 1, recv_len, fp);
			data_len -= recv_len;
		}
		fclose(fp);
	}
}


// "put" command
void client_put(int sockfd, char* cmd){	
	char filename[FILENAME_LEN];
	struct stat st;
	int file_len;
	char buf[BUFSIZE];

	strtok(cmd, " ");
	while((filename = strtok(NULL, " ")) != NULL){
		// check if file exists
		if(access(filename, F_OK) != 0){
			printf("The %s doesn't exist.\n", filename);
			continue;
		}
		//send filename and file length to server
		lstat(filename, &st);
		file_len = st.st_size;
		sprintf(buf, "%s %d", filename, file_len);
		send(sockfd, buf, sizeof(buf), 0);

		FILE *fp;
		fp = fopen(filename, "rb+");
		while(!feof(fp)){
			fread(buf, 1, BUFSIZE, fp);
			send(sockfd, buf, sizeof(buf), 0);
		}
	}
}


// "play" comment
void client_play(int sockfd, char* recv_data){
	
}
