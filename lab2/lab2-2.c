#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void waiting(),stop(),alarming();
int wait_mark;

int main(int argc,char *argv[])
{
        int p1,p2;
        if(p1=fork())  //Create subprocess1
        {
                if(p2=fork())    //Create subprocess2
                {
                        wait_mark=1;
                        signal(SIGINT,stop);  //Change to stop when getting the signal ^c
                        signal(SIGALRM,alarming); //Getting SIGALRM
                        waiting();
                        kill(p1,16);  //Sending soft interrupt signal 16 to p1
                        kill(p2,17);  //Sending soft interrupt signal 17 to p2

                        wait(0);
                        wait(0);
                        printf("Parent process is killed!\n");
                        exit(0);
                }
                else
                {
                        wait_mark=1;
                        signal(17,stop);
                        signal(SIGINT,SIG_IGN);  //Ignore ^c signal
                        while(wait_mark!=0);
                        lockf(1,1,0);
                        printf("Child process2 id killed by parent!\n");
                        lockf(1,0,0);
                        exit(0);
                }
        }
        else
        {
                wait_mark=1;
                signal(16,stop);
                signal(SIGINT,SIG_IGN);   //Ignore ^c signal
                while(wait_mark!=0);
                lockf(1,1,0);
                printf("Child process1 is killed by parent!\n");
                lockf(1,0,0);
                exit(0);
        }
}

void waiting()
{
        sleep(5);
        if(wait_mark!=0)
                kill(getpid(),SIGALRM);
}

void alarming()
{
        wait_mark=0;
}

void stop()
{
        wait_mark=0;
}
    
