#include <winsock.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define port 8080

int main() {

	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		cout << endl << "fail" << endl;
	}

	// 建立 client 的 socket
	int nclient = socket(AF_INET, SOCK_STREAM, 0);
	if (nclient == -1) {
		printf("Fail to create a socket.");
	}

	// creat server socket address
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	//serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(serveraddr.sin_zero), 0, 8);

	int err = connect(nclient, (sockaddr*)&serveraddr, sizeof(sockaddr));
	if (err == -1) {
		cout << "Connection error" << endl;
	}

	char buf[256] = {};
	
	while (1) {
		cin >> buf;
		//fgets(buf, 256, stdin);
		send(nclient, buf, sizeof(buf), 0);
	}
}
