#include "extract.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 绑定到指定接口和 ethertype
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

void extract_eth(unsigned char* eth_frame){
    eth eth_pack;
    memcpy(&eth_pack,eth_frame,ETHHEADLEN);

    eth_pack.eth_type_len = ntohs(*(unsigned short*)(eth_frame+12));//转换字节序

    eth_pack.lsc_pack = eth_frame + ETHHEADLEN;

    printf("-------Eth-------\n");
    printf("Destination: %02x:%02x:%02x:%02x:%02x:%02x\n",eth_pack.dst_mac[0], eth_pack.dst_mac[1], eth_pack.dst_mac[2], eth_pack.dst_mac[3], eth_pack.dst_mac[4], eth_pack.dst_mac[5]);
    printf("Source: %02x:%02x:%02x:%02x:%02x:%02x\n",eth_pack.src_mac[0], eth_pack.src_mac[1], eth_pack.src_mac[2], eth_pack.src_mac[3], eth_pack.src_mac[4], eth_pack.src_mac[5]);
    printf("type: %u\n",eth_pack.eth_type_len);

    switch(eth_pack.eth_type_len){
        case 0x0800:
            //extract_ip(eth_pack.lsc_pack);
            break;
        default:
            //printf("eth_type_len=%d, unknown\n",eth_pack.eth_type_len);
            break;
    }

}


void extract_ip(unsigned char* ip_frame){
    ip ip_pack;
    memcpy(&ip_pack,ip_frame,IPHEADLEN);

    ip_pack.hl = *(ip_frame) & 0xf;
	ip_pack.v = *(ip_frame) >> 4;
    ip_pack.len = ntohs(ip_pack.len);
    ip_pack.id = ntohs(ip_pack.id);
    ip_pack.checksum = ntohs(ip_pack.checksum);
	ip_pack.flags = *(ip_frame+6) >> 5;
	ip_pack.off = ((*(unsigned short *)(ip_frame+6) & 0x1f) << 8) + *(ip_frame+6 + 1); 
    ip_pack.data = ip_frame + IPHEADLEN;
    switch(ip_pack.pro){
        case 17:
            extract_udp(ip_pack.data);
            break;
        default:
            printf("ip_pack.pro=%d,unknow\n",ip_pack.pro);
    }
}

void extract_udp(unsigned char* udp_frame){
    udp udp_pack;
    memcpy(&udp_pack,udp_frame,UDPHEADLEN);
    udp_pack.src_port = ntohs(udp_pack.src_port);
    udp_pack.dst_port = ntohs(udp_pack.dst_port);
    udp_pack.len = ntohs(udp_pack.len);
    udp_pack.checksum = ntohs(udp_pack.checksum);
    udp_pack.data = udp_frame + UDPHEADLEN;
    process_data(udp_pack.data,udp_pack.len-UDPHEADLEN);

}

void process_data(unsigned char* data,int len){
    data[len] = '\0';
    printf("data: %s\n",data);
}
