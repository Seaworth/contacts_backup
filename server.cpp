#include<iostream>
#include<sqlite3.h>
using namespace std;
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define MAXLINE 4096
#define SERVER_PORT 6666//服务器端口号

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
int main(int argc, char** argv){

    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;
    int id = 1;

    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[4096];
    int  n;  
    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if (rc == SQLITE_OK)
    {
        char sql[256] = "SELECT * FROM CONTACTS";
        char** pResult;
        int nRow;
        int nCol;
        rc = sqlite3_get_table(db, sql, &pResult, &nRow, &nCol, NULL);
        if (rc == SQLITE_OK)
        {
            if (nRow >= 1){
                cout << "now the number of contact is " << nRow << endl;
                id=nRow+1;//在此数据后追加数据
            }
        }
        sqlite3_free_table(pResult);
    }
    sqlite3_close(db);

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    //在服务端bind or listen前
    unsigned int value = 0x1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(void *)&value,sizeof(value));

    //初始化servaddr
    memset(&servaddr, 0, sizeof(servaddr));//清空结构体变量
    servaddr.sin_family = AF_INET;//主机字节序列
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY表示本机的任意IP
    servaddr.sin_port = htons(SERVER_PORT);//端口号6666

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){//最多有10个用户处于连接等待状态
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======Start backing up to database======\n");  
    
    while(1){
        //NULL 表示不关心客户端的地址
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);//buff保存传来的数据
        buff[n] = '\0';
        //printf("recv msg from client: %s\n", buff);
        close(connfd);
        char *name;//联系人的名字
        char *tel;//联系人的电话
        const char *delim = "\n";
        name = strtok(buff, delim);
        tel = strtok(NULL, delim);
        cout <<id<<endl<<"recv msg from client:" << name <<" "<<tel<<endl;



        /* Open database */
        rc = sqlite3_open("test.db", &db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            exit(0);
        }
        else{
            fprintf(stdout, "Opened database successfully\n");
        }
        /* Create SQL statement */
        char BUFF[200] = {0};
        sprintf (BUFF, "INSERT INTO CONTACTS(ID,NAME,TEL)" "VALUES (%d, '%s', '%s');", id++, name, tel);
        rc = sqlite3_exec(db, BUFF, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records created successfully\n");
        }   
        cout<<endl;
    }
    close(listenfd);
    return 0;
}
