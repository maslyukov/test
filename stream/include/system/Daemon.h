/*
 * Daemon.h
 *
 *  Created on: Jul 5, 2013
 *      Author: maslyukov
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#include <iostream>

class Daemon {
private:
//	static const std::string filename;
    virtual void run();
    int savePid();
    int loadPid();
    int daemonize();
protected:
    virtual std::string filename();
public:
    virtual void stop();
    virtual void start();
    Daemon();
    virtual ~Daemon();
};

#endif /* DAEMON_H_ */
