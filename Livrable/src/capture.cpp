//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int writeVideo(struct camera, int duration)
{
    //Open the default video camera
    VideoCapture cap(0);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    Size frame_size(camera.resX, camera.resY);

    //Create and initialize the VideoWriter object 
    VideoWriter oVideoWriter("capture-liv1.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 
                                                               camera.fps, frame_size, true); 
    
    //If the VideoWriter object is not initialized successfully, exit the program
    if (oVideoWriter.isOpened() == false) 
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }
    int currentFrame = 0;
    int nbTotFrames = duration*camera.fps;
    while (true)
    {
        Mat frame;
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
