/*
 * Fridge.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: matthijs
 */

#include "Fridge.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <complex>
#include <time.h>

#define MAX_TIME 15

using namespace std;

void Fridge::sync() {

	string yes = "1";
	string no = "0";

	file->updateDoc();												//Update 'doc' with current contents of the JSON file

	int door = atoi(comm->receiveValue("Door"));					//Get value of the door switch from the fridge and store in 'door'
	double peltierTemp = atof(comm->receiveValue("PeltierTemp"));	//Get value of the inside NTC sensor from the fridge and store in 'peltierTemp'
	double fanTemp = atof(comm->receiveValue("FanTemp"));			//Get value of the outside NTC sensor from the fridge and store in 'fanTemp'

	//Convert the raw NTC data to temperature in degrees Celsius:
	peltierTemp = convertTemp(peltierTemp);
	fanTemp = convertTemp(fanTemp);

	if (!door) {												//If the door is opened

		if (!updateTimeOpened) {								//Executes only once, resets when door closes
			time(&opened);										//Store current time in 'opened'
			updateTimeOpened = 1;								//Set flag

		} else if(difftime(time(0), opened) > MAX_TIME) {		//If MAX_TIME has passed since door was first opened
			file->edit("OpenTooLong", 1);						//Set notification in JSON file
		}

	} else {													//If door is closed
		updateTimeOpened = 0;									//Reset flag
		file->edit("OpenTooLong", 0);							//Reset notification in JSON file
	}

	if (peltierTemp > targetPeltierTemperature + peltierTemperatureMargin && door) {			//If the temperature inside the fridge is too high
		comm->sendValue("Cooling", yes);														//Tell fridge to turn on peltier device

	} else if (peltierTemp < targetPeltierTemperature - peltierTemperatureMargin || !door ) {	//If the temperature inside the fridge is too low
		comm->sendValue("Cooling", no);															//Tell fridge to turn off peltier device
	}

	if (fanTemp > targetFanTemperature + fanTemperatureMargin) {								//If the temperature on the back of the fridge is too high
		comm->sendValue("Fan", yes);															//Tell fridge to turn on fan

	} else if (fanTemp < targetFanTemperature - fanTemperatureMargin) {							//If the temperature on the back of the fridge is too low
		comm->sendValue("Fan", no);																//Tell fridge to turn off fan
	}

	file->updateFile();																			//Update the file on the hard disk with the changes
}

void Fridge::setTemperature(int temperature, int margin) {		//Sets the target inside temperature and its margin

	targetPeltierTemperature = temperature;
	peltierTemperatureMargin =  margin;
}

void Fridge::setFanTemperature(int temperature, int margin) {	//Sets the target outside temperature and its margin

	targetFanTemperature = temperature;
	fanTemperatureMargin =  margin;
}

double Fridge::convertTemp(double temp) {	//Convert the NTC value to degrees celsius

	//Source: http://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/, as of 11 jan 2019
	double R1 = 10000;
	double logR2, R2, T;
	double c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

	double Vo = temp;
	R2 = R1 * (1023.0 / Vo - 1.0);
	logR2 = log(R2);
	T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
	T = T - 273.15;

	return T;
}
