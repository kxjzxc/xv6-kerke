#include "kernel/types.h"
#include "user.h"
int main(int argc,char* argv[])
{
    int fp[2],sp[2];
    pipe(fp);//父进程写入，子进程读取
    pipe(sp);
    printf("program start!\n");
    int pid = fork();
    if(pid<0)
    {
        printf("error!");
    }
    else if(pid == 0)
    {
        /*子进程 */
        char *buffer = "    ";
        close(fp[1]); // 关闭写端
        read(fp[0], buffer, 4);//阻塞等待
        printf("%d: received %s\n",getpid(),buffer);
        close(fp[0]); // 读取完成，关闭读端
        char *intput = "pong";
        close(sp[0]); // 关闭读端
        write(sp[1], intput, 4);
        close(sp[1]); // 写入完成，关闭写端

    }
    else{
        /*父进程*/ 
        char *buffer = "ping";
        close(fp[0]); // 关闭读端
        write(fp[1], buffer, 4);
        close(fp[1]); // 写入完成，关闭写端
        close(sp[1]); // 关闭写端
        read(sp[0], buffer, 4);
        printf("%d: received %s\n",getpid(),buffer);
        close(sp[0]); // 读取完成，关闭读端
    }
    exit(0);
}