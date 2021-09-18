# raw socket 进阶
通过 bind 函数在特定接口上监听特定 etherType 的报文

```c
void BindToInterface(int raw , const char *device , int protocol) { 
    struct sockaddr_ll sll;
    struct ifreq ifr; bzero(&sll , sizeof(sll));
    bzero(&ifr , sizeof(ifr)); 
    strncpy((char *)ifr.ifr_name ,device , IFNAMSIZ); 
    //copy device name to ifr 
    if((ioctl(raw , SIOCGIFINDEX , &ifr)) == -1)
    { 
        perror("Unable to find interface index");
        exit(-1); 
    }
    sll.sll_family = PF_PACKET; 
    sll.sll_ifindex = ifr.ifr_ifindex; 
    sll.sll_protocol = protocol; 
    if((bind(raw , (struct sockaddr *)&sll , sizeof(sll))) ==-1)
    {
        perror("bind: ");
        exit(-1);
    }

} 
```

# Tests
1. 如果注释掉绑定协议号的语句（即`sll.sll_protocol = protocol; `），还可以在特定接口上接收数据吗？

    仍然可以，不过会失去对报文的流向控制，从这个接口发出去的报文也会从被收上来。

2. 在 [stackoverflow](https://stackoverflow.com/questions/3998569/how-to-bind-raw-socket-to-specific-interface) 上可以看到用 `setsockopt`函数进行绑定套接字和端口的操作，这样可以成功吗？和我们的代码有什么区别呢？

    区别在于创建套接字时的参数不同。
    
    `socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))` 不适用与上述方法。

3.  这篇[博文](https://www.opensourceforu.com/2015/03/a-guide-to-using-raw-sockets/)在 `recvfrom` 函数中增加了 `&saddr,(socklen_t *)&saddr_len` 这两个参数，与不加的区别在哪里呢？

