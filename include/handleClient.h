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

void HandleWaitKey(int waitTime, uint32_t &messages);

void PrintResOptions(void);

void ChangeResClient(Mat &img, uint32_t messages, uint32_t &resX, uint32_t &resY, uint32_t &currentRes, uint32_t &imgSize);

#endif
