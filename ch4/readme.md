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
