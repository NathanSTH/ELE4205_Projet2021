#ELE4205_projet_aut2021_37
Basic text recognition algorithm based on OpenCV and Tesseract OCR

This program consist of a TCP-IP communication between the computer (client) and a ODROID-C2 module (server).
Once the connexion is established, a camera on the server side send video stream to the client.

The client can change the desired resolution and close the connexion using the keyboard. 

A photo-resistor is read through ADC on the ODROID and close the connexion if the luminosity is too low.