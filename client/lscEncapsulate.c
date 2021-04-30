#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>            // errno, perror()
#include "lscEncapsulate.h"

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


char* encapsulate_lsc(lsc* lsc_pack,unsigned char* ip,int len){
    char* lsc_frame = malloc(sizeof(unsigned char)*(len+LSCHEADLEN+1));
    memcpy(lsc_frame,lsc_pack,LSCHEADLEN);
    memcpy(lsc_frame+LSCHEADLEN,ip,len);
    return lsc_frame;
}



char* encapsulate_eth(eth* eth_pack,unsigned char* lsc,int len){
    char* eth_frame = malloc(sizeof(unsigned char)*(len+ETHHEADLEN+1));
    memcpy(eth_frame,eth_pack,ETHHEADLEN);
    memcpy(eth_frame+ETHHEADLEN,lsc,len);
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