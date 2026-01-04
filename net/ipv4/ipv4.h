#pragma once
#include <stdint.h>
#include <arp/arp.h>
#include <rtl8139/rtl8139.h>
#include <string.h>
// Process an incoming IPv4 packet (Without ethernet header)
void ipv4_process_packets(uint8_t* packet, uint16_t length);

// Send an IPv4 packet (payload = ICMP later)
void ipv4_send(uint32_t dst_ip, uint8_t protocol,
               uint8_t* payload, uint16_t payload_len);