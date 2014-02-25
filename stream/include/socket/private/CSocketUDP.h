/*
 * CSocketUDP.h
 *
 *  Created on: Feb 10, 2014
 *      Author: maslyukov
 */

#ifndef CSOCKETUDP_H_
#define CSOCKETUDP_H_

#include "ISocket.h"

class CSocketUDP : public ISocket {
public:
    CSocketUDP(struct addrinfo* data);
    ~CSocketUDP();

    /**
    * write to connection
    * @param data   - pointer to array
    * @param size   - size of buffer
    * @return       - size of written bytes
    */
    virtual int write(const unsigned char* data, unsigned int size);

    /**
    * read from connection
    * @param data   - pointer to array
    * @param size   - size of incomming buffer
    * @return       - size of read bytes
    */
    virtual int read(unsigned char* data, unsigned int size);
};


#endif /* CSOCKETUDP_H_ */
