// Proxy_Server_Final.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "afxsock.h"
#include"403_FORBIDDENRES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFFSIZ 9192

// The one and only application object

CWinApp theApp;


mutex mtx; // Sử dụng cho chặn luồng


bool isForbidden = 0;




/////////////// HÀM CHẠY LUỒNG XỬ LÍ CÁC REQUEST TỪ CLIENT //////////////////
DWORD WINAPI HandleThread(LPVOID arg)
{
	SOCKET* hConnected = (SOCKET*)arg;
	CSocket p_client;
	p_client.Attach(*hConnected);

	char c;
	int iResult;
	int totalLen = 0;
	std::string browser_request;
	char buffer[BUFFSIZ + 1] = { 0 };

	////////////////////// NHẬN HTTP REQUEST TỪ CLIENT //////////////////////////////
	do {
		iResult = p_client.Receive(buffer, BUFFSIZ, 0);
		totalLen += iResult;
		if (iResult <= 0) {
			//cout << "Thoat ujd" << endl;
			break;
		}
		buffer[iResult] = '\0';
		browser_request.append(buffer, iResult);
		if (browser_request.find("\r\n\r\n") != string::npos) {
			break;
		}
		memset(buffer, 0, BUFFSIZ + 1);
	} while (1);
	/////////////////////////////////////////////////////////////////////////////////



	std::cout << "Request: " << std::endl;
	std::cout << browser_request << endl;
	/*std::cout << "Do dai nhan: " << totalLen << std::endl;
	std::cout << "Do dai chua: " << browser_request.size() << std::endl << std::endl;*/



	ParsedRequest Parsed_Request; // Struct chứa Parsed Http Request


								  // Parse request nhận từ browser
	if (!parse_HttpRequest(&browser_request, &Parsed_Request)) {
		//cout << "Failed" << endl;
		return 0;
	} //Nếu parse không thành công

	  // Build lại 1 Http Request từ Client
	  // Chỉnh sửa: 
	  //           + Connection = close
	  //			 + Bỏ tên host trong URI-request
	Build_HTTP_Requests(browser_request);


	/*std::cout << "Request sau khi sua: " << std::endl;
	std::cout << browser_request << endl;
	std::cout << "Do dai sau khi sua: " << browser_request.size() << std::endl << std::endl;*/

	//////////////////  PROXY CHỈ HỖ TRỢ GET VÀ POST METHOD //////////////////
	if (Parsed_Request.method != "GET" && Parsed_Request.method != "POST") {
		//Chỉ xử lý GET hoặc POST
		p_client.Close();
		return 0;
	}



	//////////////// KIỂM TRA PAGE CÓ NẰM TRONG BLACKLIST //////////////////////
	if (isForbidden == 1 || isInBlackList(Parsed_Request.host)) {
		if (isForbidden == 0)
			isForbidden = 1;
		std::cout << "Ngan truy cap" << endl;
		p_client.Send(forbiddenResponse.c_str(), forbiddenResponse.size(), 0);

		p_client.Close();
		return 0;
	}



	// Tạo file name cho request
	string filename = filecachepath + toFilename(Parsed_Request.host, Parsed_Request.path);

	// KIEM TRA REQUEST TRONG CACHE
	time_t now = time(0); // Lấy thời gian hiện tại của hệ thống
	CSocket p_Server; // Socket kết nối đến server và nhận về response
	FILE * f;

	///////////////////// KIỂM TRA PROXY CACHING TRƯỚC KHI GỬI REQUEST LÊN REMOTE SERVER /////////////////////////////

	if (checkCahe(filename, now)) { //	Nếu có trong CACHE

		mtx.lock(); // Chặn các luông khác
		fopen_s(&f, filename.c_str(), "rb");
		if (f != nullptr) {
			cout << "REQUEST co trong CACHE CHO :" << filename << " va con HAN DUNG !" << endl;
			char buffer_cache[1025] = { 0 };
			while (1) {
				int len = fread_s(buffer_cache, 1025, 1, 1024, f);
				if (len == 0)
					break;
				p_client.Send(buffer_cache, len, 0);
				memset(buffer_cache, 0, 1025);
			}
			fclose(f);
			mtx.unlock();
			p_client.Close();
			return 0;
		}
	}
	else { // Nếu không thì gửi Http request vừa build lên Remote Server

		p_Server.Create(0, SOCK_STREAM); // Tạo Socket TCP

		char* ip = get_ip(Parsed_Request.host.c_str()); // Lấy địa chỉ IP từ host name request

		///////////// KẾT NỐI ĐẾN REMOTE SERVER //////////////////////
		if (p_Server.Connect(convertCharArrayToLPCWSTR(ip), 80) < 0)
		{
			std::cout << "Cannot connect to Server !" << endl;
			p_Server.Close();
			p_client.Close();
			return 0;
		}


		// Mảng chứa Http Response từ Remote server
		char * browser_req = new char[browser_request.size() + 1]; // browser_request : string nên cần chuyển sang mảng char
		char * temp = browser_req;

		memcpy_s(browser_req, browser_request.size() + 1, browser_request.c_str(), browser_request.size());
		totalLen = 0;

		//////////////// GỬI HTTP REQUEST LÊN REMOTE SERVER /////////////////////
		while (totalLen < browser_request.size()) {
			iResult = p_Server.Send(temp + totalLen, browser_request.size(), 0);
			totalLen += iResult;
			if (iResult < 0) break;
		}
		delete[]browser_req;

		//////////////// NHẬN HTTP RESPONSE TỪ REMOTE SERVER /////////////////////
		int totalSent = 0;
		int iSent;
		totalLen = 0;
		memset(buffer, 0, BUFFSIZ + 1);
		string HTTPResponse = "";

		do {
			iResult = p_Server.Receive(buffer, BUFFSIZ, 0);

			cout << "Do dai nhan " << iResult << endl;
			totalLen += iResult;
			if (iResult == 0) {
				break;
			}
			if (iResult < 0) {
				//std::cout << "Thoat kkk" << endl;
				break;
			}
			buffer[iResult] = '\0';
			iSent = 0;
			int t;
			while (iSent < iResult) {
				t = p_client.Send(buffer, iResult, 0);
				//cout << "Do dai response gui ve browser " << t << endl;
				if (t == 0) break;
				if (t < 0) {

					//std::cout << "Thoat abk" << endl;
					break;
				}

				iSent += t;
			}
			totalSent += iSent;

			HTTPResponse.append(buffer, iResult);
			memset(buffer, 0, BUFFSIZ + 1);

		} while (1);


		/////////////////// GHI NHẬN HTTP RESPONSE VÀO FILE ĐẺ LÀM CACHE CHO CÁC REQUEST TIẾP THEO /////////////////////
		mtx.lock();
		f = NULL;
		////Kiểm tra đã có luồng nào viết file chưa
		//if (fopen_s(&f, filename.c_str(), "rb") == 0) {
		//	fclose(f);
		//}

		fopen_s(&f, filename.c_str(), "wb");
		if (f != NULL) {
			fwrite(HTTPResponse.c_str(), 1, HTTPResponse.size(), f);

			// push vào CACHE
			reqInfor p;
			p.filename = filename;
			time_t now = time(0);
			p.timeout = now;

			CACHE.push_back(p);
			fclose(f);
		}
		else {
			cout << "Failed on " << filename << endl;
		}

		mtx.unlock();

	}

	p_client.Close();
	p_Server.Close();

	std::cout << "KET THUC ...\r\n";
	return 0;
}



