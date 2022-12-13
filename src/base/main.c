#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char nome[200];
  char genero[20];
  int duracao;
  char faixaEtaria[10];
  char idioma[30];
} Filme;

char listaGeneros[][20] = { "acao", "aventura", "comedia", "comedia romantica", "documentario", "drama", "faroeste", "fantasia", "ficcao cientifica", "guerra", "misterio", "musical", "policial", "romance", "suspense", "terror" };
char listaIdiomas[][30] = { "alemao", "chines", "espanhol", "frances", "ingles (Estados Unidos)", "italiano", "japones", "polones", "portugues (Brasil)", "portugues (Portugal)", "turco" };
char listaFaixaEtaria[][10] = { "livre", "+10", "+12", "+14", "+16", "+18" };

void adicionaFilmeNaColecao(Filme filme);
void cadastraFilme();
void exibeFilme(Filme filme);
void listaFilmes();
void inicializaColecao();
void organizaEAtualizaFilmeAtual();
void boasVindas();
void exibeMenu();
void exibeFilmePorId(int id);
void deletarFilme(int id);
void atualizaArquivoFilmes();
void leArquivoFilmes();

Filme colecao[999];

int filmeAtual = 0;

void main() {
  leArquivoFilmes();

  boasVindas();
  
  while(1) {
    exibeMenu();
    int opcao;
    scanf("%d", &opcao);
    printf("\n");
    switch(opcao) {
      int filmeId;
      case 1:
        listaFilmes();
        printf("Pressione qualquer botao para voltar ao menu!\n");
        getchar(); getchar();
      break;

      case 2:
        printf("Por favor digite o ID do filme:\n");
        scanf("%d", &filmeId);
        getchar();
        exibeFilmePorId(filmeId);
        printf("Pressione qualquer botao para voltar ao menu!\n");
        getchar();
      break;

      case 3:
        cadastraFilme();
        printf("\033[0;32m");
        printf("Filme cadastrado com sucesso!\n");
        printf("\033[0m");
        exibeFilme(colecao[filmeAtual - 1]);
        printf("Pressione qualquer botao para voltar ao menu!\n");
        getchar();
      break;

      case 4:
        printf("Por favor, digite o ID do filme:\n");
        scanf("%d", &filmeId);
        getchar();
        deletarFilme(filmeId);
        printf("Pressione qualquer botao para voltar ao menu!\n");
        getchar();
      break;

      case 5:
        return 0;
      break;

      default:
        printf("\033[1;31m");
        printf("Opcao invalida! Digite novamente!\n\n");
        printf("\033[0m");
      break;
    }
  }
}

void boasVindas() {
  printf("Bem-vindo ao sistema de cadastro de filmes da NetunoFlix.\n\n");
}

void exibeMenu() {
  printf("Por favor, digite o numero de uma das seguintes opcoes:\n\n");
  printf("1 - Listar filmes cadastrados\n");
  printf("2 - Buscar filme cadastrado\n");
  printf("3 - Cadastrar filme\n");
  printf("4 - Deletar filme\n");
  printf("5 - Sair do sistema\n\n");
}

void exibeFilme(Filme filme) {
  printf("\n*******************************************\n\n");
  printf("Nome do filme: %s\n", filme.nome);
  printf("Genero do filme: %s\n", filme.genero);
  printf("Idioma do filme: %s\n", filme.idioma);
  printf("Duracao do filme: %d minutos\n", filme.duracao);
  printf("Faixa etaria do filme: %s\n", filme.faixaEtaria);
  printf("\n*******************************************\n\n");
}

void exibeFilmePorId(int id) {
  if(id <= 0 || id > filmeAtual) {
    printf("\033[1;31m");
      printf("\nID invalido! Verifique o codigo do filme na lista de filmes cadastrados!\n\n");
    printf("\033[0m");
    return;
  }
  exibeFilme(colecao[id - 1]);
}

void adicionaFilmeNaColecao(Filme filme) {
  colecao[filmeAtual] = filme;
  filmeAtual++;
}

void deletarFilme(int id) {
  if(id <= 0 || id > filmeAtual) {
    printf("\033[1;31m");
      printf("\nID invalido! Verifique o codigo do filme na lista de filmes cadastrados!\n\n");
    printf("\033[0m");
    return;
  }

  sprintf(colecao[id - 1].nome, "");
  sprintf(colecao[id - 1].genero, "");
  colecao[id - 1].duracao = 0;
  sprintf(colecao[id - 1].faixaEtaria, "");
  sprintf(colecao[id - 1].idioma, "");

  organizaEAtualizaFilmeAtual();
  atualizaArquivoFilmes();

  printf("\033[0;32m");
  printf("Filme deletado com sucesso!\n");
  printf("\033[0m");
}

