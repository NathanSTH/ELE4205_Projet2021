#include "../include/handleTCP.h"
#include "../include/PWMSongParser.h"

using namespace std;

uint32_t currentRes = RES01;

int ConnectToClient(in_port_t servPort){

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
	  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0){
		close(servSock);
	    DieWithSystemMessage("bind() failed");
	  }
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

	    return clntSock;
}

void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap, Camera &camera){
	pid_t pid;
	static bool button_pressed = 0;
	uint32_t messages;
	if(checkLight()){
		bool chk = checkGPIO();
		if(chk != button_pressed){
			button_pressed = chk;
			if (!button_pressed){
				messages = MASK_SERV & PUSHB;
				pid = fork();
				if (pid == 0){
					execl("./PWMSongParser",NULL);
					_exit(EXIT_SUCCESS);
				}
			}
			else
				messages = MASK_SERV & READY;
		} else
			messages = MASK_SERV & READY;

	} else
		messages = MASK_SERV & IDOWN;
	uint32_t buf = htonl(messages);

	// Send status message to client
	ssize_t numBytesSent = send(clntSocket, &buf, sizeof(uint32_t), 0);
	if (numBytesSent < 0){
		close (clntSocket); // Close client socket
		DieWithSystemMessage("send() failed");			
	}


	// Receive message from client (OK,QUIT,RES)
	ssize_t numBytesRcvd = recv(clntSocket, &messages, sizeof(uint32_t), 0);
	messages = ntohl(messages);
	
	if (numBytesRcvd < 0){
		close (clntSocket); // Close client socket
		DieWithSystemMessage("recv() failed");	
	}

	if((messages & MASK_STATUS) == ELE4205_OK){
		if ((messages & MASK_RES) != currentRes){
      			UpdateRes(messages, cap, camera, frame);
		}
		bool isSuccess = cap.read(frame); // read a new frame from the video camera
		if(isSuccess == false)
			DieWithSystemMessage("Could not capture an image");

		frame = (frame.reshape(0,1)); // make the frame continuous
		uint32_t frameSize = frame.total()*frame.elemSize();
		ssize_t numBytesSent = send(clntSocket, frame.data, frameSize, 0);

		if (numBytesSent < 0){
			close (clntSocket); // Close client socket
			DieWithSystemMessage("send() failed");			
		}

		else if (numBytesSent != frameSize)
			DieWithUserMessage("send()", "sent unexpected number of bytes");

	} else if ((messages & MASK_STATUS) == ELE4205_QUIT) {
		close (clntSocket);
		DieWithSystemMessage("Quit message received, closing socket."); // Close client socket
	}
}

void UpdateRes (uint32_t messages, VideoCapture &cap, Camera &camera, Mat &frame){
	switch (messages & MASK_RES){
		case RES01 :
			currentRes = RES01;
			camera.resX = resX_all[12]; //1280
			camera.resY = resY_all[12]; //960
			break;
		case RES02 :
			currentRes = RES02;
			camera.resX = resX_all[6]; //800
			camera.resY = resY_all[6]; //600
			break;
		case RES03 :
			currentRes = RES03;
			camera.resX = resX_all[3]; //320
			camera.resY = resY_all[3]; //240
			break;
		case RES04 :
			currentRes = RES04;
			camera.resX = resX_all[0]; //176
			camera.resY = resY_all[0]; //144
			break;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, camera.resX);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, camera.resY);
	frame = Mat::zeros(camera.resY,camera.resX,CV_8UC3);
}
