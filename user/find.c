#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    char *p;

    //Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
      ;// 向前遍历找到最后一个斜杠字符
    p++;
    return p;// 返回斜杠后面的文件名部分
}

void
find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    
    // 打开指定的路径，如果打开失败则返回错误信息
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    
    // 获取文件的状态信息（如类型、大小等），如果失败则返回错误信息
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        // fd 是一个文件
        case T_FILE:
            if(strcmp(fmtname(path), filename) == 0){
                printf("%s\n", path);// 如果文件名匹配，则打印文件的路径
            }
            break;
        //  fd 是一个目录
        case T_DIR:
            // 检查路径长度是否超过缓冲区限制
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);// 将当前路径复制到缓冲区中
            p = buf+strlen(buf); // 指针移动到路径字符串的末尾
            *p++ = '/';// 在路径末尾添加斜杠
            // open the next file
            while(read(fd, &de, sizeof(de)) == sizeof(de)){// 循环读取目录项
                if((de.inum == 0) || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue; // 跳过无效目录项或当前目录/父目录
                memmove(p, de.name, DIRSIZ);// 将目录项的名字复制到缓冲区中
                p[DIRSIZ] = 0;// 添加字符串终止符
                find(buf, filename);// 递归调用 find() 函数，在子目录中继续查找
            }
            break;
    }
    close(fd);// 关闭文件描述符
}
int
main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(2, "Usage: find directory filename.\n");
        exit(1);// 检查命令行参数，确保传入了正确的参数个数
    }
    find(argv[1], argv[2]);// 调用 find 函数，传入目录和文件名
    exit(0);
}
