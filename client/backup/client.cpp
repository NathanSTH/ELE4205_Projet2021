#include "../include/common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
/*
	argv[1] => Server IP address
	argv[2] => Port number
*/

int main(int argc, char *argv[]){

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	socklen_t addr_len;
	
	if (argc != 3){
		perror("Invalid number of arguments.");
		return 0;
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	portno = atoi(argv[2]);
   	serv_addr.sin_family = AF_INET;
    /* serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
	addr_len = sizeof(serv_addr);*/

	char* servIP = argv[1];

	int rtnVal = inet_pton(AF_INET,servIP,&serv_addr.sin_addr.s_addr);
	// ajouter message d'erreur si adresse invalide
	serv_addr.sin_port = htons(portno);

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0){
		perror("Couldn't create the socket.");
		return 0;
	}

	if (connect(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
		perror("Connection failed.");
		return 0;
	}

	init_packet ipkt;

	if (recv(sockfd,&ipkt,sizeof(ipkt),MSG_WAITALL) < 0){
		perror("Packet couldn't be read.");
		return 0;
	}

	cout << "Le ipkt.msg est : " << ipkt.msg << endl;
	cout << "MSG_INIT = " << MSG_INIT << endl;

	if (ipkt.msg != MSG_INIT){
		perror("Packet is not an initialisation packet");
		return 0;
	}

	ofstream ofs(string(ipkt.filename).c_str(), ios::out | ios::binary);

	long long int bytes_received = 0;
	char buffer[BUFFER_SIZE];

	while (bytes_received != ipkt.filesize){
		if (BUFFER_SIZE > ipkt.filesize - bytes_received){
			if (n=read(sockfd,&buffer,ipkt.filesize-bytes_received) < 0){
				perror("Error during read.");
				return 0;			
			}
		} else {
			if (n = read(sockfd,&buffer,BUFFER_SIZE) < 0){
				perror("Error during read.");
				return 0;
			}
		}

		ofs.write(buffer,n);

		bytes_received += n;
	}

	if (recv(sockfd,&ipkt,sizeof(ipkt),MSG_WAITALL) < 0){
		perror("Packet couldn't be read.");
		return 0;
	}

	if (ipkt.msg != MSG_END){
		perror("Packet is not an end packet.");
		return 0;
	}

	cout << "Closing socket ..." << endl;

	ofs.close();

	if (close(sockfd) < 0){
		perror("Socket failed to close.");
		return 0;
	}
	return 0;
}
