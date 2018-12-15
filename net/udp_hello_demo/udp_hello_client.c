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
    char msg[] = "hello! I'm client.";
    char recv_msg[BUF_SIZE];
    socklen_t adr_sz;
    int str_len;

    struct sockaddr_in your_adr;
    socklen_t your_adr_sz;
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);
    your_adr.sin_port = htons(atoi(argv[2]));

    sendto(sock, msg, sizeof(msg), 0,
           (struct sockaddr *)&your_adr, sizeof(your_adr));

    adr_sz = sizeof(your_adr);
    str_len = recvfrom(sock, recv_msg, BUF_SIZE, 0,
                       (struct sockaddr *)&your_adr, &adr_sz);
    printf("Message form server: %s \n", recv_msg);

    close(sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
