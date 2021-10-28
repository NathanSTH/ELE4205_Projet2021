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

#include "common.h"

/**
	\fn HandleWaitKey
	\brief This function handles the verification of the pressed key to change resolution or quit.
	*@param waitTime Time to wait for a key entry (in ms).
	*@param messages Variable that contains selected resolution and status.
	**/
void HandleWaitKey(int waitTime, uint32_t &messages);
/**
	\fn PrintResOptions
	\brief This function displays the resolution options in the console.
	**/
void PrintResOptions(void);

/**
	\fn ChangeResClient
	\brief This function handles the resolution change for the client.
	*@param img Image to be updated.
	*@param messages Variable that contains selected resolution and status.
	*@param resX Image resolution width.
	*@param resY Image resolution heigth.
	*@param currentRes Current resolution.
	*@param imgSize Size of the image in bytes.	
	**/
void ChangeResClient(Mat &img, uint32_t messages, uint32_t &resX, uint32_t &resY, uint32_t &currentRes, int &imgSize);

#endif
