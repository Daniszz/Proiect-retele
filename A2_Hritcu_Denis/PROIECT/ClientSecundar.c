
#include "ClientSecundar.h"

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  port = atoi(argv[2]);

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(port);

  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Eroare la connect().\n");
    return errno;
  }
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "CLIENT PACIENT");
  g_signal_connect(window, "destroy", G_CALLBACK(send_exit_message), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget *label = gtk_label_new("Nume");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 10);

  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Type here");
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 25, 1);

  GtkWidget *label1 = gtk_label_new("Prenume");
  gtk_grid_attach(GTK_GRID(grid), label1, 0, 1, 1, 1);
  entry1 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry1), "Type here");
  gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 25, 1);

  GtkWidget *label2 = gtk_label_new("Data/ora");
  gtk_grid_attach(GTK_GRID(grid), label2, 0, 2, 1, 1);
  entry2 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry2), "Type here*(format : dd-mm-yyyy/hh:mm:ss)");
  gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 25, 1);

  GtkWidget *label3 = gtk_label_new("Varsta");
  gtk_grid_attach(GTK_GRID(grid), label3, 0, 3, 1, 1);
  entry3 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry3), "Type here*(1-100)");
  gtk_grid_attach(GTK_GRID(grid), entry3, 1, 3, 25, 1);

  GtkWidget *label4 = gtk_label_new("Greutate");
  gtk_grid_attach(GTK_GRID(grid), label4, 0, 4, 1, 1);
  entry4 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry4), "Type here*(kg)(1-400)");
  gtk_grid_attach(GTK_GRID(grid), entry4, 1, 4, 25, 1);

  GtkWidget *label5 = gtk_label_new("Inaltime");
  gtk_grid_attach(GTK_GRID(grid), label5, 0, 5, 1, 1);
  entry5 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry5), "Type here*(cm)(60-250)");
  gtk_grid_attach(GTK_GRID(grid), entry5, 1, 5, 25, 1);

  GtkWidget *label6 = gtk_label_new("RitmCardiac");
  gtk_grid_attach(GTK_GRID(grid), label6, 0, 6, 1, 1);
  entry6 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry6), "Type here*(30-220)");
  gtk_grid_attach(GTK_GRID(grid), entry6, 1, 6, 25, 1);

  GtkWidget *label7 = gtk_label_new("Sp02");
  gtk_grid_attach(GTK_GRID(grid), label7, 0, 7, 1, 1);
  entry7 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry7), "Type here*(0-100)");
  gtk_grid_attach(GTK_GRID(grid), entry7, 1, 7, 25, 1);

  GtkWidget *label8 = gtk_label_new("TensiuneSistolica");
  gtk_grid_attach(GTK_GRID(grid), label8, 0, 8, 1, 1);
  entry8 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry8), "Type here*(60-240)");
  gtk_grid_attach(GTK_GRID(grid), entry8, 1, 8, 25, 1);

  GtkWidget *label9 = gtk_label_new("TensiuneDiastolica");
  gtk_grid_attach(GTK_GRID(grid), label9, 0, 9, 1, 1);
  entry9 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry9), "Type here*(40-140)");
  gtk_grid_attach(GTK_GRID(grid), entry9, 1, 9, 25, 1);

  GtkWidget *label10 = gtk_label_new("Temperatura");
  gtk_grid_attach(GTK_GRID(grid), label10, 0, 10, 1, 1);
  entry10 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry10), "Type here*(35-42)");
  gtk_grid_attach(GTK_GRID(grid), entry10, 1, 10, 25, 1);

  GtkWidget *label11 = gtk_label_new("Glicemie");
  gtk_grid_attach(GTK_GRID(grid), label11, 0, 11, 1, 1);
  entry11 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry11), "Type here*(70-200)");
  gtk_grid_attach(GTK_GRID(grid), entry11, 1, 11, 25, 1);

  GtkWidget *label12 = gtk_label_new("ParametriSuplimentari");
  gtk_grid_attach(GTK_GRID(grid), label12, 0, 12, 1, 1);
  entry12 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry12), "Type here*");
  gtk_grid_attach(GTK_GRID(grid), entry12, 1, 12, 25, 1);

  GtkWidget *label13 = gtk_label_new("Comentarii");
  gtk_grid_attach(GTK_GRID(grid), label13, 0, 13, 1, 1);
  entry13 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry13), "Type here*");
  gtk_grid_attach(GTK_GRID(grid), entry13, 1, 13, 25, 1);

  GtkWidget *send_button = gtk_button_new_with_label("Send");
  g_signal_connect(send_button, "clicked", G_CALLBACK(send_information), NULL);
  gtk_grid_attach(GTK_GRID(grid), send_button, 1, 14, 1, 1);

  gtk_widget_show_all(window);
  gtk_main();

  close(sd);
}
