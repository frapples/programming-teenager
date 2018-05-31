#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<assert.h>
#define NDEBUG

/* unix下fork()函数用于创建子进程，位于头文件unistd.h中。 */

enum{
    PIPE_READ,
    PIPE_WRITE,
    PIPE_TOTAL,
};

#define BUF_SIZE 100

void parent(int pipeWrite);
void children(int pipeRead);


int main(void)
{
    //snprintf(NULL, 0, NULL);
    int fd[PIPE_TOTAL];
    pipe(fd);
    

    pid_t pid = fork();
    if(pid){ //父进程
        close(fd[PIPE_READ]);
        parent(fd[PIPE_WRITE]);
    } else { //子进程
        close(fd[PIPE_WRITE]);
        children(fd[PIPE_READ]);
    }
    return 0;
}

void parent(int pipeWrite)
{
    char buf[BUF_SIZE];
    fgets(buf, sizeof(buf), stdin);
    write(pipeWrite, buf, strlen(buf));
    usleep(500);
}

void children(int pipeRead)
{
    char buf[BUF_SIZE];
    read(pipeRead, buf, sizeof(buf));
    puts(buf);
    usleep(500);
}

