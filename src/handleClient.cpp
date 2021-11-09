#include "../include/handleClient.h"

using namespace std;

void HandleWaitKey(int waitTime, uint32_t &messages){

	int c = waitKey(waitTime) & 0xFF;
	
	switch (c){
		case 27 :
			messages = messages & ELE4205_QUIT;
			break;
		case 49:
			messages = RES01 | ELE4205_OK;
			break;
		case 50:
			messages = RES02 | ELE4205_OK;
			break;
		case 51 :
			messages = RES03 | ELE4205_OK;
			break;
		case 52 :
			messages = RES04 | ELE4205_OK;
			break;
		default :
			messages =  messages | ELE4205_OK;
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

void ChangeResClient(Mat &img, uint32_t messages, uint32_t &resX, uint32_t &resY, uint32_t &currentRes, int &imgSize){
	if ((messages & MASK_RES) != currentRes){
		switch (messages & MASK_RES){
			case RES01 :
				currentRes = RES01;
				resX = resX_all[12]; //1280
				resY = resY_all[12]; //960
				break;
			case RES02 :
				currentRes = RES02;
				resX = resX_all[6]; //800
				resY = resY_all[6]; //600
				break;
			case RES03 :
				currentRes = RES03;
				resX = resX_all[3]; //320
				resY = resY_all[3]; //240
				break;
			case RES04 :
				currentRes = RES04;
				resX = resX_all[0]; //176
				resY = resY_all[0]; //144
				break;
			default :
				break;
		}
		
		img = Mat::zeros(resY,resX,CV_8UC3);
		imgSize = img.total()*img.elemSize();
	}
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
