#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.4.4"
#define PORT_LAMP 3000

#include <stdio.h>
#include "Lamp.h"
#include "Door.h"
#include "Bed.h"
#include "Stoel.h"


using namespace std;

int main()
{
	//Construct devices
	//Lamp lamp(IP_LAMP, FILE_PATH_LAMP, PORT_LAMP);

	//Lamp lamp("192.168.4.4", "/var/www/html/lamp.json" , 3005);
	//Bed bed("192.168.4.8", "/var/www/html/bed.json", 3000);
	//Stoel stoel("192.168.4.9", "/var/www/html/chair.json", 3001);
	Door door("192.168.4.10", "/var/www/html/door.json" , 3010);
	//Connect to devices
	//lamp.connectToServer();
	//stoel.connectToServer();
	//bed.connectToServer();
	door.connectToServer();
	
	while(1) {
		//lamp.sync();
		//stoel.sync();
		//bed.sync();
		door.sync();
		usleep(5000);
	}

	return 0;
}
