#ifndef COMMON_H
#define COMMON_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#define FILENAME_MAX_SIZE 1024
#define BUFFER_SIZE 1024000

enum MESSAGE : uint32_t {
	MSG_OK = 0,
	MSG_QUIT = 1,
	MSG_INIT = 2,
	MSG_END = 3
};

struct init_packet{
	MESSAGE msg;
	long long int filesize;
	char filename[FILENAME_MAX_SIZE];
};

typedef struct init_packet init_packet;

long long int getFileSize(char* filename);

#endif
