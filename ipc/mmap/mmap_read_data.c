#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

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
    fd = open(argv[1], O_CREAT | O_RDWR, 0666);
    p_map = (People *)mmap(NULL, sizeof(People) * 10, PROT_READ | PROT_WRITE,
                           MAP_SHARED, fd, 0);
    for (i = 0; i < 10; i++)
    {
        printf("name: %s age %d;\n", (*(p_map + i)).name, (*(p_map + i)).age);
    }

    munmap(p_map, sizeof(People) * 10);

    return 0;
}