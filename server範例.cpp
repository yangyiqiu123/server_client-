#include <winsock.h>
#include <iostream>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define port 8080

class tcpthread {
public:
	void Main() {
		char buf[1024] = { 0 };
		for (;;) {

			//sizeof(buf) - 1 , -1 去掉最後一個
			int nlen = recv(client, buf, sizeof(buf) - 1, 0);

			// recv() 傳回實際讀到並寫入到緩衝區的 byte 數，而錯誤時傳回 -1
			if (nlen <= 0) break;

			// 告訴字串到這裡就結束了，後面的不用管
			// 可以不用一直初始化 buf
			buf[nlen] = '\0';


			if (strstr(buf, "quit") != NULL) {
				break; // 字串匹配函式，匹配到使用者傳送了"quit"
			}
			printf("You said %s\n", buf);
		}
		closesocket(client);

		delete this; // 呼叫完後，自己清理掉

	}
	int client;

};

// thread 法二
void tthread(int client) {
	char buf[1024] = { 0 };
	for (;;) {

		//sizeof(buf) - 1 , -1 去掉最後一個
		int nlen = recv(client, buf, sizeof(buf) - 1, 0);

		// recv() 傳回實際讀到並寫入到緩衝區的 byte 數，而錯誤時傳回 -1
		if (nlen <= 0) break;

		// 告訴字串到這裡就結束了，後面的不用管
		// 可以不用一直初始化 buf
		buf[nlen] = '\0';


		if (strstr(buf, "quit") != NULL) {
			break; // 字串匹配函式，匹配到使用者傳送了"quit"
		}
		printf("You said %s\n", buf);
	}
	closesocket(client);

}

int main() {

	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	// 建 server 的 socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		cout << "create sock error!\n";
		return -1;
	}

	// server socket
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	//serveraddr.sin_addr.s_addr = INADDR_ANY; 
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(serveraddr.sin_zero), 0, 8);

	int nRet = bind(sock, (sockaddr*)&serveraddr, sizeof(sockaddr));
	if (nRet < 0) {
		cout << endl << "fail to bind" << endl;
	}

	// 允許連線
	listen(sock, 5);

	for (;;) {

		// 客戶端資訊
		sockaddr_in clientaddr;
		clientaddr.sin_family = AF_INET;
		clientaddr.sin_port = htons(port);
		//serveraddr.sin_addr.s_addr = INADDR_ANY;
		clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		memset(&(clientaddr.sin_zero), 0, 8);

		int nlen = sizeof(sockaddr);
		int client = accept(sock, (sockaddr*)&clientaddr, &nlen);
		if (client <= 0)break;

		printf("accept client %d", client);
		char* ip = inet_ntoa(clientaddr.sin_addr); // 客戶端IP地址轉字串
		unsigned short cport = ntohs(clientaddr.sin_port);// 客戶端埠號
		printf("client ip: %s, port is %d\n", ip, cport); // 列印客戶端連線資訊

		// 好像也行
		/*tcpthread tp;
		tp.client = client;*/

		tcpthread* tp = new tcpthread();
		tp->client = client;

		thread sth(&tcpthread::Main, tp);

		// thread 法二
		// 利用函式tthread()
		// thread sth(tthread, client);

		sth.detach();
	}
	closesocket(sock);
	WSACleanup();
}
