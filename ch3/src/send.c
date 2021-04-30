#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encapsulate.h"

int main(int argc,char** argv) // 网卡名称 目的主机mac
{

    unsigned char src_mac[6],dst_mac[6];
    char *interface; // nic name
    unsigned char* data = "12345678901234567890123456789012345678901234567890";

    if (argc<=2){
        interface = "ens33"; 
        char dst_mac_str[20] = "00:0c:29:dd:a5:02"; // P4物理运行环境参数
        mac_str2mac(dst_mac_str,dst_mac);

    }else{
        interface = argv[1];
        mac_str2mac(argv[2],dst_mac);
    }
    interface2mac(interface,src_mac);

    udp udp_pack = {
        .src_port = htons(2333),
        .dst_port = htons(2333),
        .len = htons(UDPHEADLEN+50),
        .checksum = 0
    };

    char* udp_frame = encapsulate_udp(&udp_pack,data,50);

    ip ip_pack = {
        .hl = 4,
        .v = 4,
        .tos = 0,
        .len = htons(IPHEADLEN + UDPHEADLEN + 50),
        .id = 0,
        .off = 0,
        .flags = 0,
        .ttl = htons(255),
        .pro = 17

    };
    char* ip_frame = encapsulate_ip(&ip_pack,udp_frame,UDPHEADLEN + 50);
    free(udp_frame);



    eth eth_pack;
    memcpy(&eth_pack.dst_mac,dst_mac,6);
    memcpy(&eth_pack.src_mac,src_mac,6);
    eth_pack.eth_type_len = htons(0x0800);

    char* eth_frame = encapsulate_eth(&eth_pack,ip_frame,IPHEADLEN+UDPHEADLEN + 50);

    send_frame(interface,eth_frame,ETHHEADLEN+IPHEADLEN+UDPHEADLEN + 50);
    return 0;
}