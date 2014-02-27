/*
 * Socket.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: maslyukov
 */

#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <arpa/inet.h>
#include <log/logging.h>
#include <exception/exception.h>
#include <socket/CSocketFactory.h>

using namespace std;


//==============================================================================
//==============================================================================
// CSocketUDP class
//==============================================================================
//==============================================================================
ISocket::ISocket(tSocket new_fd) {
    struct sockaddr addr;
    socklen_t len = sizeof(addr);

    if (getpeername(new_fd, &addr, &len) == -1) {
        ::close(new_fd);
        throw Exception::CRuntime(FFL_MACRO, "Can't get peer name");
    }
    m_fd = new_fd;
    m_addr = addr;
    m_addr_info.ai_addr = &m_addr;

    //todo log. Who has established connection
}

//------------------------------------------------------------------------------
ISocket::ISocket(struct addrinfo* data) {
#ifdef _WIN32
    WSADATA wsadata;
    int error = WSAStartup(0x0202, &wsadata);
    if (error) {
        throw Exception::CRuntime(FFL_MACRO,
                "Unable to initialize Windows Socket library");
    }
#endif
    struct addrinfo* p;
    for (p = data; p != NULL; p = p->ai_next) {
        m_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (m_fd < 0) {
            continue;
        }
        break;
    }
    if (p == NULL) {
        throw Exception::CRuntime(FFL_MACRO,"Error: can't create socket");
    }
    m_addr_info = *p;
    m_addr = *p->ai_addr;
    m_addr_info.ai_addr = &m_addr;
}

//------------------------------------------------------------------------------
ISocket::~ISocket() {
#ifdef _WIN32
    ::closesocket(m_fd)
    WSACleanup(); //Clean up Winsock
#else
    close(m_fd);
#endif
}

//------------------------------------------------------------------------------
void ISocket::bind() {
    int on_reuse = 1;
    if (::bind(m_fd, m_addr_info.ai_addr, m_addr_info.ai_addrlen) < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't bind the socket");
    }
    LOGD("Socket has been bound");
    if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on_reuse,
            sizeof(on_reuse)) < 0) {
        throw Exception::CRuntime(FFL_MACRO, "Can't set sockopt to reuse address: "
                + string(strerror(errno)));
    }
    LOGD("Socket has been set to reuse address");
}


