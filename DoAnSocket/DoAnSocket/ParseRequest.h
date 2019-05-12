#pragma once
#include"stdafx.h"
#include <stdlib.h>
#include <string.h>

typedef struct httpRequest {
	char* host;
	char* request_uri;
};
void free_request(struct httpRequest *req);
// Ham tach Host va Request-URI tu HTTP Request cua Client
struct httpRequest *parse_Request(const char* req) {
	struct httpRequest *res = NULL;
	res = (httpRequest *)malloc(sizeof(struct httpRequest));
	if (!res) {
		return NULL;
	}
	memset(res, 0, sizeof(struct httpRequest));

	size_t meth_len = strcspn(req, " ");  // strcspn(s1,s2): quet trong chuoi s1 co su xuat hien dau tien cua 1 ki tu bat ki trong s2
										  // O day muon tim ki tu " " dau tien trong chuoi request = do dai cua method request

	req += meth_len + 1; // di chuyen con tro den phan cua PAGE( Request-URI)

						 // Request-URI
	size_t url_len = strcspn(req, " ");
	res->request_uri = (char*)malloc(url_len + 1);
	if (!res->request_uri) {
		free_request(res);
		return NULL;
	}
	memcpy(res->request_uri, req, url_len);
	res->request_uri[url_len] = '\0';

	// Tim ki tu ket dong Request line
	size_t ver_len = strcspn(req, "\r\n");
	req += ver_len + 3; // Cong 3 vi co " ","\r","n"
	// Den phan Request header , ta dong cua no tuc host
	size_t name_len = strcspn(req, ":"); // Bo qua "Host:" vi chi can gia tri cua no
	req += name_len + 1; // bo qua :
	while (*req == ' ') {
		req++;
	}

	// gia tri cua Host
	size_t value_len = strcspn(req, "\r\n");
	res->host = (char*)malloc(value_len + 1);
	if (!res->host) {
		free_request(res);
		return NULL;
	}
	memcpy(res->host, req, value_len);
	res->host[value_len] = '\0';
	return res;
}

void free_request(struct httpRequest *req) {
	free(req->host);
	free(req->request_uri);
	
}