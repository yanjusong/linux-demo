#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#define MAXLINE 1024

void handle_client(int fd)
{
    // 假设此时 client 奔溃, 那么 server 将接收到 client 发送的 FIN
    sleep(10);
    // 写入第一条消息
    char msg1[MAXLINE] = "Hello";
    ssize_t n = write(fd, msg1, strlen(msg1));
    printf("write %ld bytes\n", n);
    // 此时第一条消息发送成功，server 接收到 client 发送的 RST
    sleep(1);
    // 写入第二条消息，出现 SIGPIPE 信号，导致 server 被杀死
    char msg2[MAXLINE] = "World";
    n = write(fd, msg2, strlen(msg2));
    printf("%ld, %s\n", n, strerror(errno));
}

int setSockReuse(int sock)
{
    int reuse = 1;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
    {
        return -1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int)) < 0)
    {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    int serv_sock;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    serv_sock = socket(AF_INET , SOCK_STREAM , 0);
    setSockReuse(serv_sock);
    bind(serv_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(serv_sock, 128);
    int fd = accept(serv_sock, NULL, NULL);

    handle_client(fd);

    getchar();
    return 0;
}
