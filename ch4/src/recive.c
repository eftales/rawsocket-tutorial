#include <stdio.h>
#include <stdlib.h>
#include "extract.h"


int main(int argc,char** argv) // 网卡名称
{ 
	int n_read;
	unsigned char buffer[MAXETHLEN];
	unsigned char* eth_frame;
	int sock_fd;
	eth_frame = buffer;


    if((sock_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0) // PF_PACKET 收取链路层的数据 ETH_P_ALL 所有的 etherType 都捕获
    {
        printf("error while creating raw socket :%d\n",sock_fd);
        return -1;
    }
    BindToInterface(sock_fd, "ens33" , htons(0x0806));

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
