#include "../include/common.h"
#include "../include/handleClient.h"

using namespace cv;
using namespace std;

uint8_t esc_flag = 0;
ssize_t bytes = 0;
uint32_t currentRes = RES01;
static int i = 0;

int main(int argc, char *argv[]) {
	string filename;
	pid_t pid;
	uint32_t resX = 1280;
	uint32_t resY = 960;
	uint32_t messages = ELE4205_OK;
	Mat img = Mat::zeros(resY,resX,CV_8UC3);
	int imgSize = img.total()*img.elemSize();
	uchar *sockData;
	sockData = new uchar[imgSize];

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	handleSocket(argc, argv, sock);

	PrintResOptions();

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
			
		}else if(flag == IDOWN){
			messages = ELE4205_QUIT;
			esc_flag = 1;
			destroyWindow("Stream");
		}

		int tmp = currentRes;
		HandleWaitKey(30, messages, resX, resY, currentRes);

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
				destroyWindow("Stream");
			}
			Mat img(Size(resX, resY), CV_8UC3, sockData);
			namedWindow("Stream",WINDOW_AUTOSIZE);
			imshow("Stream", img);
			if(flag == PUSHB){
				i++;
				pid = fork();

				if (pid == 0){
					std::stringstream ss_i;
					ss_i << i;
					filename = (string("img") + ss_i.str() + string(".png")).c_str();
					imwrite(filename, img);
					_exit(EXIT_SUCCESS);
				}
			}	
		}	
			
	}
	

  	fputc('\n', stdout); // Print a final linefeed

	close(sock);
	exit(0);
}

