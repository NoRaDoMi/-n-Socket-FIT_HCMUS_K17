// DemoProxyServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "DoAnSocket.h"
#include "lib.h"
#include <string>
/* Khai bao thu vien */
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



/* Declare function */
int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);
void usage();

#define HOST "www.fit.hcmus.edu.vn"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"

// The one and only application object

CWinApp theApp;

using namespace std;
int bESCPressed = 0;
//Ref: http://stackoverflow.com/questions/19715144/how-to-convert-char-to-lpcwstr
wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

//DWORD WINAPI onConnect(LPVOID lp) {
//	printf("\n Da tao ra 1 Thread! \n");
//	 //Nhan request tu client :D
//	CSocket *connector1 = (CSocket*)lp;
//	char request[BUFSIZ + 1];
//	memset(request, 0, sizeof(request));
//	int total = sizeof(request) - 1;
//	int received = 0;
//	do {
//		int bytes = connector1->Receive(request, BUFSIZ, 0);
//		if (bytes < 0)
//			perror("ERROR reading response from socket");
//		if (bytes == 0)
//			break;
//		received += bytes;
//	} while (received < total);
//	/* process request */
//	printf("\nResquest:\n%s\n", request);
//	
//	return 0;
//}

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			// Khoi tao thu vien Socket
			if (AfxSocketInit() == FALSE)
			{
				cout << "Khong the khoi tao Socket Libraray";
				return FALSE;
			}

			// Tạo 1 Proxy Socket Server
			CSocket proxy;
			// Mảng chứa request từ client
			char request[BUFSIZ + 1];
			//int total, received, bytes;
			if (proxy.Create(1234, SOCK_STREAM, NULL) == 0) //SOCK_STREAM or SOCK_DGRAM.
			{
				cout << "Khoi tao that bai !!!" << endl;
				cout << proxy.GetLastError();
				return FALSE;
			}
			else
			{
				cout << "Server khoi tao thanh cong !!!" << endl;

				if (proxy.Listen(3) == FALSE)
				{
					cout << "Khong the lang nghe tren port nay !!!" << endl;
					proxy.Close();
					return FALSE;
				}
			}

			CSocket connector;
			// Khoi tao mot socket de duy tri viec ket noi va trao doi du lieu

			if (proxy.Accept(connector))
			{
				cout << "Da co Client ket Noi !!!" << endl;

				/*DWORD dwOnConnect;
				HANDLE xuli;

				xuli = CreateThread(NULL, 0, onConnect, (LPVOID)&connector, 0, &dwOnConnect);*/
				//Nhan request tu client : D

				char request[BUFSIZ + 1];
				memset(request, 0, sizeof(request));
				int total = sizeof(request) - 1;
				int received = 0;
				do {
					int bytes = connector.Receive(request, BUFSIZ, 0);
					if (bytes < 0)
						perror("ERROR reading response from socket");
					if (bytes == 0)
						break;
					received += bytes;
				} while (received < total);
				/* process request */
				printf("\nResquest:\n%s\n", request);

				struct Request *req = parse_request(request);
				struct Header *h;

				if (req) {

					printf("Method: %d\n", req->method);
					printf("Request-URI: %s\n", req->url);
					printf("HTTP-Version: %s\n", req->version);
					puts("Headers:");

					for (h = req->headers; h; h = h->next) {
						printf("%32s: %s\n", h->name, h->value);
						if (h->next == NULL)
						{
							string dc(h->value);
							printf("Host bat dc: %s\n", dc);
						}
					}


					puts("message-body:");
					puts(req->body);


				}

				free_request(req);
				// Gửi request lên server :D
				////Buoc 2: Tao socket
				//		CSocket s_client;
				//		int tmpres;
				//		char *ip;
				//		char *get;
				//		char buf[BUFSIZ + 1];
				//		char *host;
				//		char *page;

				//		/*if (argc == 1)
				//		{
				//			usage();
				//			exit(2);
				//		}
				//		host = argv[1];
				//		if (argc > 2)
				//		{
				//			page = argv[2];
				//		}
				//		else
				//		{
				//			page = PAGE;
				//		}*/

				//		s_client.Create();

				//		//Buoc 3: Ket noi toi Server
				//		ip = get_ip(host);
				//		fprintf(stderr, "IP is %s\n", ip);
				//		if (s_client.Connect(convertCharArrayToLPCWSTR(ip), PORT) < 0)
				//		{
				//			perror("Could not connect");
				//			exit(1);
				//		}
				//		get = build_get_query(host, page);
				//		fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
				//		/*
				//		Stdin nó là bàn phím, standard input, nghĩa là thiết bị nhập liệu chuẩn. Tương tự
				//		Stdout là màn hình
				//		Stderr nó sẽ nối đến hệ thống ghi log để bạn có thể xem lại các lỗi hệ thống hoặc lỗi ứng dụng
				//		*/

				//		//Send the query to the server
				//		int sent = 0;
				//		while (sent < strlen(get))
				//		{
				//			tmpres = s_client.Send(get + sent, strlen(get) - sent, 0);
				//			if (tmpres == -1) {
				//				perror("Can't send query");
				//				exit(1);
				//			}
				//			sent += tmpres;
				//		}
				//		//now it is time to receive the page = /* receive the response */	
				//		memset(buf, 0, sizeof(buf)); // char buf[BUFSIZ + 1];
				//									 /*int htmlstart = 0;
				//									 char * htmlcontent;*/
				//		while ((tmpres = s_client.Receive(buf, BUFSIZ, 0)) > 0)
				//		{
				//			if (buf) {
				//				fprintf(stdout, buf);
				//			}
				//			memset(buf, 0, tmpres);
				//		}
				//		if (tmpres < 0)
				//		{
				//			perror("Error receiving data");
				//		}
				//		
				//		free(get);
				//		free(ip);
				//		s_client.Close();
			}

			connector.Close();
			proxy.Close();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}
	system("pause");
	return nRetCode;
}

