#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *ntf_window;
  GtkWidget *titulo_input;
  GtkWidget *genero_input;
  GtkWidget *duracao_input;
  GtkWidget *faixa_etaria_input;
  GtkWidget *idioma_input;
  GtkWidget *posicao_input;
  GtkWidget *bt_cadastrar;
  GtkWidget *bt_remover;
  GtkWidget *statusbar;
  GtkListStore *colecao_data;
  GtkTreeView *colecao_table;
  GtkWidget *netuno_logo;
  GtkWidget *netuno_logo_header;
} widgets_t;

typedef struct {
  char titulo[200];
  char genero[20];
  int duracao;
  char faixaEtaria[10];
  char idioma[20];
} Filme;

typedef struct {
  char titulo[200];
  char genero[20];
  int duracao;
  char faixaEtaria[10];
  char idioma[20];
  int posicao;
} Inputs;

void cadastraFilme(GtkListStore *lista, GtkWidget *statusbar, widgets_t *widgets);
void inicializaColecao();
void organizaEAtualizaFilmeAtual();
void deletarFilme(int id, GtkWidget *statusbar, GtkListStore *lista, widgets_t *widgets);
void atualizaStatus(GtkWidget *statusbar, char *status);
void updateColecaoList(GtkListStore *lista);
void atualizaArquivoFilmes();
void leArquivoFilmes(GtkListStore *lista);

Filme colecao[999];
Inputs entradas;

int filmeAtual = 0;

int main(int argc, char *argv[]) {
  
  GtkBuilder *builder;

  widgets_t *widgets = g_slice_new(widgets_t);
  
  gtk_init(&argc, &argv);

  builder = gtk_builder_new();

  gtk_builder_add_from_file(builder, "glade/netunoflix_window.glade", NULL);

  widgets->ntf_window = GTK_WIDGET(gtk_builder_get_object(builder, "ntf_window"));
  widgets->colecao_data = GTK_LIST_STORE(gtk_builder_get_object(builder, "colecao_data"));
  widgets->colecao_table = GTK_TREE_VIEW(gtk_builder_get_object(builder, "colecao_table"));
  widgets->titulo_input = GTK_WIDGET(gtk_builder_get_object(builder, "titulo_input"));
  widgets->genero_input = GTK_WIDGET(gtk_builder_get_object(builder, "genero_input"));
  widgets->duracao_input = GTK_WIDGET(gtk_builder_get_object(builder, "duracao_input"));
  widgets->faixa_etaria_input = GTK_WIDGET(gtk_builder_get_object(builder, "faixa_etaria_input"));
  widgets->idioma_input = GTK_WIDGET(gtk_builder_get_object(builder, "idioma_input"));
  widgets->posicao_input = GTK_WIDGET(gtk_builder_get_object(builder, "posicao_input"));
  widgets->bt_cadastrar = GTK_WIDGET(gtk_builder_get_object(builder, "bt_cadastrar"));
  widgets->bt_remover = GTK_WIDGET(gtk_builder_get_object(builder, "bt_remover"));
  widgets->statusbar = GTK_WIDGET(gtk_builder_get_object(builder, "statusbar"));
  widgets->netuno_logo = GTK_WIDGET(gtk_builder_get_object(builder, "netuno_logo"));
  widgets->netuno_logo_header = GTK_WIDGET(gtk_builder_get_object(builder, "netuno_logo_header"));

  gtk_builder_connect_signals(builder, widgets);

  g_object_unref(builder);

  gtk_widget_show(widgets->ntf_window);

  gtk_image_set_from_file(GTK_IMAGE(widgets->netuno_logo), "./img/full_logo.png");

  GdkPixbuf *tempImage = gtk_image_get_pixbuf(GTK_IMAGE(widgets->netuno_logo));

  GdkPixbuf *scaledImage = gdk_pixbuf_scale_simple(tempImage, 225, 225, 3);

  gtk_image_set_from_pixbuf(GTK_IMAGE(widgets->netuno_logo), scaledImage);

  gtk_image_set_from_file(GTK_IMAGE(widgets->netuno_logo_header), "./img/logo.png");

  tempImage = gtk_image_get_pixbuf(GTK_IMAGE(widgets->netuno_logo_header));

  scaledImage = gdk_pixbuf_scale_simple(tempImage, 50, 50, 3);

  gtk_image_set_from_pixbuf(GTK_IMAGE(widgets->netuno_logo_header), scaledImage);

  leArquivoFilmes(widgets->colecao_data);

  gtk_main();

  g_slice_free(widgets_t, widgets);

  return 0;
}

void on_ntf_window_destroy() {
  gtk_main_quit();
}

