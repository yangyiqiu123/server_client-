#include <winsock.h>
#include <iostream>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define port 8080

class tcpthread {
public:
	int client;

	void Main() {

		char buf;
		char lastgame[19][19];
		char game[19][19];
		int i = 0;
		int j = 0;

		int verpos_i = 0;
		int verpos_j = 0;

		// 橫豎撇奈計數器
		int shu = 1, heng = 1, pie = 1, na = 1;

		for (;;) {

			//sizeof(buf) - 1 , -1 去掉最後一個
			int nlen = recv(client, &buf, 1, 0);
			if (nlen <= 0) break;

			game[i][j] = buf;
			j += 1;
			if (j == 19) {
				j = 0;
				i += 1;
			}

			if (i == 19) {
				/*for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						printf("%c ", game[i][j]);
						if (j == 18) {
							cout << endl;
						}
					}
					if (i == 18) {
						cout << endl;
					}
				}*/


				// 判斷是否獲勝
				// 找與上一手不同的棋，從這判斷
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						if (lastgame[i][j] != game[i][j]) {
							verpos_i = i;
							verpos_j = j;
						}
						lastgame[i][j] = game[i][j];
					}
				}

				// 上
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i - i][verpos_j] && game[verpos_i - i][verpos_j ] != '0') {
						shu++;

					}
					else {
						break;
					}
				}

				// 下
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i + i][verpos_j] && game[verpos_i + i][verpos_j ] != '0') {
						shu++;

					}
					else {
						break;
					}
				}
				// 左
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i][verpos_j-i] && game[verpos_i ][verpos_j - i] != '0') {
						heng++;

					}
					else {
						break;
					}
				}

				// 又
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i][verpos_j + i] && game[verpos_i ][verpos_j + i] != '0') {
						heng++;

					}
					else {
						break;
					}
				}

				// 右上
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i-i][verpos_j + i] && game[verpos_i - i][verpos_j + i] != '0') {
						pie++;

					}
					else {
						break;
					}
				}


				// 左下
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i + i][verpos_j - i] && game[verpos_i + i][verpos_j - i] != '0') {
						pie++;

					}
					else {
						break;
					}
				}

				// 右下
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i + i][verpos_j + i] && game[verpos_i + i][verpos_j + i] != '0') {
						na++;

					}
					else {
						break;
					}
				}
				
				// 左上
				for (int i = 1; i <= 4; i++) {
					if (game[verpos_i][verpos_j] == game[verpos_i - i][verpos_j - i]&& game[verpos_i - i][verpos_j - i]!='0') {
						na++;

					}
					else {
						break;
					}
				}

				if (shu == 5 || heng == 5 || pie == 5 || na == 5)
				{
					if (game[verpos_i][verpos_j] == '2')
					{
						/*cout << "黑子獲勝!";
						system("pause");
						break;*/
						send(client, "z", 1, 0);
					}
					//if (game[verpos_i][verpos_j] == '1')
					else
					{
						send(client, "x", 1, 0);

						/*cout << "白子獲勝!";
						system("pause");
						break;*/
					}
				}
				else {
					send(client, "f", 1, 0);
				}
				shu = 1, heng = 1, pie = 1, na = 1;

				// 歸0 下一盤
				i = 0;
			}
			
			/*printf("%c ", buf);
			i+=1;
			if (i == 19) {
				printf("\n");
				i = 0;
				j += 1;
			}if (j == 19) {
				printf("\n");
				j = 0;
			}*/

			
		}
		closesocket(client);

		delete this; // 呼叫完後，自己清理掉

	}

};
int main() {
	system("mode con cols=80 lines=60  ");

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

		tcpthread* tp = new tcpthread();
		tp->client = client;
		thread sth(&tcpthread::Main, tp);
		sth.detach();
	}
	closesocket(sock);
	WSACleanup();
}
