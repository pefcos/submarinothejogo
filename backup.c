/*
 * submarino, the jogo
 * por pedro fronchetti costa da silva e tiago dias ferreira
 *
 * universidade federal do rio grande do sul
 * instituto de informatica
 * disciplina de algoritmos e programação - inf021202
 * professora dra mara abel
 * turma c 2019/1
 */

/* bibliotecas utilizadas */
#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <time.h>
#include <windows.h>

/* valores referentes as cores */
#define CORFUNDO 15
#define CORFRENTE 0
#define CORMERGULHADOR 5
#define CORSUBMARINO 4
#define CORAGUA 9
#define CORTORPEDO 2
#define CORINIMIGO 8

/* valores referentes ao menu */
#define TOPOPTION 18
#define BOTOPTION 22
#define NOVO_JOGO 18
#define CARREGA_JOGO 19
#define CREDITOS 20
#define RECORDES 21
#define SAIR 22

/* valores do sleep no gameloop */
#define SLEEP 38

/* valores referentes aos limites do background */
#define LIMCIMA 3
#define LIMBAIXO 25
#define LIMDIR 80
#define LIMESQ 1
#define LIMAGUA 6

/* valores referentes as teclas do teclado */
#define DIREITA 77
#define ESQUERDA 75
#define CIMA 72
#define BAIXO 80
#define ESC 27
#define ESPACO 32

/* valores referentes aos obstaculos */
#define MERGULHADORES 5
#define INIMIGOS 4
#define PROB 3

/* valores referentes a posicao inicial do submarino */
#define XINI 35
#define YINI 4

/* valores referentes as propriedades do submarino */
#define OXIGENIOMAX 3000
#define OXIGENIOVAR 4
#define VIDAS 3
#define TORPEDOS 2
#define CARGA 5

/* valores referentes ao calculo da pontuacao */
#define VALOR_MERGULHADOR 20
#define VALOR_INIMIGO 50

/* valores referentes ao ranking */
#define MAXPOS 30
#define RANKINGS 10
#define TAMANHO_NOME 20

/*
 * estrutura: coordenada
 * ---------------------
 * coordenadas dos objetos na tela do jogo
 */
typedef struct coordenada
{
    int x; /* coordenada x */
    int y; /* coordenada y */
} COORDENADA;

/*
 * estrutura: submarino
 * --------------------
 * parametros do submarino do jogador
 */
typedef struct submarino
{
    COORDENADA se_pos; /* posicao do canto superior esquerdo do submarino */
    int sentido;       /* sentido para qual o submarino aponta */
    int altura;        /* altura, em linhas, do submarino */
    int largura;       /* largura, em colunas, do submarino */
    int oxigenio;      /* quantidade maxima e inicial de oxigenio */
    int vidas;         /* quantidade maxima e inicial de vidas */
    int carga;         /* quantidade de margulhadores dentro do submarino */
} SUBMARINO;

/*
 * estrutura: mergulhador
 * ---------------------_
 * parametros dos mergulhadores
 */
typedef struct mergulhador
{
    COORDENADA se_pos; /* posicao do canto superior esquerdo do mergulhador */
    int largura;       /* altura, em linhas, do mergulhador */
    int status;        /* status do mergulhador */
} MERGULHADOR;

/*
 * estrutura: inimigo
 * ------------------
 * parametros dos submarinos inimigos
 */
typedef struct inimigo
{
    COORDENADA se_pos; /* posicao do canto superior esquerdo do inimigo */
    int altura;        /* altura, em linhas, do inimigo */
    int largura;       /* largura, em colunas, do inimigo */
    int status;        /* status do inimigo */
} INIMIGO;

/*
 * estrutura: torpedo
 * ------------------
 * parametros dos torpedos lançados
 */
typedef struct torpedo
{
    COORDENADA se_pos; /* posicao do canto superior esquerdo do torpedo */
    int largura;       /* largura, em colunas, do torpedo */
    int status;        /* status do torpedo */
} TORPEDO;

/*
 * estrutura: state
 * ----------------
 * guarda estados do jogo para permitir carregamento posterior
 */
typedef struct state
{
    SUBMARINO jogador;                        /* estado do jogador */
    MERGULHADOR mergulhadores[MERGULHADORES]; /* estado dos mergulhadores */
    INIMIGO inimigos[INIMIGOS];               /* estado dos inimigos */
    TORPEDO torpedos[TORPEDOS];               /* estado dos torpedos */
    int pontuacao;                            /* pontuacao do jogador */
    char nome[TAMANHO_NOME];                  /* nome do jogador */
} STATE;

void menu();
void desenha_menu();
void move_cursor_menu(int *menu_input, int *menu_cursor);
void gameloop();
void background();
void desenha_agua();
void mostra_interface(int pontuacao, SUBMARINO jogador);
void atualiza_oxigenio(SUBMARINO *jogador);
void tela_inicial(char *nome);
void tela_jogo();
void desenha_jogador(SUBMARINO jogador);
void apaga_jogador(SUBMARINO jogador);
void move_jogador(int *input, SUBMARINO *jogador, TORPEDO *torpedos);
int testa_posicao_megulhador(SUBMARINO jogador, MERGULHADOR mergulhador);
int testa_posicao_inimigo(SUBMARINO jogador, INIMIGO inimigo);
void pega_mergulhador(SUBMARINO *jogador, MERGULHADOR *mergulhadores);
void destroi_jogador(SUBMARINO *jogador);
void colide_inimigo(SUBMARINO *jogador, INIMIGO *inimigos);
void colide_torpedo(TORPEDO *torpedos, INIMIGO *inimigos, int *pontuacao);
void inicializa_mergulhadores(MERGULHADOR *mergulhadores, INIMIGO *inimigos);
void inicializa_inimigos(INIMIGO *inimigos, MERGULHADOR *mergulhadores);
void lanca_torpedo(TORPEDO *torpedos, SUBMARINO jogador);
void desenha_mergulhador(MERGULHADOR mergulhador);
void desenha_inimigo(INIMIGO inimigo);
void desenha_torpedo(TORPEDO torpedo);
void apaga_mergulhador(MERGULHADOR mergulhador);
void apaga_inimigo(INIMIGO inimigo);
void apaga_torpedo(TORPEDO torpedo);
void move_mergulhador(MERGULHADOR *mergulhadores);
void move_inimigo(INIMIGO *inimigos);
void move_torpedo(TORPEDO *torpedos);
int verifica_vidas(SUBMARINO jogador);
void devolve_mergulhadores(SUBMARINO *jogador, int *pontuacao);
void ponto_aquatico(SUBMARINO jogador, int *pontuacao, int *contador);
void creditos();
int confirma_save();
void salva_jogo(STATE save_state, char *nome_save, int load);
void printbin(char *nome_save);
void load_game(int *pontuacao, char *nome, SUBMARINO *jogador,
               MERGULHADOR *mergulhadores, INIMIGO *inimigos, TORPEDO *torpedos,
               int *erro);
