#include "ipv4.h"

extern uint32_t local_ip;
extern uint8_t local_mac[6];

// IPv4 header (minimal)
typedef struct {
    uint8_t  ver_ihl;
    uint8_t  tos;
    uint16_t total_length;
    uint16_t id;
    uint16_t flags_fragment;
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
} __attribute__((packed)) ipv4_header_t;

// --- Simple checksum ---
static uint16_t ipv4_checksum(void* data, uint16_t len) {
    uint32_t sum = 0;
    uint16_t* ptr = data;

    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }

    if (len) sum += *(uint8_t*)ptr;

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ~sum;
}

// --- Process incoming IPv4 packet ---
void ipv4_process_packet(uint8_t* packet, uint16_t length) {
    if (length < sizeof(ipv4_header_t)) return;

    ipv4_header_t* hdr = (ipv4_header_t*)packet;

    if ((hdr->ver_ihl >> 4) != 4) return; // not IPv4
    if (hdr->dst_ip != local_ip) return;

    uint8_t ihl = (hdr->ver_ihl & 0x0F) * 4;
    uint8_t* payload = packet + ihl;
    uint16_t payload_len = length - ihl;

    // ICMP = protocol 1
    if (hdr->protocol == 1) {
        icmp_process_packet(payload, payload_len, hdr->src_ip);
    }
}

// --- Send IPv4 packet ---
void ipv4_send(uint32_t dst_ip, uint8_t protocol,
               uint8_t* payload, uint16_t payload_len) {

    uint8_t mac[6];
    if (!arp_resolve(dst_ip, mac)) {
        arp_request(dst_ip);
        return;
    }

    uint8_t packet[1500];
    ipv4_header_t* hdr = (ipv4_header_t*)(packet + 14);

    // Ethernet header
    memcpy(packet, mac, 6);
    memcpy(packet + 6, local_mac, 6);
    packet[12] = 0x08;
    packet[13] = 0x00;

    // IPv4 header
    hdr->ver_ihl = 0x45;
    hdr->tos = 0;
    hdr->total_length = sizeof(ipv4_header_t) + payload_len;
    hdr->id = 0;
    hdr->flags_fragment = 0;
    hdr->ttl = 64;
    hdr->protocol = protocol;
    hdr->src_ip = local_ip;
    hdr->dst_ip = dst_ip;
    hdr->checksum = 0;
    hdr->checksum = ipv4_checksum(hdr, sizeof(ipv4_header_t));

    memcpy(packet + 14 + sizeof(ipv4_header_t), payload, payload_len);

    rtl8139_send(packet, 14 + sizeof(ipv4_header_t) + payload_len);
}