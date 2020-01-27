
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "serveur_Camera.h"



///////////////////////////////////////////////////////////////////


SOCKET socketConnexion = -1;
SOCKET sockCom = -1;
int Client = 0;

////////////////////////////////////////////////////////////////////////
///////////////////Fonction led /////////////////////////////////////
////////////////////////////////////////////////////////////////////////

static int GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}


static int GPIOWrite(int pin, int value)
{	
	#define VALUE_MAX 30
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

////////////////////////////////////////////////////////////////////////
///////////////////Fonction socket /////////////////////////////////////
////////////////////////////////////////////////////////////////////////

static int init_connection(int port)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(port);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, 1) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }
	
   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}



void signals_handler(int signal_number)
{
    printf("Signal catched.\n");
    if (Client == 1)
    {
		int rep = 2;
		send(sockCom, &rep, sizeof(int),0); // si le serveur est interrompu on previent le client
		end_connection(sockCom);
	}
	end_connection(socketConnexion);
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int photo()
{	
	
	system("./v4l2grab -o image.jpg");
	return 1;
}

int sendPhoto(SOCKET sockCom, fd_set fd)
{
	char byte;
	char rep;
    FILE *fjpg = fopen("image.jpg","rb");
    int taille = 0;
    if (fjpg!=NULL) {
        while (feof(fjpg) == 0)
        {
			fread (&byte, sizeof(char),1, fjpg);
			taille++;
		}
		char *msg = {NULL};
		sprintf(msg,"$%d$",taille);
		send(sockCom, msg, sizeof(msg),0);
		printf(" taille : %s \n",msg);
		fseek( fjpg, 0, SEEK_SET );
		char buff[taille];
		int compteur = 0;
		while (feof(fjpg) == 0)
        {
			fread (&byte, sizeof(char),1, fjpg);
			buff[compteur] = byte;
			compteur++;
		}
		
		int test = 0;
        while (test == 0)
        {
	        if(FD_ISSET(sockCom, &fd))
			{	
	    		recv(sockCom, &rep, sizeof(char),0);
	    		test = 1;
	    	}
	   	}
	   	
	   	send(sockCom, buff, sizeof(buff),0);
	   	while (test == 0)
        {
	        if(FD_ISSET(sockCom, &fd))
			{	
	    		recv(sockCom, &rep, sizeof(char),0);
	    		test = 1;
	    	}
	   	}
	}
	
	
	return 1;
	
	
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


int app(){
	
	struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);
	
	
	
	socketConnexion = init_connection(PORT);
	
	SOCKADDR_IN csin = { 0 };
	size_t sinsize = sizeof csin;
	
	
	fd_set fd;
	int max = socketConnexion;
	
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	
	
	if (-1 == GPIOExport(PIN))
	{
		return(1);
	}
		
	if (-1 == GPIODirection(PIN, OUT))
	{
		return(2);
	}
	
	
	
	while(1)
	{
		FD_ZERO(&fd);
		FD_SET(socketConnexion,&fd);
		max = socketConnexion;
		
		
		if (Client == 1){
			max = sockCom > socketConnexion ? sockCom : socketConnexion;
			FD_SET(sockCom,&fd);
		}
		
		
		if (select(max+1, &fd, NULL,NULL, &tv) == -1)
		{
			exit(errno);
		
		}


		if (FD_ISSET(socketConnexion,&fd))
		{
			char rep = '0';
			SOCKET sockComTest = accept(socketConnexion, (SOCKADDR *)&csin, &sinsize);
			printf(" New connection \n");
			//Si un client n'est pas present on l'accepte (rep 1) sinon 
			// on renvoie 0 et on ferme la connexion
			if (Client == 0)
			{
				printf("New Client \n");
				rep = '1';
				send(sockComTest, &rep, sizeof(char),0);
				Client =1;
				sockCom = sockComTest;
			}
			else 
			{
				printf("Client refused \n");
				send(sockComTest, &rep, sizeof(char),0);
				end_connection(sockComTest);
			}
			
			
			
		}

		if (Client == 1) 
		{
			
			GPIOWrite(PIN,1);
			if(FD_ISSET(sockCom, &fd))
			{
				
				printf("newMessage \n");
				char demande = 0;
				recv(sockCom, &demande, sizeof(char),0);

				printf(" demande : %d \n", demande == 49);


				if (demande == 0)
				{
					printf(" Client disconnected \n");
					end_connection(sockCom);
					Client = 0;
				}
				
				if (demande == '1')
				{
					char rep = '1';
					printf("transfert begins\n");
					send(sockCom, &rep, sizeof(char),0);	
					sendPhoto(sockCom,fd);
						
				}
					
			}
		}
		else
		{
			GPIOWrite(PIN,0);
		}	
		
	}
	
	if (-1 == GPIOUnexport(PIN))
		return(4);
	end_connection(sockCom);
	end_connection(socketConnexion);
	
}




int main(int argc, char **argv)
{

   app();


   return EXIT_SUCCESS;
}


