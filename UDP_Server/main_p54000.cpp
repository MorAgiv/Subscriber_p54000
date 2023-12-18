#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        cout << "Can't start Winsock! " << wsOk;
        return 1;
    }

    // Create the first socket for port 54000
    SOCKET in54000 = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serverHint54000;
    serverHint54000.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint54000.sin_family = AF_INET;
    serverHint54000.sin_port = htons(54000);

    if (bind(in54000, (sockaddr*)&serverHint54000, sizeof(serverHint54000)) == SOCKET_ERROR)
    {
        cout << "Can't bind socket to port 54000! " << WSAGetLastError() << endl;
        return 1;
    }

    sockaddr_in client;
    int clientLength = sizeof(client);

    char buf[1024];

    while (true)
    {
        // Receive on port 54000
        ZeroMemory(&client, clientLength);
        ZeroMemory(buf, 1024);

        int bytesIn54000 = recvfrom(in54000, buf, 1024, 0, (sockaddr*)&client, &clientLength);
        if (bytesIn54000 != SOCKET_ERROR)
        {
            char clientIp[256];
            ZeroMemory(clientIp, 256);
            inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
            cout << "Message recv from " << clientIp << " on port 54000: " << buf << endl;
        }
    }

    // Close sockets
    closesocket(in54000);

    // Shutdown winsock
    WSACleanup();

    return 0;
}
