#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>    
#include "lsc.h"

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