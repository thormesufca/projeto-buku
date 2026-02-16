#include <stdio.h>
#include <conio.h>

int ler_caracter()
{
    char c;
    while (1)
    {
        c = getch();
        if (c == 27 || c == 3)
        {
            printf("Programa Encerrado!\n");
            exit(1);
        }
        if (c == 0 || c == -32)
        {
            c = getch();
            switch (c)
            {
            case 72:
                return 0;
            case 75:
                return 3;
            case 80:
                return 2;
            case 77:
                return 1;
            default:
                printf("Tecla invalida, use teclas direcionais ou WASD. Z para desfazer ultimo movimento\n");
                break;
            }
        }
        else if (c == 26)
        { // Ctrl + Z
            return 99;
        }
        else
        {
            switch (c)
            {
            case 'W':
            case 'w':
                return 0;
            case 'D':
            case 'd':
                return 1;
            case 'S':
            case 's':
                return 2;
            case 'A':
            case 'a':
                return 3;
            case 'Z':
            case 'z':
                return 99;
            default:
                break;
            }
        }
    }
    return 0;
}
