#ifndef HANDLE_TCP
#define HANDLE_TCP

#pragma once
#include "common.h"

/**
	\brief This function handles the TCP connection with the client, on the server side.
	*@param clntSocket Socket number assigned to the connection with the client.
	*@param frame Contains the Mat object with header details.
	*@param cap Selected video camera.
	*@param camera Selected camera specifications.
	**/
void HandleTCPClient(int clntSocket, Mat frame, VideoCapture cap, Camera &camera);

/**
	\brief This updates the image resolution for the server.
	*@param messages Variable that contains selected resolution and status.
	*@param cap Selected video camera.
	*@param camera Selected camera specifications.
	*@param frame Contains the Mat object with header details.
	**/
void UpdateRes (uint32_t messages, VideoCapture &cap, Camera &camera, Mat &frame);

#endif
