#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <gtk/gtk.h>

extern int errno;
GtkWidget *entry, *entry1, *entry2, *entry3, *entry4, *entry5, *entry6, *entry7, *entry8, *entry9, *entry10, *entry11, *entry12, *entry13;

int sd;
struct sockaddr_in server;
GtkWidget *grid;
GtkWidget *window;
int ok = 0;
int port;
void send_exit_message()
{
    if (write(sd, "exit", strlen("exit")) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return errno;
    }
}
int format_ora(char *text)
{
    if (strlen(text) != 19)
    {
        return 0;
    }

    if (text[2] != '-' || text[5] != '-' || text[10] != '/' || text[13] != ':' || text[16] != ':')
    {
        return 0;
    }

    int day = atoi(text);
    int month = atoi(text + 3);
    int year = atoi(text + 6);
    int hour = atoi(text + 11);
    int minute = atoi(text + 14);
    int second = atoi(text + 17);

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 9999 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
    {
        return 0;
    }

    return 1;
}

int format_varsta(char *text)
{
    int varsta = atoi(text);
    return (varsta >= 1 && varsta <= 100);
}
int format_greutate(char *text)
{
    float greutate = atof(text);
    return (greutate >= 1 || greutate <= 400);
}
int format_inaltime(char *text)
{
    float inaltime = atof(text);
    return (inaltime >= 60 || inaltime <= 250);
}
int format_ritmCardiac(char *text)
{
    int ritmCardiac = atoi(text);
    return (ritmCardiac >= 30 && ritmCardiac <= 220);
}

int format_spO2(char *text)
{
    int spO2 = atoi(text);
    return (spO2 >= 0 && spO2 <= 100);
}
int format_tensiuneSistolica(char *text)
{
    int tensiuneSistolica = atoi(text);
    return (tensiuneSistolica >= 60 && tensiuneSistolica <= 240);
}

int format_tensiuneDiastolica(char *text)
{
    int tensiuneDiastolica = atoi(text);
    return (tensiuneDiastolica >= 40 && tensiuneDiastolica <= 140);
}

int format_temperatura(char *text)
{
    float temperatura = atof(text);
    return (temperatura >= 35.0 && temperatura <= 42.0);
}

