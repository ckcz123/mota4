#pragma once

#pragma comment(lib,"ws2_32.lib")

class Http {
public:
	Http();
	char* get(char* host, int port, char* path, char* parameters=NULL);
	char* base64_urlencode(char* content);
	char server[200];
	int port;
private:
	HINSTANCE hInst;
	WSADATA wsaData;
	SOCKET connectToServer(char *szServerName, WORD portNum);
	int getHeaderLength(char *content);
	char* readUrl2(char *hostname, int port, char* path, char* parameters, long &bytesReturnedOut);
};