#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "../include/handleClient.h"

using namespace cv;
using namespace std;

uint8_t esc_flag = 0;
ssize_t bytes = 0;
uint32_t currentRes = RES01;
static int i = 0;
int const MAX_BINARY_VALUE = 255;

int main(int argc, char *argv[]) {
	string filename;
	pid_t pid;
	uint32_t resX = 1280;
	uint32_t resY = 960;
	uint32_t messages = ELE4205_OK;
	Mat img = Mat::zeros(resY,resX,CV_8UC3);
	Mat img_gray, img_bin;
	int threshold_value = 90;

	int imgSize = img.total()*img.elemSize();
	uchar *sockData;
	sockData = new uchar[imgSize];

	if (argc < 2 || argc > 3) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)","<Server Address> [<Server Port>]");

	char *servIP = argv[1];     // First arg: server IP address (dotted quad)

	// Third arg (optional): server port (numeric).  7 is well-known echo port
	in_port_t servPort = (argc == 3) ? atoi(argv[2]) : 7;

	int sock = handleSocket(servIP, servPort);

	PrintResOptions();

	//Creation of the Tesseract object
	tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
	ocr->Init(NULL, "eng", tesseract::OEM_TESSERACT_CUBE_COMBINED);
	ocr->SetPageSegMode(tesseract::PSM_AUTO);

	while(esc_flag == 0){
		
		messages = 0;

		// Receive message from server
		ssize_t numBytesRcvd = recv(sock, &messages, sizeof(uint32_t), 0);
		messages = ntohl(messages);
		
		if (numBytesRcvd < 0){
			DieWithSystemMessage("recv() failed");	
		}

		uint32_t flag = (messages & MASK_SERV);
		if(flag == READY || flag == PUSHB){
			messages = ELE4205_OK;
			
		} else if(flag == IDOWN){
			messages = ELE4205_QUIT;
			esc_flag = 1;
			destroyWindow("Stream");
		}

		int tmp = currentRes;
		HandleWaitKey(30, messages, resX, resY, currentRes, esc_flag);

		if (tmp != currentRes){
			img = Mat::zeros(resY,resX,CV_8UC3);
			imgSize = img.total()*img.elemSize();
		}
		sendMsg2Server(sock, messages, esc_flag);

		if (!esc_flag){
			bytes = 0;

			sockData = new uchar[imgSize];

			for (int i = 0; i < imgSize; i += bytes){			
				if ((bytes = recv(sock, sockData +i, imgSize -i, 0)) == -1){
					DieWithSystemMessage("recv() failed");
					esc_flag = 1;
				}
			}

			if ((messages & MASK_STATUS) == 0){
				esc_flag = 1;
				
			}

			Mat img(Size(resX, resY), CV_8UC3, sockData);
			namedWindow("Stream",WINDOW_AUTOSIZE);
			imshow("Stream", img);
			if(flag == PUSHB){
				i++;
				pid = fork();

				if (pid == 0){
					//Saving the image to a file "<imgXX>.png"
					std::stringstream ss_i;
					ss_i << i;
					filename = (string("img") + ss_i.str() + string(".png")).c_str();
					imwrite(filename, img);

					//Using OpenCV to read the image
					//Mat im = cv::imread(filename, IMREAD_COLOR);
					Mat im = cv::imread("Autumn_leaves-ocr.png", IMREAD_COLOR);

					ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);
					ocr->SetVariable("tessedit_char_whitelist","ABCDEFGRabcdefg0123456789#");
					ocr->SetVariable("tessedit_char_blacklist","t");

					char* outText = strcat(ocr->GetUTF8Text(),"\0");

					ocr->End();

					int sock2 = handleSocket(servIP, 4100);
					uint32_t msgLen = htonl(strlen(outText));

					ssize_t numBytesSent = send(sock2, &msgLen, sizeof(uint32_t), 0);

					if (numBytesSent < 0){
								DieWithSystemMessage("send() failed #1");
					}

					uint32_t buf;
					ssize_t numByteRcvd = recv(sock2, &buf, sizeof(uint32_t), 0);
					buf = ntohl(buf);

					if (numBytesRcvd < 0){
						DieWithSystemMessage("recv() failed");
					}else if(buf != ELE4205_OK){
						DieWithSystemMessage("server didn't receive the message");
					}else{
						cout << "~~~" << endl << outText << endl;
						numBytesSent = send(sock2, outText, ntohl(msgLen), 0);
						cout << "~~~" << endl;
						if (numBytesSent < 0){
							DieWithSystemMessage("send() failed #2");
						}
					}
					//exiting
					_exit(EXIT_SUCCESS);
				}
			}	
		} else {
			destroyWindow("Stream");
		}
	}

	fputc('\n', stdout); // Print a final linefeed

	close(sock);
	exit(0);
}

