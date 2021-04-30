#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SERV_PORT   8000

int main()
{
  // sock_fd --- socket文件描述符 创建udp套接字
  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0); //int domain, int type, int protocol
  if(sock_fd < 0)
  {
    perror("create socket failed.");
    exit(1);
  }

  // 将套接字和IP、端口绑定
  struct sockaddr_in addr_serv; // 本机信息
  int len;
  memset(&addr_serv, 0, sizeof(struct sockaddr_in));//初始化,每个字节都用0填充
  addr_serv.sin_family = AF_INET; //使用IPV4地址
  addr_serv.sin_port = htons(SERV_PORT); //端口
  // INADDR_ANY 表示监听本机上所有的IP地址（192.168.1.X,127.0.0.1 ...）
  addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);  //INADDR_ANY == inet_addr("0.0.0.0")
  len = sizeof(addr_serv);

  // 绑定socket (本质为显式设置 socket 的参数)
  if(bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
  {
    perror("bind error.");
    exit(1);
  }


  int recv_num;
  int send_num;
  char send_buf[20] = "i am server!";
  char recv_buf[20];
  struct sockaddr_in addr_client; //  remote 信息

  while(1)
  {
    printf("server wait:\n");

    recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len); // 等待客户端接入

    if(recv_num < 0)
    {
      perror("recvfrom error.");
      exit(1);
    }

    recv_buf[recv_num] = '\0';
    printf("server receive %d bytes: %s\n", recv_num, recv_buf);

    send_num = sendto(sock_fd, send_buf, recv_num, 0, (struct sockaddr *)&addr_client, len); // 向客户端发送信息

    if(send_num < 0)
    {
      perror("sendto error:");
      exit(1);
    }
  }

  close(sock_fd);

  return 0;
}