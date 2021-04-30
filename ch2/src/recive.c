#include <stdio.h>
#include <stdlib.h>
#include "extract.h"


int main(int argc,char** argv) // 网卡名称
{ 


    int protocol;
	int n_read;
	unsigned char buffer[MAXETHLEN];
	unsigned char* eth_frame;
	int sock_fd;
	eth_frame = buffer;

    unsigned char src_mac[6]; // 设置发送者的mac
    if (argc<=1){
        char *interface="ens34"; // nic name 
        interface2mac(interface,src_mac);

    }else{
        char *interface=argv[1]; // nic name
        interface2mac(interface,src_mac);
    }

    if((sock_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0) // PF_PACKET 收取链路层的数据 
    {
        printf("error while creating raw socket :%d\n",sock_fd);
        return -1;
    }
    while(1)
    {
        n_read = recvfrom(sock_fd, buffer, MAXETHLEN, 0, NULL, NULL);
        if(n_read < 46)
        {
            printf("Eth frame len < 46");
        }
        else{
            extract_eth(eth_frame);
        }
        
    }
	
}
