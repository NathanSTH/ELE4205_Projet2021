#include "../include/handleTCP.h"
#include <cstring>

using namespace std;

Camera camera;
/**
\mainpage
This project consists of a TCP server application that will be installed on an Odroid-OC2 and will use a Logitech C270 camera to transmit to a connected client.
Then, a client application will be installed on a computer and will allow the user to select a desired image resolution, stream images from the server camera and display them
on the screen. The user can change resolution anytime during the stream by pressing "1", "2", "3" or "4" on the keyboard or "ESC" to quit the application and end the TCP communication.
A photoresistor connected to the Odroid will allow light intensity detection and close the stream if the intensity is too low. A push-button connected to the Odroid will allow the user
to save an image captured with the camera. Finally, the application uses Tesseract-OCR to recognizes characters on the captured image. The application will then
parse the recognized text and play it as music.
**/


int main(int argc, char *argv[]) {
	//get_timing();
	camera.resX = resX_all[12];
	camera.resY = resY_all[12];
	camera.fps = fps_all[12];

  if (argc != 2) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  in_port_t servPort = atoi(argv[1]); // First arg:  local port
  int clntSock = ConnectToClient(servPort);

  Mat frame = Mat::zeros(camera.resY,camera.resX,CV_8UC3);

	//Open the default video camera	
	VideoCapture cap(0);
	cap.set(CAP_PROP_FRAME_WIDTH, camera.resX);
	cap.set(CAP_PROP_FRAME_HEIGHT, camera.resY);

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




