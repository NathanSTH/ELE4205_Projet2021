#ifndef HANDLE_TCP
#define HANDLE_TCP

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
#include "common.h"

/**
	\fn HandleTCPClient
	\brief This function handles the TCP connection with the client, on the server side.
	*@param clntSocket Socket number assigned to the connection with the client.
	*@param frame Contains the Mat object with header details.
	*@param cap Selected video camera.
	**/
void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap);

#endif
