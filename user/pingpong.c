//引入头文件 kernel/types.h 和 user/user.h
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2], p2[2];
    char buf[] = {'A'}; // 一个字节
    int n = sizeof(buf);

    // 创建两个管道
    pipe(p1);//用于父进程传递信息给子进程
    pipe(p2);//用于子进程传递信息给父进程

    // 创建子进程
    if(fork() == 0) {

        close(p1[1]); // 关闭子进程不使用的写入端
        close(p2[0]); // 关闭子进程不使用的读取端

        // 从管道p1读取数据，并检查读取是否成功
        if(read(p1[0],buf,n) != n){
            printf("Error: child read from p1 with parent failed.");
            exit(1);
        }

        // 打印子进程的进程ID和收到的信息
        printf("%d: received ping\n", getpid());

        // 向管道p2写入数据，并检查写入是否成功
        if(write(p2[1],buf,n) != n){
            printf("Error: child write into p2 with parent failed.");
            exit(1);
        }

        // 子进程退出
        exit(0);
    } else {
        close(p1[0]); // 关闭父进程不使用的读取端
        close(p2[1]); // 关闭父进程不使用的写入端

        // 向管道p1写入数据，并检查写入是否成功
        if(write(p1[1],buf,n) != n){
            printf("Error: parent write into p1 with child failed.");
            exit(1);
        }

        // 从管道p2读取数据，并检查读取是否成功
        if(read(p2[0],buf,n) != n){
            printf("Error: parent read from p2 with child failed.");
            exit(1);
        }

        // 打印父进程的进程ID和收到的信息
        printf("%d: received pong\n",getpid());

        // 等待子进程退出
        wait(0);

        // 父进程退出
        exit(0);
    }
}
