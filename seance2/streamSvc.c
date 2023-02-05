#include "streamInc.h"
void dialogueClt(int sd, struct sockaddr_in clt);

int main()
{
	int se, sd;
	struct sockaddr_in svc, clt;
	socklen_t cltLen;
	// Création de la socket de réception d’écoute des appels
	CHECK(se = socket(PF_INET, SOCK_STREAM, 0), "Can't create");
	// Préparation de l’adressage du service (d’appel)
	svc.sin_family = PF_INET;
	svc.sin_port = htons(PORT_SVC);
	svc.sin_addr.s_addr = INADDR_ANY;
	memset(&svc.sin_zero, 0, 8);
	// Association de l’adressage préparé avec la socket d’écoute
	//    PAUSE("Before bind");
	CHECK(bind(se, (struct sockaddr *)&svc, sizeof svc), "Can't bind");
	// Mise en écoute de la socket
	//    PAUSE("Before listen");
	CHECK(listen(se, 2), "Can't calibrate");
	// Boucle permanente de service
	while (1)
	{
		// Attente d’un appel
		cltLen = sizeof(clt);
		//	PAUSE("Before accept");
		CHECK(sd = accept(se, (struct sockaddr *)&clt, &cltLen), "Can't connect");
		// Dialogue avec le client
		dialogueClt(sd, clt);
		close(sd);
	}
	close(se);
	return 0;
}

void dialogueClt(int sd, struct sockaddr_in clt)
{
	char requete[MAX_BUFF];
	do
	{
		// PAUSE("Before read");
		read(sd, buffer, sizeof(buffer));
		sscanf(buffer, "%s:%s", requete, buffer);
		switch (atoi(requete))
		{
			case 0:
				printf("Bye: closing connection\n");
				break;
			case 100:
				//	    PAUSE("Before answer 100");
				write(sd, OK, strlen(OK) + 1);
				printf("OK: recieved message %s\n ", buffer);
				break;
			default:
				//	    PAUSE("Before answer 200");
				write(sd, NOK, strlen(NOK) + 1);
				printf("NOK: recieved %s\n", buffer);
				break;
		}
	} while (atoi(requete) != 0);
}