void imprime_ranking();
int carrega_ranking(char nomes[][TAMANHO_NOME], int *pontos);
void salva_ranking(char jog_nome[TAMANHO_NOME], int jog_pontos);

/*
 * funcao: main
 * ------------
 * chama a funcao menu
 */
int main()
{
    menu();
}

/*
 * funcao: menu
 * ------------
 * chama a funcao correspondente a opcao escolhida pelo jogador
 *     menu_input  : recebe input do teclado
 *     menu_cursor : posicao do cursor no menu
 *     endgame     : controla saida do jogo
 */
void menu()
{
    int menu_input = 0;
    int menu_cursor = TOPOPTION;
    int endgame = 0;
    while(endgame == 0)
    {
        menu_cursor = TOPOPTION;
        menu_input = 0;
        background();
        desenha_menu();
        gotoxy(18, TOPOPTION);
        while(menu_input != ESPACO)
        {
            move_cursor_menu(&menu_input, &menu_cursor);
        }
        switch(menu_cursor)
        {
        case SAIR:
            endgame = 1;
            break;
        case NOVO_JOGO:
            gameloop(0);
            break;
        case CARREGA_JOGO:
            gameloop(1);
            break;
        case RECORDES:
            imprime_ranking();
            break;
        case CREDITOS:
            creditos();
            break;
        }
    }
}

/*
 * funcao: move_cursor_menu
 * ------------------------
 * move o cursor do menu de acordo com o input do jogador
 *     menu_input  : recebe input do teclado
 *     menu_cursor : posicao do cursor no menu
 */
void move_cursor_menu(int *menu_input, int *menu_cursor)
{
    if (kbhit())
    {
        fflush(stdin);
        *menu_input = getch();
        switch(*menu_input)
        {
        case BAIXO:
            if(*menu_cursor != BOTOPTION)
            {
                (*menu_cursor)++;
                gotoxy(18, *menu_cursor);
            }
            break;
        case CIMA:
            if(*menu_cursor != TOPOPTION)
            {
                (*menu_cursor)--;
                gotoxy(18, *menu_cursor);
            }
        }
    }
}

/*
 * funcao: desenha_menu
 * --------------------
 * desenha a tela de menu do jogo
 */
void desenha_menu()
{
    cputsxy(10, 8,  "  _____ _    _ ____  __  __          _____  _____ _   _  _"
                    "___  ");
    cputsxy(10, 9,  " / ____| |  | |  _ \\|  \\/  |   /\\   |  __ \\|_   _| \\ "
                    "| |/ __ \\ ");
    cputsxy(10, 10, "| (___ | |  | | |_) | \\  / |  /  \\  | |__) | | | |  \\| "
                    "| |  | |");
    cputsxy(10, 11, " \\___ \\| |  | |  _ <| |\\/| | / /\\ \\ |  _  /  | | | . "
                    "` | |  | |");
    cputsxy(10, 12, " ____) | |__| | |_) | |  | |/ ____ \\| | \\ \\ _| |_| |\\ "
                    " | |__| |");
    cputsxy(10, 13, "|_____/ \\____/|____/|_|  |_/_/    \\_\\_|  \\_\\_____|_| "
                    "\\_|\\____/ ");
    cputsxy(10, 14, "THE JOGO");
    cputsxy(28, 14, "Por: Pedro F. C. da Silva e Tiago D. Ferreira");
    cputsxy(20, 18, "NOVO JOGO");
    cputsxy(20, 19, "CARREGAR JOGO");
    cputsxy(20, 20, "CREDITOS");
    cputsxy(20, 21, "RECORDES");
    cputsxy(20, 22, "SAIR DO JOGO");
}

/*
 * funcao: gameloop
 * ----------------
 * controla as funcoes do jogo
 *     load      : indica se novo jogo ou carregamento de jogo salvo
 *     input     : recebe input do teclado
 *     erro      : indica se ocorreu erro no carregamento do jogo salvo
 *     contador  : conta tempo pra registrar os pontos submersos
 *     pontuacao : recebe pontuacao do jogador
 *     nome      : recebe nome do jogador
 */
void gameloop(int load)
{
    int i;
    int input;
    int erro = 0;
    int contador = 0;
    int pontuacao = 0;
    char nome[20] = " ";

    /* inicializacao das variaveis relacionadas aos objetos que podem ser
     * impressos na tela, todas com seus valores iniciais
     */
    SUBMARINO jogador = {{XINI,YINI},1,2,12,OXIGENIOMAX,VIDAS,0};
    MERGULHADOR mergulhadores[MERGULHADORES] = {{{0,0},4,0},{{0,0},4,0},
                                                {{0,0},4,0},{{0,0},4,0},
                                                {{0,0},4,0}};
    INIMIGO inimigos[INIMIGOS] = {{{0,0},2,9,0},{{0,0},2,9,0},{{0,0},2,9,0},
                                  {{0,0},2,9,0}};
    TORPEDO torpedos[TORPEDOS] = {{{0,0},2,0},{{0,0},2,0}};

    background();
    if(load == 1)
    {
        load_game(&pontuacao, nome, &jogador, mergulhadores, inimigos, torpedos,
                  &erro);
    }
    else
    {
        tela_inicial(nome);
    }
    if(erro == 0)
    {
        background();
        tela_jogo();
        desenha_jogador(jogador);
        desenha_agua();
        mostra_interface(pontuacao, jogador);
        do
        {
            atualiza_oxigenio(&jogador);
            inicializa_mergulhadores(mergulhadores, inimigos);
            inicializa_inimigos(inimigos, mergulhadores);
            move_jogador(&input, &jogador, torpedos);
            Sleep(SLEEP);
            move_mergulhador(mergulhadores);
            move_inimigo(inimigos);
            move_torpedo(torpedos);
            pega_mergulhador(&jogador, mergulhadores);
            devolve_mergulhadores(&jogador, &pontuacao);
            ponto_aquatico(jogador,&pontuacao, &contador);
            colide_inimigo(&jogador, inimigos);
            colide_torpedo(torpedos, inimigos, &pontuacao);
            mostra_interface(pontuacao, jogador);
        }
        while (input != ESC
               && verifica_vidas(jogador));
        if(input == ESC)
        {
            if(confirma_save() == 1)
            {
                STATE save_state;
                save_state.jogador = jogador;
                for(i = 0; i < MERGULHADORES; i++)
                {
                    save_state.mergulhadores[i] = mergulhadores[i];
                }
                for(i = 0; i < INIMIGOS; i++)
                {
                    save_state.inimigos[i] = inimigos[i];
                }
                for(i = 0; i < TORPEDOS; i++)
                {
                    save_state.torpedos[i] = torpedos[i];
                }
                save_state.pontuacao = pontuacao;
                strcpy(save_state.nome, nome);
                salva_jogo(save_state, nome, load);
            }
        }
        else
        {
            salva_ranking(nome,pontuacao);
        }
    }
}

