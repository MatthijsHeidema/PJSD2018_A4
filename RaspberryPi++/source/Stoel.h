/*
 * Stoel.h
 *
 *  Created on: Jan 7, 2019
 *      Author: programmer
 */

#ifndef STOEL_H_
#define STOEL_H_

#include "SeizureDetectionDevice.h"

class Stoel: public SeizureDetectionDevice {
public:
	using SeizureDetectionDevice::SeizureDetectionDevice;
	void sync();
};

#endif /* STOEL_H_ */
