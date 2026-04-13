#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 9999

using namespace std;

int main(){

    char hostname [1024]={0};
    int n = gethostname(hostname,sizeof(hostname));

    if(n==0){
        cout<<"Hostname: "<<hostname<<endl;
    }
    else{
        cout<<"Hostname Failed"<<endl;
    }


    //NAME → HINT → GET → USE → FREE
    //getaddrinfo takes hostname + hints and returns addrinfo list.
    //We extract sockaddr, convert to IP using inet_ntoa, and free using freeaddrinfo.
//int getaddrinfo(const char *node,
// const char *service,
// const struct addrinfo *hints,
// struct addrinfo **res);



    addrinfo hints{}, *res;

    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    getaddrinfo(hostname,"https", &hints, &res);

    sockaddr_in *addr = (sockaddr_in*)res->ai_addr;

    std::cout << "IP: " << inet_ntoa(addr->sin_addr) << std::endl;
    std::cout << "Port: " << ntohs(addr->sin_port) << std::endl;

    freeaddrinfo(res);




    //setScoketOPT

    int sock = socket (AF_INET, SOCK_STREAM,0);
    int opt = 1; //1-> enable , 0-> disable

    // Allows reuse of local address/port immediately after socket close.
    // Prevents "Address already in use" error during quick restarts (TIME_WAIT bypass for bind scenarios).
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));

    return 0;
}