#include <iostream>
#include <windows.h> // Needed to set cursor positions
#include <string>
#include <conio.h>
#include <stdio.h>
#include <cstdio>
#include <iomanip>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define port 8080



// 換誰
// true 黑 false 白
bool change = true;

// 用上下左右控制，儲存游標位置
struct Pos
{
	int x = 0;
	int y = 0;
};

// 儲存琪盤狀態
// 沒琪 0
// 黑琪 1
// 白棋 2
// 游標位置 8
char game[19][19];

// 印出棋盤
void print_game(char game[19][19])
{
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			switch (game[i][j])
			{
			case '0':
				cout << "   "
					<< "·";
				break;
			case '1':
				cout << "  "
					<< "●";
				break;
			case '2':
				cout << "  "
					<< "○";
				break;
			case '8':
				cout << "  "
					<< "◎";
				break;
			}
		}
		cout << endl << endl;
	}
}

int main() {
	system("mode con cols=80 lines=70  ");
	memset(game, '0', sizeof(game));
	print_game(game);
	char a;
	Pos pos;
	Pos fpos;

	// 光標位置原本是什麼自
	char org = '0';

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
	cout << "按wasd控制，按j落子";


	while (a = _getch())
	{

		system("cls");
		cout << "按wasd控制，按j落子";


		if (a == 'j')
		{
			if (game[pos.y][pos.x] == '0') {
				if (change == true)
				{

					game[pos.y][pos.x] = '1';

					change = false;

				}
				else
				{
					game[pos.y][pos.x] = '2';
					change = true;
				}
			}

		}
		else {
			switch (a)
			{
			case 'w':
				pos.y -= 1;

				break;
			case 'a':
				pos.x -= 1;

				break;
			case 'd':
				pos.x += 1;

				break;
			case 's':
				pos.y += 1;

				break;
			default:
				break;
			}

			org = game[pos.y][pos.x];
			game[pos.y][pos.x] = '8';

		}
		if (game[fpos.y][fpos.x] != '1' && game[fpos.y][fpos.x] != '2') {
			game[fpos.y][fpos.x] = '0';
		}
		print_game(game);

		fpos = pos;

		if (a != 'j')
			game[pos.y][pos.x] = org;


		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++) {
				send(nclient, &game[i][j], 1, 0);
			}
		}

		char gg;
		recv(nclient, &gg, 1, 0);
		if (gg == 'z') {
			cout << "黑子獲勝!";
			system("pause");
			break;
		}
		if (gg == 'x') {
			cout << "白子獲勝!";
			system("pause");
			break;
		}
	}

}
