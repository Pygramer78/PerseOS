#include "arp.h"
#include "rtl8139.h"

// --- ARP table entry ---
typedef struct {
    uint32_t ip;     // IPv4 address
    uint8_t mac[6];  // MAC address
} arp_entry_t;

static arp_entry_t arp_table[ARP_TABLE_SIZE];

// Our IP and MAC (set elsewhere)
extern uint32_t local_ip;
extern uint8_t local_mac[6];

// --- Helper: add/update ARP entry ---
void arp_update(uint32_t ip, uint8_t mac[6]) {
    for (int i = 0; i < ARP_TABLE_SIZE; i++) {
        if (arp_table[i].ip == ip || arp_table[i].ip == 0) {
            arp_table[i].ip = ip;
            memcpy(arp_table[i].mac, mac, 6);
            return;
        }
    }
}

// --- Initialize ARP table ---
void arp_init(void) {
    memset(arp_table, 0, sizeof(arp_table));
}

// --- Resolve IP to MAC ---
uint8_t arp_resolve(uint32_t ip, uint8_t mac[6]) {
    for (int i = 0; i < ARP_TABLE_SIZE; i++) {
        if (arp_table[i].ip == ip) {
            memcpy(mac, arp_table[i].mac, 6);
            return 1;
        }
    }
    // Not found; optionally send ARP request
    return 0;
}

// --- Send ARP request ---
void arp_request(uint32_t ip) {
    uint8_t packet[42]; // Ethernet + ARP

    // Ethernet header
    memset(packet, 0xFF, 6);           // broadcast
    memcpy(packet+6, local_mac, 6);    // source MAC
    packet[12] = 0x08; packet[13] = 0x06; // EtherType = ARP

    // ARP header
    packet[14] = 0x00; packet[15] = 0x01; // hardware type = Ethernet
    packet[16] = 0x08; packet[17] = 0x00; // protocol type = IPv4
    packet[18] = 6;                       // hardware size
    packet[19] = 4;                       // protocol size
    packet[20] = 0x00; packet[21] = 0x01; // opcode = request

    memcpy(packet+22, local_mac, 6);       // sender MAC
    packet[28] = (local_ip >> 24) & 0xFF;
    packet[29] = (local_ip >> 16) & 0xFF;
    packet[30] = (local_ip >> 8) & 0xFF;
    packet[31] = local_ip & 0xFF;          // sender IP

    memset(packet+32, 0x00, 6);            // target MAC = unknown
    packet[38] = (ip >> 24) & 0xFF;
    packet[39] = (ip >> 16) & 0xFF;
    packet[40] = (ip >> 8) & 0xFF;
    packet[41] = ip & 0xFF;                // target IP

    rtl8139_send(packet, 42);
}

// --- Process incoming ARP packet ---
void arp_process_packet(uint8_t* packet, uint16_t length) {
    if (length < 28) return; // minimal ARP size

    uint16_t opcode = (packet[20] << 8) | packet[21];
    uint8_t* sender_mac = packet+22;
    uint32_t sender_ip = (packet[28]<<24) | (packet[29]<<16) | (packet[30]<<8) | packet[31];

    // Update ARP table
    arp_update(sender_ip, sender_mac);

    // If request is for us, send reply
    if (opcode == 1) { // ARP request
        uint32_t target_ip = (packet[38]<<24)|(packet[39]<<16)|(packet[40]<<8)|packet[41];
        if (target_ip == local_ip) {
            uint8_t reply[42];
            memcpy(reply, packet, 42);
            // Ethernet header
            memcpy(reply, sender_mac, 6);
            memcpy(reply+6, local_mac, 6);
            // ARP opcode = reply
            reply[20] = 0x00;
            reply[21] = 0x02;
            // Sender MAC/IP
            memcpy(reply+22, local_mac, 6);
            reply[28] = (local_ip >> 24) & 0xFF;
            reply[29] = (local_ip >> 16) & 0xFF;
            reply[30] = (local_ip >> 8) & 0xFF;
            reply[31] = local_ip & 0xFF;
            // Target MAC/IP
            memcpy(reply+32, sender_mac, 6);
            reply[38] = (sender_ip >> 24) & 0xFF;
            reply[39] = (sender_ip >> 16) & 0xFF;
            reply[40] = (sender_ip >> 8) & 0xFF;
            reply[41] = sender_ip & 0xFF;

            rtl8139_send(reply, 42);
        }
    }
}