/*
 * funcao: background
 * ------------------
 * modifica a cor do plano de fundo da tela de jogo
 */
void background()
{
    int i, j;
    for (i = 0; i <= 25; i++)
        for (j = 0; j <= 80; j++)
        {
            textbackground(CORFUNDO);
            textcolor(CORFRENTE);
            putchxy(j, i, ' ');
        }
}

/*
 * funcao: desenha_agua
 * --------------------
 * imprime as ondas da superficie da agua
 */
void desenha_agua()
{
    textcolor(CORAGUA);
    int i = 0;
    for(i = LIMESQ+1; i < LIMDIR; i++)
    {
        gotoxy(i,LIMAGUA);
        printf("%c", 247);
    }
    textcolor(CORFRENTE);
}

/*
 * funcao: mostra_interface
 * ------------------------
 * imprime a interface da tela de jogo, mostrando pontuacao, nivel de oxigenio,
 * quantidade atual de vidas e carga
 *     pontuacao : recebe pontuacao do jogador
 *     jogador   : estrutura com os parametros do jogador
 */
void mostra_interface(int pontuacao, SUBMARINO jogador)
{
    int i;
    gotoxy(2, 2);
    printf("Pontos: %i", pontuacao);
    gotoxy(19, 2);
    printf("Oxigenio:  ");
    switch(jogador.oxigenio / 150)
    {
        case 20: printf("||||||||||||||||||||"); break;
        case 19: printf("||||||||||||||||||| "); break;
        case 18: printf("||||||||||||||||||  "); break;
        case 17: printf("|||||||||||||||||   "); break;
        case 16: printf("||||||||||||||||    "); break;
        case 15: printf("|||||||||||||||     "); break;
        case 14: printf("||||||||||||||      "); break;
        case 13: printf("|||||||||||||       "); break;
        case 12: printf("||||||||||||        "); break;
        case 11: printf("|||||||||||         "); break;
        case 10: printf("||||||||||          "); break;
        case 9: printf("|||||||||            "); break;
        case 8: printf("||||||||             "); break;
        case 7: printf("|||||||              "); break;
        case 6: printf("||||||               "); break;
        case 5: printf("|||||                "); break;
        case 4: printf("||||                 "); break;
        case 3: printf("|||                  "); break;
        case 2: printf("||                   "); break;
        case 1: printf("|                    "); break;
        case 0: printf("                     "); break;
    }
    gotoxy(53, 2);
    printf("Vidas: %d", jogador.vidas);
    gotoxy(64, 2);
    printf("Capacidade: %d/%d", jogador.carga, CARGA);
}

/*
 * funcao: atualiza_oxigenio
 * -------------------------
 * controla o nivel de oxigenio do jogador, diminuindo quando submerso e
 * aumentando quando na superficie da agua
 *     jogador : estrutura com os parametros do jogador
 */
void atualiza_oxigenio(SUBMARINO *jogador)
{
    if((*jogador).se_pos.y < LIMAGUA)
    {
        if((*jogador).oxigenio < OXIGENIOMAX)
        {
            (*jogador).oxigenio += (OXIGENIOVAR*10);
            if((*jogador).oxigenio > OXIGENIOMAX)
            {
                (*jogador).oxigenio = OXIGENIOMAX;
            }
        }
    }
    else
    {
        (*jogador).oxigenio -= OXIGENIOVAR;
    }
    if((*jogador).oxigenio < 1)
    {
        (*jogador).vidas = 0;
    }
}

/*
 * funcao: tela_inicial
 * --------------------
 * recebe como input do jogador o seu nome, inserindo "jogador" como default
 * caso nenhum nome seja inserido
 *     nome : recebe nome do jogador
 */
void tela_inicial(char *nome)
{
    gotoxy(30, 10);
    printf("Insira um nome aqui.");
    gotoxy(30, 11);
    gets(nome);
    if (strcmp(nome, "") == 0)
        strcpy(nome, "jogador");
    clrscr();
}

/*
 * funcao: tela_jogo
 * -----------------
 * imprime a moldura da tela de jogo
 */
void tela_jogo()
{
    int i;

    for(i = LIMESQ + 1; i < LIMDIR; i++)
    {
        gotoxy(i, 1);
        printf("%c", 205);
        gotoxy(i, LIMCIMA);
        printf("%c", 205);
        gotoxy(i, LIMBAIXO);
        printf("%c", 205);
    }
    for(i = LIMCIMA + 1; i < LIMBAIXO; i++)
    {
        gotoxy(LIMESQ, i);
        printf("%c", 186);
        gotoxy(LIMDIR, i);
        printf("%c", 186);
    }

    gotoxy(LIMESQ, 2);
    printf("%c", 186);

    gotoxy(17, 1);
    printf("%c", 203);
    gotoxy(17, 2);
    printf("%c", 186);
    gotoxy(17, 3);
    printf("%c", 202);

    gotoxy(51, 1);
    printf("%c", 203);
    gotoxy(51, 2);
    printf("%c", 186);
    gotoxy(51, 3);
    printf("%c", 202);

    gotoxy(62, 1);
    printf("%c", 203);
    gotoxy(62, 2);
    printf("%c", 186);
    gotoxy(62, 3);
    printf("%c", 202);

    gotoxy(LIMDIR, 2);
    printf("%c", 186);

    gotoxy(1, 1);
    printf("%c", 201);
    gotoxy(LIMDIR, 1);
    printf("%c", 187);
    gotoxy(LIMESQ, LIMCIMA);
    printf("%c", 204);
    gotoxy(LIMDIR, LIMCIMA);
    printf("%c", 185);
    gotoxy(LIMESQ, LIMBAIXO);
    printf("%c", 200);
    gotoxy(LIMDIR, LIMBAIXO);
    printf("%c", 188);
}

/*
 * funcao: desenha_jogador
 * -----------------------
 * imprime o submarino do jogador de acordo com sua direcao
 *     jogador : estrutura com os parametros do jogador
 */
