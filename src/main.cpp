#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <conio.h>

#include "parts.h"

using std::string;
using std::vector;

struct terminal
{
    int width;
    int heigh;
} term;

struct cursor
{
    int x;
    int y;
    int line;
} cs;


char input[128];
char* fname;
char* dpath;

struct lines_t
{
    vector<char*> lines;
    vector<int> lenght;

    int number;
}lines;


void screenRefresh();
void insert(int i, char x);
int get_terminal_size();
int getLines();

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

    fname = argv[1];

    int i;
    char c;
    while(1)
    {
        screenRefresh();
        c = getch();
        switch(c)
        {

            case CTRL('q'):
            {
                fclose(fp);
                exit(1);
            }

            case arrow_left:
            {
                if(i <= 1)
                {
                    break;
                }
                i--;
                if(--i == '\n')
                {
                    cs.y--;
                }
                break;
            }


            case arrow_right:
            {
                if(i >= strlen(input))
                {
                    break;
                }
                i++;
                break;
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

                lines.number++;
                i++;
                break;
            }

            case '\b':
            {
                if(i == 0)
                {
                    break;
                }
                erase(input[i]);
                i--;
                cs.x--;
                break;
            }

            default:
            {
                if(c > 31 && c < 127)

                if(i != (sizeof(input)) - 1)
                {
                    insert(i, c);
                    i++;
                    break;
                }
                else
                {
                    input[i] = c;
                }

                cs.x++;

                i++;
                break;
            }
        }
    }

    return 0;
}


void screenRefresh()
{
    cls();
    int lines_count = 1;
    for(int i = 0 ; i < sizeof(input) ; i++)
    {
        switch(input[i])
        {
            case '\t':
            {
                std::cout << "    ";
                break;
            }

            case '\n':
            {
                std::cout << '\n';
                lines_count++;
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

  

    for(int i = 0 ; i < (term.heigh - lines_count) - 1 ; i++)
    {
        std::cout << green << '~' << reset << '\n';
    }

    std::string info = "lines: " + std::to_string(lines_count) + " | " + "chars: " + std::to_string(sizeof(input) - 1);
    std::cout << bg_blue << info;
    for(int i = 0 ; i < (term.width - info.length()) - strlen(fname) ; i++)
    {
        std::cout << space;
    }
    std::cout << fname;
    std::cout << reset;

    gotoxy(cs.x, cs.y);
}

void insert(int pos, char x)
{
    for (int i = pos ; i < sizeof(input) ; i++)
    {
        input[++i] = input[i];
    }
    input[pos] = x;
}

void erase(int pos)
{
    for (int i = strlen(input) ; i < pos ; i++)
    {
        input[--i] = input[i];
    }

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

int getLines()
{
    /* todo:

        for loop: 

        add actual position to vector[j][i]

        increment $i if actual position on input is '\n' 

        end:
    */

    vector<char*> lines_; 

    int j = 0;
    int it;
    for (int i = 0 ; i < strlen(input) ; i++)
    {
        lines.lines[j][it] = input[i];
        if(input[i] == '\n')
        {
            j++;
        }
    }
    
    return 0;
}
