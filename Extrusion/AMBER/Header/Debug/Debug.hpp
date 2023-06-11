#ifndef __ENGINE_DEBUG__
#define __ENGINE_DEBUG__

#include "Initializer.hpp"
#include "termcolor/termcolor.hpp"
#include <iostream>
#include <cstdarg>

#ifdef NDEBUG
#if defined(_WIN32) || defined(_WIN64)
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#elif defined(__APPLE__)

#elif defined(__unix__) || defined(__unix)

#else
#error plate-forme non prise en charge
#endif
#endif


namespace Ge
{
    class Debug
    {
        public:            
            static void Log(const char *format, ...);
            static void Error(const char *format, ...);
            static void Warn(const char *format, ...);
            static void Info(const char *format, ...);
            static void VLayer(const char *format, ...);
            static void INITSUCCESS(const char * format);
            static void INITFAILED(const char * format);
            static void RELEASESUCCESS(const char * format);
    };
}

#endif //__ENGINE_DEBUG__

