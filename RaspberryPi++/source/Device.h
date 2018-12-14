/*
 * Device.h
 *
 *  Created on: Dec 7, 2018
 *      Author: matthijs
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "JsonFile.h"
#include "TCP_Client.h"

class Device { // @suppress("Class has a virtual method and non-virtual destructor")
protected:
	TCP_Client* comm;
	JsonFile* file;

public:
	Device(const char* ip_address, const char* filePath, int port);
	virtual void sync() = 0;
	bool connectToServer();
};

#endif /* DEVICE_H_ */
