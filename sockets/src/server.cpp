/*
 * server.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: maslyukov
 */

#include <socket/CSocketFactory.h>
#include <system/Daemon.h>
#include <iostream>
#include <array>
#include <arpa/inet.h>
#include <log/logging.h>

using namespace std;

#define ROLE "[server]: "
int main(int argc, char *argv[]) {
    unsigned char buf[128]={0};


#define SOCK_TYPE 1


#if (SOCK_TYPE == 1)
    auto socket = CSocketFactory::makeTcpSocket("", "33333");
    socket->bind();

    socket->listen(5);
    while (1) {
        auto new_sock = socket->accept();
        new_sock->read(buf, 128);
        LOGI(ROLE"Message: %s", buf);
    }
#else
    auto socket = CSocketFactory::makeUdpSocket("127.0.0.1", "33333");
    socket->bind();
    while (1) {
        int bytes = socket->read(buf, 128);
        printf("Message: %s\n", buf);
    }
#endif


}
//
//class Service: public Daemon {
//    unique_ptr<SISS> server;
//    virtual string filename();
//public:
//    virtual void run();
//    virtual void stop();
//};
////------------------------------------------------------------------------------
//string Service::filename() {
//    return "/data/local/tmp/pid";
//}
////------------------------------------------------------------------------------
//void Service::stop() {
//    if (server.get())
//        server->close();
//    Daemon::stop();
//}
////------------------------------------------------------------------------------
//void Service::run() {
//    array<unsigned char, 256> buf;
//    int pr = 0;
//    server.reset(new SISS(0, 33333));
//    cout << "ready for data" << endl;
//    while (1) {
//        try {
//            auto con = server->connect();
//            int len = con->read(buf.data(), buf.size());
//            cout << string((const char*) buf.data(), len) << endl;
//            con->write((unsigned char*) "OK", 5);
//        } catch (runtime_error& e) {
//            cout << "Catch exception: " << e.what() << endl;
//        }
//    }
//}
//
////------------------------------------------------------------------------------
//int main(int argc, char *argv[]) {
//    if (argc != 2) {
//        cout << "Please use " << argv[0] << " (start/stop/restart)\n";
//        return -1;
//    }
//    if (string(argv[1]) == "start") {
//        Service().start();
//    } else if (string(argv[1]) == "stop") {
//        Service().stop();
//    } else if (string(argv[1]) == "restart") {
//        Service srv;
//        srv.stop();
//        srv.start();
//    } else {
//        cout << "Please use " << argv[0] << " (start/stop/restart)\n";
//    }
//    return 0;
//}
