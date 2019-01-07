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
	/*
	bool pressureSensorLogic(const char* value);
	bool inBed = false;
	int movementCounterBed = 0;
	bool bedUpdate = false;
	bool bedIntervalStart = true;
	time_t bedIntervalStartTime;
	time_t timeOutOfBed;
	*/
	void automaticLightOff(int timeUntilOff);
	time_t timeBedLightOn;

};


#endif /* BED_H_ */
