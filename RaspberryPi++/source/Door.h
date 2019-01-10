/*
 * Door.h
 *
 *  Created on: Jan 7, 2019
 *      Author: Mathieu
 */

#ifndef DOOR_H_
#define DOOR_H_


#include "Device.h"

class Door : public Device { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	using Device::Device;
	void sync();
};


#endif /* DOOR_H_ */
