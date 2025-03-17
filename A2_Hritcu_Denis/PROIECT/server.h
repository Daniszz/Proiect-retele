#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bits/waitflags.h>
#include <sqlite3.h>
#include <time.h>
#include <sys/wait.h>
#define PORT 2164
#define MAX_MSG_SIZE 2000

extern int errno;
char nume[MAX_MSG_SIZE];
int logat = 0;
char *strptime(const char *s, const char *format, struct tm *tm);

void receiveMessage(int client, char *msg)
{
    bzero(msg, MAX_MSG_SIZE);
    printf("[server]Asteptam mesajul...\n");
    fflush(stdout);
    if (read(client, msg, MAX_MSG_SIZE) <= 0)
    {
        perror("[server]Eroare la read() de la client.\n");
        close(client);
        exit(EXIT_FAILURE);
    }

    printf("[server]Mesajul a fost receptionat...%s\n", msg);
}

void sendMessage(int client, const char *msg)
{
    char response[MAX_MSG_SIZE];
    bzero(response, MAX_MSG_SIZE);
    strcat(response, msg);

    printf("[server]Trimitem mesajul inapoi...%s\n", response);

    if (write(client, response, strlen(response) + 1) <= 0)
    {
        perror("[server]Eroare la write() catre client.\n");
        close(client);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[server]Mesajul a fost trasmis cu succes.\n");
    }
}

void createTable()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        fprintf(stderr, "Nu s-a putut deschide/crea baza de date: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "Baza de date deschisa/creata cu succes\n");
    }

    int rows = 0;
    sqlite3_stmt *stmt;
    const char *rowCountSQL = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='MonitorizareSanatate';";
    rc = sqlite3_prepare_v2(db, rowCountSQL, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregatirea interogarii: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        rows = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if (rows == 0)
    {
        fprintf(stdout, "Tabelul nu există, creând tabelul...\n");

        const char *createTableSQL = "CREATE TABLE IF NOT EXISTS MonitorizareSanatate ("
                                     "Nume TEXT NOT NULL,"
                                     "Prenume TEXT NOT NULL,"
                                     "DataOra TEXT NOT NULL,"
                                     "Varsta INTEGER ,"
                                     "Greutate REAL,"
                                     "Inaltime REAL ,"
                                     "RitmCardiac INTEGER ,"
                                     "SpO2 REAL ,"
                                     "TensiuneSistolica INTEGER ,"
                                     "TensiuneDiastolica INTEGER ,"
                                     "Temperatura REAL ,"
                                     "Glicemie REAL ,"
                                     "ParametriSuplimentari TEXT ,"
                                     "Comentarii TEXT"
                                     ");";

        rc = sqlite3_exec(db, createTableSQL, NULL, 0, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Eroare la crearea tabelului: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }
        else
        {
            fprintf(stdout, "Tabel creat cu succes\n");
        }
    }
    else
    {
        fprintf(stdout, "Tabelul există deja\n");
    }

    sqlite3_close(db);
}

