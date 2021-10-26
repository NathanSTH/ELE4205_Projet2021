#include "../include/common.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

long long int getFileSize(char* filename){
	ifstream ifs(filename, ios::in | ios::ate | ios::binary);
	long long int filesize = (long long int) ifs.tellg();
	ifs.close();
	return filesize;
}

void DieWithUserMessage(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

void DieWithSystemMessage(const char *msg) {
	perror(msg);
	exit(1);
}

void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap){
    // frame.rows = camera.resX;
    // frame.cols = camera.resY;
	uint32_t buffer;

	// Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, &buffer, sizeof(uint32_t), 0);
	buffer = ntohl(buffer);
	
	if (numBytesRcvd < 0){
		DieWithSystemMessage("recv() failed");
		close (clntSocket); // Close client socket
	}
	
	if(buffer == ELE4205_OK){
		bool isSuccess = cap.read(frame); // read a new frame from the video camera

		if(isSuccess == false)
			DieWithSystemMessage("Could not capture an image");

		frame = (frame.reshape(0,1)); // make the frame continuous
		uint32_t frameSize = frame.total()*frame.elemSize();

		if(send(clntSocket,&frameSize,sizeof(uint32_t),0) < 0){
			DieWithSystemMessage("send() failed");
			close (clntSocket); // Close client socket
		}
		ssize_t numBytesSent = send(clntSocket, frame.data, frameSize, 0);
		if (numBytesSent < 0){
			DieWithSystemMessage("send() failed");
			close (clntSocket); // Close client socket
		}

		else if (numBytesSent != frameSize)
			DieWithUserMessage("send()", "sent unexpected number of bytes");

	} else if (buffer == ELE4205_QUIT) {
		cout << "escape key hit" << endl
		close (clntSocket); // Close client socket
	}
	else {
		cout << "else : " << buffer << endl;
	}

}

