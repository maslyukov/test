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

//==============================================================================
//==============================================================================
// Sample connection classs
//==============================================================================
//==============================================================================
class Connection {
    int fd;
    Connection() = delete;
public:
    enum Type {
        client, server,
    };
    Connection(int fd, struct sockaddr_in* addr, Type type);
    virtual ~Connection();

    /**
     * Write to connection
     * @param data	- pointer to array
     * @param size	- size of buffer
     * @return		- size of written bytes
     */
    int write(unsigned char* data, int size);

    /**
     * Read from connection
     * @param data	- pointer to array
     * @param size	- size of incomming buffer
     * @return		- size of read bytes
     */
    int read(unsigned char* data, int size);
};

//==============================================================================
//==============================================================================
// Socket Internet Stream Server
//==============================================================================
//==============================================================================
class SISS {
    enum Settings {
        address = INADDR_ANY,
        type = SOCK_STREAM,
        family = AF_INET,
        maxConnections = 5,
        protocol = IPPROTO_IP,

    };
    int socketFd;
    struct sockaddr_in addr;

    SISS() = delete;
public:
    void close();
    SISS(unsigned int ip, int port);
    virtual ~SISS();

    /**
     * Receive 	- connection from client
     * @return	- Connection
     */
    std::unique_ptr<Connection> connect();
};

//==============================================================================
//==============================================================================
// Socket Internet Stream Client
//==============================================================================
//==============================================================================
class SISC {
    enum Settings {
        type = SOCK_STREAM, family = AF_INET, protocol = IPPROTO_IP,
    };
    SISC() = delete; // : addr{0}, socketFd{0} {}

    static std::unique_ptr<Connection> make(unsigned int ip, int port);

public:

    /**
     * Connect to server
     * @param ip	- ip of server
     * @param port  - port to connect
     * @return		- Connection
     */
    static std::unique_ptr<Connection> connect(unsigned int ip, int port);

    /**
     * Connect to server
     * @param ip	- ip of server
     * @param port  - port to connect
     * @return		- Connection
     */
    static std::unique_ptr<Connection> connect(std::string ip, int port);
};

#endif /* SOCKET_H_ */
