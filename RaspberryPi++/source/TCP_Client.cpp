/*
 * TCP_Client.cpp
 *
 *  Created on: Dec 3, 2018
 *      Author: Mathieu Gatsonides, Elmer Ruschen
 */

#include "TCP_Client.h"

TCP_Client::TCP_Client(const char* address, int poort):ip_address(address),port(poort),sock(-1)
{
	createSocket();		//Create a socket when creating an object
}

int TCP_Client::createSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);		//Creates a TCP IPv4 socket
	if(sock == -1)
	{
		cout << "Socket creation error\n";
	}
	else
	{
	cout  << "Socket created\n";
	}
	return sock;
}

bool TCP_Client::connectToServer()
{
	/*		Overig
	//create socket if it isnt created yet!
	if(sock == -1)
	{
		createSocket();
		cout << "Socket already created!\n";
	}
	else{}*/

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;				// Set the protocol family to IPv4
	serv_addr.sin_port = htons(port);			// Set the port



	//IP address conversion to binary
	if(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)<=0)
	    {
	        printf("\nInvalid address/ Address not supported \n");
	        return -1;
	    }

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)			// Connect to the socket
	    {
	        printf("\nConnection Failed: ");
	        printf("%s", ip_address);
	        printf("\n");
	        return -1;
	    }

	//else
	//{
	return 0;
	//}
}


int TCP_Client::sendString(string str)
{
	return send(sock, str.c_str(), str.length(), 0);		//Sending a string and returning the bytes transmitted or -1 when an error occurs
}


int TCP_Client::sendValue(string key, string value)
{
	string sendformat = key + ":" + value + "\r";		//Putting the key and value in json format
	cout << "Verzonden string: " << sendformat << endl;
	return sendString(sendformat);									//Sending the key and value. Returning the bytes transmitted or -1 when an error occurs.
}

const char* TCP_Client::receiveValue(string key)
{
	string sendformat = key + "?\r";
	sendString(sendformat);	//First send the key where you want the value from

	buffer[16] = {0};		//clear buffer

	recv(sock, buffer, 16, 0);		//Reading the value from the socket
	//std::cout << buffer << endl;
	return buffer;					//returning the value
	//Debug printf("%s\n",buffer);
}


int TCP_Client::disconnectFromServer()	//function maybe not needed
{
	return close(sock);
}

