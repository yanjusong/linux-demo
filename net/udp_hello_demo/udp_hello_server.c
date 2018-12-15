#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char recv_msg[BUF_SIZE];
    struct sockaddr_in my_adr, your_adr;
    socklen_t adr_sz;
    int str_len;
    int reuse = 1;
    char msg[] = "hello! I'm server.";

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
        error_handling("setsockopt() error, opt:SO_REUSEADDR");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int)) < 0)
        error_handling("setsockopt() error, opt:SO_REUSEPORT");

    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");

    adr_sz = sizeof(your_adr);
    str_len = recvfrom(sock, recv_msg, BUF_SIZE, 0,
                       (struct sockaddr *)&your_adr, &adr_sz);
    printf("Message form client: %s \n", recv_msg);

    sendto(sock, msg, sizeof(msg), 0,
           (struct sockaddr *)&your_adr, sizeof(your_adr));

    close(sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
