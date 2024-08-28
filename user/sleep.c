#include "kernel/types.h" 
#include "user/user.h"   

int main(int argc, char const *argv[])
{
  // 如果命令行参数不等于2个，则打印错误信息
  if (argc != 2) { 
    printf("error! usage: sleep <time>\n"); // 打印错误信息
    // 调用 exit(1) 函数使程序异常退出，返回 1 表示有错误发生
    exit(1); // 异常退出
  }

  // 如果参数数量正确，调用 sleep 函数暂停指定的时间
  // 使用 atoi 函数将 argv[1]（字符串形式的时间参数）转换为整数
  sleep(atoi(argv[1]));

  // 正常退出程序，返回 0 表示程序成功执行
  exit(0);
}

