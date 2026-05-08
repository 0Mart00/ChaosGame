#include "network_system.h"
#include <stdio.h>

static NetworkContext net = {0};

bool InitNetwork(void) {
    if (enet_initialize() != 0) {
        fprintf(stderr, "Hiba az ENet inicializálásakor!\n");
        return false;
    }
    return true;
}

bool StartServer(int port, int maxClients) {
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    net.host = enet_host_create(&address, maxClients, 2, 0, 0);
    if (net.host == NULL) return false;

    net.isServer = true;
    net.connected = true;
    printf("Szerver elindítva a %d porton...\n", port);
    return true;
}

bool StartClient(const char* ip, int port) {
    net.host = enet_host_create(NULL, 1, 2, 0, 0);
    if (net.host == NULL) return false;

    ENetAddress address;
    enet_address_set_host(&address, ip);
    address.port = port;

    net.peer = enet_host_connect(net.host, &address, 2, 0);
    if (net.peer == NULL) return false;

    net.isServer = false;
    printf("Csatlakozás a %s:%d címhez...\n", ip, port);
    return true;
}

void UpdateNetwork(void) {
    if (!net.host) return;

    ENetEvent event;
    while (enet_host_service(net.host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("Kapcsolat létrejött: %x:%u\n", event.peer->address.host, event.peer->address.port);
                net.connected = true;
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                // Itt kezeljük a bejövő adatcsomagokat (pl. egységpozíciók)
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("Kapcsolat bontva.\n");
                net.connected = false;
                break;
            default: break;
        }
    }
}

void CleanupNetwork(void) {
    if (net.host) enet_host_destroy(net.host);
    enet_deinitialize();
}
