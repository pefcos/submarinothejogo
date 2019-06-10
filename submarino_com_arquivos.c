/*
    JOGO DO SUBMARINO
    Feito por:
        Pedro Fronchetti Costa da Silva
        Tiago Dias Ferreira
    Trabalho final da disciplina de Algoritmos e Programação, turma C do semestre 2019/1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <time.h>
#include <windows.h>

//VALORES REFERENTES A CORES
#define FUNDO 15
#define FRENTE 1

//VALORES REFERENTES AO MENU
#define TOPOPTION 18
#define BOTOPTION 22
#define NOVO_JOGO 18
#define CARREGA_JOGO 19
#define CREDITOS 20
#define RECORDES 21
#define SAIR 22

//VALOR DO SLEEP NO GAMELOOP
#define SLEEP 38

//VALORES REFERENTES AOS LIMITES DO BACKGROUND
#define LIMCIMA 2
#define LIMBAIXO 25
#define LIMDIR 80
#define LIMESQ 1
#define LIMAGUA 6

//VALORES REFERENTES AO CODIGO DAS TECLAS IMPORTANTES DO TECLADO
#define DIREITA 77
#define ESQUERDA 75
#define CIMA 72
#define BAIXO 80
#define ESC 27
#define ESPACO 32

//VALORES REFERENTES A QUANTIDADE DE OBSTACULOS
#define MERGULHADORES 5
#define INIMIGOS 4

//VALORES REFERENTES A POSICAO INICIAL DO SUBMARINO
#define XINI 35
#define YINI 4

//VALORES REFERENTES A PROPRIEDADES DO SUBMARINO
#define OXIGENIOMAX 3000
#define OXIGENIOVAR 4
#define VIDAS 3
#define MISSEIS 2
#define CARGA 5

//VALORES REFERENTES AO CALCULO DA PONTUACAO
#define VALOR_MERGULHADOR 20
#define VALOR_INIMIGO 50

//VALORES REFERENTES AO RANKING
#define RANKINGS 10
#define TAMANHO_NOME 20

typedef struct coordenada
{
    int x, y;
}COORDENADA;

typedef struct submarino
{
    //se_pos eh usado para definir a coordenada do canto superior esquerdo do submarino.
    COORDENADA se_pos;
    int sentido;
    int altura;
    int largura;
    int oxigenio;
    int vidas; //O jogador esta morto quando nao possui mais vidas (isto eh, quando o valor de vidas for 0);
    int carga;
}SUBMARINO;

typedef struct mergulhador
{
    COORDENADA se_pos;
    int largura;//4 por padrao, pois ele eh desenhado assim >->o / o<-<, ocupando 4 digitos.
    int status;//1: Andando pra direita, 0: Fora da tela, -1: Andando pra esquerda.
}MERGULHADOR;

typedef struct inimigo
{
    COORDENADA se_pos;
    int largura;
    int altura;
    int status;
}INIMIGO;

typedef struct missil
{
    COORDENADA se_pos;
    int largura;
    int status;
}MISSIL;

typedef struct state
{
    SUBMARINO jogador;
    MERGULHADOR mergulhadores[MERGULHADORES];
    INIMIGO inimigos[INIMIGOS];
    MISSIL misseis[MISSEIS];
    int pontuacao;
    char nome[20];
}STATE;

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
void move_jogador(int *input, SUBMARINO *jogador, MISSIL *misseis);
int testa_posicao_megulhador(SUBMARINO jogador, MERGULHADOR mergulhador);
int testa_posicao_inimigo(SUBMARINO jogador, INIMIGO inimigo);
void pega_mergulhador(SUBMARINO *jogador, MERGULHADOR *mergulhadores);
void destroi_jogador(SUBMARINO *jogador);
void colide_inimigo(SUBMARINO *jogador, INIMIGO *inimigos);
void colide_missil(MISSIL *misseis, INIMIGO *inimigos, int *pontuacao);
void inicializa_mergulhadores(MERGULHADOR *mergulhadores, INIMIGO *inimigos);
void inicializa_inimigos(INIMIGO *inimigos, MERGULHADOR *mergulhadores);
void atira_missil(MISSIL *misseis, SUBMARINO jogador);
void desenha_mergulhador(MERGULHADOR mergulhador);
void desenha_inimigo(INIMIGO inimigo);
void desenha_missil(MISSIL missil);
void apaga_mergulhador(MERGULHADOR mergulhador);
void apaga_inimigo(INIMIGO inimigo);
void apaga_missil(MISSIL missil);
void move_mergulhador(MERGULHADOR *mergulhadores);
void move_inimigo(INIMIGO *inimigos);
void move_missil(MISSIL *misseis);
int verifica_vidas(SUBMARINO jogador); //Retorna 1 se o jogador esta vivo e 0 se ele esta morto.
void devolve_mergulhadores(SUBMARINO *jogador, int *pontuacao);
void ponto_aquatico(SUBMARINO jogador, int *pontuacao, int *contador); //Conta ponto pra cada segundo submerso.
void creditos();
int confirma_save();
void salva_jogo(STATE save_state, char *nome_save, int load);
void printbin(char *nome_save);
void load_game(int *pontuacao, char *nome, SUBMARINO *jogador, MERGULHADOR *mergulhadores, INIMIGO *inimigos, MISSIL *misseis, int *erro);
void imprime_ranking();
int carrega_ranking(char nomes[][TAMANHO_NOME], int *pontos);
void salva_ranking(char jog_nome[TAMANHO_NOME], int jog_pontos);

int main()
{
    menu();
}

void menu()
{
    int menu_cursor = TOPOPTION;
    int menu_input = 0;
    int endgame = 0;
    char nome[20] = " ";
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

void desenha_menu()
{
    cputsxy(10, 8,  "  _____ _    _ ____  __  __          _____  _____ _   _  ____  ");
    cputsxy(10, 9,  " / ____| |  | |  _ \\|  \\/  |   /\\   |  __ \\|_   _| \\ | |/ __ \\ ");
    cputsxy(10, 10, "| (___ | |  | | |_) | \\  / |  /  \\  | |__) | | | |  \\| | |  | |");
    cputsxy(10, 11, " \\___ \\| |  | |  _ <| |\\/| | / /\\ \\ |  _  /  | | | . ` | |  | |");
    cputsxy(10, 12, " ____) | |__| | |_) | |  | |/ ____ \\| | \\ \\ _| |_| |\\  | |__| |");
    cputsxy(10, 13, "|_____/ \\____/|____/|_|  |_/_/    \\_\\_|  \\_\\_____|_| \\_|\\____/ ");
    cputsxy(10, 14, "THE JOGO");
    cputsxy(28, 14, "Por: Pedro F. C. da Silva e Tiago D. Ferreira");
    cputsxy(20, 18, "NOVO JOGO");
    cputsxy(20, 19, "CARREGAR JOGO");
    cputsxy(20, 20, "CREDITOS");
    cputsxy(20, 21, "RECORDES (not implementado ainda)");
    cputsxy(20, 22, "SAIR DO JOGO");

}

void gameloop(int load)
{
    int i;
    int input;
    int erro = 0;
    int contador = 0;//Conta tempo pra registrar os pontos submersos
    int pontuacao = 0;
    char nome[20] = " ";

    SUBMARINO jogador = {{XINI,YINI},1,2,12,OXIGENIOMAX,VIDAS,0};
    MERGULHADOR mergulhadores[MERGULHADORES] = {{{0,0},4,0},{{0,0},4,0},{{0,0},4,0},{{0,0},4,0},{{0,0},4,0}};
    INIMIGO inimigos[INIMIGOS] = {{{0,0},9,2,0},{{0,0},9,2,0},{{0,0},9,2,0},{{0,0},9,2,0}};
    MISSIL misseis[MISSEIS] = {{{0,0},2,0},{{0,0},2,0}};

    background();
    if(load == 1)
    {
        load_game(&pontuacao, nome, &jogador, mergulhadores, inimigos, misseis, &erro);
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
            move_jogador(&input, &jogador, misseis);
            Sleep(SLEEP);
            move_mergulhador(mergulhadores);
            move_inimigo(inimigos);
            move_missil(misseis);
            pega_mergulhador(&jogador, mergulhadores);
            devolve_mergulhadores(&jogador, &pontuacao);
            ponto_aquatico(jogador,&pontuacao, &contador);
            colide_inimigo(&jogador, inimigos);
            colide_missil(misseis, inimigos, &pontuacao);
            mostra_interface(pontuacao, jogador);
        }
        while (input != ESC && verifica_vidas(jogador));
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
                for(i = 0; i < MISSEIS; i++)
                {
                    save_state.misseis[i] = misseis[i];
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

void background()
{
    int i, j;
    for (i = 0; i <= 25; i++)
        for (j = 0; j <= 80; j++)
        {
            textbackground(FUNDO);
            textcolor(FRENTE);
            putchxy(j, i, ' ');
        }
}

void desenha_agua()
{
    int i = 0;
    for(i = LIMESQ+1; i < LIMDIR; i++)
    {
        putchxy(i,LIMAGUA,'~');
    }
}

void mostra_interface(int pontuacao, SUBMARINO jogador)
{
    gotoxy(1, 1);
    printf("Pontuacao: %i", pontuacao);
    gotoxy(1, LIMBAIXO + 1);
    printf("Oxigenio: %d   \n", jogador.oxigenio);
    printf("Vidas: %d \n", jogador.vidas);
    printf("Mergulhadores Pegos: %d \n", jogador.carga);
}

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

void tela_inicial(char *nome)
{
    gotoxy(30, 10);
    printf("Insira um nome aqui.");
    gotoxy(30, 11);
    gets(nome);
    clrscr();
}

void tela_jogo()
{
    int i, j;

    for (i = LIMCIMA; i <= LIMBAIXO; i++)
        for (j = LIMESQ; j <= LIMDIR; j++)
        {
            if (i > LIMCIMA && i < LIMBAIXO)
            {
                putchxy(LIMESQ, i, '|');
                putchxy(LIMDIR, i, '|');
            }
            else
            {
                putchxy(j, i, '-');
            }
        }
}

void desenha_jogador(SUBMARINO jogador)
{
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
}

void apaga_jogador(SUBMARINO jogador)
{
    cputsxy(jogador.se_pos.x, jogador.se_pos.y,    "            ");
    cputsxy(jogador.se_pos.x, jogador.se_pos.y + 1,"            ");
}

void move_jogador(int *input, SUBMARINO *jogador, MISSIL *misseis)
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
                atira_missil(misseis, *jogador);
                if(!(((*jogador).se_pos.x <= LIMESQ + 1 + misseis[0].largura) && (*jogador).sentido == 1) && !(((*jogador).se_pos.x+(*jogador).largura+misseis[0].largura >= LIMDIR) && (*jogador).sentido == -1))
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

int testa_posicao_missil(MISSIL missil, INIMIGO inimigo)
{
    if((missil.se_pos.x + missil.largura >= inimigo.se_pos.x) && (missil.se_pos.x <= inimigo.se_pos.x + inimigo.largura))
    {
        if((missil.se_pos.y == inimigo.se_pos.y) || (missil.se_pos.y == inimigo.se_pos.y + 1))
        {
            if((missil.status != 0 && inimigo.status != 0))
            {
                return 1;
            }
        }
    }
    return 0;
}

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

void destroi_jogador(SUBMARINO *jogador)
{
    apaga_jogador(*jogador);
    (*jogador).se_pos.x = XINI;
    (*jogador).se_pos.y = YINI;
    (*jogador).carga = 0;
    (*jogador).oxigenio = OXIGENIOMAX;
    (*jogador).vidas -= 1;
}

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

void colide_missil(MISSIL *misseis, INIMIGO *inimigos, int *pontuacao)
{
    int i;
    int j;
    for (i = 0; i < MISSEIS; i++)
    {
        for(j = 0; j < INIMIGOS; j++)
        {
            if (testa_posicao_missil(misseis[i], inimigos[j]) == 1)
            {
                apaga_missil(misseis[i]);
                misseis[i].se_pos.x = 0;
                misseis[i].se_pos.y = 0;
                misseis[i].status = 0;
                apaga_inimigo(inimigos[j]);
                inimigos[j].se_pos.x = 0;
                inimigos[j].se_pos.y = 0;
                inimigos[j].status = 0;
                *pontuacao += 50;
            }
        }
    }
}

void inicializa_mergulhadores(MERGULHADOR *mergulhadores, INIMIGO *inimigos)
{
    int i,j;
    int chance = 0;
    srand(time(NULL));
    for (i = 0; i < MERGULHADORES; i++)
    {
        chance = rand() % 3;
        if (mergulhadores[i].status == 0 && chance == 1)
        {
            mergulhadores[i].status = (rand () % 3) - 1;
            if(mergulhadores[i].status == -1)
            {
                mergulhadores[i].se_pos.x = LIMDIR - mergulhadores[i].largura;
                mergulhadores[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA) + (LIMCIMA + LIMAGUA);
            }
            else if (mergulhadores[i].status == 1)
            {
                mergulhadores[i].se_pos.x = LIMESQ + 1;
                mergulhadores[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA) + (LIMCIMA + LIMAGUA);
            }

            //Verifica se y é igual a outros mergulhadores ou submarinos inimigos. Se for, deixa de inicializar o mergulhador.
            for(j = 0; j < MERGULHADORES; j++)
            {
                if((mergulhadores[i].se_pos.y == mergulhadores[j].se_pos.y) && (i != j))
                {
                    mergulhadores[i].status = 0;
                    mergulhadores[i].se_pos.x = 0;
                    mergulhadores[i].se_pos.y = 0;
                }
            }
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
    for(i = 0; i < MERGULHADORES; i++)
        if (mergulhadores[i].status != 0)
            desenha_mergulhador(mergulhadores[i]);
}

void inicializa_inimigos(INIMIGO *inimigos, MERGULHADOR *mergulhadores)
{
    int i, j;
    int chance = 0;
    srand(time(NULL));
    for (i = 0; i < INIMIGOS; i++)
    {
        chance = rand() % 3;
        if (inimigos[i].status == 0 && chance == 1)
        {
           inimigos[i].status = (rand () % 3) - 1;
            if(inimigos[i].status == -1)
            {
                inimigos[i].se_pos.x = LIMDIR - inimigos[i].largura;
                inimigos[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA-1) + (LIMCIMA + LIMAGUA);
            }
            else if (inimigos[i].status == 1)
            {
                inimigos[i].se_pos.x = LIMESQ + 1;
                inimigos[i].se_pos.y = rand () % (LIMBAIXO - LIMCIMA - LIMAGUA-1) + (LIMCIMA + LIMAGUA);
            }

            //Verifica se y é igual a outros mergulhadores ou submarinos inimigos. Se for, deixa de inicializar o submarino.
            for(j = 0; j < INIMIGOS; j++)
            {
                if((inimigos[i].se_pos.y == mergulhadores[j].se_pos.y || inimigos[i].se_pos.y + 1 == mergulhadores[j].se_pos.y) && (i != j))
                {
                    inimigos[i].status = 0;
                    inimigos[i].se_pos.x = 0;
                    inimigos[i].se_pos.y = 0;
                }
            }
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
    for(i = 0; i < INIMIGOS; i++)
        if (inimigos[i].status != 0)
            desenha_inimigo(inimigos[i]);
}

void atira_missil(MISSIL *misseis, SUBMARINO jogador)
{
    int i = 0;
    int find = 0;
    if(jogador.se_pos.y >= LIMAGUA)
    {
        while(find == 0 && i < MISSEIS)
        {
            if(misseis[i].status == 0)
            {
                misseis[i].status = jogador.sentido;
                misseis[i].se_pos.y = jogador.se_pos.y + 1;
                if(misseis[i].status == 1)
                {
                    misseis[i].se_pos.x = jogador.se_pos.x + jogador.largura;
                }
                else
                {
                    misseis[i].se_pos.x = jogador.se_pos.x - misseis[i].largura;
                }
                find = 1;
            }
            i++;
        }
    }
}

void desenha_mergulhador(MERGULHADOR mergulhador)
{
    if(mergulhador.status == 1)
    {
        cputsxy(mergulhador.se_pos.x, mergulhador.se_pos.y, ">->o");
    }
    else
    {
        cputsxy(mergulhador.se_pos.x, mergulhador.se_pos.y, "o<-<");
    }
}

void desenha_inimigo(INIMIGO inimigo)
{
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
}

void desenha_missil(MISSIL missil)
{
    if(missil.status == 1)
    {
        cputsxy(missil.se_pos.x, missil.se_pos.y, "=>");
    }
    else
    {
        cputsxy(missil.se_pos.x, missil.se_pos.y, "<=");
    }
}

void apaga_mergulhador(MERGULHADOR mergulhador)
{
    cputsxy(mergulhador.se_pos.x, mergulhador.se_pos.y, "    ");
}

void apaga_inimigo(INIMIGO inimigo)
{
    cputsxy(inimigo.se_pos.x, inimigo.se_pos.y,   "         ");
    cputsxy(inimigo.se_pos.x, inimigo.se_pos.y+1, "         ");
}

void apaga_missil(MISSIL missil)
{
    cputsxy(missil.se_pos.x, missil.se_pos.y, "  ");
}

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

void move_missil(MISSIL *misseis)
{
    int i;
    for (i = 0; i < MISSEIS; i++)
    {
        switch(misseis[i].status)
        {
            case 1:
                if (misseis[i].se_pos.x < LIMDIR - misseis[i].largura - 1)
                {
                    apaga_missil(misseis[i]);
                    misseis[i].se_pos.x += 2;
                    desenha_missil(misseis[i]);
                }
                else if (misseis[i].se_pos.x == LIMDIR - misseis[i].largura || misseis[i].se_pos.x == LIMDIR - misseis[i].largura - 1)
                {
                    apaga_missil(misseis[i]);
                    misseis[i].se_pos.x = 0;
                    misseis[i].se_pos.y = 0;
                    misseis[i].status = 0;
                }
                break;
            case -1:
                if(misseis[i].se_pos.x > LIMESQ + 2 + 1)
                {
                    apaga_missil(misseis[i]);
                    misseis[i].se_pos.x -= 2;
                    desenha_missil(misseis[i]);
                }
                else if (misseis[i].se_pos.x == LIMESQ + 2 || misseis[i].se_pos.x == LIMESQ + 2 + 1)
                {
                    apaga_missil(misseis[i]);
                    misseis[i].se_pos.x = 0;
                    misseis[i].se_pos.y = 0;
                    misseis[i].status = 0;
                }
                break;
        }
    }
}

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

void devolve_mergulhadores(SUBMARINO *jogador, int *pontuacao)
{
    if((*jogador).se_pos.y < LIMAGUA)
    {
        *pontuacao += (*jogador).carga * VALOR_MERGULHADOR;
        (*jogador).carga = 0;
    }
}

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

void creditos()
{
    background();
    cputsxy(29,10,"Jogo desenvolvido por:");
    cputsxy(25,12,"Pedro Fronchetti Costa da Silva");
    cputsxy(31,13,"Tiago Dias Ferreira");
    getchar();
}

int confirma_save()
{
    char sim;
    background();
    gotoxy(30, 10);
    printf("Deseja salvar o jogo?");
    sim = getchar();
    if(sim == 's')
    {
        return 1;
    }
    else
    {
        return 0;
    }
    clrscr();
}

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
        for(i = 0; i < MISSEIS; i++)
        {
            printf("%d\n",state_buffer.misseis[i].se_pos.x);
        }
        printf("Pontos:\n");
        printf("%d\n",state_buffer.pontuacao);
        getchar();
    }
    else
    {
        printf("Erro ao ler arquivo.\n");
        getchar();
    }
    fclose(save);
}

void load_game(int *pontuacao, char *nome, SUBMARINO *jogador, MERGULHADOR *mergulhadores, INIMIGO *inimigos, MISSIL *misseis, int *erro)
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
        for(i = 0; i < MISSEIS; i++)
        {
            misseis[i] = state_buffer.misseis[i];
        }
    }
    else
    {
        printf("Erro ao ler arquivo.\n");
        *erro = 1;
        getchar();
    }
}

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
        printf("Erro ao ler ranking. Comum ocorrer em jogos novos.");
    }
    getchar();
}

int carrega_ranking(char nomes[][TAMANHO_NOME], int *pontos)
{
    FILE *arq;
    char posicao[30];
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
        getchar();
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
