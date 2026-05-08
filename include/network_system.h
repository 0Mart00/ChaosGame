#ifndef NETWORK_SYSTEM_H
#define NETWORK_SYSTEM_H

#include <enet/enet.h>
#include <stdbool.h>

typedef struct {
    ENetHost *host;
    ENetPeer *peer; // Csak kliens esetén használt, a szerverhez való kapcsolódáshoz
    bool isServer;
    bool connected;
} NetworkContext;

// Függvények
bool InitNetwork(void);
bool StartServer(int port, int maxClients);
bool StartClient(const char* ip, int port);
void UpdateNetwork(void);
void CleanupNetwork(void);

#endif