void desenha_jogador(SUBMARINO jogador)
{
    textcolor(CORSUBMARINO);
    if(jogador.sentido == 1)
    {
        cputsxy(jogador.se_pos.x, jogador.se_pos.y,    " ___|==|___ ");
        cputsxy(jogador.se_pos.x, jogador.se_pos.y + 1,"|__________)");
    }
    else
    {
        cputsxy(jogador.se_pos.x, jogador.se_pos.y,    " ___|==|___ ");
        cputsxy(jogador.se_pos.x, jogador.se_pos.y + 1,"(__________|");
    }
    textcolor(CORFRENTE);
}

/*
 * funcao: apaga_jogador
 * ---------------------
 * apaga o submarino do jogador
 *     jogador : estrutura com os parametros do jogador
 */
void apaga_jogador(SUBMARINO jogador)
{
    cputsxy(jogador.se_pos.x, jogador.se_pos.y,    "            ");
    cputsxy(jogador.se_pos.x, jogador.se_pos.y + 1,"            ");
}

/*
 * funcao: move_jogador
 * --------------------
 * recebe input do jogador e atualiza posicao do submarino do jogador de acordo
 * com o comando recebido
 *     input    : recebe input do teclado
 *     jogador  : estrutura com os parametros do jogador
 *     torpedos : estrutura com os parametros dos torpedos
 */
void move_jogador(int *input, SUBMARINO *jogador, TORPEDO *torpedos)
{
    if (kbhit())
    {
        fflush(stdin);
        *input = getch();
        switch (*input)
        {
        case CIMA:
            if ((*jogador).se_pos.y > LIMAGUA - 2)
            {
                apaga_jogador(*jogador);
                (*jogador).se_pos.y--;
                desenha_jogador(*jogador);
            }
            break;
        case BAIXO:
            if ((*jogador).se_pos.y < LIMBAIXO - 2)
            {
                apaga_jogador(*jogador);
                (*jogador).se_pos.y++;
                desenha_jogador(*jogador);
            }
            break;
        case DIREITA:
            if ((*jogador).se_pos.x+(*jogador).largura < LIMDIR)
            {
                apaga_jogador(*jogador);
                (*jogador).se_pos.x++;
                (*jogador).sentido = 1;
                desenha_jogador(*jogador);
            }
            break;
        case ESQUERDA:
            if ((*jogador).se_pos.x > LIMESQ + 1)
            {
                apaga_jogador(*jogador);
                (*jogador).se_pos.x--;
                (*jogador).sentido = -1;
                desenha_jogador(*jogador);
            }
            break;
        case ESPACO:
            lanca_torpedo(torpedos, *jogador);
            if(!(((*jogador).se_pos.x <= LIMESQ + 1 + torpedos[0].largura) && (*jogador).sentido == 1) && !(((*jogador).se_pos.x+(*jogador).largura+torpedos[0].largura >= LIMDIR) && (*jogador).sentido == -1))
            {

            }
            break;
        }
        if((*jogador).se_pos.y >= LIMAGUA-2 && (*jogador).se_pos.y <= LIMAGUA+1)
        {
            desenha_agua();
        }
    }
}

/*
 * funcao: testa_posicao_megulhador
 * --------------------------------
 * testa se ocorreu colisao entre submarino do jogador e os mergulhadores
 *     jogador     : estrutura com os parametros do jogador
 *     mergulhador : estrutura com os parametros dos mergulhadores
 */
int testa_posicao_megulhador(SUBMARINO jogador, MERGULHADOR mergulhador)
{
    if((jogador.se_pos.x + jogador.largura >= mergulhador.se_pos.x) && (mergulhador.se_pos.x >= jogador.se_pos.x - mergulhador.largura))
    {
        if((jogador.se_pos.y + jogador.altura > mergulhador.se_pos.y) && (mergulhador.se_pos.y >= jogador.se_pos.y))
        {
            return 1;
        }
    }
    return 0;
}

/*
 * funcao: testa_posicao_inimigo
 * -----------------------------
 * testa se ocorreu colisao entre submarino do jogador e os submarinos inimigos
 *     jogador : estrutura com os parametros do jogador
 *     inimigo : estrutura com os parametros dos inimigos
 */
int testa_posicao_inimigo(SUBMARINO jogador, INIMIGO inimigo)
{
    if((jogador.se_pos.x + jogador.largura >= inimigo.se_pos.x) && (inimigo.se_pos.x >= jogador.se_pos.x - inimigo.largura))
    {
        if((jogador.se_pos.y + jogador.altura > inimigo.se_pos.y) && (inimigo.se_pos.y > jogador.se_pos.y - inimigo.altura))
        {
            return 1;
        }
    }
    return 0;
}

/*
 * funcao: testa_posicao_torpedo
 * -----------------------------
 * testa se ocorreu colisao entre o torpedo lancado pelo submarino do jogador
 * e os submarinos inimigos
 *     torpedo : estrutura com os parametros dos torpedos
 *     inimigo : estrutura com os parametros dos inimigos
 */
int testa_posicao_torpedo(TORPEDO torpedo, INIMIGO inimigo)
{
    if((torpedo.se_pos.x + torpedo.largura >= inimigo.se_pos.x) && (torpedo.se_pos.x <= inimigo.se_pos.x + inimigo.largura))
    {
        if((torpedo.se_pos.y == inimigo.se_pos.y) || (torpedo.se_pos.y == inimigo.se_pos.y + 1))
        {
            if((torpedo.status != 0 && inimigo.status != 0))
            {
                return 1;
            }
        }
    }
    return 0;
}

/*
 * funcao: pega_mergulhador
 * ------------------------
 * havendo colisao entre o submarino do jogador e os mergulhadores, realiza o
 * salvamento do mergulhador
 *     jogador       : estrutura com os parametros do jogador
 *     mergulhadores : estrutura com os parametros dos mergulhadores
 */
void pega_mergulhador(SUBMARINO *jogador, MERGULHADOR *mergulhadores)
{
    int i;
    for (i = 0; i < MERGULHADORES; i++)
    {
        if (testa_posicao_megulhador(*jogador, mergulhadores[i]) == 1)
        {
            apaga_mergulhador(mergulhadores[i]);
            mergulhadores[i].se_pos.x = 0;
            mergulhadores[i].se_pos.y = 0;
            mergulhadores[i].status = 0;
            if((*jogador).carga < CARGA)
                (*jogador).carga++;
            desenha_jogador(*jogador);
        }
    }
}

/*
 * funcao: destroi_jogador
 * -----------------------
 * reseta os parametros do submarino do jogador a cada vez que colide com um
 * submarino inimigo
 *     jogador : estrutura com os parametros do jogador
 */
