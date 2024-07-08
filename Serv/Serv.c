#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI Thread_f(LPVOID lp) {
    SOCKET cli_sock = *(SOCKET*)lp;
    free(lp);
    while (1) {
        char buffer[1024] = { 0 };
        int ret = recv(cli_sock, buffer, 1024, 0);
        if (ret <= 0) break;
        printf("%llu:%s\n", cli_sock, buffer);
        send(cli_sock, buffer, strlen(buffer), 0);
    }
    printf("%llu退出\n", cli_sock);
    closesocket(cli_sock);
}

int main() {
    WSADATA wasdate;
    WSAStartup(MAKEWORD(2, 2), &wasdate);
    SOCKET serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == serv_sock) {
        printf("serv socket error\n");
        return -1;
    }
    struct sockaddr_in local = { 0 };
    local.sin_family = AF_INET;
    local.sin_port = htons(8080);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(serv_sock, (struct sockaddr_in*)&local, sizeof(local));
    if (-1 == bind) {
        printf("bind error\n");
        return -1;
    }
    listen(serv_sock, 10);
    if (-1 == listen) {
        printf("listen error\n");
    }
    while (1) {
        SOCKET cli_sock = accept(serv_sock, NULL, NULL);
        if (INVALID_SOCKET == cli_sock) {
            printf("client socket error\n");
            continue;
        }
        printf("%llu已连接\n", cli_sock);
        SOCKET* sockcli = (SOCKET*)malloc(sizeof(SOCKET));
        *sockcli = cli_sock;
        CreateThread(NULL, 0, Thread_f, sockcli, 0, NULL);
    }
    return 0;
}