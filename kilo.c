/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void die(const char *s) {
    perror(s);
    exit(1);
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    // disable a few flags.
    // BRKINT: disable break condition signal
    // ICRNL: disable \r to \n input translation
    // INPCKC: disable parity checking
    // ISTRIP: don't strip 8th bit
    // IXON: disable flow control
    // OPOST: disable \r to \n output translation
    // CS8: set character size to 8 bits perbyte
    // ECHO: don't echo stdin
    // ICANON: disable canonical mode
    // IEXTEN: disable special input processing
    // ISIG: don't generate signals
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // set min bytes of input for read()
    raw.c_cc[VMIN] = 0;

    // set max time to wait for read()
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/

int main() {
    enableRawMode();

    while(1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");

        if (isprint(c)) {
            printf("%d ('%c')\r\n", c, c);
        } else {
            printf("%d\r\n", c);
        }
        if (c == 'q') break;
    }

    return 0;
}
