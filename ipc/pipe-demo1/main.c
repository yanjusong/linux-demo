#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

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
    int		n;
    int		fd[2];
    pid_t	pid;
    char	line[MAXLINE];

    if (pipe(fd) < 0)
        err_sys("pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {		/* parent */
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    } else {					/* child */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    return 0;
}
