#include <stdio.h>
#include "JsonFile.h"
#include "Lamp.h"
#include "Bed.h"
#include "Stoel.h"
#include "definitions.h"

using namespace std;

int main()
{
	//Construct JSON object for main
	JsonFile main(FILE_PATH_MAIN, JSON_MAIN);


	//Construct devices
	//Lamp lamp("172.16.4.4", "/var/www/html/lamp.json" , 3005);
	//Bed bed("10.10.10.20", "/var/www/html/bed.json", 3000);
	Stoel stoel("10.10.10.21", FILE_PATH_CHAIR, PORT, JSON_CHAIR);


	//Connect to devices
	//lamp.connectToServer();
	//bed.connectToServer();
	stoel.connectToServer();


	while(1) {
		//lamp.sync();
		//bed.sync();
		stoel.sync();
		usleep(100000);

		/*
		if(bed.checkAbsence() && stoel.checkAbsence())
		{
			main.edit("TooLongAbsent","1");
		}
		*/

	}

	return 0;
}
