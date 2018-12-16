#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    char *str_ptr;
    char ipstr[4] = {1, 0, 0, 127};
    uint32_t ip32;

    memcpy((void *)&ip32, (void *)ipstr, 4);
    addr.sin_addr.s_addr = htonl(ip32);

    str_ptr = inet_ntoa(addr.sin_addr);
    printf("Dotted-Decimal notation1: %s \n", str_ptr);

    return 0;
}
