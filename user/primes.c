//引入头文件 kernel/types.h 和 user/user.h
#include "kernel/types.h"
#include "user/user.h"

void primes(int p[2]){
    int n, prime;
    int p1[2];
    close(p[1]); // 关闭父进程传递过来的管道的写端，当前进程只需要从管道中读取数据
    if(read(p[0], &prime, 4) == 0){
        close(p[0]);// 如果从管道读取不到数据，则关闭读端并退出
        exit(1);
    } else {
        printf("prime %d\n", prime);// 读取到的第一个数是当前的素数，将其打印出来
        pipe(p1);// 创建一个新的管道，用于传递剩余未被筛选的数字
        if(fork() == 0) {
            close(p[0]);// 子进程不需要从父管道读取数据，关闭读取端
            primes(p1);// 在新的子进程中递归调用 primes()，继续筛选数字
        } else {
            close(p1[0]);// 不需要新管道的读端，关闭它
            while(1){
                if(read(p[0],&n, 4) == 0) break;// 从父管道中读取剩余的数字
                if(n % prime != 0) write(p1[1], &n, 4);// 如果数字不能被当前素数整除，则将其写入新管道
            }
            close(p[0]);// 关闭父管道读取端
            close(p1[1]);//关闭新管道的写入端
            wait(0); // 等待子进程完成
        }
    }
    exit(0);
}

int
main(int argc, char *argv[]){
    if(argc != 1){
        fprintf(2," Usage: error.\n");
        exit(1);// 如果传入了参数，输出错误信息并退出
    }
    int n;
    int p[2]; // 创建一个管道 p，父进程将通过它传递初始数据给第一个子进程
    pipe(p);
    if(fork() == 0) {
        primes(p);// 创建第一个子进程，开始筛选素数
    } else {
        close(p[0]);// 父进程不需要从管道中读取数据，关闭读取端
        for(n = 2 ; n <= 35 ; n ++ ){// 向管道中写入 2 到 35 的数字
            write(p[1], &n, 4);// 将数字写入管道的写入端，每个数字占用 4 字节
        }
        close(p[1]);// 关闭管道的写入端，所有数据已经写完
        wait(0);// 等待子进程完成工作
    }
    exit(0);// 父进程完成任务，退出
}
