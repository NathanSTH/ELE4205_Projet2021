#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/cameraCapture.h"
#include "../include/common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <cstring>


const int duration = 5;

using namespace std;

Camera camera;

/**
\mainpage
This program calculates the framerate for the different available resolutions of the Logitech C270. It then prompts the user to select a desired resolution and it records a 5 seconds video.
**/

/*int main(int argc, char** argv)
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
    }*/

/*
	argv[1] => port number
	argv[2] => filename
*/

int main(int argc, char *argv[]){
	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t addr_len;
	int n;
	
	if (argc!=3){
		perror("Invalid number of arguments.");
		return 0;
	}

    cout << "Checking filename size..." << endl;
	if (strlen(string(argv[2]).c_str()) > FILENAME_MAX_SIZE){
		perror("File name too long.");
		return 0;	
	}

	portno = atoi(argv[1]);
	
	memset(&serv_addr,0,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
	addr_len = sizeof(serv_addr);

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	cout << "Creating dummy socket..." << endl;
	if (sockfd < 0){
		perror("Cannot create socket.");
		return 0;
	}

    cout << "Binding address and port to socket..." << endl;
	if (bind(sockfd, (struct sockaddr *)&serv_addr,addr_len) < 0){
		perror("Bind failed.");
		return 0;
	}

    cout << "Waiting for client : " << endl;
	if (listen(sockfd,5) < 0){
		perror("Listen failed.");
		return 0;
	}

    

    socklen_t cli_addr_len = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_addr_len);

	if (newsockfd < 0){
		perror("Accept failed.");
		return 0;
	}

	cout << "Connection accepted..." << endl;

	init_packet ipkt;
	memset(&ipkt,0, sizeof(init_packet));
	ipkt.msg = MSG_INIT;

	ipkt.filesize = getFileSize(argv[2]);

	string(argv[2]).copy(ipkt.filename, strlen(argv[2]));

	cout << "Le ipkt.msg est : " << ipkt.msg << endl;

	if (write(sockfd,&ipkt,sizeof(ipkt)) < 0){
		perror("Error during write.");
		return 0;
	}

	ifstream ifs(string(ipkt.filename).c_str(), ios::in | ios::binary);

	MESSAGE msg = MSG_OK;
	ipkt.msg = msg;

	if (write(newsockfd, &ipkt, sizeof(ipkt))){
		perror("Error during write");
		return 0;
	}

	long long int bytes_sent = 0;
	char buffer[BUFFER_SIZE];

	while (bytes_sent != ipkt.filesize){
		ifs.read(buffer, BUFFER_SIZE);

		if(!ifs.eof()){
			if (write(sockfd, &buffer, BUFFER_SIZE)){
				perror("Error during write.");
				return 0;
			}
		} else {
			if (write(sockfd,&buffer,ipkt.filesize-bytes_sent)){
				perror("Error during write.");
				return 0;
			}
		}

		bytes_sent += n;
	}

	msg = MSG_END;

	if (write(sockfd,&msg,sizeof(msg))){
		perror("Error during write.");
		return 0;
	}

	cout << "Closing socket ..." << endl;

	ifs.close();

	if(close(sockfd) < 0){
		perror("Socket failed to close.");
		return 0;
	}

	if(close(newsockfd) < 0){
		perror("Socket failed to close.");
		return 0;
	}

	cout << "Socket closed successfully." << endl;

	return 0;

}
