#pragma once
#include <ipv4/ipv4.h>
#include <stdint.h>
#include <string.h>

// Process incoming ICMP packet (without IPv4 header)
void icmp_process_packet(uint8_t* data,
                          uint16_t length,
                          uint32_t src_ip);

// Send ICMP Echo Request (ping)
void icmp_send_echo(uint32_t dst_ip);