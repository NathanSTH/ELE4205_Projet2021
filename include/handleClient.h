#ifndef HANDLECLIENT_H
#define HANDLECLIENT_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <netdb.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

/**
	\brief This function handles the verification of the pressed key to change resolution or quit.
	*@param waitTime Time to wait for a key entry (in ms).
	*@param messages Variable that contains selected resolution and status.
	**/
void HandleWaitKey(int waitTime, uint32_t &messages,uint32_t &resX, uint32_t &resY, uint32_t &currentRes);
/**
	\brief This function displays the resolution options in the console.
	**/
void PrintResOptions(void);

void sendMsg2Server(int sock, uint32_t messages, uint8_t &esc_flag);

void handleSocket(int argc, char *argv[], int &sock);

#endif
