#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>

static void err_sys(const char *fmt, ...)
{
    va_list		ap;

    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
    exit(1);
}

#define	MAXLINE	4096			/* max line length */

int main(void)
{
    char	line[MAXLINE];
    FILE	*fpin;

    if ((fpin = popen("./myuclc.out", "r")) == NULL)
        err_sys("popen error");

    for ( ; ; ) {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if (fgets(line, MAXLINE, fpin) == NULL)	/* read from pipe */
            break;
        if (fputs(line, stdout) == EOF)
            err_sys("fputs error to pipe");
    }

    if (pclose(fpin) == -1)
        err_sys("pclose error");

    putchar('\n');
    return 0;
}
