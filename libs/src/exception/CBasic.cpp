/*
 * CBasic.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: virtual-pc
 */

#include <exception/exception.h>
#include <sstream>

void Exception::CBasic::fillInforamtion(const string& type, const string& func,
        const string& file, const int& line, const string& str) {
    std::stringstream stream;
    stream << "Exception::" << type;
    if (line > 0 && !func.empty()) {
        stream << " at " << func << "(" << file << ":" << line << ")";
    }
    if (str.size())
        stream << ": "<< str;
    message.append(stream.str());
}

const char* Exception::CBasic::what() const _GLIBCXX_USE_NOEXCEPT  {
    return message.c_str();
}

Exception::CBasic::CBasic() _GLIBCXX_USE_NOEXCEPT {
};

Exception::CBasic::~CBasic() _GLIBCXX_USE_NOEXCEPT {
}
