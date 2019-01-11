/*
 * Zeil.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: jerry
 */

#include "Zuil.h"

void Zuil::sync() {

	file->updateDoc();
	string JA = "1";
	string NEE = "0";

	const char* Rooksensor = comm->receiveValue("Rooksensor");
	int sensorValue = atoi(Rooksensor);

	if(sensorValue > 300 && !RookStatus){
		comm->sendValue("RookMelderStatus",JA);
		file->edit("RookMelderStatus",JA);
		RookStatus = true;
	}else if(sensorValue < 300 && RookStatus){
		comm->sendValue("RookMelderStatus",NEE);
		file->edit("RookMelderStatus",NEE);
		RookStatus = false;
	}

	file->updateFile();
}
