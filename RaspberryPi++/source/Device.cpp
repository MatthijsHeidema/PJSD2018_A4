/*
 * Device.cpp
 *
 *  Created on: Dec 7, 2018
 *      Author: matthijs
 */

#include "Device.h"

Device::Device(const char* ip_address, const char* filePath, int port) : comm(new TCP_Client(ip_address, port)), file(new JsonFile(filePath)) {
}

Device::Device(const char* ip_address, const char* filePath, int port, const char* jsonContent) : comm(new TCP_Client(ip_address, port)), file(new JsonFile(filePath, jsonContent)) {
}

void Device::sync(){};

bool Device::connectToServer() { 	//Connect to domestic device, return false if it fails

	return comm->connectToServer();
}

bool Device::nighttimeCheck() {		//Returns true if it is currently night time, (for testing purposes, the first half of a minute is considered day, the second half is considered night)
	time_t currentTime = time(0);
	struct tm *tempTijd = gmtime(&currentTime);
	if(tempTijd->tm_sec > 30)
	{
		return true;	//"Nacht" de laatste 30 seconden van de minuut
	}
	else
	{
		return false;	//"Dag" de eerste 30 seconden van de minuut
	}
}
