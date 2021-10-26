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
#include <iostream>
#include <ostream>
#include "cameraCapture.h"

/**
   \var ELE4205_OK
   \brief Flag transmitted from client to server indicating that the client is ready to receive another frame.
**/
#define ELE4205_OK 0b1
/**
   \var ELE4205_QUIT
   \brief Flag transmitted from client to server indicating that the ESC key has been pressed, ending the stream.
**/
#define ELE4205_QUIT 0b0

/**
	\fn DieWithUserMessage
	\brief This function ends the running process while displaying user error messages. 
	**/
void DieWithUserMessage(const char *msg, const char *detail);
/**
	\fn DieWithUserMessage
	\brief This function ends the running process while displaying system error messages. 
	**/
void DieWithSystemMessage(const char *msg);

#endif
