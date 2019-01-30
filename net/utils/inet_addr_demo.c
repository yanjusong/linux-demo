#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

/**
 * @brief: 点分式的IP地址转化为网络序(大端)的整型
 * @pramas:
 *     const char *addr: 点分式的IP地址字符串
 *     int *n: 网络序(大端)的整型
 * @return:
 *    -1: 转化失败，给定字符串不是合法IP
 *     1: 转化成功
 * @attention: inet_addr具有局限性
 *     - 函数调用成功有些系统返回INADDR_NONE，有些系统返回-1
 *     - "255.255.255.255"转化会失败
 */
int aton(const char *addr, int *n)
{
    int stat;

    stat = inet_addr(addr);
    *n = (int)stat;

    return (stat == INADDR_NONE ? -1 : stat);
}

void test(const char *addr)
{
    int stat;
    int n;

    stat = aton(addr, &n);
    printf("addr:%s\n", addr);
    printf("Convert stat: %d\n", stat);
    printf("Network ordered integer addr: %#x \n\n", n);
}

int main(int argc, char *argv[])
{
    test("192.168.23.77");
    test("255.255.255.255");
    test("0.0.0.0");
    test("256.168.23.77");
    test("");
    test("192.168.2");

    return 0;
}