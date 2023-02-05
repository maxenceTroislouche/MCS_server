#include "streamInc.h"

#define HIHI "Hihi"
#define PW "pw"
#define ABORT "ABORT"

void dialogueClt(int sd, struct sockaddr_in clt);

void handler(int sig);

int main()
{
    int se, sd;
    struct sockaddr_in svc, clt;
    struct sockaddr_in sock_info;
    socklen_t cltLen, infoLen = sizeof(struct sockaddr);
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

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &action, NULL);

    // On ouvre le fichier user.list
    FILE *user_list = fopen("user.list", "r");
    if (user_list == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier user.list !\n");
        return -1;
    }

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
    }
    return 0;
}

void dialogueClt(int sd, struct sockaddr_in clt)
{
    char buffer[MAX_BUFF];
    

    printf("Lecture du message client ...\n");
    // On lit le message client
    read(sd, buffer, MAX_BUFF);

    printf("Message reçu : %s\n", buffer);

    if (strcmp(buffer, HIHI) == 0)
    {
        // Si Hihi alors on retourne pw
        write(sd, PW, sizeof(PW));
    }
    else 
    {
        // Sinon on retourne abort et on coupe la connexion
        write(sd, ABORT, sizeof(ABORT));
        return;
    }

    // On lit les identifiants du client
    read(sd, buffer, MAX_BUFF);

    printf("L'identification du user est <%s>\n", buffer);

    // On regarde si le format est correct (username:password)
    char *username = strtok(buffer, ":");
    char *password = strtok(NULL, ":");
    char *reste = strtok(NULL, ":");

    printf("Username : %s\n", username);
    printf("Password : %s\n", password);
    printf("Reste : %s\n", reste);

    if (username == NULL || password == NULL || reste != NULL) {
        // Mauvais format
        write(sd, ABORT, sizeof(ABORT));
        return;
    }

    // Bon format
    write(sd, OK, sizeof(OK));

    // On vérifie si les informations se trouvent dans le fichier user.list

    // Si présent alors on récupère le nom de fichier associé

    // On ouvre le fichier correspondant

    // On le lit et on stocke les informations dans un buffer

    // On envoie OK au client

    // On envoie les données du buffer
}

void handler(int sig)
{
    pid_t pid;
    while(pid = waitpid(-1, NULL, WNOHANG) > 0)
    {
        printf("Deleting child : %d\n");        
    }
}
