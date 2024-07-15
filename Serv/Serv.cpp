#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

struct sockaddr_in local = { 0 };

string GetTime()
{
    time_t now = time(NULL);
    struct tm* tmVar = localtime(&now);
    char timeBuff[16] = { '\0' };
    sprintf(timeBuff, "%02d:%02d",tmVar->tm_hour, tmVar->tm_min);
    string ntimestr(timeBuff);
    return ntimestr;
}

DWORD WINAPI Thread_f(LPVOID lp) 
{
    char Buffer[1024] = { 0 };

    SOCKET cli_sock = *(SOCKET*)lp;
    free(lp);

    while (1) 
    {
        char buffer[1024] = { 0 };
        int ret = recv(cli_sock, buffer, 1024, 0);
        if (ret <= 0) break;
        string strNow = GetTime();
        sprintf(Buffer,"[%s] %llu:%s\n", strNow.c_str(), cli_sock, buffer);
        cout << Buffer;
        send(cli_sock, Buffer, strlen(Buffer), 0);
    }
    string strNow = GetTime();
    sprintf(Buffer,"[%s] %llu已退出\n", strNow.c_str(), cli_sock);
    cout << Buffer;

    closesocket(cli_sock);
    return 1;
}

int main() 
{
    WSADATA wasdate;
    WSAStartup(MAKEWORD(2, 2), &wasdate);

    char Buffer[1024] = { 0 };

    SOCKET serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    local.sin_family = AF_INET;
    local.sin_port = htons(8080);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(serv_sock, (struct sockaddr*)&local, sizeof(local));

    listen(serv_sock, 10);

    while (1) 
    {
        SOCKET cli_sock = accept(serv_sock, NULL, NULL);
        if (INVALID_SOCKET == cli_sock) 
        {
            cout << "client socket error" << endl;
            continue;
        }
        string strNow = GetTime();
        sprintf(Buffer, "[%s] %llu已连接\n", strNow.c_str(), cli_sock);
        send(cli_sock, Buffer, strlen(Buffer), 0);
        cout << Buffer;

        SOCKET* sockcli = (SOCKET*)malloc(sizeof(SOCKET));
        *sockcli = cli_sock;

        CreateThread(NULL, 0, Thread_f, sockcli, 0, NULL);
    }
    return 0;
}