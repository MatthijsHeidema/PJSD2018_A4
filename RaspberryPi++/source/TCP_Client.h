/*
 * TCP_Client.h
 *
 *  Created on: Dec 3, 2018
 *      Author: programmer
 */

#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <unistd.h>


using namespace std;

class TCP_Client
{
public:
	TCP_Client(const char *address, int port);
	int createSocket();
	bool connectToServer();
	int disconnectFromServer();
	int sendValue(string key, string value);
	const char* receiveValue(string);

private:
	const char *ip_address;
	int port;
	int sock;
	struct sockaddr_in serv_addr;


	int sendString(string str);

};

#endif /* TCP_CLIENT_H_ */
