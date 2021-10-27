#include "../include/common.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

uint16_t resX_all[nbRes] = {176,160,320,320,352,432,800,864,960,960,1184,1280,1280};
uint16_t resY_all[nbRes] = {144,120,176,240,288,240,600,480,544,720,656,720,960};
double fps_all[nbRes];

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

