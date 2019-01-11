/*
 * Bed.h
 *
 *  Created on: Dec 10, 2018
 *      Author: matthijs
 */

#ifndef BED_H_
#define BED_H_

#include "SeizureDetectionDevice.h"

class Bed : public SeizureDetectionDevice { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	using SeizureDetectionDevice::SeizureDetectionDevice;
	void sync();
private:
	void automaticLightOff(int timeUntilOff, const char* lightStatus);
	time_t timeBedLightOn;
	int outOfBedCount = 0;
	bool nightUpdate = true;

};


#endif /* BED_H_ */
