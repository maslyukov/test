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
	std::string m_file_path;
private:
    int savePid();
    int loadPid();
    int daemonize();
    void onCreate();
    void onDestroy();
protected:
    const std::string getCurrentDir();
    const virtual std::string getFileName();
    virtual void run();
public:
    virtual void stop();
    virtual void start();
    Daemon();
    virtual ~Daemon();
};

#endif /* DAEMON_H_ */
