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
#include <memory>
#include <unistd.h>
#include <arpa/inet.h>
#include <audio/Audio.h>

using namespace std;

struct StreamSettings {
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint16_t BitsPerSample;
    uint32_t nBlockAlign;
    uint32_t size;
    uint8_t data[0];
};

class Service: public Daemon {
    unique_ptr<SISS> server;
    virtual string filename();
public:
    virtual void run();
    virtual void stop();
};
//------------------------------------------------------------------------------
string Service::filename() {
    return "/data/local/tmp/pid";
}
//------------------------------------------------------------------------------
void Service::stop() {
    if (server.get())
        server->close();
    Daemon::stop();
}
//------------------------------------------------------------------------------
void Service::run() {
    array<unsigned char, 1500> buf;
    int len = 0;
    server.reset(new SISS(0, 33333));
    cout << "ready for data" << endl;
    cout << "ready for srteam1" << endl;
    unique_ptr<OpenSL::Audio> a;

//    while (len < sizeof(StreamSettings)) {
//        len = con->read(&buf.at(len), buf.size() - len);
//    }
    cout << "ready for srteam2" << endl;
//    StreamSettings sss = {2,48000,16,4,0,{}};
//    Settings s = {2,48000,16};
//    a.reset(new OpenSL::Audio(s));
//    if(!memcmp(&sss, &s, sizeof(s))) {
//        cout << "Euqal\n";
//    }
//    unsigned char* p;
//    p = (unsigned char*)&sss;
//    for (int i = 0; i < sizeof(Settings); ++i) {
//        printf("%02X ", p[i]);
//    }
//    cout << endl;
//    p = (unsigned char*)&s;
//    for (int i = 0; i < sizeof(Settings); ++i) {
//        printf("%02X ", p[i]);
//    }

    auto con = server->connect();
    try {
        int i = 0;
        int played = 0;
        while (1) {
            len = con->read(buf.begin(), buf.size());
//            for (len = 0; len < sizeof(StreamSettings); ) {
//                len = con->read(&buf.at(len), buf.size() - len);
//            }
            StreamSettings* settings =
                    reinterpret_cast<StreamSettings*>(buf.data());
            if (a.get() == nullptr) {
                cout << "s->BitsPerSample " << settings->BitsPerSample << endl;
                cout << "s->NumChannels   " << settings->NumChannels << endl;
                cout << "s->SampleRate    " << settings->SampleRate << endl;
                cout << "s->size          " << settings->size << endl;

                Settings s;
                s.BitsPerSample = settings->BitsPerSample;
                s.NumChannels = settings->NumChannels;
                s.SampleRate = settings->SampleRate;
                cout << "Reset unique_ptr" << endl;
                a.reset(new OpenSL::Audio(s));
                cout << "Set player for playing bytes "
                        << len - sizeof(StreamSettings) << endl;
                cout << "Receive bytes from socket " << len << endl;

//                a->set(i++, settings->data, len - sizeof(StreamSettings));
//                while(1);
//                cout << "Start palying" << endl;
//                a->add(settings->data, len - sizeof(StreamSettings));
//                a->play();
//                continue;
            }
            a->add(settings->data, len - sizeof(StreamSettings));
            a->play();

//            if (!played)
//                i++;
//            if (i == 64) {
//                i = 0;
//                played = 1;
//                a->play();
//            }
//            if (!a->isPlay())
//                played = 0;
//

//            cout << "index = "<< i << " size = "
//                    << len - sizeof(StreamSettings) << endl;
//            a->play();


//            if (!len)
//                continue;
//            cout << "Has read " << len << " bytes \n"
//                    << string((const char*) buf.data(), len) << endl;
        }
    } catch (runtime_error& e) {
        cout << "Catch exception: " << e.what() << endl;
    }
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    Service().run();
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
}