void on_ntf_window_show (GtkWidget *ntf_window, void *data) {
  entradas.duracao = 1;
  entradas.posicao = 0;
}

////////////////////////////////////////////////////////////////////////////////

void on_titulo_input_changed (GtkEditable *titulo_input, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  sprintf(entradas.titulo, gtk_entry_get_text(GTK_ENTRY(widgets->titulo_input)));
}

void on_genero_input_changed (GtkComboBox *genero_input, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  sprintf(entradas.genero, gtk_combo_box_get_active_id(GTK_COMBO_BOX(widgets->genero_input)));
}

void on_duracao_input_changed (GtkEditable *duracao_input, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  entradas.duracao = atoi(gtk_entry_get_text(GTK_ENTRY(widgets->duracao_input)));
}

void on_faixa_etaria_input_changed (GtkComboBox *faixa_etaria_input, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  sprintf(entradas.faixaEtaria, gtk_combo_box_get_active_id(GTK_COMBO_BOX(widgets->faixa_etaria_input)));
}

void on_idioma_input_changed (GtkComboBox *idioma_input, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  sprintf(entradas.idioma, gtk_combo_box_get_active_id(GTK_COMBO_BOX(widgets->idioma_input)));
}

void on_bt_remover_clicked (GtkButton *bt_remover, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  int pos = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets->posicao_input));
  deletarFilme(pos, widgets->statusbar, widgets->colecao_data, widgets);
}

void on_bt_cadastrar_clicked (GtkButton *bt_cadastrar, void *data) {
  widgets_t *widgets = (widgets_t *)data;
  cadastraFilme(widgets->colecao_data, widgets->statusbar, widgets);
}

///////////////////////////////////////////////////////////////////////////////

void deletarFilme(int id, GtkWidget *statusbar, GtkListStore *lista, widgets_t *widgets) {
  if(id <= 0 || id > filmeAtual) {
    atualizaStatus(statusbar, "ID inválido! Verifique o código do filme na lista de filmes cadastrados!");
    return;
  }

  sprintf(colecao[id - 1].titulo, "");
  sprintf(colecao[id - 1].genero, "");
  colecao[id - 1].duracao = 0;
  sprintf(colecao[id - 1].faixaEtaria, "");
  sprintf(colecao[id - 1].idioma, "");

  organizaEAtualizaFilmeAtual();
  updateColecaoList(lista);
  atualizaArquivoFilmes();
  atualizaStatus(statusbar, "Filme deletado com sucesso!");
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets->posicao_input), 0);
  return;
}

void atualizaStatus(GtkWidget *statusbar, char *status) {
  gtk_label_set_text(GTK_LABEL(statusbar), status);
  return;
}

int validaInputs(GtkWidget *statusbar) {

  atualizaStatus(statusbar, "");

  if(strlen(entradas.titulo) == 0) {
    atualizaStatus(statusbar, "Por favor, digite um título!");
    return 0;
  }

  for(int i = 0; i < sizeof colecao / sizeof colecao[0]; i++) {
    if(strcmp(colecao[i].titulo, entradas.titulo) == 0){
      atualizaStatus(statusbar, "Já existe um filme cadastrado com esse título! Por favor, digite um título diferente.");
      return 0;
    }
  }

  if(strlen(entradas.titulo) < 5) {
    atualizaStatus(statusbar, "O título deve ter pelo menos 5 caracteres!");
    return 0;
  }

  if(strlen(entradas.titulo) > 50) {
    atualizaStatus(statusbar, "O título não pode ser maior que 50 caracteres!");
    return 0;
  }

  if(strcmp(entradas.genero, "") == 0 || strcmp(entradas.genero, "-1") == 0) {
    atualizaStatus(statusbar, "Por favor, selecione um gênero!");
    return 0;
  }

  if(entradas.duracao < 0) {
    atualizaStatus(statusbar, "A duração não pode ser negativa!");
    return 0;
  } else if(entradas.duracao == 0) {
    atualizaStatus(statusbar, "Por favor, digite uma duração!");
    return 0;
  } else if(entradas.duracao > 32000) {
    atualizaStatus(statusbar, "A duração não pode ser maior do que 32000 minutos!");
  }

  if(strcmp(entradas.faixaEtaria, "") == 0 || strcmp(entradas.faixaEtaria, "-1") == 0) {
    atualizaStatus(statusbar, "Por favor, selecione uma faixa etária!");
    return 0;
  }

  if(strcmp(entradas.idioma, "") == 0 || strcmp(entradas.idioma, "-1") == 0) {
    atualizaStatus(statusbar, "Por favor, selecione um idioma!");
    return 0;
  }

  return 1;
}

