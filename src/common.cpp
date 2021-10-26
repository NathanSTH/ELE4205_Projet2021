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

