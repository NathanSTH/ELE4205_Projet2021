#ifndef HANDLECLIENT_H
#define HANDLECLIENT_H

#pragma once
#include "common.h"

/**
	\brief This function handles the verification of the pressed key to change resolution or quit.
	*@param waitTime Time to wait for a key entry (in ms).
	*@param messages Variable that contains selected resolution and status.
	**/
void HandleWaitKey(int waitTime, uint32_t &messages,uint32_t &resX, uint32_t &resY, uint32_t &currentRes, uint8_t &esc_flag);
/**
	\brief This function displays the resolution options in the console.
	**/
void PrintResOptions(void);

/**
	\brief This function sends messages flags from the client to the server.
	**/
void sendMsg2Server(int sock, uint32_t messages, uint8_t &esc_flag);

/**
	\brief This function initializes the communication with the server.
	**/
int handleSocket(char *servIP, in_port_t servPort);

#endif
