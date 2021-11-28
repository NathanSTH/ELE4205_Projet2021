#include "../include/PWMSongParser.h"
#include "../include/handleTCP.h"

int main(void)
{
	int clntSock = ConnectToClient(4100);
	uint32_t size;
	char* msg_fork;

	ssize_t numBytesRcvd = recv(clntSock, &size, sizeof(uint32_t), 0);

	size = ntohl(size);

	if (numBytesRcvd < 0){
		close (clntSock); // Close client socket
		DieWithSystemMessage("recv() failed");
	}

	uint32_t messages = htonl(ELE4205_OK);
	ssize_t numBytesSent = send(clntSock, &messages, sizeof(uint32_t), 0);


	if (numBytesSent < 0){
		close (clntSock); // Close client socket
		DieWithSystemMessage("send() failed");
	}
	std::vector<char> buffer(size);

	numBytesRcvd = recv(clntSock, &(buffer[0]), buffer.size(), 0);

	if (numBytesRcvd < 0){
		close (clntSock); // Close client socket
		DieWithSystemMessage("recv() failed");
	}

	std::string receivedString;
	receivedString.assign(&(buffer[0]),buffer.size());

	//SongParser met les actions note on, note off ainsi que la fréquence dans l'invite de commande avec les timings précis
	PWMSongParser parser;

	parser.readBuffer(receivedString);

	//On joue la musique
	parser.play();

	return 0;
}

