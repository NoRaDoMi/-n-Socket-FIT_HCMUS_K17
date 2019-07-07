#pragma once
#pragma once

#include "PROXY_SERVER.h"
#include<string>
#include<iostream>
#include <fstream>
#include <mutex>
#include<vector>


using namespace std;

#define EXPIRED 180  // Thời gian tồn tại của 1 request cache trong CACHE
#define NTH	100

string filecachepath = "Cache/"; // Thư mục chứa cache


typedef struct _ParsedRequest
{
	string method; // GET or POST
	string path;
	string host;
	string range;
	string port;
}ParsedRequest;


typedef struct reqInfor {
	string filename;
	time_t timeout;
};



vector<reqInfor> CACHE;

void deleCache(int i) {
	remove(CACHE[i].filename.c_str());
	CACHE.erase(CACHE.begin() + i);
} // Hàm xóa 1 cache request trong CACHE

bool isEXPIRED(int i, time_t now) {
	return (time(&CACHE[i].timeout) - time(&now)) <= EXPIRED;
} // Hàm kiểm tra cache còn hạn không ? -- 3 phút

bool checkCahe(string filename, time_t now) {
	for (int i = 0; i < CACHE.size(); i++) {
		if (CACHE[i].filename == filename) {
			if (isEXPIRED(i, now)) // kiểm tra request cache còn hạn sử dụng không ?
				return true;
			else {
				deleCache(i); // neu no ton tai nhung thoi gian EXPIRED lon hon thoi gian cho phep thi xoa 
				return false;
			}
		}
	}
	return false;// neu ko ton tai thi return false
} // Hàm kiểm tra request có thể sử dụng trong CACHE không ?

