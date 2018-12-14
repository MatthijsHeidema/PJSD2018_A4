/*
 * Device.cpp
 *
 *  Created on: Dec 7, 2018
 *      Author: matthijs
 */

#include "Device.h"

Device::Device(const char* ip_address, const char* filePath, int port) : comm(new TCP_Client(ip_address, port)), file(new JsonFile(filePath)) {
}

void Device::sync(){};

bool Device::connectToServer() {

	return comm->connectToServer();
}
