#include"stdafx.h"
#include <stdlib.h>
#include <string.h>

#include "lib.h"

struct Request *parse_request(const char *raw) {
	struct Request *req = NULL;
	req = (Request *)malloc(sizeof(struct Request));
	if (!req) {
		return NULL;
	}
	memset(req, 0, sizeof(struct Request));

	// Method
	size_t meth_len = strcspn(raw, " ");  // strcspn(s1,s2): quet trong chuoi s1 co su xuat hien dau tien cua 1 ki tu bat ki trong s2
	// O day muon tim ki tu " " dau tien trong chuoi request = do dai cua method request
	// memcmp(s1,s2,n): so sanh n byte dau cua chuoi s1 voi n byte dau cua chuoi s2
	if (memcmp(raw, "GET", strlen("GET")) == 0) {
		req->method = GET;
	}
	else if (memcmp(raw, "HEAD", strlen("HEAD")) == 0) {
		req->method = HEAD;
	}
	else {
		req->method = UNSUPPORTED;
	}

	raw += meth_len + 1; // di chuyen con tro den phan cua PAGE( Request-URI)

						 // Request-URI
	size_t url_len = strcspn(raw, " ");
	req->url = (char*)malloc(url_len + 1);
	if (!req->url) {
		free_request(req);
		return NULL;
	}
	memcpy(req->url, raw, url_len);
	req->url[url_len] = '\0';
	raw += url_len + 1; // move past <SP>

						// HTTP-Version
	size_t ver_len = strcspn(raw, "\r\n");
	req->version = (char*)malloc(ver_len + 1);
	if (!req->version) {
		free_request(req);
		return NULL;
	}
	memcpy(req->version, raw, ver_len);
	req->version[ver_len] = '\0';
	raw += ver_len + 2; // move past <CR><LF>

	struct Header *header = NULL, *last = NULL;
	while (raw[0] != '\r' || raw[1] != '\n') {
		last = header;
		header = (Header*)malloc(sizeof(Header));
		if (!header) {
			free_request(req);
			return NULL;
		}

		// name
		size_t name_len = strcspn(raw, ":");
		header->name = (char*)malloc(name_len + 1);
		if (!header->name) {
			free_request(req);
			return NULL;
		}
		memcpy(header->name, raw, name_len);
		header->name[name_len] = '\0';
		raw += name_len + 1; // move past :
		while (*raw == ' ') {
			raw++;
		}

		// value
		size_t value_len = strcspn(raw, "\r\n");
		header->value = (char*)malloc(value_len + 1);
		if (!header->value) {
			free_request(req);
			return NULL;
		}
		memcpy(header->value, raw, value_len);
		header->value[value_len] = '\0';
		raw += value_len + 2; // move past <CR><LF>

							  // next
		header->next = last;
	}
	req->headers = header;
	raw += 2; // move past <CR><LF>

	size_t body_len = strlen(raw);
	req->body = (char*)malloc(body_len + 1);
	if (!req->body) {
		free_request(req);
		return NULL;
	}
	memcpy(req->body, raw, body_len);
	req->body[body_len] = '\0';


	return req;
}


void free_header(struct Header *h) {
	if (h) {
		free(h->name);
		free(h->value);
		free_header(h->next);
		free(h);
	}
}


void free_request(struct Request *req) {
	free(req->url);
	free(req->version);
	free_header(req->headers);
	free(req->body);
	free(req);
}