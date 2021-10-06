/* boneCVtiming.cpp
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */

#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
using namespace std;
using namespace cv;

#define nbRes 12;
#define nbFrames 2;
uint16_t resX [nbRes] = {176,160,320,320,352,432,800,864,960,960,1184,1280,1280}; // width
uint16_t resY [nbRes] = {144,120,176,240,288,240,600,480,544,720,656,720,960}; // height
double fps [nbRes];


double *get_timing()
{
  for (int i = 0; i<=nbRes; i++)
    {
      VideoCapture capture(0);
      capture.set(CV_CAP_PROP_FRAME_WIDTH,resX[i]);
      capture.set(CV_CAP_PROP_FRAME_HEIGHT,resY[i]);
      if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
      }
      Mat frame, edges;

      struct timespec start, end;
      clock_gettime( CLOCK_REALTIME, &start );

      for(int i=0; i<nbFrames; i++){
    	capture >> frame;
    	if(frame.empty()){
          cout << "Failed to capture an image" << endl;
          return -1;
    	}
    	cvtColor(frame, edges, CV_BGR2GRAY);
    	Canny(edges, edges, 0, 30, 3);
      }

      clock_gettime( CLOCK_REALTIME, &end );
      double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
      fps[i] = nbFrames/difference;
      //cout << "It took " << difference << " seconds to process " << frames << " frames" << endl;
      //cout << "Capturing and processing " << frames/difference << " frames per second " << endl;

      imwrite("edges.png", edges);
      imwrite("capture.png", frame);
    }
    
    return fps;
}
