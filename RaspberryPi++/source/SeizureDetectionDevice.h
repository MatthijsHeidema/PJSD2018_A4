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
	time_t timeOnDevice;
	bool absentTooLong = false;
	bool onDevice = false;
public:
	using Device::Device;
	bool checkAbsence();
private:
	int movementCounter = 0;
	bool deviceUpdate = false;
	bool intervalStart = true;
	time_t intervalStartTime;
};

#endif /* SEIZUREDETECTIONDEVICE_H_ */
