#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_memcpy()
{
    char buf[] = "0123456789";
    char *psrc = buf;
    char *pdst = psrc + 2;

    printf("before memcpy, buf:%s\n", buf);
    /* 在有些实现上memcpy这种交叉的复制是未定义的 */
    memcpy(pdst, psrc, 5); 
    printf("after memcpy, buf:%s\n\n", buf);
}

void test_memmove()
{
    char buf[] = "0123456789";
    char *psrc = buf;
    char *pdst = psrc + 2;

    printf("before memmove, buf:%s\n", buf);
    /* memmove这种交叉的复制是安全的 */
    memmove(pdst, psrc, 5);
    printf("after memmove, buf:%s\n\n", buf);
}

void test_memccpy()
{
    char buf[] = "0123a456789";
    char _buf[20];
    char *psrc = buf;
    char *pdst = _buf;
    char *p;

    printf("before memccpy, buf:%s\n", buf);
    /* memccpy会复制n个字符到指定buf,除非没有遇到指定字符 */
    p = memccpy(pdst, psrc, 'a', 5);
    if (p) 
        *p = '\0';
    printf("after memccpy, _buf:%s\n\n", _buf);
}

int main()
{
    test_memcpy();
    test_memmove();
    test_memccpy();

    return 0;
}