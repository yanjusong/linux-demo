#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h> 

#define CLI_PATH "/var/tmp/"
#define CLI_PERM S_IRWXU /* rwx for user only */

/*
 * Create a client endpoint and connect to a server.
 * Returns fd if all OK, <0 on error.
 */
int cli_conn(const char *name)
{
    int fd, len, err, rval;
    struct sockaddr_un un, sun;
    int do_unlink = 0;

    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;
        return (-1);
    }

    /* create a UNIX domain stream socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-1);

    /* fill socket address structure with our address */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());
    printf("file is %s\n", un.sun_path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    unlink(un.sun_path); /* in case it already exists */
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -2;
        goto errout;
    }
    if (chmod(un.sun_path, CLI_PERM) < 0)
    {
        rval = -3;
        do_unlink = 1;
        goto errout;
    }

    /* fill socket address structure with server's address */
    memset(&sun, 0, sizeof(sun));
    sun.sun_family = AF_UNIX;
    strcpy(sun.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if (connect(fd, (struct sockaddr *)&sun, len) < 0)
    {
        rval = -4;
        do_unlink = 1;
        goto errout;
    }
    return (fd);

errout:
    err = errno;
    close(fd);
    if (do_unlink)
        unlink(un.sun_path);
    errno = err;
    return (rval);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

#define MAX_BUF_SIZE 64

int main(int argc, char *argv[])
{
    int fd;
    int n;
    char buf[MAX_BUF_SIZE];

    if (argc != 2)
        error_handling("invaild params.");

    fd = cli_conn(argv[1]);
    if (fd < 0)
        error_handling("connect to servce error.");
    
    n = read(fd, (void *)buf, MAX_BUF_SIZE);
    if (n < 0)
        error_handling("read error.");
    
    printf("Receive message from server: %s, totally %d bytes.", buf, n);

    return 0;
}
