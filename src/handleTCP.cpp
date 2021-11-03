#include "../include/handleTCP.h"

using namespace std;

uint32_t currentRes = RES01;

void initTCP(int argc, char *argv[])
{
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
}

void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap, Camera &camera)
{
	uint32_t messages;

	// Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, &messages, sizeof(uint32_t), 0);
	messages = ntohl(messages);

	if (numBytesRcvd < 0)
	{
		close(clntSocket); // Close client socket
		DieWithSystemMessage("recv() failed");
	}
	if ((messages & MASK_STATUS) == ELE4205_OK)
	{
		if ((messages & MASK_RES) != currentRes)
		{
			UpdateRes(messages, cap, camera, frame);
		}
		bool isSuccess = cap.read(frame); // read a new frame from the video camera
		if (isSuccess == false)
			DieWithSystemMessage("Could not capture an image");

		frame = (frame.reshape(0, 1)); // make the frame continuous
		uint32_t frameSize = frame.total() * frame.elemSize();

		ssize_t numBytesSent = send(clntSocket, frame.data, frameSize, 0);
		if (numBytesSent < 0)
		{
			close(clntSocket); // Close client socket
			DieWithSystemMessage("send() failed");
		}
		else if (numBytesSent != frameSize)
			DieWithUserMessage("send()", "sent unexpected number of bytes");
		else if ((messages & MASK_STATUS) == ELE4205_QUIT)
		{
			close(clntSocket);
			DieWithSystemMessage("Quit message received, closing socket."); // Close client socket
		}
	}
}

void UpdateRes(uint32_t messages, VideoCapture &cap, Camera &camera, Mat &frame)
{
	switch (messages & MASK_RES)
	{
	case RES01:
		currentRes = RES01;
		camera.resX = resX_all[12]; //1280
		camera.resY = resY_all[12]; //960
		//camera.fps = fps_all[12];
		break;
	case RES02:
		currentRes = RES02;
		camera.resX = resX_all[6]; //800
		camera.resY = resY_all[6]; //600
		//camera.fps = fps_all[6];
		break;
	case RES03:
		currentRes = RES03;
		camera.resX = resX_all[3]; //320
		camera.resY = resY_all[3]; //240
		//camera.fps = fps_all[3];
		break;
	case RES04:
		currentRes = RES04;
		camera.resX = resX_all[0]; //176
		camera.resY = resY_all[0]; //144
		//camera.fps = fps_all[0];
		break;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, camera.resX);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, camera.resY);
	frame = Mat::zeros(camera.resY, camera.resX, CV_8UC3);
}