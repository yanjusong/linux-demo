#include <stdio.h>
#include <arpa/inet.h>

void prvalue(const char *tips, void *p, size_t n)
{
    int i = 0;
    char *ptr = (char *)p;

    printf("\n%s ->\n", tips);

    for (i = 0; i < n; ++i) 
    {
        printf("%#x:%#x\n", &ptr[i], ptr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x1234567812345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    prvalue("Host ordered port", (void *)&host_port, sizeof(host_port));
    prvalue("Network ordered port", (void *)&net_port, sizeof(net_port));
    prvalue("Host ordered address", (void *)&host_addr, sizeof(host_addr));
    prvalue("Network ordered address", (void *)&net_addr, sizeof(net_addr));

    return 0;
}