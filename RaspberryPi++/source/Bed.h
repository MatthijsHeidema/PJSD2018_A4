/*
 * Bed.h
 *
 *  Created on: Dec 10, 2018
 *      Author: matthijs
 */

#ifndef BED_H_
#define BED_H_

#include "Device.h"

class Bed : public Device { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	using Device::Device;
	void sync();
};


#endif /* BED_H_ */
