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


// vector<char> input;
// char input[256];
string input;
char copyBoard[16];

char* fname;
char* dpath;
char* textColor;
char last_char;


struct lines_t
{
    vector<char*> lines;
    vector<int> lenght;

    int number;
} lines;

struct selected_t
{
    int first = -1;
    int last  = -1;
    string selected;
} selected;

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

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        std::cout << red << "file no found\n" << reset;
        return 1;
    }
    else
    {
        freopen(argv[1], "w+", fp);
    }

    get_terminal_size();

    fname = argv[1];

    int i;
    char c;
    while(1)
    {   
        getLinesLenght();
        // getLines();
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
                // const char* buf = input.c_str();
                fwrite(input.c_str(), strlen(input.c_str()), 1, fp);
                break;
            }

            case CTRL('A'):
            {
                selected.first = i;
                break;
            }

            case CTRL('B'):
            {
                selected.last = i;
                break;
            }

            case CTRL('U'):
            {
                selected.first = -1;
                selected.last  = -1;
                break;
            }

            case CTRL('C'):
            {
                for(int j = selected.first ; j < selected.last ; j++)
                {
                    selected.selected += input[j];
                }
                break;
            }


            case CTRL('V'):
            {
                int index = i;
                for(int j = 0 ; j < selected.selected.length() ; j++)
                {
                    // insert(index, selected.selected[j]);
                    input += selected.selected[j];
                    cs.x++;
                    i++;
                }
                break;
            }

            case arrow_left:
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

                
                break;
            }


            case arrow_right:
            {
                if(i >= input.length())
                {
                    break;
                }
                
                i++;
                if(input[i] == '\n')
                {
                    cs.y++;
                    cs.x = 1;
                }
                else if(input[i] == '\t')
                {
                    cs.x += 4;
                }
                else
                {
                    cs.x++;
                }

                break;
            }


            case 13:
            {
                if(i != input.length())
                {
                    insert(i, '\n');
                }
                else 
                {
                    // input[i] = '\n';
                    input += '\n';
                }

                cs.x = 1;
                cs.y++;

                lines.number++;
                i++;
                break;
            }

            case '(':
            {
                if(i != input.length())
                {
                    insert(i, '(');
                    insert(++i, ')');
                }
                else 
                {
                    // input[i] = '\t';
                    input += '(';
                    input += ')';
                }

                i++;
                cs.x++;
                break;
            }

            case '\t':
            {
                if(i != input.length())
                {
                    insert(i, '\t');
                }
                else 
                {
                    // input[i] = '\t';
                    input += '\t';
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

                if(i == input.length())
                {
                    // input[i] = '\0';
                    input.erase(i);
                }
                else
                {
                    // erase(input[i]);
                    input.erase(i);
                }
                break;
            }

            default:
            {
                if(!(c > 31) && !(c < 127))
                {
                    break;
                }

                if(i != input.length())
                {
                    insert(i, c);
                    // input.insert(i, c);

                    cs.x++;

                    i++;
                    break;
                }
                else
                {
                    // input[i] = c;
                    input += c;
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
    int in_line_index;
    int lines_index = 1;
    for(int i = 0 ; i < input.length() ; i++)
    {
        if(in_line_index > term.width)
        {
            std::cout << "\n>>";
            lines_index++;
            in_line_index = 2;
        }

        if(selected.first == i)
        {
            std::cout << bg_blue;
        }
        else if(selected.last == i)
        {
            std::cout << reset;
        }
        
        switch(input[i])
        {
            case '\t':
            {
                std::cout << "    ";
                in_line_index += 4;

                break;
            }

            case '\n':
            {
                std::cout << '\n';
                lines_index++;
                in_line_index = 0;

                break;
            }

            // case '#':
            // {
            //     setTextColor(magenta);
            // }

            // case ' ':
            // {
            //     if (textColor != reset) 
            //     {
            //         setTextColor(reset);
            //     }
            // }

            default:
            {
                std::cout << input[i];
                in_line_index++;

                break;
            }
        }
        
    }
    std::cout << '\n';

  

    for(int i = 0 ; i < (term.heigh - lines_index) - 1 ; i++)
    {
        std::cout << green << '~' << reset << '\n';
    }

    std::string info = "lines: " + std::to_string(lines_index) + " | " + "chars: " + std::to_string(input.length() - 1) + " | " + "c: " + std::to_string((int)last_char);
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
    for (int i = --pos ; i < input.length() + 1 ; i++)
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
    for (int i = 0 ; i < input.length() ; i++)
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
    int in_line_index = 0;
    int current_line = 0;
    for(int i = 0 ; i < input.length() ; i++)
    {
        if(input[i] == '\n')
        {
            lines.lenght.push_back(in_line_index);
            in_line_index = 0;
            current_line++;
        }
        else
        {
            in_line_index++;
        }
    }
    lines.lenght.push_back(in_line_index);
    return 0;
}
