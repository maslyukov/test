/*
 * CSocketUDP.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: maslyukov
 */

#include <iostream>
#include <exception/exception.h>
#include <socket/CSocketFactory.h>
#include <log/logging.h>
#include <errno.h>

using std::string;

//==============================================================================
//==============================================================================
// CSocketUDP class
//==============================================================================
//==============================================================================
CSocketUDP::CSocketUDP(struct addrinfo* data) : ISocket(data) {
}

CSocketUDP::~CSocketUDP() {
}

//------------------------------------------------------------------------------
int CSocketUDP::write(const unsigned char* data, unsigned int size) {
    int numbytes = sendto(m_fd, data, size, 0, m_addr_info.ai_addr,
                m_addr_info.ai_addrlen);
    if (numbytes < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't write to the socket: "
                + string(strerror(errno)));
    }
    LOGD("Write %d bytes", numbytes);
    return numbytes;
}

//------------------------------------------------------------------------------
int CSocketUDP::read(unsigned char* data, unsigned int size) {
    struct sockaddr_storage their_addr;
    socklen_t addr_len = sizeof(their_addr);
    int numbytes = recvfrom(m_fd, (void*) data, size, 0,
            (struct sockaddr *) &their_addr, &addr_len);
    if (numbytes < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't read from the socket: "
                + string(strerror(errno)));
    }
    LOGD("Read %d bytes", numbytes);
    return numbytes;

}


