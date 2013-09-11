/*
 * Socket.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: maslyukov
 */

#include <socket/Socket.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

//==============================================================================
//==============================================================================
// Sample connection classs
//==============================================================================
//==============================================================================
Connection::Connection(int fd, struct sockaddr_in* addr, Type type) :
        fd(fd) {
    socklen_t len = sizeof(addr);

    switch (type) {
    case Type::client:
        if (addr == nullptr) {
            throw invalid_argument("struct sockaddr_in is NULL");
        }
        if (connect(this->fd, (struct sockaddr *) addr, sizeof(sockaddr_in))
                < 0) {
            throw runtime_error("Can't connect to the socket");
        }
        break;
    case Type::server:
        this->fd = ::accept(this->fd, (struct sockaddr*) addr, &len);
        if (this->fd < 0) {
            throw runtime_error("Can't open incoming socket");
        }
        break;
    default:
        throw runtime_error("Invalid type of connection");
        break;
    }
	cout << "create connection\n";
}

Connection::~Connection() {
    if (close(this->fd) < 0) {
        throw runtime_error("Can't close incoming socket");
    }
	cout << "close connection\n";
}

int Connection::write(unsigned char* data, int size) {
    int bytes = ::write(this->fd, data, size);
    if (bytes < 0) {
        throw runtime_error("Can't write to the socket");
    }
    return bytes;
}

int Connection::read(unsigned char* data, int size) {
    int bytes = ::read(this->fd, data, size);
    if (bytes < 0) {
        throw runtime_error("Can't read from the socket");
    }
    return bytes;
}

//==============================================================================
//==============================================================================
// Socket Internet Stream Server
//==============================================================================
//==============================================================================
SISS::~SISS() {
    close();
}
//------------------------------------------------------------------------------
void SISS::close() {
    if (::close(socketFd) < 0)
        throw runtime_error("Can't close the socket");
}
//------------------------------------------------------------------------------
SISS::SISS(unsigned int ip, int port) {
    addr.sin_port = htons(port & 0xFFFF);
    addr.sin_family = Settings::family;
    addr.sin_addr.s_addr = Settings::address;
    socketFd = socket(Settings::family, Settings::type, Settings::protocol);
    if (socketFd < 0) {
        throw runtime_error("Can't open the socket");
    }
    if (bind(socketFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        throw runtime_error("Can't bind the socket");
    }
    if (listen(socketFd, Settings::maxConnections) < 0) {
        throw runtime_error("Can't connect to the socket");
    }
}

//------------------------------------------------------------------------------
std::unique_ptr<Connection> SISS::connect() {
    return unique_ptr<Connection>(
            new Connection(socketFd, nullptr, Connection::Type::server));
}

//==============================================================================
//==============================================================================
// Socket Internet Stream Client
//==============================================================================
//==============================================================================
std::unique_ptr<Connection> SISC::connect(unsigned int ip, int port) {
    return make(ip, port);
}

//------------------------------------------------------------------------------
std::unique_ptr<Connection> SISC::connect(std::string ip, int port) {
    //inet_aton()
    unsigned int _ip = inet_addr(ip.data());
    if (_ip == INADDR_NONE)
        throw runtime_error("IP address isn't valid");
    return make(_ip, port);
}

//------------------------------------------------------------------------------
std::unique_ptr<Connection> SISC::make(unsigned int ip, int port) {
    struct sockaddr_in addr;
    addr.sin_port = htons(port & 0xFFFF);
    addr.sin_family = Settings::family;
    addr.sin_addr.s_addr = ip;
    int fd = socket(Settings::family, Settings::type, Settings::protocol);
    if (fd < 0) {
        throw runtime_error("Can't open the socket");
    }
    return unique_ptr<Connection>(
            new Connection(fd, &addr, Connection::Type::client));
}
