#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.4.4"
#define PORT_LAMP 3000

#include <stdio.h>
#include "Lamp.h"
#include "Bed.h"

using namespace std;

int main()
{
	//Construct devices
	//Lamp lamp(IP_LAMP, FILE_PATH_LAMP, PORT_LAMP);

	//Lamp lamp("172.16.4.4", "/var/www/html/lamp.json" , 3005);
	Bed bed("10.10.10.20", "/home/pi/Documents/bed.json", 3000);


	//Connect to devices
	//lamp.connectToServer();
	bed.connectToServer();

	while(1) {
		//lamp.sync();
		bed.sync();
		usleep(500000);
	}

	return 0;
}
