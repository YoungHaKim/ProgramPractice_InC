
#define _CRT_SECURE_NO_WARNINGS


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")


void server();

//function specific for Windows-to be called before invoking call to socket()
int init()
{
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	return 0;
}
void clean()
{
	WSACleanup();
}

int main()
{
	while (1)
	{
		init();
		server();
		clean();
	}

	return 0;
}

void server()
{
	SOCKET connectSocket;

	int sock, connected, bytes_recieved, true = 1;
	char send_data[1024], recv_data[1024];

	struct sockaddr_in server_addr, client_addr;
	int sin_size;

	if ((connectSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(connectSocket, SOL_SOCKET, SO_REUSEADDR, "1", sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	//bzero(&(server_addr.sin_zero),8); --This is for POSIX based systems
	memset(&(server_addr.sin_zero), 0, 8);
	if (bind(connectSocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("Unable to bind");
		exit(1);
	}

	if (listen(connectSocket, 5) == -1)
	{
		perror("Listen");
		exit(1);
	}

	printf("\n\nMyHTTPServer waiting on port 8000");
	fflush(stdout);


	sin_size = sizeof(struct sockaddr_in);

	connected = accept(connectSocket, (struct sockaddr *)&client_addr, &sin_size);


	// printf("\n I got a connection from (%s , %d)",
	//   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

	char kk[500];
	recv(connected, kk, sizeof(kk), 0);
	printf("\n Received:%s", kk);

	char mainContent[] = "<html><body>Hello everybody! welcome to my webserver! Horray<BR><BR>blablab123456789</body></html>";

	int contentLen = strlen(mainContent);

	char xx[999];
	sprintf(xx, "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: text/html\n\n%s", contentLen, mainContent);

	int c = send(connected, xx, sizeof(xx), 0);
	printf("\nSTATUS:%d", c);

	printf("\nSent : %s\n", xx);

	closesocket(connectSocket);
}