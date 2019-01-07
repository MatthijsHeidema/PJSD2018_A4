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
private:
	bool pressureSensorLogic(const char* value);
	void automaticLightOff(int timeUntilOff);
	bool seizureDetected = false;
	bool inBed = false;
	int movementCounter = 0;
	bool bedUpdate = false;
	bool intervalStart = true;
	time_t intervalStartTime;
	time_t timeLightOn;
	time_t timeOutOfBed;
};


#endif /* BED_H_ */
