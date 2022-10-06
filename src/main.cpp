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
    int x = 1;
    int y = 1;
} cs;


// char input[128];
vector<char> input;
char* fname;
char* dpath;
char last_char;


struct lines_t
{
    vector<char*> lines;
    vector<int> lenght;

    int number;
} lines;

void screenRefresh();
void insert(int i, char x);
void erase(int pos);
int get_terminal_size();
int getLines();
int getLinesLenght();

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
        getLinesLenght();
        screenRefresh();

        c = getch();
        switch(c)
        {

            case CTRL('Q'):
            {
                fclose(fp);
                exit(1);
            }

            case CTRL('R'):
            {
                get_terminal_size();
                break;
            }

            case CTRL('S'):
            {
                fwrite(input, strlen(input), 1, fp);
                break;
            }

            case arrow_left:
            {
                if(i <= 1)
                {
                    break;
                }
                i--;
                if(input[i] == '\n')
                {
                    cs.y--;
                    cs.x = lines.lenght[cs.y];
                }
                else
                {
                    cs.x--;
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
                if(input[i] == '\n')
                {
                    cs.y++;
                    cs.x = 1;
                }
                else
                {
                    cs.x++;
                }
                break;
            }


            case 13:
            {
                if(i != strlen(input))
                {
                    insert(i, '\n');
                }
                else 
                {
                    input[i] = '\n';
                }

                cs.x = 1;
                cs.y++;

                lines.number++;
                i++;
                break;
            }

            case 't':
            {
                if(i != strlen(input))
                {
                    insert(i, '\t');
                }
                else 
                {
                    input[i] = '\t';
                }

                cs.x += 4;

                i++;
                break;
            }

            case '\b':
            {
                if(i == 0)
                {
                    break;
                }


                i--;
                if(input[i] == '\n')
                {
                    cs.y--;
                    cs.x = lines.lenght[cs.y];

                    lines.number--;
                }
                else if(input[i] == '\t')
                {
                    cs.x -= 4;
                }
                else
                {
                    cs.x--;
                }

                if(i == strlen(input))
                {
                    input[i] = 0;
                }
                else
                {
                    erase(input[i]);
                }
                break;
            }

            default:
            {
                if(c > 31 && c < 127)

                if(i != (strlen(input)))
                {
                    insert(i, c);

                    cs.x++;

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
        last_char = c;
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

    std::string info = "lines: " + std::to_string(lines_count) + " | " + "chars: " + std::to_string(strlen(input) - 1) + " | " + "c: " + std::to_string((int)last_char);
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
    for (int i = pos ; i < strlen(input) - 1 ; i++)
    {
        input[i] = input[++i];
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

int getLinesLenght()
{
    int lenght = 0;
    int i = 0;
    for (int j ; j < strlen(input) ; j++)
    {   
        if (input[j] == '\n')
        {
            lines.lenght[j] = lenght;
            i++;
        }
        lenght++;
    }
    return 0;
}
