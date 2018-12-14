/*
 * Lamp.h
 *
 *  Created on: Dec 11, 2018
 *      Author: matthijs
 */

#ifndef LAMP_H_
#define LAMP_H_

#include "Device.h"

class Lamp : public Device { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	using Device::Device;
	void sync();
};

#endif /* LAMP_H_ */