int format_glicemie(char *text)
{
    int glicemie = atoi(text);
    return (glicemie >= 70 && glicemie <= 200);
}
void send_information()
{
    int ok = 1;
    const char *nume = gtk_entry_get_text(GTK_ENTRY(entry));
    const char *prenume = gtk_entry_get_text(GTK_ENTRY(entry1));
    const char *dataOra = gtk_entry_get_text(GTK_ENTRY(entry2));
    const char *varsta = gtk_entry_get_text(GTK_ENTRY(entry3));
    const char *greutate = gtk_entry_get_text(GTK_ENTRY(entry4));
    const char *inaltime = gtk_entry_get_text(GTK_ENTRY(entry5));
    const char *ritmCardiac = gtk_entry_get_text(GTK_ENTRY(entry6));
    const char *spO2 = gtk_entry_get_text(GTK_ENTRY(entry7));
    const char *tensiuneSistolica = gtk_entry_get_text(GTK_ENTRY(entry8));
    const char *tensiuneDiastolica = gtk_entry_get_text(GTK_ENTRY(entry9));
    const char *temperatura = gtk_entry_get_text(GTK_ENTRY(entry10));
    const char *glicemie = gtk_entry_get_text(GTK_ENTRY(entry11));
    const char *parametriSuplimentari = gtk_entry_get_text(GTK_ENTRY(entry12));
    const char *comentarii = gtk_entry_get_text(GTK_ENTRY(entry13));

    GtkWidget *existing_error_label_nume = gtk_grid_get_child_at(GTK_GRID(grid), 26, 0);
    GtkWidget *existing_error_label_prenume = gtk_grid_get_child_at(GTK_GRID(grid), 26, 1);
    GtkWidget *existing_error_label_ora = gtk_grid_get_child_at(GTK_GRID(grid), 26, 2);
    GtkWidget *existing_error_label_varsta = gtk_grid_get_child_at(GTK_GRID(grid), 26, 3);
    GtkWidget *existing_error_label_greutate = gtk_grid_get_child_at(GTK_GRID(grid), 26, 4);
    GtkWidget *existing_error_label_inaltime = gtk_grid_get_child_at(GTK_GRID(grid), 26, 5);
    GtkWidget *existing_error_label_ritmCardiac = gtk_grid_get_child_at(GTK_GRID(grid), 26, 6);
    GtkWidget *existing_error_label_spO2 = gtk_grid_get_child_at(GTK_GRID(grid), 26, 7);
    GtkWidget *existing_error_label_tensiuneSistolica = gtk_grid_get_child_at(GTK_GRID(grid), 26, 8);
    GtkWidget *existing_error_label_tensiuneDiastolica = gtk_grid_get_child_at(GTK_GRID(grid), 26, 9);
    GtkWidget *existing_error_label_temperatura = gtk_grid_get_child_at(GTK_GRID(grid), 26, 10);
    GtkWidget *existing_error_label_glicemie = gtk_grid_get_child_at(GTK_GRID(grid), 26, 11);

    if (existing_error_label_nume != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_nume);
    }
    if (existing_error_label_prenume != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_prenume);
    }
    if (existing_error_label_ora != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_ora);
    }
    if (existing_error_label_varsta != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_varsta);
    }
    if (existing_error_label_greutate != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_greutate);
    }
    if (existing_error_label_inaltime != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_inaltime);
    }
    if (existing_error_label_ritmCardiac != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_ritmCardiac);
    }

    if (existing_error_label_spO2 != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_spO2);
    }

    if (existing_error_label_tensiuneSistolica != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_tensiuneSistolica);
    }

    if (existing_error_label_tensiuneDiastolica != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_tensiuneDiastolica);
    }

    if (existing_error_label_temperatura != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_temperatura);
    }

    if (existing_error_label_glicemie != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(grid), existing_error_label_glicemie);
    }
    if (strlen(nume) == 0)
    {
        GtkWidget *label_nume = gtk_label_new("Nume este obligatoriu!");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_nume, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_nume, 26, 0, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }
    if (strlen(prenume) == 0)
    {
        GtkWidget *label_prenume = gtk_label_new("Prenume este obligatoriu!");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_prenume, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_prenume, 26, 1, 1, 1);
        gtk_widget_show_all(grid);

        ok = 0;
    }
    if (format_ora(dataOra) == 0 || strlen(dataOra) == 0)
    {
        GtkWidget *label_ora = gtk_label_new("Formatul este gresit");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_ora, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_ora, 26, 2, 1, 1);
        gtk_widget_show_all(grid);

        ok = 0;
    }
    if (format_varsta(varsta) == 0 && strlen(varsta) > 0)
    {
        GtkWidget *label_varsta = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_varsta, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_varsta, 26, 3, 1, 1);
        gtk_widget_show_all(grid);

        ok = 0;
    }
    else

        if (format_greutate(greutate) == 0 && strlen(greutate) > 0)
    {
        GtkWidget *label_greutate = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_greutate, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_greutate, 26, 4, 1, 1);
        gtk_widget_show_all(grid);

        ok = 0;
    }
    if (format_inaltime(inaltime) == 0 && strlen(inaltime) > 0)
    {
        GtkWidget *label_inaltime = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_inaltime, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_inaltime, 26, 5, 1, 1);
        gtk_widget_show_all(grid);

        ok = 0;
    }
    if (format_ritmCardiac(ritmCardiac) == 0 && strlen(ritmCardiac) > 0)
    {
        GtkWidget *label_ritmCardiac = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_ritmCardiac, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_ritmCardiac, 26, 6, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }

    if (format_spO2(spO2) == 0 && strlen(spO2) > 0)
    {
        GtkWidget *label_spO2 = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_spO2, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_spO2, 26, 7, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }

    if (format_tensiuneSistolica(tensiuneSistolica) == 0 && strlen(tensiuneSistolica) > 0)
    {
        GtkWidget *label_tensiuneSistolica = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_tensiuneSistolica, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_tensiuneSistolica, 26, 8, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }

    if (format_tensiuneDiastolica(tensiuneDiastolica) == 0 && strlen(tensiuneDiastolica) > 0)
    {
        GtkWidget *label_tensiuneDiastolica = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_tensiuneDiastolica, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_tensiuneDiastolica, 26, 9, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }

    if (format_temperatura(temperatura) == 0 && strlen(temperatura) > 0)
    {
        GtkWidget *label_temperatura = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_temperatura, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_temperatura, 26, 10, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }

    if (format_glicemie(glicemie) == 0 && strlen(glicemie) > 0)
    {
        GtkWidget *label_glicemie = gtk_label_new("Valoare invalida");
        GdkRGBA color;
        gdk_rgba_parse(&color, "red");
        gtk_widget_override_color(label_glicemie, GTK_STATE_NORMAL, &color);
        gtk_grid_attach(GTK_GRID(grid), label_glicemie, 26, 11, 1, 1);
        gtk_widget_show_all(grid);
        ok = 0;
    }

    if (ok == 1)
    {
        char msg[1000];
        snprintf(msg, sizeof(msg), "%s:%s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                 "pacient",
                 (strlen(nume) > 0) ? nume : "-",
                 (strlen(prenume) > 0) ? prenume : "-",
                 (strlen(dataOra) > 0) ? dataOra : "-",
                 (strlen(varsta) > 0) ? varsta : "-",
                 (strlen(greutate) > 0) ? greutate : "-",
                 (strlen(inaltime) > 0) ? inaltime : "-",
                 (strlen(ritmCardiac) > 0) ? ritmCardiac : "-",
                 (strlen(spO2) > 0) ? spO2 : "-",
                 (strlen(tensiuneSistolica) > 0) ? tensiuneSistolica : "-",
                 (strlen(tensiuneDiastolica) > 0) ? tensiuneDiastolica : "-",
                 (strlen(temperatura) > 0) ? temperatura : "-",
                 (strlen(glicemie) > 0) ? glicemie : "-",
                 (strlen(parametriSuplimentari) > 0) ? parametriSuplimentari : "-",
                 (strlen(comentarii) > 0) ? comentarii : "-");

        if (write(sd, msg, strlen(msg)) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");

            return errno;
        }
        bzero(msg, 100);
        if (read(sd, msg, sizeof(msg)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
        }

        printf("[client]Mesajul primit este: %s\n", msg);
    }
}