void destroi_jogador(SUBMARINO *jogador)
{
    apaga_jogador(*jogador);
    (*jogador).se_pos.x = XINI;
    (*jogador).se_pos.y = YINI;
    (*jogador).carga = 0;
    (*jogador).oxigenio = OXIGENIOMAX;
    (*jogador).vidas -= 1;
}

/*
 * funcao: colide_inimigo
 * ----------------------
 * havendo colisao entre o submarino do jogador e um submarino inimigo, apaga
 * ambos os submarinos
 *     jogador  : estrutura com os parametros do jogador
 *     inimigos : estrutura com os parametros dos inimigos
 */
void colide_inimigo(SUBMARINO *jogador, INIMIGO *inimigos)
{
    int i;
    for (i = 0; i < INIMIGOS; i++)
    {
        if (testa_posicao_inimigo(*jogador, inimigos[i]) == 1)
        {
            apaga_inimigo(inimigos[i]);
            inimigos[i].se_pos.x = 0;
            inimigos[i].se_pos.y = 0;
            inimigos[i].status = 0;
            destroi_jogador(jogador);
            desenha_jogador(*jogador);
        }
    }
}

/*
 * funcao: colide_torpedo
 * ----------------------
 * havendo colisao entre o torpedo lancado pelo submarino do jogador e um
 * submarino inimigo, apaga ambos e altera pontuacao de acordo
 *     torpedos  : estrutura com os parametros dos torpedos
 *     inimigos  : estrutura com os parametros dos inimigos
 *     pontuacao : recebe pontuacao do jogador
 */
void colide_torpedo(TORPEDO *torpedos, INIMIGO *inimigos, int *pontuacao)
{
    int i;
    int j;
    for (i = 0; i < TORPEDOS; i++)
    {
        for(j = 0; j < INIMIGOS; j++)
        {
            if (testa_posicao_torpedo(torpedos[i], inimigos[j]) == 1)
            {
                apaga_torpedo(torpedos[i]);
                torpedos[i].se_pos.x = 0;
                torpedos[i].se_pos.y = 0;
                torpedos[i].status = 0;
                apaga_inimigo(inimigos[j]);
                inimigos[j].se_pos.x = 0;
                inimigos[j].se_pos.y = 0;
                inimigos[j].status = 0;
                *pontuacao += VALOR_INIMIGO;
            }
        }
    }
}

/*
 * funcao: inicializa_mergulhadores
 * --------------------------------
 * inicializa, com chance calculada a partir de seed randomica, um mergulhador
 * ainda nao inicializado na tela
 *     mergulhador : estrutura com os parametros dos mergulhadores
 *     inimigo     : estrutura com os parametros dos inimigos
 *     chance      : recebe numero randomico
 */
void inicializa_mergulhadores(MERGULHADOR *mergulhadores, INIMIGO *inimigos)
{
    int i,j;
    int chance = 0;
    srand(time(NULL));
    for (i = 0; i < MERGULHADORES; i++)
    {
        chance = rand() % 3;
        /* verifica se mergulhador nao esta impresso na tela */
        if (mergulhadores[i].status == 0 && chance == 1)
        {
            /* atribui randomicamente a direcao do mergulhador inicializado */
            mergulhadores[i].status = (rand () % PROB) - 1;
            /* se status == -1, inicializa no lado direito da tela */
            if(mergulhadores[i].status == -1)
            {
                mergulhadores[i].se_pos.x = LIMDIR - mergulhadores[i].largura;
                mergulhadores[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA) + (LIMCIMA + LIMAGUA);
            }
            /* se status == 1, inicializa no lado esquerdo da tela */
            else if (mergulhadores[i].status == 1)
            {
                mergulhadores[i].se_pos.x = LIMESQ + 1;
                mergulhadores[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA) + (LIMCIMA + LIMAGUA);
            }

            /* verifica se coordenada y é igual a outros mergulhadores */
            /* ja inicializados, se sim, nao inicializa o mergulhador  */
            for(j = 0; j < MERGULHADORES; j++)
            {
                if((mergulhadores[i].se_pos.y == mergulhadores[j].se_pos.y) && (i != j))
                {
                    mergulhadores[i].status = 0;
                    mergulhadores[i].se_pos.x = 0;
                    mergulhadores[i].se_pos.y = 0;
                }
            }

            /* verifica se coordenada y é igual a de submarinos inimigos */
            /* ja inicializados, se sim, nao inicializa o mergulhador    */
            for(j = 0; j < MERGULHADORES; j++)
            {
                if(mergulhadores[i].se_pos.y == inimigos[j].se_pos.y || mergulhadores[i].se_pos.y == inimigos[j].se_pos.y + 1)
                {
                    mergulhadores[i].status = 0;
                    mergulhadores[i].se_pos.x = 0;
                    mergulhadores[i].se_pos.y = 0;
                }
            }
        }
    }
    /* se status != 0, imprime mergulhador na tela de jogo */
    for(i = 0; i < MERGULHADORES; i++)
        if (mergulhadores[i].status != 0)
            desenha_mergulhador(mergulhadores[i]);
}

/*
 * funcao: inicializa_inimigos
 * ---------------------------
 * inicializa, com chance calculada a partir de seed randomica, um submarino
 * inimigo ainda nao inicializado na tela
 * inimigos      : estrutura com os parametros dos inimigos
 * mergulhadores : estrutura com os parametros dos mergulhadores
 * chance        : recebe numero randomico
 */
