/*
 * SeizureDetectionDevice.h
 *
 *  Created on: Jan 7, 2019
 *      Author: programmer
 */

#ifndef SEIZUREDETECTIONDEVICE_H_
#define SEIZUREDETECTIONDEVICE_H_

#include "Device.h"

class SeizureDetectionDevice: public Device {
protected:
	bool pressureSensorLogic(const char* value);
	time_t timeOffDevice;
public:
	using Device::Device;
private:
	bool onDevice = false;
	int movementCounter = 0;
	bool deviceUpdate = false;
	bool IntervalStart = true;
	time_t IntervalStartTime;

};

#endif /* SEIZUREDETECTIONDEVICE_H_ */
