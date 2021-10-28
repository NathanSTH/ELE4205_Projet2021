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

Camera camera;

int main(int argc, char *argv[]) {
	//get_timing();
	camera.resX = resX_all[12];
	camera.resY = resY_all[12];
	camera.fps = fps_all[12];

  if (argc != 2) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  in_port_t servPort = atoi(argv[1]); // First arg:  local port

  // Create socket for incoming connections
  int servSock; // Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithSystemMessage("socket() failed");

  // Construct local address structure
  struct sockaddr_in servAddr;                  // Local address
  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
  servAddr.sin_family = AF_INET;                // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
  servAddr.sin_port = htons(servPort);          // Local port

  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("bind() failed");

  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, MAXPENDING) < 0)
    DieWithSystemMessage("listen() failed");
    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    // clntSock is connected to a client!
    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
	puts("Unable to get client address");
	Mat frame = Mat::zeros(camera.resY,camera.resX,CV_8UC3);

	//Open the default video camera	
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, camera.resX);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, camera.resY);

	// if not success, exit program
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video camera" << endl;
		cin.get(); //wait for any key press
		//return -1;
	}

  for (;;) { // Run forever
  	HandleTCPClient(clntSock, frame, cap, camera);
  }
  // NOT REACHED
}