//////////////// HÀM CHẠY LUỒNG CHÍNH QUẢN LÍ CÁC LUỒNG CON XỬ LÍ MULTIREQUEST TỪ CLIENT ////////////////////////
DWORD WINAPI MainThread(LPVOID)
{
	AfxSocketInit(NULL);
	CSocket proxy;
	int PORT = 8888;
	proxy.Create(PORT, SOCK_STREAM);

	cout << "\n  \xB2\xB2\xB2\xB2\xB2\xB2\xB2 OUR PROXY SERVER";
	cout << "\n  \xB2\xB2\xB2\xB2\xB2\xB2\xB2 IP :127.0.0.1 ";
	cout << "\n  \xB2\xB2\xB2\xB2\xB2\xB2\xB2 PORT: 8888\r\n";
	loadingAni();

	if (!proxy.Listen(SOMAXCONN)) {
		cout << "Cannot listen on this PORT !" << endl;
	}

	while (1)
	{
		CSocket p_client;
		DWORD threadID;
		HANDLE threadStatus;
		if (!proxy.Accept(p_client))
		{
			cout << "Ket noi khong thanh cong \r\n";
			continue;
		}
		/*cout << "\n \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\r\n";*/

		SOCKET* hConnected = new SOCKET();
		*hConnected = p_client.Detach();

		//Tao Thread
		threadStatus = CreateThread(NULL, 0, HandleThread, (LPVOID)hConnected, 0, &threadID);
		//_beginthread(HandleThread, 0, hConnected);

		cout << "SIZE cua cache: " << CACHE.size() << endl;

		// KIỂM TRA PAGE BỊ CẤM
		if (isForbidden == 1)
			isForbidden = 0;
	}


	return 0;
}

int main()
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
			// TODO: code your application's behavior here.
			DWORD ThreadID;
			HANDLE ThreadStatus;
			ThreadStatus = CreateThread(NULL, 0, MainThread, NULL, 0, &ThreadID);
			// Để duy trì Proxy server 
			while (1)
			{
				Sleep(10000000);
			}

		}
	}
	else
	{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}

	return nRetCode;
}
