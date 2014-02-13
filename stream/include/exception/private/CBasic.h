/*
 * CBase.hpp
 *
 *  Created on: Feb 10, 2014
 *      Author: virtual-pc
 */

#ifndef CBASE_HPP_
#define CBASE_HPP_

#include <exception>
#include <iostream>

#define FFL_MACRO  __FUNCTION__, __FILE__, __LINE__

namespace Exception {
using namespace std;

class CBasic : public exception {
    string message;
protected:
    void fillInforamtion(const string& type, const string& func,
            const string& file, const int& line, const string& str) ;
    CBasic() _GLIBCXX_USE_NOEXCEPT;
    virtual ~CBasic() _GLIBCXX_USE_NOEXCEPT;
public:
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT;
};
}



#endif /* CBASE_HPP_ */
