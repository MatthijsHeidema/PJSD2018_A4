/*
 * Bed.cpp
 *
 *  Created on: Dec 10, 2018
 *      Author: matthijs
 */

#include "Bed.h"

void Bed::sync() {

	file->updateDoc();


	const char* bedLight = comm->receiveValue("BedLight");
	comm->sendValue("BedLight", bedLight);

	//const char* outOfBed = comm->receiveValue("OutOfBed");
	//const char* seizure = comm->receiveValue("Seizure");

	//file->edit("OutOfBed", outOfBed);
	//file->edit("Seizure", seizure);
	//file->updateFile();
}
