#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.4.4"
#define PORT_LAMP 3000

#include <stdio.h>
#include "Lamp.h"

using namespace std;

int main()
{
	//Construct devices
	Lamp lamp(IP_LAMP, FILE_PATH_LAMP, PORT_LAMP);
	Lamp lamp1("192.168.4.5", "/var/www/html/lamp1.json" , 3001);

	//Connect to devices
	lamp.connectToServer();

	while(1) {

		//Synchronize devices with JSON file
		lamp.sync();
		usleep(2000000);
	}

	return 0;
}
