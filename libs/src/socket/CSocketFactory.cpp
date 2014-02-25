/*
 * CSocketFactory.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: maslyukov
 */

#include <iostream>
#include <exception/exception.h>
#include <socket/CSocketFactory.h>
#include <log/logging.h>

using std::string;

//------------------------------------------------------------------------------
std::unique_ptr<CSocketUDP> CSocketFactory::makeUdpSocket(
        const std::string& host_name, const std::string& port) {
    struct addrinfo hints;
    struct addrinfo* servinfo;
    int rv;
    LOGD("Create socket at \"%s:%s\"",
            host_name.empty() ? "localhost" : host_name.c_str(), port.c_str());
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    if ((rv = getaddrinfo(host_name.empty() ? nullptr : host_name.c_str(),
            port.c_str(), &hints, &servinfo)) != 0) {
        throw Exception::CRuntime(FFL_MACRO, std::string("getaddrinfo: ") +
                std::string(gai_strerror(rv)));
    }
    std::unique_ptr<CSocketUDP> socket(nullptr);
    socket.reset(new CSocketUDP(servinfo));
    freeaddrinfo(servinfo);
    LOGD("UDP socket has been created");
    return socket;
}
//------------------------------------------------------------------------------
std::unique_ptr<CSocketTCP> CSocketFactory::makeTcpSocket(
        const std::string& host_name, const std::string& port) {
    struct addrinfo hints;
    struct addrinfo* servinfo;
    int rv;
    LOGD("Create socket at \"%s:%s\"",
            host_name.empty() ? "localhost" : host_name.c_str(), port.c_str());
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    if ((rv = getaddrinfo(host_name.empty() ? nullptr : host_name.c_str(),
            port.c_str(), &hints, &servinfo)) != 0) {
        throw Exception::CRuntime(FFL_MACRO, std::string("getaddrinfo: ") +
                std::string(gai_strerror(rv)));
    }
    std::unique_ptr<CSocketTCP> socket;
    socket.reset(new CSocketTCP(servinfo));
    freeaddrinfo(servinfo);
    LOGD("TCP socket has been created");
    return socket;
}
