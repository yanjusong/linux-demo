#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

/**
 * @brief: 网络序(大端)的整型转化为点分式的IP地址
 * @pramas:
 *     int n: 网络序(大端)的整型
 *     char *addr: 点分式的IP地址字符串，长度必须能够接收合法的IP地址字符串，必须大于INET_ADDRSTRLEN
 * @return:
 *     -1: 转化失败，网络序不能转化为合法IP
 *     1: 转化成功
 */
int ntoa(int n, char *addr)
{
    const char *pstr;
    const void *pn = (const void *)&n;

    pstr = inet_ntop(AF_INET, pn, addr, INET_ADDRSTRLEN);

    return (pstr ? 1 : -1);
}

void printByteArray(char *array, int sz)
{
    int i;
    for (i = 0; i < sz; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void test(int n)
{
    int stat;
    char addr[16];
    char *p = (char *)&n;

    stat = ntoa(n, addr);
    printf("n=%d\n", n);
    printByteArray(p, 4);
    printf("addr:%s\n", addr);
    printf("\n");
}

int main(int argc, char *argv[])
{
    // 1293396160 -> "192.168.23.77"
    test(1293396160);

    // -1 -> "255.255.255.255"
    test(-1);

    // 33597632 -> "192.168.2.0"
    test(33597632);

    return 0;
}
