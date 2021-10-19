#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

struct camera {
  int resX;
  int resY;
  double fps;
  } camera;

#define nbRes 13
#define nbFrames 10
uint16_t resX_all [nbRes] = {176,160,320,320,352,432,800,864,960,960,1184,1280,1280}; // width
uint16_t resY_all [nbRes] = {144,120,176,240,288,240,600,480,544,720,656,720,960}; // height
double fps_all [nbRes];

const int duration = 5;

double *get_timing()
{
  for (int i = 0; i<1; ++i)
    {
      VideoCapture capture(0);
      capture.set(CV_CAP_PROP_FRAME_WIDTH,resX_all[i]);
      capture.set(CV_CAP_PROP_FRAME_HEIGHT,resY_all[i]);
      if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
      }
      Mat frame, edges;
      capture >> frame;
      capture >> frame;

      struct timespec start, end;
      clock_gettime( CLOCK_REALTIME, &start );

      for(int i=0; i<nbFrames; i++){
    	capture >> frame;
    	if(frame.empty()){
          cout << "Failed to capture an image" << endl;
          //return -1;
    	}
    	cvtColor(frame, edges, CV_BGR2GRAY);
    	Canny(edges, edges, 0, 30, 3);
      }

      clock_gettime( CLOCK_REALTIME, &end );
      double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
      fps_all[i] = nbFrames/difference;
      //cout << "It took " << difference << " seconds to process " << frames << " frames" << endl;
      //cout << "Capturing and processing " << frames/difference << " frames per second " << endl;

      imwrite("edges.png", edges);
      imwrite("capture.png", frame);
    }

    return fps_all;
}


int writeVideo(struct camera, int duration)
{
    //Open the default video camera
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,camera.resX);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,camera.resY);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    Size frame_size(camera.resX, camera.resY);

    //Create and initialize the VideoWriter object
    VideoWriter oVideoWriter("/home/root/capture-liv1.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),static_cast<int>(camera.fps), frame_size, true);

    //If the VideoWriter object is not initialized successfully, exit the program
    if (oVideoWriter.isOpened() == false)
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }
    int currentFrame = 0;
    int nbTotalFrames = duration*camera.fps;
    while (true)
    {
        Mat frame;
        frame.rows = camera.resX;
        frame.cols = camera.resY;
        bool isSuccess = cap.read(frame); // read a new frame from the video camera

        //Breaking the while loop if frames cannot be read from the camera
        if (isSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

        if (currentFrame >= nbTotalFrames){
          break;
        }

        //write the video frame to the file
        oVideoWriter.write(frame);
        currentFrame++;
    }

    //Flush and close the video file
    oVideoWriter.release();

    return 0;
}

int main(int argc, char** argv)
{
	get_timing();
	camera.resX = 176;
	camera.resY = 144;
	camera.fps = fps_all[0];
	writeVideo(camera,duration);
	return 0;
}

