/*
 * Window.h
 *
 *  Created on: Jan 10, 2019
 *      Author: programmer
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "Device.h"

class Window : public Device // @suppress("Class has a virtual method and non-virtual destructor")
{
public:
	using Device::Device;
	void sync();

private:
	bool mode;
	time_t timeChanged;
	void setWindow(int lichtSensor);
};



#endif /* WINDOW_H_ */
