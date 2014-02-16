/*
 * CSocketTCP.h
 *
 *  Created on: Feb 10, 2014
 *      Author: maslyukov
 */

#ifndef CSOCKETTCP_H_
#define CSOCKETTCP_H_

#include "ISocket.h"

class CSocketTCP : public ISocket {
    CSocketTCP(tSocket new_fd);
public:
    CSocketTCP(struct addrinfo* data);
    ~CSocketTCP();
    /**
    * write to connection
    * @param data   - pointer to array
    * @param size   - size of buffer
    * @return       - size of written bytes
    */
    virtual int write(const unsigned char*, unsigned int size);

    /**
    * read from connection
    * @param data   - pointer to array
    * @param size   - size of incoming buffer
    * @return       - size of read bytes
    */
    virtual int read(unsigned char* data, unsigned int size);
    std::unique_ptr<ISocket> accept();
    void listen(int maxConnections);
    void connect();
};



#endif /* CSOCKETTCP_H_ */
