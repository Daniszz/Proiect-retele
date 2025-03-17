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
#include <stdbool.h>

extern int errno;
int sd;
struct sockaddr_in server;
GtkWidget *grid;
GtkWidget *grid_pacient;
GtkWidget *window;
int ok = 0;
int port;

void set_entry_text_color(GtkWidget *entry, const char *color)
{
  GdkColor gdk_color;
  gdk_color_parse(color, &gdk_color);
  gtk_widget_modify_text(entry, GTK_STATE_NORMAL, &gdk_color);
}
void on_entry_activate(GtkWidget *widget, gpointer data)
{
  char msg[1000];
  bzero(msg, 1000);
  strcat(msg, "login : ");
  strcat(msg, gtk_entry_get_text(GTK_ENTRY(widget)));

  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }
  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }

  printf("[client]Mesajul primit este: %s\n", msg);
  if (strcmp(msg, "Introduceti parola") != 0)
  {
    GtkWidget *label = gtk_label_new("Username invalid");
    GdkRGBA color;
    gdk_rgba_parse(&color, "red");
    gtk_widget_override_color(label, GTK_STATE_NORMAL, &color);
    if (GTK_IS_CONTAINER(grid))
    {

      gtk_container_remove(GTK_CONTAINER(grid), gtk_grid_get_child_at(GTK_GRID(grid), 3, 0));
    }
    gtk_grid_attach(GTK_GRID(grid), label, 3, 0, 1, 1);

    gtk_entry_set_text(GTK_ENTRY(widget), "");
  }
  else
  {
    ok = 1;
    GtkWidget *label = gtk_label_new("Username valid");
    GdkRGBA color;
    gdk_rgba_parse(&color, "green");
    gtk_widget_override_color(label, GTK_STATE_NORMAL, &color);
    if (GTK_IS_CONTAINER(grid))
    {

      gtk_container_remove(GTK_CONTAINER(grid), gtk_grid_get_child_at(GTK_GRID(grid), 3, 0));
    }
    gtk_grid_attach(GTK_GRID(grid), label, 3, 0, 1, 1);
    gtk_editable_set_editable(GTK_EDITABLE(widget), FALSE);
  }
  gtk_widget_show_all(grid);
}
void on_entry_activate1(GtkWidget *widget, gpointer data)
{
  if (ok == 1)
  {
    char msg[1000];
    bzero(msg, 1000);

    strcpy(msg, gtk_entry_get_text(GTK_ENTRY(widget)));

    if (write(sd, msg, strlen(msg)) <= 0)
    {
      perror("[client]Eroare la write() spre server.\n");
      return errno;
    }
    bzero(msg, 1000);
    if (read(sd, msg, sizeof(msg)) < 0)
    {
      perror("[client]Eroare la read() de la server.\n");
      return errno;
    }

    printf("[client]Mesajul primit este: %s\n", msg);
    if (strcmp(msg, "User-ul a fost logat") != 0)
    {
      GtkWidget *label = gtk_label_new("Parola invalida");
      GdkRGBA color;
      gdk_rgba_parse(&color, "red");
      gtk_widget_override_color(label, GTK_STATE_NORMAL, &color);
      if (GTK_IS_CONTAINER(grid))
      {

        gtk_container_remove(GTK_CONTAINER(grid), gtk_grid_get_child_at(GTK_GRID(grid), 3, 1));
      }
      gtk_grid_attach(GTK_GRID(grid), label, 3, 1, 1, 1);

      gtk_entry_set_text(GTK_ENTRY(widget), "");
    }
    else
    {
      GtkWidget *label = gtk_label_new("Parola valida");
      GdkRGBA color;
      gdk_rgba_parse(&color, "green");
      gtk_widget_override_color(label, GTK_STATE_NORMAL, &color);
      if (GTK_IS_CONTAINER(grid))
      {

        gtk_container_remove(GTK_CONTAINER(grid), gtk_grid_get_child_at(GTK_GRID(grid), 3, 1));
      }
      gtk_grid_attach(GTK_GRID(grid), label, 3, 1, 1, 1);
      meniu();
      gtk_window_close(GTK_WINDOW(window));
    }
    gtk_widget_show_all(grid);
  }
  else
  {
    GtkWidget *label = gtk_label_new("Niciun user conectat");
    GdkRGBA color;
    gdk_rgba_parse(&color, "red");
    gtk_widget_override_color(label, GTK_STATE_NORMAL, &color);
    if (GTK_IS_CONTAINER(grid))
    {

      gtk_container_remove(GTK_CONTAINER(grid), gtk_grid_get_child_at(GTK_GRID(grid), 3, 0));
    }
    gtk_grid_attach(GTK_GRID(grid), label, 3, 0, 1, 1);
    gtk_entry_set_text(GTK_ENTRY(widget), "");
    gtk_widget_show_all(grid);
  }
}
void close_pacienti(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data))
  {
    gtk_widget_destroy(GTK_WIDGET(data));
  }
  else
  {
    g_warning("Invalid widget reference in close_pacienti.");
  }
}
void close_meniu(GtkWidget *widget, gpointer data)
{
  char msg[1000];
  if (write(sd, "exit", strlen("exit")) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }
  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);
  gtk_main_quit();
  return;
}

