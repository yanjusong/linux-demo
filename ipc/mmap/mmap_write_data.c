#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[4];
    int age;
} People;

int main(int argc, char **argv) // map a normal file as shared mem:
{
    if (argc < 2) 
    {
        printf("no input file.\n");
        return -1;
    }

    int fd, i;
    People *p_map;
    char temp;

    fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);

    // 将文件扩展到40字节
    lseek(fd, sizeof(People) * 5 - 1, SEEK_SET);
    write(fd, "", 1);

    p_map = (People *)mmap(NULL, sizeof(People) * 10, PROT_READ | PROT_WRITE,
                           MAP_SHARED, fd, 0);
    close(fd);
    temp = 'a';

    for (i = 0; i < 10; i++)
    {
        temp += 1;
        memcpy((*(p_map + i)).name, &temp, 2);
        (*(p_map + i)).age = 20 + i;
    }
    printf(" initialize over \n ");
        sleep(10);
    munmap(p_map, sizeof(People) * 10);
    printf("umap ok \n");

    return 0;
}