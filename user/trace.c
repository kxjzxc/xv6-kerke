#include "kernel/types.h"
#include "user.h"
#include "kernel/param.h"
int main(int argc,char* argv[]){
    if(argc < 3){
        printf("Trace needs two argument!\n"); //检查参数数量是否正确
        exit(-1);
    }
    int arg = atoi(argv[1]); //将字符串参数转为整数
    //跟踪
    trace(arg);
    //执行程序
    char *argvs[MAXARG];
    for(int i=2;i<argc;i++)
    {
        argvs[i-2]=argv[i];
    }
    argvs[argc-2]=0;
    exec(argv[2],argvs);
    exit(0); //确保进程退出
}
