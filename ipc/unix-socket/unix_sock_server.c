#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define QLEN 10

/*
 * Create a server endpoint of a connection.
 * Returns fd if all OK, <0 on error.
 */
int serv_listen(const char *name)
{
    int fd, len, err, rval;
    struct sockaddr_un un;

    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;
        return (-1);
    }

    /* create a UNIX domain stream socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-2);

    unlink(name); /* in case it already exists */

    /* fill in socket address structure */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    /* bind the name to the descriptor */
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -3;
        goto errout;
    }

    if (listen(fd, QLEN) < 0)
    { /* tell kernel we're a server */
        rval = -4;
        goto errout;
    }
    return (fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return (rval);
}

#define STALE 30 /* client's name can't be older than this (sec) */

/*
 * Wait for a client connection to arrive, and accept it.
 * We also obtain the client's user ID from the pathname
 * that it must bind before calling us.
 * Returns new fd if all OK, <0 on error
 */
int serv_accept(int listenfd, uid_t *uidptr)
{
    int clifd, err, rval;
    socklen_t len;
    time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;
    char *name;

    /* allocate enough space for longest name plus terminating null */
    if ((name = malloc(sizeof(un.sun_path) + 1)) == NULL)
        return (-1);
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0)
    {
        free(name);
        return (-2); /* often errno=EINTR, if signal caught */
    }

    /* obtain the client's uid from its calling address */
    len -= offsetof(struct sockaddr_un, sun_path); /* len of pathname */
    memcpy(name, un.sun_path, len);
    name[len] = 0; /* null terminate */
    printf("client bind path %s.\n", name);
    if (stat(name, &statbuf) < 0)
    {
        rval = -3;
        goto errout;
    }

#ifdef S_ISSOCK /* not defined for SVR4 */
    if (S_ISSOCK(statbuf.st_mode) == 0)
    {
        rval = -4; /* not a socket */
        goto errout;
    }
#endif

    if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
        (statbuf.st_mode & S_IRWXU) != S_IRWXU)
    {
        rval = -5; /* is not rwx------ */
        goto errout;
    }

    staletime = time(NULL) - STALE;
    if (statbuf.st_atime < staletime ||
        statbuf.st_ctime < staletime ||
        statbuf.st_mtime < staletime)
    {
        rval = -6; /* i-node is too old */
        goto errout;
    }

    if (uidptr != NULL)
        *uidptr = statbuf.st_uid; /* return uid of caller */
    unlink(name);                 /* we're done with pathname now */
    free(name);
    return (clifd);

errout:
    err = errno;
    close(clifd);
    free(name);
    errno = err;
    return (rval);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int servfd, acptfd;
    uid_t clntuid = -1;
    int len, n;
    char msg[] = "Hello!!! unix socket client, I am server.";

    if (argc != 2)
        error_handling("invaild params.");

    servfd = serv_listen(argv[1]);
    if (servfd < 0)
        error_handling("invaild params.");

    acptfd = serv_accept(servfd, &clntuid);
    if (acptfd < 0)
        error_handling("serv_accept error.");
    
    if (clntuid != -1)
        printf("client uid is %d.\n", clntuid);

    len = sizeof(msg);
    n = write(acptfd, (void *)msg, len);
    if (n != len)
        error_handling("write error.");

    return 0;
}
