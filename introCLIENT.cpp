 #include <iostream>
 #include <sys/socket.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <cstring>

 using namespace std;

 int main(){

    int nret=0;
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in srv{};
    srv.sin_port=htons(9900);
    srv.sin_family=AF_INET;
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&srv.sin_zero,0,8);

    nret = connect (sock, (struct sockaddr*)&srv, sizeof(srv));

    if(nret<0){
        perror("Connect");
        return -1;
    }
    else{
        cout<<endl<<"Connected to the client ";
    }

    while(1){
        char buf[1024];

        cout << "Enter message: ";
        cin.getline(buf, sizeof(buf));

        if (strcmp(buf, "exit") == 0)
            break;

        send(sock, buf, strlen(buf), 0);
    }


    close(sock);
    cout << "Disconnected\n";

    return 0;
 }