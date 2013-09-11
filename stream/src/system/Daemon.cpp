/*
 * Daemon.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: maslyukov
 */

#include <system/Daemon.h>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

#define PATH "/usr/tmp/videofix/pid"

using namespace std;

//const string Daemon::filename = "/usr/tmp/videofix/pid";
//const string Daemon::filename = "/usr/tmp/videofix/pid";

//------------------------------------------------------------------------------
Daemon::Daemon() {
}

//------------------------------------------------------------------------------
Daemon::~Daemon() {
}

//------------------------------------------------------------------------------
void Daemon::run() {
    while (1) {
        sleep(1);
    }
}

//------------------------------------------------------------------------------
std::string Daemon::filename() {
    return PATH;
}

//------------------------------------------------------------------------------
int Daemon::savePid() {
    ofstream file;
    file.open(filename().c_str(), ios_base::out);
    if (file.is_open()) {
        file << getpid();
        file.close();
        return 0;
    }
    throw runtime_error("Can't save PID");
    return -1;
}

//------------------------------------------------------------------------------
int Daemon::loadPid() {
    ifstream file;
    int pid = -1;
    file.open(filename().c_str(), ios_base::in);
    if (file.is_open()) {
        file >> pid;
        file.close();
    }
    return pid;
}

//------------------------------------------------------------------------------
int Daemon::daemonize() {
    pid_t pid, sid;	//Our process ID and Session ID
    pid = fork(); //Fork off the parent process
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    // If we got a good PID, then we can exit the parent process.
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);		// Change the file mode mask
    sid = setsid(); // Create a new SID for the child process
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
    savePid();	//save pid to the file
    // Close out the standard file descriptors
//    close(STDIN_FILENO);
//    close(STDOUT_FILENO);
//    close(STDERR_FILENO);

    try {
        run();	//Run daemon
    } catch (...) {
        remove(filename().c_str());
        throw;
    }
    exit(EXIT_SUCCESS);
    return 0;
}

//------------------------------------------------------------------------------
void Daemon::start() {
    if (loadPid() < 0) {
        cout << "Demon has run\n";
        daemonize();
    } else {
        cout << "Demon is running\n";
    }
}

//------------------------------------------------------------------------------
void Daemon::stop() {
    int PID = loadPid();
    if (PID > 0) {
        kill(PID, SIGTERM);
        remove(filename().c_str());
        cout << "Demon has stoped\n";
    } else {
        cout << "Nothing to kill\n";
    }
}
