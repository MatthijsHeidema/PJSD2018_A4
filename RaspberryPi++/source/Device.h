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
public:
	Device(const char* ip_address, const char* filePath, int port);
	virtual void sync() = 0;
	TCP_Client* comm;
	JsonFile* file;
};

#endif /* DEVICE_H_ */
