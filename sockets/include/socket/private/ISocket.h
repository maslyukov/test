/*
 * Socket.h
 *
 *  Created on: Jul 17, 2013
 *      Author: maslyukov
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <memory>
#include <netdb.h>
#include <cstring>
#include <stdexcept>



#ifdef _WIN32
typedef SOCKET tSocket;
#else
typedef unsigned int tSocket;
#endif

class ISocket {
protected:
    tSocket m_fd;
    struct addrinfo m_addr_info;
    ISocket(tSocket new_fd);
    ISocket(struct addrinfo* data);

public:

    virtual ~ISocket();
    virtual void bind();
    /**
    * write to connection
    * @param data   - pointer to array
    * @param size   - size of buffer
    * @return       - size of written bytes
    */
    virtual int write(unsigned char* data, unsigned int size) = 0;

    /**
    * read from connection
    * @param data   - pointer to array
    * @param size   - size of incomming buffer
    * @return       - size of read bytes
    */
    virtual int read(unsigned char* data, unsigned int size) = 0;
};


#endif /* SOCKET_H_ */
