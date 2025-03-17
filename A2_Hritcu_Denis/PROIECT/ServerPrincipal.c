#include "server.h"

int main()
{
    createTable();
    struct sockaddr_in server;
    struct sockaddr_in from;
    char msg[MAX_MSG_SIZE];
    int sd;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen(sd, 1) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    while (1)
    {
        int client;
        int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        client = accept(sd, (struct sockaddr *)&from, (socklen_t *)&length);

        if (client < 0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        int pid;
        if ((pid = fork()) == -1)
        {
            close(client);
            continue;
        }
        else if (pid > 0)
        {
            close(client);
            while (waitpid(-1, NULL, WNOHANG) > 0)
                ;
            continue;
        }
        else if (pid == 0)
        {
            close(sd);

            while (1)
            {
                receiveMessage(client, msg);
                if (strcmp(msg, "exit") == 0)
                {
                    printf("[server] Client oprit\n");
                    sendMessage(client, "exit");
                    break;
                }

                if (strstr(msg, "pacient:") != NULL)
                {
                    printf("[server] Serverul a primit mesajul:%s", msg);
                    insert_values(msg + 8);
                }
                else
                {
                    if (logat == 2)
                    {
                        if (strcmp(msg, "9") == 0)
                        {
                            alerte_glic(msg);
                        }
                        else if (strcmp(msg, "8") == 0)
                        {
                            alerte_temp(msg);
                        }
                        else if (strcmp(msg, "7") == 0)
                        {
                            alerte_td(msg);
                        }
                        else if (strcmp(msg, "6") == 0)
                        {
                            alerte_ts(msg);
                        }
                        else if (strcmp(msg, "5") == 0)
                        {
                            alerte_so(msg);
                        }
                        else if (strcmp(msg, "4") == 0)
                        {
                            alerte_ritm(msg);
                        }
                        else if (strcmp(msg, "3") == 0)
                        {
                            alerte_bmi(msg);
                        }
                        else if (strstr(msg, "2") != NULL)
                        {
                            afisare_date(msg + 1);
                            strcpy(msg, msg + 1);
                        }
                        else if (strcmp(msg, "1") == 0)
                        {
                            afisare_pacienti(msg);
                        }
                    }
                    if (logat == 1)
                    {
                        parola_function(msg);
                    }
                    if (logat == 0)
                    {
                        login_function(msg);
                    }
                }

                sendMessage(client, msg);
            }

            close(client);

            exit(EXIT_SUCCESS);
        }
    }

    close(sd);
    return 0;
}
