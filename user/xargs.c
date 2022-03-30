#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
   if(argc < 2){
       printf("error inputs");
       exit(0);
   }//参数不够
   char *cmd = argv[1];
   char *argvs[MAXARG];
   for(int i=1;i<argc;i++)
   {
       argvs[i-1]=argv[i];
   }
   char buff[1024];
    while(read(0,&buff,sizeof(buff)))
    {
        int len = strlen(buff);
        argvs[argc-1]=&buff[0];
        for(int i=0;i<len;i++)
        {
            if(buff[i]=='\n')
            {
                if(fork()==0)
                {
                    buff[i]=0;
                    argvs[argc]=0;
                    exec(cmd,argvs);
                }
                wait(0);
                argvs[argc-1]=&buff[i+1];
            }
        }
    }
    exit(0);

}