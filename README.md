# ELE4205 projet A2021 Équipe 37
## Text recognition algorithm based on OpenCV and Tesseract OCR

[![Build Status](https://bitbucket.org/rgourdeau/ele4205_projet_aut2021_37/src/master/README.md)](https://bitbucket.org/rgourdeau/ele4205_projet_aut2021_37)

This program consist of a TCP-IP communication between the computer (client) and a ODROID-C2 module (server). Once the connexion is established, a camera on the server side send a video stream to the client

- The client can change resolution at anytime using the keyboard
- A photo-resistor is read through ADC on the server and stop the stream if the luminosity is too low
- Pressing a push-button on the server side allows the client to save a picture in order to use optical character recognition (OCR) and get a series of notes to play back on the server side
- The server then create a fork() and a exec() to treat the buffer received from the client and convert it to usable commands
- A piezo-elctric buzzer on the server is able to play two octaves based on the commands sent from the client 

## Features

- Computer vison (OpenCV)
- Deep learning Optical Character Recognition (OCR) based on Tesseract OCR 3.0
- ODROID C-2
- TCP-IP communication

## How to use

-Clone the repository.

### Build server application
-Install and source the SDK : "environment-setup-aarch64-poky-linux".
-In the ele_projet_aut2021_37/debug_oc2 folder :
	$ cmake -DCMAKE_BUILD_TYPE=Release ./
	$ make
-Upload executable to the Odroid :
	$ scp /<path_to_project>/ele4205_projet_aut2021_37/debug_oc2/capture root@<odroid_ip_address>:/home/root/

### Build song parser application
-Install and source the SDK : "environment-setup-aarch64-poky-linux".
-In the ele_projet_aut2021_37/build_parser folder :
	$ cmake -DCMAKE_BUILD_TYPE=Release ./
	$ make
-Upload executable to the Odroid :
	$ scp /<path_to_project>/ele4205_projet_aut2021_37/build_parser/PWMSongParser root@<odroid_ip_address>:/home/root/

### Build client application
-Create a new folder outside the ele4205_projet_aut2021_37 folder, for example, "build_client".
-In the build_client folder:
	$ cmake -DCMAKE_BUILD_TYPE=Release ../ele4205_aut2021_37/
	$ make

### Run server application
-In the root directory of the Odroid:
	# ./capture 4099
	
### Run client application
-In the build_client application:
	$ ./client <odroid_ip_address> 4099

### Choose resolution
-Press the keyboard key corresponding to the desired resolution.

### Capture and read image
-Press the push-button on the Odroid circuit to capture and read an image.
