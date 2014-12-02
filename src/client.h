//#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <unistd.h>
#include <time.h>
//~ #include <pthread.h>
//~ #include <boost/thread/thread.hpp>

#define HOST "localhost"
#define PORT 0xCC00

struct cc_client_data {
    
};

class CC_Client {
  protected:
    ENetAddress address;
    ENetHost *client;
    ENetPeer *peer;
    ENetPacket *packet;
    //~ boost::thread conn_t;
    //~ int conn_t_ret;
    int queue;
    char *msg;
    
  public:
    ENetEvent evt;
    bool connected;
  
    CC_Client();
    ~CC_Client();
    void send();
    void queueMsg(char *msg, size_t len);
    void connect(const char *host, uint16_t port);
    void loop();
    void disconnect();
};
