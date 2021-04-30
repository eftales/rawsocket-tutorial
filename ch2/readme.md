# raw socket 接收数据

接收数据有点类似服务器


## Question
1. 为什么不需要绑定地址

    htons(ETH_P_ALL) 参数表示收取所有的帧

    ```c
    sock_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))
    ```

    在进阶篇中会介绍如何将 raw socket 绑定到特定的协议地址和网卡上

2. raw socket 会导致上层收不到数据嘛？

    No
    
    上行和下行的数据包都会接收到
