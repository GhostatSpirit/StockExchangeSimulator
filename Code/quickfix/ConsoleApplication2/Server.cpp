/* This code originally was created by Jpres at YouTube.
* Website: https://www.youtube.com/watch?v=9jLhDYtyA7c&list=PLcacUGyBsOIBFl6Q8d5XkQs7NzSlxlTKx&index=11
* Modified by Yang Liu, lykavin@hotmail.com
*/

#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically)//Port = port to broadcast on. BroadcastPublically = false if server is not open to the public (people outside of your router), true = server is open to everyone (assumes that the port is properly forwarded on router settings)
	:exchange("MS")		// initialize the exchange object
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically) //If server is open to public
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else //If server is only for our router
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	if (::bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (::listen(sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverptr = this;

	// initialize the exchangeMutex
	exchangeMutex = CreateSemaphore(NULL, 1, 1, NULL);		// create a semaphore for this mutex
}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	if (newConnection == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::cout << "Client Connected! ID:" << TotalConnections << std::endl;
		Connections[TotalConnections] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.

		// construct the struct object (PARAMS) to be sent
		PARAMS params;
		params.i = TotalConnections;
		params.p_exchange = &exchange;

		// Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, &params, NULL, NULL);
		

		// send welcome message to the client
		std::string welcome = "Sever: Welcome to the " + exchange.getCompID() + " exchange.\n";
		SendString(TotalConnections, welcome);
		TotalConnections += 1; //Incremenent total # of clients that have connected
		return true;
	}
}

bool Server::ProcessPacket(int ID, Packet _packettype, Exchange* p_exchange)
{
	switch (_packettype)
	{
	case P_ChatMessage: //Packet Type: chat message
	{
		std::string Message; //string to store our message we received
		if (!GetString(ID, Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
						  //Next we need to send the message out to each user

		std::cout << "> RECEIVED: " << Message << std::endl;

		WaitForSingleObject(exchangeMutex, INFINITE);		// P (resource not being used)
		// TODO: process the message

		Message = replaceChar(Message, '|', '\001');

		p_exchange->process_msg(Message);
		p_exchange->print_orderbooks(p_exchange->getTradableBuy());

		ReleaseSemaphore(exchangeMutex, 1, NULL);			// V (we've done with the resource)

		//for (int i = 0; i < TotalConnections; i++)
		//{
		//	if (i == ID) //If connection is the user who sent the message...
		//		continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
		//	if (!SendString(i, Message)) //Send message to connection at index i, if message fails to be sent...
		//	{
		//		std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << i << std::endl;
		//	}
		//}
		std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
		break;
	}

	default: //If packet type is not accounted for
	{
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	}
	return true;
}

void Server::ClientHandlerThread(void* parameter)		// PARAM* parameter
														// ID is the index in the SOCKET Connections array
{
	// recovering info from the parameter
	PARAMS* params = (PARAMS*)parameter;
	int ID = params->i;
	Exchange* p_exchange = params->p_exchange;

	Packet PacketType;
	while (true)
	{
		if (!serverptr->GetPacketType(ID, PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop

		
		// we not start using the resource
		if (!serverptr->ProcessPacket(ID, PacketType, p_exchange)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop

		


	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(serverptr->Connections[ID]);
	return;
}

string Server::replaceChar(string str, char ch1, char ch2) {
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == ch1)
			str[i] = ch2;
	}

	return str;
}