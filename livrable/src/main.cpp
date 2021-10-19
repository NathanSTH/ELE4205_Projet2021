#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/cameraCapture.h"

const int duration = 5;

using namespace std;

Camera camera;

/**
\mainpage
This program calculates the framerate for the different available resolutions of the Logitech C270. It then prompts the user to select a desired resolution and it records a 5 seconds video.
**/

int main(int argc, char** argv)
{
	int choice = 0;
	get_timing();
	while (choice == 0){
		cout << "Enter a number corresponding to the desired resolution : \n";
		for (int i=0; i<nbRes; ++i){
			cout << i+1 << " - Resolution " << resX_all[i] << "x" << resY_all[i] << "\n";
		}
		cout << "Your choice : ";
		cin >> choice;
	}

	camera.resX = resX_all[choice-1];
	camera.resY = resY_all[choice-1];
	camera.fps = fps_all[choice-1];
	writeVideo(camera,duration);
	return 0;
}