void button_pacient(GtkButton *button, gpointer user_data)
{
  char *button_info = (char *)user_data;

  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Pacient");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 1500, 400);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "2");
  strcat(msg, button_info);
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  const char *titles[] = {"Nume:", "Prenume:", "Data si Ora:", "Varsta:", "Greutate:", "Inaltime:",
                          "Ritm Cardiac:", "SpO2:", "Tensiune Sistolica:", "Tensiune Diastolica:",
                          "Temperatura:", "Glicemie:", "Parametri Suplimentari:", "Comentarii:"};

  for (int col = 0; col < 14; col++)
  {
    GtkWidget *button = gtk_button_new_with_label(titles[col]);
    gtk_grid_attach(GTK_GRID(grid1), button, col, 0, 1, 1);
  }
  char *token = strtok(msg, "!");
  int row = 1;
  while (token != NULL)
  {

    for (int col = 0; col < 14; col++)
    {

      GtkWidget *button = gtk_button_new_with_label(token);
      gtk_grid_attach(GTK_GRID(grid1), button, col, row, 1, 1);
      token = strtok(NULL, "!");
    }
    row++;
  }

  gtk_widget_show_all(new_window);
}

void search_entry_changed(GtkSearchEntry *search_entry, gpointer user_data)
{
  GtkGrid *grid = GTK_GRID(user_data);
  const gchar *search_text = gtk_entry_get_text(GTK_ENTRY(search_entry));
  gchar *search_text_lower = g_utf8_strdown(search_text, -1);

  GList *children, *iter;
  children = gtk_container_get_children(GTK_CONTAINER(grid));

  for (iter = children; iter != NULL; iter = iter->next)
  {
    GtkWidget *widget = GTK_WIDGET(iter->data);

    if (GTK_IS_BUTTON(widget))
    {
      const gchar *label_text = gtk_button_get_label(GTK_BUTTON(widget));
      gchar *label_text_lower = g_utf8_strdown(label_text, -1);

      if (strstr(label_text_lower, search_text_lower) != NULL)
        gtk_widget_show(widget);
      else
        gtk_widget_hide(widget);

      g_free(label_text_lower);
    }
  }

  g_free(search_text_lower);
  g_list_free(children);
}

void pacienti()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Meniu");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 200, 300);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "1");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }
  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  grid_pacient = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid_pacient);
  gtk_grid_set_column_spacing(GTK_GRID(grid_pacient), 10);
  gtk_grid_set_row_spacing(GTK_GRID(grid_pacient), 10);

  GtkWidget *search_entry = gtk_search_entry_new();
  g_signal_connect(G_OBJECT(search_entry), "search-changed", G_CALLBACK(search_entry_changed), grid_pacient);
  gtk_grid_attach(GTK_GRID(grid_pacient), search_entry, 0, 0, 3, 1);

  int j = 1;
  char *token = strtok(msg, "\n");
  while (token != NULL)
  {
    char *token_copy = g_strdup(token);

    GtkWidget *button = gtk_button_new_with_label(token_copy);

    g_signal_connect(button, "clicked", G_CALLBACK(button_pacient), (gpointer)token_copy);

    j++;

    gtk_grid_attach(GTK_GRID(grid_pacient), button, 0, j, 1, 1);

    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
  gtk_widget_grab_focus(search_entry);
}
void alerta_bmi()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte_bmi");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "3");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("BMI Categories:\nUnderweight: BMI < 18.5\nNormal weight: 18.5 <= BMI < 24.9\nOverweight: BMI >= 25.0");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}
void alerta_RitmCardiac()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte Ritm Cardiac");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "4");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("Valori normal :1 - 2 ani: intre 80 si 130 bpm. \n3 - 4 ani: intre 80 si 120 bpm. \n5 - 6 ani: intre 75 si 115 bpm. \n6 - 15 ani: intre 70 si 110 bpm. \nPeste 18 ani: intre 60 si 100 bpm.");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}
void alerta_spo()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte sp02");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "5");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("Valori normal : 95-100");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}

