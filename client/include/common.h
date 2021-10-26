#ifndef COMMON_H
#define COMMON_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "cameraCapture.h"

#define ELE4205_OK 0b1
#define ELE4205_QUIT 0b0

#define FILENAME_MAX_SIZE 1024
#define BUFFER_SIZE 1024000

enum MESSAGE : uint32_t {
	MSG_OK = 0,
	MSG_QUIT = 1,
};

typedef struct init_packet init_packet;

enum sizeConstants {
	MAXSTRINGLENGTH = 128,
	BUFSIZE = 512,
};

long long int getFileSize(char* filename);

void DieWithUserMessage(const char *msg, const char *detail);

void DieWithSystemMessage(const char *msg);

void HandleTCPClient(int clntSocket, Camera camera);


#endif
