/*
 * Zuil.h
 *
 *  Created on: Jan 7, 2019
 *      Author: jerry
 */

#ifndef ZUIL_H_
#define ZUIL_H_

#include "Device.h"

class Zuil : public Device { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	using Device::Device;
	void sync();

private:
	bool RookStatus = false;
};

#endif /* ZUIL_H_ */
