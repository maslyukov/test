/*
 * CRuntime.hpp
 *
 *  Created on: Feb 10, 2014
 *      Author: virtual-pc
 */

#ifndef CRUNTIME_HPP_
#define CRUNTIME_HPP_

#include <exception/private/CBasic.h>

namespace Exception {
using namespace std;


class CRuntime : public CBasic {

public:
    /**
     * Instead of first three arguments just use FFL_MACRO
     */
    CRuntime(const string& func, const string& file, const int& line,
            const string& message = "") _GLIBCXX_USE_NOEXCEPT {
        fillInforamtion(__FUNCTION__, func, file, line, message);
    }

    CRuntime() _GLIBCXX_USE_NOEXCEPT {
        fillInforamtion(__FUNCTION__, "", "", -1, "");
    }

    CRuntime(const string& message) _GLIBCXX_USE_NOEXCEPT {
        fillInforamtion(__FUNCTION__, "", "", -1, message);
    }

    virtual ~CRuntime()_GLIBCXX_USE_NOEXCEPT {
    }
};

}


#endif /* CRUNTIME_HPP_ */