void updateColecaoList(GtkListStore *lista) {
  gtk_list_store_clear(lista);
  for(int i = 0; i < filmeAtual; i++) {
    gtk_list_store_insert_with_values(lista, NULL, -1, 0, i + 1, 1, colecao[i].titulo, 2, colecao[i].genero, 3, colecao[i].duracao, 4, colecao[i].faixaEtaria, 5, colecao[i].idioma, -1);
  }
}

void cadastraFilme(GtkListStore *lista, GtkWidget *statusbar, widgets_t *widgets) {
  if(validaInputs(statusbar) == 1) {
    Filme filme;
    sprintf(filme.titulo, entradas.titulo);
    sprintf(filme.genero, entradas.genero);
    filme.duracao = entradas.duracao;
    sprintf(filme.faixaEtaria, entradas.faixaEtaria);
    sprintf(filme.idioma, entradas.idioma);

    colecao[filmeAtual] = filme;
    filmeAtual++;
    updateColecaoList(lista);
    atualizaArquivoFilmes();
    atualizaStatus(statusbar, "Filme adicionado com sucesso!");
    gtk_entry_set_text(GTK_ENTRY(widgets->titulo_input), "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->genero_input), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets->duracao_input), 1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->faixa_etaria_input), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->idioma_input), 0);
  }
  return;
}

void organizaEAtualizaFilmeAtual() {
  int filmesContabilizado = 0;
  Filme colecaoAtualizada[999];

  for(int i = 0; i < sizeof colecaoAtualizada / sizeof colecaoAtualizada[0]; i++) {
    sprintf(colecaoAtualizada[i].titulo, "");
    sprintf(colecaoAtualizada[i].genero, "");
    colecaoAtualizada[i].duracao = 0;
    sprintf(colecaoAtualizada[i].faixaEtaria, "");
    sprintf(colecaoAtualizada[i].idioma, "");
  }

  for(int i = 0; i < sizeof colecao / sizeof colecao[0]; i++) {
    if(strcmp(colecao[i].titulo, "") == 0){
      continue;
    } else {
      colecaoAtualizada[filmesContabilizado] = colecao[i];
      filmesContabilizado++;
    }
  }

  inicializaColecao();

  for(int i = 0; i < sizeof colecaoAtualizada / sizeof colecaoAtualizada[0]; i++) {
    if(colecaoAtualizada[i].titulo == ""){
      break;
    } else {
      colecao[i] = colecaoAtualizada[i];
    }
  }

  filmeAtual = filmesContabilizado;
}

void inicializaColecao()  {
  for(int i = 0; i < sizeof colecao / sizeof colecao[0]; i++) {
    sprintf(colecao[i].titulo, "");
    sprintf(colecao[i].genero, "");
    colecao[i].duracao = 0;
    sprintf(colecao[i].faixaEtaria, "");
    sprintf(colecao[i].idioma, "");
  }
}

void atualizaArquivoFilmes() {
  FILE *filmes;
  filmes = fopen("./filmes.txt", "w");
  fprintf(filmes, "");
  fclose(filmes);
  filmes = fopen("./filmes.txt", "a");
  for(int i = 0; i < filmeAtual; i++) {
    if(i == filmeAtual - 1) {
      fprintf(filmes, "%s\n%s\n%d\n%s\n%s\n;", colecao[i].titulo, colecao[i].genero, colecao[i].duracao, colecao[i].faixaEtaria, colecao[i].idioma);
    } else {
      fprintf(filmes, "%s\n%s\n%d\n%s\n%s\n", colecao[i].titulo, colecao[i].genero, colecao[i].duracao, colecao[i].faixaEtaria, colecao[i].idioma);
    }
  }
  fclose(filmes);
}

void leArquivoFilmes(GtkListStore *lista) {
  FILE *filmes;
  filmes = fopen("./filmes.txt", "r");
  while(1) {
    Filme filme;
    for(int i = 0; i < 5; i++) {
      char buff[255];
      for(int i = 0; i < 255; i++){
        buff[i] = '\0';
      }
      fgets(buff, 255, filmes);
      buff[strcspn(buff, "\n")] = 0;
      if(i == 0 && strcmp(buff, ";") == 0) {
        return;
      }
      if(i == 0 && strcmp(buff, "") == 0) {
        return;
      }
      switch (i){
        case 0:
          sprintf(filme.titulo, buff);
          break;
        case 1:
          sprintf(filme.genero, buff);
          break;
        case 2:
          filme.duracao = atoi(buff);
          break;
        case 3:
          sprintf(filme.faixaEtaria, buff);
          break;
        case 4:
          sprintf(filme.idioma, buff);
          break;
      }
    }
    colecao[filmeAtual] = filme;
    filmeAtual++;
    updateColecaoList(lista);
  }
  fclose(filmes);
}