#include <stdio.h>
#include <string.h>

#define RESET "\033[0m"
#define BASE "\033["

#define BG_PRETO "40m" // 1
#define BG_BRANCO "47m" // 2
#define BG_AZUL "44m" // 3
#define BG_VERMELHO "41m" // 4
#define BG_AMARELO "43m" // 5

#define FG_PRETO "30m" // 1
#define FG_BRANCO "37m" // 2
#define FG_AZUL "34m" // 3
#define FG_VERMELHO "31m" // 4
#define FG_AMARELO "33m" // 5

#define BOLD "1;"
#define NORMAL "0;"
#define UNDERLINE "4;"


void set_foreground_color(int color, int bold, int underline, char *buffer){
    strcpy(buffer, BASE);
    if(bold == 0 && underline == 0){
        strcat(buffer, NORMAL);
    } else if(bold != 0){
        strcat(buffer, BOLD);
    } else{
        strcat(buffer, UNDERLINE);
    }
    switch (color)
    {
    case 1:
        strcat(buffer, FG_PRETO);
        break;
    case 2:
        strcat(buffer, FG_BRANCO);
        break;
    case 3:
        strcat(buffer, FG_AZUL);
        break;
    case 4:
        strcat(buffer, FG_VERMELHO);
        break;
    case 5:
        strcat(buffer, FG_AMARELO);
        break;
    default:
        strcpy(buffer, RESET);
        break;
    }
}

void set_background_color(int color, char *buffer)
{
    strcpy(buffer, BASE);
    switch (color)
    {
        case 1:
        strcat(buffer, BG_PRETO);
        break;
        case 2:
        strcat(buffer, BG_BRANCO);
        break;
        case 3:
        strcat(buffer, BG_AZUL);
        break;
        case 4:
        strcat(buffer, BG_VERMELHO);
        break;
        case 5:
        strcat(buffer, BG_AMARELO);
        break;
        default:
        strcpy(buffer, RESET);
        break;
    }
}