void inicializa_inimigos(INIMIGO *inimigos, MERGULHADOR *mergulhadores)
{
    int i, j;
    int chance = 0;
    srand(time(NULL));
    for (i = 0; i < INIMIGOS; i++)
    {
        chance = rand() % 3;
        /* verifica se inimigo nao esta impresso na tela */
        if (inimigos[i].status == 0 && chance == 1)
        {
            /* atribui randomicamente a direcao do inimigo inicializado */
            inimigos[i].status = (rand () % PROB) - 1;
            /* se status == -1, inicializa no lado direito da tela */
            if(inimigos[i].status == -1)
            {
                inimigos[i].se_pos.x = LIMDIR - inimigos[i].largura;
                inimigos[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA-1) + (LIMCIMA + LIMAGUA);
            }
            /* se status == 1, inicializa no lado esquerdo da tela */
            else if (inimigos[i].status == 1)
            {
                inimigos[i].se_pos.x = LIMESQ + 1;
                inimigos[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA-1) + (LIMCIMA + LIMAGUA);
            }

            /* verifica se coordenada y é igual a de mergulhadores */
            /* ja inicializados, se sim, nao inicializa o inimigo  */
            for(j = 0; j < INIMIGOS; j++)
            {
                if((inimigos[i].se_pos.y == mergulhadores[j].se_pos.y || inimigos[i].se_pos.y + 1 == mergulhadores[j].se_pos.y) && (i != j))
                {
                    inimigos[i].status = 0;
                    inimigos[i].se_pos.x = 0;
                    inimigos[i].se_pos.y = 0;
                }
            }

            /* verifica se coordenada y é igual a de inimigos      */
            /* ja inicializados, se sim, nao inicializa o inimigo  */
            for(j = 0; j < INIMIGOS; j++)
            {
                if((inimigos[i].se_pos.y == inimigos[j].se_pos.y || inimigos[i].se_pos.y == inimigos[j].se_pos.y + 1 || inimigos[i].se_pos.y + 1 == inimigos[j].se_pos.y || mergulhadores[i].se_pos.y + 1 == inimigos[j].se_pos.y + 1) && (i != j))
                {
                    inimigos[i].status = 0;
                    inimigos[i].se_pos.x = 0;
                    inimigos[i].se_pos.y = 0;
                }
            }
        }
    }
    /* se status != 0, imprime inimigo na tela de jogo */
    for(i = 0; i < INIMIGOS; i++)
        if (inimigos[i].status != 0)
            desenha_inimigo(inimigos[i]);
}

/*
 * funcao: lanca_torpedo
 * ---------------------
 * inicializa torpedo na mesma direcao do submarino do jogador, limita o maximo
 * de torpedos na tela
 *     torpedos : estrutura com os parametros dos torpedos
 *     jogador  : estrutura com os parametros do jogador
 */
void lanca_torpedo(TORPEDO *torpedos, SUBMARINO jogador)
{
    int i = 0;
    if(jogador.se_pos.y >= LIMAGUA)
    {
        while(i < TORPEDOS)
        {
            if(torpedos[i].status == 0)
            {
                torpedos[i].status = jogador.sentido;
                torpedos[i].se_pos.y = jogador.se_pos.y + 1;
                if(torpedos[i].status == 1)
                {
                    torpedos[i].se_pos.x = jogador.se_pos.x + jogador.largura;
                }
                else
                {
                    torpedos[i].se_pos.x = jogador.se_pos.x - torpedos[i].largura;
                }
            }
            i++;
        }
    }
}

/*
 * funcao: desenha_mergulhador
 * ---------------------------
 * imprime mergulhador na tela de acordo com sua direcao
 *     mergulhador : estrutura com os parametros dos mergulhadores
 */
void desenha_mergulhador(MERGULHADOR mergulhador)
{
    textcolor(CORMERGULHADOR);
    if(mergulhador.status == 1)
    {
        cputsxy(mergulhador.se_pos.x, mergulhador.se_pos.y, ">->o");
    }
    else
    {
        cputsxy(mergulhador.se_pos.x, mergulhador.se_pos.y, "o<-<");
    }
    textcolor(CORFRENTE);
}

/*
 * funcao: desenha_inimigo
 * -----------------------
 * imprime inimigo na tela de acordo com sua direcao
 *     inimigo : estrutura com os parametros dos inimigos
 */
void desenha_inimigo(INIMIGO inimigo)
{
    textcolor(CORINIMIGO);
    if(inimigo.status == 1)
    {
        cputsxy(inimigo.se_pos.x, inimigo.se_pos.y,   " __|=\\___");
        cputsxy(inimigo.se_pos.x, inimigo.se_pos.y+1,  "|_______/");
    }
    else
    {
        cputsxy(inimigo.se_pos.x, inimigo.se_pos.y,    "___/=|__ ");
        cputsxy(inimigo.se_pos.x, inimigo.se_pos.y+1, "\\_______|");
    }
    textcolor(CORFRENTE);
}

/*
 * funcao: desenha_torpedo
 * -----------------------
 * imprime torpedo na tela de acordo com sua direcao
 *     torpedo : estrutura com os parametros dos torpedos
 */
void desenha_torpedo(TORPEDO torpedo)
{
    textcolor(CORTORPEDO);
    if(torpedo.status == 1)
    {
        cputsxy(torpedo.se_pos.x, torpedo.se_pos.y, "=>");
    }
    else
    {
        cputsxy(torpedo.se_pos.x, torpedo.se_pos.y, "<=");
    }
    textcolor(CORFRENTE);
}

/*
 * funcao: apaga_mergulhador
 * -------------------------
 * apaga mergulhador, imprimindo espacos em branco
 *     mergulhador : estrutura com os parametros dos mergulhadores
 */
void apaga_mergulhador(MERGULHADOR mergulhador)
{
    cputsxy(mergulhador.se_pos.x, mergulhador.se_pos.y, "    ");
}

/*
 * funcao: apaga_inimigo
 * ---------------------
 * apaga inimigo, imprimindo espacos em branco
 *     inimigo : estrutura com os parametros dos inimigos
 */
void apaga_inimigo(INIMIGO inimigo)
{
    cputsxy(inimigo.se_pos.x, inimigo.se_pos.y,   "         ");
    cputsxy(inimigo.se_pos.x, inimigo.se_pos.y+1, "         ");
}

/*
 * funcao: apaga_torpedo
 * ---------------------
 * apaga torpedo, imprimindo espacos em branco
 * torpedo : estrutura com os parametros dos torpedos
 */
void apaga_torpedo(TORPEDO torpedo)
{
    cputsxy(torpedo.se_pos.x, torpedo.se_pos.y, "  ");
}

/*
 * funcao: move_mergulhador
 * ------------------------
 * atualiza posicao dos mergulhadores, de acordo com sua direcao
 *     mergulhadores : estrutura com os parametros dos mergulhadores
 */
void move_mergulhador(MERGULHADOR *mergulhadores)
{
    int i;
    for (i = 0; i < MERGULHADORES; i++)
    {
        switch(mergulhadores[i].status)
        {
        case 1:
            if (mergulhadores[i].se_pos.x < LIMDIR - mergulhadores[i].largura)
            {
                apaga_mergulhador(mergulhadores[i]);
                mergulhadores[i].se_pos.x++;
                desenha_mergulhador(mergulhadores[i]);
            }
            else if (mergulhadores[i].se_pos.x == LIMDIR - mergulhadores[i].largura)
            {
                apaga_mergulhador(mergulhadores[i]);
                mergulhadores[i].se_pos.x = 0;
                mergulhadores[i].se_pos.y = 0;
                mergulhadores[i].status = 0;
            }
            break;
        case -1:
            if(mergulhadores[i].se_pos.x > LIMESQ + 2)
            {
                apaga_mergulhador(mergulhadores[i]);
                mergulhadores[i].se_pos.x--;
                desenha_mergulhador(mergulhadores[i]);
            }
            else if (mergulhadores[i].se_pos.x == LIMESQ + 2)
            {
                apaga_mergulhador(mergulhadores[i]);
                mergulhadores[i].se_pos.x = 0;
                mergulhadores[i].se_pos.y = 0;
                mergulhadores[i].status = 0;
            }
            break;
        }
    }
}

