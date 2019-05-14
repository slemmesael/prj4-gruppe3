#ifndef OSAPI_THREADFUNCTOR_HPP
#define OSAPI_THREADFUNCTOR_HPP


#if defined(OS_WINX86)
#elif defined(OS_LINUX)
#include "osapi/linux/ThreadFunctor.hpp"
#elif defined(OS_FREERTOS)
#else
#error "No known OS defined"
#endif



#endif
