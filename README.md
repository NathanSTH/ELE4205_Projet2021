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

