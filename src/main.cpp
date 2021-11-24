#include "../include/handleTCP.h"
#include <cstring>

using namespace std;

Camera camera;
/**
\mainpage
This project is in 2 parts. The first consists of a TCP server application that will be installed on an Odroid-OC2 and will use a Logitech C270 camera to transmit to a connected client. 
The second part, the client application, will be installed on a computer and will allow the user to select a desired image resolution, stream images from the server camera and display them 
on the screen. The user can change resolution anytime during the stream by pressing "1", "2", "3" or "4" on the keyboard or "ESC" to quit the application and end the TCP communication.
**/

std::map<string, int> notes;

void initMap(void);


int main(int argc, char *argv[]) {
	initMap();
	//get_timing();
	camera.resX = resX_all[12];
	camera.resY = resY_all[12];
	camera.fps = fps_all[12];

  if (argc != 2) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  in_port_t servPort = atoi(argv[1]); // First arg:  local port
  int clntSock = ConnectToClient(argc, servPort);

  Mat frame = Mat::zeros(camera.resY,camera.resX,CV_8UC3);

	//Open the default video camera	
	VideoCapture cap(0);
        cap.set(CV_CAP_PROP_AUTOFOCUS, 1);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, camera.resX);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, camera.resY);

	// if not success, exit program
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video camera" << endl;
		cin.get(); //wait for any key press
		//return -1;
	}
	bool chkGPIO = initGPIO();
	if (!chkGPIO){
		cout << "Cannot initialize GPIO" << endl;
	}

  for (;;) { // Run forever
  	HandleTCPClient(clntSock, frame, cap, camera);
  }
  // NOT REACHED
}

void initMap(void){
	notes.insert(std::pair<string, int>("A", 440));
	notes.insert(std::pair<string, int>("A#", 466));
	notes.insert(std::pair<string, int>("B", 494));
	notes.insert(std::pair<string, int>("C", 523));
	notes.insert(std::pair<string, int>("C#", 554));
	notes.insert(std::pair<string, int>("D", 587));
	notes.insert(std::pair<string, int>("D#", 622));
	notes.insert(std::pair<string, int>("E", 659));
	notes.insert(std::pair<string, int>("F", 698));
	notes.insert(std::pair<string, int>("F#", 734));
	notes.insert(std::pair<string, int>("G", 784));
	notes.insert(std::pair<string, int>("G#", 831));
	notes.insert(std::pair<string, int>("a", 880));
	notes.insert(std::pair<string, int>("a#", 932));
	notes.insert(std::pair<string, int>("b", 988));
	notes.insert(std::pair<string, int>("c", 1046));
	notes.insert(std::pair<string, int>("c#", 1109));
	notes.insert(std::pair<string, int>("d", 1175));
	notes.insert(std::pair<string, int>("d#", 1245));
	notes.insert(std::pair<string, int>("e", 1319));
	notes.insert(std::pair<string, int>("f", 1397));
	notes.insert(std::pair<string, int>("f#", 1480));
	notes.insert(std::pair<string, int>("g", 1568));
	notes.insert(std::pair<string, int>("g#", 1661));
}


