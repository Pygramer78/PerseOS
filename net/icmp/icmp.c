#include "icmp.h"


// ICMP header
typedef struct {
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
} __attribute__((packed)) icmp_header_t;

// --- Checksum (same idea as IPv4) ---
static uint16_t icmp_checksum(void* data, uint16_t len) {
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

// --- Process incoming ICMP packet ---
void icmp_process_packet(uint8_t* data,
                          uint16_t length,
                          uint32_t src_ip) {

    if (length < sizeof(icmp_header_t)) return;

    icmp_header_t* hdr = (icmp_header_t*)data;

    // ICMP Echo Request
    if (hdr->type == 8) {
        // Turn it into Echo Reply
        hdr->type = 0;
        hdr->checksum = 0;
        hdr->checksum = icmp_checksum(data, length);

        // Send back to sender
        ipv4_send(src_ip, 1, data, length);
    }
}

// --- Send ICMP Echo Request ---
void icmp_send_echo(uint32_t dst_ip) {
    uint8_t packet[64];
    icmp_header_t* hdr = (icmp_header_t*)packet;

    hdr->type = 8; // Echo Request
    hdr->code = 0;
    hdr->id = 1;
    hdr->sequence = 1;
    hdr->checksum = 0;

    hdr->checksum = icmp_checksum(packet, sizeof(icmp_header_t));

    ipv4_send(dst_ip, 1, packet, sizeof(icmp_header_t));
}
