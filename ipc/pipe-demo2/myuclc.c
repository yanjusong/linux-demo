#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>

static void err_sys(const char *fmt, ...)
{
    va_list		ap;

    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
    exit(1);
}

int main(void)
{
    int c;

    while ((c = getchar()) != EOF) {
        if (isupper(c))
            c = tolower(c);
        if (putchar(c) == EOF)
            err_sys("output error");
        if (c == '\n')
            fflush(stdout);
    }

    return 0;
}