void cadastraFilme() {

  char nome[200];
  char genero[20];
  int duracao;
  char faixaEtaria[10];
  char idioma[20];

  printf("Por favor, digite o nome do filme:\n");

  while(1) {
    getchar();
    fgets(nome, 200, stdin);
    nome[strcspn(nome, "\n")] = 0;

    if(strlen(nome) < 5 && strlen(nome) >= 50) {
      printf("\033[1;31m");
      printf("O nome do filme deve ter mais do que 4 caracteres e menos que 50 caracteres, digite novamente:\n");
      printf("\033[0m");
      continue;
    }
    printf("\n");
    break;
  }

  //////////////////////////////////////////////////////////////////////////////////

  printf("Por favor, digite o genero do filme:\n");
  printf("\033[0;33m");
  printf("Generos disponiveis: ");
  for(int i = 0; i < sizeof(listaGeneros) / sizeof(listaGeneros[0]); i ++) {
    if(i == (sizeof(listaGeneros) / sizeof(listaGeneros[0]) - 1)){
      printf("\"%s\"", listaGeneros[i]);
      break;
    }
    printf("\"%s\", ", listaGeneros[i]);
  }
  printf("\033[0m\n");

  int generoValido = 0;
 
  while(1) {
    fgets(genero, 20, stdin);
    genero[strcspn(genero, "\n")] = 0;

    for(int i = 0; i < sizeof(listaGeneros) / sizeof(listaGeneros[0]); i ++) {
      if(strcmp(genero, listaGeneros[i]) == 0) {
        generoValido = 1;
        break;
      }
    }
    if(generoValido == 1) {
      printf("\n");
      break;
    } else {
      printf("\033[1;31m");
      printf("O genero do filme deve ser uma dessas opcoes:\n");
      for(int i = 0; i < sizeof(listaGeneros) / sizeof(listaGeneros[0]); i ++) {
        if(i == (sizeof(listaGeneros) / sizeof(listaGeneros[0]) - 1)){
          printf("\"%s\"", listaGeneros[i]);
          break;
        }
        printf("\"%s\", ", listaGeneros[i]);
      }
      printf("\nDigite novamente:\n");
      printf("\033[0m");
    }
  }

  //////////////////////////////////////////////////////////////////////////////////
  
  printf("Por favor, digite o idioma do filme:\n");
  printf("\033[0;33m");
  printf("Idiomas disponiveis: ");
  for(int i = 0; i < sizeof(listaIdiomas) / sizeof(listaIdiomas[0]); i ++) {
    if(i == (sizeof(listaIdiomas) / sizeof(listaIdiomas[0]) - 1)){
      printf("\"%s\"", listaIdiomas[i]);
      break;
    }
    printf("\"%s\", ", listaIdiomas[i]);
  }
  printf("\033[0m\n");

  int idiomaValido = 0;

  while(1) {
    fgets(idioma, 30, stdin);
    idioma[strcspn(idioma, "\n")] = 0;

    for(int i = 0; i < sizeof(listaIdiomas) / sizeof(listaIdiomas[0]); i ++) {
      if(strcmp(idioma, listaIdiomas[i]) == 0) {
        idiomaValido = 1;
        break;
      }
    }
    if(idiomaValido == 1) {
      printf("\n");
      break;
    } else {
      printf("\033[1;31m");
      printf("O idioma do filme deve ser uma dessas opcoes:\n");
      for(int i = 0; i < sizeof(listaIdiomas) / sizeof(listaIdiomas[0]); i ++) {
        if(i == (sizeof(listaIdiomas) / sizeof(listaIdiomas[0]) - 1)){
          printf("\"%s\"", listaIdiomas[i]);
          break;
        }
        printf("\"%s\", ", listaIdiomas[i]);
      }
      printf("\nDigite novamente:\n");
      printf("\033[0m");
    }
  }

  //////////////////////////////////////////////////////////////////////////////////

  printf("Por favor, digite o duracao do filme em minutos:\n");

  while(1) {
    scanf("%d", &duracao);
    getchar();
    if(duracao < 0) {
      printf("\033[1;31m");
      printf("A duracao do filme nao pode ser negativa. Digite novamente:\n");
      printf("\033[0m");
      continue;
    } else if(duracao > 32000) {
      printf("\033[1;31m");
      printf("A duracao do filme nao pode ser maior que 32 mil minutos. Digite novamente:\n");
      printf("\033[0m");
      continue;
    }
    printf("\n");
    break;
  }

  //////////////////////////////////////////////////////////////////////////////////

  printf("Por favor, digite a faixa etaria do filme:\n");
  printf("\033[0;33m");
  printf("Classificacoes disponiveis: ");
  for(int i = 0; i < sizeof(listaFaixaEtaria) / sizeof(listaFaixaEtaria[0]); i ++) {
    if(i == (sizeof(listaFaixaEtaria) / sizeof(listaFaixaEtaria[0]) - 1)){
      printf("\"%s\"", listaFaixaEtaria[i]);
      break;
    }
    printf("\"%s\", ", listaFaixaEtaria[i]);
  }
  printf("\033[0m\n");

  int faixaEtariaValida = 0;

  while(1) {
    fgets(faixaEtaria, 10, stdin);
    faixaEtaria[strcspn(faixaEtaria, "\n")] = 0;

    for(int i = 0; i < sizeof(listaFaixaEtaria) / sizeof(listaFaixaEtaria[0]); i ++) {
      if(strcmp(faixaEtaria, listaFaixaEtaria[i]) == 0) {
        faixaEtariaValida = 1;
        break;
      }
    }
    if(faixaEtariaValida == 1) {
      printf("\n");
      break;
    } else {
      printf("\033[1;31m");
      printf("A faixa etaria do filme deve ser uma dessas opcoes:\n");
      for(int i = 0; i < sizeof(listaFaixaEtaria) / sizeof(listaFaixaEtaria[0]); i ++) {
        if(i == (sizeof(listaFaixaEtaria) / sizeof(listaFaixaEtaria[0]) - 1)){
          printf("\"%s\"", listaFaixaEtaria[i]);
          break;
        }
        printf("\"%s\", ", listaFaixaEtaria[i]);
      }
      printf("\nDigite novamente:\n");
      printf("\033[0m");
    }
  }

  //////////////////////////////////////////////////////////////////////////////////

  Filme filme;
  sprintf(filme.nome, nome);
  sprintf(filme.genero, genero);
  filme.duracao = duracao;
  sprintf(filme.faixaEtaria, faixaEtaria);
  sprintf(filme.idioma, idioma);

  adicionaFilmeNaColecao(filme);
  atualizaArquivoFilmes();
}

