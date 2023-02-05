#include "streamInc.h"

void dialogueClt(int sd, struct sockaddr_in clt);

int main()
{
  int se, sd;
  struct sockaddr_in svc, clt;
  struct sockaddr_in sock_info;
  socklen_t cltLen, infoLen=sizeof(struct sockaddr);
  CHECK(se = socket(PF_INET, SOCK_STREAM, 0), "Can't create");
  svc.sin_family = PF_INET;
  svc.sin_port = htons(PORT_SVC);
  svc.sin_addr.s_addr = INADDR_ANY;
  memset(&svc.sin_zero, 0, 8);
  // PAUSE("Before bind");
  CHECK(bind(se, (struct sockaddr *)&svc, sizeof svc), "Can't bindr");
  // PAUSE("Before listen");
  CHECK(listen(se, 5), "Can't calibrate");
  getsockname(se, (struct sockaddr *)&sock_info, &infoLen);
  printf("Server listening on %s, port %d\n", inet_ntop(AF_INET, &sock_info.sin_addr, buffer, sizeof(buffer)), ntohs(sock_info.sin_port));
  cltLen = sizeof(clt);
  while (1)
  {
    CHECK(sd = accept(se, (struct sockaddr *)&clt, &cltLen), "Can't connect");
    getpeername(sd, (struct sockaddr *)&sock_info, &infoLen);
    printf("Connection from client %s on port %d\n", inet_ntop(AF_INET, &sock_info.sin_addr, buffer, sizeof(buffer)), ntohs(sock_info.sin_port));

    int resFork = fork();
    if (resFork == -1)
    {
      printf("Erreur lors de la création du processus fils !\n");
      return -1;
    }
    else if (resFork == 0)
    {
      // Processus fils

      // Fermeture de la socket d'écoute
      close(se);

      // Dialogue avec le client
      dialogueClt(sd, clt);

      // Fermeture de la socket connectée
      close(sd);

      // Fin du process
      exit(0);
    }
    else
    {
      // Processus pere
      
      // On ferme la socket connectée
      close(sd);
    }

    // <<< fork code to put here >>
    	// Le fils prend en charge le client
    	// Le fils ferme la socket d'ecoute
    	// Le fils dialogue avec le client
    	// Le fils se termine
    	//
    	// Le pere ferme la connected socket
	// Le pere doit attendre une nouvelle connection
  }
  return 0;
}

void dialogueClt(int sd, struct sockaddr_in clt)
{
  char requete[MAX_BUFF];
  // PAUSE("Before read");
  read(sd, buffer, sizeof(buffer));
  sscanf(buffer, "%s:%s", requete, buffer);
  switch (atoi(requete))
  {
    case 100:
      write(sd, OK, strlen(OK) + 1);
      printf("OK\n");
      break;
    default:
      write(sd, NOK, strlen(NOK) + 1);
      printf("NOK : received message: %s\n", buffer);
      break;
  }
}
