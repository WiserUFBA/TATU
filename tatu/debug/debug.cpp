#include "debug.h"

#ifdef virtualDev
Debug debug;
#endif

#ifdef AVR_GCC
Debug debug(&Serial);
#endif
