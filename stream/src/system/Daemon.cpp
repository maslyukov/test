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

using namespace std;

//------------------------------------------------------------------------------
const std::string Daemon::getFileName() {
    return "capture.pid";//getCurrentDir().append("capture.pid");
}

//------------------------------------------------------------------------------
const std::string Daemon::getCurrentDir() {
    const int size = 4096;
    char* dir_char = new char[size];
    getcwd(dir_char, size);
    string dir_str;
    dir_str.append(dir_char);
    dir_str.append("/");
    free(dir_char);
    return dir_str;
}

//------------------------------------------------------------------------------
void Daemon::onCreate() {
    m_file_path.append(getCurrentDir().c_str());
    m_file_path.append(getFileName().c_str());
    //m_file_path = "/data/local/tmp/pid";
}

//------------------------------------------------------------------------------
void Daemon::onDestroy() {
}

//------------------------------------------------------------------------------
Daemon::Daemon() {
    onCreate();
}

//------------------------------------------------------------------------------
Daemon::~Daemon() {
    onCreate();
}

//------------------------------------------------------------------------------
void Daemon::run() {
    while (1) {
        sleep(1);
    }
}

//------------------------------------------------------------------------------
int Daemon::savePid() {
    ofstream file;
    file.open(m_file_path.c_str(), ios_base::out);
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
    file.open(m_file_path.c_str(), ios_base::in);
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
        remove(m_file_path.c_str());
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
        remove(m_file_path.c_str());
        cout << "Demon has stoped\n";
    } else {
        cout << "Nothing to kill\n";
    }
}
