/*
 * CSocketFactory.h
 *
 *  Created on: Feb 10, 2014
 *      Author: maslyukov
 */

#ifndef CSOCKETFACTORY_H_
#define CSOCKETFACTORY_H_

#include "private/CSocketTCP.h"
#include "private/CSocketUDP.h"

class CSocketFactory {
    CSocketFactory() = delete;
    ~CSocketFactory() = delete;
public:
    static std::unique_ptr<CSocketTCP> makeTcpSocket(
            const std::string& host_name, const std::string& port);
    static std::unique_ptr<CSocketUDP> makeUdpSocket(
            const std::string& host_name, const std::string& port);
};


#endif /* CSOCKETFACTORY_H_ */