/*
 * funcao: move_inimigo
 * --------------------
 * atualiza posicao dos inimigos, de acordo com sua direcao
 *     inimigos : estrutura com os parametros dos inimigos
 */
void move_inimigo(INIMIGO *inimigos)
{
    int i;
    for (i = 0; i < INIMIGOS; i++)
    {
        switch(inimigos[i].status)
        {
        case 1:
            if (inimigos[i].se_pos.x < LIMDIR - inimigos[i].largura)
            {
                apaga_inimigo(inimigos[i]);
                inimigos[i].se_pos.x++;
                desenha_inimigo(inimigos[i]);
            }
            else if (inimigos[i].se_pos.x == LIMDIR - inimigos[i].largura)
            {
                apaga_inimigo(inimigos[i]);
                inimigos[i].se_pos.x = 0;
                inimigos[i].se_pos.y = 0;
                inimigos[i].status = 0;
            }
            break;
        case -1:
            if(inimigos[i].se_pos.x > LIMESQ + 2)
            {
                apaga_inimigo(inimigos[i]);
                inimigos[i].se_pos.x--;
                desenha_inimigo(inimigos[i]);
            }
            else if (inimigos[i].se_pos.x == LIMESQ + 2)
            {
                apaga_inimigo(inimigos[i]);
                inimigos[i].se_pos.x = 0;
                inimigos[i].se_pos.y = 0;
                inimigos[i].status = 0;
            }
            break;
        }
    }
}

/*
 * funcao: move_torpedo
 * --------------------
 * atualiza posicao dos torpedos, de acordo com sua direcao
 *     torpedos : estrutura com os parametros dos torpedos
 */
void move_torpedo(TORPEDO *torpedos)
{
    int i;
    for (i = 0; i < TORPEDOS; i++)
    {
        switch(torpedos[i].status)
        {
        case 1:
            if (torpedos[i].se_pos.x < LIMDIR - torpedos[i].largura - 1)
            {
                apaga_torpedo(torpedos[i]);
                torpedos[i].se_pos.x += 2;
                desenha_torpedo(torpedos[i]);
            }
            else if (torpedos[i].se_pos.x == LIMDIR - torpedos[i].largura || torpedos[i].se_pos.x == LIMDIR - torpedos[i].largura - 1)
            {
                apaga_torpedo(torpedos[i]);
                torpedos[i].se_pos.x = 0;
                torpedos[i].se_pos.y = 0;
                torpedos[i].status = 0;
            }
            break;
        case -1:
            if(torpedos[i].se_pos.x > LIMESQ + 2 + 1)
            {
                apaga_torpedo(torpedos[i]);
                torpedos[i].se_pos.x -= 2;
                desenha_torpedo(torpedos[i]);
            }
            else if (torpedos[i].se_pos.x == LIMESQ + 2 || torpedos[i].se_pos.x == LIMESQ + 2 + 1)
            {
                apaga_torpedo(torpedos[i]);
                torpedos[i].se_pos.x = 0;
                torpedos[i].se_pos.y = 0;
                torpedos[i].status = 0;
            }
            break;
        }
    }
}

/*
 * funcao: verifica_vidas
 * ----------------------
 * retorna 1 se o jogador esta vivo e 0 se ele esta morto
 *     jogador : estrutura com os parametros do jogador
 */
