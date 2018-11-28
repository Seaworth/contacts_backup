#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MAXLINE 4096

int main(int argc, char** argv){
    int   sockfd, n;
    char  recvline[4096];
    char* sendline;
    char contacts[3][100] ={"zhangsan\n18883993801","lisi\n1883993802","wangwu\n18883993803"};//三个联系人
    struct sockaddr_in  servaddr;
    char serverIP[]="127.0.0.1";//服务器IP，本地通信IP:127.0.0.1

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);//服务器的端口号 
    for (int i=0; i<3;i++)
    {
            sendline=contacts[i];
            if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
                return 0;
            }

            if( inet_pton(AF_INET, serverIP, &servaddr.sin_addr) <= 0){
                printf("inet_pton error for %s\n",serverIP);
                return 0;
            }

            if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
                printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
                return 0;
            }

            printf("send msg to server: %s \n",sendline);
            //fgets(sendline, 4096, stdin);
            if( send(sockfd, sendline, strlen(sendline), 0) < 0){
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                return 0;
            }
            close(sockfd);

    }
    return 0;
}
