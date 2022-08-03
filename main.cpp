#include <iostream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "parts.h"
using std::string;

string file_name;

int width, height;
int x, y;

std::vector<char> buffer;
std::vector<string> lines;
std::vector<int> lines_count;


void screen_refresh();
int main(int argc, char **argv)
{
    std::atexit([]() {
        cls();
    });

    if (argc == 0)
    {
        std::cout << red << "Usage: " << argv[0] << reset << newl;
        return 1;
    }
    if (get_terminal_size(width, height) == -1)
    {
        std::cout << red << "Error: Cannot get terminal size";
        getch();
        return 1;
    }

    FILE *fp = fopen(argv[1], "a+");
    if (fp == NULL)
    {
        std::cout << red << "Error: Cannot open file" << reset << newl;
        getch();
        return 1;
    }
    file_name = argv[1];
    if (atoi(argv[2]) == 1)
    {
        while (true)
        {
            char c = fgetc(fp);
            if (c == EOF)
            {
                break;
            }
            buffer.push_back(c);
        }

        for (int i = 0; i < buffer.size(); i++)
        {
            if (buffer[i] == '\n')
            {
                lines.push_back(string());
                lines_count.push_back(0);
            }
            else
            {
                lines.back() += buffer[i];
                lines_count.back()++;
            }
        }
    }

    


    screen_refresh();

    int i = 1;
    std::vector<char>::iterator it;
    char c;
    while(true)
    {
        c = getch();

        it = buffer.begin() + i;
        switch (c)
        {
            case CTRL('Q'):
            {
                std::exit(0);
            }
            case CTRL('S'):
            {
                fwrite(buffer.data(), sizeof(char), buffer.size(), fp);
                break;
            }
            case CTRL('R'):
            {
                get_terminal_size(width, height);
            }

            case '\n':
            {
                buffer.insert(it, '\n');
                
                x = 1;
                y++;

                break;
            }
            case '\b':
            {
                if (buffer.size() > 0)
                {
                    if (buffer[i] == '\n')
                    {
                        y--;
                        x = 0;
                    }
                    else
                    {
                        x--;
                    }

                    buffer.erase(it);


                    i--;
                }
                break;
            }
        
        
            default:
            {
                if (c <= 32 || c >= 127)
                {
                    break;
                }
                buffer.insert(it, c);

                x++;
                i++;
            }
        }
        screen_refresh();
    }
    return 0;
}

void screen_refresh()
{   
    cls();
    gotoxy(x, y);

    std::vector<char> displayed_text;
    if (y > lines.size())
    {
        printf("work in progres");
    }

    int line_count = 0;
    int in_line_position = 0;
    for (int i = 0 ; i < lines_count.size() - 1 ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            switch (buffer[i * width + j])
            {
                case '\n':
                {
                    std::cout << '\n';
                    in_line_position = 0;
                }

                default:
                {
                    std::cout << buffer[i * width + j];
                
                    if (in_line_position == width - 1)
                    {
                        std::cout << newl;
                        in_line_position = 0;
                    }    
                    else
                    {
                        in_line_position++;
                    }
                    in_line_position++;
                }
            }
        }
    }
    std::cout << newl;

    // status bar

    std::cout << bg_blue;
    string status =
    "chars: "  + std::to_string(buffer.size()) + ' ' + '|' + ' ' +
    "lines: "  + std::to_string(lines.size())  + ' ' + '|' + ' ' + 
    "width: "  + std::to_string(width)         + ' ' + '|' + ' ' + 
    "height: " + std::to_string(height)        ;
    status += ' '*(width - status.size() - file_name.size());
    status += file_name;
}
