/*
 * CSocketTCP.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: maslyukov
 */

#include <iostream>
#include <exception/exception.h>
#include <socket/CSocketFactory.h>
#include <log/logging.h>

using std::string;

//==============================================================================
//==============================================================================
// CSocketTCP class
//==============================================================================
//==============================================================================
CSocketTCP::CSocketTCP(struct addrinfo* data) : ISocket(data) {
}

//------------------------------------------------------------------------------
CSocketTCP::~CSocketTCP() {
}

//------------------------------------------------------------------------------
CSocketTCP::CSocketTCP(tSocket new_fd) : ISocket(new_fd) {

}

//------------------------------------------------------------------------------
int CSocketTCP::write(const unsigned char* data, unsigned int size) {
    int numbytes = ::send(m_fd, (const char*)data, size, 0);
    if (numbytes < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't write to the socket: "
                + string(strerror(errno)));
    }
    LOGD("Write %d bytes", numbytes);
    return numbytes;
}

//------------------------------------------------------------------------------
int CSocketTCP::read(unsigned char* data, unsigned int size) {
    int numbytes = ::recv(m_fd, (char*)data, size, 0);
    if (numbytes < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't read from the socket: "
                + string(strerror(errno)));
    }
    LOGD("Read %d bytes", numbytes);
    return numbytes;
}

//------------------------------------------------------------------------------
std::unique_ptr<ISocket> CSocketTCP::accept() {
    unsigned int len = sizeof (m_addr_info.ai_addrlen);
    int new_fd = ::accept(m_fd, (struct sockaddr*)&m_addr_info.ai_addrlen, &len);
    if (new_fd < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't open incoming socket: "
                + string(strerror(errno)));
    }
    LOGD("Socket has accepted connection");
    return std::unique_ptr<ISocket>(new CSocketTCP(new_fd));
}

//------------------------------------------------------------------------------
void CSocketTCP::listen(int maxConnections) {
    if (::listen(m_fd, maxConnections) < 0) {
        throw Exception::CRuntime(FFL_MACRO,
                "Can't connect to the socket: " + string(strerror(errno)));
    }
    LOGD("Socket is listening max to %d connections", maxConnections);
}
//------------------------------------------------------------------------------
void CSocketTCP::connect() {
    if (::connect(m_fd, (struct sockaddr*)m_addr_info.ai_addr,
            sizeof(struct sockaddr)) < 0) {
        throw Exception::CRuntime(FFL_MACRO,
                "Can't connect to the socket: " + string(strerror(errno)));
    }
    LOGD("Socket has been connected");
}
