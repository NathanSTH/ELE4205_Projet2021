#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/cameraCapture.h"
#include "../include/common.h"
#include "../include/handleTCP.h"
#include <netdb.h>
#include <unistd.h>
#include <fstream>
#include <cstring>

static const int MAXPENDING = 5; // Maximum outstanding connection requests
using namespace std;

global Camera camera;
/**
\mainpage
This project is in 2 parts. The first consists of a TCP server application that will be installed on an Odroid-OC2 and will use a Logitech C270 camera to transmit to a connected client. 
The second part, the client application, will be installed on a computer and will allow the user to select a desired image resolution, stream images from the server camera and display them 
on the screen. The user can change resolution anytime during the stream by pressing "1", "2", "3" or "4" on the keyboard or "ESC" to quit the application and end the TCP communication.
**/

int main(int argc, char *argv[]) {
	//get_timing();
	camera.resX = resX_all[12];
	camera.resY = resY_all[12];
	camera.fps = fps_all[12];

  initTCP(int argc, char *argv[]);

  for (;;) { // Run forever
  	HandleTCPClient(clntSock, frame, cap, camera);
  }
  // NOT REACHED
}