string toFilename(string host, string path) {
	string t1;
	string t2;
	if (host.size() < 30)
	{
		t1 = host;
	}
	else
		t1 = host.substr(0, 30);

	if (path.size() < 30)
	{
		t2 = path;
	}
	else
		t2 = path.substr(path.size() - 30, 30);
	string res = "";

	string temp = t1 + t2;
	int i;

	// Xóa các kí tự không dùng cho đặt tên file trong Window
	while ((i = temp.find("<", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find(">", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find(":", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find("\"", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find("/", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}
	while ((i = temp.find("\\", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find("|", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find("*", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}

	while ((i = temp.find("?", 0)) != string::npos) {
		temp.replace(i, 1, "_");
	}
	res = res + temp + ".txt";
	return res;
} // Hàm tạo file name cho file chứa response cho 1 request từ client (host,path)

bool isInBlackList(string host) {
	ifstream blacklistFile;
	string line;

	size_t pos;

	blacklistFile.open("blacklist.conf");

	if (blacklistFile.fail()) {
		std::cout << "fail to open blacklist.conf" << endl;
		return false;
	}
	else
	{
		//std::cout << "\nopen success" << endl;
		while (getline(blacklistFile, line))
		{
			pos = line.find(host);
			if (pos != string::npos) {
				return true;
			}
		}
	}
	return false;
} // Hàm kiểm tra 1 Http Request có nằm trong blacklist của PROXY không ?

char *get_ip(const char *host)
{
	struct hostent *hent;
	struct in_addr** addr_list;

	//memset(hent, 0, sizeof(hent));
	int iplen = 15; //XXX.XXX.XXX.XXX
	char *ip = (char *)malloc(iplen + 1);
	memset(ip, 0, iplen + 1);
	if ((hent = gethostbyname(host)) == NULL)
	{
		printf("Can't get IP\n");
		return NULL;
	}

	addr_list = (struct in_addr**)hent->h_addr_list;
	if (addr_list[0] == NULL)
		return NULL;
	strcpy(ip, inet_ntoa(*addr_list[0]));
	return ip;
} // Hàm lấy IP từ 1 host name trong Http request

  //Ref: http://stackoverflow.com/questions/19715144/how-to-convert-char-to-lpcwstr
wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

void Build_HTTP_Requests(string &req) {

	int start = req.find("http://", 0);
	if (start == string::npos)
		return;

	int end = req.find("/", start + 7);
	req = req.erase(start, end - start);

	/*start = req.find("Accept-Encoding: ", 0);
	if (start == string::npos)
	return;

	end = req.find("\r\n", start) + 2;
	req = req.erase(start, end - start);*/

	start = req.find("Connection: ", 0);
	if (start == string::npos)
		return;

	end = req.find("\r\n", start);
	req = req.erase(start + 12, end - start - 12);
	req = req.insert(start + 12, "close");
	//return PORT;
} // Hàm build 1 Http Request để gửi lên Remote Server


bool parse_HttpRequest(string* client_request, ParsedRequest* req) {

	int startPos = -1;
	int endPos = -1;
	endPos = client_request->find(" ht", 0);

	if (endPos == string::npos)
	{
		return false;
	}

	startPos = 0;

	char *request_method = new char[endPos + 2];
	memset(request_method, 0, endPos + 2);
	memcpy(request_method, (char *)(*client_request).c_str(), endPos);

	req->method = request_method; // KIỂU REQUEST <> GET OR POST : BSS...

	startPos = client_request->find("://", endPos) + 3;
	endPos = client_request->find("/", startPos);


	char *request_host = new char[endPos - startPos + 2];
	memset(request_host, 0, endPos - startPos + 2);
	memcpy(request_host, (char *)(*client_request).c_str() + startPos, endPos - startPos);

	req->host = request_host; // HOST của REQUEST

	startPos = endPos;
	endPos = client_request->find(" HTTP/1", startPos);
	char *request_path = new char[endPos - startPos + 2];
	memset(request_path, 0, endPos - startPos + 2);
	memcpy(request_path, (char *)(*client_request).c_str() + startPos, endPos - startPos);

	req->path = request_path; // PATH của REQUEST

	startPos = client_request->find("Range: ", endPos);
	if (startPos == string::npos) // Nếu trong request không có RANGE.
	{
		delete[]request_host;
		delete[]request_method;
		delete[]request_path;
		return true;
	}
	// NÊU CÓ
	startPos += 7;
	endPos = client_request->find("\r\n", startPos);
	char * request_range = new char[endPos - startPos + 2];
	memset(request_range, 0, endPos - startPos + 2);
	memcpy(request_range, (char *)(*client_request).c_str() + startPos, endPos - startPos);

	req->range = request_range;

	delete[]request_host;
	delete[]request_method;
	delete[]request_path;
	return true;
} // Hàm parse 1 Http request 

void loadingAni() {
	cout << "  Launching  ";
	int i = 0;
	while (i < 3) {
		cout << ".";
		Sleep(500);
		i++;
	}
	cout << endl << "\a Ready to use !" << endl;
}

string buildHttpRequest(ParsedRequest req) {
	string m_RequestHeader;

	m_RequestHeader = m_RequestHeader + req.method + " " + req.path + " HTTP/1.1\r\n";
	m_RequestHeader = m_RequestHeader + "Host: " + req.host + "\r\n";
	m_RequestHeader = m_RequestHeader + "Connection: keep-alive\r\n";
	m_RequestHeader = m_RequestHeader + "User-Agent: Novasoft NetPlayer/4.0\r\n";
	//	m_RequestHeader=m_RequestHeader+"Cache-Control: max-age=0\r\n";
	m_RequestHeader = m_RequestHeader + "Accept: */*\r\n";
	//	m_RequestHeader=m_RequestHeader+"Origin:  http://222.73.105.196\r\n";
	//	m_RequestHeader=m_RequestHeader+"Cookie: saeut=61.188.187.53.1323685584721318\r\n";
	if (!req.range.empty())
	{
		m_RequestHeader = m_RequestHeader + "Range: " + req.range + "\r\n";
	}
	m_RequestHeader += "\r\n";

	return m_RequestHeader;
}


long getHttpResponseSize(string *m_ResponseHeader) {
	long nFileSize = 0;
	char szValue[10];
	int nPos = -1;
	nPos = m_ResponseHeader->find("Content-Length", 0);
	if (nPos != -1)
	{
		nPos += 16;
		int nCr = m_ResponseHeader->find("\r\n", nPos);
		memcpy(szValue, (char *)m_ResponseHeader->c_str() + nPos, nCr - nPos);
		nFileSize = atoi(szValue);
		return nFileSize;
	}
	else
	{
		std::cout << "Khong the lay do dai Response tu may chu\r\n";
		return -1;
	}
}