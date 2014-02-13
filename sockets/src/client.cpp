/*
 * client.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: maslyukov
 */

#include <socket/CSocketFactory.h>
#include <iostream>
#include <unistd.h>
#include <array>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <log/logging.h>
#include <exception>


using namespace std;
#define ROLE "[client]: "

int main(int argc, char* argv[]) {
    if (argc != 2) {
        LOGE(ROLE"Arguments must equals 2");
        return -1;
    }
    std::string message(argv[1]);

    auto socket = CSocketFactory::makeTcpSocket("127.0.0.1", "33333");
    socket->connect();
    int bytes = socket->write((unsigned char*)message.c_str(), message.size());
    LOGI(ROLE"Message: %s", message.c_str());

//    auto socket = CSocketFactory::makeUdpSocket("127.0.0.1", "33333");
//    int bytes = socket->write((unsigned char*)message.c_str(), message.size());

}
