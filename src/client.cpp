#include "client.h"

CC_Client::CC_Client() {
    connected = false;
    queue = 0;
    if (enet_initialize()) {
         fprintf(stderr, "An error occurred while initializing ENet.\n");
    }
    
    client = enet_host_create(NULL, 1, 2, 57600/8, 14400/8);
    
    if(client == NULL) {
        fprintf(stderr, "Could not create ENet client\n");
        exit(EXIT_FAILURE);
    } else {
        fprintf(stderr, "Started client\n");
    }
}

CC_Client::~CC_Client() {
    fprintf(stderr, "Exiting client now\n");
    this->disconnect();
    enet_host_destroy(client);
    enet_deinitialize();
}

//~ void *CC_Client::loop(void *context) {
    //~ while (connected) {
        //~ if (queue) {
            //~ me->send();
        //~ }
        //~ while (enet_host_service(client, &(me->evt), 1000) > 0) {
            //~ switch (evt.type) {
              //~ case ENET_EVENT_TYPE_RECEIVE:
                //~ printf("%s\n", (char*)evt.packet->data);
                //~ break;
              //~ case ENET_EVENT_TYPE_DISCONNECT:
                //~ me->connected = false;
                //~ printf("Disconnected from %s:%u\n", HOST, PORT);
                //~ break;
              //~ default:
                //~ break;
            //~ }
        //~ }
    //~ }
//~ }
void CC_Client::loop() {
    if (connected) {
        if (queue) {
            this->send();
        }
        while (enet_host_service(client, &evt, 1000) > 0) {
            switch (evt.type) {
              case ENET_EVENT_TYPE_RECEIVE:
                printf("(server) %s\n", (char*)evt.packet->data);
                break;
              case ENET_EVENT_TYPE_DISCONNECT:
                connected = false;
                printf("Disconnected from %s:%u\n", HOST, PORT);
                break;
              default:
                break;
            }
        }
    }
}

void CC_Client::send() {
    /* Create a reliable packet of size 7 containing "packet\0" */
    //ENetPacket *
    packet = enet_packet_create(msg, strlen (msg) + 1,
                                ENET_PACKET_FLAG_RELIABLE);
    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by */
    /* enet_host_broadcast (host, 0, packet); */
    enet_peer_send(peer, 0, packet);
    /* One could just use enet_host_service() instead. */
    enet_host_flush (client);
    //~ enet_packet_destroy(packet);
    queue--;
}

void CC_Client::connect(const char *host=HOST, uint16_t port=PORT) {
    if (connected) {
        printf("Connected already.\n");
        return;
    }
    
    enet_address_set_host(&address, host);
    address.port = port;
    
    peer = enet_host_connect(client, &address, 2, 0);
    
    if (peer == NULL) {
        fprintf(stderr, "No available peers to connect\n");
        exit(EXIT_FAILURE);
    }
    
    if (enet_host_service(client, &evt, 5000) > 0
        && evt.type == ENET_EVENT_TYPE_CONNECT) {
        printf("Connected to server %s:%u\n", HOST, PORT);
        connected = true;
    } else {
        enet_peer_reset(peer);
        printf("Connection failed to %s:%u\n", HOST, PORT);
    }
    
    //~ conn_t_ret = pthread_create(&conn_t, NULL, CC_Client::loop, (void*)this);
}

void CC_Client::disconnect() {
    enet_peer_disconnect(peer, 0);
    while (enet_host_service(client, &evt, 3000)) {
        switch (evt.type) {
          case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(evt.packet);
          case ENET_EVENT_TYPE_DISCONNECT:
            printf("Disconnected from %s:%u\n", HOST, PORT);
            break;
        }
    }
    enet_peer_reset(peer);
    connected = false;
}

void CC_Client::queueMsg(char *wut, size_t len) {
    msg = strdup(wut);
    queue++;
}

int main(int argc, char **argv, char **envp) {
    CC_Client *cli = new CC_Client();
    cli->connect();
    
    char *buffer = (char*)malloc(256*sizeof(char));
    
    while (true) {
        cli->loop();
        memset(buffer, 0, 256);
        scanf("%s", buffer);
        if (strlen(buffer)) {
            if (buffer[0] != '/') {
                cli->queueMsg(buffer, strlen(buffer));
            } else {
                if (!strncmp(buffer, "/connect", 8)) {
                    cli->connect();
                } if (!strncmp(buffer, "/disconnect", 11)) {
                    cli->disconnect();
                } else if (!strncmp(buffer, "/help", 5)) {
                    printf(
                        "Available commands:\n"
                        "/bc MESSAGE            broadcast MESSAGE\n"
                        "/help                  show this message\n"
                        "/connect HOST PORT     connect to server\n"
                        "/disconnect            disconnect from server\n"
                        "/quit                  gotta go fast\n");
                } else if (!strncmp(buffer, "/quit", 5)) {
                    break;
                }
            }
        }
    }
    
    free(buffer);
    delete cli;
    
    return 0;
}
