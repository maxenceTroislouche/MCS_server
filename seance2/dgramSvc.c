#include "dgramInc.h"

int
main ()
{
  int sock;
  socklen_t cltLen;
  struct sockaddr_in svc, clt;

// Création de la socket de réception des requêtes
  CHECK (sock = socket (PF_INET, SOCK_DGRAM, 0), "Can't create");
// Préparation de l’adressage du service
  svc.sin_family = PF_INET;
  svc.sin_port = htons (PORT_SVC);
  svc.sin_addr.s_addr = INADDR_ANY;
  memset (&svc.sin_zero, 0, 8);
// Association de l’adressage préparé avec la socket créée
// PAUSE ("Before call to bind");
  CHECK (bind (sock, (struct sockaddr *) &svc, sizeof svc), "Can't bind");

// Boucle permanente de service
  while (1)
    {
      cltLen = sizeof (clt);
      // Réception d’un message
      // PAUSE ("Before call to recvfrom");
      CHECK (recvfrom
	     (sock, buffer, sizeof (buffer), 0, (struct sockaddr *) &clt,
	      &cltLen), "Can't receive");

      // Affichage du message reçu
      printf ("Received: [%s] from: [%s]\n", buffer,
	      inet_ntoa (clt.sin_addr));

      char res[100];
      strcpy(res, "OK");
      
      CHECK (sendto
      (sock, res, 100, 0, (struct sockaddr *) &clt,
        sizeof clt), "Can't send");
    }
  close (sock);
  return 0;
}
