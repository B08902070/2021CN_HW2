#ifndef CLIENTLIB_H
#define CLIENTLIB_H

void make_folder(int client_id);

char check_error(char* recv_data);

void client_ls(int sockfd, char* recv_data);

void client_get(int sockfd, char* recv_data, char* cmd);

void client_put(int sockfd, char* cmd);

void client_play(int sockfd, char* recv_data);	
#endif
