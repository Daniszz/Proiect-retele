
#include "ClientPrincipal.h"

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
  gtk_window_set_title(GTK_WINDOW(window), "CLIENT MEDIC");

  gtk_window_set_default_size(GTK_WINDOW(window), 400, 100);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget *label = gtk_label_new("Username:");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 10);

  GtkWidget *entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Type here");
  g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), "Username");
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 1, 1);

  GtkWidget *label1 = gtk_label_new("Password:");
  gtk_grid_attach(GTK_GRID(grid), label1, 0, 1, 1, 1);

  GtkWidget *entry1 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry1), "Type here");
  g_signal_connect(entry1, "activate", G_CALLBACK(on_entry_activate1), "Password");
  gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);

  gtk_widget_show_all(window);
  gtk_main();

  close(sd);
}