int verifica_vidas(SUBMARINO jogador)
{
    if(jogador.vidas > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * funcao: devolve_mergulhadores
 * -----------------------------
 * zera carga do submarino, aumentando pontuacao de acordo com valor
 * especificado, quando submarino retorno a superficie
 *     jogador   : estrutura com os parametros do jogador
 *     pontuacao : recebe pontuacao do jogador
 */
void devolve_mergulhadores(SUBMARINO *jogador, int *pontuacao)
{
    if((*jogador).se_pos.y < LIMAGUA)
    {
        *pontuacao += (*jogador).carga * VALOR_MERGULHADOR;
        (*jogador).carga = 0;
    }
}

/*
 * funcao: ponto_aquatico
 * ----------------------
 * incrementa pontuacao em 1 pra cada 21 ciclos submerso
 *     jogador   : estrutura com os parametros do jogador
 *     pontuacao : recebe pontuacao do jogador
 *     contador  : conta tempo pra registrar os pontos submersos
 */
void ponto_aquatico(SUBMARINO jogador, int *pontuacao, int *contador)
{
    if(jogador.se_pos.y >= LIMAGUA)
    {
        if(*contador == 21)
        {
            (*pontuacao)++;
            *contador = 0;
        }
        else
        {
            (*contador)++;
        }
    }
}

/*
 * funcao: creditos
 * ----------------
 * exibe creditos do jogo
 */
void creditos()
{
    background();
    cputsxy(29,10,"Jogo desenvolvido por:");
    cputsxy(25,12,"Pedro Fronchetti Costa da Silva");
    cputsxy(31,13,"Tiago Dias Ferreira");
    getch();
}

/*
 * funcao: confirma_save
 * ---------------------
 * retorna 1 caso jogador deseja salvar o jogo e 0 caso contario
 * opcao : recebe input do teclado
 */
int confirma_save()
{
    char opcao;
    background();
    gotoxy(30, 10);
    printf("Deseja salvar o jogo?");
    opcao = getchar();
    if(opcao == 's')
    {
        return 1;
    }
    else
    {
        return 0;
    }
    clrscr();
}

/*
 * funcao: salva_jogo
 * ------------------
 * salva estado atual do jogo no buffer com nome do jogador, indicando que a
 * partida a ser iniciada parte de um jogo salvo
 *     save_state : estrutura com os parametros do estado do jogo
 *     nome_save  : recebe nome do save
 *     load       : indica se novo jogo ou carregamento de jogo salvo
 */
void salva_jogo(STATE save_state, char *nome_save, int load)
{
    FILE *save;
    STATE state_buffer;
    if(load == 1)
    {
        save = fopen(nome_save,"wb");
    }
    else
    {
        save = fopen(strcat(nome_save,".bin"),"wb");
    }
    if(!save)
    {
        background();
        printf("Erro ao salvar jogo.");
    }
    else
    {
        state_buffer = save_state;
        fwrite(&state_buffer, sizeof(STATE),1,save);
    }
    fclose(save);
}

/*
 * funcao: printbin
 * ----------------
 * salva conteudo do buffer em binario com mesmo nome que o jogador
 *     nome_save recebe nome do save
 */
void printbin(char *nome_save)
{
    int i;
    background();
    FILE *save;
    STATE state_buffer;
    tela_inicial(nome_save);
    save = fopen(strcat(nome_save,".bin"),"rb");
    if(fread(&state_buffer,sizeof(STATE),1,save) == 1)
    {
        gotoxy(1,1);
        printf("Oxigenio:\n");
        printf("%d\n", state_buffer.jogador.oxigenio);
        printf("Mergulhadores:\n");
        for(i = 0; i < MERGULHADORES; i++)
        {
            printf("%d\n",state_buffer.mergulhadores[i].se_pos.x);
        }
        printf("Inimigos:\n");
        for(i = 0; i < INIMIGOS; i++)
        {
            printf("%d\n",state_buffer.inimigos[i].se_pos.x);
        }
        printf("Misseis:\n");
        for(i = 0; i < TORPEDOS; i++)
        {
            printf("%d\n",state_buffer.torpedos[i].se_pos.x);
        }
        printf("Pontos:\n");
        printf("%d\n",state_buffer.pontuacao);
        getch();
    }
    else
    {
        printf("Erro ao ler arquivo.\n");
        getch();
    }
    fclose(save);
}

/*
 * funcao: load_game
 * -----------------
 * carrega parametros do jogo salvo no arquivo binario
 * pontuacao     : recebe pontuacao do jogador
 * nome          : recebe nome do jogador
 * jogador       : estrutura com os parametros do jogador
 * mergulhadores : estrutura com os parametros dos mergulhadores
 * inimigos      : estrutura com os parametros dos inimigos
 * torpedos      : estrutura com os parametros dos torpedos
 * erro          : indica se ocorreu erro no carregamento do jogo salvo
 */
void load_game(int *pontuacao, char *nome, SUBMARINO *jogador,
               MERGULHADOR *mergulhadores, INIMIGO *inimigos, TORPEDO *torpedos,
               int *erro)
{
    int i;
    background();
    FILE *save;
    STATE state_buffer;
    tela_inicial(nome);
    save = fopen(strcat(nome,".bin"),"rb");
    if(fread(&state_buffer,sizeof(STATE),1,save) == 1)
    {
        *pontuacao = state_buffer.pontuacao;
        nome = state_buffer.nome;
        *jogador = state_buffer.jogador;
        for(i = 0; i < MERGULHADORES; i++)
        {
            mergulhadores[i] = state_buffer.mergulhadores[i];
        }
        for(i = 0; i < INIMIGOS; i++)
        {
            inimigos[i] = state_buffer.inimigos[i];
        }
        for(i = 0; i < TORPEDOS; i++)
        {
            torpedos[i] = state_buffer.torpedos[i];
        }
    }
    else
    {
        printf("Erro ao ler arquivo.\n");
        *erro = 1;
        getch();
    }
}

/*
 * funcao: imprime_ranking
 * -----------------------
 * imprime ranking
 *     pontos : recebe pontuacao final
 *     nomes  : recebe nome do jogador
 */
void imprime_ranking()
{
    int pontos[RANKINGS] = {0};
    char nomes[RANKINGS][TAMANHO_NOME];
    int i = 0;
    background();
    gotoxy(1,1);
    for(i = 0; i < RANKINGS; i++)
    {
        pontos[i] = 0;
        strcpy(nomes[i]," ");
    }
    printf("RANKING:\n");
    if(carrega_ranking(nomes, pontos) == 0)
    {
        for(i = 0; i < RANKINGS; i++)
        {
            printf("%s",nomes[i]);
            gotoxy(TAMANHO_NOME+1,i+2);
            printf("%d\n",pontos[i]);
        }
    }
    else
    {
        printf("Erro ao ler ranking.");
    }
    getch();
}

/*
 * funcao: carrega_ranking
 * -----------------------
 * carrega ranking salvo no arquivo de texto
 *     nomes   : recebe nome dos jogadores no ranking
 *     pontos  : recebe pontuacao final dos jogadores no ranking
 *     posicao : recebe posicao e pontuacao do jogador no ranking
 */
int carrega_ranking(char nomes[][TAMANHO_NOME], int *pontos)
{
    FILE *arq;
    char posicao[MAXPOS];
    int i = 0;
    arq = fopen("ranking.txt","r");
    if(arq != NULL)
    {
        if(fgets(posicao,sizeof(posicao),arq) != NULL)
        {
            strcpy(nomes[i], strtok(posicao,";"));
            pontos[i] = atoi(strtok(NULL,";"));
            i++;
            while(!feof(arq) && i < RANKINGS)
            {
                fgets(posicao,sizeof(posicao),arq);
                strcpy(nomes[i], strtok(posicao,";"));
                pontos[i] = atoi(strtok(NULL,";"));
                i++;
            }
            return 0;
        }
        else
        {
            return 1;
        }
        fclose(arq);
    }
    else
    {
        return 1;
    }
}

/*
 * funcao: salva_ranking
 * ---------------------
 * salva ranking em arquivo de texto
 *     jog_nome   : recebe nome do jogador atual
 *     jog_pontos : recebe pontuacao final do jogador atual
 *     pontos     : recebe pontuacao final dos jogadores no ranking
 *     nomes      : recebe nome dos jogadores no raking
 *     posjog     : recebe posicao dos jogadores no ranking
 */
void salva_ranking(char jog_nome[TAMANHO_NOME], int jog_pontos)
{
    FILE *arq;
    int pontos[RANKINGS] = {0};
    char nomes[RANKINGS][TAMANHO_NOME];
    int i = 0;
    int posjog = 0;
    for(i = 0; i < RANKINGS; i++)
    {
        pontos[i] = 0;
        strcpy(nomes[i]," ");
    }
    carrega_ranking(nomes, pontos);
    i = 0;
    while(pontos[i] > jog_pontos && i < RANKINGS)
    {
        i++;
    }
    if(i < RANKINGS)
    {
        posjog = i;
        for(i = RANKINGS-1; i > posjog; i--)
        {
            pontos[i] = pontos[i-1];
            strcpy(nomes[i],nomes[i-1]);
        }
        getch();
        pontos[i] = jog_pontos;
        strcpy(nomes[i],jog_nome);
        arq = fopen("ranking.txt","w");
        for(i = 0; i < RANKINGS; i++)
        {
            fprintf(arq,"%s;%d;\n",nomes[i],pontos[i]);
        }
        fclose(arq);
    }
}
