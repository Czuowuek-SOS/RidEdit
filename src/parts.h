#include <windows.h>

#define red     "\033[1;31m"
#define green   "\033[1;32m"
#define yellow  "\033[1;33m"
#define blue    "\033[1;34m"
#define magenta "\033[1;35m"
#define cyan    "\033[1;36m"
#define reset   "\033[0m"

#define bg_red     "\033[1;41m"
#define bg_green   "\033[1;42m"
#define bg_yellow  "\033[1;43m"
#define bg_blue    "\033[1;44m"
#define bg_magenta "\033[1;45m"
#define bg_cyan    "\033[1;46m"
#define bg_reset   "\033[0m"

#define bold      "\033[1m"
#define underline "\033[4m"
#define blink     "\033[5m"
#define reverse   "\033[7m"
#define concealed "\033[8m"
#define strike    "\033[9m"

#define Esc     27
#define newl    "\n"
#define space   ' '


#define arrow_up    72
#define arrow_down  80
#define arrow_left  75
#define arrow_right 77


#define sep     green + '~' + reset

#define cls()         printf("\x1B[2J\x1B[H")
#define gotoxy(x,y)   printf("\033[%d;%dH", (y), (x))

#define CTRL(x) ((x) & 0x1f)

