#include "../include/cameraCapture.h"

using namespace cv;
using namespace std;

double *get_timing()
{
  for (int i = 0; i<13; ++i)
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
      cout << "It took " << difference << " seconds to process " << nbFrames << " frames" << endl;
      //cout << "Capturing and processing " << frames/difference << " frames per second " << endl;

      imwrite("edges.png", edges);
      imwrite("capture.png", frame);
    }

    return fps_all;
}

int writeVideo(struct camera camera, int duration)
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
    VideoWriter oVideoWriter("/home/root/capture-liv1.avi", CV_FOURCC('M', 'J', 'P', 'G'),static_cast<int>(camera.fps), frame_size, true);

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

Mat captureImage(struct camera camera)
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
		//return -1;
	}

	Mat frame = Mat::zeros(camera.resX,camera.resY,CV_8UC3);
	bool isSuccess = cap.read(frame); // read a new frame from the video camera
	cap >> frame;
	cap.release();

	return frame;
}

bool checkLight(void)
{
	int adcValue;
	FILE *fp = fopen("/sys/class/saradc/ch0","r");
	fscanf(fp, "%i", &adcValue);
	fclose(fp);
	if (adcValue < 1000){
		return true;
	} else {
		return false;
	}
}

bool initGPIO(void)
{
	system("echo 228 > /sys/class/gpio/export");
	system("echo in > /sys/class/gpio/gpio228/direction");
	bool gpio = checkGPIO();
	if (gpio == 0 | gpio == 1){
		return true;
	} else {
		return false;
	}
}

bool checkGPIO(void)
{
	int gpioValue;
	FILE *fp = fopen("/sys/class/gpio/gpio228/value", "r");
	fscanf(fp, "%i", &gpioValue);
	fclose(fp);
	if (gpioValue == 0){
		return true;
	} else {
		return false;
	}
}
