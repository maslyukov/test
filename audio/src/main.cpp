/*
 * main.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: maslyukov
 */
#include <audio/Audio.h>
#include <vector>
#include <iostream>
#include <fstream>

static const char hello[] = {
#include <samples/hello_clip.h>
        };

static const char android[] = {
#include <samples/android_clip.h>
        };

using namespace std;
//    std::fstream out("hello.pcm", std::ios_base::out);
//    for (int i = 0; i < sizeof(hello); ++i) {
//        out << hello[i];
//    }
//    out.close();

int main(int argc, char* argv[]) {

//    if (argc != 2) {
//        cout << "use with PCM file\n";
//        return -1;
//    }
    string input(argv[1]);



    std::fstream in(input, std::ios_base::in);
    const vector<short> pcm1((short*)&android[0], (short*)&android[sizeof(android) - 1]);
//    const vector<short> pcm2((short*)hello, (short*)&hello[sizeof(hello) - 1]);
    vector<char> pcm;
    char temp;
    while (!in.fail()) {
        in >> temp;
        pcm.push_back(temp);
    }
//    const vector<short> pcm1(pcm.begin(), pcm.end());

    cout << "m1" << endl;
    OpenSL::Audio a;
    a.add(0,pcm1);
    a.play();
    cout << "m2" << endl;
    while (1) {
    }
    return 0;
}
