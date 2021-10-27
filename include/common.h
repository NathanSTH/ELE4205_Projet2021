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

#define RES01 0b000
#define RES02 0b010
#define RES03 0b100
#define RES04 0b110

#define MASK_STATUS 0b1
#define MASK_RES 0b110

/**
   \var nbRes
   \brief Number of possible resolutions for the Logitech C270.
**/
#define nbRes 13

extern uint16_t resX_all[nbRes];
extern uint16_t resY_all[nbRes];
extern double fps_all[nbRes];

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
