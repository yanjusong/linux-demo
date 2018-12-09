#ifndef DEFINES_H_20181209
#define DEFINES_H_20181209

#include <unistd.h>

#ifndef _POSIX_THREAD_PROCESS_SHARED
#error "This platform does not support process shared mutex"
#endif

#define MAP_FILE_NAME "./test.out"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#endif // !DEFINES_H_20181209
