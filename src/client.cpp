#include "../include/common.h"
#include "../include/handleClient.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace cv;
using namespace std;

uint8_t esc_flag = 0;
ssize_t bytes = 0;
uint32_t currentRes = RES01;

int main(int argc, char *argv[]) {
	uint32_t resX = 1280;
	uint32_t resY = 960;
	uint32_t messages = ELE4205_OK;
	Mat img = Mat::zeros(resY,resX,CV_8UC3);
	int imgSize = img.total()*img.elemSize();
	uchar *sockData;
	sockData = new uchar[imgSize];

	if (argc < 2 || argc > 3) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)","<Server Address> <Echo Word> [<Server Port>]");

	char *servIP = argv[1];     // First arg: server IP address (dotted quad)

	// Third arg (optional): server port (numeric).  7 is well-known echo port
	in_port_t servPort = (argc == 3) ? atoi(argv[2]) : 7;

	// Create a reliable, stream socket using TCP
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
		DieWithSystemMessage("socket() failed");

	// Construct the server address structure
	struct sockaddr_in servAddr;            // Server address
	memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
	servAddr.sin_family = AF_INET;          // IPv4 address family

	// Convert address
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if (rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if (rtnVal < 0)
		DieWithSystemMessage("inet_pton() failed");
	servAddr.sin_port = htons(servPort);    // Server port

	// Establish the connection to the echo server
	if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("connect() failed");
	PrintResOptions();
	while(esc_flag == 0){
		// Send the string to the server
		size_t msgLen = sizeof(uint32_t); // Determine input length
		uint32_t buffer = htonl(messages);
		ssize_t numBytes = send(sock, &buffer, msgLen, 0);
  		if (numBytes < 0){
  			DieWithSystemMessage("send() failed");
			esc_flag = 1;
		}
  		else if (numBytes != msgLen) {
  			DieWithUserMessage("send()", "sent unexpected number of bytes");
			esc_flag = 1;
		}

		bytes = 0;
		sockData = new uchar[imgSize];

		for (int i = 0; i < imgSize; i += bytes){			
			if ((bytes = recv(sock, sockData +i, imgSize -i, 0)) == -1){
				DieWithSystemMessage("recv() failed");
				esc_flag = 1;
			}
		}

		Mat img(Size(resX, resY), CV_8UC3, sockData);
		namedWindow("Stream",CV_WINDOW_AUTOSIZE);
		imshow("Stream", img);

		HandleWaitKey(30, messages);
		if ((messages & MASK_STATUS) == 0){
			esc_flag = 1;
			destroyWindow("Stream");
		}

		ChangeResClient(img, messages, resX, resY, currentRes, imgSize);
	}
	

  	fputc('\n', stdout); // Print a final linefeed

	close(sock);
	exit(0);
}

