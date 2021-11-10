#include "../include/handleTCP.h"

using namespace std;

uint32_t currentRes = RES01;

void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap, Camera &camera){
	uint32_t messages;
	if(checkLight()){
		if(checkGPIO())
			messages = MASK_SERV & PUSHB;
		else
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
			//camera.fps = fps_all[12];
			break;
		case RES02 :
			currentRes = RES02;
			camera.resX = resX_all[6]; //800
			camera.resY = resY_all[6]; //600
			//camera.fps = fps_all[6];
			break;
		case RES03 :
			currentRes = RES03;
			camera.resX = resX_all[3]; //320
			camera.resY = resY_all[3]; //240
			//camera.fps = fps_all[3];
			break;
		case RES04 :
			currentRes = RES04;
			camera.resX = resX_all[0]; //176
			camera.resY = resY_all[0]; //144
			//camera.fps = fps_all[0];
			break;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, camera.resX);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, camera.resY);
	frame = Mat::zeros(camera.resY,camera.resX,CV_8UC3);
}
