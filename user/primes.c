#include "kernel/types.h"
#include "user.h"
#include <stddef.h>
void mapping(int n, int fd[])
{
    close(n);//关闭文件描述符n，令n映射到fd[n]
    dup(fd[n]);
    close(fd[0]);
    close(fd[1]);
}
void primes()
{
    int fd[2];
    pipe(fd);
    int prime;//当前的质数
    int ref = read(0, &prime, sizeof(int));
    if(ref == 0)return;//没有质数了
    printf("prime %d\n", prime);
    int pid = fork();
    if(pid == 0)
    {
        int num;
        mapping(1, fd);
        while(read(0,&num, sizeof(int)))
        {
            if(num%prime == 0)continue;
            write(1, &num, sizeof(int));
        }
    }
    else 
    {
        wait(NULL);
        mapping(0, fd);
        primes();
    }
}
int main(int argc,char* argv[])
{
    int fd[2];
    pipe(fd);//父进程写入，子进程读取
    printf("program start!\n");
    int pid = fork();
    if(pid<0)
    {
        printf("error!");
    }
    else if(pid == 0)
    {
        /*子进程 */
        mapping(1,fd);
        for(int i = 2;i <= 35; i++)
            write(1, &i, sizeof(int));

    }
    else{
        /*父进程*/ 
        wait(NULL);
        mapping(0, fd);
        primes();
    }
    exit(0);
}