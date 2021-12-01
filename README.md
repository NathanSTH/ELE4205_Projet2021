# ELE4205 projet A2021 Équipe 37
## Text recognition algorithm based on OpenCV and Tesseract OCR and song player

[![Build Status](https://bitbucket.org/rgourdeau/ele4205_projet_aut2021_37/src/master/README.md)](https://bitbucket.org/rgourdeau/ele4205_projet_aut2021_37)

## Authors
- Charland Gagné, Gabriel
- Saint-Hilaire, Nathan

## Description
This program consist of a TCP-IP communication between the computer (client) and a ODROID-C2 module (server). Once the connexion is established, a camera on the server side send a video stream to the client. This program was made as part of the ELE4205 undergraduate course given at Polytechnique Montreal. 

- The client can change resolution at anytime using the keyboard
- A photo-resistor is read through ADC on the server and stop the stream if the luminosity is too low
- Pressing a push-button on the server side allows the client to save a picture in order to use optical character recognition (OCR) and get a series of notes to play back on the server side
- The server then create a fork() and a exec() to treat the buffer received from the client and convert it to usable commands
- A piezo-electric buzzer on the server is able to play two octaves based on the commands sent from the client 

## Features

- Computer vison (OpenCV 2.4.5)
- Deep learning Optical Character Recognition (OCR) based on Tesseract OCR 3.0
- ODROID C-2
- TCP-IP communication

## How to use
- 


## References

[OpenCV Tutorial C++](https://www.opencv-srf.com/p/opencv-lessons.html)

[Beaglebone: Video Capture and Image Processing on Embedded Linux using OpenCV](http://derekmolloy.ie/beaglebone/beaglebone-video-capture-and-image-processing-on-embedded-linux-using-opencv/)

[Streaming Video using RTP on the Beaglebone black](http://derekmolloy.ie/streaming-video-using-rtp-on-the-beaglebone-black/)

[Beaglebone Images, Video and OpenCV](http://derekmolloy.ie/beaglebone-images-video-and-opencv/)

[Video for Linux Two API Specification](https://www.linuxtv.org/downloads/legacy/video4linux/API/V4L2_API/spec-single/v4l2.html)

[TCP/IP Sockets in C (Second Edition) disponible en-ligne à Polytechnique](www.sciencedirect.com/science/book/9780123745408)

[C++ OpenCV image sending through socket](https://stackoverflow.com/questions/20314524/c-opencv-image-sending-through-socket)

[Adaptative Thresholding with OpenCV](https://www.pyimagesearch.com/2021/05/12/adaptive-thresholding-with-opencv-cv2-adaptivethreshold/)

