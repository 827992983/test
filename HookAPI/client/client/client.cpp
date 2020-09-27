﻿// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <winsock2.h>
#include <iostream>

#include <string.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")			//add ws2_32.lib
#pragma warning(disable : 4996)

const int DEFAULT_PORT = 8000;
int main(int argc, char* argv[])
{

	WORD	wVersionRequested;
	WSADATA wsaData;
	int		err, iLen;
	wVersionRequested = MAKEWORD(2, 2);//create 16bit data
									   //(1)Load WinSock
	err = WSAStartup(wVersionRequested, &wsaData);	//load win socket
	if (err != 0)
	{
		cout << "Load WinSock Failed!";
		return -1;
	}
	//(2)create socket
	SOCKET sockClt = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClt == INVALID_SOCKET) {
		cout << "socket() fail:" << WSAGetLastError() << endl;
		return -2;
	}
	//(3)IP
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(DEFAULT_PORT);

	//(5)connect
	err = connect(sockClt, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	if (err == INVALID_SOCKET)
	{
		cout << "connect() fail" << WSAGetLastError() << endl;
		return -1;
	}

	char sendBuf[1024], hostname[100];
	if (gethostname(hostname, 100) != 0)	//get host name
		strcpy_s(hostname, 100, "None");
	strcpy_s(sendBuf, 1024, hostname);
	strcat_s(sendBuf, 1024, "have connected to you!");
	err = send(sockClt, sendBuf, strlen(sendBuf) + 1, 0);

	char recvBuf[1024] = "\0";
	iLen = recv(sockClt, recvBuf, 1024, 0);
	cout << "recv() :" << iLen << endl;

	if (iLen == 0)
	{
		return -3;
	}
	else if (iLen == SOCKET_ERROR) {
		cout << "recv() fail:" << WSAGetLastError() << endl;
		return -4;
	}
	else
	{
		recvBuf[iLen] = '\0';
		cout << recvBuf << endl;
	}
	closesocket(sockClt);

	WSACleanup();
	system("PAUSE");
	return 0;
}
