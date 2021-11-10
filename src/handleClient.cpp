#include "../include/handleClient.h"

using namespace std;

void HandleWaitKey(int waitTime, uint32_t &messages, uint32_t &resX, uint32_t &resY, uint32_t &currentRes, uint8_t &esc_flag){

	int c = waitKey(waitTime) & 0xFF;
	
	switch (c){
		case 27 :
			messages = messages & ELE4205_QUIT;
			esc_flag = 1;
			break;
		case 49:
			messages = RES01 | ELE4205_OK;
			currentRes = RES01;
			resX = resX_all[12]; //1280
			resY = resY_all[12]; //960
			break;
		case 50:
			messages = RES02 | ELE4205_OK;
			currentRes = RES02;
			resX = resX_all[6]; //800
			resY = resY_all[6]; //600
			break;
		case 51 :
			messages = RES03 | ELE4205_OK;
			currentRes = RES03;
			resX = resX_all[3]; //320
			resY = resY_all[3]; //240
			break;
		case 52 :
			messages = RES04 | ELE4205_OK;
			currentRes = RES04;
			resX = resX_all[0]; //176
			resY = resY_all[0]; //144
			break;
		default :
			messages =  currentRes | messages;
			break;
	}

}

void PrintResOptions(void){
	cout << endl;
	cout << "Change resolution by pressing the corresponding key." << endl;
	cout << "Press ESC key to exit." << endl;
	cout << "1. 1280 x 960" << endl;
	cout << "2. 800 x 600" << endl;
	cout << "3. 320 x 240" << endl;
	cout << "4. 176 x 144" << endl;
	cout << endl;
}

void sendMsg2Server(int sock, uint32_t messages, uint8_t &esc_flag){
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
}

void handleSocket(int argc, char *argv[], int &sock){
	if (argc < 2 || argc > 3) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)","<Server Address> <Echo Word> [<Server Port>]");

	char *servIP = argv[1];     // First arg: server IP address (dotted quad)

	// Third arg (optional): server port (numeric).  7 is well-known echo port
	in_port_t servPort = (argc == 3) ? atoi(argv[2]) : 7;

	// Create a reliable, stream socket using TCP
	//int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
}
