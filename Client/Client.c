#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

struct sockaddr_in target = { 0 };

int main() 
{
    WSADATA wasdate;
    WSAStartup(MAKEWORD(2, 2), &wasdate);

    SOCKET cli_sock = socket(AF_INET, SOCK_STREAM, 0);

    target.sin_family = AF_INET;
    target.sin_port = htons(8080);
    target.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(cli_sock, (struct sockaddr_in*)&target, sizeof(target));

    char Rbuffer[1024] = { 0 };
    int ret = recv(cli_sock, Rbuffer, 1024, 0);
    printf("%s", Rbuffer);

    while (1) 
    {
        char buffer[1024] = { 0 };

        printf("«Î ‰»Î£∫");
        scanf("%s", buffer);

        send(cli_sock, buffer, strlen(buffer), 0);

        char Rbuffer[1024] = { 0 };
        int ret=recv(cli_sock, Rbuffer, 1024, 0);
        if (ret <= 0) break;
        printf("%s", Rbuffer);
    }
    closesocket(cli_sock);
}