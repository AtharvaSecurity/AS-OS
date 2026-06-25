#include "network.h"
#include "terminal.h"
#include "memory.h"

static connection_t connections[MAX_CONNECTIONS];
static int connection_count = 0;
static bool network_up = false;

void network_init() {
    for(int i = 0; i < MAX_CONNECTIONS; i++) {
        connections[i].connected = false;
    }
    connection_count = 0;
    network_up = false; // Will be true when real hardware detected
}

bool network_connect(const char* ip, uint16_t port) {
    if(connection_count >= MAX_CONNECTIONS) return false;
    
    // Simulate connection
    for(int i = 0; i < IP_ADDR_LEN-1 && ip[i]; i++) {
        connections[connection_count].ip[i] = ip[i];
        connections[connection_count].ip[i+1] = '\0';
    }
    connections[connection_count].port = port;
    connections[connection_count].connected = true;
    connection_count++;
    network_up = true;
    return true;
}

void network_disconnect() {
    if(connection_count > 0) {
        connection_count--;
        connections[connection_count].connected = false;
    }
    if(connection_count == 0) network_up = false;
}

bool network_send(const char* data, uint32_t size) {
    if(!network_up) return false;
    // Simulated send - always succeeds
    return true;
}

int network_receive(char* buffer, uint32_t max_size) {
    if(!network_up) return -1;
    // Simulated receive - returns sample data
    const char* sample = "HTTP/1.1 200 OK\r\nContent: Hello from AS OS Network!\r\n";
    int len = 0;
    while(sample[len] && len < (int)max_size - 1) {
        buffer[len] = sample[len];
        len++;
    }
    buffer[len] = '\0';
    return len;
}

void network_status() {
    terminal_writestring("\n  Network Status:\n  ===============\n");
    terminal_writestring(network_up ? "  Status: CONNECTED\n" : "  Status: DISCONNECTED\n");
    if(network_up) {
        terminal_writestring("  IP: 192.168.1.100\n");
        terminal_writestring("  Gateway: 192.168.1.1\n");
        terminal_writestring("  DNS: 8.8.8.8\n");
    }
    terminal_writestring("  Interface: eth0 (Simulated)\n");
    terminal_writestring("  Real networking: Phase 8 Framework Ready\n\n");
}

bool network_ping(const char* ip) {
    terminal_writestring("\n  PING ");
    terminal_writestring(ip);
    terminal_writestring(": ");
    // Simulate ping
    for(int i = 0; i < 4; i++) {
        terminal_writestring("64 bytes: time=");
        char buf[8];
        int t = 10 + (i * 5);
        buf[0] = '0' + (t/10);
        buf[1] = '0' + (t%10);
        buf[2] = 'm';
        buf[3] = 's';
        buf[4] = '\0';
        terminal_writestring(buf);
        terminal_writestring("  ");
    }
    terminal_writestring("\n  --- ping statistics ---\n");
    terminal_writestring("  4 packets transmitted, 4 received, 0% loss\n\n");
    return true;
}

void web_request(const char* url, char* response, uint32_t max_size) {
    const char* html = "<html><body><h1>AS OS Web Browser</h1><p>Welcome to ";
    int len = 0;
    while(html[len] && len < (int)max_size - 1) {
        response[len] = html[len];
        len++;
    }
    int url_len = 0;
    while(url[url_len] && len < (int)max_size - 1) {
        response[len++] = url[url_len++];
    }
    const char* end = "</p><p>This is a simulated web page.</p></body></html>";
    int e = 0;
    while(end[e] && len < (int)max_size - 1) {
        response[len++] = end[e++];
    }
    response[len] = '\0';
}
