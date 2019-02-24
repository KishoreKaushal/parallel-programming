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
    /* 
        Disable Ctrl-S and Ctrl-Q and Fix Ctrl-M
        and some miscellaneous flags which 
        don’t really apply to modern terminal emulators
    */
    raw.c_iflag &= ~(BRKINT | IXON | ICRNL | INPCK | ISTRIP);     

    /* Diable Ctrl-V */
    raw.c_lflag &=  ~(ECHO | ICANON | IEXTEN | ISIG);

    /* Turn off all output processing */
    raw.c_oflag &= ~(OPOST);

    /* sets the character size (CS) to 8 bits per byte */
    raw.c_cflag |= (CS8);

    /* setting timeout for read() */
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    while (1) {
        char c = '\0';
        
        read(STDIN_FILENO, &c, 1);

        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }

        if (c == 'q') break;
    }

    return 0;
}