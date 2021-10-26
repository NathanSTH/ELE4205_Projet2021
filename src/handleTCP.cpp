#include "../include/handleTCP.h"

using namespace std;

void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap){

	uint32_t buffer;

	// Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, &buffer, sizeof(uint32_t), 0);
	buffer = ntohl(buffer);
	
	if (numBytesRcvd < 0){
		close (clntSocket); // Close client socket
		DieWithSystemMessage("recv() failed");
		
	}
	
	if(buffer == ELE4205_OK){
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

	} else if (buffer == ELE4205_QUIT) {
		close (clntSocket);
		DieWithSystemMessage("Quit message received, closing socket."); // Close client socket
	}
	else {
		cout << "else : " << buffer << endl;
	}
}
