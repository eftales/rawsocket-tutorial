#ifndef __EXTRACT__
#define __EXTRACT__

#pragma pack(1)

// socket head files
#include <netdb.h>            // struct addrinfo
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t, uint32_t
#include <sys/socket.h>       // needed for socket()
#include <netinet/in.h>       // IPPROTO_ICMP, INET_ADDRSTRLEN
#include <netinet/ip.h>       // struct ip and IP_MAXPACKET (which is 65535)
#include <arpa/inet.h>        // inet_pton() and inet_ntop()
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl.
#include <net/if.h>           // struct ifreq
#include <linux/if_ether.h>   // ETH_P_IP = 0x0800, ETH_P_IPV6 = 0x86DD
#include <linux/if_packet.h>  // struct sockaddr_ll (see man 7 packet)
#include <net/ethernet.h>

// linux head files
#include <unistd.h>
#include <errno.h>            // errno, perror()

// const
#define MAXETHLEN 1600
#define ETHER_LSC 0x09ab


#define ETHHEADLEN 14
#define LSCHEADLEN 8
#define IPHEADLEN 20
#define UDPHEADLEN 8
#define TCPHEADLEN 40
#define LSCLEN 8


// udp
/// --struct
typedef struct
{
	unsigned short src_port;
	unsigned short dst_port;
	unsigned short len;
	unsigned short checksum;
    unsigned char* data;
}udp;


// ip
/// --struct
typedef struct
{
	unsigned char hl:4, v:4;
	//hl为一个字节中的低4个bits，v为一个字节中的高4个bits
	unsigned char tos;
	unsigned short len;
	unsigned short id;
	unsigned short off:13, flags:3;
	unsigned char ttl;
	unsigned char pro;
	unsigned short checksum;
	unsigned char src[4];
	unsigned char dst[4];
    unsigned char* data;
}ip;


// lsc
/// --struct
typedef struct
{
    unsigned char lsc_pre[2];
    unsigned char lsc_colony_id[3];
    unsigned char lsc_dst;
    unsigned char lsc_src;
    unsigned char lsc_ctrl;
    char* ip_pack; 
}lsc;



// eth
/// --struct
typedef struct
{
    unsigned char dst_mac[6];
	unsigned char src_mac[6];
    unsigned short eth_type_len;
	char* lsc_pack; 
}eth;


// data process
void process_data(unsigned char* data,int len);


// udp
/// --func
void extract_udp(unsigned char* udp_frame);

// ip
/// --func
void extract_ip(unsigned char* ip_frame);

// lsc
/// --func
void extract_lsc(unsigned char* lsc_frame);


// eth
/// --func
void extract_eth(unsigned char* eth_frame);

// tools
/// 读取网络接口名称的mac地址
void interface2mac(char* interface,char* src_mac);

/// mac字符串转换为mac地址
void mac_str2mac(unsigned char* mac_str, unsigned char* mac);

///
int hex2dec(unsigned char hex);

#endif