#ifndef WINDOW_H
#define WINDOW_H

#include <termios.h>

extern int tty_raw(int fd);
extern int tty_reset(int fd);

extern void tty_atexit(void);

extern struct termios *tty_termios(void);

#endif /* !WINDOW_H */
