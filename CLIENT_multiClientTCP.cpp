#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 9990

int main(){
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in srv{};
    srv.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    srv.sin_family= AF_INET;
    srv.sin_port =htons(PORT);


    int conn = connect(sock, (sockaddr*)&srv, sizeof(srv));

    if(conn<0){
        perror("error : ");
    }
    else{
        cout<<" Connected to CHAT server \n";
    }



    fd_set table_client;
    while(1){
        FD_ZERO(&table_client);

        FD_SET(0, &table_client);
        FD_SET(sock,&table_client);


        int maxfd = sock;

        select(maxfd+1, &table_client, NULL, NULL , NULL);


        //USER INPUT
        if(FD_ISSET(0,&table_client)){
            char buffer[1024]={};
            cin.getline(buffer, sizeof(buffer));

            if(strcmp(buffer, "exit")==0)
            break;

            send(sock,buffer,strlen(buffer),0);

        }



        //SERVER MEssage
        if(FD_ISSET(sock,&table_client)){
            char buffer[1024]={};
            int n = recv(sock,buffer,sizeof(buffer),0);

            if(n<=0){
                cout<<"Server Discontinued\n";
                break;
            }

            cout<< buffer;
        }
    }


}