void usage()
{
	fprintf(stderr, "USAGE: htmlget host [page]\n\
		\thost: the website hostname. ex: coding.debuntu.org\n\
		\tpage: the page to retrieve. ex: index.html, default: /\n");
}

/*
Get ip from domain name
*/
char *get_ip(char *host)
{
	struct hostent *hent;
	int iplen = 15; //XXX.XXX.XXX.XXX
	char *ip = (char *)malloc(iplen + 1);
	memset(ip, 0, iplen + 1); // Khởi tạo mảng ip 15 kí tự ban đầu là 0
	if ((hent = gethostbyname(host)) == NULL)
	{
		perror("Can't get IP");
		exit(1);
	}
	if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
	{
		perror("Can't resolve host");
		exit(1);
	}
	return ip;
}


char *build_get_query(char *host, char *page)
{
	char *query;
	char *getpage = page;
	char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n"; // Giá trị khởi tạo có thể là 1 giá trị biến thiên
																		  // Có 3 %s ở đây:
																		  // %s sô 1: URI–Request  (vd: /doc/test.html)
																		  // %s số 2: Tên host (vd: www.test01.com)
																		  // %s số 3: User-Agent : Mozilla/4.0
																		  // => tpl có thể xem như là 1 HTTP GET (Request Method)
	if (getpage[0] == '/') {
		getpage = getpage + 1;
		fprintf(stderr, "Removing leading \"/\", converting %s to %s\n", page, getpage);
	}
	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = (char *)malloc(strlen(host) + strlen(getpage) + strlen(USERAGENT) + strlen(tpl) - 5);
	// ? URI-Request ở đâu
	// - 5 = -6 + 1. Tức thay 3 cái %s bằng 3 cái mới  3x2 + 1 kí tự kết chuỗi
	sprintf_s(query, strlen(query) + 1, tpl, getpage, host, USERAGENT);
	// sprintf(str, "Gia tri cua Pi = %f", M_PI);
	return query;
}