void organizaEAtualizaFilmeAtual() {
  int filmesContabilizado = 0;
  Filme colecaoAtualizada[999];

  for(int i = 0; i < sizeof colecaoAtualizada / sizeof colecaoAtualizada[0]; i++) {
    sprintf(colecaoAtualizada[i].nome, "");
    sprintf(colecaoAtualizada[i].genero, "");
    colecaoAtualizada[i].duracao = 0;
    sprintf(colecaoAtualizada[i].faixaEtaria, "");
    sprintf(colecaoAtualizada[i].idioma, "");
  }

  for(int i = 0; i < sizeof colecao / sizeof colecao[0]; i++) {
    if(strcmp(colecao[i].nome, "") == 0){
      continue;
    } else {
      colecaoAtualizada[filmesContabilizado] = colecao[i];
      filmesContabilizado++;
    }
  }

  inicializaColecao();

  for(int i = 0; i < sizeof colecaoAtualizada / sizeof colecaoAtualizada[0]; i++) {
    if(colecaoAtualizada[i].nome == ""){
      break;
    } else {
      colecao[i] = colecaoAtualizada[i];
    }
  }

  filmeAtual = filmesContabilizado;
}

void inicializaColecao()  {
  for(int i = 0; i < sizeof colecao / sizeof colecao[0]; i++) {
    sprintf(colecao[i].nome, "");
    sprintf(colecao[i].genero, "");
    colecao[i].duracao = 0;
    sprintf(colecao[i].faixaEtaria, "");
    sprintf(colecao[i].idioma, "");
  }
}

void listaFilmes() {
  printf("Lista dos filmes cadastrados:\n\n");
  if(strcmp(colecao[0].nome, "") == 0) {
    printf("Nao ha filmes cadastrados!\n\n");
  } else {
    for(int i = 0; i < filmeAtual; i++) {
      printf("\033[0;32m");
      printf("Filme %d - %s\n", i + 1, colecao[i].nome);
      printf("\033[0m");
    }
  }
  printf("\n");
}

void atualizaArquivoFilmes() {
  FILE *filmes;
  filmes = fopen("./filmes.txt", "w");
  fprintf(filmes, "");
  fclose(filmes);
  filmes = fopen("./filmes.txt", "a");
  for(int i = 0; i < filmeAtual; i++) {
    if(i == filmeAtual - 1) {
      fprintf(filmes, "%s\n%s\n%d\n%s\n%s\n;", colecao[i].nome, colecao[i].genero, colecao[i].duracao, colecao[i].faixaEtaria, colecao[i].idioma);
    } else {
      fprintf(filmes, "%s\n%s\n%d\n%s\n%s\n", colecao[i].nome, colecao[i].genero, colecao[i].duracao, colecao[i].faixaEtaria, colecao[i].idioma);
    }
  }
  fclose(filmes);
}

void leArquivoFilmes() {
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
          sprintf(filme.nome, buff);
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
    adicionaFilmeNaColecao(filme);
  }
  fclose(filmes);
}