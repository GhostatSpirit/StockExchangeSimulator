/* This code originally was created by Jpres at YouTube.
 * Website: https://www.youtube.com/watch?v=9jLhDYtyA7c&list=PLcacUGyBsOIBFl6Q8d5XkQs7NzSlxlTKx&index=11
 * Modified by Yang Liu, lykavin@hotmail.com
 * 
 * This header file defines the Server class for the Exchange project.(server)
 */

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <Windows.h>
#include <iostream>
#include <Windows.h>
#include "Exchange.h"			// we need a pointer to exchange

// define the struct we need for passing parameters to functions
// of another thread
struct PARAMS {
	int i;						// i for the index in the SOCKET Connections array
	Exchange* p_exchange;			// pointer to the exchange object
};

enum Packet
{
	P_ChatMessage,
	P_Test
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

	// destuctor: close the handle of the mutex
	~Server() {
		CloseHandle(exchangeMutex);
	}

private:
	bool sendall(int ID, char * data, int totalbytes);
	bool recvall(int ID, char * data, int totalbytes);

	bool SendInt(int ID, int _int);
	bool GetInt(int ID, int & _int);

	bool SendPacketType(int ID, Packet _packettype);
	bool GetPacketType(int ID, Packet & _packettype);

	bool SendString(int ID, std::string & _string);
	bool GetString(int ID, std::string & _string);

	// we also need the pointer to exchange to process the exchange
	bool ProcessPacket(int ID, Packet _packettype, Exchange* exchange);

	static void ClientHandlerThread(void* parameter);
	HANDLE exchangeMutex;						// Mutex to make sure that only one thread can access
														// the Exchange object at one time
														// set it as static because we only need one mutex for this class

private:
	SOCKET Connections[100];
	int TotalConnections = 0;
	
	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);
	SOCKET sListen;

	Exchange exchange;			// exchange object is placed here

	// for debug uses:
	string replaceChar(string str, char ch1, char ch2);
};

static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
