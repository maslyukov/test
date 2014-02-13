#pragma once

// Simple wrapper around Android's logging interface that also allows other
// implementations, and also some misc utilities.


// Disable annoying warnings in VS
#ifdef _MSC_VER
#pragma warning (disable:4996)   //strcpy may be dangerous
#endif


#if defined(ANDROID)

#include <android/log.h>

// Must only be used for logging
#ifndef APP_NAME
#define APP_NAME "NativeApp"
#endif

#define ILOG(...)    __android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__);
#define WLOG(...)    __android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__);
#define ELOG(...)    __android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__);
#define FLOG(...)   { __android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__); Crash(); }

#define MessageBox(a, b, c, d) __android_log_print(ANDROID_LOG_INFO, APP_NAME, "%s %s", (b), (c));

#elif defined(__SYMBIAN32__)
#include <QDebug>
#define ILOG(...) { qDebug(__VA_ARGS__);}
#define WLOG(...) { qDebug(__VA_ARGS__);}
#define ELOG(...) { qDebug(__VA_ARGS__);}
#define FLOG(...) { qDebug(__VA_ARGS__); Crash();}

#else

#ifdef _WIN32

void __ods__(const char *p);

#define ILOG(...) {char temp[512]; char *p = temp; p += sprintf(p, "I: %s:%i: ", __FILE__, __LINE__); p += sprintf(p, "I: " __VA_ARGS__); p += sprintf(p, "\n"); __ods__(temp);}
#define WLOG(...) {char temp[512]; char *p = temp; p += sprintf(p, "W: %s:%i: ", __FILE__, __LINE__); p += sprintf(p, "W: " __VA_ARGS__); p += sprintf(p, "\n"); __ods__(temp);}
#define ELOG(...) {char temp[512]; char *p = temp; p += sprintf(p, "E: %s:%i: ", __FILE__, __LINE__); p += sprintf(p, "E: " __VA_ARGS__); p += sprintf(p, "\n"); __ods__(temp);}
#define FLOG(...) {char temp[512]; char *p = temp; p += sprintf(p, "F: %s:%i: ", __FILE__, __LINE__); p += sprintf(p, "F: " __VA_ARGS__); p += sprintf(p, "\n"); __ods__(temp); Crash();}

// TODO: Win32 version using OutputDebugString
#else
#endif
#endif


#undef FFL_MACRO
#define FFL_MACRO  __FUNCTION__, __FILE__, __LINE__

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 4
#endif

#if (DEBUG_LEVEL > 0)
#ifdef DEBUG_SRC_INFO
#define LOGE(...)   { \
                        printf("[ERR]: %s(%s:%i): ", FFL_MACRO); \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                    }
#else
#define LOGE(...)   { \
                        printf("[ERR]: "); \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                    }

#endif
#else
#define LOGE(...)
#endif
#if (DEBUG_LEVEL > 1)
#ifdef DEBUG_SRC_INFO
#define LOGW(...)   { \
                        printf("[WRN]: %s(%s:%i): ", FFL_MACRO); \
                        printf(__VA_ARGS__);\
                        printf("\n");\
                    }
#else
#define LOGW(...)   { \
                        printf("[WRN]: "); \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                    }

#endif
#else
#define LOGW(...)
#endif
#if (DEBUG_LEVEL > 2)
#ifdef DEBUG_SRC_INFO
#define LOGI(...)   { \
                        printf("[INF]: %s(%s:%i): ", FFL_MACRO); \
                        printf(__VA_ARGS__); \
                        printf("\n");\
                    }
#else
#define LOGI(...)   { \
                        printf("[INF]: "); \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                    }

#endif

#else
#define LOGI(...)
#endif
#if (DEBUG_LEVEL > 3)
#ifdef DEBUG_SRC_INFO
#define LOGD(...)   { \
                        printf("[DBG]: %s(%s:%i): ", FFL_MACRO); \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                    }
#else
#define LOGD(...)   { \
                        printf("[DBG]: "); \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                    }

#endif
#else
#define LOGD(...)
#endif







//#include <iostream>
//#include <sstream>
//namespace hh {
//template <typename T>
//void  foo( std::string type,
//         std::string func,
//         std::string file,
//         int line,
//         std::string str) {
//    std::stringstream stream;
//    stream << "Exception " << type << " at " << func
//            << "(" << file << ":" << line << "): " << str;
//    throw T(stream.str());
//}
//#define THROW(type, message) foo<type>(#type, __func__ ,__FILE__ ,__LINE__, message)
//} //hh

//
//#undef CHECK
//#define CHECK(a) {if (!(a)) {FLOG("CHECK failed");}}
//#define CHECK_P(a, ...) {if (!(a)) {FLOG("CHECK failed: " __VA_ARGS__);}}
//#define CHECK_EQ(a, b) CHECK((a) == (b));
//#define CHECK_NE(a, b) CHECK((a) != (b));
//#define CHECK_GT(a, b) CHECK((a) > (b));
//#define CHECK_GE(a, b) CHECK((a) >= (b));
//#define CHECK_LT(a, b) CHECK((a) < (b));
//#define CHECK_LE(a, b) CHECK((a) <= (b));
