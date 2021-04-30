#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>            // errno, perror()
#include "encapsulate.h"


void interface2mac(char* interface,char* src_mac)
{
    int sd;
    struct ifreq ifr;
    if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {//第一次创建socket是为了获取本地网卡信息
        perror ("socket() failed to get socket descriptor for using ioctl() ");
        exit(EXIT_FAILURE);
    }
    // Use ioctl() to look up interface name and get its MAC address.
    memset (&ifr, 0, sizeof (ifr));
    snprintf (ifr.ifr_name, sizeof (ifr.ifr_name), "%s", interface);
    if (ioctl (sd, SIOCGIFHWADDR, &ifr) < 0) {
        perror ("ioctl() failed to get source MAC address ");
        exit(EXIT_FAILURE);
    }
    close(sd);
    // Copy source MAC address.
    memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6);
}

int hex2dec(unsigned char hex) {
    if (hex - '0' <= 9) {
        return  hex - '0';
    }
    else {
        if (hex - '0' > 48) {
            return hex - 'a' + 10;
        }
        else {
            return hex - 'A' + 10;
        }
    }
}



void mac_str2mac(unsigned char* mac_str, unsigned char* mac) {
    int i = 0, k = 0;
    while (i < 17) {
        mac[k] = 16 * hex2dec(mac_str[i]) + hex2dec(mac_str[i + 1]);

        k += 1;
        i += 3;

    }
}


char* encapsulate_udp(udp *udp_pack,unsigned char* data,int len){

    char* upd_frame = malloc(sizeof(unsigned char)*(len+UDPHEADLEN+1));
    memcpy(upd_frame,udp_pack,UDPHEADLEN);
    memcpy(upd_frame+UDPHEADLEN,data,len);
    return upd_frame;

}

char* encapsulate_ip(ip *ip_pack,unsigned char* udp,int len){
    char* ip_frame = malloc(sizeof(unsigned char)*(len+IPHEADLEN+1));
    memcpy(ip_frame,ip_pack,IPHEADLEN);
    memcpy(ip_frame+IPHEADLEN,udp,len);
    return ip_frame;
}




char* encapsulate_eth(eth* eth_pack,unsigned char* ip,int len){
    char* eth_frame = malloc(sizeof(unsigned char)*(len+ETHHEADLEN+1));
    memcpy(eth_frame,eth_pack,ETHHEADLEN);
    memcpy(eth_frame+ETHHEADLEN,ip,len);
    return eth_frame;
}

void send_frame(char* interface,char* frame,int len){
    struct sockaddr_ll device;
    memset (&device, 0, sizeof (device));
    if ((device.sll_ifindex = if_nametoindex (interface)) == 0) {
        perror ("if_nametoindex() failed to obtain interface index ");
        exit (EXIT_FAILURE);
    }
    device.sll_family = AF_PACKET;
    memcpy (device.sll_addr, frame+6, 6);
    device.sll_halen = htons (6);


    int sd;
    // Submit request for a raw socket descriptor.
    if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {//创建正真发送的socket
        perror ("socket() failed ");
        exit (EXIT_FAILURE);
    }
    // Send ethernet frame to socket.
    if ((sendto (sd, frame, len, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
        perror ("sendto() failed");
        exit (EXIT_FAILURE);
    }
    printf ("sended.\n");     
    close (sd);
}