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

/*void HandleTCPClient(int clntSocket, Camera camera){
	uint32_t buffer;
	//char buffer[BUFSIZE];
	Mat frame;
	cout << "Beginning of HandleTCPClient" << endl;
	
	// Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, &buffer, BUFSIZE, 0);
	
	cout << "Buffer content : " << buffer << endl;

	if (numBytesRcvd < 0){
		DieWithSystemMessage("recv() failed");
		close (clntSocket); // Close client socket
	}
	
	if(buffer == ELE4205_OK){
		cout << "In the buffer zone" << endl;
		frame = captureImage(camera);
		frame = (frame.reshape(0,1)); // make the frame continuous
		int frameSize = frame.total()*frame.elemSize();

		ssize_t numBytesSent = send(clntSocket, frame.data, frameSize, 0);
		if (numBytesSent < 0){
			DieWithSystemMessage("send() failed");
			close (clntSocket); // Close client socket
		}
		else if (numBytesSent != numBytesRcvd){
			DieWithUserMessage("send()", "sent unexpected number of bytes");
			close (clntSocket); // Close client socket
		}
	} else if (buffer == ELE4205_QUIT) {
		cout << "In the quitting zone" << endl;
		close (clntSocket); // Close client socket
	}
	else {
		cout << "else : " << buffer << endl;
	}

}*/

