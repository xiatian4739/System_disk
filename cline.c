// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct disk
{
    char dev[20];
    char type[10];
    unsigned int tital;
    unsigned int user;
    unsigned int ava;
    char bfb[4];
    char lod[40];
} disk_info;

int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000); // 大端端口
    inet_pton(AF_INET, "182.43.48.70", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect");
        exit(0);
    }

    // 3. 和服务器端通信
    int number = 0;
    while (1)
    {
        // 发送数据
        char buf[1024];
        sprintf(buf, "你好, 服务器...%d\n", number++);
        write(fd, buf, strlen(buf) + 1);

        // 接收数据
        memset(buf, 0, sizeof(buf));
        int len = read(fd, buf, sizeof(buf));
        if (len > 0)
        {
            disk_info *df = (disk_info *)buf;
            printf("文件系统:%s\n", df->dev);
            printf("类型:%s\n", df->type);
            printf("总大小:%u\n", df->tital);
            printf("已用:%u\n", df->user);
            printf("可用:%u\n", df->ava);
            printf("百分比:%s\n", df->bfb);
            printf("挂载路径:%s\n", df->lod);
            // printf("服务器say: %s\n", buf);
        }
        else if (len == 0)
        {
            printf("服务器断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
        sleep(1); // 每隔1s发送一条数据
    }

    close(fd);

    return 0;
}
