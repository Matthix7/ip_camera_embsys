
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "serveur_Camera.h"



///////////////////////////////////////////////////////////////////


SOCKET socketConnexion = -1;
SOCKET sockCom = -1;
int Client = 0;



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
	return 1;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void app(){
	
	struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);
	
	
	
	socketConnexion = init_connection(PORT);
	
	SOCKADDR_IN csin = { 0 };
	size_t sinsize = sizeof csin;
	
	
	sockCom = accept(socketConnexion, (SOCKADDR *)&csin, &sinsize);
	if(sockCom == SOCKET_ERROR)
	{
		perror("accept()");
	}
	else
	{
		printf("Connexion OK\n");
		Client = 1;
		
	}
	
	fd_set fd;
	int max = socketConnexion;
	
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	
	
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
			
			if(FD_ISSET(sockCom, &fd))
			{
				printf("newMessage \n");
				char demande = 0;
				recv(sockCom, &demande, sizeof(char),0);

				printf(" demande : %d\n", demande);

				if (demande == NULL)
				{
					printf(" Client disconnected \n");
					end_connection(sockCom);
					Client = 0;
				}
				
				if (demande == '1')
				{
					int img = photo();
					//send(sockCom, &rep, sizeof(int),0);		
				}
					
			}
		}	
		
	}
	
	end_connection(sockCom);
	end_connection(socketConnexion);
	
}




int main(int argc, char **argv)
{

   app();


   return EXIT_SUCCESS;
}


