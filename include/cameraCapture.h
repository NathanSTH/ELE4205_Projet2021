#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

// pour appels systeme
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace cv;

/**
   \var nbFrames
   \brief Number of frames to capture, to calculate the framerate.
**/
#define nbFrames 10

/**
	\var resX_all
	\brief Array that contains all possible width resolution values for Logitech C270 camera.
	**/
extern uint16_t resX_all[]; // width
/**
	\var resY_all
	\brief Array that contains all possible heigth resolution values for Logitech C270 camera.
	**/
extern uint16_t resY_all[];// height
/**
	\var fps_all
	\brief Array that contains all possible framerate values calculated with get_timing function.
	**/
extern double fps_all[];

/**
	\class camera
	\brief Structure that contains camera configuration : width resolution value, heigth resolution value and fps value.
	**/
struct camera {
  /** \var resX
      \brief Selected resolution width.
  **/
	int resX;
  /** \var resY
      \brief Selected resolution height.
  **/
	int resY;
  /** \var fps
      \brief Selected framerate.
  **/
	double fps;
};

/**
	\var Typedef for struct camera.
	**/
typedef struct camera Camera;

/**	\brief This function computes all framerates values corresponding to the resolution options contained in resX_all and resY_all.
	\return double Returns an array of frame rates.
	**/
double *get_timing();

/**	\brief This function uses the camera to record a video for a selected duration, with the selected camera parameters. This video is then saved to the Odroid-C2 on "/home/root/capture-liv1.avi".
 	\param camera The struct camera object used to record video. Contains resolution width (resX), resolution heigth (resY) and the corresponding framerate (fps).
	\param duration Duration in seconds of the video to record.
	**/
int writeVideo(struct camera, int duration);

/**	\brief This function uses the camera to capture a single frame with the selected camera parameters.
	\param camera The struct camera object used to capture the frame. Contains resolution width (resX), resolution heigth (resY) and the corresponding framerate (fps).
	**/
Mat captureImage(struct camera camera);

bool checkLight(void);

bool initGPIO(void);

bool checkGPIO(void);

#endif
