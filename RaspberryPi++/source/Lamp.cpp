/*
 * Lamp.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: matthijs
 */

#include "Lamp.h"
#include <unistd.h>

void Lamp::sync() {

	file->updateDoc();											//Update file->'doc' with current contents of the JSON file

	const char* toggleLed = file->getStringValue("ToggleLed"); 	//Get value associated with "ToggleLed" from file
	const char* color = file->getStringValue("Color");			//Get value associated with "Color" from file
	comm->sendValue("ToggleLed", toggleLed);					//Send value 'toggleLed' to the lamp
	comm->sendValue("Color", color);							//Send value 'color' to the lamp

	const char* PIR = comm->receiveValue("PIR");				//Get value associated with "PIR" from lamp (movement sensor)
	file->edit("PIR", PIR);										//Change value associated with "PIR" in file->'doc'

	file->updateFile();											//Write changes to JSON file on hard disk
}
