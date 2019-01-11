/*
 * Fridge.h
 *
 *  Created on: Jan 10, 2019
 *      Author: matthijs
 */

#ifndef FRIDGE_H_
#define FRIDGE_H_

#include "Device.h"

class Fridge : public Device { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	using Device::Device;
	void sync();
	void setTemperature(int temperature, int margin);
	void setFanTemperature(int temperature, int margin);
private:
	double convertTemp(double temp);
	int targetFanTemperature = 20;
	int fanTemperatureMargin = 1;
	int targetPeltierTemperature = 5;
	int peltierTemperatureMargin = 1;
	int updateTimeOpened = 0;
	time_t opened;
};

#endif /* FRIDGE_H_ */
