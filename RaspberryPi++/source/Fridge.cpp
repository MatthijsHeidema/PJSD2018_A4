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

//Fridge::Fridge(const char* ip_address, const char* filePath, int port) : comm(new TCP_Client(ip_address, port)), file(new JsonFile(filePath)), targetFanTemperature(5), fanTemperatureMargin(1), targetPeltierTemperature(20), peltierTemperatureMargin(1), open_check(0) {
//}
//
//Fridge::Fridge(const char* ip_address, const char* filePath, int port, const char* jsonContent) : comm(new TCP_Client(ip_address, port)), file(new JsonFile(filePath, jsonContent)), targetFanTemperature(5), fanTemperatureMargin(1), targetPeltierTemperature(20), peltierTemperatureMargin(1), open_check(0) {
//}

void Fridge::sync() {

	string yes = "1";
	string no = "0";

	file->updateDoc();

	int door = atoi(comm->receiveValue("Door"));
	double peltierTemp = atof(comm->receiveValue("PeltierTemp"));
	double fanTemp = atof(comm->receiveValue("FanTemp"));

	//bereken daadwerkelijke peltierTemp
	peltierTemp = convertTemp(peltierTemp);


	fanTemp = convertTemp(fanTemp);

	if (!door) {

		cout << "Door open" << endl;

		if (!updateTimeOpened) {
			time(&opened);
			updateTimeOpened = 1;

		} else if(difftime(time(0), opened) > MAX_TIME) {
			file->edit("OpenTooLong", 1);
		}

	} else {

		cout << "Door closed" << endl;

		updateTimeOpened = 0;
		file->edit("OpenTooLong", 0);
	}

	if (peltierTemp > targetPeltierTemperature + peltierTemperatureMargin && door) {
		comm->sendValue("Cooling", yes);
	} else if (peltierTemp < targetPeltierTemperature - peltierTemperatureMargin || !door ) {
		comm->sendValue("Cooling", no);
	}

	if (fanTemp > targetFanTemperature + fanTemperatureMargin) {
		comm->sendValue("Fan", yes);

	} else if (fanTemp < targetFanTemperature - fanTemperatureMargin) {
		comm->sendValue("Fan", no);
	}

	cout << "Door: " << door << endl;
	cout << "peltierTemp: " << peltierTemp << endl;
	cout << "fanTemp: " << fanTemp << endl;

	file->updateFile();
}

void Fridge::setTemperature(int temperature, int margin) {

	targetPeltierTemperature = temperature;
	peltierTemperatureMargin =  margin;
}

void Fridge::setFanTemperature(int temperature, int margin) {

	targetFanTemperature = temperature;
	fanTemperatureMargin =  margin;
}

double Fridge::convertTemp(double temp) {

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
