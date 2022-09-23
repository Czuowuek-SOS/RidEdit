#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <conio.h>

#include "parts.h"

struct terminal
{
    int width;
    int heigh;

} term;

struct cursor
{
    int x;
    int y;
} cs;


char input[128];
char* fname;
char* dpath;

int lines = 1;

void screenRefresh();
void insert(int i, char x);
int get_terminal_size();

int main(int argc, char* argv[1])
{
    atexit([]()
    {
        cls();
    });

    if (argc < 1)
    {
        std::cout << red << "Expected arguments" << reset << '\n';
    }

    FILE* fp = fopen(argv[1], "r+");
    if (fp == NULL)
    {
        std::cout << red << "file no found\n" << reset;
        return 1;
    }

    get_terminal_size();
    screenRefresh();

    fname = argv[1];

    int i;
    char c;
    while(1)
    {
        c = getch();
        switch(c)
        {

            case CTRL('q'):
            {
                fclose(fp);
                exit(1);
            }

            case 13:
            {
                if(i != (sizeof(input)) - 1)
                {
                    insert(i, '\n');

                }
                else 
                {
                    input[i] = '\n';
                }

                cs.x = 0;
                cs.y++;

                lines++;
                i++;
                break;
            }

            default:
            {
                if(i != (sizeof(input)) - 1)
                {
                    insert(i, 'c');
                    i++;
                    break;
                }
                else
                {
                    input[1] = c;
                }

                cs.x++;

                i++;
                break;
            }
        }
        screenRefresh();
    }

    return 0;
}


void screenRefresh()
{
    cls();
    for(int i = 0 ; i < sizeof(input) ; i++)
    {
        switch(input[i])
        {
            case '\t':
            {
                std::cout << "    ";
                break;
            }

            default:
            {
                std::cout << input[i];
                break;
            }
        }
        
    }
    std::cout << '\n';
    for(int i = term.heigh - lines ; i < term.heigh ; i++)
    {
        std::cout << green << '~' << reset << '\n';
    }
}

void insert(int pos, char x)
{
    for (int i = pos ; i < sizeof(input) ; i++)
    {
        input[++i] = input[i];
    }
    input[pos] = x;
}

int get_terminal_size()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        term.width = csbi.dwSize.X;
        term.heigh  = csbi.dwSize.Y;

        return 0;
    }
    else
    {
        return -1;
    }

}
