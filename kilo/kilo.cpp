#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(IXON | ICRNL);     /* Disable Ctrl-S and Ctrl-Q and Fix Ctrl-M*/
    raw.c_lflag &=  ~(ECHO | ICANON | IEXTEN | ISIG);    /* Diable Ctrl-V */
    raw.c_oflag &= ~(OPOST);    /* Turn off all output processing */

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c;
    
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);      /* manually processing the ouputs */
        }
    }

    return 0;
}