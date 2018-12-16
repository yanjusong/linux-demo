#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXMSZ 512

void err_sys(const char *fmt, ...)
{
    va_list		ap;

    va_start(ap, fmt);
    printf(ap);

    va_end(ap);
    exit(1);
}

struct mymesg
{
    long mtype;
    char mtext[MAXMSZ];
};

int main(int argc, char *argv[])
{
    key_t key;
    long qid;
    size_t nbytes;
    struct mymesg m;
    int n;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s KEY\n", argv[0]);
        exit(1);
    }

    key = strtol(argv[1], NULL, 0);
    if ((qid = msgget(key, IPC_CREAT | 0666)) < 0)
        err_sys("can't open queue key %s", argv[1]);

    if ( (n = msgrcv(qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0 )
    {
        err_sys("can't recv message");
    }

    m.mtext[n] = '\0';
    printf("Get msg: %s\n", m.mtext);

    exit(0);
}
