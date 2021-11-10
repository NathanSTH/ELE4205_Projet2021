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
//#include <opencv2/highgui/highgui.hpp>

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
   \var RES01
   \brief Flag transmitted from client to server indicating that the desired resolution is 1280*960.
**/
#define RES01 0b000
/**
   \var RES02
   \brief Flag transmitted from client to server indicating that the desired resolution is 700*600.
**/
#define RES02 0b010
/**
   \var RES03
   \brief Flag transmitted from client to server indicating that the desired resolution is 320*240.
**/
#define RES03 0b100
/**
   \var RES04
   \brief Flag transmitted from client to server indicating that the desired resolution is 176*144.
**/
#define RES04 0b110

/**
   \var READY
   \brief Flag transmitted from server to client indicating that the light intensity is OK and that the server is ready to transfer.
**/
#define READY 0b00000

/**
   \var IDOWN
   \brief Flag transmitted from server to client indicating that there is not enough light to capture an image.
**/
#define IDOWN 0b01000

/**
   \var PUSHB
   \brief Flag transmitted from server to client indicating that there is enough light and that the button has been pressed to save the image.
**/
#define PUSHB 0b10000

/**
   \var MASK_STATUS
   \brief Mask used to get the current status of the client : 1 = OK, 0 = QUIT.
**/
#define MASK_STATUS 0b1
/**
   \var MASK_RES
   \brief Mask used to get the current resolution.
**/
#define MASK_RES 0b110

#define MASK_SERV 0b11000

/**
   \var nbRes
   \brief Number of possible resolutions for the Logitech C270.
**/
#define nbRes 13


/**
   \var resX_all
   \brief Array that contains all the available resolutions width for the Logitech C270.
**/
extern uint16_t resX_all[nbRes];
/**
   \var resY_all
   \brief Array that contains all the available resolutions heigth for the Logitech C270.
**/
extern uint16_t resY_all[nbRes];
/**
   \var fps_all
   \brief Array that contains all the available frame rates for the Logitech C270.
**/
extern double fps_all[nbRes];

/**	\brief This function ends the running process while displaying user error messages. 
	**/
void DieWithUserMessage(const char *msg, const char *detail);

/**	\brief This function ends the running process while displaying system error messages. 
	**/
void DieWithSystemMessage(const char *msg);

#endif