void insert_values(char *msg)
{
    char *token;
    char msgCopy[1000];
    strcpy(msgCopy, msg);

    char values[14][100];

    int i = 0;
    token = strtok(msgCopy, " ");
    while (token != NULL && i < 14)
    {
        strcpy(values[i++], token);
        token = strtok(NULL, " ");
    }

    sqlite3 *db;
    int rc;

    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        fprintf(stderr, "Nu s-a putut deschide/crea baza de date: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *insertDataSQL = "INSERT INTO MonitorizareSanatate "
                                "(Nume, Prenume, DataOra, Varsta, Greutate, Inaltime, RitmCardiac, SpO2, TensiuneSistolica, "
                                "TensiuneDiastolica, Temperatura, Glicemie, ParametriSuplimentari, Comentarii) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *insertStmt;
    rc = sqlite3_prepare_v2(db, insertDataSQL, -1, &insertStmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregatirea interogarii: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    for (int j = 0; j < 14; ++j)
    {
        if (strcmp(values[j], "-") != 0)
        {
            sqlite3_bind_text(insertStmt, j + 1, values[j], -1, SQLITE_TRANSIENT);
        }
        else
        {
            sqlite3_bind_null(insertStmt, j + 1);
        }
    }

    rc = sqlite3_step(insertStmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Eroare la inserarea datelor: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(insertStmt);
    sqlite3_close(db);
}
void login_function(char *msg)
{
    char logare[MAX_MSG_SIZE];

    bzero(logare, MAX_MSG_SIZE);
    strcat(logare, msg);
    bzero(nume, MAX_MSG_SIZE);
    strcat(nume, logare + 8);
    logare[8] = '\0';

    if (strcmp(logare, "login : ") == 0)
    {
        if (logat != 2)
        {
            sqlite3 *db;
            int rc;
            rc = sqlite3_open("bazadate.db", &db);

            if (rc)
            {
                perror("[server]Eroare la deschiderea bazei de date");
                exit(EXIT_FAILURE);
            }

            char query[MAX_MSG_SIZE];
            sprintf(query, "SELECT NAME FROM UTILIZATORI WHERE NAME = ?");

            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

            if (rc != SQLITE_OK)
            {
                perror("[server]Eroare la pregatirea interogarii");
                exit(EXIT_FAILURE);
            }

            sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_STATIC);
            rc = sqlite3_step(stmt);

            if (rc == SQLITE_ROW)
            {

                logat = 1;
                bzero(msg, MAX_MSG_SIZE);
                strcat(msg, "Introduceti parola");
            }
            else
            {
                logat = 0;
                nume[0] = '\0';
                printf("[server]User neexistent \n");
                bzero(msg, MAX_MSG_SIZE);
                strcat(msg, "Username invalid");
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        else
        {
            printf("[server]Un user este deja logat \n");
            bzero(msg, MAX_MSG_SIZE);
            strcat(msg, "Un user este deja conectat");
        }
    }
}

void parola_function(char *msg)
{

    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[MAX_MSG_SIZE];
    sprintf(query, "SELECT PASSWORD FROM UTILIZATORI WHERE NAME = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        const char *parola = (const char *)sqlite3_column_text(stmt, 0);

        if (strcmp(parola, msg) == 0)
        {
            printf("[server]User-ul logat \n");
            logat = 2;
            bzero(msg, MAX_MSG_SIZE);
            strcat(msg, "User-ul a fost logat");
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (logat != 2)
    {
        printf("[server]Parola gresita  \n");
        bzero(msg, MAX_MSG_SIZE);
        strcat(msg, "Parola gresita, incercati din nou:");
    }
}
void afisare_date(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate WHERE Nume = ? AND Prenume = ?;");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }

    char copie_msg[MAX_MSG_SIZE];
    strcpy(copie_msg, msg);

    char *nume = strtok(copie_msg, " ");
    char *prenume = strtok(NULL, " ");
    nume[strlen(nume)] = '\0';
    prenume[strlen(prenume)] = '\0';

    sqlite3_bind_text(stmt, 1, nume, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, prenume, -1, SQLITE_STATIC);

    bzero(msg, MAX_MSG_SIZE);

    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcat(msg, result);
            }
            else
                strcat(msg, " ");
            strcat(msg, "!");
        }
    }
    msg[strlen(msg) - 1] = '\0';
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }
    else if (rc == SQLITE_DONE && msg[0] == '\0')
    {
        printf("No matching rows found.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void afisare_pacienti(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[MAX_MSG_SIZE];
    sprintf(query, "SELECT DISTINCT NUME,PRENUME FROM MonitorizareSanatate;");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }

    bzero(msg, MAX_MSG_SIZE);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const char *nume = (const char *)sqlite3_column_text(stmt, 0);
        const char *prenume = (const char *)sqlite3_column_text(stmt, 1);

        strcat(msg, nume);
        strcat(msg, " ");
        strcat(msg, prenume);
        strcat(msg, "\n");
    }

    if (rc != SQLITE_DONE)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void alerte_bmi(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[4][0] != '!' && values[5][0] != '!')
        {
            float greutate = atof(values[4]);
            float inaltime = atof(values[5]);
            float bmi = greutate / ((inaltime / 100) * (inaltime / 100));
            if (bmi < 18.5 || bmi > 24.9)
            {
                char copie_msg[MAX_MSG_SIZE];
                snprintf(copie_msg, MAX_MSG_SIZE, "%.2f %s %s",
                         bmi, values[0], values[1]);
                strcat(msg, copie_msg);
                strcat(msg, "\n");
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void alerte_so(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[7][0] != '!')
        {
            int so = atoi(values[7]);
            if (so < 95)
            {
                char copie_msg[MAX_MSG_SIZE];
                snprintf(copie_msg, MAX_MSG_SIZE, "%d %s %s", so, values[0], values[1]);
                strcat(msg, copie_msg);
                strcat(msg, "\n");
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void alerte_ritm(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[3][0] != '!' && values[6][0] != '!')
        {
            int varsta = atoi(values[3]);
            int ritm_cardiac = atoi(values[6]);
            if ((varsta >= 1 && varsta <= 2 && (ritm_cardiac < 80 || ritm_cardiac > 130)) ||
                (varsta >= 3 && varsta <= 4 && (ritm_cardiac < 80 || ritm_cardiac > 120)) ||
                (varsta >= 5 && varsta <= 6 && (ritm_cardiac < 75 || ritm_cardiac > 115)) ||
                (varsta >= 6 && varsta <= 17 && (ritm_cardiac < 70 || ritm_cardiac > 110)) ||
                (varsta >= 18 && (ritm_cardiac < 60 || ritm_cardiac > 100)))
            {
                char copie_msg[MAX_MSG_SIZE];
                snprintf(copie_msg, MAX_MSG_SIZE, "%d %s %s", ritm_cardiac, values[0], values[1]);
                strcat(msg, copie_msg);
                strcat(msg, "\n");
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void alerte_ts(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[8][0] != '!')
        {
            int ts = atoi(values[8]);
            if (ts >= 140)
            {
                char copie_msg[MAX_MSG_SIZE];
                snprintf(copie_msg, MAX_MSG_SIZE, "%d %s %s", ts, values[0], values[1]);
                strcat(msg, copie_msg);
                strcat(msg, "\n");
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void alerte_td(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[9][0] != '!')
        {
            int td = atoi(values[9]);
            if (td >= 90)
            {
                char copie_msg[MAX_MSG_SIZE];
                snprintf(copie_msg, MAX_MSG_SIZE, "%d %s %s", td, values[0], values[1]);
                strcat(msg, copie_msg);
                strcat(msg, "\n");
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void alerte_temp(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[10][0] != '!')
        {
            float temp = atof(values[10]);
            if (temp >= 39)
            {
                char copie_msg[MAX_MSG_SIZE];
                snprintf(copie_msg, MAX_MSG_SIZE, "%.2f %s %s", temp, values[0], values[1]);
                strcat(msg, copie_msg);
                strcat(msg, "\n");
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void alerte_glic(char *msg)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("bazadate.db", &db);

    if (rc)
    {
        perror("[server]Eroare la deschiderea bazei de date");
        exit(EXIT_FAILURE);
    }

    char query[512];
    snprintf(query, sizeof(query), "SELECT * FROM MonitorizareSanatate");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        perror("[server]Eroare la pregatirea interogarii");
        exit(EXIT_FAILURE);
    }
    bzero(msg, MAX_MSG_SIZE);
    char *values[14];
    for (int i = 0; i < 14; i++)
    {
        values[i] = malloc(MAX_MSG_SIZE);
        if (values[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    int column_count = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        for (int i = 0; i < column_count; i++)
        {
            const char *result = (const char *)sqlite3_column_text(stmt, i);
            if (result != NULL)
            {
                strcpy(values[i], result);
            }
            else
                strcpy(values[i], "!");
        }

        if (values[2][0] != '!' && values[11][0] != '!')
        {
            struct tm timestamp;
            strptime(values[2], "%d-%m-%Y/%H:%M:%S", &timestamp);

            if (timestamp.tm_hour < 12)
            {
                int glicemia = atoi(values[11]);
                if (glicemia < 70 || (glicemia > 100 && glicemia < 101) || glicemia > 140)
                {
                    char copie_msg[MAX_MSG_SIZE];
                    snprintf(copie_msg, MAX_MSG_SIZE, "%d mg/dl %s %s", glicemia, values[0], values[1]);
                    strcat(msg, copie_msg);
                    strcat(msg, "\n");
                }
            }
        }
        for (int i = 0; i < column_count; i++)
        {
            values[i][0] = '\0';
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        perror("[server]Eroare la executarea interogarii");
        exit(EXIT_FAILURE);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
