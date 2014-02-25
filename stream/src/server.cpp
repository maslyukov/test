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
#include <fstream>
#include <memory>
#include <unistd.h>
#include <arpa/inet.h>
#include <audio/Audio.h>
#include <errno.h>

#define DEBUG_LEVEL     4
#define DEBUG_SRC_INFO
#include <log/logging.h>

//#define PATH "/sdcard/data/capture.pcm"
#define PATH "/data/local/tmp/capture.pcm"

using namespace std;
#pragma pack(push,1)
struct StreamSettings {
    uint16_t nChannels;
    uint16_t bitsPerSample;
    uint32_t sampleRate;
    uint32_t nBlockAlign;
    uint32_t size;
    uint8_t data[];
};
#pragma pack(pop)
/*
class Service: public Daemon {
    unique_ptr<SISS> server;
public:
    virtual void run();
    virtual void stop();
};
//------------------------------------------------------------------------------
void Service::stop() {
    if (server.get())
        server->close();
    Daemon::stop();
}
//------------------------------------------------------------------------------
void Service::run() {
//    array<unsigned char, 8192> buf;
    vector<unsigned char> buf(sizeof(StreamSettings));
    int len = 0;
    server.reset(new SISS(0, 33333));
    unique_ptr<OpenSL::Audio> a;
    cout << "ready for srteam" << endl;

    while(1) {
    try {
        cout << "Wait for connection" << endl;
        auto con = server->connect();
        int i = 0;
        int played = 0;
        int input_length;
        int valid_connection = 1;
        int valid_data_len = 0;
        cout << "Wait for data" << endl;
        while (valid_connection) {
            input_length = sizeof(StreamSettings);
            len = 0;
            do {
                valid_data_len = con->read(&buf.at(len), input_length - len);
                if (!valid_data_len) {
                    valid_connection = 0;
                    break;
                }
                len += valid_data_len;
                if (len == sizeof(StreamSettings)) {
                    input_length = sizeof(StreamSettings)
                        + reinterpret_cast<StreamSettings*>(buf.data())->size;
                    if (input_length != buf.size()) {
                        buf.resize(input_length);
                        //cout << "resize to " << input_length << endl;
                    }
                }
            } while (len < input_length);
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
                        << settings->size << endl;
                cout << "Receive bytes from socket " << len << endl;
            } else {
                a->add(settings->data, settings->size);
                a->play();
            }
        }
    } catch (runtime_error& e) {
//        a->stop();
        a.reset();
//        a.release();
        cout << "Catch exception: " << e.what() << endl;
    }
    cout << "reset to initial status" << endl;
    a.reset();
    }
}

void playpcm() {
    fstream capturefile(PATH, ios_base::binary|ios_base::in);
    Settings s;
    s.BitsPerSample = 16;
    s.NumChannels = 2;
    s.SampleRate = 48000;
    unique_ptr<OpenSL::Audio> a(new OpenSL::Audio(s));
    vector<uint8_t> sample;
    while(!capturefile.eof()) {
        sample.push_back(capturefile.get());
    }
    a->add(sample.data(), sample.size());
    a->play();
    while(1);
}

*/
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    vector<unsigned char> buf(sizeof(StreamSettings));
    int len = 0;
    auto server = CSocketFactory::makeUdpSocket("", "33333");
    unique_ptr<OpenSL::Audio> a;

    server->bind();

    LOGI("Waiting for data");
    server->read(&buf.at(0), 11);
    int input_length;
    while (1) {
        try {
            int valid_data_len = 0;
            input_length = sizeof(StreamSettings);
            len = 0;
            do {
                valid_data_len = server->read(&buf.at(len), input_length - len);
//                if (!valid_data_len) {
//                    break;
//                }
                len += valid_data_len;
                if (len == sizeof(StreamSettings)) {
                    input_length = sizeof(StreamSettings)
                            + reinterpret_cast<StreamSettings*>(buf.data())->size;
                    if (input_length != buf.size()) {
                        buf.resize(input_length);
                        LOGD("Resize incoming buffer to %d bytes", input_length);
                    }
                }
            } while (len < input_length);

            StreamSettings* settings =
                    reinterpret_cast<StreamSettings*>(buf.data());
            if (a.get() == nullptr) {
                LOGI("%d bps", settings->bitsPerSample);
                LOGI("%d channels", settings->nChannels);
                LOGI("%d samples", settings->sampleRate);
                LOGI("%d size", settings->size);

                Settings s;
                s.BitsPerSample = settings->bitsPerSample;
                s.NumChannels = settings->nChannels;
                s.SampleRate = settings->sampleRate;
                a.reset(new OpenSL::Audio(s));

                LOGD("Audio player has been set");
            } else {
                a->add(settings->data, settings->size);
                a->play();
            }
        } catch (exception& e) {
//        a->stop();
            a.reset();
//        a.release();
            LOGE("Catch exception: %s", e.what());
        }
//    cout << "reset to initial status" << endl;
//    a.reset();
    }













//    Service().run();
//    try {
//        unique_ptr<SISS> server;
//        server.reset(new SISS(0, 33333));
//        server->connect();
//    } catch (runtime_error& e) {
//        cout << "Catch exception: " << e.what() << endl;
//        cout << "Error: " << errno << endl;
//    }
//    while(1);

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
    return 0;
}
