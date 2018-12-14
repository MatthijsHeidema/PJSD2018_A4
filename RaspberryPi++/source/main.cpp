// Client side C/C++ program to demonstrate Socket programming
// Geeks for Geeks and additions by Mathieu

#define FILE_PATH "/home/pi/Documents/PI_client/client.json"
#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.4.4"
#define PORT_LAMP 3000

#include <stdio.h>
#include "Lamp.h"


using namespace std;

int main()
{
	Lamp lamp(IP_LAMP, FILE_PATH_LAMP, PORT_LAMP);
	Lamp lamp1("192.168.4.5", "/var/www/html/lamp1.json" , 3001);
	lamp.comm->connectToServer();
	cout << "Connected to the wemos!" << endl;


	lamp1.comm->connectToServer();
	cout << "Connected to wemos 2" << endl;
	while(1) {

		//lamp.sync();
		//usleep(2000000);
		//cout << "Nu lamp 1" << endl;
		//lamp1.sync();
		string a = lamp1.comm->receiveValue("HELLO ITS MIE\r");
		cout << a << endl;
		//lamp.comm->sendValue("kleur", "green");


		usleep(2000000);
		//std::this_thread::sleep_for(std::chrono::milliseconds(x));
	}

	return 0;
}