void alerta_ts()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte Tensiunea Sistolica");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "6");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("Valori normal : Optima:<120 \n Normal: 120-129 \n Normal inalta:130-139 \n Hipertensiune >=140");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}
void alerta_td()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte Tensiunea Distolica");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "7");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("Valori normal : Optima:<80 \n Normal: 80-84\n Normal inalta:85-89 \n Hipertensiune >=90");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}
void alerta_temp()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte Temperatura");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "8");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("Valoare normala : 36-37 \n Subfrebilitate:37-38 \n Febra moderata:38-39 \n Febra ridicata:39-40 \n Hiperpierxie:40");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}
void alerte_glic()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte Glicemie");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 300, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(new_window), scrolled_window);

  GtkWidget *grid1 = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(scrolled_window), grid1);

  char msg[1000];
  bzero(msg, 1000);
  strcpy(msg, "9");
  if (write(sd, msg, strlen(msg)) <= 0)
  {
    perror("[client]Eroare la write() spre server.\n");
    return errno;
  }

  bzero(msg, 1000);
  if (read(sd, msg, sizeof(msg)) < 0)
  {
    perror("[client]Eroare la read() de la server.\n");
    return errno;
  }
  printf("[client]Mesajul primit este: %s\n", msg);

  GtkWidget *explanation_label = gtk_button_new_with_label("Valoarea normala a glicemiei la adulti este cuprinsa intre 70 si 100 mg/dl,\n a jeun (dimineata, pe nemancate) si intre 101 si 140 mg/dl la 2 ore dupa masa (postprandial).");
  gtk_grid_attach(GTK_GRID(grid1), explanation_label, 0, 0, 2, 1);
  char *token = strtok(msg, "\n");
  int i = 5;
  while (token != NULL)
  {
    GtkWidget *button = gtk_button_new_with_label(token);
    gtk_grid_attach(GTK_GRID(grid1), button, 0, i, 2, 1);
    i++;
    token = strtok(NULL, "\n");
  }

  gtk_widget_show_all(new_window);
}
void alerte()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Alerte");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 200, 200);
  // g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_pacienti), NULL);
  GtkWidget *grid1 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(new_window), grid1);

  GtkWidget *bmi = gtk_button_new_with_label("Alerte BMI");
  g_signal_connect(bmi, "clicked", G_CALLBACK(alerta_bmi), NULL);
  gtk_grid_attach(GTK_GRID(grid1), bmi, 0, 1, 1, 1);

  GtkWidget *cardiac = gtk_button_new_with_label("Alerte Ritm Cardiac");
  g_signal_connect(cardiac, "clicked", G_CALLBACK(alerta_RitmCardiac), NULL);
  gtk_grid_attach(GTK_GRID(grid1), cardiac, 0, 2, 1, 1);

  GtkWidget *spo = gtk_button_new_with_label("Alerta SpO2");
  g_signal_connect(spo, "clicked", G_CALLBACK(alerta_spo), NULL);
  gtk_grid_attach(GTK_GRID(grid1), spo, 0, 3, 1, 1);

  GtkWidget *ts = gtk_button_new_with_label("Alerta Tensiune Sistolica");
  g_signal_connect(ts, "clicked", G_CALLBACK(alerta_ts), NULL);
  gtk_grid_attach(GTK_GRID(grid1), ts, 0, 4, 1, 1);

  GtkWidget *td = gtk_button_new_with_label("Alerta Tensiune Diastolica");
  g_signal_connect(td, "clicked", G_CALLBACK(alerta_td), NULL);
  gtk_grid_attach(GTK_GRID(grid1), td, 0, 5, 1, 1);

  GtkWidget *temp = gtk_button_new_with_label("Alerta Temperatura");
  g_signal_connect(temp, "clicked", G_CALLBACK(alerta_temp), NULL);
  gtk_grid_attach(GTK_GRID(grid1), temp, 0, 6, 1, 1);

  GtkWidget *glic = gtk_button_new_with_label("Alerta Glicemie");
  g_signal_connect(glic, "clicked", G_CALLBACK(alerte_glic), NULL);
  gtk_grid_attach(GTK_GRID(grid1), glic, 0, 7, 1, 1);
  gtk_widget_show_all(new_window);
}

void meniu()
{
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Meniu");
  gtk_container_set_border_width(GTK_CONTAINER(new_window), 10);
  gtk_widget_set_size_request(new_window, 200, 100);
  g_signal_connect(G_OBJECT(new_window), "destroy", G_CALLBACK(close_meniu), NULL);

  GtkWidget *grid1 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(new_window), grid1);

  GtkWidget *pacienti_button = gtk_button_new_with_label("Pacienti");
  g_signal_connect(pacienti_button, "clicked", G_CALLBACK(pacienti), NULL);
  gtk_grid_attach(GTK_GRID(grid1), pacienti_button, 1, 1, 2, 2);

  GtkWidget *alerte_button = gtk_button_new_with_label("Alerte");
  g_signal_connect(alerte_button, "clicked", G_CALLBACK(alerte), NULL);
  gtk_grid_attach(GTK_GRID(grid1), alerte_button, 1, 3, 2, 2);

  gtk_widget_show_all(new_window);
}