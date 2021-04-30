#ifndef __LSCENCAPSULATE__
#define __LSCENCAPSULATE__

#pragma pack(1)

#include "lsc.h"

void interface2mac(char* interface,char* src_mac);

// udp
/// --func
char* encapsulate_udp(udp *udp_pack,unsigned char* data,int len);

// ip
/// --func
char* encapsulate_ip(ip *ip_pack,unsigned char* udp,int len);

// lsc
/// --func
char* encapsulate_lsc(lsc* lsc_pack,unsigned char* ip,int len);


// eth
/// --func
char* encapsulate_eth(eth* eth_pack,unsigned char* lsc,int len);

void send_frame(char* interface,char* frame,int len);
#endif