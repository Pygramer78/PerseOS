#pragma once
#include <stdint.h>
#include <string.h>
#define ARP_TABLE_SIZE 16

// Initialize ARP module
void arp_init(void);

// Resolve an IPv4 address to a MAC address
// returns 1 if mac is found and 0 if not in table
uint8_t arp_resolve(uint32_t ip, uint32_t mac[6]);

// Process an incoming arp packet
void arp_process_packet(uint8_t packet, uint16_t length);

// Send an arp request for an ip
void arp_request(uint32_t ip);

// Update Arp table manually
void arp_update(uint32_t ip, uint8_t mac[6]);
