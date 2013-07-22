/*
 * client.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: maslyukov
 */

#include <socket/Socket.h>
#include <iostream>
#include <unistd.h>
#include <array>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]) {
    string message;
    array<unsigned char, 256> buf;
    buf.fill(0);

    if (argc != 2) {
        cout << "Please use " << argv[0] << " IP\n";
        exit(1);
    }
    string str(argv[1]); // = "127.0.0.1";

    cout << "Ready for incoming data\n";
    cout << "=======================\n";
    try {
        while (1) {
            unique_ptr<Connection> con(SISC::connect(str, 33333));
            cout << "> ";
            std::getline(cin, message);
            if (message.empty())
                continue;
            con->write((unsigned char*) message.data(), message.size());
            con->read(buf.data(), buf.size());
            cout << buf.data() << endl;
        }
    } catch (exception& e) {
        cout << "catch exception " << e.what() << endl;
        throw;
    }
}
