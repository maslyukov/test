/*
 * server.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: maslyukov
 */

#include <socket/Socket.h>
#include <system/Daemon.h>
#include <iostream>
#include <array>
#include <arpa/inet.h>

using namespace std;

class Service: public Daemon {
    virtual string filename();
public:
    virtual void run();
};
//------------------------------------------------------------------------------
string Service::filename() {
    return "/data/local/tmp/pid";
}

//------------------------------------------------------------------------------
void Service::run() {
    array<unsigned char, 256> buf;
    int pr = 0;
    SISS server(0, 33333);
    cout << "ready for data" << endl;
    while (1) {
        try {
            auto con = server.connect();
            int len = con->read(buf.data(), buf.size());
            cout << string((const char*) buf.data(), len) << endl;
            con->write((unsigned char*) "OK", 5);
        } catch (runtime_error& e) {
            cout << "Catch exception: " << e.what() << endl;
        }
    }
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Please use " << argv[0] << " (start/stop/restart)\n";
        return -1;
    }
    if (string(argv[1]) == "start") {
        Service().start();
    } else if (string(argv[1]) == "stop") {
        Service().stop();
    } else if (string(argv[1]) == "restart") {
        Service srv;
        srv.stop();
        srv.start();
    } else {
        cout << "Please use " << argv[0] << " (start/stop/restart)\n";
    }
    return 0;
}
