#ifndef NETWORK_H
#define NETWORK_H

#include "stdint.h"
#include "stdbool.h"

#define MAX_CONNECTIONS 10
#define PACKET_SIZE 1024
#define IP_ADDR_LEN 16

typedef struct {
    char ip[IP_ADDR_LEN];
    uint16_t port;
    bool connected;
} connection_t;

typedef struct {
    char data[PACKET_SIZE];
    uint32_t size;
    char source_ip[IP_ADDR_LEN];
    char dest_ip[IP_ADDR_LEN];
    uint16_t source_port;
    uint16_t dest_port;
} packet_t;

// Network stack
void network_init();
bool network_connect(const char* ip, uint16_t port);
void network_disconnect();
bool network_send(const char* data, uint32_t size);
int network_receive(char* buffer, uint32_t max_size);
void network_status();
bool network_ping(const char* ip);

// Simulated web
void web_request(const char* url, char* response, uint32_t max_size);

#endif
