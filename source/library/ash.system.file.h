#pragma once


#ifdef __WIN32__
#include "ash.system.windows.file.h"
#else
#ifdef __linux__
#include "ash.system.linux.file.h"
#endif
#endif
